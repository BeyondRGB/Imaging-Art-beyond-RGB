
<script>
  import {dndzone} from 'svelte-dnd-action';
  import {flip} from 'svelte/animate';
  import ImageBubble from "@components/Process/ImageBubble.svelte";
  const flipDurationMs = 200;

  export let items = [];
  export let type;
  export let singleItem = false;

  function handleSort(e) {
    items = e.detail.items;
  }
</script>
<main>
  <section use:dndzone={{items, flipDurationMs, type, dropFromOthersDisabled: singleItem && items.length > 0}}
           on:consider={handleSort}
           on:finalize={handleSort}>
    {#each items as item (item?.id)}
      <card animate:flip={{ duration: flipDurationMs }}>
        <ImageBubble filename={item.name} minimal />
      </card>
    {/each}
  </section>
</main>

<style>
  section {
    min-height: 3em;
    width: 80%;
    margin: auto;
    background-color: #2c2c2f;
    border-radius: 10px;
    justify-content: center;
    display: flex;
    flex-wrap: wrap;
    gap: 10px;
    align-items: center;
  }

  card {
    background-color: gray;
    width: 150px;
    height: 1.8em;
    border-radius: 10px;
    color: white;
    text-align: center;
  }
</style>
