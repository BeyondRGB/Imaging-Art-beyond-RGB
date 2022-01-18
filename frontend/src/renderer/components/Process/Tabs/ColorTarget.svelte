<script>
  let rows = "";
  let cols = "";

  import { currentPage, processState } from "@util/stores";
  import ColorTargetViewer from "@components/Process/ColorTargetViewer.svelte";
  import SelectBtn from "@components/SelectBtn.svelte";
  import Loader from "@root/components/Loader.svelte";
  import { draggable } from "svelte-drag";
</script>

<main>
  <!-- <img src="placeholder.jpg" alt="background image" /> -->
  <div class="left">
    <ColorTargetViewer />
  </div>
  <div class="right">
    <div class="settings">
      <div class="box">
        <div class="color-target dark:bg-gray-600">
          Color Target
          <div class="input-group">
            <lable class="row-lable">Rows:</lable>
            <input
              class="dark:bg-gray-700"
              bind:value={rows}
              placeholder="Placeholder..."
            />
          </div>
          <div class="input-group">
            <lable class="row-lable">Columns:</lable>
            <input
              class="dark:bg-gray-700"
              bind:value={cols}
              placeholder="Placeholder..."
            />
          </div>
        </div>
      </div>
      <button class="add">+</button>
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex w-full h-full justify-between bg-gray-700;
  }

  .left {
    @apply w-full h-full flex items-center m-1;
  }

  .right {
    @apply w-full h-full flex;
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
    @apply mt-auto;
  }

  .side {
    @apply flex flex-col h-[90%];
  }
</style>
