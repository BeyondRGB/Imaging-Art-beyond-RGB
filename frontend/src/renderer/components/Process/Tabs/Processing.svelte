<script lang="ts">
  import {
    sendMessage,
    messageStore,
    processState,
    connectionState,
    connect,
  } from "@util/stores";
  import { stringify } from "postcss";
  import ImageViewer from "@components/ImageViewer.svelte";
  import { append, bubble } from "svelte/internal";

  let notConnectedMode = false;
  let textValue;
  let messageList = [];
  $: messageList = [
    [$messageStore[0], getTime($messageStore[1])],
    ...messageList,
  ];

  $: jsonTest = {
    RequestType: "processImg",
    RequestID: Date.now(),
    RequestData: {
      Images: [
        {
          Art: $processState.artStacks[0].fields.images[0]?.name,
          White: $processState.artStacks[0].fields.whitefield[0]?.name,
          Dark: $processState.artStacks[0].fields.darkfield[0]?.name,
        },
        {
          Art: $processState.artStacks[0].fields.images[1]?.name,
          White: $processState.artStacks[0].fields.whitefield[1]?.name,
          Dark: $processState.artStacks[0].fields.darkfield[1]?.name,
        },
      ],
      TargetLocation: $processState.artStacks[0].colorTarget,
      RefData: {
        RefData: "NGT_Reflectance_Data.csv",
        StandardObserver: 1931,
        Illuminants: "D50",
      },
    },
  };

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
  <div class="left">
    <div class="state">
      <h3>
        Current State: <button
          class="stateSend"
          on:click={() => sendMessage(JSON.stringify(jsonTest))}
          >Send to Server</button
        >
        <button class="" on:click={() => (textValue = JSON.stringify(jsonTest))}
          >Copy to Term</button
        >
      </h3>
      <div class="box">
        <p>
          <span class="key">RequestType</span><span
            style="font-weight: bold; background-color: transparent;">:</span
          > <span>processImg</span>,
        </p>
        <p>
          <span class="key">RequestID</span><span
            style="font-weight: bold; background-color: transparent;">:</span
          >
          <span>{jsonTest.RequestID}</span>
        </p>
        <p>
          <span class="key">RequestData</span><span
            style="font-weight: bold; background-color: transparent;">:</span
          >
        </p>
        <p class="px-2">
          <span class="key">Images</span><span
            style="font-weight: bold; background-color: transparent;">:</span
          >

          {#each jsonTest.RequestData.Images as image, index}
            <li>
              <span class="letter"
                >// Image {String.fromCharCode(65 + index)}</span
              >
              {#each Object.keys(image) as key}
                <li>
                  <span class="key">{key}</span><span
                    style="font-weight: bold; background-color: transparent;"
                    >:</span
                  >
                  <span>{image[key]}</span>
                </li>
              {/each}
            </li>
          {/each}
        </p>
        <p class="px-2">
          <span class="key">Target Loation</span><span
            style="font-weight: bold; background-color: transparent;">:</span
          >

          {#each Object.keys(jsonTest.RequestData.TargetLocation) as key}
            <li>
              <span class="key">{key}</span><span
                style="font-weight: bold; background-color: transparent;"
                >:</span
              >
              <span>{jsonTest.RequestData.TargetLocation[key]}</span>
            </li>
          {/each}
        </p>
        <p class="px-2">
          <span class="key">Reference Data</span><span
            style="font-weight: bold; background-color: transparent;">:</span
          >

          {#each Object.keys(jsonTest.RequestData.RefData) as key}
            <li>
              <span class="key">{key}</span><span
                style="font-weight: bold; background-color: transparent;"
                >:</span
              > <span>{jsonTest.RequestData.RefData[key]}</span>
            </li>
          {/each}
        </p>
      </div>
    </div>
    <div class="image">
      <h4>{$processState.outputImage?.name}</h4>
      <ImageViewer />
    </div>
  </div>
  <article>
    <div class="inputGroup">
      <button on:click={() => (textValue = "")} class="clear">X</button>
      <textarea placeholder="Enter command here" bind:value={textValue} />
      <button
        class="termSend"
        on:click={() => {
          messageList.unshift([textValue, getTime(), true]);
          sendMessage(textValue);
        }}>Send It!</button
      >
    </div>
    <div class="outputGroup">
      <h4>Server Response</h4>
      <div class="term">
        {#each messageList as message}
          {#if message[0]?.length > 512}
            <!-- <div class="msg">
              ({message[1]}) >| {message[0].substring(0, 256)} [.../...] {message[0].substring(
                message[0].length - 256
              )}
            </div>
            <br /> -->
            <details class="msg">
              <summary
                >{message[1]}) > [Expand: {message[0].length} chars]
                {message[0].substring(0, 256)} ...</summary
              >
              <code>{message[0]}</code>
            </details>
            <br />
          {:else if message[2]}
            {#if message[0]?.length > 128}
              <details class="request">
                <summary class="reqTime">
                  [Expand: {message[0].length} chars] {"<"} ({message[1]})</summary
                >
                <code>{message[0]}</code>
              </details>
              <br />
            {:else}
              <div class="request">
                <p class="reqTime">{"<"} ({message[1]})</p>
                <p class="reqMsg">{message[0]}</p>
              </div>
              <br />
            {/if}
          {:else}
            <div class="msg">
              ({message[1]}) > {message[0]}
            </div>
            <br />
          {/if}
        {/each}
      </div>
    </div>
  </article>

  {#if $connectionState !== "Connected" && !notConnectedMode}
    <div class="notConnected">
      <card>
        WARNING YOU ARE NOT CONNECTED TO THE BACKEND
        <button class="" on:click={() => connect()}>Reconnect</button>
        <button on:click={() => (notConnectedMode = true)} class="cont"
          >Continue Anyway</button
        >
      </card>
    </div>
  {/if}
</main>

<style lang="postcss">
  .notConnected {
    @apply w-full h-full absolute bg-black/75 z-50 flex justify-center items-center;
  }
  card {
    @apply bg-gray-600 w-64 h-64 p-5 flex flex-col justify-between py-10 rounded-xl font-semibold;
  }
  main {
    @apply w-full h-full flex justify-center items-center relative;
  }
  .left {
    @apply w-full h-full flex flex-col justify-center items-center ml-2;
  }
  .state {
    @apply w-full h-[45vh] py-2 flex flex-col;
  }
  .image {
    @apply w-full h-[45vh] bg-blue-600/25 relative items-center flex justify-center;
  }
  article {
    @apply w-full h-[70%] flex flex-col justify-center items-center m-2;
  }
  h4 {
    @apply text-gray-200 bg-gray-900/90 absolute top-0 pb-1 pt-0 px-2 rounded-lg z-50;
  }
  span {
    @apply bg-gray-700 text-gray-200 font-semibold whitespace-nowrap select-text;
  }
  .letter {
    @apply bg-transparent font-bold text-blue-500 select-none;
  }
  .cont {
    @apply bg-red-500;
  }
  .key {
    @apply bg-transparent font-bold text-red-500/75 select-none;
  }
  h3 {
    @apply text-lg;
  }
  code {
    @apply bg-gray-900/50;
  }
  /* .imgTitle {
    @apply absolute top-0 z-50 bg-gray-900/75 pb-1 px-2 pt-0 rounded-lg text-gray-200;
  } */
  textarea {
    /* overflow-wrap: break-word; */
    @apply bg-gray-500/25 w-full h-[30%] text-white border-2 border-green-600 max-h-60 min-h-[4rem]
            overflow-y-auto;
  }
  .clear {
    @apply absolute -left-2 top-2 text-sm px-1 py-0 text-red-500 bg-red-600/25 hover:bg-red-700;
  }
  .term {
    @apply overflow-y-scroll h-full w-[50vw] bg-gray-900 p-2 flex flex-col-reverse;
  }
  .box {
    @apply overflow-y-auto w-full h-full bg-gray-900 text-gray-100 text-sm;
  }
  li {
    @apply px-2 p-1 list-none;
  }
  .termSend {
    @apply bg-green-600 h-full rounded-l-none;
  }
  .stateSend {
    @apply bg-blue-500/50 h-full;
  }
  .inputGroup {
    @apply flex w-full justify-center items-center p-2 relative;
  }
  .outputGroup {
    @apply flex w-full h-full justify-center items-center relative flex-grow;
  }
  .msg {
    overflow-wrap: break-word;
    @apply bg-gray-700 rounded-xl p-2 select-text inline-block;
  }
  .request {
    overflow-wrap: break-word;
    @apply bg-blue-600/80 rounded-xl p-2 select-text inline-block;
  }
  .reqTime {
    text-align: right;
    @apply right-0;
  }
  ::-webkit-scrollbar-track {
    @apply bg-transparent;
  }

  /* Handle */
  ::-webkit-scrollbar-thumb {
    @apply bg-gray-600 rounded-full;
  }

  /* Handle on hover */
  ::-webkit-scrollbar-thumb:hover {
    @apply bg-gray-500;
  }
</style>
