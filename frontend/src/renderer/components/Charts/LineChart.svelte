<script>
	import "@carbon/charts/styles.min.css";
	import "carbon-components/css/carbon-components.min.css";
	import { LineChart } from "@carbon/charts-svelte";
	import { ImageIcon } from "svelte-feather-icons";
	import html2canvas from "html2canvas";
	import { element } from "svelte/internal";
	import { chart } from "svelte-apexcharts";
	import { appSettings } from "@util/stores";
	import { getCssVar } from "@util/cssUtils";
	import Button from "@components/Button.svelte";

	export let data = [];
	export let wavelengthArray = Array.from({ length: 36 }, (x, i) => i * 10 + 380);
	export let trueShadowPos;
	export let stack = false;

	// Color palette for stacked curves that works in both themes
	const CURVE_COLORS = [
		"#2563eb", // Blue
		"#dc2626", // Red
		"#16a34a", // Green
		"#9333ea", // Purple
		"#ea580c", // Orange
		"#0891b2", // Cyan
		"#c026d3", // Fuchsia
		"#65a30d", // Lime
	];

	let inputData = [];

	$: if (data.length > 1) {
		const dataDict = [];

		// Clear the graph if we aren't stacking curves
		if (!stack) {
			inputData = [];
		}

		wavelengthArray.forEach((element, i) => {
			dataDict.push({
				x: element,
				y: data[i] * 100,
			});
		});

		const curveIndex = inputData.length % CURVE_COLORS.length;
		inputData.push({
			type: "line",
			name:
				"Spectrum: (" +
				(typeof trueShadowPos.left === "number" ? trueShadowPos.left.toFixed(1) : "0") +
				";" +
				(typeof trueShadowPos.top === "number" ? trueShadowPos.top.toFixed(1) : "0") +
				")",
			data: dataDict,
		});

		// Update options with new data
		options = getOptions();
		options.series = inputData;
		data = [];
	}

	function getOptions() {
		const isDark = $appSettings?.isDarkTheme ?? true;
		const textColorPrimary = getCssVar("--color-text-primary") || (isDark ? "#ffffff" : "#1f2937");
		const textColorSecondary =
			getCssVar("--color-text-secondary") || (isDark ? "#9ca3af" : "#6b7280");
		const gridColor = isDark ? "#6b7280" : "#d1d5db";

		return {
			series: [],
			colors: CURVE_COLORS,
			stroke: {
				show: true,
				curve: "smooth",
				width: 2,
			},
			chart: {
				background: "transparent",
				foreColor: textColorPrimary,
				animations: {
					enabled: false,
				},
				zoom: {
					enabled: false,
				},
				height: "400px",
				width: "100%",
				type: "line",
				toolbar: {
					show: true,
					tools: {
						download: true,
					},
					export: {
						png: {},
						svg: {},
					},
				},
				selection: {
					enabled: false,
				},
			},
			grid: {
				borderColor: gridColor,
				strokeDashArray: 4,
			},
			tooltip: {
				shared: true,
				intersect: false,
				theme: isDark ? "dark" : "light",
				x: {
					show: true,
					formatter: function (val) {
						return val + " nm";
					},
				},
				y: {
					formatter: function (value) {
						if (value === undefined || value === null) return "";
						return `${value.toFixed(2)}%`;
					},
				},
				marker: {
					show: true,
				},
			},
			xaxis: {
				type: "numeric",
				min: 380,
				max: 730,
				tickAmount: 8,
				labels: {
					style: {
						colors: textColorSecondary,
						fontSize: "12px",
					},
					formatter: function (val) {
						return Math.round(val);
					},
				},
				title: {
					text: "Wavelength (nm)",
					style: {
						color: textColorPrimary,
						fontSize: "14px",
						fontWeight: 500,
					},
				},
				axisBorder: {
					color: gridColor,
				},
				axisTicks: {
					color: gridColor,
				},
				crosshairs: {
					stroke: {
						color: textColorSecondary,
					},
				},
			},
			yaxis: {
				showAlways: true,
				decimalsInFloat: 0,
				forceNiceScale: true,
				tickAmount: 6,
				min: 0,
				max: 100,
				labels: {
					style: {
						colors: textColorSecondary,
						fontSize: "12px",
					},
					formatter: function (val) {
						return Math.round(val);
					},
				},
				title: {
					text: "Reflectance (%)",
					style: {
						color: textColorPrimary,
						fontSize: "14px",
						fontWeight: 500,
					},
				},
			},
			legend: {
				show: true,
				showForSingleSeries: true,
				position: "bottom",
				horizontalAlign: "left",
				labels: {
					colors: textColorPrimary,
				},
			},
			title: {
				text: "Estimated Spectrum",
				align: "left",
				style: {
					color: textColorPrimary,
					fontSize: "16px",
					fontWeight: 600,
				},
			},
			markers: {
				size: 0,
				hover: {
					size: 6,
				},
			},
		};
	}

	let options = getOptions();

	// Update options when theme changes
	$: if ($appSettings) {
		options = getOptions();
		if (inputData.length > 0) {
			options.series = inputData;
		}
	}

	const createCSVContent = () => {
		let csvContent = "data:text/csv;charset=utf-8,";
		const delimiter = ",";

		csvContent += "wavelength,";
		inputData.forEach(spectralLine => {
			csvContent += spectralLine.name + delimiter;
		});
		csvContent += "\r\n";

		for (let i = 0; i < wavelengthArray.length; i++) {
			csvContent += wavelengthArray[i].toString() + delimiter;

			inputData.forEach(spectralLine => {
				let filteredArray = spectralLine.data.filter(item => {
					return item.x == wavelengthArray[i];
				});
				csvContent += filteredArray[0].y / 100 + delimiter;
			});

			csvContent += "\r\n";
		}

		return csvContent;
	};

	const downloadCSV = () => {
		let csvContent = createCSVContent();

		var encodedUri = encodeURI(csvContent);
		var link = document.createElement("a");
		link.setAttribute("href", encodedUri);
		link.setAttribute("download", "SpectralLineChart.csv");
		link.click();
	};
