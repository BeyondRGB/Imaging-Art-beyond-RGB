<script>
  import PageTransitions from "@components/PageTransitions.svelte";
  import CanvasImage from "@components/CanvasImage.svelte";
</script>

<PageTransitions>
  <main>
    <div id="image">
      <CanvasImage />
    </div>
    <div id="side">
      Settings
      <div id="view-options">View Options</div>
      <div id="metadata">Metadata</div>
    </div>
  </main>
</PageTransitions>

<style lang="postcss">
  main {
    @apply flex w-full justify-center;
  }
  #image {
    @apply w-[75%] self-center mx-2;
  }
  #side {
    @apply w-[25%];
  }
  #view-options {
    @apply bg-gray-200 h-[15%] m-2;
  }
  #metadata {
    @apply bg-gray-200 h-[15%] m-2;
  }
</style>
