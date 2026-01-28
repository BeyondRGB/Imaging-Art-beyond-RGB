<script lang="ts">
	export let routes;
	import { fade, fly } from "svelte/transition";
	import { cubicOut } from "svelte/easing";
	import { currentPage, appSettings, modal, serverError } from "@util/stores";
	import Modal from "@components/Modal.svelte";
	import RefDataModal from "@components/RefDataModal.svelte";
	import OpenQualiaModal from "@components/OpenQualiaModal.svelte";
	import ServerError from "@components/ServerError.svelte";

	let showModal = false;
	$: if (
		$modal === "Settings" ||
		$modal === "Home" ||
		$modal === "CustomRefData" ||
		$modal === "CustomRefDataVer" ||
		$modal === "OpenQualia" ||
		$modal === "ServerError"
	) {
		showModal = true;
	} else {
		showModal = false;
	}
	$: console.log($currentPage);

	$: if ($serverError != null && $modal !== "ServerError") {
		console.log("Encountered Server Error");
		$modal = "ServerError";
	}
</script>

<div class="page">
	{#each Object.keys(routes) as pageKey}
		{#if routes[pageKey].page && !routes[pageKey].disabled && $currentPage === pageKey}
			<div class="content" transition:fade={{ duration: 200 }}>
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
			on:close={() => {
				showModal = false;
				$modal = null;
			}}
		/>
	{:else if $modal === "CustomRefDataVer"}
		<Modal
			component={RefDataModal}
			on:close={() => {
				showModal = false;
				$modal = null;
			}}
		/>
	{:else if $modal === "OpenQualia"}
		<Modal
			component={OpenQualiaModal}
			on:close={() => {
				showModal = false;
				$modal = null;
			}}
		/>
	{:else if $modal === "ServerError"}
		<Modal
			component={ServerError}
			size="large"
			on:close={() => {
				showModal = false;
				$modal = null;
				serverError.set(null);
			}}
		/>
	{/if}
{/if}

<style lang="postcss" local>
	.page {
		background-color: var(--color-surface-base);
		@apply w-full h-full pt-0 relative overflow-hidden;
	}

	.content {
		@apply w-full h-full relative;
	}
</style>
