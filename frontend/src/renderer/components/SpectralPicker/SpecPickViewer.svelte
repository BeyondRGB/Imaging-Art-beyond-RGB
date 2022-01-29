<script lang="ts">
  import placeholder from "@assets/placeholder.jpg";
  import OpenSeadragon from "openseadragon";
  import { onDestroy, onMount } from "svelte";
  import Loader from "@components/Loader.svelte";
  let viewer;
  onMount(() => {
    viewer = OpenSeadragon({
      id: "specpick-seadragon-viewer",
      prefixUrl: "/openseadragon/images/",
      immediateRender: true,
      preload: true,
      showNavigator: false,
      minZoomLevel: 1,
      useCanvas: true,
      showZoomControl: false,
      showHomeControl: false,
      showFullPageControl: false,
      preserveImageSizeOnResize: true,
      // zoomPerScroll: 1.5,
      visibilityRatio: 1,
      tileSources: {
        type: "image",
        url: placeholder,
      },
    });
  });

  onDestroy(() => {
    if (viewer) {
      viewer.destroy();
      viewer = null;
      console.log("Image viewer destroyed");
    }
  });
</script>

<main>
  <!-- <div class="load"><Loader /></div> -->
  <div id="specpick-seadragon-viewer" />
</main>

<style lang="postcss">
  main {
    @apply w-full h-[90%] ring-1 ring-gray-800 bg-gray-900/50 aspect-[3/2] shadow-lg;
  }
  #specpick-seadragon-viewer {
    @apply h-full w-full;
  }
  .load {
    @apply absolute left-1/2 bottom-1/2;
  }
</style>
