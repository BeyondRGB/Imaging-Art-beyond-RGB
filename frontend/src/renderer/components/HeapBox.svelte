<script>
  import DragBox from "@components/DragBox.svelte";

  import { dndzone } from "svelte-dnd-action";
  import { flip } from "svelte/animate";
  export let items;

  const flipDurationMs = 200;
  function handleDndConsider(e) {
    items = e.detail.items;
  }
  function handleDndFinalize(e) {
    items = e.detail.items;
  }
</script>

<main class="dark:bg-blue-700/25">
  <section
    use:dndzone={{ items, flipDurationMs }}
    on:consider={handleDndConsider}
    on:finalize={handleDndFinalize}
  >
    {#each items as item (item.id)}
      <card
        animate:flip={{ duration: flipDurationMs }}
        class="dark:bg-gray-600"
      >
        {item.name}
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
