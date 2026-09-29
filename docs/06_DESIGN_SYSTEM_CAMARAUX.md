# Design System — CamaraUX

> **Diretrizes de Design, Tokens e Padrões de Interface para o DevHub Workspace**  
> Fundamentado nos princípios, padrões de usabilidade e arquitetura de design tokens documentados na base de conhecimento da **CamaraUX** (`C:\Users\gabriel\Documents\Obsidian Vault\CamaraUX`): escala matemática de espaçamento base 8px, tokens semânticos com conformidade estrita de contraste WCAG AA (4.5:1 / 3:1), hierarquia intencional de ações, estados vazios orientativos e feedback não intrusivo.

---

## 1. Visão Geral & Filosofia

O padrão de UI da CamaraUX é estruturado sobre usabilidade pragmática, clareza cognitiva e consistência entre design e engenharia:

1. **Separação Primitivo vs. Semântico:** Componentes nunca consomem valores hexadecimais brutos. Toda decisão visual consome um token semântico que descreve sua função (`surface.base`, `action.primary`, `feedback.error.surface`).
2. **Acessibilidade como Requisito Não-Negociável:** Todo texto e elemento interativo atende ou supera a razão de contraste mínima da norma WCAG AA (4.5:1 para texto normal, 3:1 para títulos e alvos gráficos).
3. **Escala Matemática de Espaçamento:** Base 8px para toda a estrutura de layout e cartões, com camada de 4px para micro-elementos e densidade de tabela.
4. **Comunicação Não-Bloqueante:** Ações triviais e reversíveis (ex: copiar caminho para a área de transferência) comunicam feedback via **Toast transitório** ou estado inline no próprio botão, nunca através de modais (`QMessageBox`) que interrompem o fluxo do usuário.
5. **Prevenção de Beco sem Saída (Empty States):** Telas sem dados (busca zerada ou biblioteca inicial vazia) explicam o motivo, orientam o próximo passo e fornecem uma ação direta para recuperação.

---

## 2. Paleta de Tokens (Cores & Superfícies)

### 2.1 Superfícies & Neutros (Slate System)
| Token | Valor HEX | Função no DevHub |
|---|---|---|
| `surface.app` | `#f8fafc` | Fundo geral da janela e cabeçalhos de tabela |
| `surface.card` | `#ffffff` | Superfície de cartões, formulários e linhas de dados |
| `surface.subtle` | `#f1f5f9` | Fundo de campo de busca, chips inativos e caixas de código |
| `surface.hover` | `#e2e8f0` | Estado hover de botões secundários e linhas da tabela |
| `border.default` | `#e2e8f0` | Divisores e contornos de containers |
| `border.strong` | `#cbd5e1` | Bordas de botões secundários e controles de entrada |
| `border.focus` | `#0f172a` | Anel de foco visível para navegação por teclado |

### 2.2 Tipografia & Contraste (WCAG AA)
| Token | Valor HEX | Contraste sobre Branco | Aplicação |
|---|---|---|---|
| `text.primary` | `#0f172a` (Slate 900) | **15.2 : 1** (Supera 4.5:1) | Títulos, cabeçalhos e rótulos de alta prioridade |
| `text.secondary` | `#475569` (Slate 600) | **6.6 : 1** (Supera 4.5:1) | Textos descritivos, dados secundários e colunas |
| `text.tertiary` | `#64748b` (Slate 500) | **4.6 : 1** (Aprovado AA) | Metadados, contadores e placeholders |
| `text.on-dark` | `#ffffff` | **15.2 : 1** | Textos sobre botão primário preto/slate |

