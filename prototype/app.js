// ==========================================================================
// DevHub C++ — Interactive Engine (Notion Clean Architecture)
// ==========================================================================

const STORAGE_KEY = 'DEVHUB_PROJECTS_V3';
const SETTINGS_KEY = 'DEVHUB_SETTINGS_V3';

const INITIAL_PROJECTS = [
  {
    id: 'proj-1',
    name: 'vulkan-render-core',
    path: 'C:\\dev\\graphics\\vulkan-render-core',
    buildSystem: 'CMake',
    cxxStandard: 'C++23',
    frameworks: 'Vulkan, GLFW',
    category: 'Empresa',
    isFavorite: true,
    preferredIde: 'VS Code',
    notes: 'Rodar cmake -B build -GNinja com Vulkan SDK 1.3 configurado no PATH.',
    lastAccessed: 'Hoje às 14:15',
    isOrphan: false
  },
  {
    id: 'proj-2',
    name: 'accounting-forensic-engine',
    path: 'D:\\work\\dds\\forensic-audit-qt',
    buildSystem: 'CMake',
    cxxStandard: 'C++20',
    frameworks: 'Qt 6.7 (Core, Quick)',
    category: 'Empresa',
    isFavorite: true,
    preferredIde: 'Qt Creator',
    notes: 'Engine de auditoria pericial contábil e importação de extratos OFX.',
    lastAccessed: 'Ontem às 19:40',
    isOrphan: false
  },
  {
    id: 'proj-3',
    name: 'fast-json-parser',
    path: 'C:\\dev\\oss\\fast-json-parser',
    buildSystem: 'CMake',
    cxxStandard: 'C++17',
    frameworks: 'Header-only',
    category: 'Open Source',
    isFavorite: false,
    preferredIde: 'VS Code',
    notes: 'Parser ultrarrápido sem alocação dinâmica. Testes unitários com ctest.',
    lastAccessed: '18 Set 2026',
    isOrphan: false
  },
  {
    id: 'proj-4',
    name: 'win32-driver-sandbox',
    path: 'C:\\projects\\legacy\\driver-sandbox',
    buildSystem: 'Visual Studio',
    cxxStandard: 'C++14',
    frameworks: 'Win32, MSVC',
    category: 'Estudos',
    isFavorite: false,
    preferredIde: 'Visual Studio',
    notes: 'Requer WDK 10 instalado para compilar o driver.',
    lastAccessed: '02 Ago 2026',
    isOrphan: true
  }
];

let projects = [];
let currentView = 'table'; // 'table' or 'cards'
let activeCategory = 'ALL';
let searchQuery = '';
let activeProject = null;

document.addEventListener('DOMContentLoaded', () => {
  loadProjects();
  setupUIEvents();
  render();
});

function loadProjects() {
  const saved = localStorage.getItem(STORAGE_KEY);
  if (saved) {
    try {
      projects = JSON.parse(saved);
    } catch (e) {
      projects = [...INITIAL_PROJECTS];
    }
  } else {
    projects = [...INITIAL_PROJECTS];
    saveToStorage();
  }
}

function saveToStorage() {
  localStorage.setItem(STORAGE_KEY, JSON.stringify(projects));
}

