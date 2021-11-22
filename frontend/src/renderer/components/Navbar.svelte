<script>
	export let routes;
	import { currentPage } from "@util/stores";
	import { fade } from "svelte/transition";
</script>

{#if $currentPage !== "Home"}
	<main
		class="bg-gray-100 dark:bg-gray-800"
		in:fade={{ duration: 350, delay: 350 }}
		out:fade={{ duration: 350 }}
	>
		{#key $currentPage}
			<nav in:fade={{ duration: 350, delay: 350 }} out:fade={{ duration: 350 }}>
				<h1 class="text-gray-900 dark:text-white">
					{routes[$currentPage].text}
				</h1>
			</nav>
		{/key}
	</main>
{/if}

<style lang="postcss">
	main {
		grid-area: Navbar;
	}

	nav {
		@apply border-b-2 flex justify-center items-center h-full;
	}

	h1 {
		@apply font-semibold text-2xl opacity-75 p-2;
	}
</style>
