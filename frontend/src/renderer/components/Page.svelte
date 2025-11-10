<script lang="ts">
	export let routes;
	export let pages;
	import { fade, fly } from "svelte/transition";
	import { cubicOut } from "svelte/easing";
	import { currentPage, appSettings, modal, serverError } from "@util/stores";
	import Modal from "@components/Modal.svelte";
	import RefDataModal from "@components/RefDataModal.svelte";
	import ServerError from "@components/ServerError.svelte";

	let showModal = false;
	$: if (
		$modal === "Settings" ||
		$modal === "Home" ||
		"CustomRefData" ||
		"CustomRefDataVer" ||
		"ServerError"
	) {
		showModal = true;
	} else {
		showModal = false;
	}

	$: if ($serverError != null) {
		console.log("Encountered Server Error");
		$modal = "ServerError";
	}
</script>

<div class="page dark:bg-gray-800 bg-white" bind:this={pages} class:sideMain={$appSettings.sideNav}>
	{#each Object.keys(routes) as pageKey}
		{#if routes[pageKey].page && !routes[pageKey].disabled}
			<div class="content">
				<svelte:component this={routes[pageKey].component} />
			</div>
		{/if}
	{/each}
</div>
{#if showModal}
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
		@apply w-full h-full pt-0 relative overflow-hidden flex;
	}
	.sideMain {
		@apply flex-col;
	}
	
	.content {
		@apply w-full h-full relative flex-shrink-0;
	}
</style>
