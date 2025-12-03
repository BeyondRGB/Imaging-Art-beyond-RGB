<script lang="ts">
  import Button from "@components/Button.svelte";
  import { FilePlusIcon } from "svelte-feather-icons";
  import { forEach } from "lodash";
  import Dropzone from "svelte-file-dropzone";
  export let type = "File";
  export let filter = "None";
  export let defaultPath = undefined;
  export let label = "Select Files";
  export let filePaths = [];
  export let icon = FilePlusIcon;
  export let largeText = false;
  let ipcResponse;

  const temp = async () => {
    ipcResponse = await window.electron.handle({ type, filter, defaultPath });
  };

  $: if (ipcResponse) {
    if (!ipcResponse.canceled) {
      filePaths = ipcResponse?.filePaths;
    }
  }

</script>

<main>
  <Button
    variant="primary"
    size={largeText ? "lg" : "md"}
    onClick={async () => {
      await temp();
    }}
    icon={icon}
    iconPosition="right"
  >
    {label}
  </Button>
</main>

<style lang="postcss">
  main {
    @apply h-full;
  }
</style>
