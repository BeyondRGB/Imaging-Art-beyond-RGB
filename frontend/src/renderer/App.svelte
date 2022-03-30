<script lang="ts">
	import { currentPage, appSettings, modal } from "@util/stores";
	// Components
	// import Navbar from "@components/Navbar.svelte";
	import Menu from "@components/Menu.svelte";
	import Page from "@components/Page.svelte";
	// Pages
	import Home from "@pages/Home.svelte";
	import ManagedRgb from "@pages/ManagedRgb.svelte";
	import SpectralOverlay from "@pages/SpectralOverlay.svelte";
	import Process from "@pages/Process.svelte";
	import ProcessOld from "@root/pages/ProcessOld.svelte";
	import Reports from "@pages/Reports.svelte";
	import SpectralPicker from "@pages/SpectralPicker.svelte";
	import Settings from "@pages/Settings.svelte";
	import Demo from "@pages/Demo.svelte";
	import TestConsole from "@components/TestConsole.svelte";

	import {
		HomeIcon,
		SettingsIcon,
		ImageIcon,
		LayersIcon,
		FileTextIcon,
		CrosshairIcon,
		ApertureIcon,
	} from "svelte-feather-icons";

	import { onDestroy } from "svelte";

	const routes: any = {
		Home: {
			text: "Home",
			component: Home,
			icon: HomeIcon,
			isShown: true,
			default: true,
		},
		Process: {
			text: "Process",
			component: Process,
			icon: ApertureIcon,
			isShown: true,
			page: true,
		},
		RGB: {
			text: "Managed RGB",
			component: ManagedRgb,
			icon: ImageIcon,
			isShown: true,
			page: true,
		},
		SpecOverlay: {
			text: "Spectral Overlay",
			component: SpectralOverlay,
			icon: LayersIcon,
			isShown: true,
			page: true,
		},
		Reports: {
			text: "Reports",
			component: Reports,
			icon: FileTextIcon,
			isShown: true,
			page: true,
		},
		SpecPicker: {
			text: "Spectral Picker",
			component: SpectralPicker,
			icon: CrosshairIcon,
			isShown: true,
			page: true,
		},
		Demo: {
			text: "Demo",
			component: Demo,
			icon: SettingsIcon,
			isShown: true,
			default: true,
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

	let pages;
	$: if (pages) {
		let activePages = [];
		Object.keys(routes).map((key) => {
			if (routes[key].page) {
				activePages.push(key);
			}
		});

		let width = pages.scrollWidth;
		let height = pages.scrollHeight;

		if ($appSettings.sideNav) {
			pages.scroll({
				top:
					activePages.findIndex((item) => item === $currentPage) *
					(height / activePages.length),
				left: 0,
				behavior: "smooth",
			});
		} else {
			pages.scroll({
				top: 0,
				left:
					activePages.findIndex((item) => item === $currentPage) *
					(width / activePages.length),
				behavior: "smooth",
			});
		}
	}

	let isOpen = false;
</script>

<main class={theme}>
	<div class="app {theme} {$appSettings.sideNav ? 'sideMenu' : ''}">
		<!-- <Navbar {routes} /> -->

		<Menu icon={SettingsIcon} {routes} />

		<Page {routes} bind:pages />
		<div class={`console ${isOpen ? "open" : ""}`}>
			<div class="testBox">
				<div class="handle" on:click={() => (isOpen = !isOpen)}>
					{isOpen ? ">" : "<"}
				</div>
				<div class="con">
					<TestConsole {isOpen} />
				</div>
			</div>
		</div>
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

		@apply flex flex-col-reverse relative overflow-hidden justify-center items-center;
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

	.console {
		@apply absolute -right-[60%] w-[60%] transition-all duration-300 z-50;
	}
	.open {
		@apply -right-0;
	}
	.handle {
		@apply bg-gray-800 h-12 w-8 absolute bottom-1/2 -left-8 flex justify-center items-center
							text-2xl rounded-l-full border-l-2 border-t-2 border-b-2 border-gray-700;
	}
	.testBox {
		@apply bg-gray-800 w-full h-full relative rounded-l-lg flex justify-center items-center
						border-2 border-gray-700 shadow-2xl;
	}
	.con {
		@apply w-full h-full;
	}

	.menuNavIcon {
		fill: transparent;
		stroke-width: 8%;
	}
</style>
