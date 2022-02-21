<script lang="ts">
  import { currentPage, processState } from "@util/stores";
  import OpenSeadragon from "openseadragon";
  import { afterUpdate, onDestroy, onMount } from "svelte";
  let viewer;
  let mouseTracker;
  let colorOverlay;
  let verifyOverlay;

  let colorTracker;
  let verifyTracker;

  let pressPos = { top: 0, bottom: 0, left: 0, right: 0 };
  let viewportPoint;
  let imagePoint;

  export let colorTarget;
  export let verifyTarget;

  let imageUrl;

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
      preserveImageSizeOnResize: false,
      // zoomPerScroll: 1.5,
      visibilityRatio: 1,
    });
    mouseTracker = new OpenSeadragon.MouseTracker({
      element: viewer.canvas,
      moveHandler: function (e) {
        viewportPoint = viewer.viewport.pointFromPixel(e.position);
        imagePoint = viewer.viewport.viewportToImageCoordinates(viewportPoint);
      },
      scrollHandler: function (e) {
        console.log(e);
        console.log([viewer.viewport.getZoom(), viewer.viewport.getZoom(true)]);
      },
    });
  };

  const destoryViewer = () => {
    if (viewer) {
      viewer.destroy();
      viewer = null;
      mouseTracker.destroy();
      mouseTracker = null;
      console.log("Color target viewer destroyed");
    }
  };

  onMount(() => {
    console.log("Color target viewer Mount");
    createViewer();
  });
  onDestroy(() => {
    console.log("Color target viewer Destroy");
    destoryViewer();
  });

  $: if ($processState.currentTab === 4) {
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
    console.log(viewer.isOpen());
  }

  function removeOverlay(id) {
    console.log("Entering removeOverlay");
    const selBox = document.getElementById(`sBox-${id}`);
    const gridBox = document.getElementById(`gBox-${id}`);

    viewer.removeOverlay(selBox);
    if (id === 0) {
      colorOverlay = false;
      colorTracker.destroy();
    } else if (id === 1) {
      verifyOverlay = false;
      verifyTracker.destroy();
    }
  }

  function addOverlay(id) {
    console.log("Entering addOverlay");
    const selBox = document.getElementById(`sBox-${id}`);
    const gridBox = document.getElementById(`gBox-${id}`);
    if (viewer && selBox && gridBox) {
      if (id === 0) {
        gridBox.style.gridTemplateColumns = `repeat(${colorTarget.cols}, auto)`;
      } else if (id === 1) {
        gridBox.style.gridTemplateColumns = `repeat(${verifyTarget.cols}, auto)`;
      }
      console.log(`Adding Overlay ${id}`);
      viewer.addOverlay({
        element: selBox,
        location: new OpenSeadragon.Rect(
          $processState.artStacks[0].colorTargets[id].left,
          $processState.artStacks[0].colorTargets[id].top,
          $processState.artStacks[0].colorTargets[id].right -
            $processState.artStacks[0].colorTargets[id].left,
          $processState.artStacks[0].colorTargets[id].bottom -
            $processState.artStacks[0].colorTargets[id].top
        ),
      });

      if (id === 0) {
        colorOverlay = true;
        colorTracker = new OpenSeadragon.MouseTracker({
          element: `sBox-${id}`,
          pressHandler: function (e) {
            var overlay = viewer.getOverlayById(`sBox-${id}`);

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
            var overlay = viewer.getOverlayById(`sBox-${id}`);

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

            colorTarget.currentPos = {
              top: overlay.bounds.y,
              left: overlay.bounds.x,
              bottom: overlay.bounds.y + overlay.bounds.height,
              right: overlay.bounds.x + overlay.bounds.width,
            };
          },
        });
      } else if (id === 1) {
        verifyOverlay = true;
        verifyTracker = new OpenSeadragon.MouseTracker({
          element: `sBox-${id}`,
          pressHandler: function (e) {
            var overlay = viewer.getOverlayById(`sBox-${id}`);

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
            var overlay = viewer.getOverlayById(`sBox-${id}`);

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

            verifyTarget.currentPos = {
              top: overlay.bounds.y,
              left: overlay.bounds.x,
              bottom: overlay.bounds.y + overlay.bounds.height,
              right: overlay.bounds.x + overlay.bounds.width,
            };
          },
        });
      }
    }
  }

  $: if (viewer) {
    // console.log($processState.artStacks[0].colorTargetImage);
    console.log("New Image");
    let temp = new Image();
    temp.src = $processState.artStacks[0].colorTargetImage?.dataURL;
    if (imageUrl === temp.src) {
      console.log("skip");
    } else {
      imageUrl = temp.src;
    }
  }

  // ----------------------------------------
  $: if (colorTarget && !colorOverlay) {
    console.log("Color Target");
    setTimeout(() => {
      addOverlay(0);
    }, 200);
  }

  $: if (!colorTarget && colorOverlay) {
    console.log("Color Target REMOVE");

    removeOverlay(0);
  }

  $: if (colorTarget?.color) {
    let root = document.documentElement;
    root.style.setProperty("--color_hue", `${colorTarget.color}`);
  }

  $: if (colorTarget?.rows) {
    const gridBox = document.getElementById("gBox-0");
    if (gridBox) {
      gridBox.style.gridTemplateRows = `repeat(${colorTarget.rows}, auto)`;
    }
  }
  // --------------------------------------
  $: if (verifyTarget && !verifyOverlay) {
    console.log("Verify Target");
    setTimeout(() => {
      addOverlay(1);
    }, 200);
  }

  $: if (!verifyTarget && verifyOverlay) {
    console.log("Verify Target REMOVE");

    removeOverlay(1);
  }

  $: if (verifyTarget?.color) {
    let root = document.documentElement;
    root.style.setProperty("--verfiy_hue", `${verifyTarget.color}`);
  }

  $: if (verifyTarget?.rows) {
    const gridBox = document.getElementById("gBox-1");
    if (gridBox) {
      gridBox.style.gridTemplateRows = `repeat(${verifyTarget.rows}, auto)`;
    }
  }
