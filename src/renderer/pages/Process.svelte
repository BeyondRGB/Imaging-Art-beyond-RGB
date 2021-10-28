<script>
  import { pageId } from '../stores';
</script>

<main>
  <div id="image">Image</div>
  <div id="stepper">
    Steps 
    <button on:click={() => pageId.set(1)} class="bg-blue-500 rounded-md px-2 py-1">[Next]</button>
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex flex-col w-full justify-center;
  }
  #image {
    @apply bg-yellow-600 h-[80%] min-w-[75%] self-center;
  }
  #stepper {
    @apply bg-red-400 h-[17%] mt-auto mb-[1%];
  }
</style>