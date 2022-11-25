<script lang="ts">
	export let routes;
	export let pages;
	import { fade } from "svelte/transition";
	import { currentPage, appSettings, modal, serverError } from "@util/stores";
	import Modal from "@components/Modal.svelte";
	import RefDataModal from "@components/RefDataModal.svelte";
	import ServerError from "@components/ServerError.svelte";

	let showModal = !!($modal === "Settings" ||
			$modal === "Home" ||
			"CustomRefData" ||
			"CustomRefDataVer" ||
			"ServerError");
	$: console.log($currentPage);

	$: if ($serverError != null) {
		console.log("Encountered Server Error");
		$modal = "ServerError";
	}
</script>

<div
	bind:this={pages}
	class="page dark:bg-gray-800 bg-white {$appSettings.sideNav ? 'sideNav' : ''}"
>
	{#each Object.keys(routes) as pageKey}
		{#if routes[pageKey].page && !routes[pageKey].disabled}
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
			component={routes.Settings.component}
			on:close={() => {
				showModal = false;
				$modal = null;
			}}
		/>
	{:else if $modal === "Home"}
		<Modal
			component={routes.Home.component}
			minimal
			on:close={() => {
				showModal = false;
				$modal = null;
			}}
		/>
	{:else if $modal === "CustomRefData"}
		<Modal
			component={RefDataModal}
			customExit
			on:close={() => {
				showModal = false;
				$modal = null;
			}}
		/>
	{:else if $modal === "CustomRefDataVer"}
		<Modal
			component={RefDataModal}
			customExit
			on:close={() => {
				showModal = false;
				$modal = null;
			}}
		/>
	{:else if $modal === "ServerError"}
		<Modal
			component={ServerError}
			customExit
			on:close={() => {
				showModal = false;
				$modal = null;
			}}
		/>
	{/if}
{/if}

<style lang="postcss" local>
	.page {
		scroll-snap-type: y mandatory;
		scroll-behavior: smooth;
		overflow: hidden;
		@apply w-full h-full pt-0 relative flex;
	}
	.sideNav {
		@apply flex-col;
	}
	::-webkit-scrollbar {
		@apply w-1;
	}
	.content {
		scroll-snap-align: start;
		@apply flex-shrink-0 w-full h-full relative;
	}
	.modal {
		@apply absolute w-full h-full bg-black/25 flex justify-center items-center;
	}
</style>
