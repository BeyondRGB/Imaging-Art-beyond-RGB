<script lang="ts">
  import ImageImporter from "@components/ImageImporter.svelte";

</script>

<main>
  <left>
    <h1>Import Images</h1>
  </left>
  <right>
    <ImageImporter/>
  </right>
</main>

<style lang="postcss">
  main {
    @apply flex justify-between h-full w-full overflow-hidden;
  }
  left {
    @apply bg-gray-600 w-full h-full p-6 flex-col overflow-auto;
  }
  right {
    @apply bg-gray-700 w-full h-full p-6 flex flex-col justify-center items-center gap-4;
  }
  h1 {
    @apply text-3xl;
  }
  p {
    @apply text-center pt-[30vh] bg-gray-500/25 m-4 h-[90%] rounded-lg;
  }
  article {
    @apply bg-gray-800 w-full min-h-[12rem] overflow-auto rounded-[32px] py-2 px-6;
  }
  .number {
    @apply flex justify-center bg-gray-700/50;
  }
  ul {
    @apply flex flex-col gap-2 w-full justify-center items-center;
  }
</style>
