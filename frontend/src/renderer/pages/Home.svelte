<script lang="ts">
  import TextLogo from "@assets/TextLogo.svg";
  import About from "@root/components/About.svelte";
  import Modal from "@root/components/Modal.svelte";
  import { currentPage, modal } from "@util/stores";
  import { fade } from "svelte/transition";
  import {
    CrosshairIcon,
    ApertureIcon,
    InfoIcon,
    CopyIcon
  } from "svelte-feather-icons";
  import Button from "@components/Button.svelte";
    

  function handleClick(page) {
    currentPage.set(page);
    modal.set(null);
  }
  
  let showAbout = false;
  
  //Open a new electron window of the BeyondRGB application using IPC
  const openNewWindow = () => {
    window.electron.openNewWindow();
  }

</script>

<main>
  {#if showAbout}
    <Modal 
      component={About} 
      size="large"
      backdropBlur="md"
      backdropOpacity="heavy"
      on:close={() => (showAbout = false)}
    />
  {/if}
  <div id="homeContent">
    <div id="welcome">
      <h1 in:fade={{ duration: 1000, delay: 1000 }} class="dark:text-gray-400">
        Welcome to
      </h1>
      <img
        in:fade={{ duration: 1500, delay: 1750 }}
        src={TextLogo}
        alt="app-logo"
      />
    </div>
    <div class="btnCol">
      <Button onClick={() => handleClick("Process")} className="homeBtn" size="lg">
        <div class="btnTitle">
          <ApertureIcon size="1.25x" />
          <h2>Process</h2>
        </div>
        <span> Process a new RAW image set </span>
      </Button>
      <Button onClick={() => handleClick("SpecPicker")} className="homeBtn" size="lg">
        <div class="btnTitle">
          <CrosshairIcon size="1.25x" />
          <h2>View</h2>
        </div>
        <span> View a previously-processed imaged set </span>
      </Button>
      <Button onClick={() => openNewWindow()} className="homeBtn" size="lg">
        <div class="btnTitle">
          <CopyIcon size="1.25x" />
          <h2>Create Another Window</h2>
        </div>
        <span> View two reports at once </span>
      </Button>
      <Button onClick={() => (showAbout = true)} className="homeBtn" size="lg">
        <div class="btnTitle">
          <InfoIcon size="1.25x" />
          <h2>About</h2>
        </div>
        <span> About the program </span>
      </Button>
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    background-color: var(--color-surface-base);
    @apply w-full h-[97%] mt-[3%] relative flex justify-center items-center;
  }

  #homeContent {
    @apply h-[50vh] flex flex-col items-center justify-between mb-[15vh];
  }

  :global(.homeBtn) {
    background-color: var(--color-surface) !important;
    color: var(--color-text-primary) !important;
    @apply w-full h-full flex flex-col justify-center items-center p-[2vh] text-lg ring-0 !important;
  }
  
  :global(.homeBtn:hover) {
    background-color: var(--color-interactive-hover) !important;
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
  .btnTitle {
    @apply w-full flex justify-center items-center gap-1 p-1 text-xl;
  }

  #welcome {
    color: var(--color-text-tertiary);
    @apply flex flex-col justify-center items-center gap-2 pb-[1vh];
  }

  span {
    @apply text-sm;
  }
</style>
