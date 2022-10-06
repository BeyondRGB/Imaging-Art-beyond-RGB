

<script>
  import HeapBox from "@components/Process/Dropbox.svelte";
  let items = [];

</script>
<hr>
<HeapBox items={items}/>

<!--
<script lang="ts">
  import HeapBox from "@components/Process/Dropbox.svelte";
  import { processState, sendMessage } from "@util/stores";
  import { dndzone } from "svelte-dnd-action";
  import { flip } from "svelte/animate";
  import ImageBubble from "./ImageBubble.svelte";

  const flipDurationMs = 150;
  function handleDndConsiderCol(e: CustomEvent) {
    $processState.artStacks = e.detail.items;
  }
  function handleDndFinalizeCol(e: CustomEvent) {
    $processState.artStacks = e.detail.items;
  }

  function handleDndConsider(cid: number, field: string, e: CustomEvent) {
    const colIdx = $processState.artStacks.findIndex((c) => c.id === cid);
    $processState.artStacks[colIdx].fields[field] = e.detail.items;
    $processState.artStacks = [...$processState.artStacks];
  }
  function handleDndFinalize(cid: number, field: string, e: CustomEvent) {
    const colIdx = $processState.artStacks.findIndex((c) => c.id === cid);
    $processState.artStacks[colIdx].fields[field] = e.detail.items;
    $processState.artStacks = [...$processState.artStacks];
  }

  function handleCloseCol(cid: number) {
    const colIdx = $processState.artStacks.findIndex((c) => c.id === cid);
    $processState.artStacks.splice(colIdx, 1);

    $processState.artStacks = [...$processState.artStacks];
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
        dragDisabled: true,
      }}
      on:consider={(e) => handleDndConsiderCol(e)}
      on:finalize={(e) => handleDndFinalizeCol(e)}
    >
      {#each $processState.artStacks as column (column.id)}
        <div class="column" animate:flip={{ duration: flipDurationMs * 2 }}>
          <h1>Image Stack</h1>
          <button
            id="removeBtn"
            on:click={() => handleCloseCol(column.id)}
            class="dark:hover:bg-red-400/50">X</button
          >
          <div class="colBody">
            {#each Object.entries(column.fields) as field, i (field[0])}

              <div
                class="row"
                class:hide={field[0].includes("target") && Object.keys($processState.imageFilePaths).length <= 6}
              >
                {#if i % 2 === 0}
                  {#if i === 0}
                    <span class="fieldTitle">object</span>
                  {:else}
                    <span class="fieldTitle">{field[0].slice(0, -1)}</span>
                  {/if}
                {/if}
                <div class="rowBody">
                  {#if i < 2}
                    <span>{field[0][field[0].length - 1]}</span>
                  {:else}
                    <br />
                  {/if}

                  <div
                    class="box strict"
                    use:dndzone={{
                      items: field[1],
                      flipDurationMs,
                      dropFromOthersDisabled:
                        $processState.artStacks[
                          $processState.artStacks.findIndex(
                            (c) => c.id === column.id
                          )
                        ].fields[field[0]]?.length > 0,
                    }}
                    on:consider={(e) =>
                      handleDndConsider(column.id, field[0], e)}
                    on:finalize={(e) =>
                      handleDndFinalize(column.id, field[0], e)}
                  >
                    {#each field[1] as item (item.id)}
                      <card
                        animate:flip={{ duration: flipDurationMs }}
                        class="selected"
                      >
                        <ImageBubble filename={item.name} minimal />
                      </card>
                    {/each}
                  </div>
                </div>
              </div>
            {/each}
          </div>
        </div>
      {/each}
    </section>
  </div>
</main>
-->
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
  .colBody {
    display: grid;
    grid-template-rows: repeat(2, auto);
    grid-template-columns: repeat(2, auto);
    @apply bg-gray-700/25 text-base rounded-xl p-0.5 relative;
  }
  .fieldTitle {
    @apply absolute top-0 left-1/2 w-full flex justify-center items-center;
  }
  .row {
    @apply flex w-full scale-100 transition-all relative;
  }
  .hide {
    @apply scale-0 h-0;
  }
  .rowBody {
    @apply flex flex-col w-full p-0.5 px-1;
  }

  section {
    @apply flex relative;
  }
  .column {
    @apply w-full rounded-lg p-2 gap-1 relative shadow-md bg-gray-500/75 flex flex-col;
  }

  span {
    @apply self-center;
  }

  .box {
    @apply flex h-12 rounded-lg w-full p-1 min-w-[8rem] ring-2 ring-gray-600/50
            justify-center items-center;
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

