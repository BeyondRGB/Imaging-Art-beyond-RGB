<script lang="ts">
  import { currentPage, viewState } from "@util/stores";
  import OpenSeadragon from "openseadragon";
  import { onDestroy, onMount } from "svelte";
  import {getZoomPercentage} from "@util/photoViewerHelper";

  export let size = 0.01;
  export let trueSize;
  export let showBrush;

  export let shadowPos;
  export let trueShadowPos;

  export let loading;

  let viewer;
  let imageUrl = "";

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
    if (viewer && !viewer.isOpen() && imageUrl && imageUrl.length > 0) {
      console.log("Opening Image on page load");
      if (!imageUrl.includes("undefined")) {
        loading = false;
      }
      setTimeout(() => {
        if (viewer && imageUrl) {
          try {
            viewer.open({
              type: "image",
              url: imageUrl,
            });
          } catch (e) {
            console.log("Error opening image on page load:", e);
          }
        }
      }, 100);
    } else if (viewer?.world && viewer.world.getItemCount() > 0) {
      try {
        trueSize = viewer.world.getItemAt(0).getContentSize().x * size;
      } catch (e) {
        console.log("Error getting content size:", e);
      }
    }
  } else {
    if (viewer && viewer.isOpen()) {
      console.log("Close Image - leaving page");
      try {
        // Remove overlays before closing
        removeOverlay();
        viewer.close();
      } catch (e) {
        console.log("Error closing viewer when leaving page:", e);
      }
    }
  }

  $: if (viewer && $viewState.colorManagedImage.dataURL && $currentPage === "SpecPicker") {
    // console.log($processState.artStacks[0].colorTargetImage);
    console.log("New Image (Spec Viewer)");
    let temp = new Image();
    temp.src = $viewState.colorManagedImage.dataURL;

    imageUrl = temp.src;

    // Close existing image before opening new one
    if (viewer.isOpen()) {
      try {
        viewer.close();
      } catch (e) {
        console.log("Error closing viewer:", e);
      }
    }

    setTimeout(() => {
      if (viewer && imageUrl) {
        try {
          viewer.open({
            type: "image",
            url: imageUrl,
          });
        } catch (e) {
          console.log("Error opening image:", e);
        }
      }
    }, 100);

    if (show) {
      console.log("Brush Enabled 3");
      setTimeout(() => {
        addOverlay();
      }, 200);
    }
  }

  function removeOverlay() {
    console.log("Remove Brush");
    try {
      const brush = document.getElementById("specView-brush");
      const shadow = document.getElementById("specView-brush-shadow");

      if (brush && viewer) {
        viewer.removeOverlay(brush);
      }
      if (shadow && viewer) {
        viewer.removeOverlay(shadow);
      }
      if (overTracker && typeof overTracker.destroy === 'function') {
        overTracker.destroy();
      }
      if (mouseTracker && typeof mouseTracker.destroy === 'function') {
        mouseTracker.destroy();
      }
    } catch (e) {
      console.log("Error removing overlay:", e);
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
    background-color: var(--color-overlay-medium);
    border: 1px solid var(--color-border);
    @apply w-full aspect-[3/2] shadow-lg;
  }
  #specpick-seadragon-viewer {
    @apply h-full w-full;
  }
  .load {
    @apply absolute left-1/2 bottom-1/2;
  }
  #specView-brush {
    background-color: var(--color-overlay-light);
    @apply hidden outline-[1%] outline-dashed outline-red-500;
  }
  #specView-brush-shadow {
    background-color: var(--color-overlay-light);
    @apply hidden outline-[1%] outline-dashed outline-blue-500;
  }
</style>