function setupUIEvents() {
  const searchInput = document.getElementById('searchInput');

  searchInput.addEventListener('input', (e) => {
    searchQuery = e.target.value.toLowerCase().trim();
    render();
  });

  // Hotkey '/' or 'Ctrl+K'
  document.addEventListener('keydown', (e) => {
    if (e.key === '/' && document.activeElement !== searchInput && !isModalOpen()) {
      e.preventDefault();
      searchInput.focus();
    }
    if (e.key === 'Escape') {
      closeSidePeek();
      closeModal('modalAddProject');
      closeModal('modalSettings');
      searchInput.blur();
    }
  });

  // View Switcher (Table vs Gallery)
  const viewTabs = document.querySelectorAll('.view-tab');
  viewTabs.forEach(tab => {
    tab.addEventListener('click', () => {
      viewTabs.forEach(t => t.classList.remove('active'));
      tab.classList.add('active');
      currentView = tab.dataset.view;
      render();
    });
  });

  // Category Filters
  const filterPills = document.querySelectorAll('.filter-tag-pill');
  filterPills.forEach(pill => {
    pill.addEventListener('click', () => {
      filterPills.forEach(p => p.classList.remove('active'));
      pill.classList.add('active');
      activeCategory = pill.dataset.cat;
      render();
    });
  });

  // Modal triggers
  document.getElementById('btnAddProject').addEventListener('click', openAddProjectModal);
  document.getElementById('btnConfirmAdd').addEventListener('click', handleAddProject);
  document.getElementById('btnBrowseFolder').addEventListener('click', simulateBrowseFolder);
  document.getElementById('inputProjectPath').addEventListener('input', handlePathInputDetection);

  document.getElementById('btnSettings').addEventListener('click', () => openModal('modalSettings'));

  // Side Peek
  document.getElementById('btnClosePeek').addEventListener('click', closeSidePeek);
  document.getElementById('sidePeekBackdrop').addEventListener('click', closeSidePeek);
  document.getElementById('btnCopyJson').addEventListener('click', copyProjectJson);
  document.getElementById('btnDeleteFromLibrary').addEventListener('click', handleRemoveActiveProject);

  // Edit Name in Peek
  document.getElementById('peekProjectName').addEventListener('input', (e) => {
    if (activeProject) {
      activeProject.name = e.target.value;
      saveToStorage();
      renderCurrentItemInList();
    }
  });

  // Notes Autosave
  const notesTextarea = document.getElementById('peekNotesTextarea');
  notesTextarea.addEventListener('input', () => {
    if (activeProject) {
      activeProject.notes = notesTextarea.value;
      saveToStorage();
      const indicator = document.getElementById('autosaveStatus');
      indicator.textContent = 'Salvando...';
      setTimeout(() => {
        indicator.textContent = 'Salvo';
      }, 300);
    }
  });

  // Quick Action Launchers in Side Peek
  document.getElementById('peekBtnIde').addEventListener('click', () => {
    triggerLaunch(activeProject ? activeProject.preferredIde : 'IDE', activeProject);
  });
  document.getElementById('peekBtnTerminal').addEventListener('click', () => {
    triggerLaunch('Terminal', activeProject);
  });
  document.getElementById('peekBtnExplorer').addEventListener('click', () => {
    triggerLaunch('Explorer', activeProject);
  });
}

function isModalOpen() {
  return !document.getElementById('modalAddProject').classList.contains('hidden') ||
         !document.getElementById('modalSettings').classList.contains('hidden') ||
         document.getElementById('sidePeekDrawer').classList.contains('open');
}

// ==========================================================================
// Rendering
// ==========================================================================
function render() {
  const tableContainer = document.getElementById('tableViewContainer');
  const galleryContainer = document.getElementById('galleryViewContainer');
  const emptyState = document.getElementById('emptyState');
  const metaSummary = document.getElementById('metaSummary');

  const filtered = projects.filter(p => {
    if (activeCategory === 'FAV' && !p.isFavorite) return false;
    if (activeCategory !== 'ALL' && activeCategory !== 'FAV' && p.category !== activeCategory) return false;

    if (searchQuery) {
      const q = searchQuery;
      return p.name.toLowerCase().includes(q) ||
             p.path.toLowerCase().includes(q) ||
             p.buildSystem.toLowerCase().includes(q) ||
             (p.cxxStandard || '').toLowerCase().includes(q) ||
             p.category.toLowerCase().includes(q);
    }
    return true;
  });

  metaSummary.textContent = `${projects.length} projetos locais indexados · Armazenamento SQLite local`;

  if (filtered.length === 0) {
    tableContainer.classList.add('hidden');
    galleryContainer.classList.add('hidden');
    emptyState.classList.remove('hidden');
    return;
  }

  emptyState.classList.add('hidden');

  if (currentView === 'table') {
    tableContainer.classList.remove('hidden');
    galleryContainer.classList.add('hidden');
    renderTable(filtered);
  } else {
    tableContainer.classList.add('hidden');
    galleryContainer.classList.remove('hidden');
    renderGallery(filtered);
  }
}

function renderTable(list) {
  const tbody = document.getElementById('tableBody');
  tbody.innerHTML = list.map(p => `
    <tr data-id="${p.id}">
      <td class="td-fav-cell" onclick="event.stopPropagation(); toggleFavorite('${p.id}')">
        <button class="table-star-btn ${p.isFavorite ? 'starred' : ''}">${p.isFavorite ? '★' : '☆'}</button>
      </td>
      <td>
        <div class="table-project-name">
          <span>${escapeHTML(p.name)}</span>
          ${p.isOrphan ? '<span class="orphan-warning-icon" title="Caminho inacessível">⚠️</span>' : ''}
        </div>
      </td>
      <td>${getBuildTagHTML(p.buildSystem)}</td>
      <td>${getCxxTagHTML(p.cxxStandard)}</td>
      <td><span class="notion-tag tag-default">${escapeHTML(p.category)}</span></td>
      <td><div class="table-mono-path" title="${escapeHTML(p.path)}">${escapeHTML(p.path)}</div></td>
      <td onclick="event.stopPropagation()">
        <div class="table-action-strip">
          <button class="btn-row-action" onclick="triggerLaunch('${p.preferredIde}', getProjectById('${p.id}'))">${escapeHTML(p.preferredIde.split(' ')[0])}</button>
          <button class="btn-row-action" onclick="triggerLaunch('Terminal', getProjectById('${p.id}'))">Terminal</button>
          <button class="btn-row-action" onclick="triggerLaunch('Explorer', getProjectById('${p.id}'))">Explorer</button>
        </div>
      </td>
    </tr>
  `).join('');

  // Row click to open Side Peek
  tbody.querySelectorAll('tr').forEach(tr => {
    tr.addEventListener('click', () => openSidePeek(tr.dataset.id));
  });
}

