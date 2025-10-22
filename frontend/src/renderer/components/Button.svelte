<script lang="ts">
  /**
   * Generic Button Component
   * Eliminates button styling duplication across the app
   */
  
  export let variant: 'default' | 'success' | 'danger' | 'secondary' = 'default';
  export let type: 'button' | 'submit' | 'reset' = 'button';
  export let disabled: boolean = false;
  export let onClick: ((e: MouseEvent) => void) | undefined = undefined;
</script>

<button 
  class="btn"
  class:btn-default={variant === 'default'}
  class:btn-success={variant === 'success'}
  class:btn-danger={variant === 'danger'}
  class:btn-secondary={variant === 'secondary'}
  {type}
  {disabled}
  on:click={onClick}
  on:click
>
  <slot />
</button>

<style lang="postcss">
  .btn {
    @apply rounded-lg self-center text-base px-3 py-1 transition-all 
           active:scale-95 shadow-sm ring-1 focus:ring-2 duration-300;
  }
  
  .btn-default {
    background-color: var(--color-interactive);
    color: var(--color-text-primary);
    border-color: var(--color-border);
    @apply focus:ring-blue-300;
  }
  
  .btn-default:hover {
    background-color: var(--color-interactive-hover);
  }
  
  /* Semantic colors can remain for success/danger/secondary as they're not theme-dependent */
  .btn-success {
    @apply bg-green-700 hover:bg-green-600 text-white 
           ring-green-600 focus:ring-green-600;
  }
  
  .btn-danger {
    @apply bg-red-600 hover:bg-red-500 text-white 
           ring-red-600 focus:ring-red-600;
  }
  
  .btn-secondary {
    background-color: var(--color-interactive);
    color: var(--color-text-primary);
    @apply focus:ring-blue-300;
  }
  
  .btn-secondary:hover {
    background-color: var(--color-interactive-hover);
  }
</style>

