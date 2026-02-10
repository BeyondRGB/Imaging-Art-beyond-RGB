<script lang="ts">
	import { customRefData, persistentCustomRefData, sendMessage, messageStore, processState } from "@util/stores";
	import { CloudIcon, FileIcon, AlertCircleIcon, CheckCircleIcon, CameraIcon } from "svelte-feather-icons";
	import FileSelector from "@components/FileSelector.svelte";
	import Button from "@components/Button.svelte";
	import { onDestroy, onMount } from "svelte";

	export let closeModal;
	export let targetType: "calibration" | "verification" = "calibration";

	let url = "";
	let filePaths = [];
	let qrImagePaths = [];
	let loading = false;
	let scanning = false;
	let error = "";
	let result = null;
	let requestId = null;
	let qrScanRequestId = null;
	let qrResult = null;
	let hasTargetImage = false;
	let autoDetectAttempted = false;
	let useQrHint = false;
	let qrMarginPercent = 10;
	let qrHintTop = 0.0;
	let qrHintLeft = 0.0;
	let qrHintBottom = 1.0;
	let qrHintRight = 1.0;
	let qrScanTimeoutHandle: ReturnType<typeof setTimeout> | null = null;
	let qrHintPreviewEl: HTMLDivElement | null = null;
	let qrHintDragging = false;
	let qrPreviewSrc = "";
	let qrPreviewAvailable = false;

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
			
			// Handle QR scan response
			if (data.ResponseType === "QRCodeScanned" && data.RequestID === qrScanRequestId) {
				clearQrScanTimeout();
				scanning = false;
				
				if (data.ResponseData.found) {
					qrResult = data.ResponseData;
					url = data.ResponseData.url;
					error = "";
					
					if (data.ResponseData.isOpenQualia) {
						// Auto-fetch if it's a valid OpenQualia URL
						fetchFromUrl();
					}
				} else {
					error = data.ResponseData.error || "No QR code found";
					qrResult = null;
				}
			}
		} catch (e) {
			// Not JSON or not our message
		}
	});

	onDestroy(() => {
		clearQrScanTimeout();
		unsubscribe();
	});

	function clearQrScanTimeout() {
		if (qrScanTimeoutHandle) {
			clearTimeout(qrScanTimeoutHandle);
			qrScanTimeoutHandle = null;
		}
	}

	function startQrScanTimeout(requestId: number) {
		clearQrScanTimeout();
		qrScanTimeoutHandle = setTimeout(() => {
			if (scanning && qrScanRequestId === requestId) {
				scanning = false;
				error = "QR scan timed out. Try again or use a QR area hint.";
			}
		}, 12000);
	}

	function clamp01(value: number): number {
		return Math.min(1, Math.max(0, value));
	}

	function toNormalizedPoint(event: MouseEvent) {
		if (!qrHintPreviewEl) return null;
		const rect = qrHintPreviewEl.getBoundingClientRect();
		const x = clamp01((event.clientX - rect.left) / rect.width);
		const y = clamp01((event.clientY - rect.top) / rect.height);
		return { x, y };
	}

	function startQrHintSelection(event: MouseEvent) {
		if (!useQrHint || !qrHintPreviewEl || scanning || loading) return;
		const point = toNormalizedPoint(event);
		if (!point) return;
		qrHintDragging = true;
		qrHintTop = point.y;
		qrHintBottom = point.y;
		qrHintLeft = point.x;
		qrHintRight = point.x;
	}

	function updateQrHintSelection(event: MouseEvent) {
		if (!qrHintDragging) return;
		const point = toNormalizedPoint(event);
		if (!point) return;
		const minY = Math.min(qrHintTop, point.y);
		const maxY = Math.max(qrHintTop, point.y);
		const minX = Math.min(qrHintLeft, point.x);
		const maxX = Math.max(qrHintLeft, point.x);
		qrHintTop = minY;
		qrHintBottom = maxY;
		qrHintLeft = minX;
		qrHintRight = maxX;
	}

	function stopQrHintSelection() {
		qrHintDragging = false;
	}

	function normalizeHintBounds() {
		const top = Math.min(qrHintTop, qrHintBottom);
		const bottom = Math.max(qrHintTop, qrHintBottom);
		const left = Math.min(qrHintLeft, qrHintRight);
		const right = Math.max(qrHintLeft, qrHintRight);
		const nextTop = clamp01(top);
		const nextBottom = clamp01(bottom);
		const nextLeft = clamp01(left);
		const nextRight = clamp01(right);
		if (
			nextTop !== qrHintTop ||
			nextBottom !== qrHintBottom ||
			nextLeft !== qrHintLeft ||
			nextRight !== qrHintRight
		) {
			qrHintTop = nextTop;
			qrHintBottom = nextBottom;
			qrHintLeft = nextLeft;
			qrHintRight = nextRight;
		}
	}

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

	function scanQRFromImage() {
		error = "";
		qrResult = null;
		
		if (!qrImagePaths || qrImagePaths.length === 0) {
			error = "Please select an image to scan";
			return;
		}
		
		scanning = true;
		qrScanRequestId = Math.floor(Math.random() * 999999);
		
		const msg = {
			RequestID: qrScanRequestId,
			RequestType: "ScanQRCode",
			RequestData: {
				imagePath: qrImagePaths[0],
				targetRegion: useQrHint
					? {
							top: qrHintTop,
							left: qrHintLeft,
							bottom: qrHintBottom,
							right: qrHintRight,
					  }
					: null,
				marginPercent: qrMarginPercent,
			},
		};
		
		console.log("[OpenQualia] Sending QR scan request:", msg);
		sendMessage(JSON.stringify(msg));
		startQrScanTimeout(qrScanRequestId);
	}

	function scanQRFromTargetImage() {
		error = "";
		qrResult = null;
		
		// Get the current target image path from processState.
		// Prefer verification image stack when loading verification target data.
		let targetImage = "";
		const stack = $processState.artStacks[0];
		
		if (targetType === "verification") {
			if (stack?.fields?.targetB?.length > 0) {
				targetImage = stack.fields.targetB[0].name;
			} else if (stack?.fields?.imageB?.[0]?.[0]?.name) {
				targetImage = stack.fields.imageB[0][0].name;
			}
		}

		if (!targetImage && stack?.fields?.targetA?.length > 0) {
			targetImage = stack.fields.targetA[0].name;
		} else if (!targetImage && stack?.fields?.imageA?.[0]?.[0]?.name) {
			targetImage = stack.fields.imageA[0][0].name;
		}
		
		if (!targetImage) {
			error = "No target image loaded. Please load images first.";
			return;
		}

		let targetRegion = null;
		const targetState: any =
			targetType === "verification" ? stack?.verificationTarget : stack?.colorTarget;
		if (useQrHint) {
			targetRegion = {
				top: qrHintTop,
				left: qrHintLeft,
				bottom: qrHintBottom,
				right: qrHintRight,
			};
		} else if (
			targetState &&
			targetState.top != null &&
			targetState.left != null &&
			targetState.bottom != null &&
			targetState.right != null
		) {
			targetRegion = {
				top: targetState.top,
				left: targetState.left,
				bottom: targetState.bottom,
				right: targetState.right,
				resolution: targetState.resolution ?? 1,
			};
		}
		
		scanning = true;
		qrScanRequestId = Math.floor(Math.random() * 999999);
		
		const msg = {
			RequestID: qrScanRequestId,
			RequestType: "ScanQRCode",
			RequestData: {
				imagePath: targetImage,
				targetRegion,
				marginPercent: qrMarginPercent,
			},
		};
		
		console.log("[OpenQualia] Scanning QR from target image:", msg);
		sendMessage(JSON.stringify(msg));
		startQrScanTimeout(qrScanRequestId);
	}

	// Check if we have a target image loaded for current target type.
	$: {
		const stack = $processState.artStacks[0];
		if (targetType === "verification") {
			hasTargetImage =
				(stack?.fields?.targetB?.length ?? 0) > 0 ||
				Boolean(stack?.fields?.imageB?.[0]?.[0]?.name) ||
				(stack?.fields?.targetA?.length ?? 0) > 0 ||
				Boolean(stack?.fields?.imageA?.[0]?.[0]?.name);
		} else {
			hasTargetImage =
				(stack?.fields?.targetA?.length ?? 0) > 0 ||
				Boolean(stack?.fields?.imageA?.[0]?.[0]?.name);
		}
	}

	$: {
		const stack = $processState.artStacks[0];
		if (targetType === "verification") {
			qrPreviewSrc =
				stack?.verificationTargetImage?.dataURL ||
				stack?.colorTargetImage?.dataURL ||
				"";
		} else {
			qrPreviewSrc = stack?.colorTargetImage?.dataURL || "";
		}
		qrPreviewAvailable = Boolean(qrPreviewSrc);
	}

	onMount(() => {
		// MVP auto-detect flow: attempt one scan as soon as modal opens.
		if (!autoDetectAttempted && hasTargetImage) {
			autoDetectAttempted = true;
			scanQRFromTargetImage();
		}
	});

	$: qrHintTop = Math.min(1, Math.max(0, qrHintTop));
	$: qrHintLeft = Math.min(1, Math.max(0, qrHintLeft));
	$: qrHintBottom = Math.min(1, Math.max(0, qrHintBottom));
	$: qrHintRight = Math.min(1, Math.max(0, qrHintRight));
	$: qrMarginPercent = Math.min(100, Math.max(0, qrMarginPercent));
	$: normalizeHintBounds();
