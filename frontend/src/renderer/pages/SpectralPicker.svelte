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
    XCircleIcon,
    Maximize2Icon,
    Minimize2Icon,
    ChevronLeftIcon,
    ChevronRightIcon,
  } from "svelte-feather-icons";
  import { fly } from "svelte/transition";
  import LineChart from "@components/Charts/LineChart.svelte";
  import SwitchRow from "@components/SwitchRow.svelte";
  import Card from "@components/Card.svelte";
  import FileSelector from "@components/FileSelector.svelte";
  import { fullScreenApi } from "openseadragon";
  import EmptyState from "@components/EmptyState.svelte";

  let brushShow = false;
  let stackCurves = false;
  let size = 0.01; // Default size matching SpecPickViewer default
  let trueSize;
  let shadowPos = { left: 0, top: 0 };
  let trueShadowPos = { left: 0, top: 0 };
  let spectrumData = []; // Initialize as empty array
  let mainfilePath;

  let loading = false;

  let expand = false; // Start collapsed like master

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

  // Fetch initial spectrum data when image is loaded
  $: if (
    $currentPage === "SpecPicker" &&
    $viewState.projectKey !== null &&
    $viewState.colorManagedImage.dataURL.length > 0 &&
    (!spectrumData || spectrumData.length === 0)
  ) {
    console.log("Fetching initial spectrum data");
    getData();
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
    <div class="noFile">
      <div class="inputBox">
        <h2>Select a project file to import into BeyondRGB</h2>
        <FileSelector bind:filePaths={mainfilePath} filter="project" />
      </div>
    </div>
  {/if}
  <div class="content" id="picker-content">
    <div class="viewer-layout">
      <div class="image-section">
        <button
          class="fullBtn"
          on:click={() => {
            if (isFullScreen) {
              document.exitFullscreen();
              isFullScreen = false;
            } else {
              document.querySelector(".image-section").requestFullscreen();
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

      <div class="drawer-wrapper" class:open={expand}>
        <button class="drawer-toggle" on:click={() => (expand = !expand)}>
          {#if expand}
            <ChevronRightIcon size="1.25x" />
          {:else}
            <ChevronLeftIcon size="1.25x" />
          {/if}
        </button>
        
        <div class="drawer-panel">
          <div class="spectral-panel">
            <div class="panel-section">
              <SwitchRow
                label="Enable Spectral Picker"
                description="Show the sampling brush on the image preview"
                bind:checked={brushShow}
                ariaLabel="Toggle spectral picker"
              />
              <SwitchRow
                label="Stack Spectral Curves"
                description="Keep previous spectra visible when sampling"
                bind:checked={stackCurves}
                ariaLabel="Toggle stacking spectral curves"
              />
            </div>

            <div class="panel-section brush-section">
              <div class="panel-heading">Brush Size</div>
              <div class="brush-controls">
                <label class="brush-label" for="brush-size">Set brush size</label>
                <div class="brush-slider">
                  <input
                    id="brush-size"
                    class="brushBar"
                    type="range"
                    min="0.001"
                    max="0.03"
                    step=".0005"
                    bind:value={size}
                  />
                  <div class="pixSize">
                    <span>{trueSize ? parseFloat(trueSize).toFixed(1) : '0.0'}</span>
                    <span>px</span>
                  </div>
                </div>
              </div>
            </div>

            <div class="panel-section chart-section">
              <Card variant="elevated" padding="md" rounded={true} shadow="medium" className="chart-card">
                <LineChart
                  bind:data={spectrumData}
                  bind:wavelengthArray
                  bind:trueShadowPos
                  stack={stackCurves}
                />
              </Card>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex h-full w-full justify-center flex-col relative;
  }
  .noFile {
    background-color: var(--color-overlay-heavy);
    @apply absolute w-full h-full z-[99] flex justify-center items-center;
  }
  .inputBox {
    background-color: var(--color-surface);
    @apply w-auto h-auto flex flex-col gap-2 justify-center items-center
          p-8 rounded-2xl;
  }
  .inputBox h2 {
    @apply text-xl;
  }
  .content {
    @apply w-full h-full flex justify-center items-center p-6;
  }
  
  .viewer-layout {
    @apply w-full h-full flex gap-0;
  }
  
  .image-section {
    background-color: var(--color-surface-sunken);
    @apply relative flex-1 flex flex-col transition-all duration-500;
  }
  
  .image-container {
    @apply relative w-full h-full overflow-visible;
  }
  .pixSize {
    background-color: var(--color-surface);
    border: 1px solid var(--color-border);
    @apply flex gap-1 px-2 py-1 rounded-lg text-sm font-medium;
  }
  .brushBar {
    @apply w-full h-2 rounded-xl;
  }
  .brushBar::-webkit-slider-thumb {
    background-color: var(--color-interactive);
    @apply w-4 h-4 cursor-pointer rounded-full outline outline-1;
    outline-color: var(--color-border);
  }

  .drawer-wrapper {
    @apply relative flex h-full transition-all duration-500;
    width: 0;
  }
  
  .drawer-wrapper.open {
    width: 30vw;
  }
  
  .drawer-toggle {
    background-color: var(--color-surface);
    color: var(--color-text-primary);
    border: 1px solid var(--color-border);
    border-right: 0;
    box-shadow: -2px 0 4px rgba(0, 0, 0, 0.1);
    @apply absolute top-1/2 -translate-y-1/2 h-12 w-10 flex justify-center 
           items-center cursor-pointer transition-all duration-500 z-10 rounded-l-full;
    left: -2.5rem;
  }
  
  .drawer-toggle:hover {
    background-color: var(--color-interactive-hover);
  }
  
  .drawer-toggle:focus {
    outline: 2px solid var(--color-border-focus);
    outline-offset: 2px;
  }
  
  .drawer-panel {
    background-color: var(--color-surface-elevated);
    border-left: 1px solid var(--color-border);
    @apply flex-1 h-full overflow-y-auto;
  }

  .spectral-panel {
    @apply flex flex-col gap-4 p-4 h-full;
  }

  .panel-section {
    @apply flex flex-col gap-4;
  }

  .brush-section {
    background-color: var(--color-surface-sunken);
    border: 1px solid var(--color-border);
    @apply rounded-lg p-4 gap-3;
  }

  .panel-heading {
    color: var(--color-text-primary);
    @apply text-sm font-semibold uppercase tracking-wide;
  }

  .brush-controls {
    @apply flex flex-col gap-3;
  }

  .brush-label {
    color: var(--color-text-secondary);
    @apply text-sm font-medium;
  }

  .brush-slider {
    @apply flex justify-center items-center gap-2;
  }
  
  .chart-section {
    @apply flex-1;
  }
  
  :global(.chart-card) {
    @apply h-full;
  }

  .loading {
    background-color: var(--color-overlay-heavy);
    @apply absolute w-full h-full z-[49] flex justify-center items-center;
  }
  .loading-box {
    @apply h-full flex flex-col gap-2 justify-center items-center text-2xl;
  }
  .loader {
    width: 48px;
    height: 48px;
    background: #11ff00;
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

  .fullBtn {
    @apply absolute right-0 m-1 z-50 p-1 bg-transparent ring-0
            hover:bg-blue-500/25 transition-all duration-500;
  }

  .closeBtn {
    @apply absolute right-8 m-1 z-50 p-1 bg-transparent ring-0
            hover:bg-red-500/25 transition-all duration-500;
  }
</style>
