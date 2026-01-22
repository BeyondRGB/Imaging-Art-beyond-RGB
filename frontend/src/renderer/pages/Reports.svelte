<script lang="ts">
	import {
		viewState,
		sendMessage,
		messageStore,
		currentPage,
		requestColorManagedTargetImage,
		clearProjectViewState,
	} from "@util/stores";
	import Heatmap from "@components/Charts/HeatMap.svelte";
	import LinearChart from "@root/components/Charts/LinearChart.svelte";
	import VectorChart from "@components/Charts/VectorChart.svelte";
	import ImageViewer from "@root/components/ImageViewer.svelte";
	import LineChartMeasured from "@components/Charts/LineChartMeasured.svelte";
	import Button from "@components/Button.svelte";
	import EmptyState from "@components/EmptyState.svelte";
	import ExpandablePanel from "@components/ExpandablePanel.svelte";
	import CloseButton from "@components/CloseButton.svelte";

	let spectrumDataHeatMap_est;
	let spectrumDataHeatMap_ref;
	let shadowPos = { left: 0, top: 0 };
	let expand = false;
	let combinedData = [];
	let trueShadowPos = shadowPos;

	let wavelengthArray = Array.from({ length: 36 }, (x, i) => i * 10 + 380);
	let p90Value: number | null = null;

	function handleP90Update(e) {
		const rawVal = e.detail.p90;
		console.log("Received p90 update:", rawVal, "typeof:", typeof rawVal);

		if (rawVal === null || rawVal === undefined) {
			return;
		}

		// Convert to number
		const num = typeof rawVal === "number" ? rawVal : parseFloat(rawVal);

		if (!isNaN(num)) {
			p90Value = num;
			console.log("p90Value set to:", p90Value);
		}
	}

	// Reactive formatted value for display
	$: p90Display = p90Value !== null ? p90Value.toFixed(2) : "—";

	function handleDataPointSelect(event) {
		const { yAxisLabel, xValue } = event.detail;
		shadowPos.left = xValue;
		shadowPos.top = yAxisLabel;
		getData();
	}

	function getData() {
		console.log("Fetching Spec Data");
		if ($viewState.projectKey !== null) {
			let msg = {
				RequestID: Math.floor(Math.random() * 999999999),
				RequestType: "SpectralPickerMeasured",
				RequestData: {
					name: $viewState.projectKey,
					coordinates: {
						x: shadowPos.left,
						y: shadowPos.top,
					},
					size: 0.01,
				},
			};
			sendMessage(JSON.stringify(msg));
		}
	}

	function getReports() {
		let rand = Math.floor(Math.random() * 99999999);
		let msg = {
			RequestID: rand,
			RequestType: "Reports",
			RequestData: {
				name: $viewState.projectKey,
			},
		};

		console.log("Fetching Reports");
		sendMessage(JSON.stringify(msg));
	}

	// Track which projectKey we've loaded
	let loadedProjectKey: string | null = null;

	// Function to load project data
	function loadProject(projectKey: string) {
		console.log(`Loading project: ${projectKey}, previous: ${loadedProjectKey}`);

		// Clear old report data if switching projects
		if (loadedProjectKey !== null && loadedProjectKey !== projectKey) {
			viewState.update(state => ({
				...state,
				reports: {
					calibration: null,
					verification: null,
				},
				colorManagedTargetImage: {
					dataURL: "",
					name: "Waiting...",
				},
			}));
		}

		// Update tracking and fetch data
		loadedProjectKey = projectKey;
		requestColorManagedTargetImage(projectKey);
		getReports();
	}

	// Handle file selection from EmptyState dropzone
	function handleFileSelect(e: CustomEvent<string[]>) {
		const filePaths = e.detail;
		if (filePaths?.length > 0) {
			const newProjectKey = filePaths[0];
			console.log("New Project Key from file selector:", newProjectKey);

			// Reset loadedProjectKey to ensure the reactive statement triggers a fresh load
			// This prevents race conditions where the old project data persists
			loadedProjectKey = null;

			// Set the projectKey in viewState
			// The reactive statement will handle calling loadProject
			viewState.update(state => ({
				...state,
				projectKey: newProjectKey,
			}));
		}
	}

	// Also handle when projectKey is set from elsewhere (e.g., returning to page)
	$: if ($viewState.projectKey !== null && $viewState.projectKey !== loadedProjectKey) {
		loadProject($viewState.projectKey);
	}

	// Handle Report and SpectralPickerMeasured responses (page-specific)
	$: if ($messageStore.length > 1 && !($messageStore[0] instanceof Blob)) {
		try {
			const message = $messageStore[0];
			if (typeof message === "string" && message.trim().length > 0) {
				let temp = JSON.parse(message);
				if (temp["ResponseType"] === "Report") {
					console.log("Report From Server");
					if (temp["ResponseData"]["reportType"] === "Calibration") {
						viewState.update(state => ({
							...state,
							reports: {
								...state.reports,
								calibration: temp["ResponseData"]["reports"],
							},
						}));
					} else if (temp["ResponseData"]["reportType"] === "Verification") {
						viewState.update(state => ({
							...state,
							reports: {
								...state.reports,
								verification: temp["ResponseData"]["reports"],
							},
						}));
					} else if (temp["ResponseData"]["reportType"] === "SpectralPickerMeasured") {
						console.log("Spectrum Data From Server");
						spectrumDataHeatMap_est = temp["ResponseData"]["estimated_spectrum"];
						spectrumDataHeatMap_ref = temp["ResponseData"]["referenced_spectrum"];
						combinedData = [spectrumDataHeatMap_est, spectrumDataHeatMap_ref];
					}
				}
			}
		} catch (e) {
			// Not JSON or parse error, ignore
		}
	}

	// Debug: log when colorManagedTargetImage changes
	$: console.log(
		"[Reports] colorManagedTargetImage changed:",
		$viewState.colorManagedTargetImage.dataURL
			? $viewState.colorManagedTargetImage.dataURL.substring(0, 50) + "..."
			: "(empty)"
	);

	$: isVerification =
		$viewState.reports.verification != null &&
		$viewState.reports.verification["double_values"].length > 0;

	function handleCloseReport() {
		// Clear only report-specific data, preserve colorManagedImage for Image Viewer
		viewState.update(state => ({
			...state,
			projectKey: null,
			reports: {
				calibration: null,
				verification: null,
			},
			colorManagedTargetImage: {
				dataURL: "",
				name: "Waiting...",
			},
		}));
		loadedProjectKey = null;
	}
