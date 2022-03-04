<script lang="ts">
  import {
    currentPage,
    messageStore,
    processState,
    sendMessage,
  } from "@util/stores";
  import { currentPage, processState } from "@util/stores";
  import placeholder from "@assets/placeholder.jpg";
  import OpenSeadragon from "openseadragon";
  import { onDestroy, onMount } from "svelte";
  import Loader from "@components/Loader.svelte";
  let viewer;
  let imageUrl;

  const createViewer = () => {
    viewer = OpenSeadragon({
      id: "image-seadragon-viewer",
      prefixUrl: "/openseadragon/images/",
      immediateRender: true,
      preload: true,
      showNavigator: false,
      minZoomLevel: 0.5,
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
  };

  onDestroy(() => {
    if (viewer) {
      viewer.destroy();
      viewer = null;
      console.log("Image viewer destroyed");
    }
  });

  const destoryViewer = () => {
    if (viewer) {
      viewer.destroy();
      viewer = null;
      console.log("Image viewer destroyed");
    }
  };

  onMount(() => {
    console.log("Image viewer Mount");
    createViewer();
  });
  onDestroy(() => {
    console.log("Image viewer Destroy");
    destoryViewer();
  });

  // $: if (viewer && $processState.artStacks[0].colorTargetImage?.dataURL) {
  //   // console.log($processState.artStacks[0].colorTargetImage);
  //   // imageUrl = $processState.artStacks[0].colorTargetImage?.dataURL;

  //   viewer.open({
  //     type: "image",
  //     url: imageUrl,
  //   });
  // }
  $: if ($processState.currentTab === 5) {
    if (viewer && !viewer.isOpen()) {
      console.log("Opening Image");
      console.log(viewer.isOpen());
      viewer.open({
        type: "image",
        url: imageUrl,
      });
    }
  } else {
    if (viewer) {
      viewer.close();
    }
  }

  $: if (viewer) {
    // console.log($processState.artStacks[0].colorTargetImage);
    console.log("New Image (Image Viewer)");
    let temp = new Image();
    temp.src = $processState.outputImage?.dataURL;
    if (imageUrl === temp.src) {
      console.log("skip (image viewer)");
    } else {
      imageUrl = temp.src;
      viewer.open({
        type: "image",
        url: imageUrl,
      });
    }
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
