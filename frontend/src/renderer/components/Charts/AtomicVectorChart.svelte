<script>
	import { onMount, onDestroy } from "svelte";
	import * as echarts from "echarts";
	import { appSettings } from "@root/util/stores";

	let container;
	let chart;

	export let dataAB;
	export let dataLC;

	let isDark = $appSettings?.isDarkTheme;

	let axisColor = isDark ? "#ddd" : "#333";
	let legendColor = isDark ? "#eee" : "#222";

	function makeLegendDotDataUri({ fill = "#f00", ring = "#fff" } = {}) {
		const svg = `
		<svg xmlns="http://www.w3.org/2000/svg" width="32" height="32" viewBox="0 0 32 32">
			<!-- outer colored dot -->
			<circle cx="16" cy="16" r="16" fill="${fill}" />

			<!-- inner ring -->
			<circle cx="16" cy="16" r="12" fill="none" stroke="${ring}" stroke-width="4" />

			<!-- center colored dot again so the ring looks inset -->
			<circle cx="16" cy="16" r="10" fill="${fill}" />
		</svg>
	`.trim();

		return `image://data:image/svg+xml;utf8,${encodeURIComponent(svg)}`;
	}

	const referenceLegendIcon = makeLegendDotDataUri({
		fill: "#f00",
		ring: "#fff",
	});

	const estimateLegendIcon = makeLegendDotDataUri({
		fill: "#f00",
		ring: "#000",
	});

	function buildOption(points) {
		const pointsByGroup = new Map();
		points.forEach(p => {
			pointsByGroup.set(
				p.group,
				points.filter(otherP => otherP.group === p.group)
			);
		});

		// For the lines
		const lineSeries = Array.from(pointsByGroup.entries()).map(([_, ps]) => ({
			type: "line",

			data: ps.map(p => (dataAB ? [p["a"], p["b"]] : [p["c"], p["l"]])),

			symbol: "none",

			lineStyle: {
				width: 4,
				color: ps[0]["color"],
				opacity: 0.7,
			},

			z: 1,
			silent: true,
		}));

		// Compute axes bounds
		const maxVal =
			Math.max(
				...[...points.map(p => (dataAB ? p.a : p.l)), ...points.map(p => (dataAB ? p.b : p.c))]
			) + 5; // plus some padding;

		return {
			grid: {
				left: 60,
				right: 20 + 40,
				top: 40,
				bottom: 60 + 40,
			},

			legend: {
				top: 10,
				right: 10,
				itemWidth: 18,
				itemHeight: 18,

				data: [
					{
						name: "Reference",
						icon: referenceLegendIcon,
					},
					{
						name: "Estimate",
						icon: estimateLegendIcon,
					},
				],
				textStyle: {
					color: legendColor,
				},
			},

			xAxis: {
				type: "value",
				name: dataAB ? "a*" : "C*",
				nameLocation: "middle",
				nameGap: 30,
				min: dataAB ? -maxVal : 0,
				max: maxVal,
				splitLine: { show: true },

				axisLabel: {
					color: axisColor,
				},

				nameTextStyle: {
					color: axisColor,
				},
			},

			yAxis: {
				type: "value",
				name: dataAB ? "b*" : "L*",
				nameLocation: "middle",
				nameGap: 40,
				min: dataAB ? -maxVal : 0,
				max: maxVal,
				splitLine: { show: true },

				axisLabel: {
					color: axisColor,
				},

				nameTextStyle: {
					color: axisColor,
				},
			},

			// scroll zoom + drag zoom
			dataZoom: [
				{ type: "inside", xAxisIndex: 0 },
				{ type: "inside", yAxisIndex: 0 },
				{ type: "slider", xAxisIndex: 0 },
				{ type: "slider", yAxisIndex: 0 },
			],

			tooltip: {
				trigger: "item",

				// The tooltip text, in HTML
				formatter: params => {
					const d = params.data;
					return `
					<div style="min-width:90px">
						<div style="font-weight:700; margin-bottom:6px;">${d.group} ${
						d.actual ? "Estimate" : "Reference "
					}</div>
					</div>
							<div style="display:flex; justify-content: space-between; gap:16px;">
								<span>${dataAB ? "a*" : "C*"} =</span>
								<span style="font-weight:600;">${d.value[0].toFixed(2)}</span>
							</div>
							<div style="display:flex; justify-content: space-between; gap:16px;">
								<span>${dataAB ? "b*" : "L*"} =</span>
								<span style="font-weight:600;">${d.value[1].toFixed(2)}</span>
							</div>
					</div>
					`;
				},
				axisPointer: {
					type: "cross",
					snap: true,
				},
				label: {
					show: true,
				},
			},

			series: [
				...lineSeries,
				// Actual point
				{
					type: "scatter",
					name: "Color point",

					symbolSize: 12,

					data: points.map(p => ({
						value: dataAB ? [p["a"], p["b"]] : [p["c"], p["l"]],
						itemStyle: {
							color: p["color"],
						},
						group: p.group,
						actual: p.actual,
					})),
					z: 2,
				},

				// Border
				{
					type: "scatter",

					symbolSize: 10,

					data: points.map(p => ({
						value: dataAB ? [p["a"], p["b"]] : [p["c"], p["l"]],
						itemStyle: {
							color: p["actual"] ? "#000" : "#fff",
						},
					})),
					z: 3,
					silent: true,
				},

				// Point again
				{
					type: "scatter",

					symbolSize: 7,

					data: points.map(p => ({
						value: dataAB ? [p["a"], p["b"]] : [p["c"], p["l"]],
						itemStyle: {
							color: p["color"],
						},
					})),
					z: 4,
					silent: true,
				},

				// Dummy legend points because reusing the actual ones wouldn't work.
				// The real data points are mixed and funky.

				// Legend: Reference (red fill, white border)
				{
					name: "Reference",
					type: "scatter",
					data: [[NaN, NaN]], // dummy (won't render meaningfully)
					symbolSize: 12,
					itemStyle: {
						color: "#f00",
						borderColor: "#fff",
						borderWidth: 3,
					},
					z: 10,
				},

				// Legend: Image (red fill, black border)
				{
					name: "Estimate",
					type: "scatter",
					data: [[NaN, NaN]],
					symbolSize: 12,
					itemStyle: {
						color: "#f00",
						borderColor: "#000",
						borderWidth: 3,
					},
					z: 10,
				},
			],
		};
	}

	onMount(() => {
		chart = echarts.init(container);

		chart.setOption(buildOption(dataAB || dataLC));

		window.addEventListener("resize", chart.resize);
	});

	// reactive update
	$: if (chart) {
		isDark = $appSettings?.isDarkTheme;

		axisColor = isDark ? "#ddd" : "#333";
		legendColor = isDark ? "#eee" : "#222";

		chart.setOption(buildOption(dataAB || dataLC));
	}

	onDestroy(() => {
		window.removeEventListener("resize", chart.resize);
		chart.dispose();
	});
</script>

<div bind:this={container} class="chart" />

<style>
	.chart {
		width: 80vh;
		height: 80vh;
		flex: 1;
	}
</style>
