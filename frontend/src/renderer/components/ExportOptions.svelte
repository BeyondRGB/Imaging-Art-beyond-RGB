<script>
  import { currentPage, modal } from "@util/stores";
  import { getContext } from "svelte";
  const { close } = getContext("simple-modal");

  function handle(page) {
    modal.set(null);
    close();
    currentPage.set(page);
  }
</script>

<main class="dark:bg-gray-600">
  <button
    class="dark:bg-gray-700 dark:hover:bg-gray-600 dark:hover:text-white dark:text-white dark:hover:ring"
    on:click={() => handle("SpecOverlay")}>View</button
  >
  <button
    class="dark:bg-gray-700 dark:hover:bg-gray-600 dark:hover:text-white dark:text-white dark:hover:ring"
    on:click={() => handle("SpecOverlay")}>Open File Location</button
  >
  <button
    class="dark:bg-gray-700 dark:hover:bg-gray-600 dark:hover:text-white dark:text-white dark:hover:ring"
    on:click={() => handle("SpecOverlay")}>Exit</button
  >
</main>

<style lang="postcss">
  main {
    @apply flex flex-col items-center gap-1;
  }
  button {
    @apply bg-gray-100 rounded-md p-2 hover:bg-gray-500 hover:text-white hover:rounded-2xl
            transition-all w-full text-xl text-gray-800 ring-gray-500 ring-inset;
  }
</style>
