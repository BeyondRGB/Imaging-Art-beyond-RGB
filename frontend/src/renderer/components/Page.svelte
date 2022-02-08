<script lang="ts">
	export let selectedPage: any;
	import Modal from "svelte-simple-modal";
	import { fade } from "svelte/transition";
	import { modal, appSettings } from "@util/stores";
	import TestConsole from "@components/TestConsole.svelte";
	$: console.log($modal);

	// in:fade={{ duration: 350, delay: 350 }}
	// out:fade={{ duration: 350 }}
	let isOpen = false;
</script>

<Modal show={$modal}>
	<div
		class="page dark:bg-gray-800 bg-white {$appSettings.sideNav
			? 'sideNav'
			: ''}"
	>
		{#key selectedPage}
			<div
				class="content"
				in:fade={{ duration: 250, delay: 250 }}
				out:fade={{ duration: 250 }}
			>
				<svelte:component this={selectedPage} />
			</div>
		{/key}
		<div class={`console ${isOpen ? "open" : ""}`}>
			<div class="testBox">
				<div class="handle" on:click={() => (isOpen = !isOpen)}>
					{isOpen ? ">" : "<"}
				</div>
				<div class="con">
					<TestConsole />
				</div>
			</div>
		</div>
	</div>
</Modal>

<style lang="postcss" local>
	.page {
		overflow: hidden;
		@apply w-full h-full pt-1 relative flex justify-center items-center;
	}
	::-webkit-scrollbar {
		@apply w-1;
	}
	.content {
		@apply w-full h-full;
	}
	.console {
		@apply absolute -right-[60%] w-[60%] transition-all duration-300 z-50;
	}
	.open {
		@apply -right-0;
	}
	.handle {
		@apply bg-gray-800 h-12 w-8 absolute bottom-1/2 -left-8 flex justify-center items-center
							text-2xl rounded-l-full border-l-2 border-t-2 border-b-2 border-gray-700;
	}
	.testBox {
		@apply bg-gray-800 w-full h-full relative rounded-l-lg flex justify-center items-center
						border-2 border-gray-700 shadow-2xl;
	}
	.con {
		@apply w-full h-full;
	}
	/* .sideNav {
		@apply pl-20 pb-0;
	} */
</style>
