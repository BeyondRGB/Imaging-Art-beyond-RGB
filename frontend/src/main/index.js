const { app, BrowserWindow, dialog, ipcMain } = require('electron');
const path = require('path');
const child_process = require('child_process');
const getPortSync = require('get-port-sync');

let freePort = 47382;

try {
  if (process.env.ELEC_ENV === 'dev') {
    freePort = 9002;
  } else {
    freePort = getPortSync();
  }
  console.log(freePort);
} catch (e) {
  console.log(e);
}
let executablePath;
let loader;

if (process.platform === 'win32')
  executablePath = path.join(__dirname, '../../lib/beyond-rgb-backend.exe');
else {
  executablePath = path.join(__dirname, '../../lib/beyond-rgb-backend');
}

// Handle creating/removing shortcuts on Windows when installing/uninstalling.
if (require('electron-squirrel-startup')) { // eslint-disable-line global-require
  app.quit();
}

process.on('loaded', () => {
  console.log('LOADED');
  console.log(app.getAppPath());

  // Start Backend Server
  loader = child_process.spawn(
    executablePath, [
    `--app_root=${app.getAppPath()}`,
    `--port=${freePort}`
  ], {
    detached: true
  }
  );
  loader.stdout.on('data', (data) => {
    console.log(`[Backend stdout]\n${data}`);
  });
  loader.stderr.on('data', (data) => {
    console.log(`========[ BACKEND STDERR ]=======\n${data}`);
  });
});

app.on('before-quit', function () {
  console.log("Quiting");
  process.kill(loader.pid);
});

ipcMain.handle('ipc-getPort', async () => {
  return freePort;
});

ipcMain.handle('ipc-Dialog', async (event, arg) => {
  let properties = ['openFile', 'multiSelections'];
  let filters = [];
  console.log(arg);
  if (arg.type === "Dir") {
    properties = ["openDirectory"];
  }
  if (arg.type === "Single") {
    properties = ["openFile"];
  }

  if (arg.filter === "csv") {
    filters.push({
      "name": "csv file",
      "extensions": ["csv"]
    });
  }
  if (arg.filter === "raws") {
    filters.push({
      "name": "raw & tiff file",
      "extensions": ["cr2", "raf", "nef", "arq", "arw", "tiff", "tif", "dng"]
    });
  }
  if (arg.filter === "project") {
    filters.push({
      "name": "BeyondRGB project file",
      "extensions": ["btrgb"]
    });
  }


  return await dialog.showOpenDialog({
    properties,
    filters
  }).then(result => {
    return result;
  }).catch(err => {
    console.log(err);
  });
});

// process.on('loaded', (event, args) => {
//   console.log('LOADED');
//   console.log(process.resourcesPath);
//   // console.log(process.getCPUUsage());
//   // console.log(process.getProcessMemoryInfo());
//   console.log(app.getAppPath());
// });

const createWindow = async () => {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 1200,
    height: 800,
    autoHideMenuBar: true,
    backgroundColor: "#2c2c2e",
    minWidth: 600,
    minHeight: 300,
    title: "Beyond RGB",
    icon: path.join(__dirname, '../../assets/icon.ico'),
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
    },
  });

  // and load the index.html of the app.
  if (process.env.ELEC_ENV === 'dev') {
    await mainWindow.loadURL('http://localhost:3000');
    // Open the DevTools.
    mainWindow.webContents.openDevTools();
  } else {
    await mainWindow.loadFile(path.join(__dirname, '../../public/index.html'));
    // mainWindow.loadURL(`file://${path.join(__dirname, '../../public/index.html')}`);
  }

};

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow);
