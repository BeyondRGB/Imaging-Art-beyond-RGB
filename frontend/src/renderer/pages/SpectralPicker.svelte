<script lang="ts">
  import SpecPickViewer from "@components/SpectralPicker/SpecPickViewer.svelte";
  import {
    processState,
    viewState,
    sendMessage,
    messageStore,
    currentPage,
  } from "@util/stores";
  import {
    XCircleIcon,
    Maximize2Icon,
    Minimize2Icon,
  } from "svelte-feather-icons";
  import { fly } from "svelte/transition";
  import LineChart from "@components/Charts/LineChart.svelte";
  import Switch from "@components/Switch.svelte";
  import FileSelector from "@components/FileSelector.svelte";
  import { fullScreenApi } from "openseadragon";

  let brushShow = false;
  let stackCurves = false;
  let size;
  let trueSize;
  let shadowPos = { left: 0, top: 0 };
  let trueShadowPos = { left: "0px", top: "0px" };
  let spectrumData;
  let mainfilePath;

  let loading = false;

  let expand = false;

  let wavelengthArray = Array.from({ length: 36 }, (x, i) => i * 10 + 380);

  function getData() {
    console.log("Fetching Spec Data");
    if ($viewState.projectKey !== null) {
      let msg = {
        RequestID: Math.floor(Math.random() * 999999999),
        RequestType: "SpectralPicker",
        RequestData: {
          name: $viewState.projectKey,
          coordinates: {
            x: shadowPos.left,
            y: shadowPos.top,
          },
          size: size,
        },
      };
      sendMessage(JSON.stringify(msg));
    }
  }

  $: if (shadowPos !== null) {
    getData();
  }

  $: if ($messageStore.length > 1 && !($messageStore[0] instanceof Blob)) {
    console.log("New Message Spec");
    try {
      let temp = JSON.parse($messageStore[0]);
      if (temp["ResponseType"] === "SpectralPicker") {
        console.log("Spectrum Data From Server");
        spectrumData = temp["ResponseData"]["spectrum"];
      }
    } catch (e) {
      console.log(e);
    }
  }

  function colorManagedImage() {
    let rand = Math.floor(Math.random() * 999999);
    $processState.CMID = rand;
    let msg = {
      RequestID: rand,
      RequestType: "ColorManagedImage",
      RequestData: {
        name: $viewState.projectKey,
        target_requested: false,
      },
    };

    console.log("Fetching Color Managed Image");
    console.log(msg);
    loading = true;
    sendMessage(JSON.stringify(msg));
  }

  $: if (
    $currentPage === "SpecPicker" &&
    $viewState.projectKey !== null &&
    $viewState.colorManagedImage.dataURL.length < 1
  ) {
    console.log("Getting FIRST Color Managed Image");
    // CALL FOR CM
    colorManagedImage();
  }

  $: if (mainfilePath?.length > 0) {
    console.log("New Project Key");
    $viewState.projectKey = mainfilePath[0];
  }

  let isFullScreen = window.innerHeight == screen.height;

  function closeImage() {
    $viewState.projectKey = null;
    $viewState.colorManagedImage.dataURL = "";
    mainfilePath = "";
  }

</script>

