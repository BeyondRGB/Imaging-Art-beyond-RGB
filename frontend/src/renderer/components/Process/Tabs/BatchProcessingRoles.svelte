<script lang="ts">
	import { processState, batchProcessState, setTabCompleted } from "@util/stores";
	import Dropbox from "@components/Process/Dropbox.svelte";
	import Button from "@components/Button.svelte";
	import { get, isEmpty, each, includes } from "lodash";
	import { autoSortBatchImages } from "@root/util/autoSortStandards";
	import { countFields } from "@root/util/storesUtil";
	import { modal } from "@util/stores";
	import Modal from "@components/Modal.svelte";
	import SortInfoModal from "@components/SortInfoModal.svelte";

	let imageStack = get($processState, "artStacks[0].fields");
	let artImageCount = 1;

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

	const autoSort = function () {
		processState.update(state => ({
			...state,
			imageFilePaths: autoSortBatchImages(getAllImages(), imageStack),
		}));
		rerenderToggle = !rerenderToggle;
	};

	const validate = function () {
		// Check calibration images
		if (
			isEmpty(imageStack?.targetA) ||
			isEmpty(imageStack?.targetB) ||
			isEmpty(imageStack?.flatfieldA) ||
			isEmpty(imageStack?.flatfieldB) ||
			isEmpty(imageStack?.darkfieldA) ||
			isEmpty(imageStack?.darkfieldB)
		) {
			return "Please ensure each calibration dropzone is assigned an image.";
		}
		// Check every object pair — each must have both A and B
		for (let i = 0; i < artImageCount; i++) {
			if (isEmpty(imageStack?.imageA?.[i]) || isEmpty(imageStack?.imageB?.[i])) {
				return (
					"Object " +
					(artImageCount > 1 ? i + 1 + " is" : "is") +
					" missing an image. Each object needs both A and B."
				);
			}
		}
		return null;
	};

	const submitSpecFileRoles = function (skipOptionalFiltering) {
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
		<!-- Left Panel: Unassigned Pool (top) + Calibration (bottom) -->
		<left>
			<!-- Unassigned Image Pool -->
			<div class="pool-zone">
				<div class="pool-header">
					<div class="pool-title">
						<span class="group-label" style="margin-bottom: 0;">Unassigned Images</span>
						<span
							class="unassigned-badge"
							class:empty-pool={$processState.imageFilePaths.length === 0}
						>
							{#if $processState.imageFilePaths.length > 0}
								<span class="badge-dot" />
								{$processState.imageFilePaths.length}
							{:else}
								<svg class="check-icon" viewBox="0 0 16 16" fill="none">
									<path
										d="M3 8.5L6.5 12L13 4"
										stroke="currentColor"
										stroke-width="2"
										stroke-linecap="round"
										stroke-linejoin="round"
									/>
								</svg>
							{/if}
						</span>
					</div>
					<Button variant="secondary" size="sm" onClick={autoSort}>Auto-sort</Button>
				</div>
				<div class="pool-dropbox">
					<Dropbox
						bind:items={$processState.imageFilePaths}
						type="image"
						singleItem={false}
						fullWidth={true}
					/>
				</div>
			</div>

			<!-- Calibration Images (bottom of left panel) -->
			<div class="calibration-zone">
				<div class="group-label">Calibration Images</div>
				<div class="column-headers">
					<div class="role-label-spacer" />
					<div class="col-header">A</div>
					<div class="col-header">B</div>
				</div>

				<div class="role-lane lane-target">
					<div class="role-label">Target</div>
					<div class="drop-cell">
						<Dropbox
							type="image"
							bind:items={imageStack.targetA}
							singleItem={true}
							showError={!!validationError}
							fullWidth={true}
						/>
					</div>
					<div class="drop-cell">
						<Dropbox
							type="image"
							bind:items={imageStack.targetB}
							singleItem={true}
							showError={!!validationError}
							fullWidth={true}
						/>
					</div>
				</div>

				<div class="role-lane lane-flatfield">
					<div class="role-label">Flat Field</div>
					<div class="drop-cell">
						<Dropbox
							type="image"
							bind:items={imageStack.flatfieldA}
							singleItem={true}
							showError={!!validationError}
							fullWidth={true}
						/>
					</div>
					<div class="drop-cell">
						<Dropbox
							type="image"
							bind:items={imageStack.flatfieldB}
							singleItem={true}
							showError={!!validationError}
							fullWidth={true}
						/>
					</div>
				</div>

				<div class="role-lane lane-darkfield">
					<div class="role-label">Dark Field</div>
					<div class="drop-cell">
						<Dropbox
							type="image"
							bind:items={imageStack.darkfieldA}
							singleItem={true}
							showError={!!validationError}
							fullWidth={true}
						/>
					</div>
					<div class="drop-cell">
						<Dropbox
							type="image"
							bind:items={imageStack.darkfieldB}
							singleItem={true}
							showError={!!validationError}
							fullWidth={true}
						/>
					</div>
				</div>
			</div>
		</left>

		<!-- Right Panel: Header + Object Images + Actions -->
		<right>
			<div class="right-header">
				<h2>Assign Image Roles</h2>
			</div>

			<!-- Object Images (scrollable) -->
			<div class="object-zone">
				<div class="group-label">Object Images</div>
				<div class="column-headers">
					<div class="role-label-spacer" />
					<div class="col-header">A</div>
					<div class="col-header">B</div>
				</div>
				{#each Array(artImageCount) as _, index (index)}
					<div class="role-lane lane-object" style="border-left-color: {catColors[index % 8]}">
						<div class="role-label">
							{#if artImageCount > 1}Object {index + 1}{:else}Object{/if}
						</div>
						<div class="drop-cell">
							<Dropbox
								type="image"
								bind:items={imageStack.imageA[index]}
								singleItem={true}
								showError={!!validationError}
								fullWidth={true}
								colorIndex={index % 8}
							/>
						</div>
						<div class="drop-cell">
							<Dropbox
								type="image"
								bind:items={imageStack.imageB[index]}
								singleItem={true}
								showError={!!validationError}
								fullWidth={true}
								colorIndex={index % 8}
							/>
						</div>
					</div>
				{/each}
			</div>

			<!-- Action Bar -->
			<div class="action-bar">
				{#if validationError}
					<div class="error-msg">
						<svg class="error-icon" viewBox="0 0 16 16" fill="none">
							<circle cx="8" cy="8" r="7" stroke="currentColor" stroke-width="1.5" />
							<path d="M8 4.5V9" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" />
							<circle cx="8" cy="11.5" r="0.75" fill="currentColor" />
						</svg>
						{validationError}
					</div>
				{/if}
				<div class="action-buttons">
					<Button variant="secondary" size="md" onClick={() => submitSpecFileRoles(false)}>
						Optional filtering
					</Button>
					<Button variant="success" size="md" onClick={() => submitSpecFileRoles(true)}>
						Next: Skip optional filtering
					</Button>
				</div>
			</div>
		</right>
	{/key}
</main>

<style lang="postcss">
	/* ── Main Split Container ── */
	main {
		@apply flex h-full w-full overflow-hidden;
	}

	/* ── Left Panel: Calibration + Pool ── */
	left {
		@apply flex flex-col h-full;
		width: 50%;
		background-color: var(--color-surface-elevated);
	}

	/* ── Unassigned Pool (top of left panel, fills remaining space) ── */
	.pool-zone {
		@apply flex-1 flex flex-col;
		min-height: 0;
		padding: 16px 20px;
	}

	/* ── Calibration Zone (bottom of left panel, compact) ── */
	.calibration-zone {
		@apply flex-shrink-0;
		padding: 12px 20px 16px;
		border-top: 1px solid var(--color-border);
	}

	:global(:root.dark) .calibration-zone {
		border-top-color: rgba(255, 255, 255, 0.06);
	}

	.pool-header {
		@apply flex items-center justify-between mb-2 flex-shrink-0;
	}

	.pool-title {
		@apply flex items-center gap-2;
	}

	.unassigned-badge {
		@apply flex items-center gap-1 text-xs font-semibold px-2 py-0.5 rounded-full;
		background-color: var(--color-surface-sunken);
		color: var(--color-warning);
		border: 1px solid var(--color-border);
		transition: all 0.3s ease;
		min-width: 22px;
		justify-content: center;
	}

	:global(:root.dark) .unassigned-badge {
		border-color: rgba(255, 255, 255, 0.1);
		background-color: rgba(255, 255, 255, 0.05);
	}

	.unassigned-badge.empty-pool {
		color: var(--color-success);
	}

	.badge-dot {
		width: 5px;
		height: 5px;
		border-radius: 50%;
		background-color: var(--color-warning);
		animation: pulse-dot 2s ease-in-out infinite;
	}

	@keyframes pulse-dot {
		0%,
		100% {
			opacity: 1;
			transform: scale(1);
		}
		50% {
			opacity: 0.5;
			transform: scale(0.75);
		}
	}

	.check-icon {
		width: 12px;
		height: 12px;
	}

	.pool-dropbox {
		@apply flex-1 overflow-y-auto;
		min-height: 60px;
		border: 1px dashed var(--color-border);
		border-radius: 10px;
		padding: 4px;
	}

	:global(:root.dark) .pool-dropbox {
		border-color: rgba(255, 255, 255, 0.12);
	}

	/* Remove the inner Dropbox container styling so it's transparent inside the pool */
	.pool-dropbox :global(.sectionStyle) {
		background-color: transparent !important;
		border: none !important;
		width: 100% !important;
		border-radius: 0 !important;
	}

	/* ── Right Panel: Header + Objects + Actions ── */
	right {
		@apply flex flex-col h-full;
		width: 50%;
		background-color: var(--color-surface);
	}

	.right-header {
		@apply flex-shrink-0;
		padding: 16px 20px 0;
	}

	.right-header h2 {
		color: var(--color-text-primary);
		font-size: 20px;
		font-weight: 600;
		letter-spacing: -0.01em;
		margin: 0 0 4px;
	}

	.object-zone {
		@apply flex-1 overflow-y-auto;
		min-height: 0;
		padding: 12px 20px;
	}

	/* ── Shared: Column Headers & Labels ── */
	.group-label {
		color: var(--color-text-tertiary);
		font-size: 11px;
		font-weight: 600;
		letter-spacing: 0.08em;
		text-transform: uppercase;
		margin-bottom: 8px;
		padding-left: 4px;
	}

	.column-headers {
		@apply flex items-center gap-3 mb-2;
		padding: 0 4px;
	}

	.role-label-spacer {
		width: 72px;
		flex-shrink: 0;
	}

	.col-header {
		@apply flex-1 text-center font-semibold;
		font-size: 14px;
		color: var(--color-text-tertiary);
		letter-spacing: 0.05em;
		text-transform: uppercase;
	}

	/* ── Role Lanes ── */
	.role-lane {
		@apply flex items-center gap-3 rounded-lg;
		padding: 8px 12px;
		margin-bottom: 6px;
		background-color: var(--color-surface-base);
		border: 1px solid var(--color-border);
		border-left-width: 3px;
		transition: background-color 0.15s ease, box-shadow 0.15s ease;
	}

	:global(:root.dark) .role-lane {
		border-color: rgba(255, 255, 255, 0.1);
		box-shadow: 0 1px 3px rgba(0, 0, 0, 0.4);
	}

	.role-lane:hover {
		background-color: var(--color-surface-elevated);
		box-shadow: 0 1px 4px var(--color-overlay-light);
	}

	:global(:root.dark) .role-lane:hover {
		box-shadow: 0 2px 8px rgba(0, 0, 0, 0.5);
		border-color: rgba(255, 255, 255, 0.18);
	}

	/* Color-coded left borders */
	.lane-object {
		/* border-left-color set via inline style for categorical colors */
	}
	.lane-target {
		border-left-color: #eab308;
	}
	.lane-flatfield {
		border-left-color: #a8a29e;
	}
	.lane-darkfield {
		border-left-color: #525252;
	}

	/* Dark mode overrides for calibration lane borders */
	:global(:root.dark) .lane-target {
		border-left-color: #facc15;
	}
	:global(:root.dark) .lane-flatfield {
		border-left-color: #d6d3d1;
	}
	:global(:root.dark) .lane-darkfield {
		border-left-color: #a8a29e;
	}

	.role-label {
		width: 72px;
		flex-shrink: 0;
		color: var(--color-text-secondary);
		font-size: 13px;
		font-weight: 500;
	}

	.drop-cell {
		@apply flex-1;
		min-width: 0;
	}

	/* ── Action Bar ── */
	.action-bar {
		@apply flex-shrink-0 flex items-center justify-between;
		padding: 12px 20px;
		border-top: 1px solid var(--color-border);
		background-color: var(--color-surface-elevated);
	}

	:global(:root.dark) .action-bar {
		border-top-color: rgba(255, 255, 255, 0.06);
		background-color: rgba(255, 255, 255, 0.03);
	}

	.error-msg {
		@apply flex items-center gap-2 text-sm;
		color: var(--color-error);
	}

	.error-icon {
		width: 16px;
		height: 16px;
		flex-shrink: 0;
	}

	.action-buttons {
		@apply flex gap-2 ml-auto;
	}
</style>
