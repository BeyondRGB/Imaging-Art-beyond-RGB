<script lang="ts">
	export let selectedPage: any;
	import Modal from "svelte-simple-modal";
	import { fade } from "svelte/transition";
	import { modal, appSettings } from "@util/stores";
	$: console.log($modal);

	// in:fade={{ duration: 350, delay: 350 }}
	// out:fade={{ duration: 350 }}
</script>

<Modal show={$modal}>
	{#key selectedPage}
		<div
			class="page dark:bg-gray-800 bg-white {$appSettings.sideNav
				? 'sideNav'
				: ''}"
		>
			<svelte:component this={selectedPage} />
		</div>
	{/key}
</Modal>

<style lang="postcss" local>
	.page {
		overflow: overlay;
		@apply w-full h-full pt-1;
	}
	/* .sideNav {
		@apply pl-20 pb-0;
	} */
</style>
