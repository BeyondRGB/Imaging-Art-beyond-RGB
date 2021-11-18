<script>
  import placeholder from "@assets/placeholder.jpg";
  import placeholder1 from "@assets/placeholderAlt1.jpg";
  import placeholder2 from "@assets/placeholderAlt2.jpg";
  import { afterUpdate, beforeUpdate, onDestroy, onMount } from "svelte";
  import panzoom from "panzoom";
  import { draggable } from "svelte-drag";
  import OpenSeadragon from "openseadragon";
  import "@util/openseadragon-curtian-sync.js";

  let image;
  let panZoom = false;

  let viewer;
  let context;
  let handlePos;
  let useCanvas = true;
  let showNavigator = false;
  let curtian2 = false;
  let curtian3 = false;

  let images = [];
  // viewer = OpenSeadragon({
  //   id: "seadragon-viewer",
  //   prefixUrl: "/openseadragon/images/",
  //   immediateRender: false,
  //   showNavigator,
  //   minZoomLevel: 1,
  //   useCanvas,
  //   showZoomControl: false,
  //   showHomeControl: false,
  //   showFullPageControl: true,
  //   preserveImageSizeOnResize: true,
  //   zoomPerScroll: 1.5,
  //   visibilityRatio: 0.99,
  //   tileSources: {
  //     type: "image",
  //     url: "placeholder.jpg",
  //     buildPyramid: false,
  //   },
  // });

  onMount(() => {
    makeViewer();
  });

  function makeViewer() {
    viewer = new CurtainSyncViewer({
      container: document.querySelector("#seadragon-viewer"),

      images: [
        {
          key: "my-key-1",
          tileSource: {
            type: "image",
            url: placeholder,
            buildPyramid: false,
          },
          shown: true,
        },
        {
          key: "my-key-2",
          tileSource: {
            type: "image",
            url: placeholder1,
            buildPyramid: false,
          },
        },
        {
          key: "my-key-3",
          tileSource: {
            type: "image",
            url: placeholder2,
            buildPyramid: false,
          },
        },
      ],
      osdOptions: {
        showNavigator,
        useCanvas,
        showZoomControl: false,
        showHomeControl: false,
        showFullPageControl: true,
        preserveImageSizeOnResize: true,
        zoomPerScroll: 1.5,
        visibilityRatio: 0.99,
      },
    });
  }

  function destroy() {
    if (viewer) {
      viewer.destroy();
      viewer = null;
    }
  }

  onDestroy(() => {
    destroy();
  });

  function handleCurtain(id) {
    if (viewer) {
      if (id === 3) {
        if (curtian3) {
          viewer.setImageShown("my-key-2", false);
          viewer.setImageShown("my-key-3", false);
          viewer.setMode("sync");
          curtian3 = false;
        } else {
          viewer.setImageShown("my-key-2", true);
          viewer.setImageShown("my-key-3", true);
          viewer.setMode("curtain");
          curtian3 = true;
        }
      } else if (id === 2) {
        if (curtian2) {
          viewer.setImageShown("my-key-2", false);
          viewer.setMode("sync");
          curtian2 = false;
        } else {
          viewer.setImageShown("my-key-3", false);
          viewer.setImageShown("my-key-2", true);
          viewer.setMode("curtain");
          curtian2 = true;
        }
      }
    }
  }
</script>

<main class="dark:ring-gray-600">
  <button
    on:click={() => (useCanvas = !useCanvas)}
    class="bg-red-400 cursor-not-allowed"
    disabled>useCanvas: {useCanvas}</button
  >
  <button on:click={() => (showNavigator = !showNavigator)}
    >showNavigator: {showNavigator}</button
  >
  <button on:click={() => handleCurtain(2)}>curtain 2</button>

  <button on:click={() => handleCurtain(3)}>curtain 3</button>
  <div id="seadragon-viewer" />
</main>

<style lang="postcss">
  main {
    @apply w-full relative ring-1 ring-gray-800;
  }

  #seadragon-viewer {
    @apply h-[80vh];
  }

  #handle {
    @apply bg-red-300 h-12 w-6 rounded-full ring-2 ring-black;
  }
  #draggable {
    @apply bg-transparent w-6 absolute z-[1];
  }

  #stack {
    @apply bg-black flex;
  }
  /* img {
    flex: 0;
    @apply max-h-[85vh] w-auto;
  } */
  #image-div {
    position: absolute;
    border: 1px solid #b0b0b0;
    overflow: hidden;
    background-size: cover;
    background-repeat: no-repeat;
    background-clip: content-box;
    @apply h-full;
  }
  #seadragon-viewer {
    cursor: crosshair;
  }
</style>
