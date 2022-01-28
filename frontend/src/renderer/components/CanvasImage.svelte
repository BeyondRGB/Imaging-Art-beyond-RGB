<script>
  import placeholder from "@assets/placeholder.jpg";
  import { onMount } from "svelte";
  let image;
  let panZoom = false;

  let canvas;

  onMount(() => {
    let context = canvas.getContext("2d");
    let image = new Image();
    image.src = placeholder;
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
    };
  });
</script>

<main class="dark:ring-gray-600">
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
    @apply block w-full ring-1 ring-gray-800;
  }
</style>
