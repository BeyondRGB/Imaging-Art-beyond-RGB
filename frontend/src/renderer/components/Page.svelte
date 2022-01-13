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
	<div
		class="page dark:bg-gray-800 bg-white {$appSettings.sideNav
			? 'sideNav'
			: ''}"
	>
		{#key selectedPage}
			<div in:fade={{ duration: 250, delay: 250 }} out:fade={{ duration: 250 }}>
				<svelte:component this={selectedPage} />
			</div>
		{/key}
	</div>
</Modal>

<style lang="postcss" local>
	.page {
		overflow: overlay;
		@apply w-full h-full pt-1;
	}
	::-webkit-scrollbar {
		@apply w-1;
	}
	div {
		@apply w-full h-full;
	}
	/* .sideNav {
		@apply pl-20 pb-0;
	} */
</style>
