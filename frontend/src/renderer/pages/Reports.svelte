<script>
  import Heatmap from "@components/Charts/Heatmap.svelte";
  import { ChevronsDownIcon, ChevronsUpIcon } from "svelte-feather-icons";
  let open = false;
</script>

<main>
  Reports
  <button class="dropdown-report-btn" on:click={() => (open = !open)}
    >Art 1
    {#if open}
      <ChevronsUpIcon size="1.5x" />
    {:else}
      <ChevronsDownIcon size="1.5x" />
    {/if}
  </button>
  <div class={`reports ${open ? "open-report" : ""}`}>
    <Heatmap />
  </div>
</main>

<style lang="postcss" global>
  .reports {
    transform-origin: left top;
    @apply w-full scale-y-0 transition-all duration-300
          bg-gray-700 rounded-b-2xl p-4 flex items-center justify-center;
  }
  .open-report {
    @apply scale-y-100;
  }
  .dropdown-report-btn {
    @apply w-full flex flex-col justify-center items-center;
  }
</style>
