<script>
  import { beforeUpdate, onMount } from "svelte";
  import panzoom from "panzoom";
  import { draggable } from "svelte-drag";
  let image;
  let panZoom = false;

  let canvas;
  let context;
  let handlePos;
  $: handlePos = { offsetX: 0, offsetY: 0 };

  onMount(() => {
    // context = canvas.getContext("2d");
    image = new Image();
    image.src = "placeholder.jpg";
    // image.onload = () => {
    //   draw();
    // };
  });

  function draw() {
    context.canvas.width = image.width;
    context.canvas.height = image.height;
    console.log(image.width, image.height);

    context.drawImage(
      image, // Image
      0, // sx
      0, // sy
      image.width - handlePos.offsetX, // sWidth
      image.height, // sHeight
      0, // dx
      0, // dy
      image.width - handlePos.offsetX, // dwidth
      image.height // dheight
    );

    if (panZoom) {
      panzoom(canvas, {
        bounds: true,
        boundsPadding: 1,
        minZoom: 1,
      });
    }
  }

  beforeUpdate(() => {
    console.log("BeforeUpdate", handlePos);
  });
  function handleMove(e) {
    handlePos = e.detail;
  }
</script>

{Math.round(handlePos.offsetX)}
<wrap>
  <main class="dark:ring-gray-600">
    <div
      id="draggable"
      use:draggable={{ bounds: "parent" }}
      on:svelte-drag={handleMove}
    >
      <div id="handle">|</div>
    </div>
    <div id="image-canvas">
      <!-- <img src="placeholder.jpg" alt="img" /> -->
      <!-- <canvas bind:this={canvas} /> -->
      <div id="stack">
        <div
          id="image-div"
          style="background-image: url(placeholder.jpg); width: {Math.round(
            handlePos.offsetX + 24
          )}px"
        >
          <!-- <img src="placeholder.jpg" alt="img" /> -->
        </div>
        <!-- <img src="placeholder.jpg" /> -->
      </div>
    </div>
  </main>
</wrap>

<style lang="postcss">
  #image-canvas {
    @apply table;
  }
  wrap {
    width: 36%;
    margin: 8px auto;
  }
  main {
    /* display: table;
    overflow: hidden; */
    padding-bottom: 75%;
    @apply w-full ring-1 ring-gray-800 relative bg-blue-500;
  }
  canvas {
    @apply block w-full;
  }
  #handle {
    @apply bg-indigo-600 h-12 w-12 rounded-full;
  }
  #draggable {
    @apply bg-black w-12 absolute z-[1];
  }
  #stack {
    @apply bg-black flex;
  }
  img {
    flex: 0;
    @apply max-h-[85vh] w-auto;
  }
  #image-div {
    position: relative;
    overflow: hidden;
    border: 1px solid #b0b0b0;
    background-size: cover;
    background-repeat: no-repeat;
    background-clip: content-box;
    @apply h-[85vh];
  }
</style>
