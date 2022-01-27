<script lang="ts">
  export let label: string = "Placeholder...";
  export let checked: boolean = false;
  import { appSettings } from "@util/stores";

  $: theme = $appSettings.theme ? "dark" : "";
</script>

<div class="{theme} bg-gray-100 dark:bg-gray-600 dark:text-white">
  <label class="group">
    <p>{label}</p>
    <input type="checkbox" class="peer" bind:checked />
    <span />
  </label>
</div>

<style lang="postcss">
  div {
    @apply flex justify-center items-center flex-col;
  }
  p {
    @apply flex-grow;
  }
  span {
    @apply w-12 h-6 flex items-center flex-shrink-0 ml-2 p-0 rounded-full 
          duration-300 ease-in-out after:w-6 after:h-6 
          after:rounded-full after:shadow-md dark:after:shadow-md after:duration-300 
          dark:bg-gray-800/20 bg-gray-300 peer-checked:after:translate-x-6
          group-hover:after:translate-x-0.5 ring-0 dark:after:bg-gray-600 
          after:bg-white dark:peer-checked:bg-blue-500 peer-checked:bg-blue-400;
  }

  label {
    @apply relative flex justify-between items-center px-4 py-2 text-base w-full 
            transition-all hover:bg-blue-200 dark:hover:bg-blue-900/30;
  }
  input {
    @apply absolute left-0 top-0 w-full h-full appearance-none rounded-md;
  }
</style>
