<script>
  import "@carbon/charts/styles.min.css";
  import "carbon-components/css/carbon-components.min.css";
  import { HeatmapChart } from "@carbon/charts-svelte";

  let testData = [
    {
      col: "A",
      row: 10,
      value: 0.9,
    },
    {
      col: "B",
      row: 10,
      value: 0.2,
    },
    {
      col: "C",
      row: 10,
      value: 1,
    },
    {
      col: "D",
      row: 10,
      value: 1.6,
    },
    {
      col: "E",
      row: 10,
      value: 1.3,
    },
    {
      col: "F",
      row: 10,
      value: 2.5,
    },
    {
      col: "G",
      row: 10,
      value: 0.5,
    },
    {
      col: "H",
      row: 10,
      value: 1,
    },
    {
      col: "I",
      row: 10,
      value: 0.3,
    },
    {
      col: "J",
      row: 10,
      value: 1.3,
    },
    {
      col: "K",
      row: 10,
      value: 0.9,
    },
    {
      col: "L",
      row: 10,
      value: 1.1,
    },
    {
      col: "M",
      row: 10,
      value: 1.8,
    },
    {
      col: "A",
      row: 9,
      value: 0.6,
    },
    {
      col: "B",
      row: 9,
      value: 0.2,
    },
    {
      col: "C",
      row: 9,
      value: 0.5,
    },
    {
      col: "D",
      row: 9,
      value: 0.1,
    },
    {
      col: "E",
      row: 9,
      value: 1.4,
    },
    {
      col: "F",
      row: 9,
      value: 0.4,
    },
    {
      col: "G",
      row: 9,
      value: 1.8,
    },
    {
      col: "H",
      row: 9,
      value: 1.2,
    },
    {
      col: "I",
      row: 9,
      value: 4.1,
    },
    {
      col: "J",
      row: 9,
      value: 0.6,
    },
    {
      col: "K",
      row: 9,
      value: 6,
    },
    {
      col: "L",
      row: 9,
      value: 2.7,
    },
    {
      col: "M",
      row: 9,
      value: 2.5,
    },
    {
      col: "A",
      row: 8,
      value: 0.9,
    },
    {
      col: "B",
      row: 8,
      value: 2.2,
    },
    {
      col: "C",
      row: 8,
      value: 1.9,
    },
    {
      col: "D",
      row: 8,
      value: 1.4,
    },
    {
      col: "E",
      row: 8,
      value: 1.4,
    },
    {
      col: "F",
      row: 8,
      value: 4,
    },
    {
      col: "G",
      row: 8,
      value: 0.2,
    },
    {
      col: "H",
      row: 8,
      value: 1,
    },
    {
      col: "I",
      row: 8,
      value: 1.8,
    },
    {
      col: "J",
      row: 8,
      value: 2.4,
    },
    {
      col: "K",
      row: 8,
      value: 0.7,
    },
    {
      col: "L",
      row: 8,
      value: 1.6,
    },
    {
      col: "M",
      row: 8,
      value: 2.4,
    },
    {
      col: "A",
      row: 7,
      value: 0.2,
    },
    {
      col: "B",
      row: 7,
      value: 0.1,
    },
    {
      col: "C",
      row: 7,
      value: 0.5,
    },
    {
      col: "D",
      row: 7,
      value: 2.2,
    },
    {
      col: "E",
      row: 7,
      value: 0.1,
    },
    {
      col: "F",
      row: 7,
      value: 2.1,
    },
    {
      col: "G",
      row: 7,
      value: 0.3,
    },
    {
      col: "H",
      row: 7,
      value: 1.2,
    },
    {
      col: "I",
      row: 7,
      value: 2.5,
    },
    {
      col: "J",
      row: 7,
      value: 2.7,
    },
    {
      col: "K",
      row: 7,
      value: 1.1,
    },
    {
      col: "L",
      row: 7,
      value: 2.9,
    },
    {
      col: "M",
      row: 7,
      value: 0.7,
    },
    {
      col: "A",
      row: 6,
      value: 2.8,
    },
    {
      col: "B",
      row: 6,
      value: 1.4,
    },
    {
      col: "C",
      row: 6,
      value: 3.5,
    },
    {
      col: "D",
      row: 6,
      value: 0.8,
    },
    {
      col: "E",
      row: 6,
      value: 1.4,
    },
    {
      col: "F",
      row: 6,
      value: 0.3,
    },
    {
      col: "G",
      row: 6,
      value: 0.7,
    },
    {
      col: "H",
      row: 6,
      value: 2.3,
    },
    {
      col: "I",
      row: 6,
      value: 3.8,
    },
    {
      col: "J",
      row: 6,
      value: 2.7,
    },
    {
      col: "K",
      row: 6,
      value: 0.5,
    },
    {
      col: "L",
      row: 6,
      value: 1.3,
    },
    {
      col: "M",
      row: 6,
      value: 2.1,
    },
    {
      col: "A",
      row: 5,
      value: 1.1,
    },
    {
      col: "B",
      row: 5,
      value: 0.8,
    },
    {
      col: "C",
      row: 5,
      value: 0.4,
    },
    {
      col: "D",
      row: 5,
      value: 3.8,
    },
    {
      col: "E",
      row: 5,
      value: 3.7,
    },
    {
      col: "F",
      row: 5,
      value: 0.1,
    },
    {
      col: "G",
      row: 5,
      value: 0.6,
    },
    {
      col: "H",
      row: 5,
      value: 1.4,
    },
    {
      col: "I",
      row: 5,
      value: 2.2,
    },
    {
      col: "J",
      row: 5,
      value: 1.9,
    },
    {
      col: "K",
      row: 5,
      value: 0.4,
    },
    {
      col: "L",
      row: 5,
      value: 2.1,
    },
    {
      col: "M",
      row: 5,
      value: 0.1,
    },
    {
      col: "A",
      row: 4,
      value: 0.4,
    },
    {
      col: "B",
      row: 4,
      value: 2,
    },
    {
      col: "C",
      row: 4,
      value: 0.4,
    },
    {
      col: "D",
      row: 4,
      value: 1.5,
    },
    {
      col: "E",
      row: 4,
      value: 7.8,
    },
    {
      col: "F",
      row: 4,
      value: 0.9,
    },
    {
      col: "G",
      row: 4,
      value: 1.3,
    },
    {
      col: "H",
      row: 4,
      value: 0.3,
    },
    {
      col: "I",
      row: 4,
      value: 0.3,
    },
    {
      col: "J",
      row: 4,
      value: 1.7,
    },
    {
      col: "K",
      row: 4,
      value: 1.1,
    },
    {
      col: "L",
      row: 4,
      value: 1.7,
    },
    {
      col: "M",
      row: 4,
      value: 1.9,
    },
    {
      col: "A",
      row: 3,
      value: 2.3,
    },
    {
      col: "B",
      row: 3,
      value: 2.8,
    },
    {
      col: "C",
      row: 3,
      value: 2.7,
    },
    {
      col: "D",
      row: 3,
      value: 1.1,
    },
    {
      col: "E",
      row: 3,
      value: 1.4,
    },
    {
      col: "F",
      row: 3,
      value: 0.2,
    },
    {
      col: "G",
      row: 3,
      value: 1.4,
    },
    {
      col: "H",
      row: 3,
      value: 0.8,
    },
    {
      col: "I",
      row: 3,
      value: 0.1,
    },
    {
      col: "J",
      row: 3,
      value: 0.8,
    },
    {
      col: "K",
      row: 3,
      value: 2.5,
    },
    {
      col: "L",
      row: 3,
      value: 0.1,
    },
    {
      col: "M",
      row: 3,
      value: 3,
    },
    {
      col: "A",
      row: 2,
      value: 0.6,
    },
    {
      col: "B",
      row: 2,
      value: 2.8,
    },
    {
      col: "C",
      row: 2,
      value: 1.7,
    },
    {
      col: "D",
      row: 2,
      value: 0.5,
    },
    {
      col: "E",
      row: 2,
      value: 0.9,
    },
    {
      col: "F",
      row: 2,
      value: 1.5,
    },
    {
      col: "G",
      row: 2,
      value: 0.4,
    },
    {
      col: "H",
      row: 2,
      value: 2.3,
    },
    {
      col: "I",
      row: 2,
      value: 1.4,
    },
    {
      col: "J",
      row: 2,
      value: 0.8,
    },
    {
      col: "K",
      row: 2,
      value: 1.5,
    },
    {
      col: "L",
      row: 2,
      value: 2.6,
    },
    {
      col: "M",
      row: 2,
      value: 0.4,
    },
    {
      col: "A",
      row: 1,
      value: 0.2,
    },
    {
      col: "B",
      row: 1,
      value: 2.9,
    },
    {
      col: "C",
      row: 1,
      value: 0.5,
    },
    {
      col: "D",
      row: 1,
      value: 2.6,
    },
    {
      col: "E",
      row: 1,
      value: 1.5,
    },
    {
      col: "F",
      row: 1,
      value: 0.9,
    },
    {
      col: "G",
      row: 1,
      value: 0,
    },
    {
      col: "H",
      row: 1,
      value: 3.8,
    },
    {
      col: "I",
      row: 1,
      value: 5.9,
    },
    {
      col: "J",
      row: 1,
      value: 1.7,
    },
    {
      col: "K",
      row: 1,
      value: 2.3,
    },
    {
      col: "L",
      row: 1,
      value: 1.8,
    },
    {
      col: "M",
      row: 1,
      value: 0.7,
    },
  ];

  let valueGrid = [];
  let temp = [...testData];
  while (temp.length) valueGrid.push(temp.splice(0, 13));

  console.log(valueGrid);
  console.log(testData);
