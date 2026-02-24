<script lang="ts">
	import {
		viewState,
		processState,
		connectionState,
		connect,
		currentPage,
		messageStore,
		resetProcess,
		modal,
	} from "@util/stores";
	import ImageViewer from "@components/ImageViewer.svelte";
	import Card from "@components/Card.svelte";
	import Button from "@components/Button.svelte";
	import Modal from "@components/Modal.svelte";
	import ProcessCompleteModal from "@components/ProcessCompleteModal.svelte";

	let notConnectedMode = false;

	let pipelineComponents = {};
	let pipelineProgress = {};

	let artImagesProcessed = 0;

	function reset() {
		pipelineComponents = {};
		pipelineProgress = {};
		resetProcess();
	}

	function resetPart() {
		pipelineComponents = {};
		pipelineProgress = {};
	}

	$: if ($messageStore.length > 1 && !($messageStore[0] instanceof Blob)) {
		try {
			let temp = JSON.parse($messageStore[0]);
			if (temp["ResponseType"] === "CalibrationComplete") {
				// Project Key handler
				console.log("CalibrationComplete Project Key From Server");
				viewState.update(state => ({
					...state,
					projectKey: temp["ResponseData"]["path"],
				}));
				processState.update(state => ({
					...state,
					pipelineComplete: true,
				}));
			} else if (temp["ResponseType"] === "Progress") {
				// Progress Update
				console.log("Progress From Server");
				pipelineProgress[temp["ResponseData"]["sender"]] = temp["ResponseData"]["value"];
			} else if (temp["ResponseType"] === "PipelineComponents") {
				// Pipeline Components
				console.log("Pipeline Components From Server");
				pipelineComponents = temp["ResponseData"]["component_json"];
			}
		} catch (e) {
			console.log(e);
		}
	}

	function handleComplete(id) {
		if (id === 0) {
			reset();
			console.log({ RESETTING: $currentPage });
			currentPage.set("SpecPicker");
		} else if (id === 1) {
			reset();
		}
	}

	// Show modal when processing is complete
	$: if ($processState.pipelineComplete) {
		artImagesProcessed += 1
		resetPart()

		console.log(`Art Images Processed: ${artImagesProcessed}`);
		if (artImagesProcessed == $processState.artImageCount) {
			modal.set("ProcessComplete");
		}
	}

	function closeCompletionModal() {
		modal.set(null);
		processState.update(state => ({
			...state,
			pipelineComplete: false,
		}));
	}

	// Open the file explorer using ipc after an image is finished processing.
	const openFileExplorer = async () => {
		// $viewState.projectKey is the filepath of the .btrgb file, but has some odd syntax with forward and backslashes, so it needs cleaning
		let defaultPath = $viewState.projectKey;

		// Match "BeyondRGB_" prefix, followed by 1+digits with ".btrgb" at the end. The 'i' at the end means case-insensitive
		let pattern = /BeyondRGB_\d+.btrgb$/i;

		// Cut off the BeyondRGB_0000.btrgb file portion to just get the directory
		let parentPath = defaultPath.replace(pattern, "");

		// Use ipc to open the file explorer in the parent directory of the .btrgb file.
		await window.electron.openFileExplorer({ directory: parentPath });
	};
</script>

