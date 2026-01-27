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
		@apply w-full h-full flex overflow-hidden;
	}
	.item {
		@apply flex-shrink-0 w-full h-full;
	}
	/* Hide inactive tabs - visibility:hidden prevents focus */
	.inactive {
		visibility: hidden;
		position: absolute;
		pointer-events: none;
	}
</style>
