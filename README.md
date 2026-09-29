# DevHub Workspace

> **O painel central de controle, descoberta e orquestração de projetos locais para desenvolvedores de software, engenheiros de sistemas e profissionais multi-stack.**

Construído de forma **100% nativa em C++20**, o **DevHub** organiza repositórios em **C++, Rust, Python, Go, TypeScript/Node.js e C#** com velocidade instantânea, persistência relacional SQLite local e interface inspirada no design system da **Uber** (Black-and-White Duet).

Este repositório segue estritamente a metodologia **HOUS3** de engenharia de produto e Clean Architecture:

$$\text{Processo} \longrightarrow \text{Persona} \longrightarrow \text{Funcionalidade} \longrightarrow \text{Requisito Funcional (RF)} \longrightarrow \text{Task Técnica} \longrightarrow \text{Entrega}$$

---

## 📚 Documentação de Produto (Metodologia HOUS3)

Todos os artefatos de produto e engenharia estão organizados na pasta [`docs/`](file:///e:/dds/docs/):

1. **[Personas & Hipóteses Iniciais](file:///e:/dds/docs/01_PERSONAS_E_HIPOTESES.md)**: Mapeamento de Personas (Desenvolvedor Multi-Stack Poliglota, Engenheiro de Sistemas/Toolchains e Tech Lead), separando certezas de hipóteses com marcações `[PENDENTE]`.
2. **[Guia de Descoberta & Validação](file:///e:/dds/docs/02_GUIA_DESCOBERTA_VALIDACAO.md)**: Roteiro prático de entrevistas qualitativas (The Mom Test) para validação de hipóteses de risco (Build, Git e Templates).
3. **[Catálogo de Funcionalidades HOUS3](file:///e:/dds/docs/03_CATALOGO_FUNCIONALIDADES_HOUS3.md)**: Especificação completa de 15 funcionalidades no padrão do Track da HOUS3 (User Story, Contexto, Problema, Resultado Esperado, Escopo, BP e 59 RFs verificáveis).
4. **[Matriz de Rastreamento & Plano de Release](file:///e:/dds/docs/04_MATRIZ_RASTREAMENTO_E_RELEASE_PLAN.md)**: Detalhamento da Release MVP (39 BP) e do Backlog (18 BP), quebrando cada RF em tasks de desenvolvimento e critérios de aceite de QA.
5. **[Arquitetura de Software & Design Técnico](file:///e:/dds/docs/05_ARQUITETURA_SISTEMA_DEVHUB.md)**: Decisões arquiteturais em Clean Architecture, schema do banco relacional SQLite local e contratos de interfaces em C++ moderno.
6. **[Design System Uber — Black-and-White Duet](file:///e:/dds/docs/06_DESIGN_SYSTEM_UBER.md)**: Diretrizes do design system da Uber.
7. **[Design System CamaraUX](file:///e:/dds/docs/06_DESIGN_SYSTEM_CAMARAUX.md)**: Especificação completa de tokens semânticos (Slate), contraste acessível WCAG AA, escala de espaçamento base 8px, empty states orientativos, feedback não-bloqueante (Toast) e tratamento seguro de ações destrutivas.

---

## 🖥️ Aplicativo Desktop Nativo em C++ (Windows)

O **DevHub** é desenvolvido em **C++20 nativo** com **Qt 6 Widgets**, **CMake**, **Ninja** e **SQLite**, aplicando os padrões de UI e design tokens do **Design System CamaraUX** (superfícies em Slate `#f8fafc`/`#ffffff`, contraste de texto superior a 15:1 WCAG AA, badges semânticos de linguagem, pílulas de ação, feedback não-intrusivo via Toast e estados vazios com recuperação direta):

- **Arquitetura 100% C++:**
  - `src/domain/Project.hpp`: Entidade de domínio pura com metadados, stack/linguagem, tags, sistema de build e flags de favorito/órfão.
  - `src/infrastructure/ProjectInspector.cpp`: Inspeciona o disco em tempo real para múltiplos ecossistemas (`CMakeLists.txt`, `Cargo.toml`, `pyproject.toml`, `package.json`, `go.mod`, `*.csproj`).
  - `src/infrastructure/ProcessLauncher.cpp`: Lançador real de processos (`code`, `devenv`, `powershell.exe` com `Set-Location`, `explorer.exe`).
  - `src/infrastructure/SqliteProjectRepository.cpp`: Persistência local em SQLite (`%APPDATA%\DevHub\devhub.db`) com migração automática e seed multi-stack.
  - `src/ui/MainWindow.cpp`: Interface desktop nativa Uber: canvas branco `#ffffff`, botões em pílula preta `#000000`, chips em cinza suave `#efefef`, tabela com container de 16px e gaveta de Side Peek.

### 🚀 Como Compilar e Executar o App em C++

**Pré-requisitos:**
- Compilador C++20 (`g++` / GCC 16+ ou MSVC)
- CMake 3.20+ e Ninja
- Qt 6 (QtCore, QtGui, QtWidgets, QtSql)

**Compilação:**
```powershell
# 1. Configurar o projeto com CMake e Ninja
cmake -G Ninja -S . -B build

# 2. Compilar o executável nativo
cmake --build build
```

**Execução:**
```powershell
# Execução direta com runtime configurado:
.\run.ps1

# Ou diretamente pelo executável:
.\build\DevHubCpp.exe
```

### 📦 Como Instalar e Distribuir em Outros Computadores

Em outros computadores **não é necessário ter C++, CMake, Qt6 ou MSYS2 instalados**. Criamos um script que empacota automaticamente o binário junto a todas as DLLs e plugins necessários:

1. **Gerar o pacote de distribuição portátil e instalador:**
   ```powershell
   .\package.ps1
   ```
   Isso cria a pasta autônoma em `dist\DevHub` e o arquivo ZIP `downloads\DevHub-v1.1.0-win-x64.zip`.

2. **Como usar no outro computador:**
   - **Opção Portátil:** Extraia o `.zip` em qualquer pasta (ou pendrive) e dê 2 cliques em `DevHub.exe`.
   - **Opção Instalador:** Dê 2 cliques em `instalar.bat` para copiar automaticamente para `%LOCALAPPDATA%\Programs\DevHub` e criar atalhos na **Área de Trabalho** e no **Menu Iniciar**.


| ID | Funcionalidade (User Story) | BP | Destino | Status |
|:---:|---|:---:|:---:|:---:|
| **F01** | Como Desenvolvedor C++, quero cadastrar meus projetos em uma biblioteca central, para encontrá-los sem procurar manualmente pelas pastas | 5 BP | Release MVP | Concluído no C++ |
| **F02** | Como Desenvolvedor C++, quero identificar automaticamente as características de um projeto, para não precisar cadastrar suas configurações manualmente | 5 BP | Release MVP | Concluído no C++ |
| **F03** | Como Desenvolvedor C++, quero encontrar e organizar meus projetos por busca, categoria e favorito, para acessar rapidamente o projeto em que preciso trabalhar | 5 BP | Release MVP | Concluído no C++ |
| **F04** | Como Desenvolvedor C++, quero abrir um projeto e suas ferramentas de desenvolvimento a partir do DevHub, para iniciar o trabalho sem navegar manualmente pelo computador | 5 BP | Release MVP | Concluído no C++ |
| **F05** | Como Desenvolvedor C++, quero visualizar as informações técnicas e a estrutura de um projeto, para entender rapidamente como ele está configurado | 3 BP | Release MVP | Concluído no C++ |
| **F06** | Como Desenvolvedor C++, quero executar o build de um projeto pelo DevHub, para iniciar a compilação sem configurar manualmente o comando a cada vez | 3 BP | Backlog | Hipótese em Validação |
| **F07** | Como Desenvolvedor C++, quero visualizar informações básicas do Git de um projeto, para saber seu estado atual sem abrir outra ferramenta | 3 BP | Backlog | Hipótese em Validação |
| **F08** | Como Desenvolvedor C++, quero criar projetos a partir de templates, para iniciar novos projetos com uma estrutura padronizada | 3 BP | Backlog | Hipótese em Validação |
| **F09** | Como Desenvolvedor C++, quero configurar as ferramentas utilizadas pelo DevHub, para que as ações sejam executadas no meu ambiente de desenvolvimento | 3 BP | Release MVP | Concluído no C++ |
| **F10** | Como Desenvolvedor C++, quero fazer uma varredura automática em uma pasta raiz, para cadastrar múltiplos projetos de uma só vez | 5 BP | Release MVP (v1.1) | Pronto para Tasks |
| **F11** | Como Desenvolvedor C++, quero visualizar o consumo de disco dos artefatos de build e limpá-los com segurança, para liberar espaço sem danificar os códigos-fonte | 5 BP | Release MVP (v1.1) | Pronto para Tasks |
| **F12** | Como Desenvolvedor C++, quero diagnosticar a compatibilidade do ambiente e compiladores instalados (Toolchain Doctor), para saber se posso compilar o projeto antes de tentar o build | 3 BP | Backlog | Hipótese em Validação |
| **F13** | Como Desenvolvedor C++, quero visualizar as dependências de bibliotecas externas (vcpkg / Conan) de cada projeto, para entender quais pacotes são necessários antes de configurar o ambiente | 3 BP | Backlog | Hipótese em Validação |
| **F14** | Como Desenvolvedor C++, quero alternar rapidamente entre os projetos recentes através de um seletor rápido (Quick Switcher com `/` ou `Ctrl+K`), para trocar de contexto sem interromper meu fluxo | 3 BP | Release MVP (v1.1) | Pronto para Tasks |
| **F15** | Como Desenvolvedor C++, quero visualizar um panorama do status Git dos meus projetos em lote (branches e alterações pendentes), para não esquecer códigos não comitados no fim do dia | 3 BP | Backlog | Hipótese em Validação |

---

## 🛠️ Princípios Técnicos Fundamentais

- **Zero poluição de repositórios:** O DevHub não cria arquivos proprietários dentro das pastas dos seus projetos.
- **Privacidade total e 100% local:** Todos os metadados são salvos em SQLite local no computador do desenvolvedor.
- **Desacoplamento absoluto:** O Core de domínio e inspeção não depende de frameworks visuais ou bibliotecas de UI.
