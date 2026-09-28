# Design System DevHub C++ — Diretrizes de Design Notion
**Metodologia HOUS3 | Engenharia de Interface**  
**Conceito:** *"Uma mesa de trabalho perfeitamente organizada sob iluminação natural."*

---

## 1. Filosofia Visual e Princípios de Aplicação

O **DevHub C++** adota a linguagem de design do **Notion**:
1. **Calma do Papel Quente:** A tela dominante nunca é o branco clínico (#ffffff), mas o off-white suave e aquecido `{colors.canvas-soft}` (`#f6f5f4`). Isso transforma a aplicação desktop em um documento relaxante, sem cansaço visual.
2. **Monocromático com Apenas Um Acento Estrutural:** Toda a estrutura, bordas e tipografia sussurram em pretos e cinzas. Apenas uma cor comanda as ações principais (CTAs, links, estados ativos): o **Notion Blue** (`#0075de`).
3. **Paleta Lúdica de Adesivos (Stickers):** Para trazer a identidade de C++ (CMake, Qt, MSVC, Ninja, Clang) sem poluir a interface, as cores vivas atuam **exclusivamente como adesivos decorativos** (badges, category dots e ícones ilustrativos). **NUNCA** são usadas para pintar botões de ação ou estruturas de layout.
4. **Elevação por Fio de Cabelo (Hairline) e Sombra Mínima:** Em vez de sombras pesadas ou drop-shadows artificiais, as superfícies usam bordas de 1px com blend sólido (`#e6e6e6`) e sombras multicamadas quase imperceptíveis.
5. **Tipografia com Tracking Negativo Apertado:** Títulos em **Inter** com peso 700 e tracking negativo agressivo, conferindo personalidade confiante e editorial.

---

## 2. Tokens de Cor (Design Tokens)

### 2.1 Cores de Estrutura e Marca
| Token | Valor Hex | Uso no DevHub C++ |
|---|---|---|
| `colors.primary` | `#0075de` | Ação primária ("+ Adicionar Projeto"), links, foco, filtro ativo |
| `colors.primary-active` | `#005bab` | Estado pressionado do botão primário |
| `colors.secondary` | `#213183` | Índigo profundo para destaque de hero/banner ou status noturno |

### 2.2 Superfícies e Linhas
| Token | Valor Hex | Uso no DevHub C++ |
|---|---|---|
| `colors.canvas-soft` | `#f6f5f4` | Fundo principal da aplicação e barras utilitárias |
| `colors.surface` | `#ffffff` | Cards de projetos, modais, campos de texto, gavetas |
| `colors.hairline` | `#e6e6e6` | Bordas de 1px de cards, divisórias e inputs |
| `colors.hairline-focus` | `#0075de` | Anel de foco em campos de formulário e cards selecionados |

### 2.3 Tipografia (Escala de Tinta)
| Token | Valor Hex / Alpha | Uso no DevHub C++ |
|---|---|---|
| `colors.ink` | `#000000` (95% opacidade) | Títulos principais de projetos e cabeçalhos |
| `colors.ink-secondary` | `#31302e` | Caminhos de diretórios, labels de propriedades |
| `colors.ink-muted` | `#615d59` | Textos de apoio, contadores, timestamps |
| `colors.ink-faint` | `#a39e98` | Placeholders de busca, atalhos de teclado |

### 2.4 Paleta de Adesivos Técnicos (Badges Decorativas)
*Uso restrito a tags técnicas, chips de compiladores e categorias:*
- **Sticker Sky** (`#62aef0`): C++20 / C++23 / Padrão da Linguagem
- **Sticker Purple** (`#d6b6f6` / `#391c57`): Framework Qt / QML
- **Sticker Orange** (`#dd5b00` / `#793400`): CMake / CMakeLists.txt
- **Sticker Teal** (`#2a9d99`): Visual Studio / MSVC / .sln
- **Sticker Green** (`#1aae39`): Ninja / Status Ativo / Repositório Limpo
- **Sticker Pink** (`#ff64c8`): Clang / LLVM / Open Source

---

## 3. Tipografia (Inter com Ajuste Editorial)

| Token | Tamanho | Peso | Line-Height | Tracking | Aplicação no DevHub |
|---|---|---|---|---|---|
| `typography.display-1` | 48px | 700 | 1.05 | -1.5px | Título do Dashboard ("Seus Projetos C++") |
| `typography.heading-1` | 32px | 700 | 1.1 | -0.8px | Títulos de Seções |
| `typography.heading-2` | 24px | 700 | 1.2 | -0.5px | Título do Projeto no Card |
| `typography.heading-3` | 18px | 600 | 1.25 | -0.2px | Títulos de Modais e Painéis |
| `typography.body-md` | 15px | 400 | 1.5 | 0 | Textos e descrições |
| `typography.body-sm` | 13px | 400 | 1.4 | 0 | Caminhos de pastas, metadados |
| `typography.code` | 12px | 500 | 1.4 | 0 | Monospace para flags e paths |
| `typography.button` | 14px | 500 | 1.4 | 0 | Rótulos de botões |
| `typography.eyebrow` | 11px | 600 | 1.3 | +0.2px | Badges de status e categorias |

---

## 4. Raios de Borda (Shapes)

- **Cards e Painéis:** `12px` (`rounded.lg`)
- **Botões Utilitários (Nav, Ações Rápidas, Filtros):** `8px` (`rounded.md`)
- **Botão Primário de Ação (CTA "+ Novo Projeto"):** `9999px` (`rounded.full` / pílula)
- **Campos de Texto e Busca:** `6px` a `8px` (inputs permanecem mais retos para ergonomia)
- **Badges de Adesivos Técnicos:** `6px` com padding `3px 8px`

---

## 5. Mapeamento de Componentes do DevHub C++

### 5.1 Card de Projeto (`project-card`)
- Superfície: `colors.surface` (`#ffffff`)
- Borda: `1px solid colors.hairline` (`#e6e6e6`)
- Raio: `12px`
- Padding: `20px`
- Elevação: Sombra de microcamadas no hover/ativo
- Conteúdo:
  - Linha superior: Nome do projeto + Estrela de favorito (toggle)
  - Subtítulo: Caminho local abreviado (`C:\dev\engine\...`) com clique para copiar
  - Linha de Badges: Chips coloridos de adesivo (ex: [CMake], [C++20], [Qt])
  - Barra de Ações Rápidas: Botões compactos (8px) "IDE", "Terminal", "Pasta"

### 5.2 Barra de Busca e Filtros
- Superfície do container: integrada ao fundo suave (`colors.canvas-soft`)
- Campo de busca: fundo `#ffffff`, borda hairline, ícone de lupa discreto, atalho visual `Ctrl+K`
- Pílulas de Categoria: botões arredondados com contagem de projetos

### 5.3 Painel de Detalhes do Projeto (Drawer Lateral)
- Superfície: `#ffffff`
- Borda esquerda: `1px solid colors.hairline`
- Ficha técnica completa: Sistema de build, compiladores, data do último acesso
- Editor de notas do desenvolvedor com salvamento instantâneo
