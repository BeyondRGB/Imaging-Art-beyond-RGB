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
  let brushShow = false;
  let size;
  let shadowPos = { left: 0, top: 0 };
  let spectrumData;

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
    console.log($messageStore[0]);
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
    $viewState.colorManagedID = Math.floor(Math.random() * 999999999);
    let msg = {
      RequestID: $viewState.colorManagedID,
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
    $viewState.projectKey?.length > 1 &&
    $viewState.colorManagedID === null &&
    $viewState.colorManagedImage?.filename?.length === 0
  ) {
    console.log("Getting Color Managed Image");
    // CALL FOR CM
    colorManagedImage();
  }

  $: console.log({ DATA: spectrumData });
</script>

<main>
  <div class="flex w-full justify-center h-full">
    <div id="image">
      <SpecPickViewer bind:shadowPos bind:show={brushShow} bind:size />
    </div>
    <div id="side">
      <input bind:value={$viewState.projectKey} />
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
    @apply flex w-full justify-center flex-col;
  }
  #image {
    @apply w-[75%] self-center mx-2 mt-1;
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
  input {
    @apply p-0.5 bg-gray-900 border-[1px] border-gray-800 rounded-lg
          focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2;
  }
</style>
