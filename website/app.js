// ==========================================================================
// CamaraUX Interações e Comportamentos — DevHub Workspace
// Feedback Não-Bloqueante (Toasts), Busca em Tempo Real, Empty States e Filtros
// ==========================================================================

document.addEventListener('DOMContentLoaded', () => {
  // ------------------------------------------------------------------------
  // 1. Sistema de Toast Flutuante Não-Bloqueante (CamaraUX Padrão 10687)
  // ------------------------------------------------------------------------
  const toastEl = document.getElementById('camarauxToast');
  const toastMsg = document.getElementById('toastMessage');
  const toastIcon = document.getElementById('toastIcon');
  let toastTimer = null;

  function showToast(message, isSuccess = true) {
    if (!toastEl || !toastMsg) return;

    if (toastTimer) {
      clearTimeout(toastTimer);
    }

    toastMsg.textContent = message;

    if (toastIcon) {
      if (isSuccess) {
        toastIcon.innerHTML = `<svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="#10b981" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"><polyline points="20 6 9 17 4 12"></polyline></svg>`;
      } else {
        toastIcon.innerHTML = `<svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="#ffffff" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><line x1="12" y1="8" x2="12" y2="12"></line><line x1="12" y1="16" x2="12.01" y2="16"></line></svg>`;
      }
    }

    toastEl.classList.add('visible');

    toastTimer = setTimeout(() => {
      toastEl.classList.remove('visible');
    }, 2800);
  }

  // ------------------------------------------------------------------------
  // 2. Copiar Checksum SHA-256
  // ------------------------------------------------------------------------
  const hashElement = document.getElementById('shaHash');
  if (hashElement) {
    hashElement.addEventListener('click', () => {
      const fullHash = hashElement.getAttribute('data-hash') || hashElement.innerText.trim();
      navigator.clipboard.writeText(fullHash).then(() => {
        const originalText = hashElement.innerText;
        hashElement.innerText = "✓ Copiado!";
        showToast("Checksum SHA-256 completo copiado com sucesso!");
        setTimeout(() => {
          hashElement.innerText = originalText;
        }, 2000);
      }).catch(() => {
        showToast("Checksum SHA-256: " + fullHash.substring(0, 16) + "...", false);
      });
    });
  }

  // ------------------------------------------------------------------------
  // 3. Copiar Comandos Terminal / PowerShell
  // ------------------------------------------------------------------------
  const copyCmdBtn = document.getElementById('copyCmdBtn');
  if (copyCmdBtn) {
    copyCmdBtn.addEventListener('click', () => {
      const cmdText = "git clone https://github.com/GabrielAraujodev/devhub.git\ncd devhub\n.\\run.ps1";
      navigator.clipboard.writeText(cmdText).then(() => {
        const label = copyCmdBtn.querySelector('span');
        if (label) label.innerText = "Copiado!";
        showToast("Comandos de compilação C++ copiados para a área de transferência!");
        setTimeout(() => {
          if (label) label.innerText = "Copiar";
        }, 2000);
      });
    });
  }

  // ------------------------------------------------------------------------
  // 4. Mockup Interativo: Busca em Tempo Real e Chips Multi-Stack
  // ------------------------------------------------------------------------
  const searchInput = document.getElementById('mockupSearchInput');
  const chips = document.querySelectorAll('.mockup-chip');
  const table = document.getElementById('mockupTable');
  const rows = document.querySelectorAll('#mockupTableBody tr');
  const emptyState = document.getElementById('mockupEmptyState');
  const emptyTitle = document.getElementById('emptyTitle');
  const emptyDesc = document.getElementById('emptyDesc');
  const emptyResetBtn = document.getElementById('emptyResetBtn');

  // Elementos do Inspetor Lateral
  const inspName = document.getElementById('inspectorName');
  const inspPath = document.getElementById('inspectorPath');
  const inspStack = document.getElementById('inspectorStack');
  const inspBuild = document.getElementById('inspectorBuild');
  const inspStandard = document.getElementById('inspectorStandard');
  const inspAi = document.getElementById('inspectorAi');
  const inspFramework = document.getElementById('inspectorFramework');
  const inspDate = document.getElementById('inspectorDate');
  const inspNotes = document.getElementById('inspectorNotes');
  const inspBtnAi = document.getElementById('inspectorBtnAi');
  const inspBtnIde = document.getElementById('inspectorBtnIde');
  const inspBtnTerminal = document.getElementById('inspectorBtnTerminal');
  const inspBtnCopy = document.getElementById('inspectorBtnCopy');

  let activeFilter = 'ALL';

  function applyFilters() {
    const query = searchInput ? searchInput.value.trim().toLowerCase() : '';
    let visibleCount = 0;

    rows.forEach(row => {
      const stack = row.getAttribute('data-stack') || '';
      const name = (row.getAttribute('data-name') || '').toLowerCase();
      const path = (row.getAttribute('data-path') || '').toLowerCase();
      const notes = (row.getAttribute('data-notes') || '').toLowerCase();
      const standard = (row.getAttribute('data-standard') || '').toLowerCase();
      const ai = (row.getAttribute('data-ai') || '').toLowerCase();

      const matchesFilter = (activeFilter === 'ALL' || stack === activeFilter);
      const matchesSearch = query === '' ||
        name.includes(query) ||
        stack.toLowerCase().includes(query) ||
        path.includes(query) ||
        notes.includes(query) ||
        standard.includes(query) ||
        ai.includes(query);

      if (matchesFilter && matchesSearch) {
        row.style.display = '';
        visibleCount++;
      } else {
        row.style.display = 'none';
      }
    });

    // Padrão CamaraUX 10865: Tratar estado vazio sem beco de saída
    if (visibleCount === 0) {
      if (table) table.style.display = 'none';
      if (emptyState) {
        emptyState.style.display = 'flex';
        if (query) {
          emptyTitle.textContent = `Nenhum projeto encontrado para "${query}"`;
          emptyDesc.textContent = `Nenhum repositório corresponde aos critérios com o filtro ${activeFilter}. Limpe o termo para restaurar.`;
        } else {
          emptyTitle.textContent = `Nenhum projeto com stack ${activeFilter}`;
          emptyDesc.textContent = `Não há repositórios ${activeFilter} cadastrados nesta visualização.`;
        }
      }
    } else {
      if (table) table.style.display = '';
      if (emptyState) emptyState.style.display = 'none';
    }
  }

  // Filtragem por Chips
  chips.forEach(chip => {
    chip.addEventListener('click', () => {
      chips.forEach(c => c.classList.remove('active'));
      chip.classList.add('active');
      activeFilter = chip.getAttribute('data-filter') || 'ALL';
      applyFilters();
    });
  });

  // Busca por Texto em Tempo Real
  if (searchInput) {
    searchInput.addEventListener('input', applyFilters);
  }

  // Botão Limpar busca e filtros (CamaraUX Padrão 10703)
  if (emptyResetBtn) {
    emptyResetBtn.addEventListener('click', () => {
      if (searchInput) searchInput.value = '';
      activeFilter = 'ALL';
      chips.forEach(c => {
        if (c.getAttribute('data-filter') === 'ALL') {
          c.classList.add('active');
        } else {
          c.classList.remove('active');
        }
      });
      applyFilters();
      showToast("Filtros restaurados para visualização geral.");
    });
  }

  // ------------------------------------------------------------------------
  // 5. Seleção de Linhas e Atualização do Inspetor Lateral
  // ------------------------------------------------------------------------
  function selectRow(row) {
    rows.forEach(r => r.classList.remove('selected-row'));
    row.classList.add('selected-row');

    const name = row.getAttribute('data-name') || '';
    const path = row.getAttribute('data-path') || '';
    const stack = row.getAttribute('data-stack') || '';
    const build = row.getAttribute('data-build') || '';
    const standard = row.getAttribute('data-standard') || '';
    const framework = row.getAttribute('data-framework') || '';
    const ai = row.getAttribute('data-ai') || 'Claude Code';
    const date = row.getAttribute('data-date') || '';
    const notes = row.getAttribute('data-notes') || '';

    if (inspName) inspName.textContent = name;
    if (inspPath) inspPath.textContent = path;
    if (inspBuild) inspBuild.textContent = build;
    if (inspStandard) inspStandard.textContent = standard;
    if (inspFramework) inspFramework.textContent = framework;
    if (inspDate) inspDate.textContent = date;
    if (inspNotes) inspNotes.textContent = notes;
    if (inspAi) {
      inspAi.innerHTML = `<span class="badge-ai-tool">${ai}</span>`;
    }

    if (inspStack) {
      const badgeClass = `stack-${stack.toLowerCase().replace('++', 'cpp').replace('#', 'sharp')}`;
      inspStack.innerHTML = `<span class="stack-badge ${badgeClass}">${stack}</span>`;
    }
  }

  rows.forEach(row => {
    row.addEventListener('click', (e) => {
      // Se não clicou em um botão de ação rápida dentro da linha
      if (!e.target.closest('button')) {
        selectRow(row);
      }
    });
  });

  // Ações do Inspetor Lateral
  if (inspBtnAi) {
    inspBtnAi.addEventListener('click', () => {
      const name = inspName ? inspName.textContent : 'projeto';
      const aiTool = inspAi ? inspAi.textContent.trim() : 'Claude Code';
      showToast(`🤖 Lançando agente ${aiTool} no terminal em ${name}...`);
    });
  }

  if (inspBtnIde) {
    inspBtnIde.addEventListener('click', () => {
      const name = inspName ? inspName.textContent : 'projeto';
      showToast(`Abrindo ${name} no VS Code...`);
    });
  }

  if (inspBtnTerminal) {
    inspBtnTerminal.addEventListener('click', () => {
      const path = inspPath ? inspPath.textContent : '';
      showToast(`Abrindo terminal no diretório: ${path}`);
    });
  }

  if (inspBtnCopy) {
    inspBtnCopy.addEventListener('click', () => {
      const path = inspPath ? inspPath.textContent : '';
      navigator.clipboard.writeText(path).then(() => {
        showToast(`Caminho copiado: ${path}`);
      });
    });
  }

  // Ações Rápidas na Tabela
  document.querySelectorAll('.action-open-ide').forEach(btn => {
    btn.addEventListener('click', (e) => {
      e.stopPropagation();
      const proj = btn.getAttribute('data-proj') || 'projeto';
      showToast(`Lançando ${proj} na IDE configurada...`);
    });
  });

  document.querySelectorAll('.action-copy-path').forEach(btn => {
    btn.addEventListener('click', (e) => {
      e.stopPropagation();
      const path = btn.getAttribute('data-path') || '';
      navigator.clipboard.writeText(path).then(() => {
        const originalText = btn.innerText;
        btn.innerText = "✓ Copiado";
        showToast(`Caminho copiado: ${path}`);
        setTimeout(() => {
          btn.innerText = originalText;
        }, 1800);
      });
    });
  });

  // Botão "+ Novo projeto" no Mockup
  const mockupAddBtn = document.getElementById('mockupAddBtn');
  if (mockupAddBtn) {
    mockupAddBtn.addEventListener('click', () => {
      showToast("Abrindo assistente nativo de novo projeto (C++, Rust, Python, Go)...");
    });
  }

  // Feedback nos links de download
  const mainDownloadBtn = document.getElementById('mainDownloadBtn');
  if (mainDownloadBtn) {
    mainDownloadBtn.addEventListener('click', () => {
      showToast("Iniciando download do pacote portátil DevHub v1.1.0 (33.8 MB)...");
    });
  }

  const exeDownloadBtn = document.getElementById('exeDownloadBtn');
  if (exeDownloadBtn) {
    exeDownloadBtn.addEventListener('click', () => {
      showToast("Iniciando download do binário DevHubCpp.exe (370 KB)...");
    });
  }

  // ------------------------------------------------------------------------
  // 7. Alternador de Modo Claro / Modo Escuro (CamaraUX Slate Dark Tokens)
  // ------------------------------------------------------------------------
  const themeToggleBtn = document.getElementById('themeToggleBtn');
  const storedTheme = localStorage.getItem('devhub-theme');
  const prefersDark = window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches;

  if (storedTheme === 'dark' || (!storedTheme && prefersDark)) {
    document.documentElement.setAttribute('data-theme', 'dark');
    if (themeToggleBtn) {
      themeToggleBtn.querySelector('.theme-icon').textContent = '☀️';
      themeToggleBtn.setAttribute('title', 'Alternar para Modo Claro');
    }
  }

  if (themeToggleBtn) {
    themeToggleBtn.addEventListener('click', () => {
      const current = document.documentElement.getAttribute('data-theme');
      if (current === 'dark') {
        document.documentElement.removeAttribute('data-theme');
        localStorage.setItem('devhub-theme', 'light');
        themeToggleBtn.querySelector('.theme-icon').textContent = '🌙';
        themeToggleBtn.setAttribute('title', 'Alternar para Modo Escuro');
        showToast("☀️ Modo Claro ativado");
      } else {
        document.documentElement.setAttribute('data-theme', 'dark');
        localStorage.setItem('devhub-theme', 'dark');
        themeToggleBtn.querySelector('.theme-icon').textContent = '☀️';
        themeToggleBtn.setAttribute('title', 'Alternar para Modo Claro');
        showToast("🌙 Modo Escuro ativado (CamaraUX Slate)");
      }
    });
  }
});
