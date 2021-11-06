<script>
  import DList from "@components/DList.svelte";
  import HeapBox from "@components/HeapBox.svelte";
  import SorterCol from "@components/SorterCol.svelte";
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
  <div id="heap">
    <HeapBox items={[...items, ...items1, ...items2, ...items3]} />
  </div>
  <div id="cols">
    <SorterCol />
    <SorterCol />
    <SorterCol />
    <SorterCol />
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex flex-col justify-center items-center;
  }
  #cols {
    @apply flex gap-2;
  }
  #heap {
    @apply mb-2;
  }
</style>
