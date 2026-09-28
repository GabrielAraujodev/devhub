# Matriz de Rastreamento & Plano de Release — DevHub C++
**Metodologia HOUS3**  
**Cadeia Completa:** *Processo → Persona → Funcionalidade → Requisito Funcional → Task Técnica → Entrega Verificável*

---

## 1. Visão Geral da Release MVP

A Release MVP é orientada a resolver a dor de **Localização, Reconhecimento e Acionamento Instantâneo** dos projetos locais de C++, sem inflar o escopo com hipóteses de alto risco que ainda demandam validação qualitativa.

### Resumo por Business Points (BP)
- **Funcionalidades no MVP Base (v1.0):** `F01 (5 BP)`, `F02 (5 BP)`, `F03 (5 BP)`, `F04 (5 BP)`, `F05 (3 BP)`, `F09 (3 BP)` = **26 BP**
- **Funcionalidades no MVP Expansão (v1.1):** `F10 (5 BP)`, `F11 (5 BP)`, `F14 (3 BP)` = **13 BP** (Total MVP consolidado: **39 BP**)
- **Funcionalidades retidas no Backlog:** `F06 (3 BP)`, `F07 (3 BP)`, `F08 (3 BP)`, `F12 (3 BP)`, `F13 (3 BP)`, `F15 (3 BP)` = **Total: 18 BP**

---

## 2. Matriz de Rastreabilidade Fim a Fim (MVP)

### F01 — Cadastro e Gestão Centralizada de Projetos (5 BP)
* **Processo:** Início da jornada de desenvolvimento e gerenciamento do acervo local de código.
* **Persona:** Desenvolvedor C++ com múltiplos projetos.

| Requisito Funcional (RF) | Tasks Técnicas de Engenharia | Critério de Aceite da Entrega (QA) |
|---|---|---|
| **RF-001** (Adicionar diretório local) | • Modelar entidade de domínio `Project`<br>• Implementar `ProjectRepository` com SQLite<br>• Integrar diálogo nativo de seleção de diretórios (`QFileDialog` / nativo)<br>• Criar comando de aplicação `AddProjectCommand` | Usuário seleciona qualquer pasta válida e ela aparece imediatamente na lista com nome da pasta e caminho. |
| **RF-002** (Impedir cadastro duplicado) | • Normalizar caminhos de arquivo (resolver caminhos relativos e links simbólicos)<br>• Adicionar restrição `UNIQUE(normalized_path)` no banco SQLite<br>• Exibir notificação amigável de duplicidade | Ao tentar adicionar uma pasta já cadastrada, o sistema não duplica e destaca o projeto existente. |
| **RF-003** (Remover sem apagar arquivos) | • Implementar `RemoveProjectCommand`<br>• Criar diálogo de confirmação com texto explícito: "Seus arquivos no disco continuarão intactos"<br>• Excluir apenas o registro no banco de dados local | Ao confirmar a remoção, o projeto some do DevHub, mas o diretório e arquivos continuam íntegros no disco. |
| **RF-004** (Sinalizar projetos órfãos) | • Implementar serviço de verificação de sanidade do sistema de arquivos (`PathValidatorService`)<br>• Checar existência do diretório ao carregar o dashboard<br>• Renderizar badge de alerta "Caminho não encontrado" | Se a pasta do projeto for renomeada ou apagada fora do app, o card exibe ícone de alerta e desabilita ações de abrir. |
| **RF-005** (Atualizar caminho do projeto) | • Criar ação "Localizar pasta" para projetos com caminho quebrado<br>• Atualizar caminho normalizado no banco | Ao reassociar o novo caminho da pasta, o alerta desaparece e as ações voltam a funcionar. |

---

### F02 — Detecção Automática de Características (5 BP)
* **Processo:** Onboarding instantâneo de projetos recém-adicionados.
* **Persona:** Desenvolvedor C++ com múltiplos projetos.

