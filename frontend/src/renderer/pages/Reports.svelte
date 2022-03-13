<script>
  import Heatmap from "@components/Charts/Heatmap.svelte";
  let open = false;
</script>

<main>
  Reports
  <button class="dropdown-report-btn" on:click={() => (open = !open)}
    >Art 1</button
  >
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
    @apply w-full h-10;
  }
</style>
