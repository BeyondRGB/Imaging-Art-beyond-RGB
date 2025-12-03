<script lang="ts">
  /**
   * Generic Text Input Row Component
   * Reusable component for labeled text inputs with consistent styling
   */
  export let label: string;
  export let value: string | number = "";
  export let placeholder: string = "";
  export let icon = null;
  export let type: "text" | "number" = "text";
  export let min: number | undefined = undefined;
  export let max: number | undefined = undefined;
  export let disabled: boolean = false;
  export let className: string = "";
</script>

<div class="input-row {className}">
  <label class="input-label">
    {#if icon}
      <svelte:component this={icon} size="1x" />
    {/if}
    <span>{label}</span>
  </label>
  {#if type === "number"}
    <input
      class="input-field"
      type="number"
      {min}
      {max}
      {placeholder}
      {disabled}
      bind:value
      on:change
      on:input
    />
  {:else}
    <input
      class="input-field"
      type="text"
      {placeholder}
      {disabled}
      bind:value
      on:change
      on:input
    />
  {/if}
</div>

<style lang="postcss">
  .input-row {
    @apply w-full flex flex-col gap-1.5;
  }
  
  .input-label {
    @apply flex items-center gap-2;
    color: var(--color-text-secondary);
    font-weight: 500;
    font-size: 0.8125rem;
    letter-spacing: 0.01em;
  }
  
  .input-field {
    background-color: var(--color-surface-base);
    border: 1px solid var(--color-border);
    color: var(--color-text-primary);
    @apply px-3 py-2 rounded-md w-full text-sm;
    transition: all 0.15s ease;
  }
  
  .input-field:hover {
    border-color: var(--color-text-tertiary);
  }
  
  .input-field:focus {
    outline: none;
    border-color: rgb(59, 130, 246);
    box-shadow: 0 0 0 3px rgba(59, 130, 246, 0.1);
  }
  
  .input-field:disabled {
    opacity: 0.5;
    cursor: not-allowed;
  }
  
  .input-field::placeholder {
    color: var(--color-text-tertiary);
  }
</style>

