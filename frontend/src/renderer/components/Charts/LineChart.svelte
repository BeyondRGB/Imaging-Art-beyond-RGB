<script>
  import "@carbon/charts/styles.min.css";
  import "carbon-components/css/carbon-components.min.css";
  import { LineChart } from "@carbon/charts-svelte";
  import { ImageIcon } from "svelte-feather-icons";
  import html2canvas from "html2canvas";
  import { element } from "svelte/internal";
  import { chart } from "svelte-apexcharts";
  import { exportSpectralCSV } from "@util/csvExport.js";

  export let data = [];
  export let wavelengthArray = Array.from({ length: 36 }, (x, i) => i * 10 + 380);;
  export let trueShadowPos;
  export let stack = false;
  export let multiDataset = false; // For handling measured vs estimated spectra
  let pointColors = ['#610061', '#79008D', '#8300B5', '#7E00DB', '#6A00FF', '#3D00FF', '#0000FF', '#0046FF', '#007BFF', '#00A9FF', '#00D5FF', '#00FFFF', '#00FF92', '#00FF00', '#36FF00', '#5EFF00', '#81FF00', '#A3FF00', '#C3FF00', '#FFFF00', '#FFDF00', '#FFBE00', '#FF9B00', '#FF7700', '#FF4F00', '#FF2100', '#FF0000', '#FF0000', '#FF0000', '#FF0000', '#FF0000', '#FF0000', '#FF0000', '#FF0000', '#FF0000', '#FF0000'];

  let inputData = [];

  // Handle single dataset (original LineChart behavior)
  $: if (!multiDataset && data.length > 1) {
      const dataDict = [];

      // Clear the graph if we aren't stacking curves
      if(!stack) {
        inputData = [];
      }

      wavelengthArray.forEach((element, i) => {
          dataDict.push({
              x: element,
              y: data[i] * 100,
              fillColor: pointColors[i]
          });
      });
      inputData.push({
          type: 'line',
          name: 'Spectrum: (' + (typeof trueShadowPos.left === 'number' ? trueShadowPos.left.toFixed(1) : '0') + ";" + (typeof trueShadowPos.top === 'number' ? trueShadowPos.top.toFixed(1) : '0') + ")",
          data: dataDict
      });
      options.series = inputData;
      data = [];
  }

  // Handle multiple datasets (LineChartMeasured behavior)
  $: if (multiDataset && data.length > 1) {
      inputData = [];

      // Add data for both estimated and reference spectrum
      data.forEach((dataset, j) => {
          const dataDict = [];
          var namer;
          wavelengthArray.forEach((element, i) => {
              dataDict.push({
                  x: element,
                  y: dataset[i] * 100,
                  fillColor: pointColors[i]
              });
          });
          if (j==0) {
              namer = 'Estimated Spectrum: (' + trueShadowPos.left + ";" + trueShadowPos.top.toFixed(1) + ")";
          }
          else {
              namer = 'Reference Spectrum: (' + trueShadowPos.left + ";" + trueShadowPos.top.toFixed(1) + ")";
          }
          inputData.push({
              type: 'line',
              name: namer,
              data: dataDict,
              color: j === 0 ? '#FFFFFF' : '#000000' 
          });
          options.series = inputData;
      });
      data = [];
  }

  const options = {
      series: [],
      stroke: {
          show: true,
          curve: 'smooth',
          width: multiDataset ? 2 : 1,
      },
      chart: {
          background: multiDataset ? '#4A4A4C' : 'rgb(58, 58, 60)',
          animations: {
              enabled: false
          },
          zoom: false,
          height: '400px',
          width: '100%',
          type: 'line',
          stroke: {
              curve: 'smooth',
          },
          toolbar: {
              // Hamburger menu which has exports such as PNG and SVG, CSV is explicitly hidden via css
              show: true,
              tools: {
                  download: true,
              },
              export: {
                png: {},
                svg: {}
              }
          },
          selection: {
              enabled: false
          }
      },
      tooltip: {
          shared: true,
          intersect: false,
          theme: 'dark',
          x: {
            show: false
          },
          y: {
            formatter: function(value) {
                return `${value.toFixed(2)}%`;
            },
            title: {
                  formatter: multiDataset ? (seriesName, info) => seriesName : () => '',
              }
          },
          z: {
              show: false
          }
      },
      xaxis: {
          categories: [350, 400, 450, 500, 550, 600, 650, 700],
          tickAmount: 9,
          labels: {
              style: {
                  colors: '#FFFFFF',
              }
          },
          title: {
              text: "Wavelength (nm)",
              offsetX: 0,
              offsetY: 110,
              style: {
                  color: '#FFFFFF',
              }
          },
      },
      yaxis: {
          showAlways: false,
          decimalsInFloat: false,
          forceNiceScale: true,
          tickAmount: 6,
          min: 0,
          max: 100,
          axisTicks: {
              show: true
          },
          labels: {
              style: {
                  colors: "#FFFFFF"
              }
          },
          title: {
              text: "Reflectance (%)",
              style: {
                  color: '#FFFFFF',
              },
          },
      },
      legend: {
          show: true,
          showForSingleSeries: !multiDataset,
          labels: {
            colors: multiDataset ? undefined : '#FFFFFF',
            useSeriesColors: multiDataset,
          },
          markers: multiDataset ? {
              fillColors: pointColors,
          } : undefined,
      },
      title: {
          text: "Estimated Spectrum",
          align: 'left',
          style: {
              color:  '#FFFFFF'
          },
      }
  };

  const downloadCSV = () => {
    exportSpectralCSV(inputData, wavelengthArray, "SpectralLineChart.csv");
  };
       
  
</script>
  
<div class="line-chart" id="EstSpecChart">
  <div id="spectral-chart" use:chart={options}></div>
    <div id="spectral-csv-download-button" on:click={downloadCSV}>Download CSV</div>
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
    color: #27293d!important;
    border: 0px;
  }

  #spectral-csv-download-button:hover {
    cursor: pointer;
  }

  .apexcharts-menu-item.exportCSV {
    display: none;
  }

</style>
