<script>
	import { onMount, onDestroy } from "svelte";
	import * as echarts from "echarts";

	let container;
	let chart;

	export let dataAB;
	export let dataLC;

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
			backgroundColor: "#111",

			grid: {
				left: 60,
				right: 20 + 40,
				top: 40,
				bottom: 60 + 40,
				backgroundColor: "#444",
			},

			xAxis: {
				type: "value",
				name: dataAB ? "a*" : "C*",
				nameLocation: "middle",
				nameGap: 30,
				axisLine: {
					lineStyle: {
						color: "#444",
					},
				},
				min: dataAB ? -maxVal : 0,
				max: maxVal,
				splitLine: { show: true },
			},

			yAxis: {
				type: "value",
				name: dataAB ? "b*" : "L*",
				nameLocation: "middle",
				nameGap: 40,
				axisLine: {
					lineStyle: {
						color: "#444",
					},
				},
				min: dataAB ? -maxVal : 0,
				max: maxVal,
				splitLine: { show: true },
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
				formatter: params => {
					const d = params.data;
					return `
					<div style="min-width:90px">
						<div style="font-weight:700; margin-bottom:6px;">${d.group} ${
						d.actual ? "Actual" : "Expected "
					}</div>					</div>
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
					backgroundColor: "#444",
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
		chart.setOption(buildOption(dataAB || dataLC));
	}

	onDestroy(() => {
		window.removeEventListener("resize", chart.resize);
		chart.dispose();
	});
</script>

<!-- <main> -->
<!-- {#key $appSettings?.isDarkTheme} -->
<div bind:this={container} class="chart" />

<!-- {/key} -->
<!-- </main> -->

<style>
	.chart {
		width: 80vh;
		height: 80vh;
		flex: 1;
	}
</style>
