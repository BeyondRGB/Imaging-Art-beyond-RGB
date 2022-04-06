<script lang="ts">
  import { FilePlusIcon } from "svelte-feather-icons";
  export let type = "File";
  export let filter = "None";
  export let label = "Select Files";
  export let filePaths = [];
  export let icon = FilePlusIcon;
  let ipcResponse;
  const temp = async () => {
    ipcResponse = await window.electron.handle({ type, filter });
    console.log(ipcResponse);
  };
  $: {
    console.log(ipcResponse);
  }
  $: {
    console.log(ipcResponse?.filePaths);
    filePaths = ipcResponse?.filePaths;
  }
</script>

<main>
  <button
    class="group"
    on:click={async () => {
      await temp();
    }}
    >{label}
    <div class="icon">
      <svelte:component this={icon} size="1.5x" />
    </div></button
  >
</main>

<style lang="postcss">
  main {
    @apply h-full;
  }
  button {
    @apply flex justify-between items-center gap-2 p-0 pl-2 whitespace-nowrap;
  }
  .icon {
    @apply bg-gray-500 p-1 group-hover:bg-blue-400 transition-all rounded-r-lg;
  }
</style>
