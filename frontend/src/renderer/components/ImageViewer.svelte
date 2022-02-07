<script lang="ts">
  import { currentPage, processState } from "@util/stores";
  import placeholder from "@assets/placeholder.jpg";
  import OpenSeadragon from "openseadragon";
  import { onDestroy, onMount } from "svelte";
  import Loader from "@components/Loader.svelte";
  let viewer;
  let imageUrl;
  onMount(() => {
    viewer = OpenSeadragon({
      id: "image-seadragon-viewer",
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
      // tileSources: {
      //   type: "image",
      //   url: placeholder,
      // },
    });
  });

  onDestroy(() => {
    if (viewer) {
      viewer.destroy();
      viewer = null;
      console.log("Image viewer destroyed");
    }
  });

  // $: if (viewer && $processState.artStacks[0].colorTargetImage?.dataURL) {
  //   // console.log($processState.artStacks[0].colorTargetImage);
  //   // imageUrl = $processState.artStacks[0].colorTargetImage?.dataURL;

  //   viewer.open({
  //     type: "image",
  //     url: imageUrl,
  //   });
  // }

  $: if (viewer && $processState.outputImage?.dataURL) {
    // console.log($processState.artStacks[0].colorTargetImage);
    let temp = new Image();
    temp.src = $processState.outputImage?.dataURL;
    imageUrl = temp.src;

    viewer.open({
      type: "image",
      url: imageUrl,
    });
  }
</script>

<main>
  <!-- <div class="load"><Loader /></div> -->
  <div id="image-seadragon-viewer" />
</main>

<style lang="postcss">
  main {
    @apply w-full h-full ring-1 ring-gray-800 bg-gray-900/50 aspect-[3/2] shadow-lg;
  }
  #image-seadragon-viewer {
    @apply h-full w-full;
  }
  .load {
    @apply absolute left-1/2 bottom-1/2;
  }
</style>
