<script lang="ts">
  import Switch from "@components/Switch.svelte";

  let isSync: boolean = false;
  let channels: boolean[] = [true, false, false];
  let showNav: boolean = false;
</script>

<main>
  <div id="image">
    <!-- <Canvas bind:imageKeys={channels} bind:isSync bind:showNav /> -->
  </div>
  <div id="side">
    <!-- <Switch label="Truecolor" /> -->
    <Switch label="Curtian - Sync" bind:checked={isSync} />
    <Switch label="Channel 1" bind:checked={channels[0]} />
    <Switch label="Channel 2" bind:checked={channels[1]} />
    <Switch label="Channel 3" bind:checked={channels[2]} />
    <Switch label="Show Nav" bind:checked={showNav} />
    <!-- {#each [1, 2, 3, 4, 5, 6, 7, 8, 9, 10] as channel}
      <Switch label={`Channel ${channel}`} />
    {/each} -->
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex w-full justify-center;
  }
  #image {
    @apply w-full h-full self-center mx-2 shadow-md;
  }
  #side {
    @apply w-full self-center shadow-md mr-2;
  }
</style>
