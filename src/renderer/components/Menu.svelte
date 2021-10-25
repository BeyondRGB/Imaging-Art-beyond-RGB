<script>
  export let menuOptions;
  export let icon;

  import { pageId } from '../stores';

  import Icon from 'svelte-awesome';

  import MenuItem from './MenuItem.svelte';

  console.log(menuOptions);
</script>

<main>
  <ul class="list">
    {#each menuOptions as item, i }
      {#if item.isShown}
        <button class="item" on:click={() => pageId.set(i)}>
          <div class="item-icon"><Icon data={item.icon} scale=1.5/></div>
          <span class="item-text">{item.text}</span>
        </button>
      {/if}
    {/each}
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
    transition: width 300ms ease;
		@apply w-[var(--menu-width)] flex flex-col text-black
					shadow-lg rounded-r-lg overflow-hidden;
	}

  main:hover {
    @apply w-[var(--menu-width-exp)] shadow-2xl rounded-r-3xl bg-green-300;
  }

  main:hover .item-text {
    display: inline;
  }

  .list {
    @apply flex flex-col list-none m-0 items-center h-[100vh];
  }

  .item-icon {
    margin: 0 1.75vw;
  }

  .item {
    @apply w-full flex items-center h-20 bg-gray-400;
  }

  .item:last-child {
    margin-top: auto;
  }

  .fa-primary,
  .fa-secondary {
    transition: var(--transition-speed);
  }

  .item:hover {
    filter: grayscale(0%) opacity(1);
    background: var(--bg-secondary);
    color: var(--text-secondary);
  }

  .item-text {
    @apply hidden ml-4 whitespace-nowrap;
  }

</style>