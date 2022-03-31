<script lang="ts">
  import SpecPickViewer from "@components/SpectralPicker/SpecPickViewer.svelte";
  import { slide } from "svelte/transition";
  import LineChart from "@components/Charts/LineChart.svelte";
  import Switch from "@components/Switch.svelte";
  let brushShow = false;
  let size;
</script>

<main>
  <div class="flex w-full justify-center h-full">
    <div id="image">
      <SpecPickViewer bind:show={brushShow} bind:size />
    </div>
    <div id="side">
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
        <LineChart />
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
</style>
