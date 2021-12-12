import { writable } from 'svelte/store';

// Stores
export const currentPage = writable(null);
export const appSettings = writable(false);
export const modal = writable(null);
export const messageStore = writable('');
export const connectionState = writable('Not Connected')

// Websocket
const socket = new WebSocket('ws://localhost:9002');

function connect() {
  socket.addEventListener('open', function (event) {
    console.log("Connected!");
    connectionState.set("Connected")
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
    connectionState.set("Closed - Error")
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
}