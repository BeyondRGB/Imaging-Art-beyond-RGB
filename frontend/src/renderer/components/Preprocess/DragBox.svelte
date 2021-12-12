<script>
  import { dndzone } from "svelte-dnd-action";
  import { flip } from "svelte/animate";
  export let items;
  export let strict;
  // export let colId;

  const flipDurationMs = 150;
  function handleDndConsider(e) {
    items = e.detail.items;
  }
  function handleDndFinalize(e) {
    items = e.detail.items;
  }
</script>

<main>
  <section
    class={strict ? "strict" : ""}
    use:dndzone={{ items, flipDurationMs }}
    on:consider={handleDndConsider}
    on:finalize={handleDndFinalize}
  >
    {#each items as item (item.id)}
      <card
        animate:flip={{ duration: flipDurationMs }}
        class={items.length > 1 ? "selected" : ""}
      >
        <span>
          {item.name}
        </span>
      </card>
    {/each}
  </section>
</main>

<style lang="postcss">
  main {
    @apply w-full flex;
  }
  section {
    @apply flex h-12 rounded-lg w-full p-1 min-h-[2.2rem];
  }

  card {
    border: 1px solid #333333;
    @apply rounded-xl bg-gray-50 text-base mb-1 flex justify-center py-1 px-1 w-full;
  }

  .strict card:first-child {
    @apply bg-green-200 text-black;
  }

  .strict .selected:nth-child(n + 2) {
    @apply bg-red-500 text-white;
  }
</style>
