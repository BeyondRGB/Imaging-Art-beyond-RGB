<script lang="ts">
  /**
   * Generic Card Component
   * Provides consistent styling for card-like containers throughout the app
   */
  
  export let variant: 'default' | 'elevated' | 'dark' | 'outlined' = 'default';
  export let padding: 'none' | 'sm' | 'md' | 'lg' = 'md';
  export let rounded: boolean = true;
  export let shadow: boolean = false;
</script>

<div 
  class="card"
  class:card-default={variant === 'default'}
  class:card-elevated={variant === 'elevated'}
  class:card-dark={variant === 'dark'}
  class:card-outlined={variant === 'outlined'}
  class:padding-none={padding === 'none'}
  class:padding-sm={padding === 'sm'}
  class:padding-md={padding === 'md'}
  class:padding-lg={padding === 'lg'}
  class:rounded={rounded}
  class:shadow={shadow}
>
  <slot />
</div>

<style lang="postcss">
  .card {
    @apply relative transition-colors duration-200;
  }
  
  /* Variant styles - NO color literals! */
  .card-default {
    background-color: var(--color-surface);
  }
  
  .card-elevated {
    background-color: var(--color-surface-elevated);
  }
  
  .card-dark {
    background-color: var(--color-surface-sunken);
  }
  
  .card-outlined {
    background-color: transparent;
    border: 1px solid var(--color-border);
  }
  
  /* Padding options */
  .padding-none {
    @apply p-0;
  }
  
  .padding-sm {
    @apply p-2;
  }
  
  .padding-md {
    @apply p-4;
  }
  
  .padding-lg {
    @apply p-6;
  }
  
  /* Rounded corners */
  .rounded {
    @apply rounded-xl;
  }
  
  /* Shadow */
  .shadow {
    @apply shadow-lg;
  }
</style>

