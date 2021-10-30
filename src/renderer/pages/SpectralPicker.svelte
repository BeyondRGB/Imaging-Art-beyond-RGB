<script>
</script>

<main>
  <div class="flex w-full justify-center h-full">
    <div id="image">Image</div>
    <div id="side">
      Settings
      <div id="brush">Brush Size</div>
      <div id="refl-spec">Reflectance Spectrum</div>
      <div id="compare">Compare Estimated and Reference</div>
    </div>
  </div>
  <div id="chart">Chart</div>
</main>

<style lang="postcss">
  main {
    @apply flex w-full justify-center flex-col;
  }
  #image {
    @apply bg-yellow-600 h-[82%] min-w-[75%] self-center mx-2;
  }
  #side {
    @apply w-[25%];
  }
  #brush {
    @apply bg-gray-50 h-[15%] m-2;
  }
  #refl-spec {
    @apply bg-gray-50 h-[15%] m-2;
  }
  #compare {
    @apply bg-gray-50 h-[15%] m-2;
  }
  #chart {
    @apply bg-gray-100 h-auto w-full m-2;
  }
</style>
