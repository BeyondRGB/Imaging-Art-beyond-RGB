<script>
  import "@carbon/charts/styles.min.css";
  import "carbon-components/css/carbon-components.min.css";
  import { HeatmapChart } from "@carbon/charts-svelte";
  export let data;

  export let matrixName = "CM DeltaE Values";

  $: if (data?.["matrix_values"]) {
    mapData = [];
    valueGrid = [];
    let temp = [];
    data["matrix_values"]
      .find((ele) => ele.name === matrixName)
      ["data"].map((row, i) => {
        let num_rows = data["matrix_values"].find(
          (ele) => ele.name === matrixName
        )["rows"];
        let num_cols = data["matrix_values"].find(
          (ele) => ele.name === matrixName
        )["cols"];
        row.map((colData, k) => {
          mapData = [
            ...mapData,
            {
              col: String.fromCharCode(k + 65),
              row: num_rows - i,
              value: data["matrix_values"].find(
                (ele) => ele.name === matrixName
              )["data"][num_rows - i - 1][k],
            },
          ];
          temp = [
            ...temp,
            { col: String.fromCharCode(k + 65), row: i + 1, value: colData },
          ];
        });
      });

    valueGrid = [];
    for (let x = 0; x < temp.length; x++) {
      valueGrid.push(
        temp.splice(
          0,
          data["matrix_values"].find((ele) => ele.name === matrixName)["cols"]
        )
      );
    }
  }

  let mapData = [];
  let valueGrid = [];
</script>

{#if mapData?.length > 1}
  <div class="heatmap-chart {matrixName}">
    <HeatmapChart
      data={mapData}
      options={{
        title: "ΔE Heatmap",
        axes: {
          bottom: {
            title: "Col",
            mapsTo: "col",
            scaleType: "labels",
          },
          left: {
            title: "Row",
            mapsTo: "row",
            scaleType: "labels",
          },
        },
        legend: {
          enabled: true,
          alignment: "center",
        },
        heatmap: {
          colorLegend: {
            title: "",
            type: "linear",
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
    <div
      class="heatmap-number-grid"
      style="--row:{valueGrid != null
        ? valueGrid.length
        : 0}; --col:{valueGrid != null ? valueGrid[0].length : 0};"
    >
      {#each valueGrid as row, i}
        {#each row as col, i}
          <p class="heatmap-value">
            {col.value.toFixed(1)}
          </p>
        {/each}
      {/each}
    </div>
  </div>
{/if}

<style lang="postcss" global>
  .heatmap-number-grid {
    display: grid;
    grid-template-rows: repeat(calc(var(--row) - 1), auto);
    grid-template-columns: repeat(var(--col), auto);
    @apply absolute w-[75vh] h-[75vh] pt-[3rem] pb-[3.8rem] pl-[1.9rem] pointer-events-none
            top-0 left-0;
  }
  .heatmap-value {
    @apply flex justify-center items-center h-full w-full font-bold text-black;
  }
  .heatmap-chart {
    @apply relative;
  }

  /* Global Chart Styles */
  .bx--cc--chart-wrapper text {
    @apply fill-white;
  }
  .bx--cc--grid rect.chart-grid-backdrop {
    @apply fill-gray-700;
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
  /* --- Tooltip */
  .bx--cc--tooltip {
    @apply bg-gray-700 text-white;
  }
  .bx--cc--tooltip .content-box {
    @apply text-white;
  }
  .bx--cc--tooltip .content-box .datapoint-tooltip p.value {
    @apply font-bold;
  }
  .bx--cc--tooltip .content-box .datapoint-tooltip {
    @apply p-2;
  }
  .bx--cc--tooltip .tooltip-color {
    @apply w-2;
  }
  /* --- */
  .bx--chart-holder {
    @apply aspect-square w-auto;
  }
  .bx--overflow-menu__icon {
    @apply fill-gray-100;
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

  .heatmap-chart .bx--cc--axes g.axis .axis-title {
    @apply hidden;
  }
</style>
