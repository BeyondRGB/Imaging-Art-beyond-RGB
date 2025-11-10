<script lang="ts">
  import SpecPickViewer from "@components/SpectralPicker/SpecPickViewer.svelte";
  import {
    processState,
    viewState,
    sendMessage,
    messageStore,
    currentPage,
    serverError,
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
  let size = 0.01; // Default size matching SpecPickViewer default
  let trueSize;
  let shadowPos = { left: 0, top: 0 };
  let trueShadowPos = { left: 0, top: 0 };
  let spectrumData;
  let mainfilePath;

  let loading = false;

  let expand = false;

  let binaryType = null;
  let binaryName = null;
  let binaryID = null;
  let binaryFor = null;

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
      // Validate message is a string before parsing
      const message = $messageStore[0];
      if (typeof message === 'string' && message.trim().length > 0) {
        let temp = JSON.parse(message);
        console.log("Parsed message ResponseType:", temp["ResponseType"], "RequestID:", temp["RequestID"], "CMID:", $processState.CMID);
        
        if (temp["ResponseType"] === "SpectralPicker") {
          console.log("Spectrum Data From Server");
          spectrumData = temp["ResponseData"]["spectrum"];
        } else if (
          // CM Art Binary Handler
          temp["ResponseType"] === "ImageBinary" &&
          temp["RequestID"] === $processState.CMID
        ) {
          console.log("Color Managed Binary From Server - setting binaryFor to ColorManaged");
          binaryType = temp["ResponseData"]["type"];
          binaryName = temp["ResponseData"]["name"];
          binaryID = temp["RequestID"];
          binaryFor = "ColorManaged";
          console.log("binaryFor is now:", binaryFor, "binaryType:", binaryType, "binaryName:", binaryName);
        } else if (temp["ResponseType"] === "Error") {
          // Error handler
          if (temp["ResponseData"] && temp["ResponseData"]["critical"]) {
            serverError.set({
              sender: temp["ResponseData"]["sender"],
              message: temp["ResponseData"]["message"],
            });
            console.log({ SERVERERROR: temp["ResponseData"] });
          }
        } else {
          console.log("Message type not handled:", temp["ResponseType"]);
        }
      } else {
        console.log("Invalid or empty message received");
      }
    } catch (e) {
      console.error("JSON parse error in SpectralPicker:", e);
      console.error("Message content:", $messageStore[0]);
    }
  }

  $: if ($messageStore.length > 1 && $messageStore[0] instanceof Blob) {
    console.log("creating blob for:", binaryFor);
    let blob = $messageStore[0].slice(0, $messageStore[0].size, binaryType);
    let temp = new Image();
    temp.src = URL.createObjectURL(blob);

    if (binaryFor === "ColorManaged") {
      console.log("Setting color managed image dataURL:", temp.src.substring(0, 50));
      viewState.update(state => ({
        ...state,
        colorManagedImage: {
          dataURL: temp.src,
          name: binaryName,
        }
      }));
      console.log("Setting loading to false");
      loading = false;
    }
    binaryType = null;
    binaryName = null;
    binaryID = null;
    binaryFor = null;
  }

  function colorManagedImage() {
    let rand = Math.floor(Math.random() * 999999);
    processState.update(state => ({
      ...state,
      CMID: rand
    }));
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
    viewState.update(state => ({
      ...state,
      projectKey: mainfilePath[0]
    }));
  }

  let isFullScreen = window.innerHeight == screen.height;

  function closeImage() {
    viewState.update(state => ({
      ...state,
      projectKey: null,
      colorManagedImage: {
        ...state.colorManagedImage,
        dataURL: ""
      }
    }));
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
