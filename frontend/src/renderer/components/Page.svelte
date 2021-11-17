<script>
	export let selectedPage;
	import Modal from "svelte-simple-modal";
	import { fade } from "svelte/transition";
	import { modal } from "../stores.js";
	$: console.log($modal);
</script>

<Modal show={$modal}>
	{#key selectedPage}
		<div
			class="page dark:bg-gray-700 bg-gray-50"
			in:fade={{ duration: 350, delay: 350 }}
			out:fade={{ duration: 350 }}
		>
			<svelte:component this={selectedPage} />
		</div>
	{/key}
</Modal>

<style lang="postcss" local>
	.page {
		grid-area: Box;
		overflow: overlay;
		@apply w-full h-full;
	}
</style>
