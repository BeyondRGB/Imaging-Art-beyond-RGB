<script lang="ts">
	import {
		serverError,
		resendMessage,
		messageStore,
		processState,
		setTabCompleted,
	} from "@util/stores";
	import { slide } from "svelte/transition";
	import { AlertCircleIcon } from "svelte-feather-icons";
	import ScrollContainer from "@components/ScrollContainer.svelte";
	import Button from "@components/Button.svelte";

	export let closeModal;
	let showTrace: boolean = false;

	function handleClose() {
		console.log("Close");
		$serverError = null;
		$messageStore = [];
		closeModal();
	}

	/**
	 * When a server error occurs, go back to the most recent tab and
	 * reset fields in the processState
	 */
	function returnToSetup() {
		let targetTab = 0;
		for (let i = 0; i < $processState.completedTabs.length - 1; i++) {
			if (
				$processState.completedTabs[i + 1] === false ||
				i === $processState.completedTabs.length - 2
			) {
				targetTab = i;
				break;
			}
		}
		const finalTab = targetTab;
		processState.update(state => ({
			...state,
			currentTab: finalTab,
			whitePatchFilled: false,
			returnedFromProcessing: true,
			artStacks: state.artStacks.map((stack, i) =>
				i === 0
					? {
							...stack,
							verificationTarget: stack.verificationTarget === null ? {} : stack.verificationTarget,
					  }
					: stack
			),
		}));
		setTabCompleted(finalTab, false);
	}
</script>

<main>
	<h2 class="title">
		<AlertCircleIcon size="1x" />
		<p>Error</p>
	</h2>
	<ScrollContainer maxHeight="60vh">
		<div class="body">
			<div class="info">
				<span>Encountered Server Error</span>
				<div class="err">
					<div class="sender">
						Sender: {$serverError?.sender}
					</div>
					<div class="msg">
						<p>Message:</p>
						{$serverError?.message}
					</div>
					<!-- optional to show stack trace -->
					{#if showTrace}
						<div class="stack_trace" transition:slide>
							{$serverError?.trace}
						</div>
						<p class="trace_reveal" on:click={() => (showTrace = !showTrace)}>Show less...</p>
					{:else}
						<p class="trace_reveal" on:click={() => (showTrace = !showTrace)}>Show more...</p>
					{/if}
				</div>
			</div>
			<div class="btns">
				<Button
					variant="secondary"
					onClick={() => {
						resendMessage();
						handleClose();
					}}
				>
					Retry
				</Button>
				<Button
					variant="primary"
					onClick={() => {
						returnToSetup();
						handleClose();
					}}
				>
					Return to Setup
				</Button>
			</div>
		</div>
	</ScrollContainer>
</main>

<style lang="postcss">
	main {
		background-color: var(--color-surface);
		@apply w-full rounded-2xl shadow-xl flex flex-col min-w-[600px] overflow-hidden;
	}

	.body {
		@apply h-full flex flex-col justify-between p-4 gap-4;
	}
	.title {
		background-color: var(--color-surface-base);
		@apply text-2xl p-2 flex items-center gap-2 text-red-600;
	}
	.title p {
		color: var(--color-text-primary);
		@apply text-2xl flex;
	}
	.info {
		@apply flex flex-col p-2;
	}
	.info span {
		@apply text-lg p-1 flex flex-col;
	}
	.err {
		@apply p-2;
	}
	.sender {
		background-color: var(--color-surface-sunken);
		border: 1px solid var(--color-border);
		@apply p-2 rounded-md font-semibold text-sm;
	}
	.msg {
		background-color: var(--color-surface-elevated);
		border: 1px solid var(--color-border);
		@apply flex flex-col p-3 rounded-md mt-2;
	}
	.msg p {
		@apply font-semibold text-sm mb-1;
	}
	.trace_reveal {
		color: var(--color-text-secondary);
		text-decoration-line: underline;
		cursor: pointer;
		@apply text-sm mt-2 hover:opacity-80 transition-opacity;
	}
	.stack_trace {
		background-color: var(--color-surface-sunken);
		border: 1px solid var(--color-border);
		color: var(--color-text-primary);
		/* overflow control */
		overflow-x: auto;
		overflow-y: auto;
		max-height: 300px;
		/* wraps on newline only */
		white-space: pre;
		/* allows text to be highlighted */
		user-select: text !important;

		/* font stuff */
		font-family: "Fira Code", "Courier New", monospace;
		font-size: 0.75rem;
		line-height: 1.4;
		@apply p-3 rounded-md mt-2;
	}
	.btns {
		@apply flex justify-end gap-2 p-2;
	}
</style>
