<script lang="ts">
  /**
   * Generic Expandable Panel Component
   * Provides a slide-out panel with expand/collapse functionality
   */
  import { ChevronLeftIcon, ChevronRightIcon } from "svelte-feather-icons";
  
  export let expanded: boolean = false;
  export let position: 'right' | 'left' = 'right';
  export let width: string = '30vw';
  export let handlePosition: string = '50%'; // CSS value for top/bottom positioning
  export let zIndex: number = 50; // Configurable z-index
</script>

<div 
  class="expandable-panel"
  class:expanded
  class:position-right={position === 'right'}
  class:position-left={position === 'left'}
  style="width: {width}; z-index: {zIndex};"
>
  <slot />
</div>

<button
  class="panel-handle"
  class:expanded
  class:position-right={position === 'right'}
  class:position-left={position === 'left'}
  style="top: {handlePosition}; --panel-width: {width}; z-index: {zIndex + 1};"
  on:click={() => (expanded = !expanded)}
  aria-label={expanded ? "Collapse panel" : "Expand panel"}
>
  {#if position === 'right'}
    {#if expanded}
      <ChevronRightIcon size="1.5x" />
    {:else}
      <ChevronLeftIcon size="1.5x" />
    {/if}
  {:else}
    {#if expanded}
      <ChevronLeftIcon size="1.5x" />
    {:else}
      <ChevronRightIcon size="1.5x" />
    {/if}
  {/if}
</button>

<style lang="postcss">
  .expandable-panel {
    background-color: var(--color-surface-elevated);
    border: 1px solid var(--color-border);
    box-shadow: -4px 0 8px rgba(0, 0, 0, 0.1);
    @apply fixed top-0 bottom-0 h-screen overflow-y-auto transition-all duration-500
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
    background-color: var(--color-surface);
    color: var(--color-text-primary);
    border: 1px solid var(--color-border);
    box-shadow: -2px 0 4px rgba(0, 0, 0, 0.1);
    @apply fixed h-12 w-10 flex justify-center items-center
           cursor-pointer transition-all duration-500
           transform -translate-y-1/2 hover:bg-opacity-90;
  }
  
  .panel-handle:hover {
    background-color: var(--color-interactive-hover);
  }
  
  .panel-handle:focus {
    outline: 2px solid var(--color-border-focus);
    outline-offset: 2px;
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

