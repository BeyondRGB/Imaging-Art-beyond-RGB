<script>
  import PageTransitions from "@components/PageTransitions.svelte";
  import CanvasImage from "@components/CanvasImage.svelte";
</script>

<PageTransitions>
  <main>
    <div class="flex w-full justify-center h-full">
      <div id="image">
        <CanvasImage />
      </div>
      <div id="side">
        <div id="brush">Brush Size</div>
        <div id="refl-spec">Reflectance Spectrum</div>
        <div id="compare">Compare Estimated and Reference</div>
      </div>
    </div>
    <div id="chart">Chart</div>
  </main>
</PageTransitions>

<style lang="postcss">
  main {
    @apply flex w-full justify-center flex-col;
  }
  #image {
    @apply w-[75%] self-center mx-2;
  }
  #side {
    @apply w-[25%];
  }
  #brush {
    @apply bg-gray-200 h-[15%] m-2;
  }
  #refl-spec {
    @apply bg-gray-200 h-[15%] m-2;
  }
  #compare {
    @apply bg-gray-200 h-[15%] m-2;
  }
  #chart {
    @apply bg-gray-300 h-auto m-2;
  }
</style>
