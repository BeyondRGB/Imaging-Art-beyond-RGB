<script lang="ts">
	import { onDestroy } from "svelte";
	import { currentPage, appSettings, modal, type ThemePreference } from "@util/stores";
	// Components
	import Menu from "@components/Menu.svelte";
	import Page from "@components/Page.svelte";
	// Pages
	import Home from "@pages/Home.svelte";
	import Process from "@pages/Process.svelte";
	import Reports from "@pages/Reports.svelte";
	import SpectralPicker from "@pages/SpectralPicker.svelte";
	import Settings from "@pages/Settings.svelte";

	
	import {
		HomeIcon,
		SettingsIcon,
		FileTextIcon,
		CrosshairIcon,
		ApertureIcon,
	} from "svelte-feather-icons";

	const routes: any = {
		Home: {
			text: "Home",
			component: Home,
			icon: HomeIcon,
			isShown: true,
			default: true,
		},
		Process: {
			text: "Select & Process",
			component: Process,
			icon: ApertureIcon,
			isShown: true,
			page: true,
		},
		SpecPicker: {
			text: "Image Viewer",
			component: SpectralPicker,
			icon: CrosshairIcon,
			isShown: true,
			page: true,
		},

		Reports: {
			text: "Calibration Reports",
			component: Reports,
			icon: FileTextIcon,
			isShown: true,
			page: true,
		},
		Settings: {
			text: "Settings",
			component: Settings,
			icon: SettingsIcon,
			isShown: true,
			default: true,
		},
	};

	currentPage.set("Process");
	setTimeout(() => {
		modal.set("Home");
	}, 0);

	// System theme detection (works on Windows, macOS, and Linux)
	const systemThemeQuery = window.matchMedia("(prefers-color-scheme: dark)");

	// Compute isDarkTheme based on preference
	function computeIsDark(pref: ThemePreference): boolean {
		if (pref === "system") {
			return systemThemeQuery.matches;
		}
		return pref === "dark";
	}

	// Update isDarkTheme in settings when preference or system changes
	function updateDarkTheme() {
		const isDark = computeIsDark($appSettings.themePreference);
		if ($appSettings.isDarkTheme !== isDark) {
			appSettings.update(s => ({ ...s, isDarkTheme: isDark }));
		}
	}

	// Apply on load
	updateDarkTheme();

	// Listen for system theme changes
	function handleSystemThemeChange() {
		if ($appSettings.themePreference === "system") {
			updateDarkTheme();
		}
	}
	systemThemeQuery.addEventListener("change", handleSystemThemeChange);
	onDestroy(() => {
		systemThemeQuery.removeEventListener("change", handleSystemThemeChange);
	});

	// React to preference changes
	$: $appSettings.themePreference, updateDarkTheme();

	// Apply theme to DOM (existing logic, unchanged)
	function applyTheme(isDark: boolean) {
		if (isDark) {
			document.documentElement.classList.add("dark");
			document.body.classList.add("dark");
		} else {
			document.documentElement.classList.remove("dark");
			document.body.classList.remove("dark");
		}
	}

	// Theme Switching Logic - reacts to isDarkTheme changes
	$: isDarkTheme = $appSettings.isDarkTheme ? "dark" : "";
	$: applyTheme($appSettings.isDarkTheme);
</script>

<main class={isDarkTheme}>
	<div class="app {isDarkTheme}" class:sideMenu={$appSettings.sideNav}>
		<Menu {routes} />

		<Page {routes} />
	</div>
</main>

<style global lang="postcss">
	@import "./styles/theme.css";

	@tailwind base;
	@tailwind components;
	@tailwind utilities;
	@tailwind variants;

	html,
	body {
		width: 100%;
		height: 100%;
		margin: 0 auto;
		background-color: var(--color-surface-base);
		color: var(--color-text-primary);
		@apply select-none text-base;
	}
	main {
		height: 100%;
		font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
	}

	.app {
		width: 100%;
		height: 100%;
		margin: 0 auto;

		@apply flex flex-col-reverse relative overflow-hidden justify-center items-center;
	}

	.sideMenu {
		@apply flex flex-row;
	}
</style>