function renderGallery(list) {
  const grid = document.getElementById('galleryGrid');
  grid.innerHTML = list.map(p => `
    <div class="gallery-card" onclick="openSidePeek('${p.id}')">
      <div>
        <div class="gallery-card-top">
          <span class="gallery-card-title">${escapeHTML(p.name)}</span>
          <button class="table-star-btn ${p.isFavorite ? 'starred' : ''}" onclick="event.stopPropagation(); toggleFavorite('${p.id}')">
            ${p.isFavorite ? '★' : '☆'}
          </button>
        </div>
        <div class="gallery-card-path">${escapeHTML(p.path)}</div>
        <div class="gallery-tags-row">
          <span class="notion-tag tag-default">${escapeHTML(p.category)}</span>
          ${getBuildTagHTML(p.buildSystem)}
          ${getCxxTagHTML(p.cxxStandard)}
        </div>
      </div>
      <div class="gallery-card-bottom" onclick="event.stopPropagation()">
        <div class="table-action-strip">
          <button class="btn-row-action" onclick="triggerLaunch('${p.preferredIde}', getProjectById('${p.id}'))">${escapeHTML(p.preferredIde.split(' ')[0])}</button>
          <button class="btn-row-action" onclick="triggerLaunch('Terminal', getProjectById('${p.id}'))">Terminal</button>
        </div>
        <span style="font-size:11px; color:var(--text-secondary);">${escapeHTML(p.lastAccessed)}</span>
      </div>
    </div>
  `).join('');
}

function renderCurrentItemInList() {
  render();
}

function getProjectById(id) {
  return projects.find(p => p.id === id);
}

function getBuildTagHTML(buildSystem) {
  if (buildSystem.includes('CMake')) return `<span class="notion-tag tag-orange">CMake</span>`;
  if (buildSystem.includes('Visual Studio')) return `<span class="notion-tag tag-purple">MSVC</span>`;
  return `<span class="notion-tag tag-default">${escapeHTML(buildSystem)}</span>`;
}

function getCxxTagHTML(std) {
  if (!std) return '';
  return `<span class="notion-tag tag-blue">${escapeHTML(std)}</span>`;
}

// ==========================================================================
// Side Peek Document Drawer
// ==========================================================================
function openSidePeek(id) {
  const proj = getProjectById(id);
  if (!proj) return;

  activeProject = proj;

  document.getElementById('peekProjectName').value = proj.name;
  document.getElementById('peekPath').textContent = proj.path;
  document.getElementById('peekBuildSystemTag').innerHTML = getBuildTagHTML(proj.buildSystem);
  document.getElementById('peekCxxStdTag').innerHTML = getCxxTagHTML(proj.cxxStandard);
  document.getElementById('peekFrameworks').textContent = proj.frameworks || 'Nenhum';
  document.getElementById('peekCategoryTag').innerHTML = `<span class="notion-tag tag-default">${escapeHTML(proj.category)}</span>`;
  document.getElementById('peekIde').textContent = proj.preferredIde;
  document.getElementById('peekNotesTextarea').value = proj.notes || '';
  document.getElementById('autosaveStatus').textContent = 'Salvo';

  document.getElementById('sidePeekDrawer').classList.add('open');
  document.getElementById('sidePeekBackdrop').classList.remove('hidden');
}

function closeSidePeek() {
  document.getElementById('sidePeekDrawer').classList.remove('open');
  document.getElementById('sidePeekBackdrop').classList.add('hidden');
  activeProject = null;
}

function copyProjectJson() {
  if (!activeProject) return;
  navigator.clipboard.writeText(activeProject.path).then(() => {
    showToast('Caminho copiado');
  });
}

function toggleFavorite(id) {
  const p = getProjectById(id);
  if (p) {
    p.isFavorite = !p.isFavorite;
    saveToStorage();
    render();
  }
}

function triggerLaunch(toolName, project) {
  if (!project) return;
  if (project.isOrphan) {
    showToast('⚠️ Caminho de pasta inacessível no disco.');
    return;
  }
  project.lastAccessed = 'Agora mesmo';
  saveToStorage();
  render();
  showToast(`Iniciando ${toolName}: ${project.name}`);
}

