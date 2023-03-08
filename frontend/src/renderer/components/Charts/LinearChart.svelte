<script>
  import "@carbon/charts/styles.min.css";
  import "carbon-components/css/carbon-components.min.css";
  import { ScatterChart } from "@carbon/charts-svelte";
  import {lab2rgb} from "@util/lab2rgb";

  export let data;
  export let matrix = "CM";

  $: if (data?.["matrix_values"]) {
    dataLL = [];
    colors = {};
    let chartData = [];
    data["matrix_values"]
      .find((ele) => ele.name === `${matrix} L*_camera`)
      ["data"].map((row, i) => {
        row.map((colData, k) => {
          chartData = [
            ...chartData,
            {
              col: String.fromCharCode(k + 65),
              row: i + 1,
              lcam: data["matrix_values"].find(
                (ele) => ele.name === `${matrix} L*_camera`
              )["data"][i][k],
              lref: data["matrix_values"].find(
                (ele) => ele.name === `${matrix} L*_ref`
              )["data"][i][k],
            },
          ];
        });
      });

    chartData.map((value) => {
      let rgb = lab2rgb([(value.lcam + value.lref) / 2, 0, 0]);
      dataLL.push({
        group: `${value.col}:${value.row}`,
        lref: value.lref,
        lcam: value.lcam,
      });
      colors[
        `${value.col}:${value.row}`
      ] = `rgb(${rgb[0]}, ${rgb[1]}, ${rgb[2]})`;
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
  .liner-chart .bx--cc--scatter circle {
    stroke: black !important;
  }

</style>
