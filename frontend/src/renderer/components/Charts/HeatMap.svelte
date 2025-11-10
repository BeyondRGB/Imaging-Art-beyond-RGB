<script>
  import "@carbon/charts/styles.min.css";
  import "carbon-components/css/carbon-components.min.css";
  import { find } from "lodash";
  import AtomicHeatMap from "@components/Charts/AtomicHeatMap.svelte";
  import Button from "@components/Button.svelte";
  import Switch from "@components/Switch.svelte";
  import { createEventDispatcher } from 'svelte';
  import { exportHeatmapCSV } from "@util/csvExport.js";
  export let data;

  const dispatch = createEventDispatcher();

  function handleDataPointSelect(event) {
    // Re-dispatch the event to the parent
    dispatch('datapointselect', event.detail);
  }

  export let visionDeficiencyMode = false;
  let mapData = [];
  $: if (data?.matrix_values) {
    const deltaE = find(data?.matrix_values, {'name': 'CM DeltaE Values'});
    mapData = deltaE?.data.slice().reverse();
  }
  $: flattenedValues = mapData ? mapData.flat() : [];
  $: sortedValues = flattenedValues.slice().sort((a, b) => a - b);
  let p90obj = null;

  $: p90obj = sortedValues.length ? sortedValues[(Math.floor(0.9 * sortedValues.length) - 1 )] : null;
  $: if (p90obj !== null) {
        console.log("Dispatching p90update:", p90obj);
        dispatch('p90update', { p90: p90obj });
      }

  const exportCSV = function () {
    exportHeatmapCSV(mapData, p90obj, 'calibrationReport.csv');
  };

</script>

{#if mapData?.length > 1}
  <div class="heatmap-chart">
    <div class="heatmap-header">
      <span class="heatmap-title">ΔE Heatmap</span>
      <div class="heatmap-controls">
        <Switch
          label="Grayscale"
          bind:checked={visionDeficiencyMode}
          ariaLabel="Toggle grayscale mode"
        />
        <Button 
          variant="secondary" 
          size="sm" 
          onClick={exportCSV}
        >
          Export
        </Button>
      </div>
    </div>
     <div style="display: flex; align-items: center;">
        <AtomicHeatMap
          on:datapointselect={handleDataPointSelect}
          data={mapData}
          visionDeficiencyMode={visionDeficiencyMode}>
        </AtomicHeatMap>
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
    color: var(--color-text-primary);
    @apply flex justify-center items-center h-full w-full font-bold;
  }
  .heatmap-chart {
    @apply relative;
  }
  
  .heatmap-header {
    @apply flex justify-between items-center mb-4;
  }
  
  .heatmap-title {
    color: var(--color-text-primary);
    @apply text-lg font-semibold;
  }
  
  .heatmap-controls {
    @apply flex items-center gap-3;
  }

  /* Global Chart Styles */
  .bx--cc--chart-wrapper text {
    fill: var(--color-text-primary);
  }
  .bx--cc--grid rect.chart-grid-backdrop {
    fill: var(--color-surface-sunken);
  }
  .bx--cc--axes g.axis g.tick text {
    fill: var(--color-text-secondary);
  }
  .bx--cc--title p.title {
    color: var(--color-text-primary);
  }
  .bx--cc--axes g.axis .axis-title {
    fill: var(--color-text-primary);
  }
  /* --- Tooltip */
  .bx--cc--tooltip {
    background-color: var(--color-surface);
    color: var(--color-text-primary);
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
