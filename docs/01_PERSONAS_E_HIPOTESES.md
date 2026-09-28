# Personas & Hipóteses de Produto — DevHub
**Metodologia HOUS3**  
**Status:** Hipótese de produto expandida para atender desenvolvedores multi-stack e poliglotas que mantêm múltiplos repositórios locais.

---

## 1. Visão Geral das Hipóteses

O **DevHub** é concebido como uma aplicação desktop nativa que atua como **painel central de controle, descoberta e orquestração** de projetos locais para qualquer desenvolvedor de software, com suporte aprofundado a C++, Rust, Python, Go, TypeScript/Node.js e C#.

### O que o produto É:
- Catálogo centralizado e de alta densidade de projetos locais no computador.
- Inspetor automático de configurações técnicas (CMake, Cargo, pyproject.toml, package.json, toolchains).
- Lançador contextualizado de ferramentas (VS Code, Visual Studio, CLion, RustRover, PyCharm, Terminais, Explorer).
- Gestor de higiene de armazenamento local (limpeza de artefatos pesados de compilação/dependências).

### O que o produto NÃO É:
- Não edita código-fonte.
- Não substitui IDEs.
- Não substitui sistemas de controle de versão (Git).
- Não armazena nem duplica código-fonte.

---

## 2. Mapa do Ecossistema de Personas

A gestão de múltiplos repositórios locais é uma dor compartilhada por diferentes papéis de engenharia:

```text
               Engenheiro de Software / Desenvolvedor
                                 │
         ┌───────────────────────┼───────────────────────┐
         │                       │                       │
         ▼                       ▼                       ▼
Persona 1: Multi-Stack      Persona 2: Sistemas/Toolchains   Persona 3: Tech Lead
(C++, Rust, Python, TS)     (Compiladores conflitantes)     (Auditoria de 30+ repos)
         │                       │                       │
         └───────────────────────┼───────────────────────┘
                                 ▼
                              DevHub
```

---

## 3. Persona 1: Desenvolvedor Multi-Stack (Poliglota) com Múltiplos Projetos

* **Analogia:** O profissional que precisa de um "painel de garagem universal" para saber onde estão todos os seus projetos (C++, Rust, Python, Web) e como colocar cada um para rodar sem perder tempo.
* **Quem opera na prática:** Desenvolvedor que atua em sistemas modernos, mantendo código nativo (C++/Rust) e tooling/serviços auxiliares (Python, Go, Node.js) no mesmo computador.

### Contexto
Mantém dezenas de repositórios em diretórios locais variados (`C:\dev`, `D:\work`, `E:\oss`). Seus projetos utilizam diferentes ecossistemas:
- Motores e bibliotecas de performance em C++ (CMake, Qt, Ninja)
- Ferramentas de linha de comando ou utilitários em Rust (`Cargo.toml`)
- Scripts de automação, machine learning ou backends em Python (`pyproject.toml`, `requirements.txt`)
- Frontends ou interfaces em TypeScript/JavaScript (`package.json`)
- Microsserviços em Go (`go.mod`)

### Lacunas e Dúvidas Críticas
* `[PENDENTE]` Quantos projetos de diferentes linguagens o desenvolvedor mantém simultaneamente na máquina?
* `[PENDENTE]` Quantas vezes por dia/semana alterna entre projetos de stacks diferentes?
* `[PENDENTE]` Os projetos ficam concentrados em uma pasta raiz (ex: `C:\dev`) ou espalhados em vários discos (`C:`, `D:`, repositórios de rede)?
* `[PENDENTE]` Qual o tempo gasto semanalmente apenas localizando e preparando o ambiente para abrir um projeto?

### Frustrações Hipotéticas
1. Tempo perdido navegando no Windows Explorer até encontrar o diretório correto.
2. Limitação dos menus de "Recents" das IDEs (o histórico do VS Code mistura projetos web com backend e apaga facilmente; o Visual Studio só enxerga `.sln`).
3. Dificuldade de lembrar qual IDE ou qual terminal deve ser aberto para cada tipo de projeto.
4. Ao mover uma pasta de lugar, atalhos do Windows ou scripts manuais quebram.
2. Limitação dos menus de "Recents" das IDEs (misturam projetos, perdem histórico ao renomear pastas ou mudar de máquina).
3. Projetos antigos ficam esquecidos ou tornam-se difíceis de rodar por falta de lembrança de onde ficava o entrypoint.
4. Ao mover uma pasta de lugar, atalhos do Windows ou scripts manuais quebram.

### Soluções Atuais e Suas Falhas
| Solução Atual | O que resolve | Onde quebra |
|---|---|---|
| **Windows Explorer** | Navegação básica de arquivos | Exige lembrar o caminho exato; sem contexto técnico |
| **Recentes da IDE** | Acesso rápido aos 5-10 últimos | Não cobre múltiplas IDEs; histórico limpa fácil |
| **Terminal / Scripts** | Abertura ágil para quem memorizou | Manutenção cara de scripts; quebra se caminhos mudarem |

---

## 4. Persona 2: Desenvolvedor C++ com Configurações Diferentes

* **Analogia:** O profissional que mantém várias "estações de trabalho" com ferramentas e versões de compiladores conflitantes no mesmo computador.
* **Quem opera na prática:** Desenvolvedor sênior, mantenedor de biblioteca ou consultor que alterna entre diferentes padrões C++ (C++14 até C++23) e ecossistemas (Qt, Win32 nativo, CMake cross-platform).

### Contexto
Trabalha com projetos com requisitos incompatíveis entre si:
- Projeto A: C++17 + CMake + Visual Studio 2019 + MSVC
- Projeto B: C++20 + CMake + VS Code + Clang/LLVM + Ninja
- Projeto C: C++23 + Qt 6 + QMake/CMake + Qt Creator

### Lacunas e Dúvidas Críticas
* `[PENDENTE]` Quantas toolchains/compiladores diferentes estão instaladas na máquina?
* `[PENDENTE]` O desenvolvedor usa variáveis de ambiente globais ou scripts `vcvarsall.bat` / environments virtuais?
* `[PENDENTE]` O maior atrito é na descoberta das dependências, na configuração do CMake ou no disparo da compilação?

### Frustrações Hipotéticas
1. Abrir um projeto na IDE errada e sofrer com erros de intellisense e compilação desconfigurada.
2. Dificuldade de lembrar quais flags de CMake ou gerador (Ninja vs Visual Studio Generators) foram usados originalmente.
3. Necessidade de abrir múltiplos arquivos (`CMakeLists.txt`, `.vscode/settings.json`) apenas para inspecionar o setup.

---

## 5. Hipóteses de Risco Crítico (Features sob Teste)

As seguintes ideias extraídas de PRDs legados **NÃO** devem ser tratadas como certas até a fase de Descoberta:

1. **Compilação integrada no DevHub (Build):**  
   *Hipótese de risco:* O desenvolvedor realmente deseja compilar no DevHub, ou ele sempre preferirá o atalho `Ctrl+Shift+B` da sua IDE com highlight de erros no código?
2. **Dashboard Git integrado (Git):**  
   *Hipótese de risco:* Exibir status do Git agrega valor real de decisão, ou é redundante em relação ao Git CLI/GitLens?
3. **Gerador de Templates (Scaffolding):**  
   *Hipótese de risco:* O desenvolvedor cria projetos novos com frequência suficiente para justificar templates no DevHub, ou ele copia uma pasta existente / usa geradores de linha de comando (`cookiecutter`, `cmake-init`)?
