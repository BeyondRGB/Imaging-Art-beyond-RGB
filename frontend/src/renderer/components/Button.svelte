<script lang="ts">
  /**
   * Generic Button Component
   * Eliminates button styling duplication across the app
   */
  
  export let variant: 'default' | 'success' | 'danger' | 'secondary' = 'default';
  export let size: 'sm' | 'md' | 'lg' = 'md';
  export let type: 'button' | 'submit' | 'reset' = 'button';
  export let disabled: boolean = false;
  export let loading: boolean = false;
  export let onClick: ((e: MouseEvent) => void) | undefined = undefined;
  export let icon: any = undefined; // Icon component from svelte-feather-icons
  export let iconPosition: 'left' | 'right' = 'left';
  export let className: string = '';
</script>

<button 
  class="btn {className}"
  class:btn-default={variant === 'default'}
  class:btn-success={variant === 'success'}
  class:btn-danger={variant === 'danger'}
  class:btn-secondary={variant === 'secondary'}
  class:btn-sm={size === 'sm'}
  class:btn-md={size === 'md'}
  class:btn-lg={size === 'lg'}
  class:btn-disabled={disabled || loading}
  {type}
  disabled={disabled || loading}
  on:click={onClick}
  on:click
>
  {#if loading}
    <span class="btn-loading">Loading...</span>
  {:else}
    {#if icon && iconPosition === 'left'}
      <span class="btn-icon-left">
        <svelte:component this={icon} size="1em" />
      </span>
    {/if}
    <slot />
    {#if icon && iconPosition === 'right'}
      <span class="btn-icon-right">
        <svelte:component this={icon} size="1em" />
      </span>
    {/if}
  {/if}
</button>

<style lang="postcss">
  .btn {
    @apply rounded-lg self-center transition-all duration-200
           active:scale-[0.98] shadow-sm ring-1 focus:ring-2 focus:outline-none
           flex items-center justify-center gap-2 font-medium;
    border: 1px solid transparent;
  }
  
  /* Size variants */
  .btn-sm {
    @apply text-sm px-2 py-1;
  }
  
  .btn-md {
    @apply text-base px-4 py-2;
  }
  
  .btn-lg {
    @apply text-lg px-6 py-3;
  }
  
  /* Variant styles */
  .btn-default {
    background-color: var(--color-interactive);
    color: var(--color-text-primary);
    border-color: var(--color-border);
  }
  
  .btn-default:focus {
    outline: 2px solid var(--color-border-focus);
    outline-offset: 2px;
  }
  
  .btn-default:hover:not(.btn-disabled) {
    background-color: var(--color-interactive-hover);
    @apply shadow-md;
  }
  
  .btn-default:active:not(.btn-disabled) {
    background-color: var(--color-interactive-active);
  }
  
  /* Semantic colors can remain for success/danger/secondary as they're not theme-dependent */
  .btn-success {
    @apply bg-green-700 hover:bg-green-600 active:bg-green-800 text-white 
           ring-green-600 focus:ring-green-600 focus:ring-offset-2;
  }
  
  .btn-success:hover:not(.btn-disabled) {
    @apply shadow-md;
  }
  
  .btn-danger {
    @apply bg-red-600 hover:bg-red-500 active:bg-red-700 text-white 
           ring-red-600 focus:ring-red-600 focus:ring-offset-2;
  }
  
  .btn-danger:hover:not(.btn-disabled) {
    @apply shadow-md;
  }
  
  .btn-secondary {
    background-color: var(--color-interactive);
    color: var(--color-text-primary);
  }
  
  .btn-secondary:focus {
    outline: 2px solid var(--color-border-focus);
    outline-offset: 2px;
  }
  
  .btn-secondary:hover:not(.btn-disabled) {
    background-color: var(--color-interactive-hover);
    @apply shadow-md;
  }
  
  .btn-secondary:active:not(.btn-disabled) {
    background-color: var(--color-interactive-active);
  }
  
  /* Disabled state */
  .btn-disabled {
    @apply opacity-50 cursor-not-allowed;
  }
  
  .btn-disabled:hover {
    @apply shadow-sm;
  }
  
  /* Icon positioning */
  .btn-icon-left,
  .btn-icon-right {
    @apply flex items-center;
  }
  
  /* Loading state */
  .btn-loading {
    @apply opacity-75;
  }
</style>

