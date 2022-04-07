<script lang="ts">
  import SpecPickViewer from "@components/SpectralPicker/SpecPickViewer.svelte";
  import {
    processState,
    viewState,
    sendMessage,
    messageStore,
    currentPage,
  } from "@util/stores";
  import { fly } from "svelte/transition";
  import LineChart from "@components/Charts/LineChart.svelte";
  import Switch from "@components/Switch.svelte";
  import FileSelector from "@components/FileSelector.svelte";
  import { fullScreenApi } from "openseadragon";

  let brushShow = false;
  let size;
  let shadowPos = { left: 0, top: 0 };
  let spectrumData;
  let mainfilePath;

  let loading = false;

  let currentTab;

  let expand = false;

  let wavelengthArray = Array.from({ length: 35 }, (x, i) => i * 10 + 380);

  $: console.log(shadowPos);

  $: console.log($viewState);

  function getData() {
    console.log("Fetching Spec Data");
    if ($viewState.projectKey !== null) {
      console.log("Found Key");
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
      console.log(msg);
      sendMessage(JSON.stringify(msg));
    }
  }

  $: if (shadowPos !== null) {
    console.log("Getting Est Spectrum Data");
    getData();
  }

  $: if ($messageStore.length > 1) {
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
    let rand =
      (Object.keys($viewState.colorManagedImages).length + 1000) * 1000 +
      Math.floor(Math.random() * 999);
    currentTab = rand;
    $viewState.colorManagedImages[rand] = { projectKey: $viewState.projectKey };
    let msg = {
      RequestID: rand,
      RequestType: "ColorManagedImage",
      RequestData: {
        name: $viewState.projectKey,
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
    Object.keys($viewState.colorManagedImages).length === 0
  ) {
    console.log("Getting FIRST Color Managed Image");
    // CALL FOR CM
    colorManagedImage();
  }

  $: if (mainfilePath?.length > 0) {
    console.log("New Project Key");
    $viewState.projectKey = mainfilePath[0];
  }

  $: console.log({ DATA: spectrumData });

  $: console.log(Object.keys($viewState.colorManagedImages));

  const openDialog = async () => {
    let ipcResponse = await window.electron.handle({
      type: "File",
      filter: "project",
    });
    console.log(ipcResponse);
    $viewState.projectKey = ipcResponse.filePaths[0];
    colorManagedImage();
  };

  async function handleTab(tab) {
    if (tab === "+") {
      console.log("New Project");
      openDialog();
    } else {
      console.log("Switch Image");
      $viewState.projectKey = $viewState.colorManagedImages[tab].projectKey;
      currentTab = tab;
    }
  }

  $: if (currentTab === undefined) {
    currentTab = Object.keys($viewState.colorManagedImages)[0];
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
    <div class="image-tabs">
      <button
        class="fullBtn"
        on:click={() => {
          if (window.innerHeight == screen.height) {
            document.exitFullscreen();
          } else {
            document.querySelector(".image-tabs").requestFullscreen();
          }
        }}>FULL</button
      >
      <div class="tabs">
        {#each [...Object.keys($viewState.colorManagedImages), "+"] as tab, i}
          <button
            class="tab"
            class:selected={currentTab == tab}
            on:click={() => handleTab(tab)}
          >
            {#if tab !== "+"}
              {i + 1}
            {:else}
              {tab}
            {/if}
          </button>
        {/each}
      </div>
      {#if currentTab !== undefined}
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
              <Switch label="Enable Spectrum Picker" bind:checked={brushShow} />
              <div class="sizeSettings">
                Set Brush Size:
                <div class="flex justify-center items-center gap-1">
                  <input
                    type="range"
                    min="0.001"
                    max="0.1"
                    step=".0005"
                    bind:value={size}
                  />
                  <input
                    class="numberInput"
                    type="number"
                    min="0.001"
                    max="0.1"
                    step=".0005"
                    bind:value={size}
                  />
                </div>
              </div>
            </div>
            <div id="chart">
              <LineChart bind:data={spectrumData} bind:wavelengthArray />
            </div>
          </div>

          <SpecPickViewer
            bind:dataURL={$viewState.colorManagedImages[currentTab].dataURL}
            bind:shadowPos
            bind:show={brushShow}
            bind:size
            bind:loading
          />
        </div>
      {/if}
    </div>
    {#if !expand}
      <div
        class="side"
        in:fly={{ x: -200, duration: 500 }}
        out:fly={{ x: -200, duration: 500 }}
      >
        <div class="box" id="brush">
          <Switch label="Enable Spectrum Picker" bind:checked={brushShow} />
          <input
            type="range"
            min="0.001"
            max="0.1"
            step=".0005"
            bind:value={size}
          />
          <input
            class="numberInput"
            type="number"
            min="0.001"
            max="0.1"
            step=".0005"
            bind:value={size}
          />
        </div>
        <div id="chart">
          <LineChart bind:data={spectrumData} bind:wavelengthArray />
        </div>
      </div>
    {/if}
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex h-full w-full justify-center flex-col relative;
  }
  .noFile {
    @apply absolute w-full h-full z-50 flex justify-center bg-gray-800
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
  .image-container {
    @apply relative w-full h-full bg-gray-500 overflow-hidden;
  }
  .image {
    @apply bg-green-400 overflow-auto h-[90%] aspect-[3/2];
  }
  .image-tabs {
    @apply w-full h-[90%] bg-red-500/50 rounded-t-lg overflow-auto relative 
    flex flex-col;
  }
  .tabs {
    @apply w-auto h-8 bg-gray-700 flex gap-1 pt-1 px-1 rounded-t-lg;
  }
  .tab {
    @apply w-16 h-full bg-gray-700 flex justify-center items-center
          rounded-t-xl rounded-b-none focus:ring-0;
  }

  .floatBox {
    @apply absolute h-auto w-[30vw] bg-gray-600/75 z-50 right-0 transition-all duration-500
            translate-x-0;
  }

  .notExpanded {
    @apply translate-x-full;
  }

  .loading {
    @apply bg-gray-700 absolute w-full h-full z-50 flex justify-center items-center;
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
    @apply w-1/3;
  }

  .fullBtn {
    @apply absolute right-0 m-1 z-50;
  }

  #chart {
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
    @apply bg-gray-800 h-12 w-8 absolute bottom-1/2 -left-8 flex justify-center items-center
							text-2xl rounded-l-full border-l-2 border-t-2 border-b-2 border-gray-700;
  }
  .sizeSettings {
    @apply flex justify-between items-center text-base pl-4;
  }
</style>
