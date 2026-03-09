<script lang="ts">
	import {
		messageStore,
		processState,
		sendMessage,
		viewState,
		serverError,
		batchImagesA,
		batchImagesB,
		clearTabsAfter,
	} from "@util/stores";

	import ColorTarget from "@root/components/Process/Tabs/ColorTarget.svelte";
	import ImportImages from "@components/Process/Tabs/ImportImages.svelte";
	import SelectDest from "@components/Process/Tabs/SelectDest.svelte";
	import SpecFileRoles from "@components/Process/Tabs/SpecFileRoles.svelte";
	import AdvOpts from "@components/Process/Tabs/AdvOpts.svelte";
	import Processing from "@root/components/Process/Tabs/Processing.svelte";
	import BatchProcessingRoles from "@root/components/Process/Tabs/BatchProcessingRoles.svelte";
	import SelectProcessingType from "@root/components/Process/Tabs/SelectProcessingType.svelte";
	import Layout from "@components/Process/Layout.svelte";
	import Button from "@components/Button.svelte";
	import ConfirmDialog from "@components/ConfirmDialog.svelte";
	let tabList;

	let showWhitePatchWarning = false;
	let showDialog = false;
	let binaryType = null;
	let binaryName = null;
	let binaryFor = null;
	let binaryID = null;
	let batchCount = 0;

	$: tabs = [
		{ name: "Select Processing Type", component: SelectProcessingType },
		{ name: "Import Images", component: ImportImages },
		{ name: "Select Destination", component: SelectDest },
		{
			name:
				$processState.processType === "Batch" ? "Specify File Roles - Batch" : "Specify File Roles",
			component: $processState.processType === "Batch" ? BatchProcessingRoles : SpecFileRoles,
		},
		{ name: "Advanced Options", component: AdvOpts },
		{ name: "Color Target", component: ColorTarget },
		{ name: "Processing", component: Processing, hidden: true },
	];

	function nextTab() {
		if ($processState.currentTab !== tabs.length - 1) {
			if ($processState.completedTabs[$processState.currentTab]) {
				processState.update(state => ({
					...state,
					currentTab: state.currentTab + 1,
				}));
			}
		} else {
			console.log("Error overflow");
		}
	}

	$: if (
		$processState.pipelineComplete &&
		$processState.artStacks[0].fields.imageA.length >= 2 &&
		$processState.artStacks[0].fields.imageA[1].length !== 0
	) {
		processState.update(state => ({
			...state,
			completedTabs: [true, true, true, true, false, true],
			batch: true,
			currentTab: state.currentTab - 1,
			pipelineComplete: false,
			artStacks: state.artStacks.map((stack, i) =>
				i === 0
					? {
							...stack,
							fields: {
								...stack.fields,
								imageA: stack.fields.imageA.slice(1),
								imageB: stack.fields.imageB.slice(1),
							},
					  }
					: stack
			),
		}));
		batchCount += 1;
		handleConfirm();
	}

	$: if ($viewState.projectKey != null && processRequest?.RequestData) {
		processRequest.RequestData.outputDirectory = $viewState.projectKey;
	}

	function prevTab() {
		if ($processState.currentTab !== 0) {
			const newTab = $processState.currentTab - 1;
			processState.update(state => ({
				...state,
				currentTab: newTab,
			}));
			// Clear completed status for tabs after the one we're going back to
			clearTabsAfter(newTab);
		} else {
			console.log("Error overflow");
		}
	}

	// Handle Process-specific messages (Thumbnails, Output images, HalfSizedPreview, etc.)
	// Note: ColorManaged and ColorManagedTarget images are handled centrally in stores.ts
	$: if ($messageStore.length > 1 && !($messageStore[0] instanceof Blob)) {
		try {
			const message = $messageStore[0];
			if (typeof message === "string" && message.trim().length > 0) {
				let temp = JSON.parse(message);

				if (
					// Thumbnail Binary Handler
					temp["ResponseType"] === "ImageBinary" &&
					temp["RequestID"] === $processState.thumbnailID
				) {
					console.log("Thumbnail Binary From Server");
					binaryType = temp["ResponseData"]["type"];
					binaryName = temp["ResponseData"]["name"];
					binaryFor = "Thumbnail";
				} else if (
					// Output binary handler (not CMID or CMTID)
					temp["ResponseType"] === "ImageBinary" &&
					temp["RequestID"] !== $processState.CMID &&
					temp["RequestID"] !== $processState.CMTID
				) {
					console.log("Output Binary From Server");
					binaryType = temp["ResponseData"]["type"];
					binaryName = temp["ResponseData"]["name"];
					binaryFor = "Output";
				} else if (temp["RequestID"] === $processState.colorTargetID) {
					// Base64 Halfsize handler
					console.log("HalfSizedPreview From Server");
					processState.update(state => ({
						...state,
						colorTargetID: null,
						artStacks: state.artStacks.map((stack, i) =>
							i === 0
								? {
										...stack,
										colorTargetImage: temp["ResponseData"],
								  }
								: stack
						),
					}));
				} else if (
					// Base64 Thumbnail Handler
					temp["ResponseType"] === "ImageBase64" &&
					temp["RequestID"] === $processState.thumbnailID
				) {
					console.log("Thumbnail Base64 From Server");
					processState.update(state => ({
						...state,
						imageThumbnails: {
							...state.imageThumbnails,
							[temp["ResponseData"]["name"]]: temp["ResponseData"]["dataURL"],
						},
					}));
				} else if (temp["ResponseType"] === "ImageBase64") {
					// base64 output handler
					console.log("Base64 From Server");
					processState.update(state => ({
						...state,
						outputImage: temp["ResponseData"],
					}));
				} else if (temp["ResponseType"] === "Error") {
					// Error handler
					if (temp["ResponseData"]["critical"]) {
						serverError.set({
							sender: temp["ResponseData"]["sender"],
							message: temp["ResponseData"]["message"],
							trace: temp["ResponseData"]["trace"],
						});
						console.log({ SERVERERROR: temp["ResponseData"] });
					}
				}
			}
		} catch (e) {
			// Not JSON or parse error, ignore
		}
	}

	// Handle Process-specific blob messages (Thumbnails, Output images)
	// ColorManaged images are handled centrally in stores.ts
	$: if ($messageStore.length > 1 && $messageStore[0] instanceof Blob && binaryFor !== null) {
		console.log("Processing blob for:", binaryFor);
		let blob = $messageStore[0].slice(0, $messageStore[0].size, binaryType);
		let temp = new Image();
		temp.src = URL.createObjectURL(blob);

		if (binaryFor === "Thumbnail") {
			processState.update(state => ({
				...state,
				imageThumbnails: {
					...state.imageThumbnails,
					[binaryName]: temp.src,
				},
			}));
		} else if (binaryFor === "Output") {
			processState.update(state => ({
				...state,
				artStacks: state.artStacks.map((stack, i) =>
					i === 0
						? {
								...stack,
								colorTargetImage: {
									dataURL: temp.src,
									filename: binaryName,
								},
						  }
						: stack
				),
				outputImage: {
					dataURL: temp.src,
					name: binaryName,
				},
			}));
		}

		// Clear the pending binary request
		binaryType = null;
		binaryName = null;
		binaryID = null;
		binaryFor = null;
	}

	$: processRequest = {
		RequestType: "Process",
		RequestID: Date.now(),
		RequestData: {
			images: [
				{
					art: $processState.artStacks[0].fields.imageA[0]?.[0]?.name,
					white: $processState.artStacks[0].fields.flatfieldA[0]?.name,
					dark: $processState.artStacks[0].fields.darkfieldA[0]?.name,
					...($processState.artStacks[0].fields.targetA.length !== 0 &&
					$processState.artStacks[0].colorTarget?.refData !== undefined
						? { target: $processState.artStacks[0].fields.targetA[0]?.name }
						: {}),
				},
				{
					art: $processState.artStacks[0].fields.imageB[0]?.[0]?.name,
					white: $processState.artStacks[0].fields.flatfieldB[0]?.name,
					dark: $processState.artStacks[0].fields.darkfieldB[0]?.name,
					...($processState.artStacks[0].fields.targetB.length !== 0 &&
					$processState.artStacks[0].colorTarget?.refData !== undefined
						? { target: $processState.artStacks[0].fields.targetB[0]?.name }
						: {}),
				},
			],
			destinationDirectory: $processState.destDir,
			outputFileName: $processState.destFileName,
			outputDirectory: $viewState.projectKey || $processState.destDir,
			sharpenString: $processState.artStacks[0].sharpenString,
			batch: $processState.batch,
			targetLocation: $processState.artStacks[0].colorTarget
				? {
						...$processState.artStacks[0].colorTarget,
						refData: $processState.artStacks[0].colorTarget.refData
							? {
									...$processState.artStacks[0].colorTarget.refData,
									standardObserver: 1931,
									illuminants: "D50",
							  }
							: undefined,
				  }
				: undefined,
			...($processState.artStacks[0].verificationTarget != null &&
			Object.keys($processState.artStacks[0].verificationTarget).length > 0
				? {
						verificationLocation: {
							...$processState.artStacks[0].verificationTarget,
							refData: $processState.artStacks[0].verificationTarget.refData
								? {
										...$processState.artStacks[0].verificationTarget.refData,
										standardObserver: 1931,
										illuminants: "D50",
								  }
								: undefined,
						},
				  }
				: {}),
		},
	};

	function handleConfirm() {
		showDialog = false;

		if ($processState.currentTab !== tabs.length - 1) {
			processState.update(state => ({
				...state,
				currentTab: state.currentTab + 1,
			}));
			console.log("Sending Process Request");
			console.log(processRequest);
			setTimeout(() => {
				sendMessage(JSON.stringify(processRequest));
			}, 150);
		} else {
			console.log("Error overflow");
		}
	}
