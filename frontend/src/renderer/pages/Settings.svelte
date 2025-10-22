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
        description="Toggle between light and dark mode"
        bind:checked={$appSettings.theme}
        ariaLabel="Toggle dark theme"
      />
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    background-color: var(--color-surface-elevated);
    @apply w-[600px] max-h-[80vh] rounded-xl shadow-2xl overflow-hidden;
  }

  .settings {
    @apply flex flex-col p-8 gap-8 overflow-y-auto;
  }

  h1 {
    color: var(--color-text-primary);
    @apply text-2xl font-semibold mb-2;
  }

  .section {
    @apply flex flex-col gap-4;
  }

  h2 {
    color: var(--color-text-tertiary);
    @apply text-sm font-medium uppercase tracking-wider mb-2;
  }

  /* Scrollbar inherits from global theme.css */
</style>
