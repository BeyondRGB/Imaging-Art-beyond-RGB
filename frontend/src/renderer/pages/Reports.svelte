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
	import FileSelector from "@components/FileSelector.svelte";
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
	let p90Value = null;

	function handleP90Update(e) {
		const rawVal = e.detail.p90;
		console.log("Received p90 update:", rawVal, "typeof:", typeof rawVal, "JSON:", JSON.stringify(rawVal));
		if (rawVal === null || rawVal === undefined) {
			console.log("Received null or undefined value. Ignoring update.");
			return;
		}

		let num;
		if (typeof rawVal === "object") {
			num = +rawVal;
		} else {
			num = parseFloat(rawVal);
		}

		if (!isNaN(num)) {
			p90Value = num;
		}
		console.log("Converted p90Value is now:", p90Value);
	}

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
				}
			}));
		}
		
		// Update tracking and fetch data
		loadedProjectKey = projectKey;
		requestColorManagedTargetImage(projectKey);
		getReports();
	}

	// Load an image from a file - trigger fetch immediately when file is selected
	let mainfilePath;
	$: if (mainfilePath?.length > 0) {
		const newProjectKey = mainfilePath[0];
		console.log("New Project Key from file selector:", newProjectKey);
		
		// Set the projectKey in viewState
		viewState.update(state => ({
			...state,
			projectKey: newProjectKey
		}));
		
		// Immediately load the project data
		loadProject(newProjectKey);
	}
	
	// Also handle when projectKey is set from elsewhere (e.g., returning to page)
	$: if ($viewState.projectKey !== null && $viewState.projectKey !== loadedProjectKey) {
		loadProject($viewState.projectKey);
	}

	// Handle Report and SpectralPickerMeasured responses (page-specific)
	$: if ($messageStore.length > 1 && !($messageStore[0] instanceof Blob)) {
		try {
			const message = $messageStore[0];
			if (typeof message === 'string' && message.trim().length > 0) {
				let temp = JSON.parse(message);
				if (temp["ResponseType"] === "Report") {
					console.log("Report From Server");
					if (temp["ResponseData"]["reportType"] === "Calibration") {
						viewState.update(state => ({
							...state,
							reports: {
								...state.reports,
								calibration: temp["ResponseData"]["reports"]
							}
						}));
					} else if (temp["ResponseData"]["reportType"] === "Verification") {
						viewState.update(state => ({
							...state,
							reports: {
								...state.reports,
								verification: temp["ResponseData"]["reports"]
							}
						}));
					} else if (temp["ResponseData"]["reportType"] === "SpectralPickerMeasured") {
						console.log("Spectrum Data From Server");
						spectrumDataHeatMap_est = temp["ResponseData"]["estimated_spectrum"];
						spectrumDataHeatMap_ref = temp["ResponseData"]["referenced_spectrum"];
						combinedData = [
							spectrumDataHeatMap_est, 
							spectrumDataHeatMap_ref
						];
					}
				}
			}
		} catch (e) {
			// Not JSON or parse error, ignore
		}
	}

	// Debug: log when colorManagedTargetImage changes
	$: console.log("[Reports] colorManagedTargetImage changed:", 
		$viewState.colorManagedTargetImage.dataURL ? 
		$viewState.colorManagedTargetImage.dataURL.substring(0, 50) + "..." : "(empty)");

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
			}
		}));
		loadedProjectKey = null;
		mainfilePath = null;
	}

</script>

<main class="reports-main">
	{#if $viewState.projectKey === null}
		<EmptyState title="Select a project file to import into BeyondRGB">
			<FileSelector bind:filePaths={mainfilePath} filter="project" />
		</EmptyState>
	{:else}
		<div class="art">
			<div class="report-header" class:show={$currentPage === "Reports"}>
        	<CloseButton variant="absolute-top-right" onClick={handleCloseReport} />
				<div class="report-left">
					<div class="report-name">
						{$viewState.projectKey?.split("\\").length > 2
							? $viewState.projectKey?.split("\\").at(-1)
							: $viewState.projectKey?.split("/").at(-1)}
					</div>
					<div class="report-info">
						{#if $viewState.reports.calibration?.["double_values"]?.[0]?.["data"] !== undefined}
							Mean ΔE: {parseFloat($viewState.reports.calibration["double_values"][0]["data"]).toFixed(4)}
							{#if p90Value !== null && !isNaN(p90Value)}
								<br>90th Percentile: {p90Value.toFixed(2)}
							{/if}
						{:else}
							Loading report data...
						{/if}
					</div>

            <Button variant="secondary" size="md" onClick={() => { window.electron.openNewWindow() }}>View Another Report</Button>

					{#if isVerification && $viewState.reports.verification?.["double_values"]?.[0]?.["data"] !== undefined}
						<div class="report-info">
							Verification Mean ΔE: {parseFloat($viewState.reports.verification["double_values"][0]["data"]).toFixed(4)}
						</div>
					{/if}
				</div>
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
						  <ImageViewer srcUrl={$viewState.colorManagedTargetImage.dataURL} identifier="CM_target"/>
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
						<VectorChart
							data={$viewState.reports.calibration}
							matrix={"CM"}
							ab={true}
						/>
					</div>
					<div class="report-item">
						<!-- LC vector chart -->
						<VectorChart
							data={$viewState.reports.calibration}
							matrix={"CM"}
							ab={false}
						/>
					</div>
					<div class="report-item">
						<LinearChart
							data={$viewState.reports.calibration}
							matrix={"CM"}
						/>
					</div>
				</div>
			</div>
		</div>
	{/if}
</main>

<style lang="postcss" global>
  .reports-main {
    @apply flex flex-col w-full h-full overflow-y-scroll ;
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
		width:fit-content;
		background-color: var(--color-surface-base);
    border: 1px solid var(--color-border);
		@apply h-full flex justify-center items-center p-4 pr-8 rounded-xl;
	}
	.dropdown-report-btn {
		@apply w-full flex flex-col justify-center items-center z-50;
	}
  .report-header {
    width: 100%;
    height: 20vh;
    background-color: var(--color-surface-base);
    border-bottom: 1px solid var(--color-border);
    @apply sticky top-0 z-30 flex px-[1vw] pr-[5vw] py-4 rounded-b-xl -translate-y-full
            transition-all delay-150 duration-300 ease-in justify-between;
  }
  .report-header.show {
    @apply translate-y-0;
  }
  .report-left {
    @apply w-full flex flex-col justify-center;
  }

	.report-right {
		@apply w-full rounded-xl overflow-auto;
	}
	.report-name {
		@apply text-4xl;
	}
	.reports h2 {
		@apply w-full text-3xl;
	}
	.verificationBar {
		background-color: var(--color-surface-sunken);
		@apply w-full h-full flex flex-col p-2 rounded-xl;
	}
	.new-window-button {
		align-self: baseline;
		margin-top: 5px;
	}
  .target-image-container {
    height:450px;
    @apply w-full bg-blue-600/25 relative items-center flex justify-center;
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
		background-color: var(--color-surface-elevated); @apply m-2 p-2 pb-4 rounded-lg pr-4;
	}

	.box {
		@apply m-2 shadow-md px-2 pt-1 rounded-lg p-2; background-color: var(--color-surface-elevated);
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