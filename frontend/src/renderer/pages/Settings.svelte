<script lang="ts">
  import SwitchRow from "@components/SwitchRow.svelte";
  import { appSettings } from "@util/stores";
</script>

<main>
  <div class="settings">
    <h1>Settings</h1>
    
    <div class="section">
      <h2>Interface</h2>
      
      <SwitchRow
        label="Side Menu"
        description="Display navigation on the left side"
        bind:checked={$appSettings.sideNav}
        ariaLabel="Toggle side menu navigation"
      />

      <SwitchRow
        label="Theme"
        description="not fully supported yet"
        checked={true}
        disabled={true}
        ariaLabel="Toggle dark theme (currently disabled)"
      />
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply w-[600px] max-h-[80vh] bg-gray-800 rounded-xl shadow-2xl overflow-hidden;
  }

  .settings {
    @apply flex flex-col p-8 gap-8 overflow-y-auto;
  }

  h1 {
    @apply text-2xl font-semibold text-gray-100 mb-2;
  }

  .section {
    @apply flex flex-col gap-4;
  }

  h2 {
    @apply text-sm font-medium text-gray-400 uppercase tracking-wider mb-2;
  }

  .settings::-webkit-scrollbar {
    @apply w-2;
  }

  .settings::-webkit-scrollbar-thumb {
    @apply bg-gray-600 rounded-full;
  }
</style>
