<script lang="ts">
  import { currentPage, processState } from "@util/stores";
  import placeholder from "@assets/placeholder.jpg";
  import OpenSeadragon from "openseadragon";
  import { onDestroy, onMount } from "svelte";
  import Loader from "@components/Loader.svelte";
  let viewer: any = false;

  export let rows = 10;
  export let cols = 14;

  let pressPos = { top: 0, bottom: 0, left: 0, right: 0 };
  export let currentPos = { top: 0, bottom: 0, left: 0, right: 0 };
  let viewportPoint;
  let imagePoint;

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
      viewer.open({
        type: "image",
        url: placeholder,
      });

      overlay();
    }
  } else {
    if (viewer) {
      viewer.close();
    }
  }
  // $: if (viewer) {
  //   viewer.addHandler("canvas-enter", function (event) {
  //     viewportPoint = viewer.viewport.pointFromPixel(event.position);
  //   });
  // }
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

      new OpenSeadragon.MouseTracker({
        // userData: "selectorBox.Tracker",
        element: "selectorBox",
        // preProcessEventHandler: function (eventInfo) {
        //   switch (eventInfo.eventType) {
        //     case "pointerdown":
        //     case "pointerup":
        //       // prevent drag, click, pinch, etc. gestures on the viewer
        //       // when events bubble, preventDefault true indicates to viewer
        //       //    that we handled the events
        //       eventInfo.preventDefault = true;
        //       break;
        //     case "contextmenu":
        //       // prevent context menu from popping up
        //       eventInfo.preventDefault = true;
        //       break;
        //     default:
        //       break;
        //   }
        // },
        // clickHandler: function (e) {
        //   console.log(e.position);
        // },
        pressHandler: function (e) {
          var overlay = viewer.getOverlayById("selectorBox");

          // Overlay box coords
          let topPos = overlay.bounds.y;
          let botPos = overlay.bounds.y + overlay.bounds.height;
          let leftPos = overlay.bounds.x;
          let rightPos = overlay.bounds.x + overlay.bounds.width;

          // Event Relative coords
          let offset = viewer.viewport.deltaPointsFromPixels(e.position);
          let eViewX = leftPos + offset.x;
          let eViewY = topPos + offset.y;

          // Event Distance to Edge
          pressPos = {
            top: Math.abs(eViewY - topPos),
            left: Math.abs(eViewX - leftPos),
            right: Math.abs(eViewX - rightPos),
            bottom: Math.abs(eViewY - botPos),
          };
        },
        dragHandler: function (e) {
          var overlay = viewer.getOverlayById("selectorBox");

          let viewDeltaPoint = viewer.viewport.deltaPointsFromPixels(e.delta);

          let box = new OpenSeadragon.Rect(
            overlay.bounds.x,
            overlay.bounds.y,
            overlay.width,
            overlay.height
          );
          if (pressPos.right < 0.008) {
            box.width += viewDeltaPoint.x;
          }
          if (pressPos.bottom < 0.008) {
            box.height += viewDeltaPoint.y;
          }
          if (pressPos.top < 0.008) {
            box.y += viewDeltaPoint.y;
            box.height -= viewDeltaPoint.y;
          }
          if (pressPos.left < 0.008) {
            box.x += viewDeltaPoint.x;
            box.width -= viewDeltaPoint.x;
          }
          if (
            pressPos.bottom > 0.008 &&
            pressPos.right > 0.008 &&
            pressPos.top > 0.008 &&
            pressPos.left > 0.008
          ) {
            box.x += viewDeltaPoint.x;
            box.y += viewDeltaPoint.y;
          }
          overlay.update(box);
          overlay.drawHTML(viewer.overlaysContainer, viewer.viewport);
          currentPos = {
            top: overlay.bounds.y,
            left: overlay.bounds.x,
            bottom: overlay.bounds.y + overlay.bounds.height,
            right: overlay.bounds.x + overlay.bounds.width,
          };
        },
      });
      new OpenSeadragon.MouseTracker({
        element: viewer.canvas,
        moveHandler: function (e) {
          viewportPoint = viewer.viewport.pointFromPixel(e.position);
          imagePoint =
            viewer.viewport.viewportToImageCoordinates(viewportPoint);
        },
      });
    }
  }

  $: if (viewer) {
    document.getElementById(
      "gridBox"
    ).style.gridTemplateColumns = `repeat(${cols}, auto)`;
  }
</script>

<main>
  <!-- <div class="load"><Loader /></div> -->
  <p>
    top: {viewportPoint?.y.toFixed(3)} ({imagePoint?.y.toFixed(2)} px) | left: {viewportPoint?.x.toFixed(
      3
    )} ({imagePoint?.x.toFixed(2)} px)
  </p>
  <div id="color-seadragon-viewer" />
  <div id="selectorBox">
    <div id="gridBox">
      {#each [...Array(rows * cols).keys()].map((i) => i + 1) as i}
        <div class="line"><div class="target" /></div>
      {/each}
    </div>
    <div id="tl" />
    <div id="tr" />
    <div id="bl" />
    <div id="br" />
  </div>
  <div id="rowCol">
    <p>
      top: {currentPos.top.toFixed(4)} | left: {currentPos.left.toFixed(4)} | bottom:
      {currentPos.bottom.toFixed(4)} | right: {currentPos.right.toFixed(4)}
    </p>
  </div>
</main>

<style lang="postcss">
  main {
    @apply w-full h-[90%] ring-1 ring-gray-800 bg-gray-900/50 aspect-[3/2] shadow-lg flex flex-col;
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

    @apply bg-transparent relative ring-2 ring-blue-700;
  }
  #gridBox {
    display: grid;
    grid-template-columns: auto;
    @apply w-full h-full absolute overflow-hidden;
  }
  .line {
    background: radial-gradient(
      circle at center,
      transparent 80%,
      rgba(59, 131, 246, 0.35) 80%
    );

    @apply w-full h-full border-[2px] border-blue-700
      flex items-center justify-center;
  }
  .target {
    @apply border-[3px] border-blue-700/50 w-1/2 h-1/2 rounded-full;
  }
  button {
    @apply z-50;
  }
  #tl {
    @apply absolute h-3 aspect-square bg-blue-800/50 -top-1.5 -left-1.5 cursor-nw-resize;
  }
  #tr {
    @apply absolute h-3 aspect-square bg-blue-800 -top-1.5 -right-1.5 cursor-ne-resize;
  }
  #bl {
    @apply absolute h-3 aspect-square bg-blue-800 -bottom-1.5 -left-1.5 cursor-ne-resize;
  }
  #br {
    @apply absolute h-3 aspect-square bg-blue-800 -bottom-1.5 -right-1.5 cursor-nw-resize;
  }

  #rowCol {
    @apply bg-transparent flex;
  }

  #rowCol > input {
    @apply bg-black w-12;
  }
  #rowCol > p {
    @apply bg-black flex px-2;
  }
  /* width */
  ::-webkit-scrollbar {
    @apply relative transition-all w-2 h-2;
  }

  /* Track */
  ::-webkit-scrollbar-track {
    @apply bg-transparent;
  }

  /* Handle */
  ::-webkit-scrollbar-thumb {
    @apply bg-red-500 rounded-full;
  }

  /* Handle on hover */
  ::-webkit-scrollbar-thumb:hover {
    @apply bg-red-500;
  }
</style>
