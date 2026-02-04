<script lang="ts">
	import Dropdown from "@root/components/Dropdown.svelte";
	import { processState, setTabCompleted } from "@util/stores";
	// let sharpingSettings = ["None", "Low", "Medium", "High"];
	let sharpingSettings = [
		{
			name: "None",
			value: "N",
		},
		{
			name: "Low",
			value: "L",
		},
		{
			name: "Medium",
			value: "M",
		},
		{
			name: "High",
			value: "H",
		},
	];

	let selected = sharpingSettings[0];

	$: if ($processState.currentTab === 4 && !$processState.completedTabs[4]) {
		setTabCompleted(4);
	}

	$: if (selected) {
		processState.update(state => ({
			...state,
			artStacks: state.artStacks.map((stack, i) =>
				i === 0
					? {
							...stack,
							sharpenString: selected.value,
					  }
					: stack
			),
		}));
	}
</script>

<main>
	<left>
		<h1>Advanced Options</h1>
	</left>
	<right>
		<div class="settings">
			<div class="sharpness">
				<p>Sharpening Level</p>
				<Dropdown values={sharpingSettings} bind:selected />
			</div>
		</div>
	</right>
</main>

<style lang="postcss">
	main {
		@apply flex justify-between h-full w-full overflow-hidden;
	}
	left {
		background-color: var(--color-surface-elevated);
		@apply w-full h-full p-6 flex-col;
	}
	right {
		background-color: var(--color-surface);
		@apply w-full h-full p-6 flex justify-center;
	}
	h1 {
		@apply text-3xl;
	}
	.settings {
		@apply w-[75%] flex justify-center items-start;
	}
	.settings .sharpness {
		background-color: var(--color-surface-elevated);
		@apply flex w-full max-w-lg justify-between rounded-xl p-2;
	}

	.sharpness p {
		@apply text-xl;
	}
</style>
