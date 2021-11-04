<script>
  import { dndzone } from "svelte-dnd-action";
  import { flip } from "svelte/animate";
  export let items;
  export let strict;

  const flipDurationMs = 100;
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
    use:dndzone={{ items, flipDurationMs, type: "files" }}
    on:consider={handleDndConsider}
    on:finalize={handleDndFinalize}
  >
    {#each items as item (item.id)}
      <card
        animate:flip={{ duration: flipDurationMs }}
        class={items.length > 1 ? "selected" : ""}
      >
        {item.name}
      </card>
    {/each}
  </section>
</main>

<style lang="postcss">
  main {
    @apply w-full;
  }
  section {
    @apply h-full rounded-lg w-[5vw] p-1 min-h-[2.2rem];
  }
  card {
    width: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
    border: 1px solid #333333;
    @apply mb-1 rounded-xl bg-gray-50;
  }

  .strict .selected:nth-child(n + 2) {
    @apply bg-red-600 text-white;
  }
</style>
