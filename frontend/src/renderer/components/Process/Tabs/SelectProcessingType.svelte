<script lang="ts">
  import { currentPage, modal, processState, setTabCompleted } from "@util/stores";
  import {
    FileIcon,
    FolderIcon,
  } from "svelte-feather-icons";
  import Button from "@components/Button.svelte";

  function handleClick(page) {
    processState.update(state => ({
      ...state,
      processType: page,
      currentTab: state.currentTab + 1
    }));
    setTabCompleted(0);
    modal.set(null);
  }

  let showAbout = false;
</script>

<main>
  <div id="selectProcessingTypesContent">
    <div class="btnCol">
      <Button onClick={() => handleClick("Single")} className="homeBtn" size="lg">
        <div class="btnTitle">
          <FileIcon size="1.25x" />
          <h2>Single Image Processing</h2>
        </div>
        <span> Process a single RAW image set </span>
      </Button>
      <Button onClick={() => handleClick("Batch")} className="homeBtn" size="lg">
        <div class="btnTitle">
          <FolderIcon size="1.25x" />
          <h2>Batch Processing</h2>
        </div>
        <span> Process multiple image sets with the same Target, Flatfield and Darkfield </span>
      </Button>
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    background-color: var(--color-surface-base);
    @apply w-full h-[97%] mt-[3%] relative flex justify-center items-center;
  }

  #selectProcessingTypesContent {
    @apply h-[50vh] flex flex-col items-center justify-between mb-[15vh];
  }

  :global(.homeBtn) {
    background-color: var(--color-surface-sunken) !important;
    color: var(--color-text-primary) !important;
    @apply w-full h-full flex flex-col justify-center items-center p-[2vh] text-lg ring-0 !important;
  }
  
  :global(.homeBtn:hover) {
    background-color: var(--color-surface) !important;
    color: var(--color-text-primary) !important;
  }
  .btnCol {
    @apply w-[60vw] flex flex-col justify-center items-center gap-4;
  }
  h1 {
    @apply text-4xl font-bold;
  }
  img {
    /* transform: scale(0.5); */
    pointer-events: none;
    @apply h-[10vh];
  }
  .aboutContent {
    @apply absolute w-full h-full flex justify-center items-center;
  }
  .aboutBg {
    @apply absolute w-full h-full bg-black/40 flex justify-center items-center;
  }
  .aboutBox {
    @apply absolute w-1/2 h-auto z-[9999];
  }
  .closeBtn {
    @apply absolute right-0 m-2 bg-transparent ring-0 hover:bg-red-400/50
            p-1;
  }

  .btnTitle {
    @apply w-full flex justify-center items-center gap-1 p-1 text-xl;
  }

  #welcome {
    @apply text-black text-opacity-40 flex flex-col justify-center 
            items-center gap-2 pb-[1vh];
  }

  #getting-started {
    @apply text-base flex flex-col justify-center items-center;
  }

  span {
    @apply text-sm;
  }
</style>
