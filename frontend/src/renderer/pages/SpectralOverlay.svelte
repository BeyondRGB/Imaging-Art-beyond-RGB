<script>
  import Switch from "@components/Switch.svelte";
  import CanvasImage from "@components/CanvasImage.svelte";
</script>

<main>
  <div id="image">
    <CanvasImage />
  </div>
  <div id="side">
    <Switch label="Truecolor" />
    {#each [1, 2, 3, 4, 5, 6, 7, 8, 9, 10] as channel}
      <Switch label={`Channel ${channel}`} />
    {/each}
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex w-full justify-center;
  }
  #image {
    @apply w-[75%] self-center mx-2 shadow-md;
  }
  #side {
    @apply w-[25%] self-center shadow-md mr-2;
  }
</style>
