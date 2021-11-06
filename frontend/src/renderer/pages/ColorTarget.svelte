<script>
  let rows = "";

  import { currentPage } from "../stores";
  import PageTransitions from "@components/PageTransitions.svelte";
  import CanvasImage from "@components/CanvasImage.svelte";
</script>

<PageTransitions>
  <main>
    <!-- <img src="placeholder.jpg" alt="background image" /> -->
    <CanvasImage />
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
</PageTransitions>

<style lang="postcss">
  main {
    @apply flex w-full;
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
    @apply w-10 h-10 my-2 bg-blue-600 rounded-full self-center text-gray-200 text-2xl
          align-middle justify-center pb-1;
  }

  .next {
    @apply mb-12 bg-blue-600 rounded-md self-center text-gray-200 text-lg px-4 py-2 mt-auto;
  }

  .side {
    @apply flex flex-col;
  }
</style>
