<script>
  import placeholder from "@assets/placeholder.jpg";
  import placeholder1 from "@assets/placeholderAlt1.jpg";
  import placeholder2 from "@assets/placeholderAlt2.jpg";
  import { afterUpdate, beforeUpdate, onDestroy, onMount } from "svelte";
  import { draggable } from "svelte-drag";
  import "@util/openseadragon-curtian-sync.js";

  export let isSync;
  export let imageKeys;
  export let showNav;

  let prevShowNav = showNav;
  let viewer;
  let useCanvas = true;

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

  $: if (viewer) {
    if (isSync) {
      viewer.setMode("sync");
    } else {
      viewer.setMode("curtain");
    }
  }
  $: {
    if (showNav !== prevShowNav) {
      destroy();
      makeViewer();
      prevShowNav = showNav;
    }
  }

  $: if (viewer) {
    imageKeys.forEach((ele, i) => viewer.setImageShown(`my-key-${i}`, ele));
  }

  onMount(() => {
    makeViewer();
  });

  function makeViewer() {
    viewer = new CurtainSyncViewer({
      container: document.querySelector("#seadragon-viewer"),

      images: [
        {
          key: "my-key-0",
          tileSource: {
            type: "image",
            url: placeholder,
            buildPyramid: false,
          },
          shown: true,
        },
        {
          key: "my-key-1",
          tileSource: {
            type: "image",
            url: placeholder1,
            buildPyramid: false,
          },
        },
        {
          key: "my-key-2",
          tileSource: {
            type: "image",
            url: placeholder2,
            buildPyramid: false,
          },
        },
      ],
      osdOptions: {
        showNavigator: showNav,
        navigatorMaintainSizeRatio: true,
        useCanvas,
        showZoomControl: false,
        showHomeControl: false,
        showFullPageControl: true,
        // preserveImageSizeOnResize: true,
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

  function enterFullScreen() {
    if (viewer) {
      viewer.enterFullScreen();
    }
  }

  onDestroy(() => {
    destroy();
  });
</script>

<main class="dark:ring-gray-600">
  <button on:click={() => enterFullScreen()}>Wide</button>
  <div id="seadragon-viewer" />
</main>

<style lang="postcss">
  main {
    @apply w-full relative ring-1 ring-gray-800 bg-gray-900/50;
  }

  #seadragon-viewer {
    @apply h-[90vh];
  }

  button {
    @apply absolute right-0 z-10;
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
