<script>
	import { currentPage, appSettings } from "@util/stores";
	// Components
	import Navbar from "@components/Navbar.svelte";
	import Menu from "@components/Menu.svelte";
	import Page from "@components/Page.svelte";
	// Pages
	import Home from "@pages/Home.svelte";
	import ManagedRgb from "@pages/ManagedRgb.svelte";
	import SpectralOverlay from "@pages/SpectralOverlay.svelte";
	import Preprocess from "@pages/Preprocess.svelte";
	import ColorTarget from "@pages/ColorTarget.svelte";
	import Process from "@pages/Process.svelte";
	import Reports from "@pages/Reports.svelte";
	import SpectralPicker from "@pages/SpectralPicker.svelte";
	import Settings from "@pages/Settings.svelte";

	import {
		photo,
		fileImageO,
		home,
		cog,
		github,
		fileText,
		eyedropper,
	} from "svelte-awesome/icons";

	const routes = {
		Home: {
			text: "Home",
			component: Home,
			icon: home,
			isShown: true,
			default: true,
		},
		RGB: {
			text: "Color Managed RGB Image",
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
		Settings: {
			text: "Settings",
			component: Settings,
			icon: cog,
			isShown: true,
			default: true,
		},
		Preprocessing: {
			text: "Preprocessing",
			component: Preprocess,
			icon: home,
			isShown: false,
		},
		ColorTarget: {
			text: "Color Target",
			component: ColorTarget,
			icon: home,
			isShown: false,
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
	darkThemeMq.addEventListener("change", (e) => {
		appSettings.set(e.matches);
	});
	// $: appSettings.set(window.matchMedia("(prefers-color-scheme: dark)").matches);

	$: selectedPage = routes[$currentPage];
	$: theme = $appSettings ? "dark" : "";
	$: if (theme !== "") {
		console.log("Theme Change");
		document.documentElement.classList.add(theme);
		document.body.classList.add(theme);
	} else {
		document.documentElement.classList.remove("dark");
		document.body.classList.remove("dark");
	}
</script>

<main class={theme}>
	<div class="app">
		<Navbar {routes} />

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
		@apply bg-gray-50 dark:bg-gray-700 dark:border-gray-600 border-red-600 select-none;
	}
	main {
		height: 100%;
		font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto,
			Helvetica, Arial, sans-serif;
	}
	:root {
		--icon-mr: 80%;
	}

	@media (min-width: 900px) {
		:root {
			--icon-mr: 90%;
		}
	}

	.app {
		width: 100%;
		height: 100%;
		margin: 0 auto;

		display: grid;
		gap: 0rem;

		/* Explicit grid */
		grid-template-areas:
			"Navbar Navbar Navbar"
			"Menu Box Box"
			"Menu Box Box";

		/* grid-template-rows: repeat(3, auto); */
		grid-template-rows:
			7%
			auto
			auto;

		grid-template-columns:
			var(--menu-width)
			3fr
			3fr;
		/* grid-template-columns: repeat(3, auto); */
	}

	button {
		@apply hover:bg-blue-600 bg-blue-400 text-gray-50 rounded-md self-center text-lg px-4 py-2
						transition-all hover:rounded-lg dark:bg-blue-700 dark:hover:bg-blue-600 dark:hover:text-gray-800
						active:scale-95 shadow-md;
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
		@apply bg-gray-600;
	}

	/* Handle on hover */
	::-webkit-scrollbar-thumb:hover {
		@apply bg-gray-500;
	}
</style>
