<script lang="ts">
  import FileSelector from "@components/FileSelector.svelte";

  let filePaths = [];
  $: console.log(filePaths);
</script>

<main>
  <left>
    <h1>Select Destination Files Location</h1>
    <p>
      Select the folder where you would like to save the spectral images after
      processing
    </p>
  </left>
  <right>
    <div class="fileSelector">
      <FileSelector bind:filePaths type="Dir" />
    </div>
    <article>
      {filePaths}
    </article>
  </right>
</main>

<style lang="postcss">
  main {
    @apply flex justify-between h-full w-full overflow-hidden;
  }
  left {
    @apply bg-gray-600 w-full h-full p-6 flex-col;
  }
  right {
    @apply bg-gray-700 w-full h-full p-6 flex flex-col items-center justify-center;
  }
  h1 {
    @apply text-3xl;
  }
  p {
    @apply text-center pt-[30vh] bg-gray-500/25 m-6 h-[90%] rounded-lg;
  }
  article {
    @apply w-full h-full bg-gray-800 m-16;
  }
</style>