| Requisito Funcional (RF) | Tasks Técnicas de Engenharia | Critério de Aceite da Entrega (QA) |
|---|---|---|
| **RF-006** (Detectar arquivos de build) | • Implementar `ProjectInspectorService`<br>• Criar scanners para detecção de `CMakeLists.txt`, `*.sln`, `*.vcxproj`, `build.ninja`<br>• Persistir tags de build inferidas na entidade | Pasta contendo `CMakeLists.txt` ganha badge "CMake"; pasta contendo `.sln` ganha badge "Visual Studio". |
| **RF-007** (Identificar padrão C++) | • Criar regex parser seguro para diretivas de padrão em `CMakeLists.txt` (`CMAKE_CXX_STANDARD (14\|17\|20\|23)`)<br>• Mapear padrão detectado para campo `cxx_standard` | Projeto com `set(CMAKE_CXX_STANDARD 20)` exibe chip "C++20" automaticamente. |
| **RF-008** (Detectar Qt) | • Detectar menções a `find_package(Qt5...)`, `find_package(Qt6...)` ou arquivos `.ui` / `.qml`<br>• Adicionar tag "Qt" | Projeto com dependência de Qt exibe chip com logo/tag "Qt". |
| **RF-009** (Editar características) | • Criar modal de edição de propriedades técnicas do projeto<br>• Permitir sobrescrever badges automáticas | Usuário consegue adicionar ou remover manualmente tags como "CMake" ou "C++23". |

---

### F03 — Busca, Categorias e Favoritos (5 BP)
* **Processo:** Localização e alternância ágil de projetos no dia a dia.
* **Persona:** Desenvolvedor C++ com múltiplos projetos.

| Requisito Funcional (RF) | Tasks Técnicas de Engenharia | Critério de Aceite da Entrega (QA) |
|---|---|---|
| **RF-010** (Busca em tempo real) | • Implementar filtro reativo no modelo da lista (filtro por nome e caminho)<br>• Debounce de 150ms na digitação para fluidez | Ao digitar "engine", apenas projetos cujo nome ou caminho contenha "engine" permanecem visíveis. |
| **RF-011** (Marcar favoritos) | • Campo booleano `is_favorite` na entidade `Project`<br>• Toggle interativo com clique na estrela do card<br>• Ordenação priorizando favoritos | Clicar na estrela marca o projeto como favorito e o mantém fixado no topo ou acessível no filtro rápido. |
| **RF-012** (Gestão de categorias) | • Tabela `categories` e relação N:N `project_categories`<br>• Diálogo simples para criar/excluir categorias (ex: "Empresa", "Estudos") | Usuário cria a categoria "Trabalho" e ela passa a aparecer na barra lateral de filtros. |
| **RF-013** (Vincular projeto a categorias) | • Seletor múltiplo de categorias na edição/card do projeto | Projeto pode ser associado à categoria "Trabalho" e "Open Source". |
| **RF-014** (Filtro rápido de favoritos) | • Botão de filtro toggle "Apenas Favoritos" na barra superior | Ao ativar o filtro, apenas os projetos com estrela preenchida são exibidos. |

---

### F04 — Acionamento Direto de Ferramentas (5 BP)
* **Processo:** Transição da descoberta para o início efetivo da codificação.
* **Persona:** Desenvolvedor C++ com múltiplos projetos.

| Requisito Funcional (RF) | Tasks Técnicas de Engenharia | Critério de Aceite da Entrega (QA) |
|---|---|---|
| **RF-015** (Abrir na IDE) | • Implementar `ProcessLauncherService`<br>• Suportar lançamento desacoplado: `code <caminho>` e `devenv <sln_ou_pasta>`<br>• Lançar processos sem travar a thread de interface | Clicar em "Abrir IDE" inicializa o VS Code ou Visual Studio na pasta do projeto. |
| **RF-016** (Definir IDE padrão por projeto) | • Campo `preferred_ide` na entidade `Project`<br>• Menu dropdown para escolher entre VS Code, Visual Studio ou padrão global | Usuário pode definir que o Projeto A abre no Visual Studio e o Projeto B no VS Code. |
| **RF-017** (Abrir no terminal) | • Detectar terminal configurado e invocar processo passando `cwd = project_path`<br>• Suporte a `wt.exe`, `powershell.exe`, `cmd.exe` | Clicar no ícone de terminal abre a janela do PowerShell/Windows Terminal já dentro da pasta do projeto. |
| **RF-018** (Abrir no Explorer) | • Invocar o explorador nativo (`explorer.exe <caminho>` no Windows) | Clicar no ícone de pasta abre a janela do Windows Explorer exatamente na raiz do projeto. |
| **RF-019** (Feedback de IDE ausente) | • Validar existência do binário antes do lançamento<br>• Exibir toast com orientação de configuração caso falhe | Se a máquina não tiver VS Code instalado, o sistema avisa amigavelmente e sugere configurar o caminho em F09. |

---

### F05 — Visualização de Metadados Técnicos (3 BP)
* **Processo:** Compreensão e contextualização rápida de projetos existentes.
* **Persona:** Desenvolvedor C++ com configurações diferentes.

