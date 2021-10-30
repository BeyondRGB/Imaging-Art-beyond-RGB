<script>
	export let routes;
	import { currentPage } from "../stores";
</script>

<div class="navbar">
	{routes[$currentPage].text}
</div>

<style lang="postcss">
	.navbar {
		grid-area: Navbar;
		background-color: var(--nav-color);
		@apply font-extrabold text-xl text-center;
	}
</style>
