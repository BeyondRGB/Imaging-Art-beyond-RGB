<script>
  import { onMount } from "svelte";
  import panzoom from "panzoom";
  let image;

  let canvas;

  onMount(() => {
    let context = canvas.getContext("2d");
    let image = new Image();
    image.src = "placeholder.jpg";
    image.onload = function () {
      context.canvas.width = image.width;
      context.canvas.height = image.height;

      context.canvas.addEventListener(
        "click",
        () => {
          console.log("click");
        },
        false
      );

      context.drawImage(
        image, // Image
        0, // sx
        0, // sy
        image.width, // sWidth
        image.height, // sHeight
        0, // dx
        0, // dy
        image.width, // dwidth
        image.height // dheight
      );

      panzoom(canvas, {
        bounds: true,
        boundsPadding: 1,
        minZoom: 1,
      });
    };
  });
</script>

<main>
  <div id="image-canvas">
    <canvas bind:this={canvas} />
  </div>
</main>

<style lang="postcss">
  #image-canvas {
    @apply table;
  }
  main {
    display: table;
    overflow: hidden;
    @apply w-full;
  }
  canvas {
    @apply block w-full;
  }
</style>
