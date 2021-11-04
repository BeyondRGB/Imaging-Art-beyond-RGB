<script>
  import { currentPage } from "../stores";
  import { getContext } from "svelte";
  const { close } = getContext("simple-modal");

  function handle(page) {
    currentPage.set(page);
    close();
  }
</script>

<main>
  <button on:click={() => handle("SpecOverlay")}>View</button>
  <button on:click={() => handle("SpecOverlay")}>Open File Location</button>
  <button on:click={() => handle("SpecOverlay")}>Exit</button>
</main>

<style lang="postcss">
  main {
    @apply flex flex-col items-center gap-1;
  }
  button {
    @apply bg-gray-100 rounded-md p-2 hover:bg-gray-500 hover:text-white hover:rounded-2xl
            transition-all w-[80%] text-xl;
  }
</style>
