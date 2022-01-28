<script lang="ts">
  import SpecPickViewer from "@components/SpectralPicker/SpecPickViewer.svelte";
</script>

<main>
  <div class="flex w-full justify-center h-full">
    <div id="image">
      <SpecPickViewer />
    </div>
    <div id="side">
      <div
        class="box dark:bg-gray-800 bg-gray-100 text-black dark:text-gray-200 ring-1 ring-gray-300 dark:ring-gray-900"
        id="brush"
      >
        Brush Size <p>[WIP]</p>
      </div>
      <div
        class="box dark:bg-gray-800 bg-gray-100 text-black dark:text-gray-200 ring-1 ring-gray-300 dark:ring-gray-900"
        id="refl-spec"
      >
        Reflectance Spectrum
        <p>[WIP]</p>
      </div>
      <div
        class="box dark:bg-gray-800 bg-gray-100 text-black dark:text-gray-200 ring-1 ring-gray-300 dark:ring-gray-900"
        id="compare"
      >
        Compare Estimated and Reference
        <p>[WIP]</p>
      </div>
    </div>
  </div>
  <div id="chart">Chart</div>
</main>

<style lang="postcss">
  main {
    @apply flex w-full justify-center flex-col;
  }
  #image {
    @apply w-[70%] self-center mx-2 mt-1;
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
    @apply bg-gray-300 h-full m-2;
  }

  .box {
    @apply h-[15%] m-2 shadow-md px-2 pt-1 bg-gray-200;
  }
</style>