</script>

<main>
	{#if !tabs[$processState.currentTab].hidden}
		<nav class="dark:bg-gray-800/25">
			{#if $processState.currentTab !== 0}
				<Button id="backBtn" onClick={prevTab} variant="secondary" size="sm">Back</Button>
			{/if}

			<tabs>
				{#each tabs as tab, i}
					{#if !tab.hidden}
						<div
							class="tab"
							class:completed={$processState.completedTabs[i]}
							class:selected={tabs[$processState.currentTab].name === tab.name}
							id={tab.name}
						/>
					{/if}
				{/each}
			</tabs>
		</nav>
	{/if}
	<Layout {tabs} bind:tabList />
	<botnav class="dark:bg-transparent">
		<div class="nextBtn">
			{#if tabs[$processState.currentTab + 1]?.name === "Processing"}
				<Button
					variant="success"
					onClick={() => {
						if (!$processState.whitePatchFilled) {
							showWhitePatchWarning = true;
							return;
						}
						if ($processState.completedTabs[$processState.currentTab]) {
							console.log("Opening confirm dialog");
							showDialog = true;
							return;
						}
						showWhitePatchWarning = false;
						showDialog = false;
					}}>Begin Processing</Button
				>
			{:else if tabs[$processState.currentTab].hidden}
				<br />
			{:else if tabs[$processState.currentTab + 1]?.name !== "Advanced Options" && $processState.currentTab !== 0}
				<Button variant="success" onClick={nextTab}>Next</Button>
			{/if}
		</div>
	</botnav>

	<ConfirmDialog
		bind:show={showWhitePatchWarning}
		message="Please select a white patch before continuing"
		type="warning"
		confirmLabel="Close"
		cancelLabel=""
		onConfirm={() => (showWhitePatchWarning = false)}
	/>

	<ConfirmDialog
		bind:show={showDialog}
		message="Are you sure you're ready to proceed?"
		type="info"
		confirmLabel="Confirm"
		cancelLabel="Cancel"
		onConfirm={handleConfirm}
		onCancel={() => (showDialog = false)}
	/>
</main>

<style lang="postcss" local>
	main {
		@apply relative h-full w-full overflow-hidden flex flex-col min-h-0;
	}
	nav {
		background-color: var(--color-surface-sunken);
		@apply flex h-12;
	}
	botnav {
		@apply absolute h-10 bottom-2 flex items-center right-0;
	}
	tabs {
		@apply flex-grow justify-center flex mr-[5vw];
	}
	.nextBtn {
		@apply m-4;
	}
	.tab {
		background-color: var(--color-interactive);
		border: 1px solid var(--color-border);
		@apply w-16 h-2 rounded-full self-center mx-2 ring-1 
          transition-all duration-700 ease-out;
	}

	.selected {
		@apply bg-blue-400 ring-blue-400;
	}
	.completed {
		@apply bg-green-400 ring-green-400;
	}
	:global(#backBtn) {
		@apply absolute h-8 py-0 ml-8 my-2;
	}
</style>
