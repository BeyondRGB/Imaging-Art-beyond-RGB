<script lang="ts">
  import { currentPage, viewState } from "@util/stores";
  import OpenSeadragon from "openseadragon";
  import { onDestroy, onMount } from "svelte";
  import {getZoomPercentage} from "@util/photoViewerHelper";

  export let size = 0.01;
  export let trueSize;
  export let show = true;

  export let shadowPos;
  export let trueShadowPos;

  export let loading;

  let viewer;
  let imageUrl;

  let pressPos = { top: 0, bottom: 0, left: 0, right: 0 };
  let linearZoom = 0;
  let viewportPoint;
  let imagePoint;
  let mouseTracker;
  let overTracker;

  const createViewer = () => {
    viewer = OpenSeadragon({
      id: "specpick-seadragon-viewer",
      prefixUrl: "/openseadragon/images/",
      immediateRender: true,
      preload: true,
      showNavigator: false,
      minZoomLevel: 0.9,
      useCanvas: true,
      showZoomControl: false,
      showHomeControl: false,
      showFullPageControl: false,
      preserveImageSizeOnResize: true,
      maxZoomPixelRatio: 30,
      zoomPerScroll: 1.15,
      visibilityRatio: 0.7,
      animationTime: 0.4,
    });

    viewer.addHandler("zoom", handleZoom);
  };

  onDestroy(() => {
    if (viewer) {
      viewer.destroy();
      viewer = null;
      console.log("SpecPicker viewer destroyed");
    }
  });

  const destoryViewer = () => {
    if (viewer) {
      viewer.destroy();
      viewer = null;
      console.log("SpecPicker viewer destroyed");
    }
  };

  onMount(() => {
    console.log("SpecPicker viewer Mount");
    createViewer();
  });
  onDestroy(() => {
    console.log("SpecPicker viewer Destroy");
    destoryViewer();
  });

  $: if ($currentPage === "SpecPicker") {
    if (viewer && !viewer.isOpen()) {
      console.log("Opening Image");
      if (!imageUrl.includes("undefined")) {
        loading = false;
      }
      setTimeout(() => {
        viewer.open({
          type: "image",
          url: imageUrl,
        });
      }, 50);
      if (show) {
        console.log("Brush Enabled 1");
        setTimeout(() => {
          addOverlay();
        }, 150);
      }
    } else if (viewer?.world) {
      trueSize = viewer.world.getItemAt(0).getContentSize().x * size;
    }
  } else {
    if (viewer) {
      console.log("Close Image");
      viewer.close();
    }
  }

  $: if (viewer) {
    // console.log($processState.artStacks[0].colorTargetImage);
    console.log("New Image (Spec Viewer)");
    let temp = new Image();
    temp.src = $viewState.colorManagedImage.dataURL;

    imageUrl = temp.src;

    setTimeout(() => {
      viewer.open({
        type: "image",
        url: imageUrl,
      });
    }, 50);

    if (show) {
      console.log("Brush Enabled 3");
      setTimeout(() => {
        addOverlay();
      }, 150);
    } else {
      removeOverlay();
    }
  }

  // $: if (show) {
  //   console.log("Brush Enabled 3");
  //   setTimeout(() => {
  //     addOverlay();
  //   }, 0);
  // } else {
  //   removeOverlay();
  // }

  function removeOverlay() {
    console.log("Remove Brush");
    try {
      const brush = document.getElementById("specView-brush");
      const shadow = document.getElementById("specView-brush-shadow");

      viewer.removeOverlay(brush);
      viewer.removeOverlay(shadow);
      overTracker.destroy();
      mouseTracker.destroy();
    } catch (e) {
      console.log(e);
    }
  }

  function addOverlay() {
    const brush = document.getElementById("specView-brush");
    const shadow = document.getElementById("specView-brush-shadow");
    viewer.addOverlay({
      element: brush,
      location: new OpenSeadragon.Rect(0.5, 0.5, size, size),
    });
    viewer.addOverlay({
      element: shadow,
      location: new OpenSeadragon.Rect(0.5, 0.5, size, size),
    });

    overTracker = new OpenSeadragon.MouseTracker({
      element: "specView-brush",
      clickHandler: function (e) {
        if (viewer !== null) {
          var overlay = viewer.getOverlayById("specView-brush");
          var overlayShadow = viewer.getOverlayById("specView-brush-shadow");

          overlayShadow.update(
            new OpenSeadragon.Rect(
              viewportPoint.x - overlay.width / 2,
              viewportPoint.y - overlay.height / 2,
              overlay.width,
              overlay.height
            )
          );
          overlayShadow.drawHTML(viewer.overlaysContainer, viewer.viewport);
          shadowPos = {
            top: viewportPoint.y,
            left: viewportPoint.x,
            size,
          };
          let pixelCoords =
            viewer.viewport.viewportToImageCoordinates(viewportPoint);
          trueShadowPos = {
            top: pixelCoords.y,
            left: pixelCoords.x,
          };
        } else {
          console.log("Viewer NULL");
        }
      },
    });

    mouseTracker = new OpenSeadragon.MouseTracker({
      element: viewer.canvas,
      moveHandler: function (e) {
        viewportPoint = viewer.viewport.pointFromPixel(e.position);
        imagePoint = viewer.viewport.viewportToImageCoordinates(viewportPoint);
        var overlay = viewer.getOverlayById("specView-brush");
        if (overlay !== null) {
          overlay.update(
            new OpenSeadragon.Rect(
              viewportPoint.x - size / 2,
              viewportPoint.y - size / 2,
              size,
              size
            )
          );
          overlay.drawHTML(viewer.overlaysContainer, viewer.viewport);
        }
      },
    });
  }

  function updateSize() {
    try {
      var overlay = viewer.getOverlayById("specView-brush");
      var overlayShadow = viewer.getOverlayById("specView-brush-shadow");

      if (overlay !== null) {
        overlay.update(
          new OpenSeadragon.Rect(
            viewportPoint.x - size / 2,
            viewportPoint.y - size / 2,
            size,
            size
          )
        );
        overlay.drawHTML(viewer.overlaysContainer, viewer.viewport);
        trueSize = viewer.world.getItemAt(0).getContentSize().x * size;
      }
    } catch (e) {
      console.log(e);
    }
  }

  $: if (size) {
    console.log("Size CHANGE");
    updateSize();
  }

  function handleZoom(e) {
    if (e.zoom > 10) {
      viewer.drawer.setImageSmoothingEnabled(false);
    } else {
      viewer.drawer.setImageSmoothingEnabled(true);
    }

    linearZoom = getZoomPercentage(viewer.viewport.getZoom(true));
  }
</script>

<main>
  <!-- <div class="load"><Loader /></div> -->
  <div id="specpick-seadragon-viewer" />
  {#if linearZoom > 1}
    <h1 id="zoom">{Math.floor(linearZoom)}%</h1>
  {/if}
  <div id="specView-brush" />
  <div id="specView-brush-shadow" />
</main>

<style lang="postcss">
  main {
    @apply w-full ring-1 ring-gray-800 bg-gray-900/50 aspect-[3/2] shadow-lg;
  }
  #specpick-seadragon-viewer {
    @apply h-full w-full;
  }
  .load {
    @apply absolute left-1/2 bottom-1/2;
  }
  #specView-brush {
    @apply bg-gray-800/25 hidden  outline-[1%] outline-dashed outline-red-500;
  }
  #specView-brush-shadow {
    @apply bg-gray-800/25 hidden  outline-[1%] outline-dashed outline-blue-500;
  }
</style>
