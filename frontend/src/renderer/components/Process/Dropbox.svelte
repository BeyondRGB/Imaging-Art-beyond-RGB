<script>
	import { dndzone } from "svelte-dnd-action";
	import { flip } from "svelte/animate";
	import ImageBubble from "@components/Process/ImageBubble.svelte";
	import { isEmpty } from "lodash";
	const flipDurationMs = 200;

	export let items = [];
	export let type;
	export let singleItem = true;
	export let showError = false;
	export let dragDisabled = false;
	export let fullWidth = false;
	export let colorIndex = -1;

	const catColors = [
		"#3b82f6",
		"#8b5cf6",
		"#06b6d4",
		"#f59e0b",
		"#10b981",
		"#ec4899",
		"#6366f1",
		"#14b8a6",
	];

	$: cardColor = colorIndex >= 0 ? catColors[colorIndex % catColors.length] : "";

	function handleSort(e) {
		items = e.detail.items;
	}
</script>

<main>
	<div
		class="sectionStyle {showError && singleItem && isEmpty(items) ? 'errorStyle' : ''} {fullWidth
			? 'fullWidth'
			: ''}"
	>
		<section
			use:dndzone={{
				items,
				flipDurationMs,
				type,
				dragDisabled,
				dropFromOthersDisabled: singleItem && items.length > 0,
			}}
			on:consider={handleSort}
			on:finalize={handleSort}
		>
			{#each items as item (item?.id)}
				<card
					animate:flip={{ duration: flipDurationMs }}
					class={singleItem ? "verified" : "pool-item"}
					style={cardColor ? "background-color: " + cardColor : ""}
				>
					<ImageBubble filename={item.name} minimal />
				</card>
			{/each}
			{#if items.length === 0 && singleItem}
				<span class="placeholder">Drop image here</span>
			{/if}
		</section>
	</div>
</main>

<style lang="postcss">
	section {
		min-height: 3.25em;
		margin: auto;
		border-radius: 10px;
		justify-content: center;
		display: flex;
		flex-wrap: wrap;
		gap: 10px;
		align-items: center;
		padding: 10px;
	}
	.sectionStyle {
		background-color: var(--color-surface-elevated);
		border: 1px solid var(--color-border);
		height: auto;
		width: 80%;
		border-radius: 10px;
		margin: auto;
	}
	.fullWidth {
		width: 100%;
	}
	.errorStyle {
		background-color: var(--color-error);
		border-color: var(--color-error);
	}
	.placeholder {
		color: var(--color-text-disabled);
		font-size: 12px;
		user-select: none;
		pointer-events: none;
	}
	card {
		color: var(--color-text-primary);
		width: auto;
		padding: 4px 12px;
		height: auto;
		min-height: 1.8em;
		border-radius: 8px;
		text-align: center;
		display: inline-flex;
		align-items: center;
		font-size: 13px;
		cursor: grab;
		transition: box-shadow 0.15s ease, transform 0.1s ease;
	}
	card:active {
		cursor: grabbing;
		transform: scale(1.02);
	}
	.pool-item {
		background-color: var(--color-surface-sunken);
		border: 1px solid var(--color-border);
	}
	:global(:root.dark) .pool-item {
		border-color: rgba(255, 255, 255, 0.12);
		background-color: rgba(255, 255, 255, 0.08);
	}
	.pool-item:hover {
		box-shadow: 0 1px 4px var(--color-overlay-light);
	}
	.verified {
		background-color: var(--color-success);
		color: white;
		border: 1px solid transparent;
		font-weight: 500;
		box-shadow: 0 1px 3px rgba(0, 0, 0, 0.15);
	}
	:global(:root.dark) .verified {
		box-shadow: 0 1px 4px rgba(0, 0, 0, 0.4);
	}
</style>
