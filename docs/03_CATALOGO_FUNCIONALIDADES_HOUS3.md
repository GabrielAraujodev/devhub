# Catálogo de Funcionalidades — DevHub Workspace
**Metodologia HOUS3**  
**Conexão:** *Processo → Persona → Funcionalidade → Requisito Funcional → Task → Entrega*

> **Nota de Escopo de Personas:** O DevHub atende desenvolvedores de software, engenheiros de sistemas e profissionais multi-stack/poliglotas que mantêm múltiplos repositórios locais (C++, Rust, Python, Go, TypeScript/Node.js, C#), mantendo inspeções aprofundadas para C++ e toolchains nativas.

---

## Índice das Funcionalidades

| ID | Nome da Funcionalidade (User Story) | Persona Principal | BP | Destino |
|:---:|---|---|:---:|:---:|
| **F01** | Como Desenvolvedor de Software, quero cadastrar meus projetos em uma biblioteca central, para encontrá-los sem procurar manualmente pelas pastas | Desenvolvedor Multi-Stack com múltiplos projetos | 5 | Release MVP |
| **F02** | Como Desenvolvedor de Software, quero identificar automaticamente a stack e características de um projeto, para não precisar cadastrar suas configurações manualmente | Desenvolvedor Multi-Stack com múltiplos projetos | 5 | Release MVP |
| **F03** | Como Desenvolvedor de Software, quero encontrar e organizar meus projetos por busca, stack, categoria e favorito, para acessar rapidamente o repositório em que preciso trabalhar | Desenvolvedor Multi-Stack com múltiplos projetos | 5 | Release MVP |
| **F04** | Como Desenvolvedor de Software, quero abrir um projeto e suas ferramentas de desenvolvimento a partir do DevHub, para iniciar o trabalho sem navegar manualmente pelo computador | Desenvolvedor Multi-Stack com múltiplos projetos | 5 | Release MVP |
| **F05** | Como Desenvolvedor de Software, quero visualizar as informações técnicas e a estrutura de um projeto, para entender rapidamente como ele está configurado | Desenvolvedor com configurações heterogêneas | 3 | Release MVP |
| **F06** | Como Desenvolvedor de Software, quero executar o build de um projeto pelo DevHub, para iniciar a compilação sem configurar manualmente o comando a cada vez | Desenvolvedor com configurações heterogêneas | 3 | Backlog |
| **F07** | Como Desenvolvedor de Software, quero visualizar informações básicas do Git de um projeto, para saber seu estado atual sem abrir outra ferramenta | Desenvolvedor Multi-Stack com múltiplos projetos | 3 | Backlog |
| **F08** | Como Desenvolvedor de Software, quero criar projetos a partir de templates, para iniciar novos repositórios com uma estrutura padronizada | Desenvolvedor com configurações heterogêneas | 3 | Backlog |
| **F09** | Como Desenvolvedor de Software, quero configurar as ferramentas utilizadas pelo DevHub, para que as ações sejam executadas no meu ambiente de desenvolvimento | Desenvolvedor com configurações heterogêneas | 3 | Release MVP |
| **F10** | Como Desenvolvedor de Software, quero fazer uma varredura automática em uma pasta raiz, para cadastrar múltiplos projetos de uma só vez | Desenvolvedor Multi-Stack com múltiplos projetos | 5 | Release MVP (v1.1) |
| **F11** | Como Desenvolvedor de Software, quero visualizar o consumo de disco dos artefatos de build e limpá-los com segurança, para liberar espaço sem danificar os códigos-fonte | Desenvolvedor Multi-Stack com múltiplos projetos | 5 | Release MVP (v1.1) |
| **F12** | Como Desenvolvedor de Software, quero diagnosticar a compatibilidade do ambiente e compiladores instalados (Toolchain Doctor), para saber se posso compilar o projeto antes de tentar o build | Desenvolvedor com configurações heterogêneas | 3 | Backlog |
| **F13** | Como Desenvolvedor de Software, quero visualizar as dependências de bibliotecas externas (vcpkg, Cargo, Pip, npm) de cada projeto, para entender quais pacotes são necessários | Desenvolvedor com configurações heterogêneas | 3 | Backlog |
| **F14** | Como Desenvolvedor de Software, quero alternar rapidamente entre os projetos recentes através de um seletor rápido (Quick Switcher com `/` ou `Ctrl+K`), para trocar de contexto sem interromper meu fluxo | Desenvolvedor Multi-Stack com múltiplos projetos | 3 | Release MVP (v1.1) |
| **F15** | Como Desenvolvedor de Software, quero visualizar um panorama do status Git dos meus projetos em lote (branches e alterações pendentes), para não esquecer códigos não comitados no fim do dia | Desenvolvedor Multi-Stack com múltiplos projetos | 3 | Backlog |

---

## Detalhamento das Funcionalidades

---

### F01 — Como Desenvolvedor C++, quero cadastrar meus projetos em uma biblioteca central, para encontrá-los sem procurar manualmente pelas pastas

#### Descrição

##### Contexto
Ao iniciar a rotina de trabalho ou ao alternar de atividade, o desenvolvedor precisa abrir um projeto específico que está salvo em algum disco ou pasta do computador.

##### Problema atual
Projetos ficam dispersos em múltiplos diretórios e volumes (`C:\dev`, `D:\work`, downloads temporários). O desenvolvedor gasta tempo navegando por árvores de diretórios no Windows Explorer ou tentando lembrar onde salvou determinado repositório.

##### Resultado esperado
Uma listagem centralizada e persistida localmente onde todos os projetos do desenvolvedor ficam acessíveis com um clique, sem duplicação de dados e com integridade de caminhos.

##### Escopo
**Inclui:**
- Adição de projeto via diálogo nativo de seleção de pasta.
- Validação de existência do diretório selecionado.
- Detecção e prevenção de cadastro duplicado (mesmo caminho físico).
- Remoção do projeto da biblioteca do DevHub mantendo intactos todos os arquivos físicos no disco.
- Detecção de projetos órfãos (diretório apagado ou movido externamente no sistema operacional).

**Não inclui:**
- Mover, renomear ou deletar pastas físicas do sistema operacional.
- Sincronização em nuvem ou login de usuário (arquitetura 100% desktop local).

##### Pendências conhecidas
- *Qual o comportamento esperado quando uma pasta adicionada contém múltiplos subprojetos C++ (ex.: um monorepo)?*

#### Referências
- Documento de Personas (`docs/01_PERSONAS_E_HIPOTESES.md`)
- Problema 1: Dispersão de diretórios no sistema de arquivos.

#### Personas relacionadas
- Principal: Desenvolvedor C++ com múltiplos projetos.

#### Business Points
- **5 BP** (Capacidade fundacional crítica: sem a biblioteca central, nenhuma outra funcionalidade do produto tem utilidade).

#### Design
- **Requer design:** Sim.
- **Entrega esperada:** Estado de tela vazia (empty state com call to action claro para adicionar o primeiro projeto), diálogo de adição de pasta e indicação visual para caminhos inválidos.

#### Requisitos funcionais
- `RF-001` — O sistema deve permitir que o desenvolvedor adicione um diretório local do computador como um projeto na biblioteca.
- `RF-002` — O sistema deve validar o caminho selecionado e impedir a inclusão duplicada de um projeto já cadastrado.
- `RF-003` — O sistema deve permitir que o desenvolvedor remova um projeto da biblioteca sem excluir nem modificar os arquivos físicos no disco.
- `RF-004` — O sistema deve verificar a acessibilidade do diretório no sistema de arquivos e sinalizar visualmente quando o caminho do projeto não for encontrado.
- `RF-005` — O sistema deve permitir a atualização do caminho de um projeto caso o diretório tenha sido movido ou renomeado no sistema operacional.

#### Destino
- **Release MVP**.

---

### F02 — Como Desenvolvedor C++, quero identificar automaticamente as características de um projeto, para não precisar cadastrar suas configurações manualmente

#### Descrição

##### Contexto
Imediatamente após a seleção de uma pasta para cadastro, o DevHub inspeciona os arquivos da raiz do projeto para inferir seu ecossistema.

##### Problema atual
Preencher manualmente formulários técnicos (qual sistema de build usa, qual versão de C++, se usa Qt, se usa MSVC) gera atrito, cansaço operacional e desestimula a adoção da ferramenta.

##### Resultado esperado
O sistema analisa os arquivos descritores presentes na pasta e preenche automaticamente as tags técnicas e o sistema de build do projeto.

##### Escopo
**Inclui:**
- Detecção de arquivos conhecidos: `CMakeLists.txt` (CMake), `*.sln` / `*.vcxproj` (Visual Studio / MSVC), `build.ninja` (Ninja), `*.pro` / `CMakeLists.txt com Qt` (Qt).
- Detecção básica do padrão C++ declarado (ex.: flags `set(CMAKE_CXX_STANDARD 20)` no CMake).
- Permissão para o usuário revisar e ajustar as características inferidas.

**Não inclui:**
- Análise profunda de árvore de includes ou compilação silenciosa para deduzir dependências.
- Suporte a sistemas exóticos legados no MVP (foco em CMake e Visual Studio).

##### Pendências conhecidas
- *Se o projeto tiver tanto `CMakeLists.txt` quanto `.sln` (ex: build gerado do CMake), qual deve ter precedência na identificação primária?*

#### Referências
- Documento de Personas (`docs/01_PERSONAS_E_HIPOTESES.md`)
- Problema 2: Sobrecarga de setup manual de propriedades.

#### Personas relacionadas
- Principal: Desenvolvedor C++ com múltiplos projetos.
- Secundária: Desenvolvedor C++ com configurações diferentes.

#### Business Points
- **5 BP** (Transforma o DevHub em um produto inteligente e reduz atrito de onboarding para zero).

#### Design
- **Requer design:** Sim.
- **Entrega esperada:** Badges/chips visuais para sistemas de build e compiladores; modal ou painel de confirmação de características pós-cadastro.

#### Requisitos funcionais
- `RF-006` — O sistema deve escanear a raiz do projeto e identificar automaticamente a presença de arquivos de build (`CMakeLists.txt`, `.sln`, `.vcxproj`).
- `RF-007` — O sistema deve identificar o padrão C++ configurado quando explicitamente declarado nos arquivos de configuração do CMake.
- `RF-008` — O sistema deve sinalizar se o projeto possui dependência ou configuração para o framework Qt.
- `RF-009` — O sistema deve permitir que o desenvolvedor altere ou adicione manualmente características caso a detecção automática falhe ou seja incompleta.

#### Destino
- **Release MVP**.

---

### F03 — Como Desenvolvedor C++, quero encontrar e organizar meus projetos por busca, categoria e favorito, para acessar rapidamente o projeto em que preciso trabalhar

#### Descrição

##### Contexto
O desenvolvedor já tem mais de 10 a 20 projetos na biblioteca e precisa localizar um deles em poucos segundos sem rolagem exaustiva.

##### Problema atual
Listas longas sem filtro ou categorização tornam a busca demorada e frustrante, anulando o benefício de ter uma biblioteca centralizada.

##### Resultado esperado
O desenvolvedor digita parte do nome do projeto ou clica em uma categoria/favorito e a visualização é filtrada instantaneamente (< 100ms).

##### Escopo
**Inclui:**
- Campo de busca textual com filtro em tempo real (nome do projeto e tags).
- Marcação de projetos favoritos (fixados ou destacados).
- Criação, edição e exclusão de categorias personalizadas (ex.: "Empresa", "Estudos", "Open Source").
- Filtro rápido por características técnicas (ex.: "Apenas CMake", "Apenas Qt").

**Não inclui:**
- Hierarquias ilimitadas de pastas virtuais aninhadas (organização em tags/categorias planas).

##### Pendências conhecidas
- *Um projeto pode pertencer a múltiplas categorias ou apenas a uma categoria principal?*

#### Referências
- Documento de Personas (`docs/01_PERSONAS_E_HIPOTESES.md`).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com múltiplos projetos.

#### Business Points
- **5 BP** (Acesso ágil diário: sem boa busca e categorização, a aplicação perde utilidade com o crescimento do catálogo).

#### Design
- **Requer design:** Sim.
- **Entrega esperada:** Barra de busca de alto contraste, seletor de categorias na barra lateral ou superior, e ícone de estrela/favorito com resposta tátil visual.

#### Requisitos funcionais
- `RF-010` — O sistema deve filtrar a lista de projetos em tempo real conforme o usuário digita termos de busca.
- `RF-011` — O sistema deve permitir marcar e desmarcar projetos como favoritos.
- `RF-012` — O sistema deve permitir criar, renomear e excluir categorias de projetos.
- `RF-013` — O sistema deve permitir atribuir uma ou mais tags ou categorias a um projeto.
- `RF-014` — O sistema deve disponibilizar filtro rápido para exibir apenas projetos favoritos.

#### Destino
- **Release MVP**.

---

### F04 — Como Desenvolvedor C++, quero abrir um projeto e suas ferramentas de desenvolvimento a partir do DevHub, para iniciar o trabalho sem navegar manualmente pelo computador

#### Descrição

##### Contexto
O desenvolvedor encontrou o card do projeto e agora quer começar a trabalhar imediatamente.

##### Problema atual
Ele precisa abrir a IDE manualmente, ir em "Open Folder", navegar pelas pastas, ou abrir o PowerShell e dar `cd` até a pasta do projeto.

##### Resultado esperado
A partir do DevHub, com 1 clique o desenvolvedor abre a IDE correta (VS Code, Visual Studio), abre um terminal posicionado no diretório ou abre o Windows Explorer na pasta do projeto.

##### Escopo
**Inclui:**
- Ação "Abrir na IDE" (com suporte automático para VS Code `code .` e Visual Studio `devenv`).
- Ação "Abrir no Terminal" (iniciando o terminal padrão do SO no diretório-raiz do projeto).
- Ação "Abrir no Explorer" (abrindo a pasta no gerenciador de arquivos).
- Escolha da IDE preferida por projeto caso mais de uma esteja instalada.

**Não inclui:**
- Emulador de terminal embutido dentro da UI do DevHub (o terminal nativo do Windows/Linux é invocado).

##### Pendências conhecidas
- *Se o projeto for puramente CMake, mas a máquina tiver VS Code e Visual Studio, qual deve ser a ação de clique padrão?*

#### Referências
- Documento de Personas (`docs/01_PERSONAS_E_HIPOTESES.md`).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com múltiplos projetos.
- Secundária: Desenvolvedor C++ com configurações diferentes.

#### Business Points
- **5 BP** (Capacidade core de orquestração: conecta a descoberta à ação prática de codificação).

#### Design
- **Requer design:** Sim.
- **Entrega esperada:** Botões de ação rápida no card do projeto (ícones de IDE, terminal e pasta) com tooltips autoexplicativos.

#### Requisitos funcionais
- `RF-015` — O sistema deve abrir o projeto na IDE associada ao projeto ao acionar a ação de abertura.
- `RF-016` — O sistema deve permitir que o desenvolvedor escolha qual IDE deve ser a padrão para aquele projeto específico.
- `RF-017` — O sistema deve abrir uma janela de terminal do sistema operacional inicializada diretamente na pasta-raiz do projeto.
- `RF-018` — O sistema deve abrir o gerenciador de arquivos do sistema operacional na pasta-raiz do projeto.
- `RF-019` — O sistema deve notificar o usuário com mensagem clara caso a IDE configurada não esteja instalada ou não seja encontrada no sistema.

#### Destino
- **Release MVP**.

---

### F05 — Como Desenvolvedor C++, quero visualizar as informações técnicas e a estrutura de um projeto, para entender rapidamente como ele está configurado

#### Descrição

##### Contexto
Ao retornar a um projeto após meses ou ao receber um repositório de outro desenvolvedor, é necessário inspecionar o setup técnico antes de mexer no código.

##### Problema atual
É necessário abrir arquivos como `CMakeLists.txt`, verificar se há pastas de build residuais e ler notas espalhadas para saber qual padrão e gerador utilizar.

##### Resultado esperado
Uma visão detalhada e limpa com o resumo das propriedades técnicas do projeto, caminho completo, data do último acesso e anotações pessoais do desenvolvedor.

##### Escopo
**Inclui:**
- Visualização de metadados: caminho físico, sistema de build identificado, padrão C++, data de inclusão e último acesso.
- Campo de notas/observações do desenvolvedor (ex.: "Usar flag -DENABLE_TESTS=OFF no build").

**Não inclui:**
- Editor de texto embutido para modificar código-fonte.

##### Pendências conhecidas
- *O desenvolvedor precisa visualizar a árvore de diretórios básica dentro do DevHub ou o Explorer/IDE já suprem isso?*

#### Referências
- Documento de Personas (`docs/01_PERSONAS_E_HIPOTESES.md`).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com configurações diferentes.

#### Business Points
- **3 BP** (Redução de carga cognitiva e facilidade de contextualização).

#### Design
- **Requer design:** Sim.
- **Entrega esperada:** Página/painel de detalhes do projeto estruturado em blocos de fácil leitura.

#### Requisitos funcionais
- `RF-020` — O sistema deve exibir os metadados técnicos consolidados do projeto em uma visão de detalhes.
- `RF-021` — O sistema deve registrar e exibir a data e hora do último acesso ao projeto via DevHub.
- `RF-022` — O sistema deve permitir que o desenvolvedor salve anotações de texto livre vinculadas ao projeto.

#### Destino
- **Release MVP**.

---

### F06 — Como Desenvolvedor C++, quero executar o build de um projeto pelo DevHub, para iniciar a compilação sem configurar manualmente o comando a cada vez

#### Descrição

##### Contexto
O desenvolvedor precisa rodar a compilação do projeto sem necessariamente ter aberto a IDE pesada.

##### Problema atual
Compilar via terminal exige abrir o prompt correto (ex.: Developer Command Prompt do VS), dar `cd` e lembrar comandos como `cmake --build build --config Release`.

##### Resultado esperado
Um botão "Build" no DevHub que dispara a compilação do projeto utilizando a toolchain configurada.

##### Escopo
**Inclui:**
- Execução de comando padrão de build (`cmake --build`).
- Exibição de indicador de progresso (executando, sucesso, erro).

**Não inclui:**
- Resolução interativa de erros ou depuração passo a passo.

##### Pendências conhecidas
- *`[PENDENTE CRÍTICO]` O desenvolvedor realmente deseja compilar pelo DevHub ou prefere usar a própria IDE onde os erros clicáveis levam à linha de código?*
- *`[PENDENTE]` Como capturar e exibir logs de build longos sem comprometer o desempenho da interface?*

#### Referências
- Hipótese de Risco 1 (`docs/01_PERSONAS_E_HIPOTESES.md`).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com configurações diferentes.

#### Business Points
- **3 BP** (Valor condicional à validação com usuários reais).

#### Design
- **Requer design:** Sim (painel de saída de log e estados de compilação).

#### Requisitos funcionais
- `RF-023` — O sistema deve disparar o processo de compilação com base no sistema de build configurado para o projeto.
- `RF-024` — O sistema deve exibir o status do build (em execução, concluído com sucesso, falhou).
- `RF-025` — O sistema deve permitir visualizar o log textual gerado pelo processo de build.

#### Destino
- **Backlog** (retido até confirmação na pesquisa de descoberta).

---

### F07 — Como Desenvolvedor C++, quero visualizar informações básicas do Git de um projeto, para saber seu estado atual sem abrir outra ferramenta

#### Descrição

##### Contexto
Ao examinar a lista de projetos, o desenvolvedor quer saber quais projetos têm alterações pendentes não commitadas ou em qual branch estão.

##### Problema atual
É preciso abrir o terminal ou a IDE em cada pasta para checar `git status` e `git branch`.

##### Resultado esperado
O card do projeto exibe a branch atual e um indicador visual discreto informando se o repositório está limpo (*clean*) ou com modificações (*dirty*).

##### Escopo
**Inclui:**
- Leitura não destrutiva da branch ativa (`HEAD`).
- Verificação se há arquivos modificados ou não rastreados.

**Não inclui:**
- Comandos destrutivos de escrita (commit, push, pull, merge, rebase).

##### Pendências conhecidas
- *`[PENDENTE CRÍTICO]` A consulta contínua de status do Git em 50 projetos causa lentidão de I/O em discos magnéticos ou pastas de rede?*
- *`[PENDENTE]` O desenvolvedor toma decisões baseadas nessa informação no dashboard ou só quando já está com o projeto aberto?*

#### Referências
- Hipótese de Risco 2 (`docs/01_PERSONAS_E_HIPOTESES.md`).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com múltiplos projetos.

#### Business Points
- **3 BP** (Informativo, não essencial para o primeiro uso).

#### Design
- **Requer design:** Sim (indicador de branch e status de dirty/clean no card).

#### Requisitos funcionais
- `RF-026` — O sistema deve identificar se o diretório do projeto é um repositório Git válido.
- `RF-027` — O sistema deve exibir o nome da branch atualmente ativa do repositório.
- `RF-028` — O sistema deve exibir se existem alterações locais pendentes de commit.

#### Destino
- **Backlog** (retido até avaliação de performance e validação de interesse).

---

### F08 — Como Desenvolvedor C++, quero criar projetos a partir de templates, para iniciar novos projetos com uma estrutura padronizada

#### Descrição

##### Contexto
O desenvolvedor precisa iniciar um novo experimento, biblioteca ou executável C++ e não quer criar manualmente arquivos `CMakeLists.txt`, `.gitignore` e `main.cpp`.

##### Problema atual
O processo manual é repetitivo e propenso a esquecimento de boas práticas de estrutura de diretórios (`src`, `include`, `tests`).

##### Resultado esperado
Um assistente onde o desenvolvedor escolhe um modelo (ex.: "CMake CLI C++20", "Qt 6 Quick Application"), define o nome e pasta de destino, e o DevHub gera o esqueleto pronto e já o cadastra na biblioteca.

##### Escopo
**Inclui:**
- Criação de novos projetos baseados em templates embutidos.
- Inclusão automática do projeto gerado na biblioteca do DevHub.

**Não inclui:**
- Editor visual de templates no MVP.

##### Pendências conhecidas
- *`[PENDENTE CRÍTICO]` Com que frequência o desenvolvedor inicia projetos do zero para justificar essa funcionalidade no MVP?*

#### Referências
- Hipótese de Risco 3 (`docs/01_PERSONAS_E_HIPOTESES.md`).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com configurações diferentes.

#### Business Points
- **3 BP** (Funcionalidade de conveniência).

#### Design
- **Requer design:** Sim (fluxo modal de criação em passos: template, nome, caminho).

#### Requisitos funcionais
- `RF-029` — O sistema deve disponibilizar modelos pré-configurados de projetos C++ com CMake.
- `RF-030` — O sistema deve permitir informar o nome do projeto e a pasta de destino para a criação.
- `RF-031` — O sistema deve gerar os arquivos do template e cadastrar automaticamente o novo projeto na biblioteca.

#### Destino
- **Backlog** (aguarda validação de frequência de uso).

---

### F09 — Como Desenvolvedor C++, quero configurar as ferramentas utilizadas pelo DevHub, para que as ações sejam executadas no meu ambiente de desenvolvimento

#### Descrição

##### Contexto
O desenvolvedor instala o DevHub em uma máquina onde IDEs e terminais podem estar em caminhos personalizados ou versões portáteis.

##### Problema atual
Se a ferramenta assumir cegamente caminhos hardcoded, falhará ao tentar disparar ações em ambientes com instalações customizadas.

##### Resultado esperado
Uma área de configurações onde o DevHub detecta as ferramentas instaladas no sistema e permite ao desenvolvedor ajustar o caminho dos executáveis (VS Code, Visual Studio, Terminal padrão).

##### Escopo
**Inclui:**
- Detecção automática de executáveis disponíveis no `PATH`.
- Campo para apontar caminho manual de IDEs ou terminais.
- Seleção de terminal padrão (PowerShell, Windows Terminal, cmd, Git Bash).

**Não inclui:**
- Download e instalação automática de compiladores ou IDEs.

##### Pendências conhecidas
- *Quais variáveis de ambiente adicionais precisam ser passadas para as ferramentas lançadas?*

#### Referências
- Documento de Personas (`docs/01_PERSONAS_E_HIPOTESES.md`).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com configurações diferentes.

#### Business Points
- **3 BP** (Essencial para garantir que as ações de F04 funcionem em qualquer máquina).

#### Design
- **Requer design:** Sim (tela ou gaveta de configurações de ambiente).

#### Requisitos funcionais
- `RF-032` — O sistema deve detectar automaticamente a presença das IDEs suportadas instaladas no sistema.
- `RF-033` — O sistema deve permitir que o desenvolvedor especifique caminhos customizados para os executáveis de IDE e terminal.
- `RF-034` — O sistema deve permitir selecionar o terminal padrão a ser utilizado nas ações do DevHub.
- `RF-035` — O sistema deve persistir as preferências de ambiente de forma segura e local.

#### Destino
- **Release MVP**.

---

### F10 — Como Desenvolvedor C++, quero fazer uma varredura automática em uma pasta raiz, para cadastrar múltiplos projetos de uma só vez

#### Descrição

##### Contexto
No onboarding inicial do DevHub ou ao reorganizar um disco de desenvolvimento (`C:\dev`, `D:\workspace`), o desenvolvedor possui dezenas de pastas com projetos C++ dispersos em subdiretórios.

##### Problema atual
Cadastrar 20 a 50 projetos individualmente através do diálogo de seleção de pasta gera atrito, lentidão e desestimula a adoção do sistema.

##### Resultado esperado
O desenvolvedor seleciona uma pasta raiz, o sistema faz uma varredura recursiva segura (até 2 níveis de profundidade), identifica pastas contendo descritores C++ conhecidos (`CMakeLists.txt`, `*.sln`, `*.vcxproj`, `build.ninja`) e exibe uma lista de conferência pré-importação para seleção e cadastro atômico em lote.

##### Escopo
**Inclui:**
- Seleção de diretório raiz via diálogo nativo do sistema operacional.
- Varredura recursiva com profundidade controlada (2 níveis).
- Ignorar pastas de sistema e pastas de build conhecidas (`build`, `.git`, `.vs`, `out`, `CMakeFiles`, `node_modules`).
- Diálogo modal de conferência com listagem dos projetos encontrados, caminhos, sistemas de build inferidos e indicador se já estão cadastrados.
- Checkboxes para marcar/desmarcar projetos antes de confirmar a importação.
- Persistência atômica dos projetos selecionados no banco SQLite local.

**Não inclui:**
- Varredura irrestrita em toda a unidade raiz de disco (`C:\`).
- Download de repositórios remotos.

##### Pendências conhecidas
- *Qual profundidade padrão de varredura equilibra tempo de I/O em discos rígidos e precisão de descoberta?* (Regra: profundidade máxima de 2 níveis com opção de busca avançada).

#### Referências
- Documento de Personas (`docs/01_PERSONAS_E_HIPOTESES.md`).
- Extensão natural das funcionalidades F01 e F02.

#### Personas relacionadas
- Principal: Desenvolvedor C++ com múltiplos projetos.

#### Business Points
- **5 BP** (Reduz o tempo de onboarding e adoção da ferramenta de minutos/horas para poucos segundos).

#### Design
- **Requer design:** Sim (modal de conferência em lote com checkboxes e contadores estatísticos em estilo xAI).

#### Requisitos funcionais
- `RF-036` — O sistema deve permitir que o desenvolvedor selecione uma pasta raiz para efetuar uma varredura recursiva de projetos C++.
- `RF-037` — O sistema deve ignorar pastas de artefatos de build e controle de versão conhecidas (`build`, `.git`, `.vs`, `out`, `CMakeFiles`) durante a varredura para evitar falsos positivos.
- `RF-038` — O sistema deve apresentar a lista de projetos candidatos identificados com nome, caminho, sistema de build inferido e indicador se o item já está cadastrado.
- `RF-039` — O sistema deve permitir que o desenvolvedor marque ou desmarque individualmente os projetos antes de confirmar a importação.
- `RF-040` — O sistema deve persistir todos os projetos selecionados no banco SQLite local em uma única transação atômica.

#### Destino
- **Release MVP (v1.1)**.

---

### F11 — Como Desenvolvedor C++, quero visualizar o consumo de disco dos artefatos de build e limpá-los com segurança, para liberar espaço sem danificar os códigos-fonte

#### Descrição

##### Contexto
Projetos C++ compilados geram grandes volumes de arquivos binários intermediários (`.obj`, `.pdb`, `.ilk`, `.cache`, `CMakeFiles`). Em múltiplos projetos, essas pastas frequentemente acumulam de 30 a 100 GB em SSDs de alta performance.

##### Problema atual
O desenvolvedor fica sem espaço no disco e precisa navegar manualmente por dezenas de pastas para encontrar onde estão as compilações antigas ou abandonadas, com risco constante de apagar acidentalmente códigos-fonte ou pastas de configuração.

##### Resultado esperado
O DevHub calcula o tamanho ocupado pelas pastas de compilação detectadas (`build/`, `out/`, `.vs/`, `bin/`) de cada projeto cadastrado e fornece um comando explícito de limpeza segura com dupla confirmação e relatório de bytes liberados.

##### Escopo
**Inclui:**
- Cálculo assíncrono do tamanho de pastas de build conhecidas.
- Exibição de indicador de peso no Side Peek e na tabela de projetos.
- Botão de limpeza segura (`Clean Build Artifacts`).
- Validação prévia de arquivos de cache de build (`CMakeCache.txt`, `.ninja_log`, `*.obj`) antes de autorizar a deleção.
- Diálogo modal de confirmação exibindo exatamente os diretórios que serão apagados.

**Não inclui:**
- Deletar código-fonte, arquivos `.h`/`.cpp`, arquivos de versionamento `.git` ou dependências instaladas globalmente no sistema operacional.

##### Pendências conhecidas
- *Como garantir que uma pasta chamada `bin` ou `build` personalizada pelo usuário não contenha código-fonte manual antes de deletar?* (Regra: verificar a presença de arquivos de build como `CMakeCache.txt`, `.ninja_log` ou `*.vcxproj.FileListAbsolute.txt`).

#### Referências
- Pesquisa de dores operacionais de desenvolvedores C++ em ambientes com SSDs limitados.
- Princípio de não-poluição e integridade de dados do DevHub.

#### Personas relacionadas
- Principal: Desenvolvedor C++ com múltiplos projetos.

#### Business Points
- **5 BP** (Resolve uma dor física imediata de infraestrutura local e economia de hardware).

#### Design
- **Requer design:** Sim (indicador mono de tamanho em `Geist Mono` e diálogo modal com confirmação explícita no estilo xAI).

#### Requisitos funcionais
- `RF-041` — O sistema deve identificar pastas de build associadas a cada projeto (`build`, `out`, `bin`, `.vs`) e calcular o tamanho total ocupado em disco em segundo plano.
- `RF-042` — O sistema deve exibir o volume de disco ocupado na gaveta lateral (*Side Peek*) com formatação legível (ex.: `MB`, `GB`).
- `RF-043` — O sistema deve exigir confirmação explícita do desenvolvedor exibindo exatamente quais diretórios serão apagados antes de executar a limpeza.
- `RF-044` — O sistema deve validar a presença de arquivos de cache de build (`CMakeCache.txt`, `build.ninja`, `*.obj`) antes de autorizar a deleção, impedindo a exclusão de diretórios de código.
- `RF-045` — O sistema deve atualizar imediatamente o contador de espaço em disco após a conclusão da limpeza.

#### Destino
- **Release MVP (v1.1)**.

---

### F12 — Como Desenvolvedor C++, quero diagnosticar a compatibilidade do ambiente e compiladores instalados (Toolchain Doctor), para saber se posso compilar o projeto antes de tentar o build

#### Descrição

##### Contexto
Projetos C++ exigem versões específicas de compiladores (ex.: C++20 requer GCC 10+, Clang 10+ ou MSVC 19.29+), geradores de build (CMake 3.20+) e ferramentas de linha de comando.

##### Problema atual
O desenvolvedor abre um projeto antigo ou clonado recentemente e a tentativa de compilação falha silenciosamente com erros complexos de sintaxe ou CMake devido à ausência de compilador compatível no `PATH`.

##### Resultado esperado
Um painel "Toolchain Doctor" no DevHub que inspeciona os compiladores instalados na máquina (`gcc --version`, `clang --version`, `cl.exe`, `cmake --version`, `ninja --version`) e cruza com os requisitos declarados no `CMakeLists.txt` do projeto, sinalizando antecipadamente a prontidão do ambiente.

##### Escopo
**Inclui:**
- Detecção no `PATH` de `g++`, `clang++`, `cl.exe`, `cmake`, `ninja`.
- Comparação da versão do C++ requerida no projeto com o suporte máximo do compilador encontrado.
- Alerta visual caso o ambiente não atenda aos requisitos do projeto.

**Não inclui:**
- Baixar ou instalar compiladores automaticamente pela internet.

##### Pendências conhecidas
- *Como detectar toolchains instaladas em locais não presentes no PATH global (ex.: instâncias específicas do Visual Studio Community/Enterprise via `vswhere.exe`)?*

#### Referências
- Catálogo F02 (Identificação automática) e F09 (Configuração de ferramentas).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com configurações diferentes.

#### Business Points
- **3 BP** (Previne frustrações e diagnósticos demorados de erros crípticos de compilação).

#### Design
- **Requer design:** Sim (tabela diagnóstica com status em pílulas outline discretas).

#### Requisitos funcionais
- `RF-046` — O sistema deve executar verificações locais assíncronas para detectar versões de `cmake`, `ninja`, `gcc`, `clang` e `cl` instaladas no sistema.
- `RF-047` — O sistema deve comparar a versão do padrão C++ declarada no projeto (`CMAKE_CXX_STANDARD`) com as capacidades do compilador padrão configurado.
- `RF-048` — O sistema deve alertar o desenvolvedor quando uma ferramenta obrigatória para aquele projeto não for encontrada no ambiente local.

#### Destino
- **Backlog**.

---

### F13 — Como Desenvolvedor C++, quero visualizar as dependências de bibliotecas externas (vcpkg / Conan) de cada projeto, para entender quais pacotes são necessários antes de configurar o ambiente

#### Descrição

##### Contexto
O ecossistema moderno de C++ adota gerenciadores de pacotes declarativos baseados em arquivos no repositório (`vcpkg.json`, `conanfile.txt` ou `conanfile.py`).

##### Problema atual
Para saber quais bibliotecas externas (Boost, OpenCV, fmt, OpenSSL, Qt) o projeto necessita, o desenvolvedor precisa abrir e inspecionar manualmente os arquivos de manifesto.

##### Resultado esperado
O DevHub lê arquivos manifestos presentes na raiz e lista as dependências de pacotes em pílulas limpas na gaveta lateral (*Side Peek*).

##### Escopo
**Inclui:**
- Leitura de `vcpkg.json` (seção `dependencies`) e `conanfile.txt` (seção `[requires]`).
- Exibição dos nomes dos pacotes na visualização técnica do Side Peek.

**Não inclui:**
- Executar `vcpkg install` ou `conan install` diretamente (evita efeitos colaterais de rede não autorizados).

##### Pendências conhecidas
- *Como lidar com projetos que utilizam Git Submodules ou `FetchContent` do CMake para dependências?*

#### Referências
- Arquitetura F05 (Visualização técnica).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com configurações diferentes.

#### Business Points
- **3 BP** (Facilita o entendimento imediato da arquitetura e bibliotecas de qualquer projeto).

#### Design
- **Requer design:** Não (reutiliza o padrão existente de tags em pílula na gaveta lateral).

#### Requisitos funcionais
- `RF-049` — O sistema deve identificar a presença de `vcpkg.json` e `conanfile.txt` no diretório raiz do projeto.
- `RF-050` — O sistema deve realizar o parse das dependências declaradas e exibi-las como tags técnicas legíveis no *Side Peek*.
- `RF-051` — Quando nenhum gerenciador de pacotes for identificado, o sistema deve registrar `Nenhum gerenciador de pacotes externo detectado`.

#### Destino
- **Backlog**.

---

### F14 — Como Desenvolvedor C++, quero alternar rapidamente entre os projetos recentes através de um seletor rápido (Quick Switcher com `/` ou `Ctrl+K`), para trocar de contexto sem interromper meu fluxo

#### Descrição

##### Contexto
Ao longo do dia, o desenvolvedor alterna frequentemente entre 2 ou 3 projetos ativos (ex.: biblioteca core e aplicativo cliente).

##### Problema atual
Toda troca de projeto exige navegar com mouse na tabela ou digitar buscas repetitivas no campo de texto principal.

##### Resultado esperado
Um diálogo de paleta de comando modal estilo xAI / Spotlight (`Ctrl+K` ou `Ctrl+P`) que se abre instantaneamente sobre a interface, listando os projetos por ordem de último acesso (`last_accessed`), permitindo seleção rápida via teclado (setas + Enter) para abrir diretamente na IDE ou terminal.

##### Escopo
**Inclui:**
- Atalho de teclado global na janela (`Ctrl+K` / `Ctrl+P`).
- Lista ordenada pelos projetos mais recentes.
- Navegação por setas do teclado com feedback visual.
- Ação direta com `Enter` (abre IDE) e `Ctrl+Enter` (abre Terminal).

**Não inclui:**
- Paleta de comandos fora da janela da aplicação (global system hook).

##### Pendências conhecidas
- *Qual o número ideal de projetos recentes a serem pré-listados no modal antes de qualquer digitação?* (Hipótese: 5 mais recentes).

#### Referências
- Padrões de produtividade para desenvolvedores (VS Code Command Palette, Raycast).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com múltiplos projetos.

#### Business Points
- **3 BP** (Velocidade e ergonomia de uso diário contínuo).

#### Design
- **Requer design:** Sim (modal flutuante centralizado, canvas `#141414`, borda hairline `#212327`, input integrado e navegação por teclado).

#### Requisitos funcionais
- `RF-052` — O sistema deve exibir a paleta de alternância rápida ao pressionar `Ctrl+K` ou `Ctrl+P`.
- `RF-053` — A paleta deve apresentar inicialmente os últimos projetos acessados, ordenados decrescentemente pela data/hora de abertura.
- `RF-054` — A digitação na paleta deve filtrar instantaneamente os projetos por nome, caminho ou tags.
- `RF-055` — Pressionar `Enter` no item selecionado deve disparar a IDE preferida configurada para aquele projeto e fechar a paleta.

#### Destino
- **Release MVP (v1.1)**.

---

### F15 — Como Desenvolvedor C++, quero visualizar um panorama do status Git dos meus projetos em lote (branches e alterações pendentes), para não esquecer códigos não comitados no fim do dia

#### Descrição

##### Contexto
No final da jornada de trabalho ou antes de realizar reuniões de alinhamento, o desenvolvedor precisa saber se deixou arquivos modificados, branches desatualizadas ou commits locais não enviados em algum dos seus 10+ repositórios.

##### Problema atual
Abrir individualmente cada terminal ou cliente Git para rodar `git status` em 10 projetos consome tempo e é frequentemente esquecido, gerando perda de contexto no dia seguinte.

##### Resultado esperado
Uma coluna ou visão "Git Radar" que executa comandos rápidos de leitura (`git status --porcelain`, `git branch --show-current`) sem travar a UI e exibe de forma sucinta: nome da branch atual e se há arquivos alterados (ex.: `main · 3 modificados`).

##### Escopo
**Inclui:**
- Leitura de status Git assíncrona.
- Exibição resumida na tabela de projetos.
- Indicação visual em pílula outline (`main`, `clean` ou `3 modified`).

**Não inclui:**
- Comandos de escrita (`git commit`, `git push`, `git rebase`, resolver conflitos).

##### Pendências conhecidas
- *Qual o impacto de desempenho ao consultar o status Git de 50 repositórios simultaneamente?* (Regra: execução em thread pool em segundo plano com timeout de 1s por repositório).

#### Referências
- Catálogo F07 (Informações básicas do Git no Backlog).

#### Personas relacionadas
- Principal: Desenvolvedor C++ com múltiplos projetos.

#### Business Points
- **3 BP** (Visibilidade de alto valor sobre integridade do trabalho local).

#### Design
- **Requer design:** Sim (coluna discreta na tabela com badges em `Geist Mono`).

#### Requisitos funcionais
- `RF-056` — O sistema deve verificar de forma assíncrona se a pasta do projeto é um repositório Git (presença do diretório `.git`).
- `RF-057` — O sistema deve extrair o nome da branch ativa e a contagem de arquivos pendentes de commit via comando Git local de leitura rápida.
- `RF-058` — O sistema deve exibir a informação de forma não-bloqueante na tabela de projetos através de pílulas informativas.
- `RF-059` — Caso o Git não esteja instalado no sistema ou o comando falhe por timeout, o sistema deve apresentar `Git Indisponível` sem interromper a exibição do projeto.

#### Destino
- **Backlog**.

