"use strict";

function isMissingProcessError(error) {
	return Boolean(error && (error.code === "ESRCH" || error.errno === "ESRCH"));
}

function terminateBackendProcess(
	loader,
	{ kill = process.kill.bind(process), platform = process.platform, logger = console } = {}
) {
	if (!loader || !loader.pid) {
		return undefined;
	}

	const logFailure = error => {
		if (!isMissingProcessError(error)) {
			logger.log(`Failed to terminate backend process ${loader.pid}: ${error.message ?? error}`);
		}
	};

	if (platform !== "win32") {
		try {
			kill(-loader.pid, "SIGTERM");
			return undefined;
		} catch (error) {
			if (isMissingProcessError(error)) {
				return undefined;
			}

			try {
				kill(loader.pid, "SIGTERM");
			} catch (fallbackError) {
				logFailure(fallbackError);
			}

			return undefined;
		}
	}

	try {
		kill(loader.pid);
	} catch (error) {
		logFailure(error);
	}

	return undefined;
}

function registerAppLifecycleHandlers(app, onWillQuit) {
	let hasRunCleanup = false;

	app.once("will-quit", () => {
		if (hasRunCleanup) {
			return;
		}

		hasRunCleanup = true;
		onWillQuit();
	});

	app.on("window-all-closed", () => {
		app.quit();
	});
}

module.exports = {
	registerAppLifecycleHandlers,
	terminateBackendProcess,
};
