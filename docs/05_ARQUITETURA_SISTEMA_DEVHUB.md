# Arquitetura de Software — DevHub C++
**Documento de Engenharia & Decisões Arquiteturais**

---

## 1. Princípios Norteadores

1. **Separação Rígida de Responsabilidades:** O núcleo de regras de negócio (entidades, detecção de toolchain, persistência) é 100% agnóstico de UI.
2. **Zero Invasão no Código do Usuário:** O DevHub apenas lê os arquivos descritores (`CMakeLists.txt`, `.sln`); ele nunca altera o código-fonte nem escreve metadados ocultos nas pastas dos projetos (nada de arquivos `.devhub` poluindo os repositórios do desenvolvedor).
3. **Resiliência e Desempenho Local:** Nenhuma chamada de rede necessária para o funcionamento básico. Operações de I/O em lote executadas de forma assíncrona para manter a UI em 60 FPS.
4. **Persistência Centralizada e Transparente:** Todos os metadados residem em um único banco SQLite no diretório de dados do usuário (`%APPDATA%/DevHub/devhub.db`).

---

## 2. Visão em Camadas (Clean Architecture)

```text
┌────────────────────────────────────────────────────────┐
│                   Apresentação (UI)                   │
│   (Qt Quick / QML ou Desktop Viewport, Widgets, Views) │
└──────────────────────────┬─────────────────────────────┘
                           │ Invoca Use Cases / ViewModels
┌──────────────────────────▼─────────────────────────────┐
│                 Aplicação (Use Cases)                  │
│  - AddProjectUseCase         - ScanProjectUseCase      │
│  - LaunchToolUseCase         - SearchProjectsUseCase   │
│  - ManageFavoritesUseCase    - ConfigureEnvUseCase     │
└──────────────────────────┬─────────────────────────────┘
                           │ Opera sobre entidades e interfaces
┌──────────────────────────▼─────────────────────────────┐
│                  Domínio (Domain Core)                 │
│  - Project (Entity)          - ProjectId, ProjectPath  │
│  - BuildSystem (ValueObject) - ToolchainInfo           │
│  - Category (Entity)         - IProjectRepository      │
└──────────────────────────┬─────────────────────────────┘
                           │ Implementado por
┌──────────────────────────▼─────────────────────────────┐
│                Infraestrutura (Adapters)               │
│  - SQLiteProjectRepository   - CMakeFileInspector      │
│  - WindowsProcessLauncher    - NativeFileDialogService │
│  - LocalEnvironmentDetector  - Win32PathNormalizer     │
└────────────────────────────────────────────────────────┘
```

---

## 3. Modelo de Dados Relacional (SQLite)

O schema do banco local `%APPDATA%/DevHub/devhub.db` é simples, robusto e normalizado:

```sql
-- Tabela de Projetos
CREATE TABLE IF NOT EXISTS projects (
    id TEXT PRIMARY KEY,                       -- UUID v4
    name TEXT NOT NULL,                        -- Nome amigável (default: nome da pasta)
    normalized_path TEXT NOT NULL UNIQUE,      -- Caminho absoluto normalizado
    build_system TEXT NOT NULL,                -- 'CMAKE', 'VISUAL_STUDIO', 'NINJA', 'CUSTOM', 'UNKNOWN'
    cxx_standard TEXT,                         -- '14', '17', '20', '23', etc.
    has_qt INTEGER NOT NULL DEFAULT 0,         -- 0 = falso, 1 = verdadeiro
    is_favorite INTEGER NOT NULL DEFAULT 0,    -- 0 = falso, 1 = verdadeiro
    preferred_ide TEXT,                        -- 'VSCODE', 'VISUAL_STUDIO', 'CLION', 'DEFAULT'
    developer_notes TEXT,                      -- Anotações livres do desenvolvedor
    created_at TEXT NOT NULL,                  -- ISO8601 UTC
    last_accessed_at TEXT                      -- ISO8601 UTC
);

-- Tabela de Categorias
CREATE TABLE IF NOT EXISTS categories (
    id TEXT PRIMARY KEY,                       -- UUID v4
    name TEXT NOT NULL UNIQUE,                 -- ex: 'Trabalho', 'Open Source', 'Estudos'
    color_hex TEXT                             -- ex: '#4A90E2'
);

-- Relação N:N Projeto <-> Categoria
CREATE TABLE IF NOT EXISTS project_categories (
    project_id TEXT NOT NULL REFERENCES projects(id) ON DELETE CASCADE,
    category_id TEXT NOT NULL REFERENCES categories(id) ON DELETE CASCADE,
    PRIMARY KEY (project_id, category_id)
);

-- Tabela de Configurações de Ambiente
CREATE TABLE IF NOT EXISTS environment_settings (
    key TEXT PRIMARY KEY,                      -- 'vscode_path', 'visual_studio_path', 'terminal_type'
    value TEXT NOT NULL
);

-- Índices de Desempenho
CREATE INDEX IF NOT EXISTS idx_projects_name ON projects(name);
CREATE INDEX IF NOT EXISTS idx_projects_favorite ON projects(is_favorite);
CREATE INDEX IF NOT EXISTS idx_projects_last_accessed ON projects(last_accessed_at);
```

