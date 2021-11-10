<script>
	export let selectedPage;
	export let pageName;
	import Modal from "svelte-simple-modal";
	import { fade } from "svelte/transition";
	import { getContext, onMount } from "svelte";
	import { modal } from "../stores.js";
	import Settings from "@pages/Settings.svelte";

	let prevPage;
	let prevName;
	// $: {
	// 	if (pageName === "Settings") {
	// 		openModal(Settings);
	// 		selectedPage = prevPage;
	// 	}

	// 	prevPage = selectedPage;
	// 	prevName = pageName;
	// }
	// const { open } = getContext("modal");

	// const openModal = (page) => {
	// 	open(page, {}, { closeButton: true });
	// };

	// const openModal = (page) => {
	// 	modal.set(page);
	// 	//open(ExportOptions, {}, { closeButton: false });
	// };

	// $: if (activeStep === 3) {
	//   openModal();
	// }
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
