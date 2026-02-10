import { writable, derived, get } from "svelte/store";

// Theme preference: 'light', 'dark', or 'system' (follow OS)
export type ThemePreference = "light" | "dark" | "system";

// Load settings from localStorage
function loadSettings() {
	if (typeof window !== "undefined") {
		const saved = localStorage.getItem("appSettings");
		if (saved) {
			try {
				const parsed = JSON.parse(saved);
				// Migrate from old boolean isDarkTheme to new themePreference
				if (parsed.themePreference === undefined && parsed.isDarkTheme !== undefined) {
					return {
						themePreference: parsed.isDarkTheme ? "dark" : ("light" as ThemePreference),
						isDarkTheme: parsed.isDarkTheme,
						sideNav: parsed.sideNav ?? true,
					};
				}
				return {
					themePreference: parsed.themePreference ?? ("system" as ThemePreference),
					isDarkTheme: parsed.isDarkTheme ?? false,
					sideNav: parsed.sideNav ?? true,
				};
			} catch (e) {
				console.error("Failed to parse saved settings:", e);
			}
		}
	}
	return { themePreference: "system" as ThemePreference, isDarkTheme: false, sideNav: true };
}

// Stores
export const currentPage = writable(null);
export const appSettings = writable(loadSettings());
export const modal = writable(null);
export const openQualiaTargetType = writable<"calibration" | "verification">("calibration");

// Persist settings to localStorage
if (typeof window !== "undefined") {
	appSettings.subscribe(settings => {
		localStorage.setItem("appSettings", JSON.stringify(settings));
	});
}

export const batchImagesA = writable(["E:\\BeyondRGBPics\\picasso_1_A.ARW"]);
export const batchImagesB = writable(["E:\\BeyondRGBPics\\picasso_1_B.ARW"]);

export const persistentCustomRefData = writable({
	calibration: [],
	verification: [],
});

export const viewState = writable({
	projectKey: null,

	colorManagedImage: { dataURL: "", name: "Waiting..." },
	colorManagedTargetImage: { dataURL: "", name: "Waiting..." },
	reports: {
		calibration: null,
		verification: null,
	},
});

export const serverError = writable(null);

// Page Stores
export const customRefData = writable({
	calibration: null,
	verification: null,
});
export const processState = writable({
	batch: false,
	processType: "Single",
	artImageCount: 1,
	currentTab: 0,
	completedTabs: [false, false, false, false, false, false],
	pipelineComplete: false,
	destDir: "",
	destFileName: "",
	imageFilePaths: [],
	thumbnailID: null as number,
	colorTargetID: null,
	CMID: null,
	CMTID: null,
	whitePatchFilled: false,
	imageThumbnails: {},
	outputImage: { dataURL: "", name: "Waiting..." },
	returnedFromProcessing: false,
	artStacks: [
		{
			id: 1,
			name: "Art 1",
			colorTargetImage: { dataURL: "", filename: "" },
			verificationTargetImage: { dataURL: "", filename: "" },
			colorTarget: {},
			verificationTarget: {},
			sharpenString: "N",
			fields: {
				imageA: [],
				imageB: [],
				targetA: [],
				targetB: [],
				flatfieldA: [],
				flatfieldB: [],
				darkfieldA: [],
				darkfieldB: [],
			},
		},
	],
});

export const batchProcessState = writable({
	artImagesA: [],
	artImagesB: [],
	artImageOutputUrls: [],
	proccessingImages: {
		targetA: [],
		targetB: [],
		flatfieldA: [],
		flatfieldB: [],
		darkfieldA: [],
		darkfieldB: [],
	},
});

