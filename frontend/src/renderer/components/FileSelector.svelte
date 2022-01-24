<script lang="ts">
  export let type = "File";
  export let filePaths = [];
  let ipcResponse;
  const temp = async () => {
    ipcResponse = await window.electron.handle(type);
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
    on:click={async () => {
      await temp();
    }}>Select Files</button
  >
</main>

<style lang="postcss">
  main {
    @apply w-full h-full;
  }
</style>
