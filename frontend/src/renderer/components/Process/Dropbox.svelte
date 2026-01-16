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

	function handleSort(e) {
		items = e.detail.items;
	}
</script>

<main>
	<div class="sectionStyle {showError && singleItem && isEmpty(items) ? 'errorStyle' : ''}">
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
				<card animate:flip={{ duration: flipDurationMs }} class={singleItem ? "verified" : ""}>
					<ImageBubble filename={item.name} minimal />
				</card>
			{/each}
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
	.errorStyle {
		background-color: var(--color-error);
		border-color: var(--color-error);
	}
	card {
		background-color: var(--color-surface-sunken);
		color: var(--color-text-primary);
		width: auto;
		padding-right: 10px;
		padding-left: 10px;
		height: 1.8em;
		border-radius: 10px;
		text-align: center;
	}
	.verified {
		background-color: var(--color-success);
		color: white;
	}
</style>
