<script>
  import { processState } from "@util/stores";

  import { dndzone } from "svelte-dnd-action";
  import { flip } from "svelte/animate";
  const flipDurationMs = 200;
  function handleDndConsider(e) {
    $processState.imageFilePaths = e.detail.items;
  }
  function handleDndFinalize(e) {
    $processState.imageFilePaths = e.detail.items;
  }
</script>

<main class="dark:bg-blue-700/25">
  <section
    use:dndzone={{ items: $processState.imageFilePaths, flipDurationMs }}
    on:consider={handleDndConsider}
    on:finalize={handleDndFinalize}
  >
    {#each $processState.imageFilePaths as item (item?.id)}
      <card
        animate:flip={{ duration: flipDurationMs }}
        class="dark:bg-gray-600"
      >
        {item?.name?.split("\\")?.at(-1)}
      </card>
    {/each}
  </section>
</main>

<style lang="postcss">
  main {
    @apply bg-blue-200 w-full;
  }

  section {
    @apply h-full rounded-lg p-1;
  }

  card {
    border: 1px solid #333333;
    @apply rounded-xl bg-gray-50 text-base mb-1 inline-grid justify-center py-1 px-2 shadow;
  }
</style>