export function resetProcess() {
	processState.set({
		batch: false,
		processType: "Single",
		artImageCount: 1,
		currentTab: 0,
		completedTabs: [false, false, false, false, false, false],
		pipelineComplete: false,
		destDir: "",
		destFileName: "",
		imageFilePaths: [],
		thumbnailID: null,
		colorTargetID: null,
		whitePatchFilled: false,
		CMID: null,
		CMTID: null,
		imageThumbnails: {},
		outputImage: { dataURL: "", name: "Waiting..." },
		returnedFromProcessing: false,
		artStacks: [
			{
				id: 1,
				name: "Art 1",
				colorTargetImage: { dataURL: "", filename: "" },
				verificationTargetImage: { dataURL: "", filename: "" },
				colorTarget: {},
				verificationTarget: {},
				sharpenString: "N",
				fields: {
					imageA: [[]],
					imageB: [[]],
					targetA: [],
					targetB: [],
					flatfieldA: [],
					flatfieldB: [],
					darkfieldA: [],
					darkfieldB: [],
				},
			},
		],
	});
}

/**
 * Helper to set a specific tab's completion status.
 * @param tabIndex - The index of the tab to update
 * @param completed - Whether the tab should be marked as completed
 */
export function setTabCompleted(tabIndex: number, completed: boolean = true) {
	processState.update(state => ({
		...state,
		completedTabs: state.completedTabs.map((status, i) => (i === tabIndex ? completed : status)),
	}));
}

/**
 * Helper to clear completion status for all tabs after a given index.
 * @param afterIndex - Tabs with index greater than this will be set to false
 */
export function clearTabsAfter(afterIndex: number) {
	processState.update(state => ({
		...state,
		completedTabs: state.completedTabs.map((status, i) => (i > afterIndex ? false : status)),
	}));
}

// Websocket Stores
export const messageStore = writable([]);
export const messageLog = writable([]);
export const connectionState = writable("Not Connected");

// Centralized binary request tracking for ColorManaged images
// This is handled directly in the WebSocket message handler to avoid race conditions
interface PendingBinaryRequest {
	type: "ColorManaged" | "ColorManagedTarget";
	requestID: number;
	binaryType?: string;
	binaryName?: string;
}

let pendingColorManagedBinary: PendingBinaryRequest | null = null;

// Process incoming WebSocket message for ColorManaged images
// Called directly from the WebSocket listener before messageStore is updated
function handleColorManagedMessage(data: any): boolean {
	// Handle JSON message (binary notification)
	if (typeof data === "string") {
		try {
			const parsed = JSON.parse(data);

			if (parsed["ResponseType"] === "ImageBinary") {
				const currentState = get(processState);
				const requestID = parsed["RequestID"];

				console.log(
					"[WS Handler] ImageBinary received, RequestID:",
					requestID,
					"CMID:",
					currentState.CMID,
					"CMTID:",
					currentState.CMTID
				);

				if (currentState.CMID !== null && requestID === currentState.CMID) {
					console.log("[WS Handler] Matched CMID - waiting for ColorManaged blob");
					pendingColorManagedBinary = {
						type: "ColorManaged",
						requestID: requestID,
						binaryType: parsed["ResponseData"]["type"],
						binaryName: parsed["ResponseData"]["name"],
					};
					return true; // We'll handle this
				} else if (currentState.CMTID !== null && requestID === currentState.CMTID) {
					console.log("[WS Handler] Matched CMTID - waiting for ColorManagedTarget blob");
					pendingColorManagedBinary = {
						type: "ColorManagedTarget",
						requestID: requestID,
						binaryType: parsed["ResponseData"]["type"],
						binaryName: parsed["ResponseData"]["name"],
					};
					return true; // We'll handle this
				}
			}
		} catch (e) {
			// Not JSON, ignore
		}
		return false;
	}

	// Handle Blob message
	if (data instanceof Blob && pendingColorManagedBinary) {
		console.log("[WS Handler] Processing blob for:", pendingColorManagedBinary.type);

		const blob = data.slice(0, data.size, pendingColorManagedBinary.binaryType);
		const tempImg = new Image();
		tempImg.src = URL.createObjectURL(blob);

		if (pendingColorManagedBinary.type === "ColorManaged") {
			viewState.update(state => ({
				...state,
				colorManagedImage: {
					dataURL: tempImg.src,
					name: pendingColorManagedBinary!.binaryName || "",
				},
			}));
			console.log("[WS Handler] Updated colorManagedImage:", tempImg.src.substring(0, 50));
		} else if (pendingColorManagedBinary.type === "ColorManagedTarget") {
			viewState.update(state => ({
				...state,
				colorManagedTargetImage: {
					dataURL: tempImg.src,
					name: pendingColorManagedBinary!.binaryName || "",
				},
			}));
			console.log("[WS Handler] Updated colorManagedTargetImage:", tempImg.src.substring(0, 50));
		}

		// Clear the pending request
		pendingColorManagedBinary = null;
		return true; // We handled this blob
	}

	return false;
}

