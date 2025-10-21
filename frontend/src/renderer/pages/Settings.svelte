<script lang="ts">
  import Switch from "@components/Switch.svelte";
  import { appSettings } from "@util/stores";
</script>

<main>
  <div class="settings">
    <h1>Settings</h1>
    
    <div class="section">
      <h2>Interface</h2>
      
      <div class="setting-row">
        <div class="setting-info">
          <span class="setting-label">Side Menu</span>
          <span class="setting-desc">Display navigation on the left side</span>
        </div>
        <Switch bind:checked={$appSettings.sideNav} />
      </div>

      <div class="setting-row disabled">
        <div class="setting-info">
          <span class="setting-label">Theme</span>
          <span class="setting-desc">not fully supported yet</span>
        </div>
        <Switch checked={true} on:click={(e) => e.preventDefault()} />
      </div>
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

  .setting-row {
    @apply flex items-center justify-between gap-6 py-3 px-4 
           bg-gray-700/50 rounded-lg hover:bg-gray-700 transition-colors;
  }

  .setting-row.disabled {
    @apply opacity-50 cursor-not-allowed hover:bg-gray-700/50;
  }

  .setting-info {
    @apply flex flex-col gap-1 flex-1;
  }

  .setting-label {
    @apply text-base text-gray-100 font-medium;
  }

  .setting-desc {
    @apply text-sm text-gray-400;
  }

  .settings::-webkit-scrollbar {
    @apply w-2;
  }

  .settings::-webkit-scrollbar-thumb {
    @apply bg-gray-600 rounded-full;
  }
</style>
