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

<main class="dark:ring-gray-600">
  <div
    id="draggable"
    use:draggable={{ bounds: "parent" }}
    on:svelte-drag={handleMove}
  >
    <div id="handle" />
  </div>
  <div id="image-canvas">
    <!-- <img src="placeholder.jpg" alt="img" /> -->
    <!-- <canvas bind:this={canvas} /> -->
    <div id="stack">
      <div
        id="image-div"
        style="background-image: url(placeholderAlt1.jpg); width: 100%"
      >
        <!-- <img src="placeholder.jpg" /> -->
      </div>
      <div
        id="image-div"
        style="background-image: url(placeholder.jpg); width: {Math.round(
          handlePos.offsetX
        ) + 12}px"
      >
        <!-- <img src="placeholder.jpg" alt="img" /> -->
      </div>
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    display: table;
    overflow: hidden;
    padding-bottom: 66.67%;
    @apply w-full relative ring-1 ring-gray-800;
  }

  #handle {
    @apply bg-red-300 h-12 w-6 rounded-full ring-2 ring-black;
  }
  #draggable {
    @apply bg-transparent w-6 absolute z-[1];
  }

  #stack {
    @apply bg-black flex;
  }
  /* img {
    flex: 0;
    @apply max-h-[85vh] w-auto;
  } */
  #image-div {
    position: absolute;
    border: 1px solid #b0b0b0;
    overflow: hidden;
    background-size: cover;
    background-repeat: no-repeat;
    background-clip: content-box;
    @apply h-full;
  }
</style>
