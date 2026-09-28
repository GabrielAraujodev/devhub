const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('devhubAPI', {
  openDirectoryDialog: () => ipcRenderer.invoke('dialog:openDirectory'),
  inspectProject: (path) => ipcRenderer.invoke('fs:inspectProject', path),
  launchProcess: (opts) => ipcRenderer.invoke('process:launch', opts),
  loadProjects: () => ipcRenderer.invoke('storage:loadProjects'),
  saveProjects: (projects) => ipcRenderer.invoke('storage:saveProjects', projects),
  loadSettings: () => ipcRenderer.invoke('storage:loadSettings'),
  saveSettings: (settings) => ipcRenderer.invoke('storage:saveSettings', settings)
});
