<script>
  import Switch from "@components/Switch.svelte";
  import { appSettings } from "../stores";
  import { getContext } from "svelte";
  import { modal } from "../stores";

  function handle() {
    modal.set(null);
  }
</script>

<main>
  <h1>App Settings</h1>
  <div>
    <Switch label="Dark Mode" bind:checked={$appSettings} />
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex-col justify-center w-full p-6;
  }
  div {
    @apply w-[50%];
  }
</style>
