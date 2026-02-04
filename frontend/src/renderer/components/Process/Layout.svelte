<script>
	export let tabs;
	export let tabList;
	import { processState } from "@util/stores";
</script>

<!-- All tabs rendered for state management, but inactive ones are hidden -->
<!-- visibility:hidden prevents focus on elements inside -->
<div class="pages" bind:this={tabList}>
	{#each tabs as tab, i}
		<div 
			class="item" 
			class:inactive={i !== $processState.currentTab}
			data-tab-index={i}
		>
			<svelte:component this={tab.component} />
		</div>
	{/each}
</div>

<style lang="postcss">
	.pages {
		@apply w-full h-full overflow-hidden relative;
	}
	.item {
		@apply w-full h-full;
	}
	/* Hide inactive tabs - visibility:hidden prevents focus */
	/* position:absolute + z-index:-1 ensures they're always behind active tab */
	.inactive {
		visibility: hidden;
		position: absolute;
		top: 0;
		left: 0;
		z-index: -1;
		pointer-events: none;
		opacity: 0;
	}
</style>
