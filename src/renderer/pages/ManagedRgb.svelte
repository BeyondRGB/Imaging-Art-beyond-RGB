<script>
</script>

<main>
  <div id="image">Image</div>
  <div id="side">
    Settings
    <div id="view-options">View Options</div>
    <div id="metadata">Metadata</div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex w-full justify-center;
  }
  #image {
    @apply bg-yellow-600 h-[82%] min-w-[75%] self-center mx-2;
  }
  #side {
    @apply w-[25%];
  }
  #view-options {
    @apply bg-gray-50 h-[15%] m-2;
  }
  #metadata {
    @apply bg-gray-50 h-[15%] m-2;
  }
</style>
