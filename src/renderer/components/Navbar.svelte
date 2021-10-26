<script>
	export let menuOptions;
	import { pageId } from '../stores';
</script>

<div class="navbar">
  {menuOptions[$pageId].text}
</div>

<style lang="postcss">
  .navbar {
		grid-area: Navbar;
		background-color: var(--nav-color);
		@apply font-extrabold text-xl text-center;
	}
</style>