</script>

<main class="reports-main">
	{#if $viewState.projectKey === null}
		<EmptyState
			title="Select a project file to import into BeyondRGB"
			filter="project"
			on:select={handleFileSelect}
		/>
	{:else}
		<div class="art">
			<div class="report-header" class:show={$currentPage === "Reports"}>
				<div class="header-top-row">
					<div class="report-name">
						{$viewState.projectKey?.split("\\").length > 2
							? $viewState.projectKey?.split("\\").at(-1)
							: $viewState.projectKey?.split("/").at(-1)}
					</div>
					<div class="header-actions">
						<Button
							variant="secondary"
							size="sm"
							onClick={() => {
								window.electron.openNewWindow();
							}}>View Another Report</Button
						>
						<CloseButton onClick={handleCloseReport} />
					</div>
				</div>

				{#if $viewState.reports.calibration?.["double_values"]?.[0]?.["data"] !== undefined}
					<div class="stats-grid">
						<div class="stat-card">
							<span class="stat-label">Mean ΔE</span>
							<span class="stat-value"
								>{parseFloat($viewState.reports.calibration["double_values"][0]["data"]).toFixed(
									4
								)}</span
							>
						</div>

						<div class="stat-card">
							<span class="stat-label">90th Percentile</span>
							<span class="stat-value">{p90Display}</span>
						</div>

						{#if isVerification && $viewState.reports.verification?.["double_values"]?.[0]?.["data"] !== undefined}
							<div class="stat-card">
								<span class="stat-label">Verification Mean ΔE</span>
								<span class="stat-value"
									>{parseFloat($viewState.reports.verification["double_values"][0]["data"]).toFixed(
										4
									)}</span
								>
							</div>

							<div class="stat-card">
								<span class="stat-label">Verification 90th %</span>
								<span class="stat-value">
									{#if $viewState.reports.verification?.["double_values"]?.[1]?.["data"] !== undefined}
										{parseFloat(
											$viewState.reports.verification["double_values"][1]["data"]
										).toFixed(2)}
									{:else}
										—
									{/if}
								</span>
							</div>
						{/if}
					</div>
				{:else}
					<div class="loading-text">Loading report data...</div>
				{/if}
			</div>
			<div class="reports">
				<div class="reportBody">
					<div class="report-item">
						<Heatmap
							on:datapointselect={handleDataPointSelect}
							on:p90update={handleP90Update}
							data={$viewState.reports.calibration}
						/>
						<div class="target-image-container">
							<ImageViewer
								srcUrl={$viewState.colorManagedTargetImage.dataURL}
								identifier="CM_target"
							/>
						</div>
						{#if $currentPage === "Reports"}
							<ExpandablePanel
								bind:expanded={expand}
								position="right"
								width="30vw"
								handlePosition="50%"
								zIndex={40}
							>
								<div class="box" id="brush">
									<p>Click on a Heatmap box to view the Estimated vs. Reference Spectral Curve</p>
								</div>
								<div class="chart">
									<LineChartMeasured
										bind:data={combinedData}
										bind:wavelengthArray
										bind:trueShadowPos
									/>
								</div>
							</ExpandablePanel>
						{/if}
					</div>

					<div class="report-item">
						<!-- AB vector chart -->
						<VectorChart data={$viewState.reports.calibration} matrix={"CM"} ab={true} />
					</div>
					<div class="report-item">
						<!-- LC vector chart -->
						<VectorChart data={$viewState.reports.calibration} matrix={"CM"} ab={false} />
					</div>
					<div class="report-item">
						<LinearChart data={$viewState.reports.calibration} matrix={"CM"} />
					</div>
				</div>
			</div>
		</div>
	{/if}
</main>

<style lang="postcss" global>
	.reports-main {
		@apply flex flex-col w-full h-full overflow-y-scroll overflow-x-hidden;
	}
	.art {
		@apply flex flex-col relative;
	}
	.reports {
		@apply w-full h-full flex flex-col gap-2;
	}
	.reportBody {
		transform-origin: left top;
		background-color: var(--color-surface);
		@apply w-full h-full transition-all duration-300
          rounded-b-2xl p-4 flex flex-col items-center justify-center gap-2;
	}
	.report-item {
		background-color: var(--color-surface-base);
		border: 1px solid var(--color-border);
		@apply h-full flex justify-center items-center p-4 rounded-xl max-w-full flex-wrap gap-4;
	}

	.report-item > :global(*) {
		@apply min-w-0 flex-shrink;
	}
	.dropdown-report-btn {
		@apply w-full flex flex-col justify-center items-center z-50;
	}
	.report-header {
		width: 100%;
		background-color: var(--color-surface-base);
		@apply sticky top-0 z-30 flex flex-col gap-3 px-6 py-4 -translate-y-full
            transition-all delay-150 duration-300 ease-in;
	}
	.report-header.show {
		@apply translate-y-0;
	}

	.header-top-row {
		@apply flex items-center justify-between gap-4;
	}

	.header-actions {
		@apply flex items-center gap-3;
	}

	.report-name {
		@apply text-2xl font-medium;
		color: var(--color-text-primary);
	}

	.stats-grid {
		@apply flex flex-wrap gap-6;
	}

	.stat-card {
		@apply flex flex-col gap-0.5;
	}

	.stat-label {
		color: var(--color-text-secondary);
		@apply text-xs font-medium uppercase tracking-wider opacity-70;
	}

	.stat-value {
		color: var(--color-text-primary);
		@apply text-xl font-semibold;
	}

	.loading-text {
		color: var(--color-text-secondary);
		@apply text-base opacity-70;
	}
	.reports h2 {
		@apply w-full text-3xl;
	}
	.target-image-container {
		height: 450px;
		max-width: 500px;
		@apply flex-1 min-w-[300px] relative items-center flex justify-center;
	}

	.brushBar {
		@apply w-full h-2 rounded-xl;
	}
	.brushBar::-webkit-slider-thumb {
		background-color: var(--color-interactive);
		@apply w-4 h-4 cursor-pointer rounded-full outline outline-1;
		outline-color: var(--color-border);
	}

	.pixSize {
		background-color: var(--color-surface-sunken);
		@apply flex gap-1 p-1 rounded-lg;
	}

	.chart {
		background-color: var(--color-surface-elevated);
		@apply m-2 p-2 pb-4 rounded-lg pr-4;
	}

	.box {
		@apply m-2 shadow-md px-2 pt-1 rounded-lg p-2;
		background-color: var(--color-surface-elevated);
	}

	.numberInput {
		background-color: var(--color-surface-sunken);
		border: 1px solid var(--color-border);
		@apply p-0.5 rounded-lg
          focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2;
	}
	.sizeSettings {
		@apply flex justify-between items-center text-base pl-4;
	}
</style>
