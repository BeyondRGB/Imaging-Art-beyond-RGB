const { app, BrowserWindow, dialog, ipcMain } = require('electron');
const path = require('path');
const child_process = require('child_process');
const {getPort} = require('get-port-please');
const { shell } = require('electron')

let freePort = 47382;

try {
  if (process.env.ELEC_ENV === 'dev') {
    freePort = 9002;
  } else {
    freePort = getPort();
  }
  console.log(freePort);
} catch (e) {
  console.log(e);
}
var executablePath;
var loader;

if (process.platform == 'win32')
  executablePath = path.join(__dirname, '../../lib/beyond-rgb-backend.exe');
else {
  executablePath = path.join(__dirname, '../../lib/beyond-rgb-backend');
}

// Handle creating/removing shortcuts on Windows when installing/uninstalling.
// if (require('electron-squirrel-startup')) { // eslint-disable-line global-require
//   setTimeout(app.quit, 1000);
// }

process.on('loaded', (event, args) => {
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

ipcMain.handle('ipc-getPort', async (event, arg) => {
  return freePort;
});

//Use the shell to open the File explorer as a separate process,
//as oppossed to a child process of the BeyondRGB application.
ipcMain.handle('ipc-openFileExplorer', async (event, arg) => {
  const result = await shell.openPath(arg.directory) // Open the given directory/file in the default manner.
  return result ;
});


//Opens a "file selector" as a child process of BeyondRGB application.
//Intended for user to select files/directories during Processing setup
ipcMain.handle('ipc-Dialog', async (event, arg) => {
  let properties = ['openFile', 'multiSelections'];
  let filters = [];
  let defaultPath;
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

  if(arg.defaultPath !== undefined) {
    defaultPath = arg.defaultPath;
  }

  const dia = await dialog.showOpenDialog({
    defaultPath,
    properties,
    filters
  }).then(result => {
    return result;
  }).catch(err => {
    console.log(err);
  });
  return dia;
});

// process.on('loaded', (event, args) => {
//   console.log('LOADED');
//   console.log(process.resourcesPath);
//   // console.log(process.getCPUUsage());
//   // console.log(process.getProcessMemoryInfo());
//   console.log(app.getAppPath());
// });

const createMainWindow = () => {
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
    mainWindow.loadURL('http://localhost:3000');
    // Open the DevTools.
    mainWindow.webContents.openDevTools();
  } else {
    mainWindow.loadFile(path.join(__dirname, '../../public/index.html'));
    // mainWindow.loadURL(`file://${path.join(__dirname, '../../public/index.html')}`);
  }

};

const createNewWindow = () => {
  // Create the browser window.
  const secondWindow = new BrowserWindow({
    width: 1200,
    height: 800,
    autoHideMenuBar: true,
    backgroundColor: "#2c2c2e",
    minWidth: 600,
    minHeight: 300,
    title: "Beyond RGB - Secondary View",
    icon: path.join(__dirname, '../../assets/icon.ico'),
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
    },
  });

  // and load the index.html of the app.

  if (process.env.ELEC_ENV === 'dev') {
    secondWindow.loadURL('http://localhost:3000');
    // Open the DevTools.
    secondWindow.webContents.openDevTools();
  } else {
    secondWindow.loadFile(path.join(__dirname, '../../public/index.html'));
    // mainWindow.loadURL(`file://${path.join(__dirname, '../../public/index.html')}`);
  }

};

ipcMain.handle('ipc-createNewWindow', async (event, arg) => {
  createNewWindow();
});

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createMainWindow);
