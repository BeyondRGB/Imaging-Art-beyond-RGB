<script>
  let rows = "";

  import { currentPage } from "@util/stores";
  import CanvasImage from "@components/CanvasImage.svelte";

  import { draggable } from "svelte-drag";
</script>

<main>
  <!-- <img src="placeholder.jpg" alt="background image" /> -->
  <div id="image">
    <CanvasImage />
  </div>
  <div class="side">
    <div class="settings">
      Settings
      <div class="box">
        <div class="color-target">
          Color Target
          <div class="input-group">
            <lable class="row-lable">Rows:</lable>
            <input bind:value={rows} placeholder="Placeholder..." />
          </div>
          <div class="input-group">
            <lable class="row-lable">Columns:</lable>
            <input bind:value={rows} placeholder="Placeholder..." />
          </div>
        </div>
      </div>
      <button class="add">+</button>
    </div>
    <button class="next" on:click={() => currentPage.set("Process")}
      >Confirm -></button
    >
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex w-full;
  }

  #image {
    @apply w-[75%] self-center mx-2;
  }

  .settings {
    @apply w-full px-4 flex flex-col;
  }

  .box {
    @apply bg-gray-200 rounded-md shadow-md w-full;
  }

  .color-target {
    @apply px-2 py-1 w-full;
  }

  .input-group {
    @apply flex w-full;
  }

  .row-lable {
    @apply my-1 py-1 mx-1;
  }

  input {
    @apply w-full mx-2 my-1 px-2 py-1;
  }

  .add {
    @apply rounded-full text-2xl;
  }

  .next {
    @apply mb-12 mt-auto;
  }

  .side {
    @apply flex flex-col;
  }
</style>
