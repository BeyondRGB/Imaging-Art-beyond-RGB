<script lang="ts">
	import { currentPage, appSettings } from "@util/stores";
	// Components
	// import Navbar from "@components/Navbar.svelte";
	import Menu from "@components/Menu.svelte";
	import Page from "@components/Page.svelte";
	// Pages
	import Home from "@pages/Home.svelte";
	import ManagedRgb from "@pages/ManagedRgb.svelte";
	import SpectralOverlay from "@pages/SpectralOverlay.svelte";
	import Preprocess from "@pages/Preprocess.svelte";
	import Process from "@pages/Process.svelte";
	import Reports from "@pages/Reports.svelte";
	import SpectralPicker from "@pages/SpectralPicker.svelte";
	import Settings from "@pages/Settings.svelte";
	import Demo from "@pages/Demo.svelte";

	import {
		photo,
		fileImageO,
		home,
		cog,
		github,
		fileText,
		eyedropper,
		infoCircle,
		gears,
	} from "svelte-awesome/icons";
	import { onDestroy } from "svelte";

	const routes: any = {
		Home: {
			text: "Home",
			component: Home,
			icon: home,
			isShown: true,
			default: true,
		},
		Preprocessing: {
			text: "Preprocessing",
			component: Preprocess,
			icon: gears,
			isShown: true,
		},
		RGB: {
			text: "Managed RGB",
			component: ManagedRgb,
			icon: photo,
			isShown: true,
		},
		SpecOverlay: {
			text: "Spectral Overlay",
			component: SpectralOverlay,
			icon: fileImageO,
			isShown: true,
		},
		Reports: {
			text: "Reports",
			component: Reports,
			icon: fileText,
			isShown: true,
		},
		SpecPicker: {
			text: "Spectral Picker",
			component: SpectralPicker,
			icon: eyedropper,
			isShown: true,
		},
		Demo: {
			text: "Demo",
			component: Demo,
			icon: infoCircle,
			isShown: true,
			default: true,
		},
		Settings: {
			text: "Settings",
			component: Settings,
			icon: cog,
			isShown: true,
			default: true,
		},

		Process: {
			text: "Process",
			component: Process,
			icon: home,
			isShown: false,
		},
	};

	currentPage.set("Home");

	const darkThemeMq = window.matchMedia("(prefers-color-scheme: dark)");
	appSettings.set({
		theme: darkThemeMq.matches,
		sideNav: $appSettings.sideNav,
	});
	darkThemeMq.addEventListener("change", (e) => {
		appSettings.set({ theme: e.matches, sideNav: $appSettings.sideNav });
	});

	$: selectedPage = routes[$currentPage];
	$: theme = $appSettings.theme ? "dark" : "";
	$: if (theme !== "") {
		console.log("Theme Change");
		document.documentElement.classList.add(theme);
		document.body.classList.add(theme);
	} else {
		document.documentElement.classList.remove("dark");
		document.body.classList.remove("dark");
	}

	onDestroy(() => {
		darkThemeMq.removeEventListener("change", (e) => {
			appSettings.set({ theme: e.matches, sideNav: $appSettings.sideNav });
		});
	});
</script>

<main class={theme}>
	<div class="app {theme} {$appSettings.sideNav ? 'sideMenu' : ''}">
		<!-- <Navbar {routes} /> -->

		<Menu icon={github} {routes} />

		<Page selectedPage={selectedPage.component} />
	</div>
</main>

<style global lang="postcss">
	@tailwind base;
	@tailwind components;
	@tailwind utilities;
	@tailwind variants;
	html,
	body {
		width: 100%;
		height: 100%;
		margin: 0 auto;
		@apply bg-white dark:bg-gray-800 dark:border-gray-600 border-red-600 select-none
						dark:text-gray-50;
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

		@apply flex flex-col-reverse;
	}

	.sideMenu {
		@apply flex flex-row;
	}

	button {
		@apply hover:bg-blue-200 bg-gray-100 text-gray-900 dark:text-white rounded-lg 
						self-center text-base px-3 py-1 transition-all dark:bg-gray-600 
						dark:hover:bg-blue-500 dark:hover:text-white active:scale-95 shadow-sm 
						dark:ring-gray-500 ring-1 ring-gray-300 dark:focus:ring-blue-500 focus:ring-2 
						focus:ring-blue-300 duration-300;
	}

	/* width */
	::-webkit-scrollbar {
		@apply relative transition-all w-1;
	}

	/* Track */
	::-webkit-scrollbar-track {
		@apply bg-transparent;
	}

	/* Handle */
	::-webkit-scrollbar-thumb {
		@apply bg-gray-600 rounded-full;
	}

	/* Handle on hover */
	::-webkit-scrollbar-thumb:hover {
		@apply bg-gray-500;
	}
</style>