</script>

<HeatmapChart
  data={testData}
  options={{
    title: "ΔE Heatmap",
    axes: {
      bottom: {
        title: "",
        mapsTo: "col",
        scaleType: "labels",
      },
      left: {
        title: "",
        mapsTo: "row",
        scaleType: "labels",
      },
    },
    heatmap: {
      colorLegend: {
        title: "",
      },
    },
    experimental: true,
    toolbar: {
      enabled: true,
      // controls: [
      //   { type: "Show as table" },
      //   { type: "Make Fullscrean" },
      //   { type: "Zoom out" },
      // ],
      numberOfIcons: 1,
    },
    legend: {
      enabled: true,
      alignment: "center",
    },
    height: "75vh",
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
<div class="heatmap-number-grid">
  {#each valueGrid.reverse() as row, i}
    {#each row as col, i}
      <p class="heatmap-value">
        {col.value.toFixed(1)}
      </p>
    {/each}
  {/each}
</div>

<style lang="postcss" global>
  .bx--cc--grid rect.chart-grid-backdrop {
    @apply fill-gray-600;
  }
  .bx--cc--axes g.axis g.tick text {
    @apply fill-white;
  }
  .bx--cc--title p.title {
    @apply text-white;
  }
  .bx--cc--axes g.axis .axis-title {
    @apply fill-white;
  }
  .bx--overflow-menu__icon {
    @apply fill-gray-100;
  }
  .bx--chart-holder {
    @apply aspect-square w-auto;
  }
  .heatmap-number-grid {
    display: grid;
    grid-template-rows: repeat(9, auto);
    grid-template-columns: repeat(13, auto);
    @apply absolute w-[75vh] h-[75vh] pt-[8vh] pb-[10vh] pl-[4.5vh] pointer-events-none;
  }
  .heatmap-value {
    @apply flex justify-center items-center h-full w-full font-bold text-black;
  }
  .bx--cc--chart-wrapper text {
    @apply fill-white;
  }
  /* .bx--chart-holder .bx--overflow-menu,
  .bx--chart-holder .bx--overflow-menu__trigger {
    @apply hidden;
  } */
  /* .bx--chart-holder .bx--overflow-menu,
  .bx--chart-holder
    .bx--overflow-menu__trigger:first-child
    .toolbar-control:first-child {
    @apply hidden;
  } */
  .bx--cc--tooltip {
    @apply bg-gray-700 text-white;
  }
  .bx--cc--tooltip .content-box {
    @apply text-white;
  }
  /* .bx--cc--tooltip .content-box .datapoint-tooltip div.label {
    @apply hidden;
  } */
  .bx--cc--tooltip .content-box .datapoint-tooltip p.value {
    @apply font-bold;
  }
  .bx--cc--tooltip .content-box .datapoint-tooltip {
    @apply p-2;
  }
  .bx--cc--tooltip .tooltip-color {
    @apply w-2;
  }
  .bx--overflow-menu:hover,
  .bx--overflow-menu__trigger:hover {
    @apply bg-transparent;
  }
  .bx--overflow-menu.bx--overflow-menu--open:hover {
    @apply bg-transparent;
  }
  .bx--chart-holder .bx--cc--toolbar .bx--overflow-menu--flip.is-open {
    @apply bg-transparent;
  }
  .bx--overflow-menu-options__option:hover {
    @apply bg-transparent;
  }
  .bx--overflow-menu.bx--overflow-menu--open,
  .bx--overflow-menu.bx--overflow-menu--open .bx--overflow-menu__trigger {
    @apply bg-transparent;
  }
</style>
