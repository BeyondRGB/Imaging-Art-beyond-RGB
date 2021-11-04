<script>
	import { currentPage } from "./stores";
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
			component: Home,
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

	$: selectedPage = routes[$currentPage];
</script>

<main>
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
	html,
	body {
		width: 100%;
		height: 100%;
		margin: 0 auto;
		background-color: rgb(249, 250, 251);
		@apply overflow-hidden;
	}
	main {
		height: 100%;
		font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto,
			Helvetica, Arial, sans-serif;
	}
	:root {
		--nav-color: rgb(243, 244, 246);
		--menu-color: rgb(209, 213, 219);
		--box-color: rgb(249, 250, 251);
		--menu-width: 6%;
		--menu-width-exp: 20rem;
		--icon-mr: 80%;
	}

	@media (min-width: 900px) {
		:root {
			--icon-mr: 90%;
		}
	}

	@media (min-width: 1200px) {
		:root {
			--menu-width: 5%;
		}
	}

	@media (min-width: 1500px) {
		:root {
			--menu-width: 5%;
			--menu-width-exp: 21%;
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

	.start-button {
		@apply bg-blue-500 w-1/2 h-[12%] rounded-lg text-white text-xl active:bg-blue-600 select-none
						flex flex-col justify-center align-middle;
	}
</style>
