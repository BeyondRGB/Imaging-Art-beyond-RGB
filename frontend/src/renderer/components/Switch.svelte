<script>
  export let label = "Placeholder...";
  export let checked = false;
</script>

<div class="bg-gray-100 dark:bg-gray-600 dark:text-white">
  <label class="group hover:bg-green-200 dark:hover:bg-gray-900/50">
    <p>{label}</p>
    <input type="checkbox" class="peer" bind:checked />
    <span
      class="dark:after:bg-gray-300 after:bg-white dark:peer-checked:bg-green-800 peer-checked:bg-green-400 ring-gray-200 dark:ring-gray-500"
    />
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
    @apply w-8 h-5 flex items-center flex-shrink-0 ml-2 p-0.5 rounded-full 
          duration-300 ease-in-out after:w-4 after:h-4  
          after:rounded-full after:shadow-md after:duration-300 
          peer-checked:after:translate-x-3 group-hover:after:translate-x-0.5 ring ring-2;
  }

  label {
    @apply relative flex justify-between items-center px-4 py-2 text-base w-full transition-all;
  }
  input {
    @apply absolute left-0 top-0 w-full h-full appearance-none rounded-md;
  }
</style>
