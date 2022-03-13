<script lang="ts">
  import { currentPage, processState } from "@util/stores";
  import OpenSeadragon from "openseadragon";
  import { afterUpdate, onDestroy, onMount } from "svelte";
  import { PlusIcon, MinusIcon } from "svelte-feather-icons";
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

  export let colorPos;
  export let verifyPos;

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
      maxZoomPixelRatio: 30,
      zoomPerScroll: 1.4,
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
    viewer.addHandler("zoom", handleZoom);
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

  function handleZoom(e) {
    if (e.zoom > 10) {
      console.log({ "Big Zoom": e });
      let drawer = viewer.drawer;
      drawer.setImageSmoothingEnabled(false);
    } else {
      let drawer = viewer.drawer;
      drawer.setImageSmoothingEnabled(true);
    }
  }

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
              ele: e.originalEvent.target,
              top: Math.abs(eViewY - topPos),
              left: Math.abs(eViewX - leftPos),
              right: Math.abs(eViewX - rightPos),
              bottom: Math.abs(eViewY - botPos),
            };
            e.originalEvent.path.forEach((element) => {
              if (element.classList?.length > 0) {
                if (element.classList[0] === "inc") {
                  console.log("increase");
                  if (
                    element.parentElement.classList[1] === "top" ||
                    element.parentElement.classList[1] === "bottom"
                  ) {
                    colorTarget.rows = colorTarget.rows + 1;
                  } else if (
                    element.parentElement.classList[1] === "left" ||
                    element.parentElement.classList[1] === "right"
                  ) {
                    colorTarget.cols = colorTarget.cols + 1;
                  }
                } else if (element.classList[0] === "dec") {
                  console.log("decrease");
                  if (
                    element.parentElement.classList[1] === "top" ||
                    element.parentElement.classList[1] === "bottom"
                  ) {
                    colorTarget.rows = colorTarget.rows - 1;
                  } else if (
                    element.parentElement.classList[1] === "left" ||
                    element.parentElement.classList[1] === "right"
                  ) {
                    colorTarget.cols = colorTarget.cols - 1;
                  }
                }
              }
            });
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

            if (pressPos.ele.classList[1] === "tl") {
              box.y += viewDeltaPoint.y;
              box.height -= viewDeltaPoint.y;
              box.x += viewDeltaPoint.x;
              box.width -= viewDeltaPoint.x;
            } else if (pressPos.ele.classList[1] === "tr") {
              box.y += viewDeltaPoint.y;
              box.height -= viewDeltaPoint.y;
              box.width += viewDeltaPoint.x;
            } else if (pressPos.ele.classList[1] === "bl") {
              box.height += viewDeltaPoint.y;
              box.x += viewDeltaPoint.x;
              box.width -= viewDeltaPoint.x;
            } else if (pressPos.ele.classList[1] === "br") {
              box.height += viewDeltaPoint.y;
              box.width += viewDeltaPoint.x;
            } else if (
              pressPos.ele.classList[0] === "line" ||
              pressPos.ele.classList[0] === "target" ||
              pressPos.ele.classList[0] === "verTarget"
            ) {
              box.x += viewDeltaPoint.x;
              box.y += viewDeltaPoint.y;
            }
            overlay.update(box);
            overlay.drawHTML(viewer.overlaysContainer, viewer.viewport);

            colorPos = {
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
            console.log("Verification press handler activeted");
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
              ele: e.originalEvent.target,
              top: Math.abs(eViewY - topPos),
              left: Math.abs(eViewX - leftPos),
              right: Math.abs(eViewX - rightPos),
              bottom: Math.abs(eViewY - botPos),
            };
            // console.log(e);
            // console.log(e.originalEvent.path);
            e.originalEvent.path.forEach((element) => {
              if (element.classList?.length > 0) {
                if (element.classList[0] === "inc") {
                  console.log("increase");
                  if (
                    element.parentElement.classList[1] === "top" ||
                    element.parentElement.classList[1] === "bottom"
                  ) {
                    verifyTarget.rows = verifyTarget.rows + 1;
                  } else if (
                    element.parentElement.classList[1] === "left" ||
                    element.parentElement.classList[1] === "right"
                  ) {
                    verifyTarget.cols = verifyTarget.cols + 1;
                  }
                } else if (element.classList[0] === "dec") {
                  console.log("decrease");
                  if (
                    element.parentElement.classList[1] === "top" ||
                    element.parentElement.classList[1] === "bottom"
                  ) {
                    verifyTarget.rows = verifyTarget.rows - 1;
                  } else if (
                    element.parentElement.classList[1] === "left" ||
                    element.parentElement.classList[1] === "right"
                  ) {
                    verifyTarget.cols = verifyTarget.cols - 1;
                  }
                }
              }
            });
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

            if (pressPos.ele.classList[1] === "tl") {
              box.y += viewDeltaPoint.y;
              box.height -= viewDeltaPoint.y;
              box.x += viewDeltaPoint.x;
              box.width -= viewDeltaPoint.x;
            } else if (pressPos.ele.classList[1] === "tr") {
              box.y += viewDeltaPoint.y;
              box.height -= viewDeltaPoint.y;
              box.width += viewDeltaPoint.x;
            } else if (pressPos.ele.classList[1] === "bl") {
              box.height += viewDeltaPoint.y;
              box.x += viewDeltaPoint.x;
              box.width -= viewDeltaPoint.x;
            } else if (pressPos.ele.classList[1] === "br") {
              box.height += viewDeltaPoint.y;
              box.width += viewDeltaPoint.x;
            } else if (
              pressPos.ele.classList[0] === "line" ||
              pressPos.ele.classList[0] === "target" ||
              pressPos.ele.classList[0] === "verTarget"
            ) {
              box.x += viewDeltaPoint.x;
              box.y += viewDeltaPoint.y;
            }
            overlay.update(box);
            overlay.drawHTML(viewer.overlaysContainer, viewer.viewport);

            verifyPos = {
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

  let root = document.documentElement;

  // ----------------------------------------
  $: if (colorTarget && !colorOverlay) {
    console.log("Color Target");
    setTimeout(() => {
      addOverlay(0);
      setTimeout(() => {
        colorTarget.color = colorTarget.color + 1;
      }, 0);
    }, 0);
  }

  $: if (!colorTarget && colorOverlay) {
    console.log("Color Target REMOVE");

    removeOverlay(0);
  }

  $: if (colorTarget?.color) {
    root.style.setProperty("--color_hue", `${colorTarget.color}`);
  }

  $: if (colorTarget?.rows) {
    const gridBox = document.getElementById("gBox-0");
    if (gridBox) {
      gridBox.style.gridTemplateRows = `repeat(${colorTarget.rows}, auto)`;
    }
  }
  $: if (colorTarget?.cols) {
    const gridBox = document.getElementById("gBox-0");
    if (gridBox) {
      gridBox.style.gridTemplateColumns = `repeat(${colorTarget.cols}, auto)`;
    }
  }

  $: if (colorTarget?.size) {
    root.style.setProperty("--color_size", `${colorTarget.size * 100}%`);
  }

  // --------------------------------------
  $: if (verifyTarget && !verifyOverlay) {
    console.log("Verify Target");
    setTimeout(() => {
      addOverlay(1);
      setTimeout(() => {
        verifyTarget.color = verifyTarget.color + 1;
      }, 0);
    }, 0);
  }

  $: if (!verifyTarget && verifyOverlay) {
    console.log("Verify Target REMOVE");

    removeOverlay(1);
  }

  $: if (verifyTarget?.color) {
    console.log("verify hue");
    root.style.setProperty("--verfiy_hue", `${verifyTarget.color}`);
  }

  $: if (verifyTarget?.size) {
    console.log("verify size");

    root.style.setProperty("--verify_size", `${verifyTarget.size * 100}%`);
  }

  $: if (verifyTarget?.rows) {
    const gridBox = document.getElementById("gBox-1");
    if (gridBox) {
      gridBox.style.gridTemplateRows = `repeat(${verifyTarget.rows}, auto)`;
    }
  }

  $: if (verifyTarget?.cols) {
    const gridBox = document.getElementById("gBox-1");
    if (gridBox) {
      gridBox.style.gridTemplateColumns = `repeat(${verifyTarget.cols}, auto)`;
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
              <!-- <span class="targetNum"><svg><text>{boxIndex}</text></svg></span> -->
            </div>
          {/each}
        </div>

        <div class="exp top group">
          <button class="dec">
            <MinusIcon size="1.5x" />
          </button>
          <button class="inc">
            <PlusIcon size="1.5x" />
          </button>
        </div>
        <!-- <div class="exp left group">
          <button class="dec">
            <MinusIcon size="1.5x" />
          </button>
          <button class="inc">
            <PlusIcon size="1.5x" />
          </button>
        </div> -->
        <div class="exp right group">
          <button class="dec">
            <MinusIcon size="1.5x" />
          </button>
          <button class="inc">
            <PlusIcon size="1.5x" />
          </button>
        </div>
        <!-- <div class="exp bottom group">
          <button class="dec">
            <MinusIcon size="1.5x" />
          </button>
          <button class="inc">
            <PlusIcon size="1.5x" />
          </button>
        </div> -->
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
              <div class="verTarget" />
              <!-- <span class="targetNum"><svg><text>{boxIndex}</text></svg></span> -->
            </div>
          {/each}
        </div>

        <div class="exp top group">
          <button class="dec">
            <MinusIcon size="1.5x" />
          </button>
          <button class="inc">
            <PlusIcon size="1.5x" />
          </button>
        </div>
        <!-- <div class="exp left group">
          <button class="dec">
            <MinusIcon size="1.5x" />
          </button>
          <button class="inc">
            <PlusIcon size="1.5x" />
          </button>
        </div> -->
        <div class="exp right group">
          <button class="dec">
            <MinusIcon size="1.5x" />
          </button>
          <button class="inc">
            <PlusIcon size="1.5x" />
          </button>
        </div>
        <!-- <div class="exp bottom group">
          <button class="dec">
            <MinusIcon size="1.5x" />
          </button>
          <button class="inc">
            <PlusIcon size="1.5x" />
          </button>
        </div> -->
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
      top: {colorPos?.top?.toFixed(4)} | left: {colorPos?.left?.toFixed(4)} | bottom:
      {colorPos?.bottom?.toFixed(4)} | right: {colorPos?.right?.toFixed(4)}
    </p>
  </div>
  <div id="rowCol">
    Verification Target:
    <p>
      top: {verifyPos?.top?.toFixed(4)} | left: {verifyPos?.left?.toFixed(4)} | bottom:
      {verifyPos?.bottom?.toFixed(4)} | right: {verifyPos?.right?.toFixed(4)}
    </p>
  </div>
</main>

<style lang="postcss">
  :root {
    --color_hue: 50;
    --verfiy_hue: 100;
    --verify_size: 50%;
    --color_size: 50%;
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
      hsla(var(--color_hue), 100%, 50%, 0.3) 80%
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
      hsla(var(--verfiy_hue), 100%, 50%, 0.3) 80%
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
          duration-500 transition-all delay-150 ease-in flex justify-center rounded-lg
          gap-1;
  }
  .inc {
    @apply bg-transparent text-transparent group-hover:text-white group-hover:bg-green-500/90
            align-middle justify-center flex items-center transition-all delay-200;
  }
  /* w-[40%] h-[80%] */
  .dec {
    @apply bg-transparent text-transparent group-hover:text-white group-hover:bg-red-500/90
            align-middle justify-center flex items-center transition-all delay-200;
  }
  .top {
    grid-area: top;
    @apply w-[70%] ml-[15%] flex overflow-hidden;
  }
  .top .inc {
    @apply w-1/4;
  }
  .top .dec {
    @apply w-1/4;
  }
  .left {
    grid-area: left;
    @apply flex flex-col-reverse overflow-hidden;
  }
  .left .inc {
    @apply h-1/4;
  }
  .left .dec {
    @apply h-1/4;
  }
  .right {
    grid-area: right;
    @apply flex flex-col-reverse overflow-hidden;
  }
  .right .inc {
    @apply h-1/4;
  }
  .right .dec {
    @apply h-1/4;
  }
  .bottom {
    grid-area: bottom;
    @apply w-[70%] ml-[15%] flex overflow-hidden;
  }
  .bottom .inc {
    @apply w-1/4;
  }
  .bottom .dec {
    @apply w-1/4;
  }
  .target {
    border-color: hsla(var(--color_hue), 100%, 50%, 0.5);
    width: var(--color_size);
    height: var(--color_size);
    @apply border-[3px] rounded-none;
  }

  .verTarget {
    border-color: hsla(var(--verfiy_hue), 100%, 50%, 0.5);
    width: var(--verify_size);
    height: var(--verify_size);
    @apply border-[3px] rounded-none;
  }

  .corner {
    background-color: hsla(var(--color_hue), 100%, 50%, 0.75);
    @apply absolute h-[5%] aspect-square z-50;
  }

  .ver .corner {
    background-color: hsla(var(--verfiy_hue), 100%, 50%, 0.75);
  }

  .tl {
    @apply -top-[2.5%] -left-[2%] cursor-nw-resize;
  }
  .tr {
    @apply -top-[2.5%] -right-[2%] cursor-ne-resize;
  }
  .bl {
    @apply -bottom-[2.5%] -left-[2%] cursor-ne-resize;
  }
  .br {
    @apply -bottom-[2.5%] -right-[2%] cursor-nw-resize;
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
