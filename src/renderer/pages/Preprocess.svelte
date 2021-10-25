<script>

</script>

<main>
  <div class="settings">
    <div class="file-select">
      <span>Select Folder:</span>
      <button>Select Folder</button>
      <span class="selected-folder"></span>
    </div>
    <div class="export-select">
      <span>Export Destination:</span>
      <button>Select Folder</button>
      <span class="export-folder"></span>
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply w-full px-[10%] py-[5%] text-center;
  }

  .file-select {
    @apply flex gap-4;
  }
</style>