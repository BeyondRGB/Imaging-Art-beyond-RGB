<script lang="ts">
  export let routes: any;
  import TextLogo from "@assets/TextLogo.svg";
  import TextLogoAlt from "@assets/TextLogoAlt.svg";
  import { SvelteToast, toast } from '@zerodevx/svelte-toast';
  import { RefreshCwIcon } from "svelte-feather-icons";

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

      {#if $connectionState === "Connected"}
        <div class="status-indicator connected"></div>
      {:else}
        <button
          on:click={() => connect()}
          class="reconnect-btn disconnected"
        >
          <RefreshCwIcon size="1.75x" />
        </button>
      {/if}
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
    background-color: var(--color-surface);
    @apply flex list-none m-0 justify-between h-full w-full;
  }

  .sideMain ul {
    @apply flex-col;
  }

  span {
    @apply text-xs;
  }

  .selected span {
    color: var(--color-text-primary);
  }

  button {
    background-color: transparent;
    color: var(--color-text-secondary);
    @apply rounded-none h-full w-full flex flex-col justify-center items-center
            ring-0 shadow-none;
  }
  
  button:hover {
    background-color: var(--color-interactive-hover);
    color: var(--color-text-primary);
  }

  :root {
    --toastBarHeight: 0;
    --toastContainerTop: auto;
    --toastContainerRight: auto;
    --toastContainerBottom: 2vh;
    --toastContainerLeft: 10vw;
  }

  .selected {
    background-color: var(--color-surface-sunken);
    @apply border-b-4 border-blue-500;
  }
  
  .selected:hover {
    background-color: var(--color-surface-sunken);
    opacity: 0.8;
  }

  .disabled {
    color: var(--color-text-disabled);
    @apply cursor-not-allowed;
  }

  .menuBtns {
    @apply flex list-none w-[60%] whitespace-nowrap overflow-hidden 
            self-center justify-start;
  }

  .sideMain .menuBtns {
    @apply flex-col h-[60%] w-full whitespace-normal overflow-visible items-stretch;
  }
  
  .sideMain .menuBtns button {
    @apply h-full flex-1;
  }


  .ctlBtns {
    background-color: var(--color-overlay-light);
    @apply flex rounded-l-2xl px-4 gap-2 items-center;
  }

  .ctlBtns button {
    color: var(--color-text-primary);
    @apply rounded-full hover:text-blue-500 hover:bg-transparent;
  }

  .sideMain .ctlBtns {
    @apply flex-col py-4 px-0 rounded-l-none rounded-t-2xl items-center justify-center;
  }

  .status-indicator {
    @apply w-7 h-7 rounded-full;
  }
  
  .status-indicator.connected {
    background-color: var(--color-success);
    @apply shadow-lg;
    box-shadow: 0 0 9px var(--color-success);
  }
  
  .reconnect-btn {
    @apply transition-all duration-200;
  }
  
  .reconnect-btn.disconnected {
    color: var(--color-error);
    @apply animate-pulse;
  }
  
  .reconnect-btn.disconnected:hover {
    @apply scale-110;
    color: var(--color-error);
  }
</style>
