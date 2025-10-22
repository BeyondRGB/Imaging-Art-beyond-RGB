<script lang="ts">
  /**
   * Generic Scroll Container
   * Provides consistent scrollbar styling across the app
   * Eliminates duplicate scrollbar CSS
   */
  
  export let maxHeight: string = 'none';
  export let className: string = '';
</script>

<div 
  class="scroll-container {className}"
  style="max-height: {maxHeight};"
>
  <slot />
</div>

<style lang="postcss">
  .scroll-container {
    @apply overflow-auto;
  }
  
  /* Consistent scrollbar styling for all scroll containers */
  .scroll-container::-webkit-scrollbar {
    @apply w-1 h-1;
  }

  .scroll-container::-webkit-scrollbar-track {
    @apply bg-transparent;
  }

  .scroll-container::-webkit-scrollbar-thumb {
    background-color: var(--color-border);
    @apply rounded-full;
  }

  .scroll-container::-webkit-scrollbar-thumb:hover {
    background-color: var(--color-interactive-hover);
  }
</style>

