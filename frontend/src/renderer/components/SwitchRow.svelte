<script lang="ts">
  import Switch from "@components/Switch.svelte";
  
  export let label: string;
  export let description: string;
  export let checked: boolean;
  export let disabled: boolean = false;
  export let ariaLabel: string = "";
  
  let keyboardFocus = false;
  
  const toggle = () => !disabled && (checked = !checked);
  const handleKey = (e: KeyboardEvent) => {
    if (!disabled && (e.key === 'Enter' || e.key === ' ')) {
      e.preventDefault();
      keyboardFocus = true;
      toggle();
    }
  };
</script>

<div 
  class="row" 
  class:disabled
  class:focus={keyboardFocus}
  role="switch"
  tabindex={disabled ? -1 : 0}
  aria-label={ariaLabel || `Toggle ${label}`}
  aria-checked={checked}
  aria-disabled={disabled}
  on:click={toggle}
  on:keydown={handleKey}
  on:mousedown={() => keyboardFocus = false}
  on:blur={() => keyboardFocus = false}
>
  <div class="info">
    <span class="label">{label}</span>
    <span class="desc">{description}</span>
  </div>
  <Switch bind:checked {disabled} />
</div>

<style lang="postcss">
  .row {
    background-color: var(--color-surface-sunken);
    @apply flex items-center justify-between gap-6 py-3 px-4 rounded-lg 
           transition-colors cursor-pointer focus:outline-none;
  }
  
  .row:hover:not(.disabled) {
    background-color: var(--color-interactive-hover);
  }
  
  .row.focus {
    @apply ring-2 ring-blue-500 ring-offset-2;
    ring-offset-color: var(--color-surface-base);
  }

  .row.disabled {
    @apply opacity-50 cursor-not-allowed;
  }

  .info {
    @apply flex flex-col gap-1 flex-1 pointer-events-none select-none;
  }

  .label {
    color: var(--color-text-primary);
    @apply text-base font-medium;
  }

  .desc {
    color: var(--color-text-tertiary);
    @apply text-sm;
  }
  
  .row :global(.group) {
    @apply pointer-events-none;
  }
</style>

