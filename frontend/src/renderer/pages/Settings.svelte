<script lang="ts">
  import Switch from "@components/Switch.svelte";
  import { appSettings } from "@util/stores";
  import { modal } from "@util/stores";

  import SelectBtn from "@components/SelectBtn.svelte";
  function handle() {
    modal.set(null);
  }
  $: console.log($appSettings);
</script>

<div>
  <SelectBtn />
  <Switch label="Dark Mode" bind:checked={$appSettings.theme} />
  <Switch label="Side Menu" bind:checked={$appSettings.sideNav} />
</div>

<style lang="postcss">
  div {
    @apply flex-col justify-center w-full p-6;
  }
</style>
