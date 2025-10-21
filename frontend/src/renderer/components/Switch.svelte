<script lang="ts">
  export let label: string = "Placeholder...";
  export let checked: boolean = false;
  export let large = false;
  import { appSettings } from "@util/stores";

  $: theme = $appSettings.theme ? "dark" : "";
</script>

<label class="group" class:large>
  {#if label}
    <p>{label}</p>
  {/if}
  <input type="checkbox" class="peer" bind:checked />
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
    @apply w-11 h-6 flex items-center flex-shrink-0 p-0.5 rounded-full 
          duration-200 ease-in-out after:w-5 after:h-5 
          after:rounded-full after:shadow-sm after:duration-200 
          bg-gray-600 peer-checked:after:translate-x-5
          after:bg-gray-300 peer-checked:bg-blue-500;
  }
  .large p {
    @apply text-xl;
  }
  input {
    @apply absolute left-0 top-0 w-full h-full appearance-none cursor-pointer opacity-0;
  }
</style>