<main>
  {#if $viewState.projectKey === null}
    <div class="noFile">
      <div class="inputBox">
        <h2>Select a project file to import into BeyondRGB</h2>
        <FileSelector bind:filePaths={mainfilePath} filter="project" />
      </div>
    </div>
  {/if}
  <div class="content" id="picker-content">
    <div class="panel">
      <div class="image-tabs" class:aspect={expand}>
        <button
          class="fullBtn"
          on:click={() => {
            if (isFullScreen) {
              document.exitFullscreen();
              isFullScreen = false;
            } else {
              document.querySelector(".image-tabs").requestFullscreen();
              isFullScreen = true;
            }
          }}
        >
          {#if isFullScreen}
            <Minimize2Icon size="1.25x" />
          {:else}
            <Maximize2Icon size="1.25x" />
          {/if}
        </button>
        <button class="closeBtn" on:click={closeImage}>
          <XCircleIcon size="1.25x" />
        </button>

        <div class="image-container">
          {#if loading}
            <div class="loading">
              <div class="loading-box">Loading<span class="loader" /></div>
            </div>
          {/if}

          <div class="floatBox" class:notExpanded={!expand}>
            <div class="handle" on:click={() => (expand = !expand)}>
              {expand ? ">" : "<"}
            </div>
            <div class="box" id="brush">
              <Switch label="Enable Spectral Picker" bind:checked={brushShow} />
              <Switch label="Stack Spectral Curves" bind:checked={stackCurves} />
              <div class="sizeSettings">
                Set Brush Size:
                <div class="flex justify-center items-center gap-1">
                  <input
                    class="brushBar"
                    type="range"
                    min="0.001"
                    max="0.03"
                    step=".0005"
                    bind:value={size}
                  />
                  <div class="pixSize">
                    <span>{parseFloat(trueSize).toFixed(1)}</span>
                    <span>px</span>
                  </div>
                </div>
              </div>
            </div>
            <div class="chart">
              <LineChart
                bind:data={spectrumData}
                bind:wavelengthArray
                bind:trueShadowPos
                stack={stackCurves}
              />
            </div>
          </div>
          <SpecPickViewer
            bind:shadowPos
            bind:trueShadowPos
            bind:trueSize
            bind:show={brushShow}
            bind:size
            bind:loading
          />
        </div>
      </div>
      {#if !expand}
        <div class="side" />
      {/if}
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex h-full w-full justify-center flex-col relative;
  }
  .noFile {
    @apply absolute w-full h-full z-[99] flex justify-center bg-gray-800
            items-center;
  }
  .inputBox {
    @apply w-auto h-auto bg-gray-700 flex flex-col gap-2 justify-center items-center
          p-8 rounded-2xl;
  }
  .inputBox h2 {
    @apply text-xl;
  }
  .content {
    @apply w-full h-full flex justify-center items-center p-6;
  }
  .panel {
    @apply w-full h-full flex justify-center items-center;
  }
  .image-container {
    @apply relative w-full h-full bg-gray-500 overflow-visible;
  }
  .image {
    @apply bg-green-400 overflow-auto h-[90%] aspect-[3/2];
  }
  .image-tabs {
    @apply h-full w-full bg-red-500/50 relative
          flex flex-col;
  }

  .aspect {
    @apply aspect-[145/100] w-auto;
  }
  .tabs {
    @apply w-auto bg-gray-700 flex gap-1 pt-1 px-1 rounded-t-lg;
  }
  .tab {
    @apply relative;
  }
  .btnTab {
    @apply w-16 h-full bg-gray-700 flex justify-center items-center
          rounded-t-xl rounded-b-none relative;
  }
  .pixSize {
    @apply flex bg-gray-800/50 gap-1 p-1 rounded-lg;
  }
  .brushBar {
    @apply w-full h-2 rounded-xl;
  }
  .brushBar::-webkit-slider-thumb {
    @apply w-4 h-4 bg-gray-600 cursor-pointer rounded-full outline outline-1
          outline-gray-200;
  }

  .closeTab {
    @apply absolute -right-0.5 -top-0.5 flex justify-center items-center
            p-0.5 bg-transparent ring-0 text-gray-400 
            hover:bg-red-500/50 hover:text-white z-[49];
  }

  .floatBox {
    border: 1px solid red;
    @apply absolute h-auto w-[30vw] bg-gray-500/75 z-[49] right-0 transition-all duration-500
            translate-x-0 border border-gray-700 rounded-bl-xl;
  }

  .notExpanded {
    @apply translate-x-full;
  }

  .loading {
    @apply bg-gray-700 absolute w-full h-full z-[49] flex justify-center items-center;
  }
  .loading-box {
    @apply h-full flex flex-col gap-2 justify-center items-center
            text-2xl;
  }
  .loader {
    /* position: absolute; */
    width: 48px;
    height: 48px;
    background: #11ff00;
    transform: rotateX(65deg) rotate(45deg);
    /* remove bellows command for perspective change */
    transform: perspective(200px) rotateX(65deg) rotate(45deg);
    color: rgb(255, 0, 0);
    animation: layers1 1s linear infinite alternate;
    @apply z-50;
  }
  .loader:after {
    content: "";
    position: absolute;
    inset: 0;
    background: rgb(0, 0, 255);
    animation: layerTr 1s linear infinite alternate;
  }

  @keyframes layers1 {
    0% {
      box-shadow: 0px 0px 0 0px;
    }
    90%,
    100% {
      box-shadow: 20px 20px 0 -4px;
    }
  }
  @keyframes layerTr {
    0% {
      transform: translate(0, 0) scale(1);
    }
    100% {
      transform: translate(-25px, -25px) scale(1);
    }
  }

  .selected {
    @apply bg-blue-500/50;
  }

  .side {
    @apply h-full w-[45vw];
  }

  .fullBtn {
    @apply absolute right-0 m-1 z-50 p-1 bg-transparent ring-0
            hover:bg-blue-500/25 transition-all duration-500;
  }

  .closeBtn {
    @apply absolute right-8 m-1 z-50 p-1 bg-transparent ring-0
            hover:bg-red-500/25 transition-all duration-500;
  }

  .chart {
    @apply bg-gray-600 m-2 p-2 pb-4 rounded-lg pr-4;
  }

  .box {
    @apply m-2 shadow-md px-2 pt-1 bg-gray-600 rounded-lg p-2;
  }
  .numberInput {
    @apply p-0.5 bg-gray-900 border-[1px] border-gray-800 rounded-lg
          focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2;
  }
  .handle {
    border: 1px solid red;
    @apply bg-gray-500/75 h-12 w-8 absolute bottom-1/2 -left-8 flex justify-center items-center
							text-2xl rounded-l-full border border-r-[0px] border-gray-800;
  }

  .sizeSettings {
    @apply flex justify-between items-center text-base pl-4;
  }
</style>