<main>
	<!-- SVG gradient definition for progress circles -->
	<svg width="0" height="0" style="position: absolute;">
		<defs>
			<linearGradient id="progressGradient" x1="0%" y1="0%" x2="100%" y2="0%">
				<stop offset="0%" style="stop-color: rgb(224, 62, 62);" />
				<stop offset="50%" style="stop-color: rgb(68, 217, 83);" />
				<stop offset="100%" style="stop-color: rgb(66, 86, 215);" />
			</linearGradient>
		</defs>
	</svg>

	{#if $modal === "ProcessComplete"}
		<Modal
			component={ProcessCompleteModal}
			on:close={closeCompletionModal}
			size="medium"
			onViewImage={() => handleComplete(0)}
			onOpenFileLocation={openFileExplorer}
			onProcessAnother={() => handleComplete(1)}
		/>
	{/if}
	<div class="top">
		<div class="right">
			<div class="image">
				<h4>{$processState.outputImage?.name}</h4>
				<ImageViewer
					srcUrl={$processState.outputImage?.dataURL}
					identifier={$processState.outputImage?.name}
				/>
			</div>
		</div>
	</div>
	<div class="bottom">
		<div class="stepBox">
			{#if Object.keys(pipelineComponents).length < 1}
				<Card variant="elevated" padding="lg" rounded={true} shadow="medium" className="waitingBox">
					<div class="waitingMsg">Waiting for processing to begin...</div>
				</Card>
			{:else}
				{#each Object.keys(pipelineComponents) as component1, i1}
					{#if component1 !== "name"}
						{#each Object.keys(pipelineComponents[component1]) as component2, i2}
							<Card
								variant="dark"
								padding="sm"
								rounded={true}
								borderWidth="thin"
								className="stepGroup"
							>
								<div class="stepTitle">
									{pipelineComponents[component1][component2]["name"]}
								</div>
								<div class="steps">
									{#if component2 !== "name"}
										{#each Object.keys(pipelineComponents[component1][component2]) as component3, i3}
											{#if component3 !== "name"}
												{#each Object.keys(pipelineComponents[component1][component2][component3]) as component4, i4}
													{@const progressValue =
														pipelineComponents[component1][component2][component3][component4][
															"name"
														] in pipelineProgress
															? pipelineProgress[
																	pipelineComponents[component1][component2][component3][
																		component4
																	]["name"]
															  ]
															: 0}
													<div class="progress-circle" class:completed={progressValue === 1}>
														<svg class="progress-ring" viewBox="0 0 100 100">
															<circle class="progress-ring-bg" cx="50" cy="50" r="45" />
															<circle
																class="progress-ring-fill"
																cx="50"
																cy="50"
																r="45"
																style="stroke-dashoffset: {282.7 * (1 - progressValue)}"
															/>
														</svg>
														<div class="stepper">
															<span class="sender"
																>{pipelineComponents[component1][component2][component3][
																	component4
																]["name"]}</span
															>
														</div>
													</div>
												{/each}
											{/if}
										{/each}
									{/if}
									{#if pipelineComponents[component1][component2]["name"].includes("Results") || pipelineComponents[component1][component2]["name"].includes("Verifi")}
										{@const progressValue2 =
											pipelineComponents[component1][component2]["name"] in pipelineProgress
												? pipelineProgress[pipelineComponents[component1][component2]["name"]]
												: 0}
										<div class="progress-circle" class:completed={progressValue2 === 1}>
											<svg class="progress-ring" viewBox="0 0 100 100">
												<circle class="progress-ring-bg" cx="50" cy="50" r="45" />
												<circle
													class="progress-ring-fill"
													cx="50"
													cy="50"
													r="45"
													style="stroke-dashoffset: {282.7 * (1 - progressValue2)}"
												/>
											</svg>
											<div class="stepper">
												<span class="sender"
													>{pipelineComponents[component1][component2]["name"]}</span
												>
											</div>
										</div>
									{/if}
								</div>
							</Card>
						{/each}
					{/if}
				{/each}
			{/if}
		</div>
	</div>
	{#if $connectionState !== "Connected" && !notConnectedMode}
		<div class="notConnected">
			<Card
				variant="elevated"
				padding="lg"
				rounded={true}
				shadow="medium"
				className="notConnectedCard"
			>
				<div class="notConnectedMsg">WARNING YOU ARE NOT CONNECTED TO THE BACKEND</div>
				<div class="notConnectedBtns">
					<Button variant="primary" size="md" onClick={() => connect()}>Reconnect</Button>
					<Button variant="secondary" size="md" onClick={() => (notConnectedMode = true)}
						>Continue Anyway</Button
					>
				</div>
			</Card>
		</div>
	{/if}
</main>

<style lang="postcss">
	.notConnected {
		@apply w-full h-full absolute bg-black/75 z-50 flex justify-center items-center;
	}
	:global(.notConnectedCard) {
		@apply flex flex-col gap-4 items-center;
	}
	.notConnectedMsg {
		color: var(--color-text-primary);
		@apply text-xl font-semibold text-center;
	}
	.notConnectedBtns {
		@apply flex gap-4;
	}
	main {
		@apply w-full h-full flex flex-col relative p-2 gap-2;
	}
	.bottom {
		@apply w-full h-auto flex flex-col justify-center items-center relative gap-2;
	}
	.stepBox {
		@apply w-[90%] flex justify-between items-center relative gap-2;
	}
	.stepper {
		margin-left: 5%;
		margin-top: 5%;
		overflow-wrap: break-word;
		background-color: var(--color-surface-elevated);
		@apply w-[90%] h-[90%] rounded-full flex flex-col justify-center items-center
            absolute top-0 left-0;
	}
	.stepTitle {
		color: var(--color-text-primary);
		@apply font-medium;
	}
	:global(.stepGroup) {
		@apply gap-1 flex flex-col justify-items-center items-center
            text-lg p-4;
	}
	.steps {
		@apply flex w-full justify-center gap-2;
	}
	.sender {
		word-break: break-word;
		white-space: pre-line;
		background-color: var(--color-surface-elevated);
		@apply w-[90%] text-base flex rounded-full justify-center items-center text-center;
	}
	.progress-circle {
		@apply h-[8vw] w-[8vw] rounded-full p-1 relative;
	}

	.progress-circle.completed .progress-ring-fill {
		stroke: rgb(34, 197, 94); /* green-500 */
	}

	.progress-ring {
		@apply w-full h-full absolute top-0 left-0;
		transform: rotate(-90deg); /* Start from top */
	}

	.progress-ring-bg {
		fill: none;
		stroke: rgb(125, 123, 123);
		stroke-width: 10;
	}

	.progress-ring-fill {
		fill: none;
		stroke: url(#progressGradient);
		stroke-width: 10;
		stroke-linecap: round;
		stroke-dasharray: 282.7; /* 2 * PI * 45 (radius) */
		stroke-dashoffset: 282.7;
		transition: stroke-dashoffset 0.5s ease-out;
	}

	:global(.waitingBox) {
		@apply w-1/2 flex flex-col;
	}

	.waitingMsg {
		color: var(--color-text-primary);
		@apply text-xl text-center;
	}

	.top {
		@apply w-full h-full flex justify-center items-center overflow-hidden;
	}
	.right {
		@apply w-full h-full flex justify-center items-center ml-2;
	}

	.image {
		background-color: var(--color-overlay-medium);
		@apply w-full h-full relative items-center flex justify-center;
	}
	h4 {
		background-color: var(--color-overlay-heavy);
		color: var(--color-text-secondary);
		@apply absolute top-0 pb-1 pt-0 px-2 rounded-lg z-50;
	}
	span {
		background-color: var(--color-surface);
		color: var(--color-text-secondary);
		@apply font-semibold whitespace-nowrap select-text;
	}
	.cont {
		@apply bg-red-500;
	}
</style>
