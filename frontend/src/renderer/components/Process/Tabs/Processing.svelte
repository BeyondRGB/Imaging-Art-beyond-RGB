<script lang="ts">
  import {
    sendMessage,
    processState,
    connectionState,
    connect,
    currentPage,
    messageStore,
  } from "@util/stores";
  import ImageViewer from "@components/ImageViewer.svelte";
  import AdvOpts from "./AdvOpts.svelte";

  let notConnectedMode = false;
  let info = { sender: "Waiting...", value: 0 };

  function reset() {
    currentPage.set("Process");
    processState.set({
      currentTab: 0,
      destDir: "",
      imageFilePaths: [],
      outputImage: { dataURL: "", name: "Waiting..." },
      artStacks: [
        {
          id: 1,
          name: "Art 1",
          colorTargetImage: { dataURL: "", filename: "" },
          colorTargets: [
            // {
            //   top: 0.25,
            //   left: 0.25,
            //   bottom: 0.50,
            //   right: 0.50,
            //   cols: 14,
            //   rows: 10,
            // },
          ],
          fields: {
            images: [],
            whitefield: [],
            darkfield: [],
          },
        },
      ],
    });
  }

  $: jsonTest = {
    RequestType: "Process",
    RequestID: Date.now(),
    RequestData: {
      images: [
        {
          art: $processState.artStacks[0].fields.images[0]?.name,
          white: $processState.artStacks[0].fields.whitefield[0]?.name,
          dark: $processState.artStacks[0].fields.darkfield[0]?.name,
        },
        {
          art: $processState.artStacks[0].fields.images[1]?.name,
          white: $processState.artStacks[0].fields.whitefield[1]?.name,
          dark: $processState.artStacks[0].fields.darkfield[1]?.name,
        },
      ],
      destinationDirectory: $processState.destDir,
      targetLocation: {
        top: $processState.artStacks[0].colorTargets[0]?.top,
        left: $processState.artStacks[0].colorTargets[0]?.left,
        bottom: $processState.artStacks[0].colorTargets[0]?.bottom,
        right: $processState.artStacks[0].colorTargets[0]?.right,
        cols: $processState.artStacks[0].colorTargets[0]?.cols,
        rows: $processState.artStacks[0].colorTargets[0]?.rows,
        size: $processState.artStacks[0].colorTargets[0]?.size,
      },
      refData: {
        name: "NGT_Reflectance_Data.csv",
        standardObserver: 1931,
        illuminants: "D50",
      },
    },
  };

  $: if ($messageStore.length > 1) {
    try {
      let temp = JSON.parse($messageStore[0]);
      if (temp["ResponseType"] === "Progress") {
        console.log("Progress From Server");
        info = {
          sender: temp["ResponseData"]["sender"],
          value: temp["ResponseData"]["value"],
        };
      }
    } catch (e) {
      console.log(e);
    }
  }
</script>

<main>
  <div class="top">
    <div class="left">
      <div class="state">
        <button on:click={reset}>Reset</button>
        <h3>
          Current State: <button
            class="stateSend"
            on:click={() => sendMessage(JSON.stringify(jsonTest))}
            >Send to Server</button
          >
          <!-- <button class="" on:click={() => (textValue = JSON.stringify(jsonTest))}
          >Copy to Term</button
        > -->
        </h3>
        <div class="box">
          <p>
            <span class="key">RequestType</span><span
              style="font-weight: bold; background-color: transparent;">:</span
            > <span>Process</span>,
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
            <span class="key">images</span><span
              style="font-weight: bold; background-color: transparent;">:</span
            >

            {#each jsonTest.RequestData.images as image, index}
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
          <p>
            <span class="key">destinationDirectory</span><span
              style="font-weight: bold; background-color: transparent;">:</span
            >
            <span>{jsonTest.RequestData.destinationDirectory}</span>
          </p>
          <p class="px-2">
            <span class="key">targetLoation</span><span
              style="font-weight: bold; background-color: transparent;">:</span
            >

            {#each Object.keys(jsonTest.RequestData.targetLocation) as key}
              <li>
                <span class="key">{key}</span><span
                  style="font-weight: bold; background-color: transparent;"
                  >:</span
                >
                <span>{jsonTest.RequestData.targetLocation[key]}</span>
              </li>
            {/each}
          </p>
          <p class="px-2">
            <span class="key">refData</span><span
              style="font-weight: bold; background-color: transparent;">:</span
            >

            {#each Object.keys(jsonTest.RequestData.refData) as key}
              <li>
                <span class="key">{key}</span><span
                  style="font-weight: bold; background-color: transparent;"
                  >:</span
                > <span>{jsonTest.RequestData.refData[key]}</span>
              </li>
            {/each}
          </p>
        </div>
      </div>
    </div>
    <div class="right">
      <div class="image">
        <h4>{$processState.outputImage?.name}</h4>
        <ImageViewer />
      </div>
    </div>
  </div>
  <div class="bottom">
    <div class="progress-circle" style="--progress:{info.value * 100}">
      <div class="stepper">
        <span class="sender">{info.sender}</span>
        <!-- <span class="message">{info.message}</span> -->
        <span class="value">{info.value}</span>
      </div>
    </div>
  </div>
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
    @apply w-full h-full flex flex-col relative p-2;
  }
  .bottom {
    @apply w-full flex justify-center items-center relative;
  }
  .stepper {
    @apply w-[20vh] h-[20vh] rounded-full bg-gray-500 flex flex-col justify-center items-center;
  }
  .progress-circle {
    background: conic-gradient(
      rgb(5 150 105) calc(var(--progress) * 1%),
      #0000 0
    );
    @apply bg-gray-600 rounded-full p-1;
  }
  .sender {
    @apply bg-gray-500 text-lg rounded-lg;
  }
  .message {
    @apply bg-gray-500 rounded-lg p-0.5;
  }
  .top {
    @apply w-full h-full flex justify-center items-center overflow-hidden;
  }
  .left {
    @apply w-[30%] h-full flex justify-center items-center ml-2;
  }
  .right {
    @apply w-full h-full flex justify-center items-center ml-2;
  }
  .state {
    @apply w-full h-full py-2 flex flex-col;
  }
  .image {
    @apply w-full h-full bg-blue-600/25 relative items-center flex justify-center;
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
  /* .imgTitle {
    @apply absolute top-0 z-50 bg-gray-900/75 pb-1 px-2 pt-0 rounded-lg text-gray-200;
  } */
  .box {
    @apply overflow-y-auto w-full h-full bg-gray-900 text-gray-100 text-sm;
  }
  li {
    @apply px-2 p-1 list-none;
  }
  .stateSend {
    @apply bg-blue-500/50 h-full;
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
