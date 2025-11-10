<script lang="ts">
  /**
   * Generic Expandable Panel Component
   * Provides a slide-out panel with expand/collapse functionality
   */
  export let expanded: boolean = false;
  export let position: 'right' | 'left' = 'right';
  export let width: string = '30vw';
  export let handlePosition: string = '50%'; // CSS value for top/bottom positioning
</script>

<div 
  class="expandable-panel"
  class:expanded
  class:position-right={position === 'right'}
  class:position-left={position === 'left'}
  style="width: {width};"
>
  <slot />
</div>

<button
  class="panel-handle"
  class:expanded
  class:position-right={position === 'right'}
  class:position-left={position === 'left'}
  style="top: {handlePosition}; width: {width};"
  on:click={() => (expanded = !expanded)}
>
  {#if position === 'right'}
    {expanded ? ">" : "<"}
  {:else}
    {expanded ? "<" : ">"}
  {/if}
</button>

<style lang="postcss">
  .expandable-panel {
    background-color: var(--color-overlay-medium);
    border: 1px solid var(--color-border);
    @apply fixed top-0 bottom-0 h-screen overflow-y-auto z-10 transition-all duration-500
           transform;
  }
  
  /* Right-side positioning */
  .expandable-panel.position-right {
    @apply right-0 rounded-bl-xl translate-x-full;
  }
  
  .expandable-panel.position-right.expanded {
    @apply translate-x-0;
  }
  
  /* Left-side positioning */
  .expandable-panel.position-left {
    @apply left-0 rounded-br-xl -translate-x-full;
  }
  
  .expandable-panel.position-left.expanded {
    @apply translate-x-0;
  }
  
  /* Handle button */
  .panel-handle {
    background-color: var(--color-overlay-medium);
    border: 1px solid var(--color-border);
    @apply fixed h-12 w-8 flex justify-center items-center
           text-2xl z-20 cursor-pointer transition-all duration-500
           transform -translate-y-1/2;
  }
  
  .panel-handle.position-right {
    @apply right-0 rounded-l-full border-r-0;
  }
  
  .panel-handle.position-right.expanded {
    right: var(--panel-width, 30vw);
  }
  
  .panel-handle.position-left {
    @apply left-0 rounded-r-full border-l-0;
  }
  
  .panel-handle.position-left.expanded {
    left: var(--panel-width, 30vw);
  }
</style>

