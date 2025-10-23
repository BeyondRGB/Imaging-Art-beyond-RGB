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
    @apply flex items-center justify-between gap-6 py-3 px-4 
           bg-gray-700/50 rounded-lg hover:bg-gray-700 transition-colors 
           cursor-pointer focus:outline-none;
  }
  
  .row.focus {
    @apply ring-2 ring-blue-500 ring-offset-2 ring-offset-gray-800;
  }

  .row.disabled {
    @apply opacity-50 cursor-not-allowed hover:bg-gray-700/50;
  }

  .info {
    @apply flex flex-col gap-1 flex-1 pointer-events-none select-none;
  }

  .label {
    @apply text-base text-gray-100 font-medium;
  }

  .desc {
    @apply text-sm text-gray-400;
  }
  
  .row :global(.group) {
    @apply pointer-events-none;
  }
</style>

