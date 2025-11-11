<script lang="ts">
  import FileSelector from "@components/FileSelector.svelte";
  import TextInputRow from "@components/TextInputRow.svelte";
  import { FolderPlusIcon, FolderIcon, FileTextIcon } from "svelte-feather-icons";
  import { currentPage, processState } from "@util/stores";
  import Card from "@components/Card.svelte";
  
  let filePaths = [];
  $: console.log(filePaths);

  $: if (filePaths) {
    processState.update(state => ({
      ...state,
      destDir: filePaths[0]
    }));
  }
  $: if (
    $processState.destDir &&
    $processState.destDir?.length > 1 &&
    !$processState.completedTabs[2]
  ) {
    processState.update(state => ({
      ...state,
      completedTabs: state.completedTabs.map((completed, i) => i === 2 ? true : completed)
    }));
  }
</script>

<main>
  <left>
    <h1>Output Destination</h1>
    <p>Select the destination for output files</p>
  </left>
  <right>
    <div class="content-wrapper">
      <Card variant="default" padding="lg" rounded={true} className="destination-card">
        <div class="card-header">
          <FolderIcon size="1.5x" />
          <h2>Destination Settings</h2>
        </div>
        
        <div class="form-section">
          <div class="select-button-wrapper">
            <FileSelector
              bind:filePaths
              type="Dir"
              label="Select Folder"
              icon={FolderPlusIcon}
              largeText
            />
          </div>
          
          {#if filePaths?.length > 0}
            <div class="selected-folder">
              <div class="folder-icon-wrapper">
                <FolderIcon size="1.25x" />
              </div>
              <div class="folder-path">
                <span class="folder-label">Selected Location</span>
                <span class="folder-value">{$processState.destDir}</span>
              </div>
            </div>
          {/if}
        </div>
        
        <div class="divider"></div>
        
        <div class="form-section">
          <TextInputRow 
            label="Output Filename"
            icon={FileTextIcon}
            bind:value={$processState.destFileName}
            placeholder="Enter output filename..."
            type="text"
          />
        </div>
      </Card>
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
    @apply w-full h-full p-8 flex items-center justify-center;
  }
  
  h1 {
    color: var(--color-text-primary);
    @apply text-3xl font-semibold mb-4;
  }
  
  p {
    background-color: var(--color-overlay-light);
    color: var(--color-text-secondary);
    @apply text-center pt-[30vh] m-6 h-[90%] rounded-lg text-base;
  }
  
  .content-wrapper {
    @apply w-full max-w-xl;
  }
  
  :global(.destination-card) {
    background-color: var(--color-surface-elevated);
    @apply shadow-sm !important;
  }
  
  .card-header {
    @apply flex items-center gap-2.5 mb-5 pb-4;
    color: var(--color-text-primary);
    border-bottom: 1px solid var(--color-border);
  }
  
  .card-header h2 {
    @apply text-lg font-semibold;
  }
  
  .form-section {
    @apply flex flex-col gap-5;
  }
  
  .select-button-wrapper {
    @apply w-full flex justify-center;
  }
  
  .selected-folder {
    background-color: var(--color-surface-base);
    border: 1px solid var(--color-border);
    @apply flex items-start gap-3 p-3.5 rounded-lg transition-all duration-150;
  }
  
  .folder-icon-wrapper {
    color: rgb(59, 130, 246);
    @apply flex items-center justify-center w-9 h-9 rounded-md flex-shrink-0;
    background-color: rgba(59, 130, 246, 0.08);
  }
  
  .folder-path {
    @apply flex flex-col gap-0.5 flex-1 min-w-0;
  }
  
  .folder-label {
    color: var(--color-text-secondary);
    @apply text-xs font-medium;
  }
  
  .folder-value {
    color: var(--color-text-primary);
    @apply text-sm break-all font-mono;
  }
  
  .divider {
    background-color: var(--color-border);
    @apply h-px w-full my-3;
  }
</style>