| Requisito Funcional (RF) | Tasks Técnicas de Engenharia | Critério de Aceite da Entrega (QA) |
|---|---|---|
| **RF-020** (Exibir metadados técnicos) | • Desenvolver tela/painel lateral de detalhes do projeto<br>• Exibir tabela formatada com caminho, build system, standard e datas | Painel de detalhes exibe com clareza as propriedades inferidas e o caminho completo. |
| **RF-021** (Data de último acesso) | • Atualizar coluna `last_accessed_at` no banco sempre que uma ação de F04 for disparada | O card e os detalhes mostram "Acessado há 2 horas" ou data legível. |
| **RF-022** (Anotações do desenvolvedor) | • Campo de texto `developer_notes` na entidade `Project`<br>• Área de texto com autosave ou botão salvar | Desenvolvedor escreve dicas de build e as anotações permanecem salvas para consultas futuras. |

---

### F09 — Configurações de Ambiente Local (3 BP)
* **Processo:** Setup inicial da ferramenta e adequação ao ambiente da máquina.
* **Persona:** Desenvolvedor C++ com configurações diferentes.

| Requisito Funcional (RF) | Tasks Técnicas de Engenharia | Critério de Aceite da Entrega (QA) |
|---|---|---|
| **RF-032** (Detectar IDEs instaladas) | • Implementar `EnvironmentDiscoveryService`<br>• Varrer registro do Windows / `PATH` em busca de `code.cmd`, `devenv.exe`, `clion64.exe` | Tela de configurações mostra "VS Code detectado no PATH" e "Visual Studio detectado". |
| **RF-033** (Caminhos manuais de executáveis) | • Permitir override manual de caminho de executáveis caso não estejam no PATH | Desenvolvedor pode apontar manualmente para um VS Code portátil em `D:\tools\vscode\bin\code.cmd`. |
| **RF-034** (Escolha de terminal padrão) | • Dropdown com opções: Windows Terminal (`wt`), PowerShell, CMD | Usuário escolhe Windows Terminal e todas as ações de terminal abrem via `wt`. |
| **RF-035** (Persistência local) | • Tabela `settings` ou arquivo de configuração `settings.json` local | As configurações de ambiente persistem entre reinicializações do app. |

---

### F10 — Varredura Automática e Importação em Lote (5 BP — MVP v1.1)
* **Processo:** Onboarding massivo e descoberta de repositórios em diretórios de desenvolvimento.
* **Persona:** Desenvolvedor C++ com múltiplos projetos.

| Requisito Funcional (RF) | Tasks Técnicas de Engenharia | Critério de Aceite da Entrega (QA) |
|---|---|---|
| **RF-036** (Selecionar pasta raiz) | • Adicionar ação e diálogo de seleção de diretório base para varredura recursiva | O usuário seleciona `C:\dev` e o sistema inicia a varredura sem congelar a interface. |
| **RF-037** (Filtro de exclusão segura) | • Criar lista negra de pastas a ignorar (`build`, `.git`, `.vs`, `out`, `CMakeFiles`, `node_modules`)<br>• Limitar busca a 2 níveis de profundidade | Pastas intermediárias de build ou controle de versão não são cadastradas como projetos. |
| **RF-038** (Listar candidatos encontrados) | • Exibir diálogo modal xAI com lista de repositórios identificados e sistema de build inferido | Apresenta lista organizada destacando quais já constam no banco e quais são novos. |
| **RF-039** (Seleção granular com checkboxes) | • Implementar checkboxes por linha e botão "Selecionar Todos / Nenhum" | Usuário pode desmarcar projetos indesejados antes de prosseguir. |
| **RF-040** (Persistência atômica em lote) | • Implementar `saveBatch(QList<Project>)` com transação única no SQLite | Todos os projetos selecionados são persistidos atomicamente em menos de 1 segundo. |

---

### F11 — Limpeza Segura de Artefatos de Build (5 BP — MVP v1.1)
* **Processo:** Manutenção de armazenamento local e liberação de espaço em disco.
* **Persona:** Desenvolvedor C++ com múltiplos projetos.

