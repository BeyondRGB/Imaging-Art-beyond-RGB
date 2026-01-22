<script lang="ts">
	import SpecPickViewer from "@components/SpectralPicker/SpecPickViewer.svelte";
	import {
		viewState,
		sendMessage,
		messageStore,
		currentPage,
		serverError,
		requestColorManagedImage,
		clearProjectViewState,
	} from "@util/stores";
	import {
		XIcon,
		Maximize2Icon,
		Minimize2Icon,
		ChevronLeftIcon,
		ChevronRightIcon,
	} from "svelte-feather-icons";
	import LineChart from "@components/Charts/LineChart.svelte";
	import SwitchRow from "@components/SwitchRow.svelte";
	import Card from "@components/Card.svelte";
	import EmptyState from "@components/EmptyState.svelte";

	let showBrush = false;
	let stackCurves = false;
	let size = 0.01; // Default size matching SpecPickViewer default
	let trueSize;
	let shadowPos = { left: 0, top: 0 };
	let trueShadowPos = { left: 0, top: 0 };
	let spectrumData = []; // Initialize as empty array
	let oldProjectKey: string | null = null;

	let loading = false;
	let expand = false; // Start collapsed like master

	let wavelengthArray = Array.from({ length: 36 }, (x, i) => i * 10 + 380);

	function getData() {
		console.log("Fetching Spec Data");
		if ($viewState.projectKey !== null) {
			let msg = {
				RequestID: Math.floor(Math.random() * 999999999),
				RequestType: "SpectralPicker",
				RequestData: {
					name: $viewState.projectKey,
					coordinates: {
						x: shadowPos.left,
						y: shadowPos.top,
					},
					size: size,
				},
			};
			sendMessage(JSON.stringify(msg));
		}
	}

	$: if (shadowPos !== null) {
		getData();
	}

	// Handle SpectralPicker and Error responses (page-specific)
	$: if ($messageStore.length > 1 && !($messageStore[0] instanceof Blob)) {
		try {
			const message = $messageStore[0];
			if (typeof message === "string" && message.trim().length > 0) {
				let temp = JSON.parse(message);

				if (temp["ResponseType"] === "SpectralPicker") {
					console.log("Spectrum Data From Server");
					spectrumData = temp["ResponseData"]["spectrum"];
				} else if (temp["ResponseType"] === "Error") {
					// Error handler
					if (temp["ResponseData"] && temp["ResponseData"]["critical"]) {
						serverError.set({
							sender: temp["ResponseData"]["sender"],
							message: temp["ResponseData"]["message"],
						});
						console.log({ SERVERERROR: temp["ResponseData"] });
					}
				}
			}
		} catch (e) {
			// Not JSON or parse error, ignore
		}
	}

	// Track loading state based on colorManagedImage updates
	$: if ($viewState.colorManagedImage.dataURL.length > 0) {
		loading = false;
	}

	// Handle project key changes. When project changes, clear old project state before loading new one.
	$: if ($viewState.projectKey !== null) {
		if (oldProjectKey !== $viewState.projectKey && oldProjectKey !== null) {
			console.log(`Project changed from ${oldProjectKey} to ${$viewState.projectKey}`);
			// Clear the old project state before loading the new one
			clearProjectViewState();
			// Restore the new project key that was just set
			viewState.update(state => ({
				...state,
				projectKey: $viewState.projectKey,
			}));
			// Reset spectrum data for the new project
			spectrumData = [];
		}
		oldProjectKey = $viewState.projectKey;
	}

	// Handle file selection from EmptyState dropzone
	function handleFileSelect(e: CustomEvent<string[]>) {
		const filePaths = e.detail;
		if (filePaths?.length > 0) {
			console.log("New Project Key from file selector:", filePaths[0]);
			viewState.update(state => ({
				...state,
				projectKey: filePaths[0],
			}));
		}
	}

	// When a new project is opened and colorManagedImage is not available, fetch the image
	$: if (
		$currentPage === "SpecPicker" &&
		$viewState.projectKey !== null &&
		$viewState.colorManagedImage.dataURL.length < 1
	) {
		console.log("Getting Color Managed Image using centralized handler");
		loading = true;
		requestColorManagedImage($viewState.projectKey);
	}

	// Fetch initial spectrum data when image is loaded
	$: if (
		$currentPage === "SpecPicker" &&
		$viewState.projectKey !== null &&
		$viewState.colorManagedImage.dataURL.length > 0 &&
		(!spectrumData || spectrumData.length === 0)
	) {
		console.log("Fetching initial spectrum data");
		getData();
	}

	let isFullScreen = window.innerHeight == screen.height;

	function closeImage() {
		// Clear all project state using the centralized helper
		clearProjectViewState();
		showBrush = false;
		spectrumData = [];
		oldProjectKey = null;
	}
