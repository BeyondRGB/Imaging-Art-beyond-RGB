<script lang="ts">
	import { customRefData, persistentCustomRefData, sendMessage, messageStore } from "@util/stores";
	import { modal } from "@util/stores";
	import { CloudIcon, FileIcon, AlertCircleIcon, CheckCircleIcon } from "svelte-feather-icons";
	import FileSelector from "@components/FileSelector.svelte";
	import Button from "@components/Button.svelte";
	import { onDestroy } from "svelte";

	export let closeModal;
	export let targetType = "calibration"; // 'calibration' or 'verification'

	let url = "";
	let filePaths = [];
	let loading = false;
	let error = "";
	let result = null;
	let requestId = null;

	// Handle WebSocket responses
	const unsubscribe = messageStore.subscribe(value => {
		if (!value || !value[0]) return;
		
		try {
			const data = typeof value[0] === "string" ? JSON.parse(value[0]) : value[0];
			
			if (data.ResponseType === "OpenQualiaLoaded" && data.RequestID === requestId) {
				loading = false;
				
				if (data.ResponseData.success) {
					result = data.ResponseData;
					error = "";
				} else {
					error = data.ResponseData.error || "Unknown error occurred";
					result = null;
				}
			}
		} catch (e) {
			// Not JSON or not our message
		}
	});

	onDestroy(() => {
		unsubscribe();
	});

	function validateUrl(url: string): boolean {
		if (!url.startsWith("https://")) {
			error = "URL must use HTTPS";
			return false;
		}
		if (!url.includes("Manufacturer=") || !url.includes("TargetType=") || !url.includes("TargetID=")) {
			error = "URL must contain Manufacturer, TargetType, and TargetID parameters";
			return false;
		}
		return true;
	}

	function fetchFromUrl() {
		error = "";
		result = null;
		
		if (!url.trim()) {
			error = "Please enter a URL";
			return;
		}
		
		if (!validateUrl(url)) {
			return;
		}
		
		loading = true;
		requestId = Math.floor(Math.random() * 999999);
		
		const msg = {
			RequestID: requestId,
			RequestType: "FetchOpenQualia",
			RequestData: {
				url: url.trim(),
				filePath: null,
			},
		};
		
		console.log("[OpenQualia] Sending request:", msg);
		sendMessage(JSON.stringify(msg));
	}

	function loadFromFile() {
		error = "";
		result = null;
		
		if (!filePaths || filePaths.length === 0) {
			error = "Please select a file";
			return;
		}
		
		loading = true;
		requestId = Math.floor(Math.random() * 999999);
		
		const msg = {
			RequestID: requestId,
			RequestType: "FetchOpenQualia",
			RequestData: {
				url: null,
				filePath: filePaths[0],
			},
		};
		
		console.log("[OpenQualia] Sending request:", msg);
		sendMessage(JSON.stringify(msg));
	}

	function useThisData() {
		if (!result || !result.csvPath) return;
		
		const refData = {
			name: `OpenQualia: ${result.targetName} (${result.serialNumber})`,
			fileName: result.csvPath,
			standardObserver: 1931,
			illuminants: "D50",
		};
		
		if (targetType === "calibration") {
			$customRefData.calibration = refData;
			$persistentCustomRefData.calibration.push(refData);
		} else {
			$customRefData.verification = refData;
			$persistentCustomRefData.verification.push(refData);
		}
		
		closeModal();
	}

	function getFileName(path: string): string {
		return path.split(/[/\\]/).pop() || path;
	}
</script>

