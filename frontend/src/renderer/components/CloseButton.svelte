<script lang="ts">
  /**
   * Generic Close Button Component
   * Provides consistent close button styling across the app
   */
  import { XIcon } from "svelte-feather-icons";
  
  export let onClick: () => void = () => {};
  export let variant: 'default' | 'absolute-top-right' | 'absolute-top-left' | 'floating' = 'default';
  export let size: string = "1.25x";
  export let iconComponent = XIcon;
</script>

<button 
  class="close-btn"
  class:absolute-top-right={variant === 'absolute-top-right'}
  class:absolute-top-left={variant === 'absolute-top-left'}
  class:floating={variant === 'floating'}
  on:click={onClick}
  aria-label="Close"
>
  <svelte:component this={iconComponent} {size} />
</button>

<style lang="postcss">
  .close-btn {
    @apply bg-transparent ring-0 p-1 transition-all duration-300
           hover:bg-red-400/50;
  }
  
  .close-btn.absolute-top-right {
    @apply absolute right-0 top-0 m-2 z-50;
  }
  
  .close-btn.absolute-top-left {
    @apply absolute left-0 top-0 m-2 z-50;
  }
  
  .close-btn.floating {
    @apply absolute right-8 m-1 z-50;
  }
</style>

