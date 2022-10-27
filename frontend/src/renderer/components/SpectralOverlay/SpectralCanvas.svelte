<script>
  // import placeholder from "@assets/placeholder.jpg";
  // import placeholder1 from "@assets/placeholderAlt1.jpg";
  // import placeholder2 from "@assets/placeholderAlt2.jpg";
  import { afterUpdate, beforeUpdate, onDestroy, onMount } from "svelte";
  import { draggable } from "svelte-drag";

  export let isSync;
  export let imageKeys;
  export let showNav;

  let placeholder, placeholder1, placeholder2;

  let prevShowNav = showNav;
  let viewer;
  let useCanvas = true;

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
      container: document.querySelector("#spec-seadragon-viewer"),

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
        showFullPageControl: false,
        // preserveImageSizeOnResize: true,
        zoomPerScroll: 1.5,
        visibilityRatio: 1,
      },
    });
  }

  function destroy() {
    if (viewer) {
      viewer.destroy();
      viewer = null;
      console.log("Spectral Canvas Destoryed");
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
  <div id="spec-seadragon-viewer" />
</main>

<style lang="postcss">
  main {
    @apply w-full h-[90%] relative ring-1 aspect-[3/2] ring-gray-800 bg-gray-900/50;
  }

  #spec-seadragon-viewer {
    cursor: crosshair;
    @apply h-full w-full;
  }

  button {
    @apply absolute right-0 z-10;
  }
</style>