</script>

<main>
	{#if $viewState.projectKey === null}
		<EmptyState
			title="Select a project file to import into BeyondRGB"
			filter="project"
			on:select={handleFileSelect}
		/>
	{/if}
	<div class="content" id="picker-content">
		<div class="viewer-layout">
			<div class="image-section">
				<button
					class="fullBtn"
					on:click={() => {
						if (isFullScreen) {
							document.exitFullscreen();
							isFullScreen = false;
						} else {
							document.querySelector(".image-section").requestFullscreen();
							isFullScreen = true;
						}
					}}
				>
					{#if isFullScreen}
						<Minimize2Icon size="1.25x" />
					{:else}
						<Maximize2Icon size="1.25x" />
					{/if}
				</button>
				<button class="closeBtn" on:click={closeImage}>
					<XIcon size="1.25x" />
				</button>

				<div class="image-container">
					{#if loading}
						<div class="loading">
							<div class="loading-box">Loading<span class="loader" /></div>
						</div>
					{/if}

					<SpecPickViewer
						bind:shadowPos
						bind:trueShadowPos
						bind:trueSize
						bind:showBrush
						bind:size
						bind:loading
					/>
				</div>
			</div>

			<div class="drawer-wrapper" class:open={expand}>
				<button class="drawer-toggle" on:click={() => (expand = !expand)}>
					{#if expand}
						<ChevronRightIcon size="1.25x" />
					{:else}
						<ChevronLeftIcon size="1.25x" />
					{/if}
				</button>

				<div class="drawer-panel">
					<div class="spectral-panel">
						<div class="panel-section">
							<SwitchRow
								label="Enable Spectral Picker"
								description="Show the sampling brush on the image preview"
								bind:checked={showBrush}
								ariaLabel="Toggle spectral picker"
							/>
							<SwitchRow
								label="Stack Spectral Curves"
								description="Keep previous spectra visible when sampling"
								bind:checked={stackCurves}
								ariaLabel="Toggle stacking spectral curves"
							/>
						</div>

						<div class="panel-section brush-section">
							<div class="panel-heading">Brush Size</div>
							<div class="brush-controls">
								<label class="brush-label" for="brush-size">Set brush size</label>
								<div class="brush-slider">
									<input
										id="brush-size"
										class="brushBar"
										type="range"
										min="0.001"
										max="0.03"
										step=".0005"
										bind:value={size}
									/>
									<div class="pixSize">
										<span>{trueSize ? parseFloat(trueSize).toFixed(1) : "0.0"}</span>
										<span>px</span>
									</div>
								</div>
							</div>
						</div>

						{#if expand}
							<div class="panel-section chart-section">
								<Card
									variant="elevated"
									padding="md"
									rounded={true}
									shadow="medium"
									className="chart-card"
								>
									<LineChart
										bind:data={spectrumData}
										bind:wavelengthArray
										bind:trueShadowPos
										stack={stackCurves}
									/>
								</Card>
							</div>
						{/if}
					</div>
				</div>
			</div>
		</div>
	</div>
</main>

<style lang="postcss">
	main {
		@apply flex h-full w-full justify-center flex-col relative;
	}
	.content {
		@apply w-full h-full flex justify-center items-center p-6;
	}

	.viewer-layout {
		@apply w-full h-full flex gap-0;
	}

	.image-section {
		background-color: var(--color-surface-sunken);
		@apply relative flex-1 flex flex-col transition-all duration-500;
	}

	.image-container {
		@apply relative w-full h-full overflow-visible;
	}
	.pixSize {
		background-color: var(--color-surface);
		border: 1px solid var(--color-border);
		@apply flex gap-1 px-2 py-1 rounded-lg text-sm font-medium;
	}
	.brushBar {
		@apply w-full h-2 rounded-xl;
	}
	.brushBar::-webkit-slider-thumb {
		background-color: var(--color-interactive);
		@apply w-4 h-4 cursor-pointer rounded-full outline outline-1;
		outline-color: var(--color-border);
	}

	.drawer-wrapper {
		@apply relative flex h-full transition-all duration-500;
		width: 0;
	}

	.drawer-wrapper.open {
		width: 30vw;
	}

	.drawer-toggle {
		background-color: var(--color-surface);
		color: var(--color-text-primary);
		border: 1px solid var(--color-border);
		border-right: 0;
		box-shadow: -2px 0 4px rgba(0, 0, 0, 0.1);
		@apply absolute top-1/2 -translate-y-1/2 h-12 w-10 flex justify-center 
           items-center cursor-pointer transition-all duration-500 z-10 rounded-l-full;
		left: -2.5rem;
	}

	.drawer-toggle:hover {
		background-color: var(--color-interactive-hover);
	}

	.drawer-toggle:focus {
		outline: 2px solid var(--color-border-focus);
		outline-offset: 2px;
	}

	.drawer-panel {
		background-color: var(--color-surface-elevated);
		border-left: 1px solid var(--color-border);
		@apply flex-1 h-full overflow-y-auto;
	}

	.spectral-panel {
		@apply flex flex-col gap-4 p-4 h-full;
	}

	.panel-section {
		@apply flex flex-col gap-4;
	}

	.brush-section {
		background-color: var(--color-surface-sunken);
		border: 1px solid var(--color-border);
		@apply rounded-lg p-4 gap-3;
	}

	.panel-heading {
		color: var(--color-text-primary);
		@apply text-sm font-semibold uppercase tracking-wide;
	}

	.brush-controls {
		@apply flex flex-col gap-3;
	}

	.brush-label {
		color: var(--color-text-secondary);
		@apply text-sm font-medium;
	}

	.brush-slider {
		@apply flex justify-center items-center gap-2;
	}

	.chart-section {
		@apply flex-1;
	}

	:global(.chart-card) {
		@apply h-full;
	}

	.loading {
		background-color: var(--color-overlay-heavy);
		@apply absolute w-full h-full z-[49] flex justify-center items-center;
	}
	.loading-box {
		@apply h-full flex flex-col gap-2 justify-center items-center text-2xl;
	}
	.loader {
		width: 48px;
		height: 48px;
		background: #11ff00;
		transform: perspective(200px) rotateX(65deg) rotate(45deg);
		color: rgb(255, 0, 0);
		animation: layers1 1s linear infinite alternate;
		@apply z-50;
	}
	.loader:after {
		content: "";
		position: absolute;
		inset: 0;
		background: rgb(0, 0, 255);
		animation: layerTr 1s linear infinite alternate;
	}

	@keyframes layers1 {
		0% {
			box-shadow: 0px 0px 0 0px;
		}
		90%,
		100% {
			box-shadow: 20px 20px 0 -4px;
		}
	}
	@keyframes layerTr {
		0% {
			transform: translate(0, 0) scale(1);
		}
		100% {
			transform: translate(-25px, -25px) scale(1);
		}
	}

	.fullBtn {
		@apply absolute right-0 m-1 z-50 p-1 bg-transparent ring-0
            hover:bg-blue-500/25 transition-all duration-500;
	}

	.closeBtn {
		@apply absolute right-8 m-1 z-50 p-1 bg-transparent ring-0
            hover:bg-red-500/25 transition-all duration-500;
	}
</style>
