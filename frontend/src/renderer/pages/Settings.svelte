<script lang="ts">
  import Switch from "@components/Switch.svelte";
  import { appSettings } from "@util/stores";
  import { modal } from "@util/stores";
</script>

<main>
  <div class="settings">
    Settings:
    <!-- <Switch label="Dark Mode" bind:checked={$appSettings.theme} /> -->
    <Switch label="Side Menu" bind:checked={$appSettings.sideNav} />
  </div>
</main>

<style lang="postcss">
  main {
    @apply w-2/5 h-2/5 flex justify-center items-center bg-gray-700 rounded-2xl
            shadow-xl;
  }
  .settings {
    @apply flex-col justify-center p-6;
  }
</style>
