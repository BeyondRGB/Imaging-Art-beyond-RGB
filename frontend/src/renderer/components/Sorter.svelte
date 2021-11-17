<script>
  import HeapBox from "@components/HeapBox.svelte";
  import SorterCol from "@components/SorterCol.svelte";
  import { dndzone } from "svelte-dnd-action";
  import { flip } from "svelte/animate";
  import { fade } from "svelte/transition";
  let items = [
    { id: 1, name: "item1" },
    { id: 2, name: "item2" },
    { id: 3, name: "item3" },
    { id: 4, name: "item4" },
    { id: 5, name: "item5" },
    { id: 6, name: "item6" },
    { id: 7, name: "item7" },
    { id: 8, name: "item8" },
    { id: 9, name: "item9" },
    { id: 10, name: "item10" },
    { id: 11, name: "item11" },
    { id: 12, name: "item12" },
    { id: 13, name: "item13" },
    { id: 14, name: "item14" },
    { id: 15, name: "item15" },
    { id: 16, name: "item16" },
  ];

  let columns = [
    {
      id: 20,
      name: "Art 1",
      fields: [
        { name: "Image", items: [] },
        { name: "Whitefield", items: [] },
        { name: "Darkfield", items: [] },
      ],
    },
    {
      id: 21,
      name: "Art 2",
      fields: [
        { name: "Image", items: [] },
        { name: "Whitefield", items: [] },
        { name: "Darkfield", items: [] },
      ],
    },
    {
      id: 22,
      name: "Art 3",
      fields: [
        { name: "Image", items: [] },
        { name: "Whitefield", items: [] },
        { name: "Darkfield", items: [] },
      ],
    },
  ];

  const flipDurationMs = 150;
  function handleDndConsiderCol(e) {
    console.log(e.detail);
    columns = e.detail.items;
  }
  function handleDndFinalizeCol(e) {
    columns = e.detail.items;
  }

  function handleDndConsider(cid, field, e) {
    const colIdx = columns.findIndex((c) => c.id === cid);
    const fieldIdx = columns[colIdx].fields.findIndex((f) => f.name === field);
    columns[colIdx].fields[fieldIdx].items = e.detail.items;
    columns = [...columns];
  }
  function handleDndFinalize(cid, field, e) {
    const colIdx = columns.findIndex((c) => c.id === cid);
    const fieldIdx = columns[colIdx].fields.findIndex((f) => f.name === field);
    columns[colIdx].fields[fieldIdx].items = e.detail.items;
    columns = [...columns];
  }

  function handleCloseCol(cid) {
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
          { name: "Image", items: [] },
          { name: "Whitefield", items: [] },
          { name: "Darkfield", items: [] },
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
    <button
      id="addBtn"
      on:click={handleAddCol}
      class="dark:hover:bg-green-400/50">+</button
    >
    <section
      use:dndzone={{ items: columns, flipDurationMs, type: "col" }}
      on:consider={handleDndConsiderCol}
      on:finalize={handleDndFinalizeCol}
    >
      {#each columns as column (column.id)}
        <div
          class="column dark:bg-blue-700/25 bg-blue-200"
          animate:flip={{ duration: flipDurationMs * 2 }}
        >
          <h1>{column.name}</h1>
          <button
            id="removeBtn"
            on:click={handleCloseCol(column.id)}
            class="dark:hover:bg-red-400/50">X</button
          >
          <div class="flex">
            {#each ["A", "B"] as letter}
              <item>
                {#each column.fields as field (field.name)}
                  <span>{field.name} {letter}</span>
                  <itemBox
                    class={"strict"}
                    use:dndzone={{ items: field.items, flipDurationMs }}
                    on:consider={(e) =>
                      handleDndConsider(column.id, field.name, e)}
                    on:finalize={(e) =>
                      handleDndFinalize(column.id, field.name, e)}
                  >
                    {#each field.items as item (item.id)}
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
            {/each}
          </div>
        </div>
      {/each}
    </section>
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex flex-col justify-center items-center;
  }
  #cols {
    @apply flex gap-2 relative pr-9;
  }
  #heap {
    @apply mb-2 shadow-md;
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

  button {
    @apply py-0 bg-transparent right-0 top-0 transition-all text-gray-400 absolute shadow-none;
  }

  #removeBtn {
    @apply px-1 hover:text-red-400 hover:bg-red-400/25 text-base;
  }

  #addBtn {
    @apply hover:text-green-400 z-10 text-2xl px-2 hover:bg-green-200/50 h-full 
              rounded-full duration-500 text-gray-500;
  }
</style>
