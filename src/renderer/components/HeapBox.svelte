<script>
  import DragBox from "@components/DragBox.svelte";

  import { dndzone } from "svelte-dnd-action";
  import { flip } from "svelte/animate";
  export let items;

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
    use:dndzone={{ items, flipDurationMs, type: "files" }}
    on:consider={handleDndConsider}
    on:finalize={handleDndFinalize}
  >
    {#each items as item (item.id)}
      <card animate:flip={{ duration: flipDurationMs }}>
        <span>{item.name}</span>
      </card>
    {/each}
  </section>
</main>

<style lang="postcss">
  main {
    @apply bg-blue-200;
  }
  div {
    @apply w-full bg-blue-200 rounded-lg;
  }

  card {
    width: 100%;
    border: 1px solid #333333;
    justify-content: center;
    align-items: center;
    @apply rounded-xl bg-gray-50 text-lg mb-1;
  }

  span {
    @apply py-1 px-2;
  }
</style>
