<script>
  export let routes;
  export let icon;

  import { currentPage } from "../stores";

  import Icon from "svelte-awesome";

  import MenuItem from "./MenuItem.svelte";
</script>

<main>
  <ul class="list">
    <logo>
      <div class="logo-icon">
        <Icon data={icon} scale="2" />
      </div>
      <span class="logo-text">Beyond RGB</span>
    </logo>
    {#if routes[$currentPage].isShown & ($currentPage !== "Home") & ($currentPage !== "Settings")}
      {#each Object.keys(routes).map((key) => [key, routes[key]]) as item, i}
        {#if item[1].isShown}
          <button class="item" on:click={() => currentPage.set(item[0])}>
            <div class="item-icon">
              <Icon data={item[1].icon} scale="1.5" />
            </div>
            <span class="item-text">{item[1].text}</span>
          </button>
        {/if}
      {/each}
    {:else}
      {#each Object.keys(routes).map((key) => [key, routes[key]]) as item, i}
        {#if item[1].default}
          <button class="item" on:click={() => currentPage.set(item[0])}>
            <div class="item-icon">
              <Icon data={item[1].icon} scale="1.5" />
            </div>
            <span class="item-text">{item[1].text}</span>
          </button>
        {/if}
      {/each}
    {/if}
  </ul>
</main>

<style lang="postcss">
  :root {
    --text-secondary: rgb(255, 255, 255);
    --bg-secondary: rgb(45, 87, 78);
    --transition-speed: 600ms;
  }

  main {
    grid-area: Navbar-start / Menu-start / Menu-end / Box-Start;
    background-color: var(--menu-color);
    height: 100%;
    z-index: 1;
    transition-property: all;
    transition-timing-function: cubic-bezier(0.4, 0, 0.2, 1);
    transition-duration: 400ms;
    transition-delay: 100ms;
    /* transition: width 400ms ease; */
    @apply w-[var(--menu-width)] flex flex-col text-black
					shadow-lg rounded-r-lg overflow-hidden;
  }

  logo {
    @apply pb-12 pt-2;
  }

  main:hover {
    @apply w-[var(--menu-width-exp)] shadow-2xl rounded-r-3xl bg-green-300;
  }

  .list {
    @apply flex flex-col list-none m-0 items-center h-[100vh];
  }

  .item:last-child {
    margin-top: auto;
  }

  main:hover .item-text {
    @apply opacity-100;
  }

  main:hover .logo-text {
    @apply left-4 transition-all duration-700 ease-in-out;
  }

  .item:hover {
    filter: grayscale(0%) opacity(1);
    background: var(--bg-secondary);
    color: var(--text-secondary);
    @apply transition-all ease-in duration-200;
  }

  .item {
    @apply w-full flex items-center h-20 bg-gray-400;
  }

  .item-text {
    @apply ml-4 whitespace-nowrap select-none opacity-0 duration-200;
  }

  .logo-text {
    @apply whitespace-nowrap select-none absolute -left-64 duration-200 text-2xl;
  }

  .item-icon {
    @apply ml-[2vw] duration-200;
  }

  .logo-icon {
    @apply duration-200 mt-6 pb-2;
  }
</style>
