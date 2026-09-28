# Design System — xAI Engineered Restraint

> **Diretrizes de Design e Tokens para o DevHub C++**  
> Inspirado na estética visual do laboratório de IA de fronteira xAI: engineered restraint, near-black canvas `#0a0a0a`, outline pills como vocabulário interativo universal e tipografia disciplinada sem ruído nem "AI slop".

---

## 1. Visão Geral & Filosofia

O site e as interfaces da **xAI** vestem uma postura de contenção de engenharia (*engineered restraint*):
- **Superfície Única:** Um canvas quase negro `{colors.canvas}` (`#0a0a0a`) de ponta a ponta.
- **Vocabulário Interativo Universal:** Pílulas com contorno branco translúcido (`{rounded.pill}` `9999px`) em absolutamente todos os elementos interativos.
- **Tipografia em Dois Tons:**
  1. *Universal Sans* (ou *Inter* / *Geist* como substitutos abertos) em peso 400 com tracking negativo agressivo (`-1.2px` a `-2.4px`) para display e títulos.
  2. *Geist Mono* (ou *JetBrains Mono*) em caixa alta com tracking positivo (`+1.2px` a `+1.4px`) para eyebrows de seção, contadores de métricas e tags técnicas.
- **Zero AI Slop:** Sem gradientes roxos genéricos, sem cards cartoonizados, sem sombras pesadas ou ilustrações falsas. A elevação é definida unicamente por bordas capilares de 1px (*hairlines* `#212327` ou `rgba(255, 255, 255, 0.15–0.25)`).

---

## 2. Paleta de Tokens (Cores & Superfícies)

### Superfícies & Fundo
| Token | Hex / Valor | Aplicação no DevHub C++ |
|---|---|---|
| `colors.canvas` | `#0a0a0a` | Fundo principal da janela e viewport |
| `colors.canvas-card` | `#141414` / `#191919` | Fundo da tabela de projetos e do Side Peek drawer |
| `colors.canvas-soft` | `#1a1c20` | Campo de busca, inputs e estados de hover sutil |
| `colors.canvas-mid` | `#26292e` | Caixas de código mono e visualizador de caminhos |
| `colors.hairline` | `#212327` | Divisores e bordas estruturais de 1px |
| `colors.hairline-translucent`| `rgba(255, 255, 255, 0.20)` | Bordas dos botões outline pill |

### Texto & Tinta
| Token | Hex / Valor | Aplicação |
|---|---|---|
| `colors.primary` / `ink` | `#ffffff` | Títulos, rótulos de botões ativos, texto principal |
| `colors.body` | `#dadbdf` | Nomes de projetos, texto padrão de leitura |
| `colors.body-mid` / `mute` | `#7d8187` | Caminhos de diretório, cabeçalhos de tabela, eyebrows |
| `colors.on-primary` | `#0a0a0a` | Texto dentro do botão primário preenchido em branco |

### Acentos Sutis (Tags & Identificadores Técnicos)
| Token | Hex | Aplicação Técnica no DevHub |
|---|---|---|
| `colors.accent-sunset` | `#ff7a17` | Indicador de build `CMake` |
| `colors.accent-twilight` | `#c4b5fd` | Indicador de padrão `C++20` / `C++23` |
| `colors.accent-breeze` | `#a0c3ec` | Indicador de framework `Qt6` / `Qt5` |
| `colors.accent-dusk` | `#7c3aed` | Acento de categoria `Open Source` |
| `colors.accent-gold` | `#dfab01` | Estrela de favorito ativo (★) |

---

## 3. Tipografia & Escala

| Token | Família | Tamanho | Peso | Letter-Spacing | Uso |
|---|---|---|---|---|---|
| `typography.display-lg` | Inter / Geist | 28px | 400 | -1.2px | Título principal "PROJETOS C++" |
| `typography.display-sm` | Inter / Geist | 20px | 400 | -0.6px | Título do projeto no Side Peek |
| `typography.caption-mono` | Geist Mono | 11px | 400 | +1.4px | Eyebrow de seção (UPPERCASE) |
| `typography.body-md` | Inter / Geist | 13px | 400 | 0px | Dados da tabela e corpo de texto |
| `typography.code-mono` | Geist Mono | 12px | 400 | 0px | Caminhos no disco e comandos |
| `typography.button-md` | Geist Mono / Inter | 12px | 400 | +0.4px | Texto das pílulas de ação |

---

## 4. Sistema de Formas & Bordas

| Token | Raio | Uso |
|---|---|---|
| `rounded.pill` | `9999px` | **Todos os botões interativos** (`+ Novo`, `IDE`, `Terminal`, `Explorer`, abas de filtro) |
| `rounded.sm` | `8px` | Container da tabela, gaveta de Side Peek, inputs |
| `rounded.none` | `0px` | Divisores e bordas estruturais de 1px |

---

## 5. Mapeamento de Componentes Qt6 Widgets

### 1. Botão Primário (`button-primary`)
- `background: #ffffff; color: #0a0a0a; border: 1px solid #ffffff; border-radius: 9999px; font-weight: 500;`
- Usado exclusivamente na ação principal: `+ NOVO PROJETO`.

### 2. Botão Secundário / Ações Rápidas (`button-outline-on-dark`)
- `background: transparent; color: #ffffff; border: 1px solid rgba(255, 255, 255, 0.22); border-radius: 9999px;`
- Hover: `background: rgba(255, 255, 255, 0.08); border-color: rgba(255, 255, 255, 0.5);`
- Usado em: `VS CODE`, `TERMINAL`, `EXPLORER`, `COPIAR CAMINHO`, filtros de categoria.

### 3. Tabela de Projetos (`card-content`)
- Container: `background: #141414; border: 1px solid #212327; border-radius: 8px;`
- Cabeçalhos: `background: #101010; color: #7d8187; font-family: 'Geist Mono', monospace; text-transform: uppercase; font-size: 11px; letter-spacing: 1px;`
- Linhas: altura de 42px, linhas alternadas discretas, seleção em `rgba(255, 255, 255, 0.05)`.

### 4. Eyebrow de Seção (`eyebrow-mono`)
- `// LOCAL REPOSITORIES INDEXED · C++ WORKSPACE`
- `color: #7d8187; font-family: 'Geist Mono', monospace; font-size: 11px; letter-spacing: 1.4px; text-transform: uppercase;`