function handleRemoveActiveProject() {
  if (!activeProject) return;
  if (confirm(`Remover "${activeProject.name}" do DevHub?\n\nSeus arquivos locais em ${activeProject.path} permanecerão intactos.`)) {
    projects = projects.filter(p => p.id !== activeProject.id);
    saveToStorage();
    closeSidePeek();
    render();
    showToast('Projeto removido da biblioteca.');
  }
}

// ==========================================================================
// Add Project Flow
// ==========================================================================
function openAddProjectModal() {
  document.getElementById('inputProjectPath').value = '';
  document.getElementById('inputProjectName').value = '';
  document.getElementById('selectCategory').value = 'Empresa';
  updateInspectPreview('C:\\dev\\novo-projeto');
  openModal('modalAddProject');
}

function simulateBrowseFolder() {
  const paths = [
    'C:\\dev\\game-physics-engine',
    'D:\\work\\audio-dsp-synth',
    'C:\\projects\\qt-client-v2',
    'C:\\dev\\simd-matrix-ops'
  ];
  const chosen = paths[Math.floor(Math.random() * paths.length)];
  document.getElementById('inputProjectPath').value = chosen;
  handlePathInputDetection();
}

function handlePathInputDetection() {
  const path = document.getElementById('inputProjectPath').value.trim();
  const nameInput = document.getElementById('inputProjectName');
  if (path) {
    const parts = path.split(/[\\/]/).filter(Boolean);
    if (parts.length > 0) {
      nameInput.value = parts[parts.length - 1];
    }
    updateInspectPreview(path);
  }
}

function updateInspectPreview(path) {
  const container = document.getElementById('inspectTagsPreview');
  let tags = `<span class="notion-tag tag-orange">CMake</span><span class="notion-tag tag-blue">C++20</span>`;
  if (path.toLowerCase().includes('qt') || path.toLowerCase().includes('client')) {
    tags += `<span class="notion-tag tag-purple">Qt 6</span>`;
  }
  if (path.toLowerCase().includes('driver') || path.toLowerCase().includes('win32')) {
    tags = `<span class="notion-tag tag-purple">MSVC</span><span class="notion-tag tag-blue">C++17</span>`;
  }
  container.innerHTML = tags;
}

function handleAddProject() {
  const path = document.getElementById('inputProjectPath').value.trim();
  const name = document.getElementById('inputProjectName').value.trim();
  const category = document.getElementById('selectCategory').value;

  if (!path) {
    showToast('Informe o diretório do projeto.');
    return;
  }

  const exists = projects.some(p => p.path.toLowerCase() === path.toLowerCase());
  if (exists) {
    showToast('⚠️ Esse diretório já está cadastrado.');
    return;
  }

  const isQt = path.toLowerCase().includes('qt') || path.toLowerCase().includes('client');
  const isMsvc = path.toLowerCase().includes('driver');

  const newProj = {
    id: 'proj-' + Date.now(),
    name: name || 'Projeto C++',
    path: path,
    buildSystem: isMsvc ? 'Visual Studio' : 'CMake',
    cxxStandard: isMsvc ? 'C++17' : 'C++20',
    frameworks: isQt ? 'Qt 6' : 'Nenhum',
    category: category,
    isFavorite: false,
    preferredIde: isMsvc ? 'Visual Studio' : 'VS Code',
    notes: 'Adicionado recentemente.',
    lastAccessed: 'Recém-adicionado',
    isOrphan: false
  };

  projects.unshift(newProj);
  saveToStorage();
  closeModal('modalAddProject');
  render();
  showToast(`"${newProj.name}" adicionado.`);
}

function saveSettings() {
  closeModal('modalSettings');
  showToast('Preferências salvas.');
}

function openModal(id) {
  document.getElementById(id).classList.remove('hidden');
}

function closeModal(id) {
  document.getElementById(id).classList.add('hidden');
}

function showToast(text) {
  const hub = document.getElementById('toastHub');
  const toast = document.createElement('div');
  toast.className = 'toast-pill';
  toast.textContent = text;
  hub.appendChild(toast);
  setTimeout(() => {
    toast.style.opacity = '0';
    toast.style.transition = 'opacity 0.2s ease';
    setTimeout(() => toast.remove(), 200);
  }, 2200);
}

function escapeHTML(str) {
  return String(str || '').replace(/[&<>'"]/g, tag => ({
    '&': '&amp;',
    '<': '&lt;',
    '>': '&gt;',
    "'": '&#39;',
    '"': '&quot;'
  }[tag] || tag));
}
