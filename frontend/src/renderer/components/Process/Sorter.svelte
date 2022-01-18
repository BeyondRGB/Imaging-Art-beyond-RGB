<script lang="ts">
  import HeapBox from "@components/Process/HeapBox.svelte";
  import { dndzone } from "svelte-dnd-action";
  import { flip } from "svelte/animate";
  let items: any[] = [
    { id: 1, name: "photo1.cr2" },
    { id: 2, name: "photo2.cr2" },
    { id: 3, name: "photo3.cr2" },
    { id: 4, name: "photo4.cr2" },
    { id: 5, name: "photo5.cr2" },
    { id: 6, name: "photo6.cr2" },
    { id: 7, name: "photo7.cr2" },
    { id: 8, name: "photo8.cr2" },
    { id: 9, name: "photo9.cr2" },
    { id: 10, name: "photo10.cr2" },
    { id: 11, name: "photo11.cr2" },
    { id: 12, name: "photo12.cr2" },
    { id: 13, name: "photo13.cr2" },
    { id: 14, name: "photo14.cr2" },
    { id: 15, name: "photo15.cr2" },
    { id: 16, name: "photo16.cr2" },
  ];

  let columns: any[] = [
    {
      id: 20,
      name: "Art 1",
      fields: [
        { name: "ImageA", items: [] },
        { name: "WhitefieldA", items: [] },
        { name: "DarkfieldA", items: [] },
      ],
    },
  ];

  const flipDurationMs = 150;
  function handleDndConsiderCol(e: CustomEvent) {
    columns = e.detail.items;
  }
  function handleDndFinalizeCol(e: CustomEvent) {
    columns = e.detail.items;
  }

  function handleDndConsider(cid: number, field: string, e: CustomEvent) {
    const colIdx = columns.findIndex((c) => c.id === cid);
    const fieldIdx = columns[colIdx].fields.findIndex((f) => f.name === field);
    columns[colIdx].fields[fieldIdx].items = e.detail.items;
    columns = [...columns];
  }
  function handleDndFinalize(cid: number, field: string, e: CustomEvent) {
    const colIdx = columns.findIndex((c) => c.id === cid);
    const fieldIdx = columns[colIdx].fields.findIndex((f) => f.name === field);
    columns[colIdx].fields[fieldIdx].items = e.detail.items;
    columns = [...columns];
  }

  function handleCloseCol(cid: number) {
    const colIdx = columns.findIndex((c) => c.id === cid);
    columns.splice(colIdx, 1);
    console.log(columns);
    columns = [...columns];
  }

  function handleAddCol() {
    columns = [
      ...columns,
      {
        id: Date.now(),
        name: `Art ${columns.length + 1}`,
        fields: [
          { name: "ImageA", items: [] },
          { name: "WhitefieldA", items: [] },
          { name: "DarkfieldA", items: [] },
        ],
      },
    ];
  }
</script>

<main>
  <div id="heap">
    <HeapBox {items} />
  </div>
  <div id="cols">
    <section
      use:dndzone={{ items: columns, flipDurationMs, type: "col" }}
      on:consider={(e) => handleDndConsiderCol(e)}
      on:finalize={(e) => handleDndFinalizeCol(e)}
    >
      {#each columns as column (column.id)}
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
              {#each column.fields as field (field.name)}
                <span>{field.name}</span>
                <itemBox
                  class={"strict"}
                  use:dndzone={{ items: field.items, flipDurationMs }}
                  on:consider={(e) =>
                    handleDndConsider(column.id, field.name, e)}
                  on:finalize={(e) =>
                    handleDndFinalize(column.id, field.name, e)}
                >
                  {#each field.items as item (item.name)}
                    <card
                      animate:flip={{ duration: flipDurationMs }}
                      class={items.length > 1 ? "selected" : ""}
                      style=""
                    >
                      {item.name}
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
