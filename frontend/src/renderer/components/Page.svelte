<script lang="ts">
	export let selectedPage: any;
	import Modal from "svelte-simple-modal";
	import { fade } from "svelte/transition";
	import { modal } from "@util/stores";
	$: console.log($modal);
</script>

<Modal show={$modal}>
	{#key selectedPage}
		<div
			class="page dark:bg-gray-800 bg-white"
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
		@apply w-full h-full justify-center flex pt-1;
	}
</style>
