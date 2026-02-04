<script>
	import "@carbon/charts/styles.min.css";
	import "carbon-components/css/carbon-components.min.css";
	import { ScatterChart } from "@carbon/charts-svelte";
	import { lab2rgb } from "@util/lab2rgb";

	export let data;
	export let matrix = "CM";

	$: if (data?.["matrix_values"]) {
		dataLL = [];
		colors = {};
		let chartData = [];
		data["matrix_values"]
			.find(ele => ele.name === `${matrix} L*_camera`)
			["data"].map((row, i) => {
				row.map((colData, k) => {
					chartData = [
						...chartData,
						{
							col: String.fromCharCode(k + 65),
							row: i + 1,
							lcam: data["matrix_values"].find(ele => ele.name === `${matrix} L*_camera`)["data"][
								i
							][k],
							lref: data["matrix_values"].find(ele => ele.name === `${matrix} L*_ref`)["data"][i][
								k
							],
						},
					];
				});
			});

		chartData.map(value => {
			let rgb = lab2rgb([(value.lcam + value.lref) / 2, 0, 0]);
			dataLL.push({
				group: `${value.col}:${value.row}`,
				lref: value.lref,
				lcam: value.lcam,
			});
			colors[`${value.col}:${value.row}`] = `rgb(${rgb[0]}, ${rgb[1]}, ${rgb[2]})`;
		});
	}

	let dataLL = [];
	let colors = {};
</script>

<div class="liner-chart">
	<ScatterChart
		data={dataLL}
		options={{
			title: "L*ref vs L*cam",
			axes: {
				bottom: {
					title: "L*ref",
					mapsTo: "lref",
					ticks: {
						values: [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100],
					},
					domain: [0, 100],
				},
				left: {
					title: "L*cam",
					mapsTo: "lcam",
					ticks: {
						values: [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100],
					},
					domain: [0, 100],
				},
			},
			legend: {
				enabled: false,
			},
			toolbar: {
				enabled: true,
				// controls: [
				//   { type: "Show as table" },
				//   { type: "Make Fullscreen" },
				//   { type: "Zoom out" },
				// ],
				numberOfIcons: 1,
			},
			height: "700px",
			resizable: true,
			color: {
				scale: colors,
			},
		}}
	/>
</div>

<!-- <div class="heatmap-number-grid">
  {#each valueGrid.reverse() as row, i}
    {#each row as col, i}
      <p class="heatmap-value">
        {col.value}
      </p>
    {/each}
  {/each}
</div> -->
<style lang="postcss" global>
	.liner-chart {
		@apply w-full flex justify-center;
	}
	.liner-chart .bx--chart-holder {
		@apply aspect-square w-auto;
	}
	.liner-chart .bx--cc--scatter circle {
		fill-opacity: 1;
		r: 7;
	}
	/* Remove hardcoded stroke black to allow theme support if needed, 
     or strictly keep it if data visibility on light background requires it.
     However, since this is points on a chart, adjusting stroke based on theme 
     might be better handled via CSS variables if we wanted fully dynamic. 
     For now, leaving as requested or modifying to variable?
     User asked for light/dark compliance. Black stroke on dark theme might be hard to see if fill is dark.
     Let's use a variable for stroke. */
	.liner-chart .bx--cc--scatter circle {
		stroke: var(--color-text-primary) !important;
	}

	/* Carbon Charts overrides for theming */
	.liner-chart .bx--cc--axes text {
		fill: var(--color-text-secondary) !important;
	}
	.liner-chart .bx--cc--axes .axis-title {
		fill: var(--color-text-primary) !important;
	}
	.liner-chart .bx--cc--grid line {
		stroke: var(--color-border) !important;
	}
	.liner-chart .bx--chart-title {
		fill: var(--color-text-primary) !important;
	}
</style>
