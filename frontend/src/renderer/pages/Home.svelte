<script lang="ts">
  import TextLogo from "@assets/TextLogo.svg";
  import About from "@root/components/About.svelte";
  import { currentPage, modal } from "@util/stores";
  import { fade } from "svelte/transition";
  import {
    CrosshairIcon,
    ApertureIcon,
    InfoIcon,
    XCircleIcon,
    CopyIcon
  } from "svelte-feather-icons";
    

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
    <div class="aboutContent">
      <div class="aboutBg" on:click={() => (showAbout = false)} />
      <div class="aboutBox">
        <button class="closeBtn" on:click={() => (showAbout = false)}
          ><XCircleIcon size="1.5x" /></button
        >
        <About />
      </div>
    </div>
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
      <button on:click={() => handleClick("Process")} class="homeBtn">
        <div class="btnTitle">
          <ApertureIcon size="1.25x" />
          <h2>Process</h2>
        </div>

        <span> Process a new RAW image set </span>
      </button>
      <button on:click={() => handleClick("SpecPicker")} class="homeBtn">
        <div class="btnTitle">
          <CrosshairIcon size="1.25x" />
          <h2>View</h2>
        </div>
        <span> View a previously-processed imaged set </span>
      </button>
      <button on:click={() => openNewWindow()} class="homeBtn">
        <div class="btnTitle">
          <CopyIcon size="1.25x" />
          <h2>Create Another Window</h2>
        </div>
        <span> View two reports at once </span>
      </button>
      <button on:click={() => (showAbout = true)} class="homeBtn">
        <div class="btnTitle">
          <InfoIcon size="1.25x" />
          <h2>About</h2>
        </div>
        <span> About the program </span>
      </button>
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

  .homeBtn {
    background-color: var(--color-surface);
    color: var(--color-text-primary);
    @apply w-full h-full flex flex-col justify-center items-center p-[2vh] text-lg ring-0;
  }
  
  .homeBtn:hover {
    background-color: var(--color-interactive-hover);
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
    background-color: var(--color-overlay-heavy);
    @apply absolute w-full h-full flex justify-center items-center;
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
    color: var(--color-text-tertiary);
    @apply flex flex-col justify-center items-center gap-2 pb-[1vh];
  }

  #getting-started {
    @apply text-base flex flex-col justify-center items-center;
  }

  span {
    @apply text-sm;
  }
</style>
