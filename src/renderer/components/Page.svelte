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
		background-color: rgb(201, 200, 255);
		display: flex;
		justify-content: center;
		align-items: center;
		@apply w-[100%];
	}
</style>
