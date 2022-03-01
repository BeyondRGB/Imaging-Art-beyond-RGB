<script lang="ts">
	export let selectedPage: any;
	export let routes;
	export let pages;
	import { fade, fly } from "svelte/transition";
	import { currentPage, appSettings, modal } from "@util/stores";
	import Modal from "@components/Modal.svelte";

	let showModal = false;
	$: console.log($modal);
	$: if ($modal === "Settings" || $modal === "Home") {
		showModal = true;
	}
</script>

<div
	bind:this={pages}
	class="page dark:bg-gray-800 bg-white {$appSettings.sideNav ? 'sideNav' : ''}"
>
	{#each Object.keys(routes) as pageKey}
		{#if routes[pageKey].page}
			<div
				class="content"
				in:fade={{ duration: 250, delay: 250 }}
				out:fade={{ duration: 250 }}
			>
				<svelte:component this={routes[pageKey].component} />
			</div>
		{/if}
	{/each}
</div>
{#if showModal}
	<!-- <h2 slot="header">
      modal
      <small><em>adjective</em> mod·al \ˈmō-dəl\</small>
    </h2> -->
	{#if $modal === "Settings"}
		<Modal
			on:close={() => {
				showModal = false;
				$modal = null;
			}}
		>
			<!-- <h1 slot="header">App Settings</h1> -->
			<svelte:component this={routes.Settings.component} />
		</Modal>
	{:else if $modal === "Home"}
		<Modal
			minimal
			on:close={() => {
				showModal = false;
				$modal = null;
			}}
		>
			<svelte:component this={routes.Home.component} />
		</Modal>
	{/if}
{/if}

<style lang="postcss" local>
	.page {
		scroll-snap-type: y mandatory;
		scroll-behavior: smooth;
		overflow: hidden;
		@apply w-full h-full pt-1 relative flex;
	}
	.sideNav {
		@apply flex-col;
	}
	::-webkit-scrollbar {
		@apply w-1;
	}
	.content {
		scroll-snap-align: start;
		@apply flex-shrink-0 w-full h-full;
	}
	.modal {
		@apply absolute w-full h-full bg-black/25 flex justify-center items-center;
	}
</style>
