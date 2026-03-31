import { spawn } from "node:child_process";

const isWindows = process.platform === "win32";
const serverUrl = "http://127.0.0.1:3000";
const waitTimeoutMs = 30000;
const pollIntervalMs = 500;
const forwardedArgs = process.argv.slice(2).join(" ");

function spawnCommand(command) {
	return spawn(command, {
		shell: true,
		stdio: "inherit",
		detached: !isWindows,
	});
}

async function waitForServer(url, timeoutMs) {
	const start = Date.now();

	while (Date.now() - start < timeoutMs) {
		try {
			const response = await fetch(url);
			if (response.ok) {
				return;
			}
		} catch (_) {
			// Keep polling until the dev server is ready.
		}

		await new Promise(resolve => setTimeout(resolve, pollIntervalMs));
	}

	throw new Error(`Timed out waiting for ${url}`);
}

async function terminateServer(serverProcess) {
	if (!serverProcess.pid) {
		return;
	}

	if (isWindows) {
		await new Promise(resolve => {
			const killer = spawn("taskkill", ["/pid", String(serverProcess.pid), "/T", "/F"], {
				stdio: "ignore",
			});
			killer.on("exit", () => resolve());
			killer.on("error", () => resolve());
		});
		return;
	}

	try {
		process.kill(-serverProcess.pid, "SIGTERM");
	} catch (_) {
		// The process may have already exited.
	}
}

async function main() {
	const serverProcess = spawnCommand("npm run svelte-dev");
	let shuttingDown = false;

	const cleanup = async () => {
		if (shuttingDown) {
			return;
		}
		shuttingDown = true;
		await terminateServer(serverProcess);
	};

	process.on("SIGINT", async () => {
		await cleanup();
		process.exit(130);
	});

	process.on("SIGTERM", async () => {
		await cleanup();
		process.exit(143);
	});

	try {
		await waitForServer(serverUrl, waitTimeoutMs);

		const testExitCode = await new Promise((resolve, reject) => {
			const testCommand = forwardedArgs
				? `npm run test:e2e:runner -- ${forwardedArgs}`
				: "npm run test:e2e:runner";
			const testProcess = spawnCommand(testCommand);
			testProcess.on("exit", code => resolve(code ?? 1));
			testProcess.on("error", reject);
		});

		await cleanup();
		process.exit(testExitCode);
	} catch (error) {
		await cleanup();
		console.error(error instanceof Error ? error.message : error);
		process.exit(1);
	}
}

await main();
