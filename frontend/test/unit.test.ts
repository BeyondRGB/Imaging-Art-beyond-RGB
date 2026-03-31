import test from "node:test";
import assert from "node:assert/strict";

import { compareTwoStrings, findBestMatch } from "../src/renderer/util/stringCompare.js";
import {
	createHeatmapCSVContent,
	createSpectralCSVContent,
	downloadCSV,
} from "../src/renderer/util/csvExport.js";
import { countFields } from "../src/renderer/util/storesUtil.ts";
import { getCssVar } from "../src/renderer/util/cssUtils.ts";
import { suffixSortImages } from "../src/renderer/util/autoSortStandards.ts";
import {
	connectionState,
	processState,
	resetProcess,
	setTabCompleted,
	clearTabsAfter,
} from "../src/renderer/util/stores.ts";
import { get } from "svelte/store";
import backendLifecycle from "../src/main/backendLifecycle.js";

const { registerAppLifecycleHandlers, terminateBackendProcess } = backendLifecycle;

test("compareTwoStrings returns exact matches and similarity scores", () => {
	assert.equal(compareTwoStrings("beyond rgb", "beyond rgb"), 1);
	assert.equal(compareTwoStrings("a", "ab"), 0);
	assert.ok(compareTwoStrings("flat field", "flatfield") > 0.9);
});

test("findBestMatch picks the highest-rated candidate", () => {
	const result = findBestMatch("dark field", ["target", "darkfield", "white"]);

	assert.equal(result.bestMatch.target, "darkfield");
	assert.equal(result.bestMatchIndex, 1);
	assert.equal(result.ratings.length, 3);
});

test("createSpectralCSVContent builds a wavelength-first CSV table", () => {
	const csv = createSpectralCSVContent(
		[
			{
				name: "Sample A",
				data: [
					{ x: 400, y: 25 },
					{ x: 500, y: 50 },
				],
			},
			{
				name: "Sample B",
				data: [
					{ x: 400, y: 75 },
					{ x: 500, y: 100 },
				],
			},
		],
		[400, 500]
	);

	assert.ok(csv.startsWith("data:text/csv;charset=utf-8,wavelength,Sample A,Sample B,\r\n"));
	assert.ok(csv.includes("400,0.25,0.75,\r\n"));
	assert.ok(csv.includes("500,0.5,1,\r\n"));
});

test("createHeatmapCSVContent flips row order and appends percentile data", () => {
	const csv = createHeatmapCSVContent(
		[
			[1, 2],
			[3, 4],
		],
		12.345
	);

	assert.ok(csv.includes('"1","A","3"'));
	assert.ok(csv.includes('"1","B","4"'));
	assert.ok(csv.includes('"2","A","1"'));
	assert.ok(csv.includes('"Computed 90th Percentile","12.35"'));
});

test("createHeatmapCSVContent omits percentile rows when none is provided", () => {
	const csv = createHeatmapCSVContent([[5, 6]]);

	assert.ok(csv.includes('"1","A","5"'));
	assert.equal(csv.includes("Computed 90th Percentile"), false);
});

test("downloadCSV creates a downloadable anchor and clicks it", () => {
	const originalDocument = globalThis.document;
	let clicked = false;
	const attributes = new Map<string, string>();

	globalThis.document = {
		createElement(tag: string) {
			assert.equal(tag, "a");
			return {
				setAttribute(name: string, value: string) {
					attributes.set(name, value);
				},
				click() {
					clicked = true;
				},
			};
		},
	} as unknown as Document;

	try {
		downloadCSV("data:text/csv;charset=utf-8,a,b", "test.csv");
		assert.equal(attributes.get("download"), "test.csv");
		assert.equal(attributes.get("href"), "data:text/csv;charset=utf-8,a,b");
		assert.equal(clicked, true);
	} finally {
		globalThis.document = originalDocument;
	}
});

test("countFields counts nested art image arrays and single-file roles", () => {
	const imageStack = {
		imageA: [[{ name: "art-a-1" }], []],
		imageB: [[{ name: "art-b-1" }, { name: "art-b-2" }]],
		targetA: [{ name: "target-a" }],
		targetB: [],
		flatfieldA: [{ name: "flat-a" }],
		flatfieldB: [{ name: "flat-b" }],
		darkfieldA: [],
		darkfieldB: [{ name: "dark-b" }],
	};

	assert.equal(countFields(imageStack), 7);
});

test("suffixSortImages sorts two-image sets with target files by suffix", () => {
	const images = [
		{ name: "painting-image_A" },
		{ name: "painting-image_B" },
		{ name: "painting-target_A" },
		{ name: "painting-target_B" },
		{ name: "painting-flatfield_A" },
		{ name: "painting-flatfield_B" },
		{ name: "painting-darkfield_A" },
		{ name: "painting-darkfield_B" },
	];
	const externalStack: Record<string, unknown> = {};

	const remaining = suffixSortImages([...images], externalStack, true);

	assert.deepEqual(remaining, []);
	assert.equal(
		(externalStack.imageA as Array<Array<{ name: string }>>)[0][0].name,
		"painting-image_A"
	);
	assert.equal(
		(externalStack.imageB as Array<Array<{ name: string }>>)[0][0].name,
		"painting-image_B"
	);
	assert.equal((externalStack.targetA as Array<{ name: string }>)[0].name, "painting-target_A");
	assert.equal(
		(externalStack.darkfieldB as Array<{ name: string }>)[0].name,
		"painting-darkfield_B"
	);
});

