<script>
  export let label = "Placeholder...";
</script>

<div>
  <label class="group">
    <p>{label}</p>
    <input type="checkbox" class="peer" />
    <span />
  </label>
</div>

<style lang="postcss">
  div {
    @apply flex justify-center items-center flex-col bg-gray-100;
  }
  p {
    @apply flex-grow;
  }
  span {
    @apply w-8 h-5 flex items-center flex-shrink-0 ml-2 p-0.5 bg-gray-300
          rounded-full duration-300 ease-in-out after:w-4 after:h-4 after:bg-white 
          after:rounded-full after:shadow-md after:duration-300 peer-checked:bg-green-400
          peer-checked:after:translate-x-3 group-hover:after:translate-x-0.5;
  }

  label {
    @apply relative flex justify-between items-center px-4 py-2 text-base w-full hover:bg-green-200 transition-all;
  }
  input {
    @apply absolute left-0 top-0 w-full h-full appearance-none rounded-md;
  }
</style>