| Requisito Funcional (RF) | Tasks Técnicas de Engenharia | Critério de Aceite da Entrega (QA) |
|---|---|---|
| **RF-041** (Calcular tamanho de build) | • Implementar scanner assíncrono que soma bytes de pastas `build`, `out`, `bin`, `.vs`<br>• Rodar em thread secundária | O sistema calcula o tamanho ocupado sem travar a interface da tabela. |
| **RF-042** (Exibir peso no Side Peek) | • Formatar bytes para `MB` e `GB` em `Geist Mono` no painel lateral de detalhes | O desenvolvedor visualiza de relance: "Artefatos de Build: 8.4 GB". |
| **RF-043** (Confirmação explícita de deleção) | • Diálogo modal exibindo caminhos absolutos a serem apagados com botão de confirmação | O usuário tem certeza absoluta de quais pastas serão removidas antes da exclusão. |
| **RF-044** (Validação contra deleção indevida) | • Checar presença de arquivos de compilação conhecidos (`CMakeCache.txt`, `build.ninja`, `*.obj`) | O sistema se recusa a apagar diretórios que contenham código-fonte manual (`.cpp`, `.h`). |
| **RF-045** (Atualização imediata de espaço) | • Recalcular e atualizar contadores visuais após a remoção | O contador no painel zera imediatamente e notifica a quantidade de bytes liberados. |

---

### F14 — Quick Switcher Global com Paleta de Teclado (3 BP — MVP v1.1)
* **Processo:** Alternância contínua entre projetos ativos ao longo do dia.
* **Persona:** Desenvolvedor C++ com múltiplos projetos.

| Requisito Funcional (RF) | Tasks Técnicas de Engenharia | Critério de Aceite da Entrega (QA) |
|---|---|---|
| **RF-052** (Atalho `Ctrl+K` / `Ctrl+P`) | • Registrar `QShortcut` global na janela principal para abrir paleta modal flutuante | Ao teclar `Ctrl+K`, uma janela de busca centralizada aparece instantaneamente. |
| **RF-053** (Ordenar por mais recentes) | • Carregar os 5 projetos com `last_accessed` mais recente como sugestão inicial | A paleta já abre mostrando os últimos projetos trabalhados pelo usuário. |
| **RF-054** (Filtro instantâneo por digitação) | • Campo de entrada com pesquisa instantânea em memória por nome, caminho ou tags | Digitar "core" refina a lista imediatamente para repositórios com "core". |
| **RF-055** (Disparo via teclado `Enter`) | • Capturar `Enter` para abrir na IDE padrão e fechar a paleta<br>• Capturar `Ctrl+Enter` para abrir no Terminal | O desenvolvedor troca de projeto e abre sua IDE sem tirar as mãos do teclado. |

---

## 3. Gestão das Funcionalidades Retidas no Backlog

As funcionalidades `F06`, `F07`, `F08`, `F12`, `F13` e `F15` permanecem no **Backlog** aguardando a rodada de validação com desenvolvedores reais:

| ID | Funcionalidade | BP | Hipótese a Validar / Condição de Desbloqueio |
|:---:|---|:---:|---|
| **F06** | Executar build pelo DevHub | 3 BP | Se desenvolvedores realmente desejam compilar fora de suas IDEs ou se preferem manter o build nas ferramentas dedicadas. |
| **F07** | Informações básicas do Git | 3 BP | Se a visualização de branch individual no card gera lentidão de I/O em discos magnéticos. |
| **F08** | Criar projetos a partir de templates | 3 BP | Se a criação de novos projetos é frequente o suficiente para justificar templates embarcados. |
| **F12** | Toolchain Doctor & Diagnóstico | 3 BP | Se a detecção de compiladores no PATH cobre adequadamente instalações customizadas (ex: Visual Studio via `vswhere.exe`). |
| **F13** | Inspetor de Pacotes vcpkg / Conan | 3 BP | Se a leitura puramente declarativa de `vcpkg.json`/`conanfile.txt` atende antes de suportar Git submodules. |
| **F15** | Radar Git Multirrepositório em lote | 3 BP | Se o impacto de subprocessos `git status` em 50+ repositórios simultâneos mantém o tempo de resposta aceitável. |

```text
[Descoberta e Entrevistas com Desenvolvedores]
         │
         ├── Se limpeza e escaneamento em lote tiverem adoção máxima ──► CONSOLIDAR F10 & F11 NA RELEASE
         ├── Se trocas de contexto forem frequentes pelo teclado ──────► ADOTAR F14 QUICK SWITCHER
         ├── Se ferramentas locais forem heterogêneas ─────────────────► PROMOVER F12 TOOLCHAIN DOCTOR
         └── Se controle multirrepositório for crítico ────────────────► PROMOVER F15 GIT RADAR
```

