<script lang="ts">
  import {
    viewState,
    processState,
    connectionState,
    connect,
    currentPage,
    messageStore,
  } from "@util/stores";
  import ImageViewer from "@components/ImageViewer.svelte";

  let notConnectedMode = false;
  let info = { sender: "Waiting...", value: 0 };

  function reset() {
    currentPage.set("Process");
    processState.set({
      currentTab: 0,
      destDir: "",
      imageFilePaths: [],
      thumbnailID: null,
      colorTargetID: null,
      imageThumbnails: {},
      outputImage: { dataURL: "", name: "Waiting..." },
      artStacks: [
        {
          id: 1,
          name: "Art 1",
          colorTargetImage: { dataURL: "", filename: "" },
          verificationTargetImage: { dataURL: "", filename: "" },
          colorTarget: {},
          verificationTarget: null,
          fields: {
            images: [],
            whitefield: [],
            darkfield: [],
          },
        },
      ],
    });
  }

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
  {#if $viewState.projectKey?.length > 1}
    COMPLETED
  {/if}
  <div class="top">
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