---

## 4. Design de Interfaces e Contratos (C++ Moderno)

### 4.1 Entidade de Domínio: `Project`
```cpp
#pragma once
#include <string>
#include <vector>
#include <optional>
#include <chrono>

namespace devhub::domain {

enum class BuildSystem {
    Unknown,
    CMake,
    VisualStudio,
    Ninja,
    Custom
};

struct Project {
    std::string id;
    std::string name;
    std::string normalized_path;
    BuildSystem build_system{BuildSystem::Unknown};
    std::optional<std::string> cxx_standard;
    bool has_qt{false};
    bool is_favorite{false};
    std::optional<std::string> preferred_ide;
    std::string developer_notes;
    std::chrono::system_clock::time_point created_at;
    std::optional<std::chrono::system_clock::time_point> last_accessed_at;
};

} // namespace devhub::domain
```

### 4.2 Repositório Abstrato: `IProjectRepository`
```cpp
#pragma once
#include "Project.hpp"
#include <vector>
#include <optional>

namespace devhub::domain {

class IProjectRepository {
public:
    virtual ~IProjectRepository() = default;
    
    virtual void save(const Project& project) = 0;
    virtual void update(const Project& project) = 0;
    virtual void remove(const std::string& project_id) = 0;
    
    virtual std::optional<Project> find_by_id(const std::string& project_id) = 0;
    virtual std::optional<Project> find_by_path(const std::string& normalized_path) = 0;
    virtual std::vector<Project> find_all() = 0;
    virtual std::vector<Project> find_favorites() = 0;
    virtual std::vector<Project> search_by_query(const std::string& query) = 0;
};

} // namespace devhub::domain
```

### 4.3 Scanner de Características: `IProjectInspector`
```cpp
#pragma once
#include "Project.hpp"
#include <filesystem>

namespace devhub::domain {

struct InspectionResult {
    BuildSystem build_system{BuildSystem::Unknown};
    std::optional<std::string> cxx_standard;
    bool has_qt{false};
};

class IProjectInspector {
public:
    virtual ~IProjectInspector() = default;
    virtual InspectionResult inspect(const std::filesystem::path& project_path) = 0;
};

} // namespace devhub::domain
```

### 4.4 Lançador de Ferramentas: `IProcessLauncher`
```cpp
#pragma once
#include <string>
#include <filesystem>

namespace devhub::domain {

enum class ToolType {
    Ide,
    Terminal,
    Explorer
};

class IProcessLauncher {
public:
    virtual ~IProcessLauncher() = default;
    virtual bool launch_ide(const std::filesystem::path& project_path, const std::string& ide_identifier) = 0;
    virtual bool launch_terminal(const std::filesystem::path& project_path) = 0;
    virtual bool launch_explorer(const std::filesystem::path& project_path) = 0;
};

} // namespace devhub::domain
```

---

## 5. Estrutura de Diretórios do Código-Fonte

```text
e:\dds\
├── CMakeLists.txt              # Build raiz com C++20 / C++23
├── docs/                       # Documentação HOUS3 completa
│   ├── 01_PERSONAS_E_HIPOTESES.md
│   ├── 02_GUIA_DESCOBERTA_VALIDACAO.md
│   ├── 03_CATALOGO_FUNCIONALIDADES_HOUS3.md
│   ├── 04_MATRIZ_RASTREAMENTO_E_RELEASE_PLAN.md
│   └── 05_ARQUITETURA_SISTEMA_DEVHUB.md
├── src/
│   ├── domain/                 # Entidades e interfaces puras
│   │   ├── Project.hpp
│   │   ├── IProjectRepository.hpp
│   │   ├── IProjectInspector.hpp
│   │   └── IProcessLauncher.hpp
│   ├── application/            # Casos de uso
│   │   ├── AddProjectUseCase.hpp
│   │   ├── AddProjectUseCase.cpp
│   │   ├── LaunchToolUseCase.hpp
│   │   └── LaunchToolUseCase.cpp
│   ├── infrastructure/         # Implementações de SO e DB
│   │   ├── CMakeInspector.hpp
│   │   ├── CMakeInspector.cpp
│   │   ├── SQLiteProjectRepository.hpp
│   │   ├── SQLiteProjectRepository.cpp
│   │   ├── WindowsProcessLauncher.hpp
│   │   └── WindowsProcessLauncher.cpp
│   └── ui/                     # Interface de Usuário
│       └── ...
└── tests/                      # Testes unitários (Catch2 ou GoogleTest)
    ├── domain_tests.cpp
    ├── inspector_tests.cpp
    └── repository_tests.cpp
```
