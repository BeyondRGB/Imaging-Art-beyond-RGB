<script>
  import "@carbon/charts/styles.min.css";
  import "carbon-components/css/carbon-components.min.css";
  import { ScatterChart } from "@carbon/charts-svelte";

  let testData = [
    { group: "dataset 1", a: -100, b: -100 },
    { group: "test", a: 0, b: 0 },
    { group: "test", a: 100, b: -100 },
    { group: "test", a: -100, b: 100 },
    { group: "test", a: 100, b: -100 },
  ];

  let valueGrid = [];
  let temp = [...testData];
  while (temp.length) valueGrid.push(temp.splice(0, 13));

  console.log(valueGrid);
  console.log(testData);
</script>

<div class="scatter-charts">
  <ScatterChart
    data={testData}
    options={{
      title: "ΔE Heatmap",
      axes: {
        bottom: {
          title: "",
          mapsTo: "a",
          scaleType: "labels",
        },
        left: {
          title: "",
          mapsTo: "b",
          scaleType: "labels",
        },
      },
      height: "70vh",
      resizable: true,
      color: {
        scale: {
          "dataset 1": "blue",
        },
      },
    }}
  />
  <ScatterChart
    data={testData}
    options={{
      title: "ΔE Heatmap",
      axes: {
        bottom: {
          title: "",
          mapsTo: "a",
          scaleType: "labels",
        },
        left: {
          title: "",
          mapsTo: "b",
          scaleType: "labels",
        },
      },
      height: "70vh",
      resizable: true,
      color: {
        gradient: {
          enabled: true,
          colors: [
            "#0a9400",
            "#199a00",
            "#2a9f00",
            "#3ca500",
            "#4eab00",
            "#62b000",
            "#77b600",
            "#8dbb00",
            "#a4c100",
            "#bdc700",
            "#ccc300",
            "#d2b300",
            "#d8a300",
            "#dd9200",
            "#e37f00",
            "#e86c00",
            "#ee5700",
            "#f44100",
            "#f92a00",
            "#ff1200",
          ],
        },
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
  .scatter-charts {
    @apply w-full flex justify-center;
  }
  .bx--chart-holder {
    @apply aspect-square w-auto;
  }
  circle {
    fill-opacity: 1;
    r: 7;
  }
</style>
