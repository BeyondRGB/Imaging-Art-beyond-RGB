<script>
  import DList from "./DList.svelte";
  let items = [
    { id: 1, name: "item1" },
    { id: 2, name: "item2" },
    { id: 3, name: "item3" },
    { id: 4, name: "item4" },
  ];
  let items1 = [
    { id: 5, name: "item5" },
    { id: 6, name: "item6" },
    { id: 7, name: "item7" },
    { id: 8, name: "item8" },
  ];
  let items2 = [
    { id: 9, name: "item9" },
    { id: 10, name: "item10" },
    { id: 11, name: "item11" },
    { id: 12, name: "item12" },
  ];
  let items3 = [
    { id: 13, name: "item13" },
    { id: 14, name: "item14" },
    { id: 15, name: "item15" },
    { id: 16, name: "item16" },
  ];
</script>

<main>
  <DList {items} />
  <DList items={items1} />
  <DList items={items2} />
  <DList items={items3} />
</main>

<style lang="postcss">
  main {
    @apply flex gap-2 bg-gray-300 h-[35vh] w-[50vw];
  }
</style>
