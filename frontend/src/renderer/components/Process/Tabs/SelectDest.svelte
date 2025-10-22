<script lang="ts">
  import FileSelector from "@components/FileSelector.svelte";
  import { FolderPlusIcon, FolderIcon } from "svelte-feather-icons";
  import { currentPage, processState } from "@util/stores";
  let filePaths = [];
  $: console.log(filePaths);

  $: if (filePaths) {
    $processState.destDir = filePaths[0];
  }
  $: if (
    $processState.destDir &&
    $processState.destDir?.length > 1 &&
    !$processState.completedTabs[2]
  ) {
    $processState.completedTabs[2] = true;
  }
</script>

<main>
  <left>
    <h1>Output Destination</h1>
    <p>Select the destination for output files</p>
  </left>
  <right>
    <div class="selectBox">
      <div class="fileSelector">
        <FileSelector
          bind:filePaths
          type="Dir"
          label="Select Folder"
          icon={FolderPlusIcon}
          largeText
        />
      </div>
      {#if filePaths?.length > 0}
        <div class="folderDisp">
          <div class="destLabel">
            <FolderIcon size="3x" />
            Destination Folder:
          </div>
          <div class="folderLoc">
            {$processState.destDir}
          </div>
        </div>
      {/if}
    </div>
    <div>
      <div class="outputNameDisp">
        <div class="outputLabel">
          Destination Filename:
        </div>
        <input class="outputName" bind:value={$processState.destFileName} />
      </div>
    </div>
  </right>
</main>

<style lang="postcss">
  main {
    @apply flex justify-between h-full w-full overflow-hidden;
  }
  left {
    background-color: var(--color-surface-elevated);
    @apply w-full h-full p-6 flex-col;
  }
  right {
    background-color: var(--color-surface);
    @apply w-full h-full p-6 flex flex-col items-center;
  }
  h1 {
    @apply text-3xl;
  }
  p {
    background-color: var(--color-overlay-light);
    @apply text-center pt-[30vh] m-6 h-[90%] rounded-lg;
  }
  .folderDisp {
    @apply flex justify-between m-1 text-base;
  }
  .destLabel {
    @apply bg-blue-500/50 flex justify-between items-center p-1 rounded-l-xl;
  }
  .folderLoc {
    background-color: var(--color-surface-base);
    @apply flex items-center justify-center rounded-r-xl p-1;
  }
  .selectBox {
    @apply h-[70%] flex flex-col justify-center items-center gap-2;
  }
  .outputNameDisp {
    @apply flex justify-between m-1 text-base;
  }
  .outputLabel {
    @apply bg-blue-500/50 flex justify-between items-center p-1 rounded-l-xl;
  }
  .outputName {
    background-color: var(--color-surface-base);
    @apply flex items-center justify-center rounded-r-xl p-1;
  }
</style>