</script>

<svelte:window on:mousemove={updateQrHintSelection} on:mouseup={stopQrHintSelection} />

<main>
	<div class="modal-container">
		<div class="header">
			<CloudIcon size="1.5x" />
			<h2>Load OpenQualia Reference Data</h2>
		</div>

		<div class="content">
			<!-- QR Scan Section -->
			<div class="section qr-section">
				<h3>Scan QR Code from Image</h3>
				<p class="description">
					Automatically detect and read QR codes from target images.
				</p>
				<div class="qr-buttons">
					<Button
						variant="primary"
						size="md"
						onClick={scanQRFromTargetImage}
						disabled={scanning || loading || !hasTargetImage}
						className="scan-btn"
					>
						<CameraIcon size="1.25x" />
						<span>{scanning ? "Scanning..." : "Scan Current Target Image"}</span>
					</Button>
					<div class="qr-file-row">
						<FileSelector 
							bind:filePaths={qrImagePaths}
							label="Or select image file" 
							type="Single" 
							filter="tif,tiff,jpg,jpeg,png"
						/>
						{#if qrImagePaths && qrImagePaths.length > 0}
							<Button
								variant="secondary"
								size="sm"
								onClick={scanQRFromImage}
								disabled={scanning || loading}
							>
								{scanning ? "Scanning..." : "Scan"}
							</Button>
						{/if}
					</div>
				</div>
				<div class="qr-hint-box">
					<label class="hint-toggle">
						<input type="checkbox" bind:checked={useQrHint} />
						<span>Use QR area hint (for tiny QR codes)</span>
					</label>
					{#if useQrHint}
						<p class="hint-desc">Normalized image coords (0.0 to 1.0)</p>
						{#if qrPreviewAvailable}
							<div
								class="hint-preview"
								bind:this={qrHintPreviewEl}
								on:mousedown={startQrHintSelection}
							>
								<img src={qrPreviewSrc} alt="Target preview for QR area selection" draggable="false" />
								<div
									class="hint-selection"
									style="
										top: {qrHintTop * 100}%;
										left: {qrHintLeft * 100}%;
										width: {Math.max(0.01, (qrHintRight - qrHintLeft) * 100)}%;
										height: {Math.max(0.01, (qrHintBottom - qrHintTop) * 100)}%;
									"
								/>
							</div>
							<p class="hint-help">Drag on preview to define QR patch selection.</p>
						{:else}
							<p class="hint-missing">
								Target preview not available yet. You can still enter coordinates manually.
							</p>
						{/if}
						<div class="hint-grid">
							<label>
								Top
								<input
									type="number"
									min="0"
									max="1"
									step="0.01"
									bind:value={qrHintTop}
									disabled={scanning || loading}
								/>
							</label>
							<label>
								Left
								<input
									type="number"
									min="0"
									max="1"
									step="0.01"
									bind:value={qrHintLeft}
									disabled={scanning || loading}
								/>
							</label>
							<label>
								Bottom
								<input
									type="number"
									min="0"
									max="1"
									step="0.01"
									bind:value={qrHintBottom}
									disabled={scanning || loading}
								/>
							</label>
							<label>
								Right
								<input
									type="number"
									min="0"
									max="1"
									step="0.01"
									bind:value={qrHintRight}
									disabled={scanning || loading}
								/>
							</label>
							<label class="margin-field">
								Margin %
								<input
									type="number"
									min="0"
									max="100"
									step="1"
									bind:value={qrMarginPercent}
									disabled={scanning || loading}
								/>
							</label>
						</div>
					{/if}
				</div>
				{#if qrResult}
					<div class="qr-found">
						<CheckCircleIcon size="1x" />
						<span>QR found{qrResult.isOpenQualia ? " (OpenQualia URL)" : ""}</span>
					</div>
				{/if}
			</div>

			<div class="divider">
				<span>OR ENTER MANUALLY</span>
			</div>

			<!-- URL Entry Section -->
			<div class="section">
				<h3>Enter OpenQualia URL</h3>
				<p class="description">
					Paste the URL from an OpenQualia label or target documentation.
				</p>
				<div class="url-input-row">
					<input
						type="text"
						bind:value={url}
						placeholder="https://targets.digitaltransitions.com/measurements?Manufacturer=..."
						disabled={loading || scanning}
					/>
					<Button
						variant="primary"
						size="sm"
						onClick={fetchFromUrl}
						disabled={loading || scanning || !url.trim()}
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
				<h3>Load Local OQM File</h3>
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
							disabled={loading || scanning}
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

	.qr-section {
		background-color: var(--color-surface-elevated);
		@apply p-4 rounded-lg;
	}

	.qr-buttons {
		@apply flex flex-col gap-3;
	}

	:global(.scan-btn) {
		@apply flex items-center justify-center gap-2 w-full !important;
	}

	.qr-file-row {
		@apply flex items-end gap-2;
	}

	.qr-found {
		color: rgb(34, 197, 94);
		@apply flex items-center gap-2 text-sm mt-2 font-medium;
	}

	.qr-hint-box {
		@apply mt-3;
	}

	.hint-toggle {
		@apply flex items-center gap-2 text-sm;
		color: var(--color-text-secondary);
	}

	.hint-toggle input {
		@apply w-4 h-4;
	}

	.hint-desc {
		@apply text-xs mt-2 mb-2;
		color: var(--color-text-tertiary);
	}

	.hint-grid {
		@apply grid grid-cols-2 gap-2;
	}

	.hint-preview {
		background-color: var(--color-surface-sunken);
		border: 1px solid var(--color-border);
		@apply relative rounded overflow-hidden mt-2 mb-2 cursor-crosshair;
	}

	.hint-preview img {
		@apply block w-full;
		max-height: 180px;
		object-fit: contain;
	}

	.hint-selection {
		border: 2px solid rgba(59, 130, 246, 0.95);
		background: rgba(59, 130, 246, 0.2);
		@apply absolute pointer-events-none;
	}

	.hint-help {
		@apply text-xs mb-2;
		color: var(--color-text-tertiary);
	}

	.hint-missing {
		@apply text-xs mb-2;
		color: var(--color-text-tertiary);
	}

	.hint-grid label {
		@apply text-xs flex flex-col gap-1;
		color: var(--color-text-secondary);
	}

	.hint-grid input {
		background-color: var(--color-surface-sunken);
		border: 1px solid var(--color-border);
		color: var(--color-text-primary);
		@apply px-2 py-1 rounded text-sm;
	}

	.margin-field {
		@apply col-span-2;
	}
</style>
