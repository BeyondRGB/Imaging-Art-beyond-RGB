<script>
  import { onMount } from "svelte";
  import panzoom from "panzoom";
  let image;
  let isDown;
  let canvas;
  let context;
  onMount(() => {
    image = new Image();
    image.src = "placeholder.jpg";
    image.onload = function () {
      context = canvas.getContext("2d");
      draw();
    };
  });

  function draw() {
    context.canvas.width = image.width;
    context.canvas.height = image.height;

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

    context.lineWidth = 26;
    context.strokeStyle = "red";
    context.rect(600, 600, m.x * 2, m.y * 2);
    context.stroke();

    // panzoom(canvas, {
    //   bounds: true,
    //   boundsPadding: 1,
    //   minZoom: 1,
    // });
    requestAnimationFrame(draw);
  }

  let m = { x: 0, y: 0 };
  function handleMousemove(event) {
    if (isDown) {
      m.x = event.clientX;
      m.y = event.clientY;
    }
  }
</script>

<main class="dark:ring-gray-600">
  {m.x} x {m.y}
  <div id="image-canvas">
    <canvas
      bind:this={canvas}
      on:mousemove={handleMousemove}
      on:mousedown={(e) => (isDown = true)}
      on:mouseup={(e) => (isDown = false)}
    />
  </div>
</main>

<style lang="postcss">
  #image-canvas {
    @apply table;
  }
  main {
    display: table;
    overflow: hidden;
    @apply w-full ring-1 ring-gray-800;
  }
  canvas {
    @apply block w-full;
  }
</style>
