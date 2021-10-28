<script>
  import Switch from "../components/Switch.svelte"
</script>

<main>
  <div id="image">Image</div>
  <div id="side">
    Settings
    <Switch label="Truecolor"/>
    {#each [1,2,3,4,5,6,7,8,9,10] as channel}
      <Switch label={`Channel ${channel}`}/>
    {/each}
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex w-full justify-center;
  }
  #image {
    @apply bg-yellow-600 h-[82%] min-w-[75%] self-center mx-2;
  }
  #side {
    @apply w-[25%] self-center;
  }
</style>