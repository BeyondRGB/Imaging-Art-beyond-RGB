<script>
	import { pageId } from './stores';

	import Navbar from './components/Navbar.svelte';
	import Menu from './components/Menu.svelte';
	import Page from './components/Page.svelte';

	import Launch from './pages/Launch.svelte';
	import ManagedRgb from './pages/ManagedRgb.svelte';
	import SpectralOverlay from './pages/SpectralOverlay.svelte';
	import Preprocess from './pages/Preprocess.svelte';

  import { beer, refresh, comment, codeFork, camera, ban } from 'svelte-awesome/icons';

	const menuOptions = [
		{ text: 'Color Managed RGB Image', component: ManagedRgb, icon: ban, isShown: true },
		{ text: 'Spectral Overlay', component: SpectralOverlay, icon: refresh, isShown: true },
		{ text: 'Home',  component: Launch, icon: camera, isShown: true },
		{ text: 'Settings', component: Launch, icon: comment, isShown: true },
		{ text: 'Preprocessing', component: Preprocess, icon: comment, isShown: false }
	];

	pageId.set(2);

	$: selectedPage = menuOptions[$pageId];
</script>

<main>
	<div class="app">
		<Navbar menuOptions={menuOptions}/>

		<Menu
			icon={beer}
			menuOptions={menuOptions}
			selectedPage={selectedPage.text}
		/>

		<Page selectedPage={selectedPage.component}/>
	</div>	
</main>

<style global lang="postcss">
  @tailwind base;
  @tailwind components;
  @tailwind utilities;

	html, body {
		width: 100%;
		height: 100%;
		margin: 0 auto;
	}
	main {
		height: 100%;
		font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
		background-color: rgb(206, 143, 203);
	}
	:root {
		--nav-color: rgb(255, 197, 197);
		--menu-color: rgb(177, 255, 181);
		--box-color: rgb(201, 200, 255);
		--menu-width: 4rem;
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
			--menu-width: 6rem;
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
		gap: .5rem;

		/* Explicit grid */
		grid-template-areas: 
		"Navbar Navbar Navbar"
		"Menu Box Box"
		"Menu Box Box";

		/* grid-template-rows: repeat(3, auto); */
		grid-template-rows: 7%
												auto 
												auto;
												
		grid-template-columns: var(--menu-width)
													3fr 
													3fr;
		/* grid-template-columns: repeat(3, auto); */
	}

	.start-button {
		@apply bg-blue-500 w-1/2 h-[12%] rounded-lg text-white text-xl active:bg-blue-600 select-none
						flex flex-col justify-center align-middle;
	}
</style>