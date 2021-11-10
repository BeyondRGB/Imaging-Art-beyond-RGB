<script>
  import CanvasImage from "@components/CanvasImage.svelte";
</script>

<main>
  <div id="image">
    <CanvasImage />
  </div>
  <div id="side">
    <div
      class="box dark:bg-gray-800 bg-gray-100 text-black dark:text-gray-200 ring-1 ring-gray-300 dark:ring-gray-900"
      id="view-options"
    >
      View Options
      <p>[WIP]</p>
      <p>{"<- Use the mouse wheel to zoom and click & drag to pan"}</p>
    </div>
    <div
      class="box dark:bg-gray-800 bg-gray-100 text-black dark:text-gray-200 ring-1 ring-gray-300 dark:ring-gray-900"
      id="metadata"
    >
      Metadata
      <ul class="text-black dark:text-gray-50">
        <li>Dimensions: 5184 x 3456</li>
        <li>Size: 20.9 MB</li>
        <li>Camera: EOS 60D</li>
        <li>ISO: 100</li>
        <li>F-stop: f/10</li>
        <li>Exposure: 1/4 sec.</li>
        <li>Focal length: 41mm</li>
      </ul>
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex w-full justify-center;
  }
  #image {
    @apply w-[75%] self-center mx-2;
  }
  #side {
    @apply w-[25%];
  }
  /* #view-options {
    @apply bg-gray-200 h-[15%] m-2;
  } */
  #metadata {
    @apply overflow-auto h-[15vh];
  }

  ul {
    @apply ml-3;
  }

  .box {
    @apply h-[15%] m-2 shadow-md px-2 pt-1;
  }
</style>
