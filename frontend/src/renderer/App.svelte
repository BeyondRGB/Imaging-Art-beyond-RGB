<script lang="ts">
	import { currentPage, appSettings, modal } from "@util/stores";
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

	// Initialize theme from settings (default to LIGHT mode)
	if ($appSettings.theme === undefined) {
		appSettings.set({
			theme: false,  // false = light mode, true = dark mode
			sideNav: $appSettings.sideNav
		});
	}

	// Apply theme on initial load
	function applyTheme(isDark: boolean) {
		if (isDark) {
			document.documentElement.classList.add("dark");
			document.body.classList.add("dark");
		} else {
			document.documentElement.classList.remove("dark");
			document.body.classList.remove("dark");
		}
	}

	// Apply theme immediately on load
	applyTheme($appSettings.theme);

	// Theme Switching Logic - Now enabled with centralized theme system
	$: theme = $appSettings.theme ? "dark" : "";
	$: if (theme !== "") {
		console.log("Theme Change: Dark Mode");
		applyTheme(true);
	} else {
		console.log("Theme Change: Light Mode");
		applyTheme(false);
	}

	// Optional: Auto-detect system theme preference
	// const darkThemeMq = window.matchMedia("(prefers-color-scheme: dark)");
	// darkThemeMq.addEventListener("change", (e) => {
	// 	appSettings.set({ theme: e.matches, sideNav: $appSettings.sideNav });
	// });
	// onDestroy(() => {
	// 	darkThemeMq.removeEventListener("change", (e) => {
	// 		appSettings.set({ theme: e.matches, sideNav: $appSettings.sideNav });
	// 	});
	// });
</script>

<main class={theme}>
	<div class="app {theme}" class:sideMenu={$appSettings.sideNav}>
		<Menu {routes} />

		<Page {routes} />
	</div>
</main>

<style global lang="postcss">
	@import './styles/theme.css';
	
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
		font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto,
			Helvetica, Arial, sans-serif;
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
