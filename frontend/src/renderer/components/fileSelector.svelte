<script>
  export let directory = false;
  export let title = "";
  let files;
</script>

<main class="">
  <span class="selector-text">{title}</span>
  {#if directory}
    <input id="ctrl" type="file" multiple webkitdirectory />
  {:else}
    <input type="file" bind:files accept="*" multiple />
  {/if}
</main>

<style lang="postcss">
  main {
    display: grid;

    grid-template-areas: "title button temp";

    /* grid-template-rows: repeat(3, auto); */
    grid-template-rows: auto;

    grid-template-columns: auto, auto, auto;
  }
  .selector {
    @apply flex select-none py-3 align-middle;
  }

  .selector-text {
    grid-area: title;
    @apply px-2 py-1;
  }

  .text-box {
    outline: 2px solid rgb(209, 213, 219);
    @apply bg-white px-2 py-1;
  }

  button {
    grid-area: button;
    z-index: 1;
    @apply bg-blue-500 rounded-md active:bg-blue-400 transition-all
            hover:bg-red-200 text-white font-bold py-2 px-4 w-full inline-flex items-center;
  }

  input {
    grid-area: button;
    @apply cursor-pointer block opacity-100;
  }
</style>
