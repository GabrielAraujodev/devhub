# Design System — Uber Black-and-White Duet

> **Diretrizes de Design e Tokens para o DevHub Workspace**  
> Inspirado na estética visual e arquitetura de componentes da Uber: contraste purista em preto e branco (*black-and-white duet*), pílulas de 999px em todos os controles interativos, cards com raio de 16px (`rounded.xl`), tipografia em caixa-alta apenas para eyebrows raros, e ausência total de acentos cromáticos decorativos (sem azul, verde ou laranja desnecessários).

---

## 1. Visão Geral & Filosofia

A interface da Uber comunica escala e clareza operacional através de contenção radical:
- **Dueto Preto e Branco:** Fundo canvas branco `{colors.canvas}` (`#ffffff`) com o preto `{colors.primary}` (`#000000`) como âncora absoluta de conversão e contraste (todo botão primário, cabeçalho de destaque e faixas de polaridade invertida).
- **A Pílula como Assinatura de Interação:** Absolutamente todos os elementos interativos usam raio pílula `{rounded.pill}` (`999px`) — botão primário preto, secundário branco, chips de categoria em cinza suave e botões de ação nas linhas.
- **Tipografia Funcional (UberMove / UberMoveText):**
  - Títulos em peso 700 (`display-*`), sempre em *sentence-case* (nunca em all-caps gritante), com entrelinha justa (1.22 a 1.25) e sem tracking artificial.
  - Corpo de texto, metadados e botões em peso 400 e 500 (`body-*`), legíveis e neutros.
- **Hierarquia de Formas Limpa:**
  - Cards e containers principais: `{rounded.xl}` `16px`.
  - Campos de entrada e busca: `{rounded.md}` `8px` ou pílula `999px`.
  - Pílulas interativas: `{rounded.pill}` `999px`.
- **Zero Acentos Cromáticos:** Não há paleta terciária de arco-íris. Toda a expressividade é transmitida por peso tipográfico, espaçamento em múltiplos de 4px e inversão de polaridade (black bands).

---

## 2. Paleta de Tokens (Cores & Superfícies)

### Cores Principais & Superfícies
| Token | Hex / Valor | Aplicação no DevHub |
|---|---|---|
| `colors.primary` / `ink` | `#000000` | Botão primário (`+ Novo Projeto`), texto principal, faixas de destaque |
| `colors.canvas` | `#ffffff` | Fundo principal da aplicação e cards elevados |
| `colors.canvas-soft` | `#efefef` | Fundo de chips de filtro inativos, inputs e botões sutis |
| `colors.canvas-softer`| `#f6f6f6` | Fundo de linhas alternadas da tabela e do painel lateral |
| `colors.surface-pressed`| `#e2e2e2` | Estado de clique e hover de chips cinzas |
| `colors.black-elevated`| `#1a1a1a` / `#282828` | Hover de botões pretos e cartões de polaridade invertida |
| `colors.hairline` | `#e5e5e5` | Divisores sutis de 1px |

### Cores de Texto
| Token | Hex / Valor | Aplicação |
|---|---|---|
| `colors.ink` | `#000000` | Títulos, rótulos de colunas e texto de alta ênfase |
| `colors.body` | `#5e5e5e` | Texto secundário, descrições, caminhos de diretório |
| `colors.mute` | `#afafaf` | Placeholders de busca, textos desabilitados, fine print |
| `colors.on-dark` | `#ffffff` | Todo texto sobre fundo preto (botão primário, tooltips pretas) |

---

## 3. Tipografia & Escala

| Token | Família | Tamanho | Peso | Line-Height | Uso |
|---|---|---|---|---|---|
| `typography.display-xl` | Inter / UberMove | 32px | 700 | 40px | Título principal "Workspace de projetos" |
| `typography.display-md` | Inter / UberMove | 22px | 700 | 28px | Título do projeto no Side Peek |
| `typography.button-md` | Inter / UberMoveText | 14px | 500 | 20px | Rótulos de botões primários e secundários |
| `typography.body-md-strong` | Inter / UberMoveText | 13px | 600 | 18px | Nomes de projetos na tabela e cabeçalhos |
| `typography.body-sm` | Inter / UberMoveText | 12px | 400 | 18px | Dados de tabela, caminhos e categorias |
| `typography.caption-mono` | JetBrains / Consolas | 11px | 500 | 16px | Caminhos no disco e flags técnicas |

---

## 4. Sistema de Formas & Elevação

| Token | Raio | Elevação | Uso |
|---|---|---|---|
| `{rounded.pill}` | `999px` | Flat / Level 0 | Botões (`+ Novo`, `IDE`, `Terminal`, `Explorer`), chips de filtro |
| `{rounded.xl}` | `16px` | Level 1: `rgba(0,0,0,0.06) 0px 4px 16px` | Container da tabela de projetos e Side Peek drawer |
| `{rounded.md}` | `8px` | Flat | Campo de pesquisa e caixas de anotação técnica |

---

## 5. Mapeamento de Componentes Qt6 Widgets

### 1. Botão Primário (`button-primary`)
- `background-color: #000000; color: #ffffff; border-radius: 999px; padding: 8px 20px; font-weight: 500; font-size: 13px;`
- Hover: `background-color: #282828;`

### 2. Chip de Categoria / Filtro (`category-button`)
- Inativo: `background-color: #efefef; color: #000000; border-radius: 999px; padding: 6px 16px; font-size: 12px; font-weight: 500; border: none;`
- Hover: `background-color: #e2e2e2;`
- Ativo: `background-color: #000000; color: #ffffff;`

### 3. Botões de Ação na Tabela (`button-subtle-sm`)
- Pílula cinza suave: `background-color: #efefef; color: #000000; border-radius: 999px; padding: 4px 12px; font-size: 11px; font-weight: 500;`
- Hover: `background-color: #000000; color: #ffffff;`

### 4. Container da Tabela (`card-elevated`)
- `background-color: #ffffff; border: 1px solid #e5e5e5; border-radius: 16px;`
- Cabeçalhos: `background-color: #ffffff; color: #5e5e5e; font-size: 11px; font-weight: 600; text-transform: uppercase; padding: 10px 14px; border-bottom: 1px solid #e5e5e5;`
- Linhas: 42px de altura, seleção suave em `#f6f6f6`.
