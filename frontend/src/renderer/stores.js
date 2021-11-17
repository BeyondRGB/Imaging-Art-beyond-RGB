import { writable } from 'svelte/store';


// Stores
export const currentPage = writable(null);
export const appSettings = writable(0);
export const modal = writable(null);
export const messageStore = writable('');

// Websocket
const socket = new WebSocket('ws://localhost:9002');

socket.addEventListener('open', function (event) {
  console.log("Connected!", socket);
});

socket.addEventListener('message', function (event) {
  messageStore.set(event.data);
});

export const sendMessage = (message) => {
  if (socket.readyState === 1) {
    socket.send(message);
  }
}