test("suffixSortImages can sort six-file sets without targets", () => {
	const images = [
		{ name: "painting-image_A" },
		{ name: "painting-image_B" },
		{ name: "painting-flatfield_A" },
		{ name: "painting-flatfield_B" },
		{ name: "painting-darkfield_A" },
		{ name: "painting-darkfield_B" },
	];
	const externalStack: Record<string, unknown> = {};

	const remaining = suffixSortImages([...images], externalStack, false);

	assert.deepEqual(remaining, []);
	assert.deepEqual(externalStack.targetA, []);
	assert.deepEqual(externalStack.targetB, []);
});

test("getCssVar returns the fallback in a non-browser environment", () => {
	assert.equal(getCssVar("--missing-color", "#123456"), "#123456");
});

test("getCssVar reads a CSS custom property when browser globals exist", () => {
	const originalWindow = globalThis.window;
	const originalDocument = globalThis.document;
	const originalGetComputedStyle = globalThis.getComputedStyle;

	globalThis.window = {} as Window & typeof globalThis;
	globalThis.document = {
		documentElement: {},
	} as Document;
	globalThis.getComputedStyle = (() => ({
		getPropertyValue(name: string) {
			return name === "--color-text-primary" ? " #abc123 " : "";
		},
	})) as typeof getComputedStyle;

	try {
		assert.equal(getCssVar("--color-text-primary", "#000000"), "#abc123");
	} finally {
		globalThis.window = originalWindow;
		globalThis.document = originalDocument;
		globalThis.getComputedStyle = originalGetComputedStyle;
	}
});

test("store helpers reset and update process completion state", () => {
	resetProcess();
	processState.update(state => ({
		...state,
		currentTab: 4,
		completedTabs: [true, true, true, true, true, false],
		destFileName: "bad:name",
	}));

	setTabCompleted(5, true);
	clearTabsAfter(2);

	const stateAfterUpdates = get(processState);
	assert.deepEqual(stateAfterUpdates.completedTabs, [true, true, true, false, false, false]);

	resetProcess();
	const resetState = get(processState);
	assert.equal(resetState.currentTab, 0);
	assert.equal(resetState.destFileName, "");
	assert.deepEqual(resetState.completedTabs, [false, false, false, false, false, false]);
});

test("connectionState falls back to closed when Electron APIs are unavailable", () => {
	assert.equal(get(connectionState), "Closed");
});

test("terminateBackendProcess ignores missing loaders", () => {
	assert.equal(terminateBackendProcess(undefined), undefined);
	assert.equal(terminateBackendProcess(null), undefined);
	assert.equal(terminateBackendProcess({}), undefined);
});

test("terminateBackendProcess kills detached POSIX process groups first", () => {
	const calls: Array<Array<number | string>> = [];

	terminateBackendProcess(
		{ pid: 321 },
		{
			platform: "darwin",
			kill(pid: number, signal?: string) {
				calls.push(signal ? [pid, signal] : [pid]);
			},
		}
	);

	assert.deepEqual(calls, [[-321, "SIGTERM"]]);
});

test("terminateBackendProcess falls back to direct pid kill without crashing", () => {
	const calls: Array<Array<number | string>> = [];
	const logs: string[] = [];

	terminateBackendProcess(
		{ pid: 654 },
		{
			platform: "darwin",
			logger: {
				log(message: string) {
					logs.push(message);
				},
			},
			kill(pid: number, signal?: string) {
				calls.push(signal ? [pid, signal] : [pid]);
				if (pid < 0) {
					const error = new Error("group missing") as NodeJS.ErrnoException;
					error.code = "EPERM";
					throw error;
				}
				if (pid > 0) {
					const error = new Error("process missing") as NodeJS.ErrnoException;
					error.code = "ESRCH";
					throw error;
				}
			},
		}
	);

	assert.deepEqual(calls, [
		[-654, "SIGTERM"],
		[654, "SIGTERM"],
	]);
	assert.deepEqual(logs, []);
});

test("terminateBackendProcess logs unexpected Windows kill failures", () => {
	const logs: string[] = [];

	terminateBackendProcess(
		{ pid: 777 },
		{
			platform: "win32",
			logger: {
				log(message: string) {
					logs.push(message);
				},
			},
			kill() {
				const error = new Error("permission denied") as NodeJS.ErrnoException;
				error.code = "EPERM";
				throw error;
			},
		}
	);

	assert.equal(logs.length, 1);
	assert.match(logs[0], /Failed to terminate backend process 777/);
});

test("registerAppLifecycleHandlers wires app quit behavior", () => {
	const events = new Map<string, Function>();
	let quitCalls = 0;
	let cleanupCalls = 0;

	registerAppLifecycleHandlers(
		{
			on(eventName: string, handler: Function) {
				events.set(`on:${eventName}`, handler);
			},
			once(eventName: string, handler: Function) {
				events.set(`once:${eventName}`, handler);
			},
			quit() {
				quitCalls += 1;
			},
		},
		() => {
			cleanupCalls += 1;
		}
	);

	events.get("on:window-all-closed")?.();
	events.get("once:will-quit")?.();
	events.get("once:will-quit")?.();

	assert.equal(quitCalls, 1);
	assert.equal(cleanupCalls, 1);
});