// Helper function to request the color managed art image (for Image Viewer / SpectralPicker)
export function requestColorManagedImage(projectKey: string) {
	const requestID = Math.floor(Math.random() * 999999);

	processState.update(state => ({
		...state,
		CMID: requestID,
	}));

	const msg = {
		RequestID: requestID,
		RequestType: "ColorManagedImage",
		RequestData: {
			name: projectKey,
			target_requested: false,
		},
	};

	console.log("[Centralized] Requesting ColorManagedImage:", msg);
	sendMessage(JSON.stringify(msg));

	return requestID;
}

// Helper function to request the color managed target image (for Reports)
export function requestColorManagedTargetImage(projectKey: string) {
	const requestID = Math.floor(Math.random() * 999999);

	processState.update(state => ({
		...state,
		CMTID: requestID,
	}));

	const msg = {
		RequestID: requestID,
		RequestType: "ColorManagedImage",
		RequestData: {
			name: projectKey,
			target_requested: true,
		},
	};

	console.log("[Centralized] Requesting ColorManagedTargetImage:", msg);
	sendMessage(JSON.stringify(msg));

	return requestID;
}

// Helper function to clear view state when closing a project
export function clearProjectViewState() {
	viewState.update(state => ({
		...state,
		projectKey: null,
		colorManagedImage: { dataURL: "", name: "Waiting..." },
		colorManagedTargetImage: { dataURL: "", name: "Waiting..." },
		reports: {
			calibration: null,
			verification: null,
		},
	}));
}

// Websocket
// let socket = new WebSocket('ws://localhost:9002');
let socket;

export async function connect() {
	console.log({ "Attempting to Connect": socket });

	// connect or restart
	let ipcResponse = undefined;
	let errorCount = 0;
	do {
		ipcResponse = await window.electron.getPort();

		// if port is undefined, fully restart the backend (this means its crashed)
		if (!ipcResponse || ipcResponse === 3000) {
			errorCount++;
			console.log(`Error occurred when reattaching backend. Error number ${errorCount}`);
			await window.electron.restartBackend();
		}
	} while (!ipcResponse && errorCount < 5);

	// if still disconnected after all that, set to closed
	if (!ipcResponse || ipcResponse === 3000) {
		console.log(
			`Too many errors occured when trying to reconnect backend. Please try again later.`
		);
		connectionState.set("Closed");
		return;
	}

	// set websocket listeners
	console.log(ipcResponse);
	socket = new WebSocket(`ws://localhost:${ipcResponse}`);
	socket.addEventListener("open", function (event) {
		console.log("Connected!");
		connectionState.set("Connected");
	});

	socket.addEventListener("close", function (event) {
		console.log("Closed - Trying again in 15 seconds.");
		connectionState.set("Closed");
		close();
	});

	socket.addEventListener("error", function (event) {
		console.log("Error");
		connectionState.set("Closed - Error");
		socket.close();
	});

	socket.addEventListener("message", function (event) {
		console.log({ RECEIVED: event.data });

		// Handle ColorManaged messages directly before updating messageStore
		// This ensures the state is updated before any reactive statements run
		handleColorManagedMessage(event.data);

		// Still update messageStore for other handlers (reports, thumbnails, etc.)
		messageStore.set([event.data, new Date()]);
	});
}
connect();

export function close() {
	console.log("Closing all websocket listeners");
	socket.close();
}

let prevMessage;
export const sendMessage = message => {
	if (socket.readyState === 1) {
		console.log({ SendMessage: message });
		prevMessage = message;
		socket.send(message);
	}
};

export const resendMessage = () => {
	if (socket.readyState === 1) {
		console.log({ ReSendMessage: prevMessage });
		socket.send(prevMessage);
	}
};
