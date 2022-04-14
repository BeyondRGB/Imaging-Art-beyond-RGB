<script lang="ts">
  import TextLogo from "@assets/TextLogo.svg";
  import { currentPage, modal } from "@util/stores";
  import { fade } from "svelte/transition";
  import { CrosshairIcon, ApertureIcon } from "svelte-feather-icons";

  function handleClick(page) {
    currentPage.set(page);
    modal.set(null);
  }
</script>

<main>
  <div class="content">
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
          <h2>Proccess</h2>
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
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply w-full h-[97%] bg-gray-800 mt-[3%] relative flex justify-center items-center;
  }

  .content {
    @apply h-[50vh] flex flex-col items-center justify-between mb-[15vh];
  }

  .homeBtn {
    @apply w-full h-full flex flex-col justify-center items-center p-[2vh] text-lg
          bg-gray-900/25 ring-0 hover:bg-gray-700 text-gray-100 hover:text-white;
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
