<script>
  import Switch from "@components/Switch.svelte";
  import PageTransitions from "@components/PageTransitions.svelte";
  import CanvasImage from "@components/CanvasImage.svelte";
</script>

<PageTransitions>
  <main>
    <div id="image">
      <CanvasImage />
    </div>
    <div id="side">
      Settings
      <Switch label="Truecolor" />
      {#each [1, 2, 3, 4, 5, 6, 7, 8, 9, 10] as channel}
        <Switch label={`Channel ${channel}`} />
      {/each}
    </div>
  </main>
</PageTransitions>

<style lang="postcss">
  main {
    @apply flex w-full justify-center;
  }
  #image {
    @apply w-[75%] self-center mx-2;
  }
  #side {
    @apply w-[25%] self-center;
  }
</style>
