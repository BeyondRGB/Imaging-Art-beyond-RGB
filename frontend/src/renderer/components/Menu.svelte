<script>
  export let routes;
  export let icon;

  import { currentPage, modal } from "../stores";
  import Icon from "svelte-awesome";

  function handleClick(newPage) {
    if (newPage[0] === "Settings") {
      console.log("Settings to Settings", newPage[1]);
      modal.set(newPage[1].component);
    } else {
      currentPage.set(newPage[0]);
    }
  }
</script>

<main class="dark:bg-gray-900 bg-gray-300 dark:text-white text-gray-800">
  <ul class="list">
    <logo>
      <div class="logo-icon">
        <Icon data={icon} scale="2.5" />
      </div>
      <span class="logo-text">Beyond RGB</span>
    </logo>
    {#each Object.keys(routes).map((key) => [key, routes[key]]) as item, i}
      {#if item[1].isShown}
        <button
          class="item dark:text-gray-50 text-gray-800 dark:hover:text-gray-50"
          on:click={() => handleClick(item)}
          disabled={($currentPage !== "RGB") &
          ($currentPage !== "SpecOverlay") &
          ($currentPage !== "Reports") &
          ($currentPage !== "SpecPicker")
            ? !item[1].default
            : false}
        >
          <div class="item-icon">
            <Icon data={item[1].icon} scale="1.5" />
          </div>
          <span class="item-text">{item[1].text}</span>
        </button>
      {/if}
    {/each}
  </ul>
</main>

<style lang="postcss">
  :root {
    --menu-width: 6vw;
    --menu-width-exp: 30vw;
  }

  @media (min-width: 1200px) {
    :root {
      --menu-width: 5vw;
      --menu-width-exp: 20vw;
    }
  }

  main {
    grid-area: Navbar-start / Menu-start / Menu-end / Box-Start;
    transition-property: width, border-radius, box-shadow;
    transition-timing-function: cubic-bezier(0.4, 0, 0.2, 1);
    transition-duration: 400ms;
    transition-delay: 100ms;
    @apply w-[var(--menu-width)] flex flex-col h-full
					shadow-lg rounded-r-lg overflow-hidden z-10;
  }

  /* Logo */
  logo {
    @apply pb-12 pt-2 relative justify-center text-center w-full;
  }

  .logo-text {
    transition-property: left, position;
    @apply whitespace-nowrap select-none absolute -left-48 duration-200 text-2xl;
  }

  .logo-icon {
    @apply duration-200 mt-6 pb-2;
  }

  /* main */
  main:hover {
    @apply w-[var(--menu-width-exp)] shadow-2xl rounded-r-3xl;
  }

  .list {
    @apply flex flex-col list-none m-0 items-center h-[100vh];
  }

  main:hover .item-text {
    @apply opacity-100 transition-opacity ease-linear;
  }

  main:hover .logo-text {
    transition-property: left, position;
    @apply left-0 absolute duration-1000 w-full;
  }

  /* Menu Item */
  .item {
    @apply w-full flex items-center h-20 transition-colors duration-500 bg-transparent
            p-0 disabled:hover:bg-gray-400 disabled:text-gray-400 disabled:cursor-not-allowed
            disabled:hover:text-gray-50 active:scale-100 shadow-none;
  }

  .item:first-of-type {
    @apply border-t-[.0625rem];
  }

  .item:last-of-type {
    margin-top: auto;
    @apply border-b-0;
  }

  .item:hover {
    /* filter: grayscale(0%) opacity(1); */
    @apply transition-colors duration-500 bg-blue-400/75;
  }

  .item-text {
    @apply ml-4 whitespace-nowrap select-none opacity-0 duration-200;
  }

  .item-icon {
    @apply ml-[1.75vw] duration-200;
  }
</style>
