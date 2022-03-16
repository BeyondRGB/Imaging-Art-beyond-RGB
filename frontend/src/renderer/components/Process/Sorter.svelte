<script lang="ts">
  import HeapBox from "@components/Process/HeapBox.svelte";
  import { processState, sendMessage } from "@util/stores";
  import { dndzone } from "svelte-dnd-action";
  import { flip } from "svelte/animate";

  const flipDurationMs = 150;
  function handleDndConsiderCol(e: CustomEvent) {
    $processState.artStacks = e.detail.items;
  }
  function handleDndFinalizeCol(e: CustomEvent) {
    $processState.artStacks = e.detail.items;
  }

  function handleDndConsider(cid: number, field: string, e: CustomEvent) {
    const colIdx = $processState.artStacks.findIndex((c) => c.id === cid);
    //const fieldIdx = columns[colIdx].fields.findIndex((f) => f.name === field);
    $processState.artStacks[colIdx].fields[field] = e.detail.items;
    $processState.artStacks = [...$processState.artStacks];
  }
  function handleDndFinalize(cid: number, field: string, e: CustomEvent) {
    const colIdx = $processState.artStacks.findIndex((c) => c.id === cid);
    //const fieldIdx = columns[colIdx].fields.findIndex((f) => f.name === field);
    $processState.artStacks[colIdx].fields[field] = e.detail.items;
    $processState.artStacks = [...$processState.artStacks];
  }

  function handleCloseCol(cid: number) {
    const colIdx = $processState.artStacks.findIndex((c) => c.id === cid);
    $processState.artStacks.splice(colIdx, 1);

    $processState.artStacks = [...$processState.artStacks];
  }

  function handleAddCol() {
    console.log("ADD COL");
    // $processState.artStacks = [
    //   ...$processState.artStacks,
    //   {
    //     id: Date.now(),
    //     name: `Art ${$processState.artStacks.length + 1}`,
    //     fields: {
    //       images: [],
    //       whitefield: [],
    //       darkfield: [],
    //     },
    //   },
    // ];
  }
</script>

<main>
  <div id="heap">
    <HeapBox items={$processState.imageFilePaths} />
  </div>
  <div id="cols">
    <section
      use:dndzone={{
        items: $processState.artStacks,
        flipDurationMs,
        type: "col",
      }}
      on:consider={(e) => handleDndConsiderCol(e)}
      on:finalize={(e) => handleDndFinalizeCol(e)}
    >
      {#each $processState.artStacks as column (column.id)}
        <div
          class="column dark:bg-blue-700/25 bg-blue-200"
          animate:flip={{ duration: flipDurationMs * 2 }}
        >
          <h1>{column.name}</h1>
          <button
            id="removeBtn"
            on:click={() => handleCloseCol(column.id)}
            class="dark:hover:bg-red-400/50">X</button
          >
          <div class="flex">
            <item>
              {#each Object.entries(column.fields) as field (field[0])}
                <span>{field[0]}</span>
                <itemBox
                  class={"strict"}
                  use:dndzone={{ items: field[1], flipDurationMs }}
                  on:consider={(e) => handleDndConsider(column.id, field[0], e)}
                  on:finalize={(e) => handleDndFinalize(column.id, field[0], e)}
                >
                  {#each field[1] as item (item.id)}
                    <card
                      animate:flip={{ duration: flipDurationMs }}
                      class={999 > 1 ? "selected" : ""}
                      style=""
                    >
                      {item?.name?.split("\\").length > 2
                        ? item?.name?.split("\\").at(-1)
                        : item?.name?.split("/").at(-1)}
                    </card>
                  {/each}
                </itemBox>
              {/each}
            </item>
          </div>
        </div>
      {/each}
    </section>
    <button
      id="addBtn"
      on:click={handleAddCol}
      class="dark:hover:bg-green-400/50">+</button
    >
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex flex-col justify-center items-center;
  }
  #cols {
    @apply flex gap-2 relative items-center justify-center align-middle mt-12;
  }
  #heap {
    @apply relative;
  }

  section {
    @apply flex relative;
  }
  .column {
    @apply w-full rounded-lg m-1 p-1 relative shadow-md;
  }
  item {
    @apply text-base justify-center flex-col;
  }

  span {
    @apply self-center;
  }

  itemBox {
    @apply flex h-12 rounded-lg w-full p-1 min-h-[2.2rem];
  }

  card {
    border: 1px solid #333333;
    @apply rounded-xl bg-gray-50 text-base mb-1 flex justify-center py-1 px-1 w-full shadow;
  }

  .strict card:first-child {
    @apply bg-green-400 text-black dark:bg-green-800 dark:text-white;
  }

  .strict .selected:nth-child(n + 2) {
    @apply bg-red-400 text-black dark:bg-red-800 dark:text-white;
  }

  #removeBtn {
    @apply px-2 hover:text-red-400 hover:bg-red-400/25 text-base py-0 
          bg-transparent right-0 top-0 transition-all text-gray-400 absolute shadow-none
          ring-0;
  }

  #addBtn {
    margin: 0 auto;
    display: table-cell;
    vertical-align: middle;
    @apply hover:text-green-500 z-10 hover:bg-green-300/25 h-10 w-10
              rounded-full duration-500 text-gray-300 relative ring-1 ring-green-400/50 bg-transparent
              text-center text-3xl p-0 pb-1;
  }
</style>
