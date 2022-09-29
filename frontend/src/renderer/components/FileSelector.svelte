<script lang="ts">
  import { FilePlusIcon } from "svelte-feather-icons";
  import { forEach } from "lodash";
  import Dropzone from "svelte-file-dropzone";
  export let type = "File";
  export let filter = "None";
  export let label = "Select Files";
  export let filePaths = [];
  export let icon = FilePlusIcon;
  export let largeText = false;
  let ipcResponse;
  let files = {
    accepted: [],
    rejected: []
  };
  // const temp (e) => {
  //   console.log("drag");
  //   ipcResponse = await window.electron.handle({ type, filter });
  // }; 
  $: if (ipcResponse) {
    if (!ipcResponse.canceled) {
      filePaths = ipcResponse?.filePaths;
    }
  }

  function handleFilesSelect(e) {
    const { acceptedFiles, fileRejections } = e.detail;
    files.accepted = [...files.accepted, ...acceptedFiles];
    files.rejected = [...files.rejected, ...fileRejections];
  
    forEach(files.accepted, (f) =>{
      filePaths.push({
        id: f.path,
        name: f.name
      });
    });
    console.log(filePaths); 
  }

</script>

<main>
  <Dropzone
      on:drop={handleFilesSelect}
      noClick
      containerClasses="custom-dropzone">
      <!-- <button
        class="group"
        class:largeText
        on:click={async () => {
          await temp();
        }}
        >{label}
        <div class="icon">
          <svelte:component this={icon} size="1.5x" />
        </div>
      </button> -->

      <p>or</p>
      <p>Drag and drop them here</p>
  </Dropzone>
  <ol>
    {#each filePaths as item}
      <li>{item}</li>
    {/each}
  </ol>

  <!-- <button
    class="group"
    class:largeText
    on:click={async () => {
      await temp();
    }}
    >{label}
    <div class="icon">
      <svelte:component this={icon} size="1.5x" />
    </div></button
  > -->
</main>

<style lang="postcss">
  main {
    @apply h-full;
  }
  button {
    @apply flex justify-between items-center gap-2 p-0 pl-2 whitespace-nowrap;
  }
  .largeText {
    @apply text-lg;
  }
  .icon {
    @apply bg-gray-500 p-1 group-hover:bg-blue-400 transition-all rounded-r-lg;
  }
</style>
