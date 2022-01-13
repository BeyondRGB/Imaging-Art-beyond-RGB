<script lang="ts">
  export let routes: any;
  export let icon: any;
  import Modal from "@components/Modal.svelte";
  import Settings from "@root/pages/Settings.svelte";
  import TextLogo from "@assets/TextLogo.svg";
  let showModal = false;

  import { currentPage, modal, appSettings } from "@util/stores";
  import Icon from "svelte-awesome";

  $: theme = $appSettings.theme ? "dark" : "";
  function handleClick(newPage: any[]) {
    if (newPage[0] === "Settings") {
      modal.set(newPage[1].component);
    } else {
      currentPage.set(newPage[0]);
    }
  }
</script>

<main class="{$appSettings.sideNav ? 'sideMain' : ''} {theme}">
  <ul>
    <div class="logoBox">
      <!-- <img src={TextLogo} alt="app-logo" /> -->
      <p>Beyond RGB</p>
    </div>
    <div class="menuBtns">
      {#each Object.keys(routes).map((key) => [key, routes[key]]) as item, i}
        {#if item[1].isShown && !item[1].default}
          <button
            class={$currentPage === item[0] ? "selected" : ""}
            on:click={() => handleClick(item)}
          >
            <Icon data={item[1].icon} scale={1.5} />
            <span>{item[1].text}</span>
          </button>
        {/if}
      {/each}
    </div>
    <div class="ctlBtns">
      <button on:click={() => currentPage.set("Home")}>
        <Icon data={routes["Home"].icon} scale={1.75} />
      </button>

      <button on:click={() => (showModal = true)}>
        <Icon data={routes["Settings"].icon} scale={1.75} />
      </button>
    </div>
  </ul>
</main>

{#if showModal}
  <Modal on:close={() => (showModal = false)}>
    <!-- <h2 slot="header">
      modal
      <small><em>adjective</em> mod·al \ˈmō-dəl\</small>
    </h2> -->

    <h1 slot="header">App Settings</h1>
    <Settings />
  </Modal>
{/if}

<style lang="postcss">
  main {
    @apply w-full flex flex-col h-14 overflow-hidden;
  }

  .sideMain {
    @apply bottom-auto w-24 h-full;
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
            dark:text-gray-50/60 dark:hover:text-gray-50 shadow-none;
  }

  .selected {
    @apply dark:bg-gray-900 bg-gray-400 dark:hover:bg-gray-900/40 hover:bg-gray-600/40 
          dark:text-blue-500 border-b-4 border-blue-500;
  }

  .menuBtns {
    @apply flex h-full list-none w-[60%] whitespace-nowrap overflow-hidden 
            self-center;
  }

  .sideMain .menuBtns {
    @apply flex-col h-[60%] w-full whitespace-normal;
  }

  .ctlBtns {
    @apply flex dark:bg-gray-600/30 bg-gray-200/30 rounded-l-2xl px-4 gap-2;
  }

  .ctlBtns button {
    @apply rounded-full hover:text-blue-500 dark:hover:text-blue-500 
            hover:bg-transparent dark:text-gray-50;
  }

  .sideMain .ctlBtns {
    @apply flex-col py-4 px-0;
  }
  img {
    /* transform: scale(0.5); */
    pointer-events: none;
    filter: grayscale(0.5);
    @apply h-1/2 mt-3.5 pl-3;
  }
  p {
    @apply text-xl mt-2.5 pl-3;
  }
</style>
