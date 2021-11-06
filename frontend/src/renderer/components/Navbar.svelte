<script>
	export let routes;
	import { currentPage } from "../stores";
	import PageTransitions from "@components/PageTransitions.svelte";
</script>

{#if $currentPage !== "Home"}
	<main>
		<PageTransitions>
			<nav>
				<h1><PageTransitions>{routes[$currentPage].text}</PageTransitions></h1>
			</nav>
		</PageTransitions>
	</main>
{/if}

<style lang="postcss">
	main {
		grid-area: Navbar;
	}

	nav {
		background-color: var(--nav-color);
		@apply border-b-2 flex justify-center items-center;
	}

	h1 {
		@apply font-semibold text-2xl opacity-75 p-2;
	}
</style>
