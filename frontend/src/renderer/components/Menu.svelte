<script lang="ts">
  export let routes: any;
  import TextLogo from "@assets/TextLogo.svg";
  import TextLogoAlt from "@assets/TextLogoAlt.svg";
  import { SvelteToast, toast } from '@zerodevx/svelte-toast';

  import {
    currentPage,
    modal,
    appSettings,
    connectionState,
    connect,
  } from "@util/stores";

  $: theme = $appSettings.theme ? "dark" : "";
  function handleClick(newPage: any[]) {
    if (newPage[0] === "Settings") {
      modal.set(newPage[1].component);
    } else {
      currentPage.set(newPage[0]);
    }
  }

  $: if ($connectionState === "Closed" || $connectionState === "Closed - Error"){
    toast.push('An error has occurred. Click the red icon to attempt to reconnect, or restart the program.')
  }
</script>

<main class:sideMain={$appSettings.sideNav} class={theme}>
  <ul>
    <div class="logoBox" class:altLogo={$appSettings.sideNav}>
      {#if $appSettings.sideNav}
        <img src={TextLogoAlt} alt="app-logo" class="sideImg" />
      {:else}
        <img src={TextLogo} alt="app-logo" class="mainImg" />
      {/if}
    </div>
    <div class="menuBtns">
      {#each Object.keys(routes).map((key) => [key, routes[key]]) as item, i}
        {#if item[1].isShown && !item[1].default}
          <button
            class:selected={$currentPage === item[0]}
            class:disabled={item[1].disabled}
            disabled={item[1].disabled}
            on:click={() => handleClick(item)}
          >
            <svelte:component
              this={item[1].icon}
              size="1.75x"
              class="menuNavIcon"
            />
            <span>{item[1].text}</span>
          </button>
        {/if}
      {/each}
    </div>
    <div class="ctlBtns">
      <button on:click={() => modal.set("Home")}>
        <svelte:component this={routes["Home"].icon} size="1.75x" />
      </button>

      <button on:click={() => modal.set("Settings")}>
        <svelte:component this={routes["Settings"].icon} size="1.75x" />
      </button>

      <button
        on:click={() => connect()}
        
        class:connected={$connectionState === "Connected"}
        class:disconnected={$connectionState !== "Connected"}
      />
    </div>
  </ul>
  <SvelteToast/>
</main>

<style lang="postcss">
  main {
    @apply w-full flex flex-col h-16 overflow-hidden border-t-[0.0625rem] border-gray-700/25;
  }

  .logoBox {
    @apply h-full mx-[1vw] flex justify-center items-center;
  }

  .altLogo {
    height: auto;
    @apply mt-[3vh] mx-0.5;
  }

  .mainImg {
    @apply h-[3.5vh] aspect-[44/7];
  }

  .sideMain {
    @apply bottom-auto w-24 h-full border-t-0 border-r-[0.0625rem];
  }

  ul {
    @apply flex list-none m-0 justify-between h-full w-full bg-gray-100 
          dark:bg-gray-700;
  }

  .sideMain ul {
    @apply flex-col;
  }

  span {
    @apply text-xs;
  }

  .selected span {
    @apply dark:text-gray-50;
  }

  button {
    @apply rounded-none h-full w-full flex flex-col justify-center items-center
            ring-0 bg-transparent dark:hover:bg-gray-800 hover:bg-gray-200 
            dark:text-gray-300 dark:hover:text-gray-50 shadow-none;
  }

  :root {
    --toastBarHeight: 0;
    --toastContainerTop: auto;
    --toastContainerRight: auto;
    --toastContainerBottom: 2vh;
    --toastContainerLeft: 10vw;
  }

  .selected {
    @apply dark:bg-gray-900 bg-gray-400 dark:hover:bg-gray-900/40 hover:bg-gray-600/40 
          dark:text-blue-500 border-b-4 border-blue-500;
  }

  .disabled {
    @apply dark:text-gray-500/50 cursor-not-allowed dark:hover:text-gray-500;
  }

  .menuBtns {
    @apply flex list-none w-[60%] whitespace-nowrap overflow-hidden 
            self-center justify-start items-stretch;
  }

  .sideMain .menuBtns {
    @apply flex-col h-[60%] w-full whitespace-normal overflow-visible;
  }
  
  .sideMain .menuBtns button {
    @apply h-auto min-h-[4rem];
  }

  .ctlBtns {
    @apply flex dark:bg-gray-600/30 bg-gray-200/30 rounded-l-2xl px-4 gap-2 items-center;
  }

  .ctlBtns button {
    @apply rounded-full hover:text-blue-500 dark:hover:text-blue-500 
            hover:bg-transparent dark:text-gray-50;
  }

  .sideMain .ctlBtns {
    @apply flex-col py-4 px-0 rounded-l-none rounded-t-2xl items-center justify-center;
  }

  .connected {
    @apply w-2 h-4 rounded-full bg-green-400 dark:hover:bg-green-500 dark:hover:scale-125;
  }
  .disconnected {
    @apply w-2 h-4 bg-red-500 dark:hover:bg-red-400 dark:hover:scale-125;
  }
</style>