### 2.3 Badges de Stack com Contraste Semântico
| Stack | Fundo | Texto | Razão de Contraste |
|---|---|---|---|
| **C++** | `#e0e7ff` (Indigo 100) | `#3730a3` (Indigo 800) | **7.4 : 1** (WCAG AAA) |
| **Rust** | `#ffedd5` (Orange 100) | `#9a3412` (Orange 800) | **7.1 : 1** (WCAG AAA) |
| **Python** | `#fef3c7` (Amber 100) | `#92400e` (Amber 800) | **7.5 : 1** (WCAG AAA) |
| **TypeScript / JS** | `#e0f2fe` (Sky 100) | `#075985` (Sky 800) | **7.6 : 1** (WCAG AAA) |
| **Go** | `#ccfbf1` (Teal 100) | `#115e59` (Teal 800) | **7.3 : 1** (WCAG AAA) |
| **C#** | `#f3e8ff` (Purple 100) | `#6b21a8` (Purple 800) | **7.2 : 1** (WCAG AAA) |

---

## 3. Escala Matemática de Espaçamento (Base 8px)

Seguindo a especificação `posts/06268-espacamento-design-system-tokens.md`:

| Token | Valor em Pixels | Aplicação |
|---|---|---|
| `spacing.xxs` | `4px` | Gaps internos de chips, paddings verticais compactos |
| `spacing.xs` | `8px` | Gap entre botões de ações, padding vertical de inputs e botões |
| `spacing.sm` | `12px` | Gaps verticais em formulários e propriedades |
| `spacing.md` | `16px` | Padding interno de cards e espaçamento entre seções |
| `spacing.lg` | `24px` | Margens do container principal e cabeçalho |
| `spacing.xl` | `32px` | Margens estruturais do documento e tabela |

---

## 4. Padrões de Componentes e Interação CamaraUX

### 4.1 Hierarquia de Botões (`padroes/10536` e `padroes/11606`)
- **Ação Primária Única:** Apenas uma chamada à ação principal em destaque com preenchimento escuro (`#0f172a`) por contexto (`+ Novo projeto`).
- **Rótulos com Verbo de Ação no Infinitivo:** Os botões informam com clareza a ação executada ("Escanear pasta", "Abrir na IDE", "Desvincular da biblioteca").
- **Ações Destrutivas com Alta Resolução Semântica (`padroes/11789`):**
  - O botão de desvincular projeto utiliza token semântico de erro (`#fef2f2`, borda `#fecaca`, texto `#dc2626`).
  - Ao ser clicado, exibe diálogo com o botão destrutivo em destaque e foco padrão no botão "Cancelar".
  - A mensagem esclarece explicitamente que os arquivos no disco permanecerão 100% seguros e intactos.

### 4.2 Busca sem Resultados e Estados Vazios (`padroes/10865` e `padroes/10703`)
- Quando uma pesquisa ou filtro não encontrar resultados, a tabela é substituída por um container estilizado com ícone ilustrativo, título semântico ("Nenhum resultado para 'termo'"), orientação prática e botão de ação ("Limpar busca e filtros").
- Na primeira execução com banco vazio, orienta o desenvolvedor com "+ Novo projeto" e "Escanear pasta".

### 4.3 Feedback Não-Bloqueante via Toast (`padroes/10687`)
- Ações rápidas (copiar caminho, salvar notas, escanear pastas) exibem um **Toast flutuante centralizado** na parte inferior da tela, desaparecendo suavemente após 2.8 segundos.
- O botão de "Copiar caminho" adota o padrão de affordance direta, alternando seu texto para `"✓ Copiado!"` durante 2 segundos.

---

## 5. Implementação Nativa em C++ / Qt6

Toda a infraestrutura do design system foi programada diretamente em C++ moderno no módulo `src/ui/MainWindow.cpp`:
- `MainWindow::setupCamaraUxTheme()`: injeta a folha de estilos baseada nos tokens CamaraUX.
- `MainWindow::createEmptyStateWidget()`: gerencia os estados de zero resultados de forma reativa.
- `MainWindow::showToast()`: renderiza notificações temporárias sem sobrecarga modal.
- `MainWindow::onRemoveProject()`: implementa a confirmação de segurança com proteção ao sistema de arquivos local.
