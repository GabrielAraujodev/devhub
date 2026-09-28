const { app, BrowserWindow, ipcMain, dialog, shell } = require('electron');
const path = require('path');
const fs = require('fs');
const { spawn, exec } = require('child_process');

let mainWindow = null;

// Local storage paths
const appDataDir = path.join(app.getPath('appData'), 'DevHub');
const projectsFile = path.join(appDataDir, 'projects.json');
const settingsFile = path.join(appDataDir, 'settings.json');

// Ensure directory exists
if (!fs.existsSync(appDataDir)) {
  fs.mkdirSync(appDataDir, { recursive: true });
}

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1240,
    height: 820,
    minWidth: 920,
    minHeight: 600,
    title: 'DevHub C++',
    backgroundColor: '#f7f6f3',
    autoHideMenuBar: true,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      contextIsolation: true,
      nodeIntegration: false
    }
  });

  mainWindow.loadFile(path.join(__dirname, 'renderer', 'index.html'));
}

app.whenReady().then(() => {
  createWindow();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit();
});

// ==========================================================================
// IPC HANDLERS — NATIVE DESKTOP CAPABILITIES (HOUS3 F01, F02, F04, F09)
// ==========================================================================

// F01: Native Windows Directory Picker
ipcMain.handle('dialog:openDirectory', async () => {
  const result = await dialog.showOpenDialog(mainWindow, {
    properties: ['openDirectory'],
    title: 'Selecionar pasta do projeto C++'
  });
  if (result.canceled || result.filePaths.length === 0) {
    return null;
  }
  return result.filePaths[0];
});

// F02: Real File System Inspection
ipcMain.handle('fs:inspectProject', async (event, projectPath) => {
  if (!fs.existsSync(projectPath)) {
    return { error: 'Caminho não encontrado no sistema de arquivos' };
  }

  let buildSystem = 'Custom';
  let cxxStandard = null;
  let hasQt = false;
  let hasNinja = false;

  const entries = fs.readdirSync(projectPath);

  // Check for CMake
  if (entries.includes('CMakeLists.txt')) {
    buildSystem = 'CMake';
    try {
      const content = fs.readFileSync(path.join(projectPath, 'CMakeLists.txt'), 'utf8');
      const matchStd = content.match(/CMAKE_CXX_STANDARD\s+(11|14|17|20|23|26)/i);
      if (matchStd) {
        cxxStandard = 'C++' + matchStd[1];
      }
      if (content.includes('Qt5') || content.includes('Qt6') || content.includes('find_package(Qt')) {
        hasQt = true;
      }
    } catch (e) {}
  } else if (entries.some(f => f.endsWith('.sln') || f.endsWith('.vcxproj'))) {
    buildSystem = 'Visual Studio';
    cxxStandard = 'C++17';
  }

  if (entries.includes('build.ninja') || entries.includes('ninja.build')) {
    hasNinja = true;
  }

  const folderName = path.basename(projectPath);

  return {
    name: folderName,
    path: projectPath,
    buildSystem,
    cxxStandard,
    hasQt,
    hasNinja
  };
});

// F04: Real Process Launcher
ipcMain.handle('process:launch', async (event, { type, targetPath, ideName }) => {
  if (!fs.existsSync(targetPath)) {
    return { success: false, error: 'O caminho do projeto não existe no disco.' };
  }

  try {
    if (type === 'explorer') {
      shell.openPath(targetPath);
      return { success: true };
    }

    if (type === 'terminal') {
      // Try Windows Terminal (wt.exe), fallback to PowerShell
      const child = spawn('cmd.exe', ['/c', 'start', 'powershell.exe', '-NoExit', '-Command', `Set-Location -LiteralPath '${targetPath}'`], {
        detached: true,
        stdio: 'ignore'
      });
      child.unref();
      return { success: true };
    }

    if (type === 'ide') {
      const ide = (ideName || '').toLowerCase();
      if (ide.includes('visual studio') && !ide.includes('code')) {
        // Look for .sln file first
        const files = fs.readdirSync(targetPath);
        const sln = files.find(f => f.endsWith('.sln'));
        const launchTarget = sln ? path.join(targetPath, sln) : targetPath;
        const child = spawn('cmd.exe', ['/c', 'start', '""', launchTarget], { detached: true, stdio: 'ignore' });
        child.unref();
      } else {
        // Default: VS Code
        const child = spawn('cmd.exe', ['/c', 'code', targetPath], { detached: true, stdio: 'ignore' });
        child.unref();
      }
      return { success: true };
    }
  } catch (err) {
    return { success: false, error: err.message };
  }

  return { success: true };
});

// Storage: Load & Save Projects
ipcMain.handle('storage:loadProjects', async () => {
  if (fs.existsSync(projectsFile)) {
    try {
      const data = fs.readFileSync(projectsFile, 'utf8');
      const projects = JSON.parse(data);
      // Validate path existence
      return projects.map(p => ({
        ...p,
        isOrphan: !fs.existsSync(p.path)
      }));
    } catch (e) {}
  }
  return null;
});

ipcMain.handle('storage:saveProjects', async (event, projects) => {
  fs.writeFileSync(projectsFile, JSON.stringify(projects, null, 2), 'utf8');
  return true;
});

ipcMain.handle('storage:loadSettings', async () => {
  if (fs.existsSync(settingsFile)) {
    try {
      return JSON.parse(fs.readFileSync(settingsFile, 'utf8'));
    } catch (e) {}
  }
  return { ide: 'VS Code', terminal: 'PowerShell' };
});

ipcMain.handle('storage:saveSettings', async (event, settings) => {
  fs.writeFileSync(settingsFile, JSON.stringify(settings, null, 2), 'utf8');
  return true;
});
