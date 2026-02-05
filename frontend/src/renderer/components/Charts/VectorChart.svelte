<script>
	// import "@carbon/charts/styles.min.css";
	// import "carbon-components/css/carbon-components.min.css";
	import AtomicVectorChart from "@components/Charts/AtomicVectorChart.svelte";
	import { lab2rgb } from "@util/lab2rgb";

	export let data;
	export let matrix = "CM";

	// If this is the AB chart, otherwise it is LC
	export let ab = false;

	let dataLC = [];
	let dataAB = [];

	const addVectorPoint = function (value, actual) {
		const rgb = lab2rgb([value.l, value.a, value.b]);
		const color = `rgb(${Math.round(rgb[0])}, ${Math.round(rgb[1])}, ${Math.round(rgb[2])})`;
		dataAB.push({
			group: `${value.col}:${value.row}`,
			a: value.a,
			b: value.b,
			color,
		});
		console.log();

		dataLC.push({
			group: `${value.col}:${value.row}`,
			l: value.l,
			c: Math.sqrt(value.a * value.a + value.b * value.b),
			color,
		});
	};

	$: if (data?.["matrix_values"]) {
		let chartData = [];
		let refData = [];
		data["matrix_values"]
			.find(ele => ele.name === `${matrix} L*_camera`)
			["data"].map((row, i) => {
				row.map((colData, k) => {
					chartData = [
						...chartData,
						{
							col: String.fromCharCode(k + 65),
							row: i + 1,
							l: data["matrix_values"].find(ele => ele.name === `${matrix} L*_camera`)["data"][i][
								k
							],
							a: data["matrix_values"].find(ele => ele.name === `${matrix} a*_camera`)["data"][i][
								k
							],
							b: data["matrix_values"].find(ele => ele.name === `${matrix} b*_camera`)["data"][i][
								k
							],
						},
					];
				});
			});

		data["matrix_values"]
			.find(ele => ele.name === `${matrix} L*_ref`)
			["data"].map((row, i) => {
				row.map((colData, k) => {
					refData = [
						...refData,
						{
							col: String.fromCharCode(k + 65),
							row: i + 1,
							l: data["matrix_values"].find(ele => ele.name === `${matrix} L*_ref`)["data"][i][k],
							a: data["matrix_values"].find(ele => ele.name === `${matrix} a*_ref`)["data"][i][k],
							b: data["matrix_values"].find(ele => ele.name === `${matrix} b*_ref`)["data"][i][k],
						},
					];
				});
			});

		chartData.map(value => {
			addVectorPoint(value, true);
		});

		refData.map(value => {
			addVectorPoint(value, false);
		});
	}
</script>

<!-- <div>I'm a Vector Chart</div> -->
<!-- <div bind:this={container} class="scatter-charts" /> -->

<div class="scatter-charts">
	{#if dataAB?.length > 1 && ab}
		<div class="chart-title">a* vs b*</div>
		<AtomicVectorChart {dataAB} />
	{/if}
	{#if dataLC?.length > 1 && !ab}
		<div class="chart-title">L* vs C*</div>
		<AtomicVectorChart {dataLC} />
	{/if}
</div>

<style lang="postcss">
	.scatter-charts {
		@apply relative w-full h-[400px];
	}

	.chart-title {
		color: var(--color-text-primary);
		@apply text-lg font-semibold mb-2;
	}
</style>
