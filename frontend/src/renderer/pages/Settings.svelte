<script lang="ts">
  import SwitchRow from "@components/SwitchRow.svelte";
  import { appSettings, type ThemePreference } from "@util/stores";
  import ScrollContainer from "@components/ScrollContainer.svelte";
  import { SunIcon, MoonIcon, MonitorIcon } from "svelte-feather-icons";

  const themeOptions: { value: ThemePreference; label: string; icon: typeof SunIcon }[] = [
    { value: 'light', label: 'Light', icon: SunIcon },
    { value: 'dark', label: 'Dark', icon: MoonIcon },
    { value: 'system', label: 'System', icon: MonitorIcon },
  ];

  function setTheme(pref: ThemePreference) {
    appSettings.update(settings => ({
      ...settings,
      themePreference: pref
    }));
  }
</script>

<main>
  <ScrollContainer className="settings">
    <h1>Settings</h1>
    
    <div class="section">
      <h2>Interface</h2>
      
      <SwitchRow
        label="Side Menu"
        description="Display navigation on the left side"
        bind:checked={$appSettings.sideNav}
        ariaLabel="Toggle side menu navigation"
      />

      <div class="setting-row">
        <div class="setting-info">
          <span class="setting-label">Theme</span>
          <span class="setting-description">Choose light, dark, or follow system preference</span>
        </div>
        <div class="theme-selector">
          {#each themeOptions as option}
            <button
              class="theme-option"
              class:selected={$appSettings.themePreference === option.value}
              on:click={() => setTheme(option.value)}
              aria-label="Set theme to {option.label}"
              aria-pressed={$appSettings.theme === option.value}
            >
              <svelte:component this={option.icon} size="16" />
              <span>{option.label}</span>
            </button>
          {/each}
        </div>
      </div>
    </div>
  </ScrollContainer>
</main>

<style lang="postcss">
  main {
    background-color: var(--color-surface-elevated);
    @apply w-[600px] max-h-[80vh] rounded-xl shadow-2xl overflow-hidden;
  }

  :global(.settings) {
    @apply flex flex-col p-8 gap-8;
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

  .setting-row {
    @apply flex items-center justify-between py-3 px-4 rounded-lg;
    background-color: var(--color-surface);
  }

  .setting-info {
    @apply flex flex-col gap-0.5;
  }

  .setting-label {
    color: var(--color-text-primary);
    @apply font-medium;
  }

  .setting-description {
    color: var(--color-text-tertiary);
    @apply text-sm;
  }

  .theme-selector {
    @apply flex gap-1 p-1 rounded-lg;
    background-color: var(--color-surface-sunken);
  }

  .theme-option {
    @apply flex items-center gap-1.5 px-3 py-1.5 rounded-md text-sm font-medium;
    @apply transition-all duration-150 ease-in-out;
    @apply cursor-pointer border-none outline-none;
    color: var(--color-text-secondary);
    background-color: transparent;
  }

  .theme-option:hover:not(.selected) {
    background-color: var(--color-interactive-hover);
    color: var(--color-text-primary);
  }

  .theme-option.selected {
    background-color: var(--color-surface-elevated);
    color: var(--color-text-primary);
    @apply shadow-sm;
  }

  .theme-option:focus-visible {
    @apply ring-2 ring-offset-1;
    ring-color: var(--color-border-focus);
  }

  /* Scrollbar inherits from global theme.css */
</style>
