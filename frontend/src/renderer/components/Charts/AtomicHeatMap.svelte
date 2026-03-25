<script>
	import { chart } from "svelte-apexcharts";
	import { createEventDispatcher } from "svelte";
	import { getCssVar } from "@util/cssUtils";
	import { appSettings } from "@util/stores";

	const dispatch = createEventDispatcher();

	export let data;

	// Colorblind option
	export let visionDeficiencyMode = false;

	// Parse the needed data for the heatmap
	function getData() {
		let formattedData = [];
		let tempData = [];
		for (let i = 0; i < data.length; i++) {
			tempData[i] = [];
			for (let j = 0; j < data[i].length; j++) {
				tempData[i][j] = {
					y: data[i][j].toFixed(2),
					x: String.fromCharCode(j + 65),
				};
			}
			formattedData.push({
				name: i,
				data: tempData[i],
			});
		}

		return formattedData;
	}

	const colors = [
		"#2a9f00",
		"#62b000",
		"#8dbb00",
		"#bdc700",
		"#d2b300",
		"#dd9200",
		"#e86c00",
		"#f44100",
		"#f92a00",
		"#f92a00",
		"#f92a00",
		"#f92a00",
	];

	const grayScale = [
		"#ffffff",
		"#d3d3d3",
		"#b5b5b5",
		"#898989",
		"#585858",
		"#4a4a4a",
		"#383838",
		"#313131",
		"#1c1c1c",
		"#171717",
		"#000000",
		"#000000",
	];

	function ranges() {
		const result = [];
		for (let i = 0; i < 11; i++) {
			result.push({
				from: i,
				to: i + 1,
				color: visionDeficiencyMode ? grayScale[i] : colors[i],
				name: i,
			});
		}
		result[result.length - 1].to = Number.MAX_SAFE_INTEGER;
		return result;
	}

	const getOptions = function () {
		// Get theme colors from CSS variables with dark/light mode fallbacks
		const isDark = $appSettings?.isDarkTheme ?? true;
		const textColorSecondary =
			getCssVar("--color-text-secondary") || (isDark ? "#9ca3af" : "#6b7280");

		return {
			series: getData(),
			chart: {
				events: {
					dataPointSelection: function (event, chartContext, config) {
						let yAxisLabel = data.length - config.seriesIndex;
						let xValue = config.w.config.series[config.seriesIndex].data[config.dataPointIndex].x;
						console.log(yAxisLabel);
						console.log(xValue);
						dispatch("datapointselect", { yAxisLabel, xValue });
						// Dispatch a custom event with the selected data point information
						//dispatch('datapointselect', { yAxisLabel, xValue });
					},
				},
				height: "650px",
				width: "650px",
				type: "heatmap",
				toolbar: {
					// Hamburger menu which has exports such as CSV etc.
					// I have had issues displaying this, I believe some unrelated global CSS is causing issues
					show: false,
				},
				selection: {
					enabled: true,
				},
			},
			tooltip: {
				enabled: true,
				theme: isDark ? "dark" : "light",
				y: {
					title: {
						//todo keep????? y axis tooltip
						// Displayed before the value on the tooltip, unnecessary
						formatter: value => "", //{return data?.length-value}
					},
				},
			},
			yaxis: {
				labels: {
					formatter: function (value, info) {
						if (info === data?.length) {
							// This gets hit before we even get to the heatmap and displays a random axisLabel
							return "";
						} else if (isFinite(info)) {
							// This gets hit for each row on the heatmap (displayed as y-axis)
							return data?.length - info;
						} else {
							// This gets hit for each individual square on the heatmap (displayed as tooltip value)
							return value;
						}
					},
					style: {
						colors: [textColorSecondary],
					},
				},
			},
			xaxis: {
				labels: {
					style: {
						//for some reason colors: ["#ffffff", "#ffffff"] would only make the first two labels white??
						//so make an array the length of the x-axis of just "#ffffff"
						colors: Array.from({ length: data?.[0].length }, () => textColorSecondary),
					},
				},
			},
			plotOptions: {
				heatmap: {
					shadeIntensity: 0.5,
					radius: 0,
					useFillColorAsStroke: true,
					// EnableShades ends up shading higher-value greens darker shades, which is not what we want
					enableShades: false,
					colorScale: {
						ranges: ranges(),
					},
				},
			},
			dataLabels: {
				enabled: true,
				style: {
					colors: visionDeficiencyMode ? ["#e60b0b"] : ["#ffffff"],
				},
			},
		};
	};
</script>

<main>
	{#key [visionDeficiencyMode, $appSettings?.isDarkTheme]}
		<div use:chart={getOptions()} />
	{/key}
</main>
