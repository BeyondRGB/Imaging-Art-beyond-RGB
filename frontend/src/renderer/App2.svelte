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

	currentPage.set("SpecPicker");
	setTimeout(() => {
		modal.set("Home");
	}, 0);

	appSettings.set({
		theme: true,
		sideNav: $appSettings.sideNav,
	});
	let theme = "dark";

	document.documentElement.classList.add(theme);
	document.body.classList.add(theme);

	// --- Theme Switching Logic (Disabled as light theme is incomplete) ---
	// $: theme = $appSettings.theme ? "dark" : "";
	// $: if (theme !== "") {
	// 	console.log("Theme Change");
	// 	document.documentElement.classList.add(theme);
	// 	document.body.classList.add(theme);
	// } else {
	// 	document.documentElement.classList.remove("dark");
	// 	document.body.classList.remove("dark");
	// }

	// const darkThemeMq = window.matchMedia("(prefers-color-scheme: dark)");

	// darkThemeMq.addEventListener("change", (e) => {
	// 	appSettings.set({ theme: e.matches, sideNav: $appSettings.sideNav });
	// });

	// onDestroy(() => {
	// 	darkThemeMq.removeEventListener("change", (e) => {
	// 		appSettings.set({ theme: e.matches, sideNav: $appSettings.sideNav });
	// 	});
	// });
	// ------
	let pages;
	$: if (pages) {
		let activePages = [];
		Object.keys(routes).map((key) => {
			if (routes[key].page && !routes[key].disabled) {
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
</script>

<main class={theme}>
	<div class="app {theme}" class:sideMenu={$appSettings.sideNav}>
		<Menu {routes} />

		<Page {routes} bind:pages />
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
						dark:text-gray-50 text-base;
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
</style>
