<!--
  @component
  Empty State Box/Panel with Dropzone
  Displays a centered message with file selection button and drag-and-drop support
-->
<script lang="ts">
  import { createEventDispatcher } from "svelte";
  import { FilePlusIcon } from "svelte-feather-icons";
  import Dropzone from "svelte-file-dropzone";
  import Button from "@components/Button.svelte";

  export let title: string = "No File Selected";
  export let description: string = "";
  export let buttonLabel: string = "Select Files";
  export let filter: "project" | "raws" | "csv" | "none" = "none";

  const dispatch = createEventDispatcher<{ select: string[] }>();

  let isDragOver = false;

  // Map filter to accepted extensions
  const filterExtensions: Record<string, string[]> = {
    project: [".btrgb"],
    raws: [".cr2", ".raf", ".nef", ".arq", ".arw", ".tiff", ".tif", ".dng"],
    csv: [".csv"],
    none: []
  };

  function getAcceptedExtensions(): string[] {
    return filterExtensions[filter] || [];
  }

  function isValidFile(fileName: string): boolean {
    const extensions = getAcceptedExtensions();
    if (extensions.length === 0) return true;
    const lowerName = fileName.toLowerCase();
    return extensions.some(ext => lowerName.endsWith(ext));
  }

  function handleDrop(e: CustomEvent<{ acceptedFiles: File[] }>) {
    isDragOver = false;
    const files = e.detail.acceptedFiles;
    const validFiles = files.filter(f => isValidFile(f.name));
    
    if (validFiles.length > 0) {
      const paths = validFiles.map(f => (f as any).path);
      dispatch("select", paths);
    }
  }

  function handleDragEnter() {
    isDragOver = true;
  }

  function handleDragLeave() {
    isDragOver = false;
  }

  async function handleButtonClick() {
    const response = await window.electron.handle({ type: "File", filter });
    if (response && !response.canceled && response.filePaths?.length > 0) {
      dispatch("select", response.filePaths);
    }
  }
</script>

<div class="empty-state">
  <Dropzone
    on:drop={handleDrop}
    on:dragenter={handleDragEnter}
    on:dragleave={handleDragLeave}
    noClick
    disableDefaultStyles
    containerClasses="dropzone-container {isDragOver ? 'drag-over' : ''}"
  >
    <div class="empty-state-box">
      {#if title}
        <h2>{title}</h2>
      {/if}
      {#if description}
        <p>{description}</p>
      {/if}
      
      <Button
        variant="primary"
        size="md"
        onClick={handleButtonClick}
        icon={FilePlusIcon}
        iconPosition="right"
      >
        {buttonLabel}
      </Button>
      
      <span class="drop-hint">or drag and drop files here</span>
    </div>
  </Dropzone>
</div>

<style lang="postcss">
  .empty-state {
    background-color: var(--color-surface-base);
    @apply absolute w-full h-full z-[99] flex justify-center items-center;
  }
  
  .empty-state :global(.dropzone-container) {
    @apply flex justify-center items-center p-8 rounded-2xl transition-all duration-200;
    border: 2px dashed var(--color-border);
  }
  
  .empty-state :global(.dropzone-container.drag-over) {
    border-color: var(--color-interactive);
    background-color: var(--color-surface-elevated);
    border-width: 3px;
  }
  
  .empty-state-box {
    background-color: var(--color-surface);
    @apply w-auto h-auto flex flex-col gap-4 justify-center items-center
          p-8 rounded-2xl;
  }
  
  .empty-state-box h2 {
    @apply text-xl;
  }
  
  .empty-state-box p {
    @apply text-sm text-center;
    color: var(--color-text-secondary);
  }
  
  .drop-hint {
    @apply text-sm;
    color: var(--color-text-tertiary);
  }
</style>
