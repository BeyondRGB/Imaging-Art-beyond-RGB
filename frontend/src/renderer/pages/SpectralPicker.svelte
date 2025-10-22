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
    Maximize2Icon,
    Minimize2Icon,
  } from "svelte-feather-icons";
  import { fly } from "svelte/transition";
  import LineChart from "@components/Charts/LineChart.svelte";
  import Switch from "@components/Switch.svelte";
  import FileSelector from "@components/FileSelector.svelte";
  import { fullScreenApi } from "openseadragon";
  import EmptyState from "@components/EmptyState.svelte";
  import ExpandablePanel from "@components/ExpandablePanel.svelte";
  import CloseButton from "@components/CloseButton.svelte";
  import LoadingOverlay from "@components/LoadingOverlay.svelte";

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
    <EmptyState title="Select a project file to import into BeyondRGB">
      <FileSelector bind:filePaths={mainfilePath} filter="project" defaultPath="" />
    </EmptyState>
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
        <CloseButton variant="floating" onClick={closeImage} />

        <div class="image-container">
          <LoadingOverlay show={loading} message="Loading" />

          <ExpandablePanel 
            bind:expanded={expand}
            position="right"
            width="30vw"
            handlePosition="0%"
          >
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
          </ExpandablePanel>
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
  .content {
    @apply w-full h-full flex justify-center items-center p-6;
  }
  .panel {
    @apply w-full h-full flex justify-center items-center;
  }
  .image-container {
    background-color: var(--color-surface-sunken);
    @apply relative w-full h-full overflow-visible;
  }
  .image-tabs {
    @apply h-full w-full bg-red-500/50 relative
          flex flex-col;
  }

  .aspect {
    @apply aspect-[145/100] w-auto;
  }
  .pixSize {
    background-color: var(--color-surface-sunken);
    @apply flex gap-1 p-1 rounded-lg;
  }
  .brushBar {
    @apply w-full h-2 rounded-xl;
  }
  .brushBar::-webkit-slider-thumb {
    background-color: var(--color-interactive);
    @apply w-4 h-4 cursor-pointer rounded-full outline outline-1;
    outline-color: var(--color-border);
  }

  .side {
    @apply h-full w-[45vw];
  }

  .fullBtn {
    @apply absolute right-0 m-1 z-50 p-1 bg-transparent ring-0
            hover:bg-blue-500/25 transition-all duration-500;
  }

  .chart {
    background-color: var(--color-surface-elevated);
    @apply m-2 p-2 pb-4 rounded-lg pr-4;
  }

  .box {
    background-color: var(--color-surface-elevated);
    @apply m-2 shadow-md px-2 pt-1 rounded-lg p-2;
  }

  .sizeSettings {
    @apply flex justify-between items-center text-base pl-4;
  }
</style>
