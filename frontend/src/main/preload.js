const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('electron', {
  async handle(arg) {
    return await ipcRenderer.invoke('ipc-Dialog', arg);
  },
  async getPort() {
    return await ipcRenderer.invoke('ipc-getPort');
  }
});
