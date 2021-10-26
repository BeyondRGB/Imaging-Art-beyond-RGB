<script>
  import { pageId } from '../stores';
</script>

<main>
  <button on:click={() => pageId.set(4)}>Proccess</button>
  <button>View</button>
</main>

<style lang="postcss">
  main {
    background-color: var(--box-color);
    @apply w-full h-full flex flex-col items-center justify-center gap-4;  
  }

  button {
    @apply bg-blue-500 w-[60%] h-[12%] rounded-lg text-white text-xl active:bg-blue-600 select-none
						flex flex-col justify-center items-center;
  }
</style>