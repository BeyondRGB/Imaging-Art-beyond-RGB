<script>
  export let selectedPage;
</script>

<div class="page">
  <svelte:component this={selectedPage}/>
</div>

<style lang="postcss">
  .page {
		grid-area: Box;
		background-color: var(--box-color);
		display:flex;
		justify-content:center;
		align-items:center;
		@apply w-[100%];
	}
</style>