<!-- <script>
	import { chart } from "svelte-apexcharts";
	import { find, forEach } from "lodash";
	import { getCssVar } from "@util/cssUtils";

	export let dataAB;
	export let dataLC;

	const colors = [];
	const annotations = [];
	let data = [];

	const getData = function () {
		forEach(dataAB || dataLC, function (item) {
			let entry = find(data, { series: item.group });
			if (!entry) {
				entry = {
					name: item.group,
					series: item.group,
					data: [],
				};
				data.push(entry);
				colors.push(item.color);
			}

			entry.data.push({
				y: item.b || item.l,
				x: item.a || item.c,
			});
			annotations.push({
				y: item.b || item.l,
				x: item.a || item.c,
				marker: {
					fillColor: item.color,
					strokeWidth: 1,
					size: 3,
					strokeColor: entry.data.length % 2 == 0 ? "#000" : "#FFF",
				},
			});
			if ("B:4" == entry.series) {
				console.log(item);
			}
		});
		return data;
	};

	const getOptions = function () {
		const textColorPrimary = getCssVar("--color-text-primary") || "#ffffff";
		const textColorSecondary = getCssVar("--color-text-secondary") || "#ffffff";

		return {
			series: getData(),
			markers: {
				size: 5,
				colors: colors,
				strokeWidth: 0,
			},
			annotations: {
				points: annotations,
			},
			chart: {
				animations: {
					enabled: false,
				},
				height: "700px",
				width: "700px",
				type: "line",
				stroke: {
					curve: "straight",
				},
				toolbar: {
					// Hamburger menu which has exports such as CSV etc.
					// I have had issues displaying this, I believe some unrelated global CSS is causing issues
					show: false,
				},
				selection: {
					enabled: false,
				},
			},
			tooltip: {
				shared: false,
				intersect: true,
				theme: "dark",
				x: {
					show: false,
				},
				y: {
					formatter: undefined,
					title: {
						formatter: function (seriesName) {
							return seriesName;
						},
					},
				},
				z: {
					show: false,
				},
			},
			legend: {
				show: true,
				labels: {
					colors: [textColorPrimary, textColorPrimary],
				},
				customLegendItems: ["Reference", "Image"],
				markers: {
					size: 5,
					strokeWidth: 2,
					strokeColor: textColorPrimary,
					fillColors: [textColorPrimary, "#000"],
				},
			},
			xaxis: {
				labels: {
					style: {
						colors: textColorSecondary,
					},
				},
				title: {
					text: dataAB ? "a*" : "C*",
					offsetY: 75,
					style: {
						color: textColorPrimary,
					},
				},
			},
			yaxis: {
				labels: {
					style: {
						colors: textColorSecondary,
					},
					formatter: function (value, info) {
						if (isFinite(info)) {
							// This gets hit for each row (displayed as y-axis)
							return value;
						} else if (isFinite(info?.dataPointIndex)) {
							// This gets hit for each individual point on the heatmap (displayed as tooltip value)
							return info?.dataPointIndex % 2 ? "Expected" : "Actual";
						}
					},
				},
				title: {
					text: dataAB ? "b*" : "L*",
					style: {
						color: textColorPrimary,
					},
				},
				decimalsInFloat: 0,
			},
			colors: colors,
		};
	};
</script>

<main>
	<div use:chart={getOptions()} />
</main> -->

<script>
	import { onMount, onDestroy } from "svelte";
	import { Deck, OrthographicView } from "@deck.gl/core";
	import { ScatterplotLayer, LineLayer, TextLayer } from "@deck.gl/layers";

	export let dataAB;
	export let dataLC;

	console.dir(dataLC);

	const parseRGB = rgb => rgb.match(/\d+/g).map(Number);

	const createScatter = data =>
		new ScatterplotLayer({
			id: "scatter",
			data,

			getPosition: d => (dataAB ? [d["a"], d["b"]] : [d["l"], d["c"]]),
			getFillColor: d => parseRGB(d["color"]),

			radiusUnits: "pixels",
			getRadius: 3,

			pickable: true,

			stroked: true,
			getLineColor: d => (d["actual"] ? [0, 0, 0] : [255, 255, 255]),
		});

	const createAxes = bounds => {
		const { minX, maxX, minY, maxY } = bounds;

		return new LineLayer({
			id: "axes",
			data: [
				// X axis
				{
					sourcePosition: [minX, 0],
					targetPosition: [maxX, 0],
				},
				// Y axis
				{
					sourcePosition: [0, minY],
					targetPosition: [0, maxY],
				},
			],

			getSourcePosition: d => d.sourcePosition,
			getTargetPosition: d => d.targetPosition,

			getColor: [180, 180, 180],
			getWidth: 2,
			widthUnits: "pixels",
		});
	};

	const createTicks = bounds => {
		const { minX, maxX, minY, maxY } = bounds;
		const ticks = [];

		const spacing = 100;

		for (let x = minX; x <= maxX; x += spacing) {
			ticks.push({
				position: [x, 0],
				text: `${x}`,
			});
		}

		for (let y = minY; y <= maxY; y += spacing) {
			ticks.push({
				position: [0, y],
				text: `${y}`,
			});
		}

		return new TextLayer({
			id: "ticks",
			data: ticks,

			getPosition: d => d.position,
			getText: d => d.text,

			getSize: 14,
			sizeUnits: "pixels",

			getColor: [220, 220, 220],
			getTextAnchor: "middle",
			getAlignmentBaseline: "top",
		});
	};

	const getBounds = points => {
		const xs = points.map(p => (dataAB ? p["a"] : p["l"]));
		const ys = points.map(p => (dataAB ? p["b"] : p["c"]));

		return {
			minX: Math.min(...xs),
			maxX: Math.max(...xs),
			minY: Math.min(...ys),
			maxY: Math.max(...ys),
		};
	};

	let container;
	let deckInstance;

	onMount(() => {
		deckInstance = new Deck({
			parent: container,
			controller: true,

			views: [new OrthographicView({ id: "ortho" })],

			initialViewState: {
				target: [0, 0, 0],
				zoom: 0,
			},

			layers: [createScatter(dataAB || dataLC)],
		});
	});

	$: if (deckInstance) {
		const bounds = getBounds(dataAB ? dataAB : dataLC);

		deckInstance.setProps({
			layers: [createAxes(bounds), createTicks(bounds), createScatter(dataAB || dataLC)],
		});
	}

	onDestroy(() => {
		deckInstance?.finalize();
	});
</script>

<main>
	<div bind:this={container} />
</main>
