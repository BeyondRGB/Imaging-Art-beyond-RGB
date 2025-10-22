<script lang="ts">
  export let label: string = "Placeholder...";
  export let checked: boolean = false;
  export let large = false;
  export let disabled: boolean = false;
  import { appSettings } from "@util/stores";

  $: theme = $appSettings.theme ? "dark" : "";
</script>

<label class="group" class:large class:disabled>
  {#if label}
    <p>{label}</p>
  {/if}
  <input type="checkbox" class="peer" bind:checked {disabled} />
  <span />
</label>

<style lang="postcss">
  label {
    @apply relative flex items-center gap-3 cursor-pointer;
  }
  p {
    @apply hidden;
  }
  span {
    background-color: var(--color-interactive);
    @apply w-11 h-6 flex items-center flex-shrink-0 p-0.5 rounded-full 
          duration-200 ease-in-out after:w-5 after:h-5 
          after:rounded-full after:shadow-sm after:duration-200 
          peer-checked:after:translate-x-5 peer-checked:bg-blue-500;
  }
  
  span::after {
    background-color: var(--color-surface-elevated);
    content: "";
  }
  .large p {
    @apply text-xl;
  }
  input {
    @apply absolute left-0 top-0 w-full h-full appearance-none opacity-0;
  }
  .disabled {
    @apply opacity-50;
  }
</style>
