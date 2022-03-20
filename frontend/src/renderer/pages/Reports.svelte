<script>
  import Heatmap from "@components/Charts/Heatmap.svelte";
  import ScatterCharts from "@root/components/Charts/ScatterCharts.svelte";
  import { ChevronsDownIcon, ChevronsUpIcon } from "svelte-feather-icons";
  let open = false;
</script>

<main class="reports-main">
  <div class="art">
    <button class="dropdown-report-btn" on:click={() => (open = !open)}
      >Art 1
      {#if open}
        <ChevronsUpIcon size="1.5x" />
      {:else}
        <ChevronsDownIcon size="1.5x" />
      {/if}
    </button>
    <div class={`reports ${open ? "open-report" : ""}`}>
      <div class="report-item"><Heatmap /></div>
      <div class="report-item"><ScatterCharts /></div>
    </div>
  </div>
  <div class="art">
    <button class="dropdown-report-btn">Art 2 </button>
  </div>
</main>

<style lang="postcss" global>
  .reports-main {
    @apply flex flex-col w-full h-full overflow-y-scroll;
  }
  .art {
    @apply flex flex-col;
  }
  .reports {
    transform-origin: left top;
    @apply w-full h-0 scale-y-0 transition-all duration-300
          bg-gray-700 rounded-b-2xl p-4 flex flex-col items-center justify-center;
  }
  .report-item {
    @apply w-full h-full flex justify-center items-center;
  }
  .open-report {
    @apply scale-y-100 h-full;
  }
  .dropdown-report-btn {
    @apply w-full flex flex-col justify-center items-center z-50;
  }
</style>