<main>
	<div class="modal-container">
		<div class="header">
			<CloudIcon size="1.5x" />
			<h2>Load OpenQualia Reference Data</h2>
		</div>

		<div class="content">
			<!-- URL Entry Section -->
			<div class="section">
				<h3>Option 1: Enter OpenQualia URL</h3>
				<p class="description">
					Paste the URL from an OpenQualia label or target documentation.
				</p>
				<div class="url-input-row">
					<input
						type="text"
						bind:value={url}
						placeholder="https://targets.digitaltransitions.com/measurements?Manufacturer=..."
						disabled={loading}
					/>
					<Button
						variant="primary"
						size="sm"
						onClick={fetchFromUrl}
						disabled={loading || !url.trim()}
					>
						{loading ? "Fetching..." : "Fetch"}
					</Button>
				</div>
			</div>

			<div class="divider">
				<span>OR</span>
			</div>

			<!-- File Selection Section -->
			<div class="section">
				<h3>Option 2: Load Local OQM File</h3>
				<p class="description">
					Select an OQM/CGATS measurement file (.oqm.txt or .cgats.txt).
				</p>
				<div class="file-row">
					<FileSelector 
						bind:filePaths 
						label="Select OQM File" 
						type="Single" 
						filter="txt,oqm"
					/>
					{#if filePaths && filePaths.length > 0}
						<Button
							variant="primary"
							size="sm"
							onClick={loadFromFile}
							disabled={loading}
						>
							{loading ? "Loading..." : "Load"}
						</Button>
					{/if}
				</div>
			</div>

			<!-- Error Display -->
			{#if error}
				<div class="error-box">
					<AlertCircleIcon size="1.25x" />
					<span>{error}</span>
				</div>
			{/if}

			<!-- Result Display -->
			{#if result}
				<div class="result-box">
					<div class="result-header">
						<CheckCircleIcon size="1.25x" />
						<span>Reference Data Loaded Successfully</span>
					</div>
					<div class="result-details">
						<div class="detail-row">
							<span class="label">Target:</span>
							<span class="value">{result.targetName}</span>
						</div>
						<div class="detail-row">
							<span class="label">Serial:</span>
							<span class="value">{result.serialNumber}</span>
						</div>
						<div class="detail-row">
							<span class="label">Measured:</span>
							<span class="value">{result.measurementDate}</span>
						</div>
						<div class="detail-row">
							<span class="label">Patches:</span>
							<span class="value">{result.patchCount}</span>
						</div>
					</div>
				</div>
			{/if}
		</div>

		<div class="footer">
			<Button variant="secondary" onClick={closeModal}>Cancel</Button>
			<Button
				variant="success"
				onClick={useThisData}
				disabled={!result}
			>
				Use This Reference Data
			</Button>
		</div>
	</div>
</main>

<style lang="postcss">
	main {
		background-color: var(--color-surface);
		@apply w-[550px] flex flex-col rounded-xl shadow-2xl overflow-hidden;
	}

	.modal-container {
		@apply flex flex-col h-full;
	}

	.header {
		@apply p-4 flex items-center justify-center gap-2;
		color: var(--color-text-primary);
	}

	h2 {
		@apply text-xl font-semibold m-0;
	}

	h3 {
		@apply text-base font-medium mb-2;
		color: var(--color-text-primary);
	}

	.content {
		@apply p-6 flex flex-col gap-4;
	}

	.section {
		@apply flex flex-col;
	}

	.description {
		@apply text-sm mb-3;
		color: var(--color-text-secondary);
	}

	.url-input-row {
		@apply flex gap-2;
	}

	.url-input-row input {
		background-color: var(--color-surface-sunken);
		border: 1px solid var(--color-border);
		color: var(--color-text-primary);
		@apply flex-1 px-3 py-2 rounded-lg text-sm;
	}

	.url-input-row input:focus {
		outline: 2px solid var(--color-border-focus);
		outline-offset: 1px;
	}

	.url-input-row input::placeholder {
		color: var(--color-text-tertiary);
	}

	.file-row {
		@apply flex flex-col gap-2;
	}

	.divider {
		@apply flex items-center gap-4 py-2;
	}

	.divider::before,
	.divider::after {
		content: "";
		@apply flex-1 h-px;
		background-color: var(--color-border);
	}

	.divider span {
		@apply text-sm font-medium;
		color: var(--color-text-tertiary);
	}

	.error-box {
		background-color: rgba(239, 68, 68, 0.1);
		border: 1px solid rgba(239, 68, 68, 0.3);
		color: rgb(239, 68, 68);
		@apply p-3 rounded-lg flex items-center gap-2 text-sm;
	}

	.result-box {
		background-color: rgba(34, 197, 94, 0.1);
		border: 1px solid rgba(34, 197, 94, 0.3);
		@apply p-4 rounded-lg;
	}

	.result-header {
		color: rgb(34, 197, 94);
		@apply flex items-center gap-2 font-medium mb-3;
	}

	.result-details {
		@apply flex flex-col gap-2;
	}

	.detail-row {
		@apply flex items-center gap-2 text-sm;
	}

	.detail-row .label {
		@apply font-medium min-w-[80px];
		color: var(--color-text-secondary);
	}

	.detail-row .value {
		color: var(--color-text-primary);
	}

	.footer {
		background-color: var(--color-surface-elevated);
		@apply p-4 flex justify-end gap-3;
	}
</style>
