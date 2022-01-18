<script lang="ts">
  import Sorter from "@components/Process/Sorter.svelte";
</script>

<main>
  <left>
    <h1>Specifiy File Roles</h1>
    <p>
      Drag and drop the images to there corresponding role in the Spectral Image
    </p>
  </left>
  <right>
    <div>
      <Sorter />
    </div></right
  >
</main>

<style lang="postcss">
  main {
    @apply flex justify-between h-full w-full overflow-hidden;
  }
  left {
    @apply bg-gray-600 w-full h-full p-6 flex-col;
  }
  right {
    @apply bg-gray-700 w-full h-full p-6 flex justify-center overflow-auto;
  }
  h1 {
    @apply text-3xl;
  }
  p {
    @apply text-center pt-[30vh] bg-gray-500/25 m-6 h-[90%] rounded-lg;
  }
  input {
    @apply translate-y-[35vh];
  }
</style>
