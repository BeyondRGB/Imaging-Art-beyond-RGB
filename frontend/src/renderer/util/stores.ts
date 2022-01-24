import { writable } from 'svelte/store';

// Stores
export const currentPage = writable(null);
export const appSettings = writable({ theme: false, sideNav: true });
export const modal = writable(null);
// Page Stores
export const processState = writable({
  currentTab: 0,
  destDir: "",
  imageFilePaths: [],
  artStacks: [
    {
      id: 1,
      name: "Art 1",
      fields: {
        images: [],
        whitefield: [],
        darkfield: [],
      },
    },]
});
// Webstocket Stores
export const messageStore = writable('');
export const connectionState = writable('Not Connected');


// Websocket
const socket = new WebSocket('ws://localhost:9002');

function connect() {
  socket.addEventListener('open', function (event) {
    console.log("Connected!");
    connectionState.set("Connected");
  });

  socket.addEventListener('close', function (event) {
    console.log("Closed - Trying again in 15 seconds.");
    connectionState.set("Closed");
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
    messageStore.set(event.data);
  });
}
connect();

export const sendMessage = (message) => {
  if (socket.readyState === 1) {
    socket.send(message);
  }
};