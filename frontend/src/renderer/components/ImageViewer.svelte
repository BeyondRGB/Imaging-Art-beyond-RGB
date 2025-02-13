<script lang="ts">
  import { currentPage, processState } from "@util/stores";
  import OpenSeadragon from "openseadragon";
  import { onDestroy, onMount } from "svelte";
  let viewer;
  let imageUrl;
  let loading = true;
  export let srcUrl;
  export let identifier = "unique-identifier";
  let imageId = identifier+"-seadragon-viewer";

  const createViewer = () => {
    setTimeout(() => {
      viewer = OpenSeadragon({
      id: imageId,
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
      maxZoomPixelRatio: 30,
      zoomPerScroll: 1.15,
      visibilityRatio: 1,
      animationTime: 0.4,
      // tileSources: {
      //   type: "image",
      //   url: placeholder,
      // },
    });

    viewer.addHandler("zoom", handleZoom);
    }, 100);
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
  $: if ($processState.currentTab >= 6 || $currentPage === "Reports") {
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

  $: if (viewer && srcUrl) {
    // console.log($processState.artStacks[0].colorTargetImage);
    console.log("New Image (Image Viewer)");
    let temp = new Image();
    temp.src = srcUrl;

    imageUrl = temp.src;

    temp.onload = () => {
      loading = false;
      viewer.open({
            type: "image",
            url: imageUrl,
          });
    }
    
  }

  function handleZoom(e) {
    if (e.zoom > 10) {
      viewer.drawer.setImageSmoothingEnabled(false);
    } else {
      viewer.drawer.setImageSmoothingEnabled(true);
    }
  }
</script>

<main>
  <div class="image-seadragon-wrapper">
    {#if loading}
      <div class="loading">
        <div class="loading-box">Loading<span class="loader" /></div>
      </div>
    {:else}
      <div class="image-seadragon-viewer" id={imageId} />
    {/if}
  </div>
</main>

<style lang="postcss">
  main {
    @apply w-full h-full ring-1 ring-gray-800 bg-gray-900/50 aspect-[3/2] shadow-lg;
  }
  .image-seadragon-wrapper {
    @apply h-full w-full;
  }
  .image-seadragon-viewer {
    @apply h-full w-full;
  }
  .load {
    @apply absolute left-1/2 bottom-1/2;
  }


	.loading {
    @apply bg-gray-700 absolute w-full h-full z-[49] flex justify-center items-center;
  }
  .loading-box {
    @apply h-full flex flex-col gap-2 justify-center items-center
            text-2xl;
  }
  .loader {
    /* position: absolute; */
    width: 48px;
    height: 48px;
    background: #11ff00;
    transform: rotateX(65deg) rotate(45deg);
    /* remove bellows command for perspective change */
    transform: perspective(200px) rotateX(65deg) rotate(45deg);
    color: rgb(255, 0, 0);
    animation: layers1 1s linear infinite alternate;
    @apply z-50;
  }
  .loader:after {
    content: "";
    position: absolute;
    inset: 0;
    background: rgb(0, 0, 255);
    animation: layerTr 1s linear infinite alternate;
  }

  @keyframes layers1 {
    0% {
      box-shadow: 0px 0px 0 0px;
    }
    90%,
    100% {
      box-shadow: 20px 20px 0 -4px;
    }
  }
  @keyframes layerTr {
    0% {
      transform: translate(0, 0) scale(1);
    }
    100% {
      transform: translate(-25px, -25px) scale(1);
    }
  }
</style>
