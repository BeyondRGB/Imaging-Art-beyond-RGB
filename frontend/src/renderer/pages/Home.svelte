<script lang="ts">
  import TextLogo from "@assets/TextLogo.svg";
  import { currentPage, modal } from "@util/stores";
  // import { angleDoubleDown } from "svelte-awesome/icons";
  import { fade, draw } from "svelte/transition";
  // import Icon from "svelte-awesome";
  import { ChevronsDownIcon } from "svelte-feather-icons";

  function handleClick(page) {
    currentPage.set(page);
    modal.set(null);
  }
</script>

<main>
  <div id="welcome">
    <h1 in:fade={{ duration: 1000, delay: 1000 }} class="dark:text-gray-400">
      Welcome to
    </h1>
    <img
      in:fade={{ duration: 1500, delay: 1750 }}
      src={TextLogo}
      alt="app-logo"
    />
    <div
      id="getting-started"
      in:fade={{ duration: 1000, delay: 3000 }}
      class="dark:text-gray-400"
    >
      <h3 class="dark:text-gray-400">Get started below</h3>
      <div class="downarrow">
        <ChevronsDownIcon size="2x" />
      </div>
    </div>
  </div>
  <button
    on:click={() => handleClick("Process")}
    class="dark:bg-gray-700/25 dark:text-white dark:hover:bg-gray-600/60 dark:hover:text-white dark:border-gray-600"
  >
    Learn More
    <span> Learn how to use Beyond RGB </span>
  </button>
  <button
    on:click={() => handleClick("Process")}
    class="dark:bg-gray-700/25 dark:text-white dark:hover:bg-gray-600/60 dark:hover:text-white dark:border-gray-600"
  >
    Proccess
    <span> Process a new Spectral Image </span>
  </button>
  <button
    on:click={() => handleClick("SpecOverlay")}
    class="dark:bg-gray-700/25 dark:text-white dark:hover:bg-gray-600/60 dark:hover:text-white dark:border-gray-600"
  >
    View
    <span> View already processed Image </span>
  </button>
</main>

<style lang="postcss">
  main {
    @apply w-full h-[97%] flex flex-col items-center justify-center gap-4
            bg-gray-800 mt-[3%] relative;
  }

  button {
    @apply bg-gray-100/25 w-[60%] h-[12%] rounded-md text-2xl flex flex-col 
            justify-center items-center border-0 hover:bg-gray-100/75 
            transition-all duration-500 hover:rounded-2xl text-gray-700 ring-0;
  }

  button:last-of-type {
    @apply mb-[20vh];
  }
  h1 {
    @apply text-4xl font-bold;
  }
  img {
    /* transform: scale(0.5); */
    pointer-events: none;
    @apply h-[10vh];
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

  .downarrow {
    animation: bounce 5s;
    animation-delay: 3s;
    animation-iteration-count: infinite;
    @apply mt-2;
  }

  @keyframes bounce {
    0%,
    25%,
    50%,
    75%,
    100% {
      transform: translateY(0rem);
    }
    40% {
      transform: translateY(0.75rem);
    }
    60% {
      transform: translateY(0.5rem);
    }
  }
</style>
