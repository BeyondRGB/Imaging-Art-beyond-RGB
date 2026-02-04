<script lang="ts">
	import { processState } from "@util/stores";
	import { ImageIcon, TrashIcon } from "svelte-feather-icons";
	import { createEventDispatcher } from "svelte";
	import Card from "@components/Card.svelte";
	import Button from "@components/Button.svelte";

	export let filename = "Empty...";
	export let minimal = false;

	let show = false;
	let btnRef;
	// let popRef;

	const dispatch = createEventDispatcher();

	const toggleDropdown = () => {
		if (show) {
			show = false;
		} else {
			show = true;
		}
	};
	const remove = () => {
		dispatch("remove");
	};
</script>

<main class="group">
	{#if minimal}
		<div on:mouseenter={toggleDropdown} on:mouseleave={toggleDropdown} bind:this={btnRef}>
			{filename.split("\\").length > 2 ? filename.split("\\").at(-1) : filename.split("/").at(-1)}
		</div>
	{:else}
		<Card
			variant="elevated"
			padding="sm"
			rounded={true}
			borderWidth="thin"
			className="bubble-card"
			on:mouseenter={toggleDropdown}
			on:mouseleave={toggleDropdown}
		>
			<div class="bubble-content" bind:this={btnRef}>
				<div class="file-info">
					<ImageIcon size="1.5x" />
					<span class="filename"
						>{filename.split("\\").length > 2
							? filename.split("\\").at(-1)
							: filename.split("/").at(-1)}</span
					>
				</div>
				<Button
					variant="danger"
					size="sm"
					onClick={remove}
					icon={TrashIcon}
					iconOnly={true}
					className="remove-btn"
				/>
			</div>
		</Card>
	{/if}

	<div class="body">
		<img src={$processState.imageThumbnails[filename]} alt={filename} />
	</div>
</main>

<style lang="postcss">
	main {
		@apply w-full h-full;
	}

	:global(.bubble-card) {
		@apply w-full;
	}

	.bubble-content {
		@apply flex justify-between items-center gap-3 w-full;
	}

	.file-info {
		@apply flex items-center gap-2 overflow-hidden;
	}

	.filename {
		color: var(--color-text-primary);
		@apply text-sm truncate;
	}

	:global(.remove-btn) {
		@apply flex-shrink-0;
	}

	.body {
		background-color: var(--color-overlay-heavy);
		@apply w-0 opacity-0 fixed max-w-[35vw] p-1
           group-hover:w-full group-hover:opacity-100 rounded-md transition-all 
           bottom-0 left-24 delay-100;
	}
	img {
		@apply w-full h-full;
	}
</style>
