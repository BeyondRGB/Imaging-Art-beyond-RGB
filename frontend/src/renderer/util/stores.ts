import { writable, derived } from 'svelte/store';

// Stores
export const currentPage = writable(null);
export const appSettings = writable({ theme: false, sideNav: true });
export const modal = writable(null);
// Page Stores
export const processState = writable({
  currentTab: 0,
  destDir: "",
  imageFilePaths: [],
  thumbnailID: null,
  imageThumbnails: {},
  outputImage: { dataURL: "", name: "Waiting..." },
  artStacks: [
    {
      id: 1,
      name: "Art 1",
      colorTargetImage: { dataURL: "", filename: "", },
      verificationTargetImage: { dataURL: "", filename: "" },
      colorTarget: {},
      verificationTarget: {},
      fields: {
        images: [],
        whitefield: [],
        darkfield: [],
      },
    },]
});
// Webstocket Stores
export const messageStore = writable([]);
export const messageLog = writable([]);
export const connectionState = writable('Not Connected');


// Websocket
let socket = new WebSocket('ws://localhost:9002');

export function connect() {
  console.log({ "Attempting to Connect": socket });
  socket = new WebSocket('ws://localhost:9002');
  socket.addEventListener('open', function (event) {
    console.log("Connected!");
    connectionState.set("Connected");
  });

  socket.addEventListener('close', function (event) {
    console.log("Closed - Trying again in 15 seconds.");
    connectionState.set("Closed");
    close();
    // setTimeout(function () {
    //   connect();
    // }, 15000);
  });

  socket.addEventListener('error', function (event) {
    console.log("Error");
    connectionState.set("Closed - Error");
    socket.close();
  });

  socket.addEventListener('message', function (event) {
    let messageObj = [event.data, new Date()];
    messageStore.set([event.data, new Date()]);
    if (event.data.length > 1000) {
      console.log("Recived large message");
      messageLog.update(current => [[event.data.slice(0, 1000), new Date()], ...current]);
    } else {
      messageLog.update(current => [messageObj, ...current]);
    }
  });
}
connect();

export function close() {
  console.log("Closing all websocket listeners");
  socket.close();
}

export const sendMessage = (message) => {
  if (socket.readyState === 1) {
    let messageObj = [message, new Date(), true];
    messageLog.update(current => [messageObj, ...current]);
    socket.send(message);
  }
};