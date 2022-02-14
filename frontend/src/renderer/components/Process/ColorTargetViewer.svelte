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
  export let hue;

  let imageUrl = placeholder;

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
        url: imageUrl,
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
        location: new OpenSeadragon.Rect(
          $processState.artStacks[0].colorTarget.left,
          $processState.artStacks[0].colorTarget.top,
          $processState.artStacks[0].colorTarget.right -
            $processState.artStacks[0].colorTarget.left,
          $processState.artStacks[0].colorTarget.bottom -
            $processState.artStacks[0].colorTarget.top
        ),
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
    ).style.gridTemplateRows = `repeat(${rows}, auto)`;
  }

  $: if (viewer) {
    // console.log($processState.artStacks[0].colorTargetImage);
    let temp = new Image();
    temp.src = $processState.artStacks[0].colorTargetImage?.dataURL;
    imageUrl = temp.src;
  }

  $: if (hue) {
    let root = document.documentElement;
    root.style.setProperty("--hue", `${hue}`);
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
    <div class="layout">
      <div id="gridBox">
        {#each [...Array(rows * cols).keys()].map((i) => i + 1) as i}
          <div class="line">
            <div class="target" />
            <span class="targetNum">{i}</span>
          </div>
        {/each}
      </div>

      <button class="exp top" on:click={() => console.log("Hi")} />
      <button class="exp left" on:click={() => console.log("Hi")} />
      <button class="exp right" on:click={() => console.log("Hi")} />
      <button class="exp bottom" on:click={() => console.log("Hi")} />
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
  :root {
    --hue: 50;
  }

  main {
    @apply w-full h-[90%] ring-1 ring-gray-800 bg-gray-900/50 aspect-[3/2] shadow-lg flex flex-col;
  }
  #color-seadragon-viewer {
    @apply h-full w-full;
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
    grid-template-rows: auto;
    grid-auto-flow: column;
    grid-area: box;
    @apply w-full h-full overflow-hidden z-40;
  }
  .line {
    background: radial-gradient(
      circle at center,
      transparent 80%,
      hsla(var(--hue), 100%, 50%, 0.35) 80%
    );
    border-color: hsl(var(--hue), 100%, 50%);

    @apply w-full h-full border-[2px] flex items-center justify-center relative overflow-hidden z-10;
  }
  .layout {
    display: grid;
    grid-template-rows: 14% auto 14%;
    grid-template-columns: 14% auto 14%;
    /* grid-auto-flow: column; */
    grid-template-areas:
      "top top top"
      "left box right"
      "bottom bottom bottom";
    @apply w-[140%] h-[140%] absolute -top-[20%] -left-[20%] overflow-hidden;
  }
  .exp {
    @apply bg-transparent hover:bg-green-400/50 border-2 
          border-gray-600/25 hover:border-green-500 z-0 w-full h-full
          duration-500 transition-all delay-150 ease-in;
  }
  .top {
    grid-area: top;
    @apply w-[70%] ml-[15%];
  }
  .left {
    grid-area: left;
  }
  .right {
    grid-area: right;
  }
  .bottom {
    grid-area: bottom;
    @apply w-[70%] ml-[15%];
  }
  .target {
    border-color: hsla(var(--hue), 100%, 50%, 0.5);
    @apply border-[3px] w-[75%] h-[75%] rounded-full;
  }
  #tl {
    background-color: hsla(var(--hue), 100%, 50%, 0.75);
    @apply absolute h-3 aspect-square -top-1.5 -left-1.5 cursor-nw-resize z-50;
  }
  #tr {
    background-color: hsl(var(--hue), 100%, 50%);
    @apply absolute h-3 aspect-square -top-1.5 -right-1.5 cursor-ne-resize z-50;
  }
  #bl {
    background-color: hsl(var(--hue), 100%, 50%);
    @apply absolute h-3 aspect-square -bottom-1.5 -left-1.5 cursor-ne-resize z-50;
  }
  #br {
    background-color: hsl(var(--hue), 100%, 50%);
    @apply absolute h-3 aspect-square -bottom-1.5 -right-1.5 cursor-nw-resize z-50;
  }
  #selectorBox {
    @apply filter;
  }
  #rowCol {
    @apply bg-transparent flex;
  }
  .targetNum {
    color: hsla(var(--hue), 100%, 50%, 0.75);
    @apply absolute top-0 left-0 p-1 text-base font-semibold;
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