</script>

<main>
  <p>
    top: {viewportPoint?.y.toFixed(3)} ({imagePoint?.y.toFixed(2)} px) | left: {viewportPoint?.x.toFixed(
      3
    )} ({imagePoint?.x.toFixed(2)} px)
  </p>
  <div id="color-seadragon-viewer" />
  {#if colorTarget}
    <div class="selectorBox" id={`sBox-0`}>
      <div class="layout">
        <div class="gridBox" id={`gBox-0`}>
          {#each [...Array(colorTarget.rows * colorTarget.cols).keys()].map((i) => i + 1) as boxIndex}
            <div class="line">
              <div class="target" />
              <span class="targetNum"><svg><text>{boxIndex}</text></svg></span>
            </div>
          {/each}
        </div>

        <!-- <div class="exp top group">
          <button class="inc">+</button>
        </div>
        <div class="exp left" />
        <div class="exp right" />
        <div class="exp bottom" /> -->
      </div>
      <div class="corner tl" />
      <div class="corner tr" />
      <div class="corner bl" />
      <div class="corner br" />
    </div>
  {/if}

  {#if verifyTarget}
    <div class="selectorBox ver" id={`sBox-1`}>
      <div class="layout">
        <div class="gridBox" id={`gBox-1`}>
          {#each [...Array(verifyTarget.rows * verifyTarget.cols).keys()].map((i) => i + 1) as boxIndex}
            <div class="line ver">
              <div class="target" />
              <span class="targetNum"><svg><text>{boxIndex}</text></svg></span>
            </div>
          {/each}
        </div>
        <!-- 
        <div class="exp top group">
          <button class="inc">+</button>
        </div>
        <div class="exp left" />
        <div class="exp right" />
        <div class="exp bottom" /> -->
      </div>
      <div class="corner tl" />
      <div class="corner tr" />
      <div class="corner bl" />
      <div class="corner br" />
    </div>
  {/if}

  <div id="rowCol">
    Color Target:
    <p>
      top: {colorTarget?.currentPos?.top?.toFixed(4)} | left: {colorTarget?.currentPos?.left?.toFixed(
        4
      )} | bottom:
      {colorTarget?.currentPos?.bottom?.toFixed(4)} | right: {colorTarget?.currentPos?.right?.toFixed(
        4
      )}
    </p>
  </div>
  <div id="rowCol">
    Verification Target:
    <p>
      top: {verifyTarget?.currentPos?.top?.toFixed(4)} | left: {verifyTarget?.currentPos?.left?.toFixed(
        4
      )} | bottom:
      {verifyTarget?.currentPos?.bottom?.toFixed(4)} | right: {verifyTarget?.currentPos?.right?.toFixed(
        4
      )}
    </p>
  </div>
</main>

<style lang="postcss">
  :root {
    --color_hue: 50;
    --verfiy_hue: 100;
  }

  main {
    @apply w-full h-[90%] ring-1 ring-gray-800 bg-gray-900/50 aspect-[3/2] shadow-lg flex flex-col;
  }
  #color-seadragon-viewer {
    @apply h-full w-full;
  }
  .selectorBox {
    /* background-size: 40px 40px;
    background-image: linear-gradient(to right, black 2px, transparent 1px),
      linear-gradient(to bottom, black 2px, transparent 1px),
      radial-gradient(circle, #000000 3px, rgba(0, 0, 0, 0) 1px); */

    @apply bg-transparent relative ring-2 ring-blue-700 filter;
  }
  .gridBox {
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
      hsla(var(--color_hue), 100%, 50%, 0.35) 80%
    );
    border-color: hsl(var(--color_hue), 100%, 50%);

    @apply w-full h-full border-[2px] flex items-center justify-center relative overflow-hidden z-10;
  }

  .ver {
    border-color: hsl(var(--verfiy_hue), 100%, 50%);
  }

  .ver .line {
    background: radial-gradient(
      circle at center,
      transparent 80%,
      hsla(var(--verfiy_hue), 100%, 50%, 0.35) 80%
    );
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
          duration-500 transition-all delay-150 ease-in flex justify-center;
  }
  .inc {
    font-size: 10vw;
    @apply bg-transparent text-transparent group-hover:text-white group-hover:bg-green-500
            align-middle justify-center flex items-center w-[10%] h-[50%];
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
    border-color: hsla(var(--color_hue), 100%, 50%, 0.5);
    @apply border-[3px] w-[75%] h-[75%] rounded-full;
  }

  .ver .target {
    border-color: hsla(var(--verfiy_hue), 100%, 50%, 0.5);
    @apply border-[3px] w-[75%] h-[75%] rounded-full;
  }

  .corner {
    background-color: hsla(var(--color_hue), 100%, 50%, 0.75);
    @apply absolute h-3 aspect-square z-50;
  }

  .ver .corner {
    background-color: hsla(var(--verfiy_hue), 100%, 50%, 0.75);
  }

  .tl {
    @apply -top-1.5 -left-1.5 cursor-nw-resize;
  }
  .tr {
    @apply -top-1.5 -right-1.5 cursor-ne-resize;
  }
  .bl {
    @apply -bottom-1.5 -left-1.5 cursor-ne-resize;
  }
  .br {
    @apply -bottom-1.5 -right-1.5 cursor-nw-resize;
  }
  #rowCol {
    @apply bg-transparent flex;
  }
  .targetNum {
    color: hsla(var(--color_hue), 100%, 50%, 0.75);
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
