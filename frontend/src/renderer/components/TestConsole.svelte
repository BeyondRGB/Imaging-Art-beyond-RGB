<script lang="ts">
  import {
    sendMessage,
    messageStore,
    messageLog,
    processState,
    connectionState,
    connect,
  } from "@util/stores";
  import { flip } from "svelte/animate";
  let notConnectedMode = false;
  let textValue;
  export let isOpen;

  function getTime(time = new Date()) {
    let minutes = time.getMinutes();
    let seconds = time.getSeconds();
    let mili = time.getMilliseconds();
    let hours = time.getHours();
    let ofDay = "AM";
    if (hours > 12) {
      hours -= 12;
      ofDay = "PM";
    }
    return `${hours < 10 ? "0" : ""}${hours}:${
      minutes < 10 ? "0" : ""
    }${minutes}:${seconds < 10 ? "0" : ""}${seconds}:${
      mili < 10 ? "00" : mili < 100 ? "0" : ""
    }${mili} ${ofDay}`;
  }
</script>

<main>
  <div class="inputGroup">
    <button on:click={() => (textValue = "")} class="clear">X</button>
    <textarea placeholder="Enter command here" bind:value={textValue} />
    <button
      class="termSend"
      on:click={() => {
        sendMessage(textValue);
      }}>Send It!</button
    >
  </div>
  <div class="outputGroup">
    <h4>
      Server Response <button
        class="clearTerm"
        on:click={() => ($messageLog = [])}>clear</button
      >
    </h4>
    {#if isOpen}
      <div class="term">
        {#each $messageLog as message (message)}
          <div
            class={`message ${message[2] ? "request" : "response"}`}
            animate:flip={{ duration: 250 }}
          >
            {#if message[2]}
              {#if message[0]?.length > 128}
                <details class="requestDropdown">
                  <summary class="reqTime">
                    [Expand: {message[0].length} chars] {"<"} ({getTime(
                      message[1]
                    )})</summary
                  >
                  <code>{message[0]}</code>
                </details>
              {:else}
                <div class="requestMessage">
                  <p class="reqTime">{"<"} ({getTime(message[1])})</p>
                  <p class="reqMsg">{message[0]}</p>
                </div>
              {/if}
            {:else if message[0]?.length > 512}
              <details class="responseDropdown">
                <summary
                  >{getTime(message[1])}) > [Expand: {message[0].length}+ chars]
                  {message[0].substring(0, 256)} ...</summary
                >
                <code>{message[0]} ...</code>
              </details>
            {:else}
              <div class="responseMessage">
                ({getTime(message[1])}) > {message[0]}
              </div>
            {/if}
          </div>
        {/each}
      </div>
    {/if}
  </div>
</main>

<style lang="postcss">
  main {
    display: grid;
    grid-template-columns: auto;
    grid-template-rows: auto auto;
    @apply w-full h-full p-2;
  }
  h4 {
    @apply text-gray-200 bg-gray-900/90 absolute top-0 pb-1 pt-0 px-2 rounded-lg z-50;
  }
  code {
    @apply bg-gray-900/50;
  }
  textarea {
    /* overflow-wrap: break-word; */
    @apply bg-gray-500/25 w-full h-full text-white border-2 border-blue-600/75 max-h-60 min-h-[4rem]
            overflow-y-auto rounded-l-lg p-1 focus:outline-blue-600;
  }
  .clear {
    @apply absolute -left-2 top-2 text-sm px-1 py-0 text-red-500 bg-red-600/25 hover:bg-red-700;
  }
  .clearTerm {
    @apply bg-red-500/20 hover:bg-red-500/50 text-sm;
  }
  .term {
    scroll-behavior: smooth;
    @apply overflow-y-auto h-full w-full bg-gray-900 p-2 flex flex-col-reverse rounded-lg;
  }
  .termSend {
    @apply bg-blue-600/75 h-full rounded-l-none active:bg-green-600;
  }
  .inputGroup {
    @apply flex w-full h-full justify-center items-center p-2 relative;
  }
  .outputGroup {
    /* max-width: calc(50vw + 2rem); */
    max-width: calc(60vw - 4rem);
    max-height: 60vh;
    min-height: 25vh;
    @apply w-full h-full relative flex justify-center items-center;
  }
  .message {
    overflow-wrap: break-word;
    @apply w-[97%] h-full bg-red-400 rounded-3xl py-3 px-4 select-text inline-block mb-2;
  }
  .response {
    overflow-wrap: break-word;
    @apply bg-gray-700 mr-auto;
  }
  .request {
    overflow-wrap: break-word;
    @apply bg-blue-600/80 ml-auto;
  }
  .reqTime {
    text-align: right;
    @apply right-0;
  }
  .responseDropdown {
    @apply overflow-auto w-full h-full;
  }

  ::-webkit-scrollbar {
    @apply w-2;
  }

  ::-webkit-scrollbar-track {
    @apply bg-gray-700/25;
  }

  /* Handle */
  ::-webkit-scrollbar-thumb {
    @apply bg-gray-500 rounded-full;
  }

  /* Handle on hover */
  ::-webkit-scrollbar-thumb:hover {
    @apply bg-blue-800;
  }
</style>
