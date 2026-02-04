<script lang="ts">
	import OpenSeadragon from "openseadragon";
	import { onDestroy, onMount } from "svelte";

	let viewer: any = null;
	let viewerReady: boolean = false;
	let imageUrl: string = "";
	export let srcUrl: string = "";
	export let identifier = "unique-identifier";
	let imageId = identifier + "-seadragon-viewer";

	const createViewer = () => {
		viewer = OpenSeadragon({
			id: imageId,
			prefixUrl: "/openseadragon/images/",
			immediateRender: true,
			preload: true,
			showNavigator: false,
			minZoomLevel: 0.5,
			useCanvas: true,
			showZoomControl: false,
			showHomeControl: false,
			showFullPageControl: false,
			preserveImageSizeOnResize: true,
			maxZoomPixelRatio: 30,
			zoomPerScroll: 1.15,
			visibilityRatio: 1,
			animationTime: 0.4,
		});

		viewer.addHandler("zoom", handleZoom);

		// Mark viewer as ready and trigger any pending image load
		viewerReady = true;
		console.log(`[ImageViewer ${identifier}] Viewer created and ready`);

		// If srcUrl was set before viewer was ready, open it now
		if (srcUrl && srcUrl.length > 0 && srcUrl !== imageUrl) {
			openImage(srcUrl);
		}
	};

	const destroyViewer = () => {
		if (viewer) {
			viewer.destroy();
			viewer = null;
			viewerReady = false;
			console.log(`[ImageViewer ${identifier}] Viewer destroyed`);
		}
	};

	function openImage(url: string) {
		if (!viewer || !viewerReady) {
			console.log(`[ImageViewer ${identifier}] Cannot open image, viewer not ready`);
			return;
		}

		console.log(`[ImageViewer ${identifier}] Opening image:`, url.substring(0, 50) + "...");
		let temp = new Image();
		temp.src = url;
		imageUrl = temp.src;

		try {
			viewer.open({
				type: "image",
				url: imageUrl,
			});
		} catch (e) {
			console.error("OpenSeadragon Error:", e);
		}
	}

	function closeImage() {
		if (viewer && viewer.isOpen()) {
			console.log(`[ImageViewer ${identifier}] Closing image`);
			viewer.close();
			imageUrl = "";
		}
	}

	onMount(() => {
		console.log(
			`[ImageViewer ${identifier}] Mount, srcUrl:`,
			srcUrl ? srcUrl.substring(0, 50) + "..." : "(empty)"
		);
		createViewer();
	});

	onDestroy(() => {
		console.log(`[ImageViewer ${identifier}] Destroy`);
		destroyViewer();
	});

	// Debug: log srcUrl changes
	$: console.log(
		`[ImageViewer ${identifier}] srcUrl prop changed:`,
		srcUrl ? srcUrl.substring(0, 50) + "..." : "(empty)"
	);

	// React to srcUrl changes
	$: {
		if (viewerReady) {
			if (srcUrl && srcUrl.length > 0) {
				if (srcUrl !== imageUrl) {
					openImage(srcUrl);
				}
			} else {
				closeImage();
			}
		}
	}

	function handleZoom(e) {
		if (e.zoom > 10) {
			viewer.drawer.setImageSmoothingEnabled(false);
		} else {
			viewer.drawer.setImageSmoothingEnabled(true);
		}
	}
</script>

<main>
	<div class="image-seadragon-viewer" id={imageId} />
</main>

<style lang="postcss">
	main {
		background-color: var(--color-surface-base);
		border: 1px solid var(--color-border);
		@apply w-full h-full aspect-[3/2] shadow-lg;
	}
	.image-seadragon-viewer {
		@apply h-full w-full;
	}
	.load {
		@apply absolute left-1/2 bottom-1/2;
	}
</style>
