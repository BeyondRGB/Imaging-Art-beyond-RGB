<script lang="ts">
	import {
		processState,
		batchProcessState,
		batchImagesA,
		batchImagesB,
		setTabCompleted,
	} from "@util/stores";
	import Dropbox from "@components/Process/Dropbox.svelte";
	import Button from "@components/Button.svelte";
	import { get, isEmpty, each, includes } from "lodash";
	import { autoSortBatchImages } from "@root/util/autoSortStandards";
	import { countFields } from "@root/util/storesUtil";
	import { modal } from "@util/stores";
	import Modal from "@components/Modal.svelte";
	import SortInfoModal from "@components/SortInfoModal.svelte";

	let imageStack = get($processState, "artStacks[0].fields");
	let artImageStackA = get($batchImagesA);
	let artImageStackB = get($batchImagesB, "artStacks[0].fields.imageB");
	let artImageCount = 1;

	let rerenderToggle = false;
	let validationError = null;

	// this helps force a rerender once the imageStack has been reset
	$: if ($processState.currentTab === 3) {
		imageStack = get($processState, "artStacks[0].fields");
	}
	// get art image count after images are updated but before we render this screen.
	$: if ($processState.currentTab === 2) {
		let totalImageCount =
			$processState.imageFilePaths.length + countFields($processState.artStacks[0].fields);
		if (totalImageCount >= 6 && totalImageCount <= 8) {
			artImageCount = 1;
		} else if (totalImageCount > 8) {
			artImageCount = Math.ceil((totalImageCount - 6) / 2);
		}
	}

	const getAllImages = function () {
		let allImages = [];
		each(imageStack, function (field) {
			if (!isEmpty(field) && !isEmpty(field[0])) {
				allImages.push(field[0]);
			}
		});
		each($processState.imageFilePaths, function (image) {
			if (!includes(allImages, image)) {
				allImages.push(image);
			}
		});
		return allImages;
	};

	const showTargetDropZones = function () {
		return getAllImages().length > 6;
	};

	const autoSort = function () {
		processState.update(state => ({
			...state,
			imageFilePaths: autoSortBatchImages(getAllImages(), imageStack),
		}));
		rerenderToggle = !rerenderToggle;
	};

	const validate = function () {
		console.log($batchProcessState);
		console.log(imageStack);
		if (
			(showTargetDropZones() && (isEmpty(imageStack?.targetA) || isEmpty(imageStack?.targetB))) ||
			isEmpty(imageStack?.imageA) ||
			isEmpty(imageStack?.imageB) ||
			isEmpty(imageStack?.flatfieldA) ||
			isEmpty(imageStack?.flatfieldB) ||
			isEmpty(imageStack?.darkfieldA) ||
			isEmpty(imageStack?.darkfieldB)
		) {
			return "Please ensure each dropzone is assigned an image.";
		} else {
			return null;
		}
		return validationError;
	};

	const submitSpecFileRoles = function (skipOptionalFiltering) {
		validate();
		if ((validationError = validate())) {
			return;
		}
		processState.update(state => ({
			...state,
			imageFilePaths: [],
			currentTab: skipOptionalFiltering ? state.currentTab + 2 : state.currentTab + 1,
		}));
		setTabCompleted(3);
	};
</script>

