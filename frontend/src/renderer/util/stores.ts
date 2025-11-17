import { writable, derived } from 'svelte/store';
import { ImageField } from "@util/ImageField";

// Stores
export const currentPage = writable(null);
export const appSettings = writable({ 
  theme: false, 
  sideNav: true
});
export const modal = writable(null);

export const batchImagesA = writable(['E:\\BeyondRGBPics\\picasso_1_A.ARW']);
export const batchImagesB = writable(['E:\\BeyondRGBPics\\picasso_1_B.ARW']);

export const persistentCustomRefData = writable({
  calibration: [],
  verification: []
});

export const viewState = writable({
  projectKey: null,

  colorManagedImage: { dataURL: "", name: "Waiting..." },
  colorManagedTargetImage: { dataURL: "", name: "Waiting..." },
  reports: {
    calibration: null,
    verification: null
  }
});

export const serverError = writable(null);

// Page Stores
export const customRefData = writable({
  calibration: null,
  verification: null
});
export const processState = writable({
  batch : false,
  processType: "Single",
  artImageCount: 1,
  currentTab: 0,
  completedTabs: [false, false, false, false, false, false],
  pipelineComplete: false,
  destDir: "",
  destFileName: "",
  imageFilePaths: [],
  thumbnailID: null as number,
  colorTargetID: null,
  CMID: null, CMTID:null,
  whitePatchFilled: false,
  imageThumbnails: {},
  outputImage: { dataURL: "", name: "Waiting..." },
  returnedFromProcessing: false,
  artStacks: [
    {
      id: 1,
      name: "Art 1",
      colorTargetImage: { dataURL: "", filename: "", },
      verificationTargetImage: { dataURL: "", filename: "" },
      colorTarget: {},
      verificationTarget: {},
      sharpenString: "N",
      fields: {
          [ImageField.IMAGE_A]: [],
          [ImageField.IMAGE_B]: [],
          [ImageField.TARGET_A]: [],
          [ImageField.TARGET_B]: [],
          [ImageField.FLAT_A]: [],
          [ImageField.FLAT_B]: [],
          [ImageField.DARK_A]: [],
          [ImageField.DARK_B]: [],
      },
    },]
});

export const batchProcessState = writable({
  artImagesA: [],
  artImagesB: [],
  artImageOutputUrls: [],
  proccessingImages: {
      [ImageField.TARGET_A]: [],
      [ImageField.TARGET_B]: [],
      [ImageField.FLAT_A]: [],
      [ImageField.FLAT_B]: [],
      [ImageField.DARK_A]: [],
      [ImageField.DARK_B]: [],
  },
})

export function resetProcess() {
  processState.set({
    batch: false,
    processType: "Single",
    artImageCount: 1,
    currentTab: 0,
    completedTabs: [false, false, false, false, false, false],
    pipelineComplete: false,
    destDir: "",
    imageFilePaths: [],
    thumbnailID: null,
    colorTargetID: null,
    whitePatchFilled: false,
    CMID: null, CMTID:null,
    imageThumbnails: {},
    outputImage: { dataURL: "", name: "Waiting..." },
    returnedFromProcessing: false,
    artStacks: [
      {
        id: 1,
        name: "Art 1",
        colorTargetImage: { dataURL: "", filename: "" },
        verificationTargetImage: { dataURL: "", filename: "" },
        colorTarget: {},
        verificationTarget: {},
        sharpenString: "N",
        fields: {
            [ImageField.IMAGE_A]: [[]],
            [ImageField.IMAGE_B]: [[]],
            [ImageField.TARGET_A]: [],
            [ImageField.TARGET_B]: [],
            [ImageField.FLAT_A]: [],
            [ImageField.FLAT_B]: [],
            [ImageField.DARK_A]: [],
            [ImageField.DARK_B]: [],
        },
      },
    ],
  });
}


// Webstocket Stores
export const messageStore = writable([]);
export const messageLog = writable([]);
export const connectionState = writable('Not Connected');


// Websocket
// let socket = new WebSocket('ws://localhost:9002');
let socket;

export async function connect() {
  console.log({ "Attempting to Connect": socket });
  
  // connect or restart
  let ipcResponse = undefined;
  let errorCount = 0;
  do {
    ipcResponse = await window.electron.getPort();

    // if port is undefined, fully restart the backend (this means its crashed)
    if (!ipcResponse || ipcResponse === 3000)
    {
      errorCount++;
      console.log(`Error occurred when reattaching backend. Error number ${errorCount}`);
      await window.electron.restartBackend();
    }
  } while (!ipcResponse && errorCount < 5);

  // if still disconnected after all that, set to closed
  if (!ipcResponse || ipcResponse === 3000)
  {
    console.log(`Too many errors occured when trying to reconnect backend. Please try again later.`);
    connectionState.set("Closed");
    return;
  }
  
  // set websocket listeners
  console.log(ipcResponse);
  socket = new WebSocket(`ws://localhost:${ipcResponse}`);
  socket.addEventListener('open', function (event) {
    console.log("Connected!");
    connectionState.set("Connected");
  });

  socket.addEventListener('close', function (event) {
    console.log("Closed - Trying again in 15 seconds.");
    connectionState.set("Closed");
    close();
  });

  socket.addEventListener('error', function (event) {
    console.log("Error");
    connectionState.set("Closed - Error");
    socket.close();
  });

  socket.addEventListener('message', function (event) {
    console.log({ RECIVED: event.data });
    messageStore.set([event.data, new Date()]);
  });
}
connect();

export function close() {
  console.log("Closing all websocket listeners");
  socket.close();
}

let prevMessage;
export const sendMessage = (message) => {
  if (socket.readyState === 1) {
    console.log({ SendMessage: message });
    prevMessage = message;
    socket.send(message);
  }
};

export const resendMessage = () => {
  if (socket.readyState === 1) {
    console.log({ ReSendMessage: prevMessage });
    socket.send(prevMessage);
  }
};