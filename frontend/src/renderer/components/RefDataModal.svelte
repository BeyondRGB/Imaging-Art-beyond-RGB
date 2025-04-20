<script lang="ts">
  import Switch from "@components/Switch.svelte";
  import { appSettings, processState, customRefData, persistentCustomRefData } from "@util/stores";
  import { modal } from "@util/stores";
  import { DownloadCloudIcon } from "svelte-feather-icons";
  import RefDataTemplate from "@assets/RefDataTemplate.csv?url";
  import FileSelector from "@components/FileSelector.svelte";

  export let closeModal;
  let filePaths = [];
  $: console.log(filePaths);
  function handle() {
    modal.set(null);
  }
  $: console.log($appSettings);
  $: console.log(closeModal);
  function confirm() {
    console.log("Confirm");
    if (filePaths) {
      if ($modal === "CustomRefData") {
        console.log("Saving to color target");
        $customRefData.calibration = {
          name: filePaths[0].split('\\').pop(),
          fileName: filePaths[0],
          standardObserver: 1931,
          illuminants: "D50",
        };
        $persistentCustomRefData.calibration.push({
          name: filePaths[0].split('\\').pop(),
          fileName: filePaths[0],
          standardObserver: 1931,
          illuminants: "D50",
        });
      } else if ($modal === "CustomRefDataVer") {
        $customRefData.verification = {
          name: filePaths[0].split('\\').pop(),
          fileName: filePaths[0],
          standardObserver: 1931,
          illuminants: "D50",
        };
        $persistentCustomRefData.verification.push({
          name: filePaths[0].split('\\').pop(),
          fileName: filePaths[0],
          standardObserver: 1931,
          illuminants: "D50",
        });
      }
    }
    closeModal();
  }
</script>

<main>
  <div class="settings">
    <h2>Custom Reference Data</h2>
    <div class="template">
      <p>
        To use custom reference data you must use the template. Note: All data
        and coordinates must be inputed correctly!
      </p>
      <a
        class="downloadLink"
        href={RefDataTemplate}
        download="RefDataTemplate.csv"
        ><button class="downloadBtn">
          Open Template
          <div class="iconBox"><DownloadCloudIcon size="1.25x" /></div>
        </button></a
      >
    </div>

    <div class="inputDiv">
      <FileSelector
        bind:filePaths
        label={"Select Reference Data"}
        type="Single"
        filter="csv"
      />
      <div class="flex flex-col gap-1">
        {#if filePaths && filePaths.length > 0}
          <div class="flex items-center justify-center w-full">
            <p class="inputLabel new">New File:</p>
            <div class="input">
              {filePaths}
            </div>
          </div>
        {/if}
        {#if $processState.artStacks[0].colorTarget?.refData}
          <div class="flex items-center justify-center w-full">
            <p class="inputLabel">Current File:</p>
            <div class="input">
              {$processState.artStacks[0].colorTarget?.refData?.name}
            </div>
          </div>
        {/if}
        {#if $processState.artStacks[0].verificationTarget?.refData}
          <div class="flex items-center justify-center w-full">
            <p class="inputLabel">Current File:</p>
            <div class="input">
              {$processState.artStacks[0].verificationTarget?.refData?.name}
            </div>
          </div>
        {/if}
      </div>
    </div>
    <div class="buttonGroup">
      <button class="cancel" on:click={closeModal}>Cancel</button>
      <button class="confirm" on:click={confirm}>Confirm</button>
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply max-w-[50%] max-h-[50%] flex justify-center items-center bg-gray-700 rounded-2xl
            shadow-xl;
  }
  .settings {
    @apply w-full h-full flex flex-col justify-between p-6 pb-6;
  }
  .downloadLink {
    @apply p-0 w-1/2;
  }
  .downloadBtn {
    @apply w-full p-0 pl-2 text-lg flex justify-between gap-2 items-center whitespace-nowrap;
  }
  h2 {
    @apply w-full text-xl flex justify-center;
  }
  .template {
    @apply w-full flex justify-between items-center gap-2;
  }
  .inputDiv {
    @apply flex flex-col justify-center items-center gap-1;
  }
  .iconBox {
    @apply h-full bg-green-500/90 rounded-r-lg p-1;
  }
  .input {
    @apply w-full bg-gray-800 rounded-r-lg h-full flex items-center justify-center p-2;
  }

  .buttonGroup {
    @apply flex justify-end gap-2;
  }
  .confirm {
    @apply bg-green-600/75 hover:bg-green-500;
  }
  .cancel {
    @apply hover:bg-red-500/50;
  }
  .inputLabel {
    @apply h-full flex justify-center items-center bg-blue-600/25 rounded-l-lg
          px-2 whitespace-nowrap;
  }
  .new {
    @apply bg-green-600/25;
  }
</style>
