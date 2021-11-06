<script>
  import { dndzone } from "svelte-dnd-action";
  import { flip } from "svelte/animate";
  export let items;

  const flipDurationMs = 100;
  function handleDndConsider(e) {
    // console.log(e);
    items = e.detail.items;
  }
  function handleDndFinalize(e) {
    items = e.detail.items;
  }
</script>

<main>
  <section
    use:dndzone={{ items, flipDurationMs }}
    on:consider={handleDndConsider}
    on:finalize={handleDndFinalize}
  >
    {#each items as item (item.id)}
      <card animate:flip={{ duration: flipDurationMs }}>
        {item.name}
      </card>
    {/each}
  </section>
  [
  {#each items as item (item.id)}
    {item.id},
  {/each}
  ]
</main>

<style lang="postcss">
  main {
    @apply w-[25%];
  }
  section {
    @apply h-full bg-red-200 rounded-lg w-full;
  }
  card {
    width: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
    background-color: #dddddd;
    border: 1px solid #333333;
    @apply mb-1;
  }
</style>