<main>
	{#if $modal === "SortInfoModal"}
		<Modal
			component={SortInfoModal}
			on:close={() => {
				$modal = null;
			}}
		/>
	{/if}
	{#key rerenderToggle}
		<panel>
			<div class="leftHeader">
				<h1>Specify Image Roles Batch</h1>
				<p>Drag and drop each image into its appropriate role</p>
				<div class="leftStartBox">
					<Dropbox bind:items={$processState.imageFilePaths} type="image" singleItem={false} />
				</div>
				<div class="btnGroup" style="margin: 50px 65px 2px 0;">
					<Button variant="secondary" size="md" onClick={autoSort}>Auto-sort images</Button>
				</div>
			</div>
			<div class="leftBoxes">
				<div class="centerFlexBox">
					<div id="imageStack">
						<div class="inputGroup">
							<div class="imageLetter">A</div>
							<div class="imageLetter">B</div>
						</div>
						<div class="objectDropBoxes">
							<div class="text">Object</div>
							{#each Array(artImageCount) as _, index (index)}
								<div class="inputGroup">
									<div class="cell">
										<Dropbox
											type="image"
											bind:items={imageStack.imageA[index]}
											singleItem={true}
											showError={!!validationError}
										/>
									</div>
									<div class="cell">
										<Dropbox
											type="image"
											bind:items={imageStack.imageB[index]}
											singleItem={true}
											showError={!!validationError}
										/>
									</div>
								</div>
							{/each}
						</div>
					</div>
				</div>
			</div>
		</panel>
		<right>
			<div class="rightContent">
				<div class="centerFlexBox">
					<div id="imageStack">
						<div class="inputGroup">
							<div class="imageLetter">A</div>
							<div class="imageLetter">B</div>
						</div>
						<div class="text">Target</div>
						<div class="inputGroup">
							<div class="cell">
								<Dropbox
									type="image"
									bind:items={imageStack.targetA}
									singleItem={true}
									showError={!!validationError}
								/>
							</div>
							<div class="cell">
								<Dropbox
									type="image"
									bind:items={imageStack.targetB}
									singleItem={true}
									showError={!!validationError}
								/>
							</div>
						</div>
						<div class="text">FlatField</div>
						<div class="inputGroup">
							<div class="cell">
								<Dropbox
									type="image"
									bind:items={imageStack.flatfieldA}
									singleItem={true}
									showError={!!validationError}
								/>
							</div>
							<div class="cell">
								<Dropbox
									type="image"
									bind:items={imageStack.flatfieldB}
									singleItem={true}
									showError={!!validationError}
								/>
							</div>
						</div>
						<div class="text">DarkField</div>
						<div class="inputGroup">
							<div class="cell">
								<Dropbox
									type="image"
									bind:items={imageStack.darkfieldA}
									singleItem={true}
									showError={!!validationError}
								/>
							</div>
							<div class="cell">
								<Dropbox
									type="image"
									bind:items={imageStack.darkfieldB}
									singleItem={true}
									showError={!!validationError}
								/>
							</div>
						</div>
						{#if validationError && imageStack && validate()}
							<div class="errorText">
								{validationError}
							</div>
						{/if}
					</div>
				</div>
			</div>
			<div class="btnGroup">
				<Button variant="secondary" size="md" onClick={() => submitSpecFileRoles(false)}
					>Optional filtering</Button
				>
				<Button variant="success" size="md" onClick={() => submitSpecFileRoles(true)}
					>Next: Skip optional filtering</Button
				>
			</div>
		</right>
	{/key}
</main>

<style lang="postcss">
	main {
		@apply flex h-full w-full overflow-hidden;
	}
	panel {
		@apply flex flex-col h-full;
		width: 50%;
		background-color: var(--color-surface-elevated);
		overflow: hidden;
	}
	right {
		background-color: var(--color-surface);
		width: 50%;
		@apply h-full p-6 flex flex-col;
	}
	.rightContent {
		flex: 1;
		min-height: 0;
		overflow-y: auto;
		display: flex;
		align-items: flex-start;
		justify-content: center;
		width: 100%;
	}
	h1 {
		color: var(--color-text-primary);
		margin: 20px 25px 10px 25px;
		font-size: 32px;
	}
	p {
		color: var(--color-text-secondary);
		margin: 0 25px 15px 25px;
		font-size: 16px;
	}
	#imageStack {
		background-color: var(--color-surface-base);
		border: 1px solid var(--color-border);
		border-radius: 20px;
		display: flex;
		flex-direction: column;
		padding: 20px 30px 25px 30px;
		width: 100%;
		align-self: stretch;
	}
	panel #imageStack {
		min-height: 220px;
	}
	right #imageStack {
		min-height: 360px;
	}
	.centerFlexBox {
		display: flex;
		flex-direction: column;
		align-items: stretch;
		width: 100%;
		max-width: 600px;
		margin: 0 auto;
	}
	.inputGroup {
		display: flex;
		flex-direction: row;
		gap: 20px;
		width: 100%;
		justify-content: center;
		padding-top: 10px;
	}
	.cell {
		flex: 1;
		min-width: 0;
	}
	.text {
		color: var(--color-text-secondary);
		text-align: center;
		margin-top: 15px;
		font-size: 14px;
	}
	.imageLetter {
		color: var(--color-text-primary);
		width: 50%;
		text-align: center;
		font-size: 28px;
	}
	.errorText {
		text-align: center;
		color: var(--color-error);
		margin-top: 10px;
	}
	.btnGroup {
		@apply flex justify-end gap-2;
		flex-shrink: 0;
		padding-top: 20px;
		width: 100%;
		max-width: 600px;
		align-self: center;
	}
	.leftHeader {
		flex-shrink: 0;
	}
	.leftBoxes {
		flex: 1;
		overflow-y: auto;
		padding: 10px 20px 20px 20px;
		display: flex;
		justify-content: center;
	}
	.leftStartBox {
		max-height: 300px;
		overflow-y: auto;
		margin: 0 25px;
	}
	.objectDropBoxes {
		padding-bottom: 10px;
	}
</style>
