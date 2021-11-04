<script>
	export let selectedPage;
	import Modal from "svelte-simple-modal";
</script>

<Modal>
	<div class="page">
		<svelte:component this={selectedPage} />
	</div>
</Modal>

<style lang="postcss">
	.page {
		grid-area: Box;
		@apply w-full h-full;
	}
</style>
