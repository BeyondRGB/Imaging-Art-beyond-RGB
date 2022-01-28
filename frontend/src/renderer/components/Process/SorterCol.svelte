<script lang="ts">
  import DragBox from "@components/Process/DragBox.svelte";
  export let colId: number;
</script>

<main>
  <div>
    <span>Image</span>
    <DragBox items={[]} strict {colId} />
  </div>
  <div>
    <span>White Field</span>
    <DragBox items={[]} strict {colId} />
  </div>
  <div>
    <span>Dark Field</span>
    <DragBox items={[]} strict {colId} />
  </div>
</main>

<style lang="postcss">
  main {
    @apply gap-2;
  }
  div {
    @apply w-full bg-blue-200 rounded-lg p-1;
  }
</style>
