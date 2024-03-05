const { contextBridge, ipcRenderer } = require('electron');
const { remote } = require('electron');

contextBridge.exposeInMainWorld('electron', {
  async handle(arg) {
    const result = await ipcRenderer.invoke('ipc-Dialog', arg);
    return result;
  },
  async getPort() {
    const result = await ipcRenderer.invoke('ipc-getPort');
    return result;
  },
  async openNewWindow() {
    const result = await ipcRenderer.invoke('ipc-createNewWindow');
    return result;
  },
  async openFileExplorer(arg) {
    const result = ipcRenderer.invoke('ipc-openFileExplorer', arg);
    return result;
  }
});