</script>

<div class="line-chart" id="EstSpecChart">
	{#key $appSettings?.isDarkTheme}
		<div id="spectral-chart" use:chart={options} />
	{/key}
	<Button variant="secondary" size="sm" onClick={downloadCSV} className="download-btn">
		Download CSV
	</Button>
</div>

<style lang="postcss" global>
	.line-chart {
		@apply relative;
	}
	.line-chart .bx--chart-holder {
		@apply w-auto;
	}
	.line-chart .saveBtn {
		@apply flex w-fit p-1 rounded-lg absolute top-0 right-10 z-50;
	}
	.line-chart .bx--cc--scatter circle {
		fill-opacity: 1;
		r: 6;
		stroke: none;
	}

	.line-chart .bx--cc--line path.line {
		stroke-width: 1%;
	}

	.line-chart .bx--chart-holder.filled,
	.bx--chart-holder.fullscreen {
		@apply bg-transparent;
	}

	.line-chart .bx--chart-holder.filled .bx--cc--chart-wrapper,
	.bx--chart-holder.fullscreen .bx--cc--chart-wrapper {
		@apply bg-transparent;
	}

	.line-chart .bx--chart-holder.fullscreen,
	.bx--chart-holder:-webkit-full-screen {
		width: 0;
		height: 0;
		padding: 0;
	}

	.apexcharts-menu {
		color: var(--color-text-primary) !important;
		background-color: var(--color-surface) !important;
		border: 1px solid var(--color-border);
	}

	.apexcharts-menu-item.exportCSV {
		display: none;
	}

	:global(.download-btn) {
		@apply mt-3;
	}
</style>
