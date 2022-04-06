<script lang="ts">
  import SpecPickViewer from "@components/SpectralPicker/SpecPickViewer.svelte";
  import {
    processState,
    viewState,
    sendMessage,
    messageStore,
    currentPage,
  } from "@util/stores";
  import { slide } from "svelte/transition";
  import LineChart from "@components/Charts/LineChart.svelte";
  import Switch from "@components/Switch.svelte";
  import FileSelector from "@components/FileSelector.svelte";

  let brushShow = false;
  let size;
  let shadowPos = { left: 0, top: 0 };
  let spectrumData;
  let mainfilePath;

  let currentTab;

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
    let rand = Math.floor(Math.random() * 999999999);
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
  <div class="content">
    <div class="image-tabs">
      <div class="tabs">
        {#each [...Object.keys($viewState.colorManagedImages), "+"] as tab, i}
          <button
            class="tab"
            class:selected={currentTab == tab}
            on:click={() => handleTab(tab)}>{tab}</button
          >
        {/each}
        {currentTab}
      </div>
      {#if currentTab !== undefined}
        <div class="image">
          {currentTab}
          <SpecPickViewer
            dataURL={$viewState.colorManagedImages[currentTab].dataURL}
            bind:shadowPos
            bind:show={brushShow}
            bind:size
          />
        </div>
      {/if}
      <!-- <div class="image">
        <SpecPickViewer
          dataURL={$viewState.colorManagedImages.dataURL}
          bind:shadowPos
          bind:show={brushShow}
          bind:size
        />
      </div> -->
    </div>
    <div id="side">
      {$viewState.projectKey}
      <div class="box" id="brush">
        <Switch label="Estimate Spectrum Picker" bind:checked={brushShow} />
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
    @apply w-auto h-auto bg-red-600/10 flex flex-col gap-2;
  }
  .inputBox h2 {
    @apply text-lg;
  }
  .content {
    @apply w-full h-full flex justify-start items-center;
  }
  .image {
  }
  .image-tabs {
    @apply w-[75%] self-center mx-2 mt-1 bg-red-500 p-2;
  }
  .tabs {
    @apply w-auto h-8 bg-green-400 flex gap-2;
  }
  .tab {
    @apply w-24 h-full bg-blue-400 flex justify-center items-center
          rounded-t-xl;
  }
  .selected {
    @apply bg-red-400;
  }
  #side {
    @apply w-[25%];
  }
  /* #brush {
    @apply bg-gray-200 h-[15%] m-2;
  }
  #refl-spec {
    @apply bg-gray-200 h-[15%] m-2;
  }
  #compare {
    @apply bg-gray-200 h-[15%] m-2;
  } */
  #chart {
    @apply bg-gray-600 m-2 p-2 pb-4 rounded-lg;
  }

  .box {
    @apply m-2 shadow-md px-2 pt-1 bg-gray-600 rounded-lg p-2;
  }
  .numberInput {
    @apply p-0.5 bg-gray-900 border-[1px] border-gray-800 rounded-lg
          focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2;
  }
</style>
