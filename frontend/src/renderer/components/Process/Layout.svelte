<script>
  export let tabs;
  export let tabList;
</script>

<div class="pages" bind:this={tabList}>
  {#each tabs as tab}
    <div class="item">
      <svelte:component this={tab.component} />
    </div>
  {/each}
</div>

<style lang="postcss">
  .pages {
    overflow-x: hidden;
    scroll-snap-type: x mandatory;
    scroll-behavior: smooth;
    overflow: overlay;
    @apply w-full h-full flex overflow-hidden;
  }
  .item {
    scroll-snap-align: start;
    @apply flex-shrink-0 w-full h-full;
  }
</style>
