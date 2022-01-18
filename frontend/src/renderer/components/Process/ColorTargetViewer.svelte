<script lang="ts">
  import { currentPage, processState } from "@util/stores";
  import placeholder from "@assets/placeholder.jpg";
  import OpenSeadragon from "openseadragon";
  import { onDestroy, onMount } from "svelte";
  import Loader from "@components/Loader.svelte";
  let viewer;

  export let rows = 12;
  export let cols = 10;

  const createViewer = () => {
    viewer = OpenSeadragon({
      id: "color-seadragon-viewer",
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
  };

  const destoryViewer = () => {
    if (viewer) {
      viewer.destroy();
      viewer = null;
      console.log("Color target viewer destroyed");
    }
  };

  onMount(() => {
    createViewer();
  });
  onDestroy(() => {
    destoryViewer();
  });

  // onDestroy(() => {
  //   if (viewer) {
  //     destoryViewer();
  //   }
  // });
  $: if ($processState.currentTab === 4) {
    if (viewer) {
      setTimeout(() => {
        viewer.open({
          type: "image",
          url: placeholder,
        });
        overlay();

        let temp = viewer.getOverlayById("selectorBox");
        console.log(temp.getBounds());
      }, 550);
    }
  } else {
    if (viewer) {
      viewer.close();
    }
  }
  function overlay() {
    if (viewer) {
      let ele = document.querySelector("#selectorBox");
      document.getElementById(
        "gridBox"
      ).style.gridTemplateColumns = `repeat(${cols}, auto)`;
      viewer.addOverlay({
        element: ele,
        location: new OpenSeadragon.Rect(0.25, 0.25, 0.25, 0.25),
      });
    }
  }
</script>

<main>
  <!-- <div class="load"><Loader /></div> -->
  <div id="color-seadragon-viewer" />
  <div id="selectorBox">
    <div id="gridBox">
      {#each [...Array(rows * cols).keys()].map((i) => i + 1) as i}
        <div class="line" />
      {/each}
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply w-full h-[90%] ring-1 ring-gray-800 bg-gray-900/50 aspect-[3/2] shadow-lg;
  }
  #color-seadragon-viewer {
    @apply h-full w-full;
  }
  .load {
    @apply absolute left-1/2 bottom-1/2;
  }
  #selectorBox {
    /* background-size: 40px 40px;
    background-image: linear-gradient(to right, black 2px, transparent 1px),
      linear-gradient(to bottom, black 2px, transparent 1px),
      radial-gradient(circle, #000000 3px, rgba(0, 0, 0, 0) 1px); */

    @apply bg-transparent relative;
  }
  #gridBox {
    display: grid;
    grid-template-columns: auto auto auto;
    @apply w-full h-full absolute;
  }
  .line {
    background-color: rgba(255, 255, 255, 0.8);
    border: 1px solid rgba(0, 0, 0, 0.8);
    @apply w-full h-full;
  }
</style>
