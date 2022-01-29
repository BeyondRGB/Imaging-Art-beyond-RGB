const { contextBridge, ipcRenderer } = require('electron');
const { remote } = require('electron');

contextBridge.exposeInMainWorld('electron', {
  async handle(arg) {
    const result = await ipcRenderer.invoke('ipc-Dialog', arg);
    return result;
  },
  async resizeImage() {
    // window.electron = {};
    // window.electron.dialog = dialog;
    const { dialog } = require('electron');
    dialog.showOpenDialog({
      properties: ['openFile', 'openDirectory']
    }).then(result => {
      console.log(result.canceled);
      console.log(result.filePaths);
      return result;
    }).catch(err => {
      console.log(err);
      return err;
    });
  },
});
