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
	let currentData = [];

	// Distinct colors for estimated vs reference spectrum that work in both themes
	const ESTIMATED_COLOR = "#2563eb"; // Blue
	const REFERENCE_COLOR = "#dc2626"; // Red

	let inputData = [];

	$: if (data.length > 1) {
		inputData = [];

		// Add data for both estimated and reference spectrum
		data.forEach((dataset, j) => {
			const dataDict = [];
			var namer;
			wavelengthArray.forEach((element, i) => {
				dataDict.push({
					x: element,
					y: dataset[i] * 100,
				});
			});
			if (j === 0) {
				namer =
					"Estimated Spectrum: (" + trueShadowPos.left + ";" + trueShadowPos.top.toFixed(1) + ")";
			} else {
				namer =
					"Reference Spectrum: (" + trueShadowPos.left + ";" + trueShadowPos.top.toFixed(1) + ")";
			}
			inputData.push({
				type: "line",
				name: namer,
				data: dataDict,
			});
		});

		// Update chart with new data and colors
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
			colors: [ESTIMATED_COLOR, REFERENCE_COLOR],
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
				position: "bottom",
				horizontalAlign: "left",
				labels: {
					colors: textColorPrimary,
				},
				markers: {
					fillColors: [ESTIMATED_COLOR, REFERENCE_COLOR],
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

	// Creating CSV Content...

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
	<div id="spectral-chart" use:chart={options} />
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
