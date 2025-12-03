<script lang="ts">
  import Switch from "@components/Switch.svelte";
  import { appSettings, processState, customRefData, persistentCustomRefData } from "@util/stores";
  import { modal } from "@util/stores";
  import { DownloadCloudIcon } from "svelte-feather-icons";
  import RefDataTemplate from "@assets/RefDataTemplate.csv?url";
  import FileSelector from "@components/FileSelector.svelte";
  import Button from "@components/Button.svelte";

  export let closeModal;
  let filePaths = [];
  $: console.log(filePaths);
  function handle() {
    modal.set(null);
  }
  $: console.log($appSettings);
  $: console.log(closeModal);
  // Extract filename from path, handling both Windows (\) and Unix (/) separators
  function getFileName(path: string): string {
    return path.split(/[/\\]/).pop() || path;
  }

  function confirm() {
    console.log("Confirm");
    if (filePaths) {
      const fileName = getFileName(filePaths[0]);
      if ($modal === "CustomRefData") {
        console.log("Saving to color target");
        $customRefData.calibration = {
          name: fileName,
          fileName: filePaths[0],
          standardObserver: 1931,
          illuminants: "D50",
        };
        $persistentCustomRefData.calibration.push({
          name: fileName,
          fileName: filePaths[0],
          standardObserver: 1931,
          illuminants: "D50",
        });
      } else if ($modal === "CustomRefDataVer") {
        $customRefData.verification = {
          name: fileName,
          fileName: filePaths[0],
          standardObserver: 1931,
          illuminants: "D50",
        };
        $persistentCustomRefData.verification.push({
          name: fileName,
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
    <div class="header">
      <h2>Custom Reference Data</h2>
    </div>

    <div class="content">
      <div class="info-section">
        <div class="template-info">
          <p>
            To use custom reference data you must use the template. All data and coordinates must be inputed correctly!
          </p>
          <a
            class="downloadLink"
            href={RefDataTemplate}
            download="RefDataTemplate.csv"
          >
            <Button className="w-full justify-between" variant="default" size="md" icon={DownloadCloudIcon} iconPosition="right">
              Open Template
            </Button>
          </a>
        </div>
      </div>

      <div class="input-section">
        <FileSelector
          bind:filePaths
          label={"Select Reference Data"}
          type="Single"
          filter="csv"
        />
        
        <div class="file-status">
          {#if filePaths && filePaths.length > 0}
            <div class="status-row">
              <span class="label new">New File:</span>
              <span class="value" title={filePaths[0]}>
                {filePaths[0].split(/[/\\]/).pop()}
              </span>
            </div>
          {/if}
          {#if $processState.artStacks[0].colorTarget?.refData}
            <div class="status-row">
              <span class="label">Current:</span>
              <span class="value">
                {$processState.artStacks[0].colorTarget?.refData?.name}
              </span>
            </div>
          {/if}
          {#if $processState.artStacks[0].verificationTarget?.refData}
            <div class="status-row">
              <span class="label">Current:</span>
              <span class="value">
                {$processState.artStacks[0].verificationTarget?.refData?.name}
              </span>
            </div>
          {/if}
        </div>
      </div>
    </div>

    <div class="footer">
      <Button variant="success" onClick={confirm} disabled={!filePaths || filePaths.length === 0}>Confirm</Button>
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    background-color: var(--color-surface);
    @apply w-[500px] flex flex-col rounded-xl shadow-2xl overflow-hidden;
  }

  .settings {
    @apply flex flex-col h-full;
  }

  .header {
    @apply p-4;
  }

  h2 {
    @apply text-xl font-semibold text-center m-0;
    color: var(--color-text-primary);
  }

  .content {
    @apply p-6 flex flex-col gap-6;
  }

  .info-section {
    @apply p-4 rounded-lg border border-blue-100 dark:border-blue-800;
  }

  .template-info p {
    @apply text-sm mb-3 leading-relaxed;
    color: var(--color-text-secondary);
  }

  .downloadLink {
    @apply block no-underline;
  }

  .input-section {
    @apply flex flex-col gap-4;
  }

  .file-status {
    @apply flex flex-col gap-2 text-sm;
  }

  .status-row {
    border: 1px solid var(--color-border);
    @apply flex items-center gap-2 p-2 rounded;
  }

  .label {
    @apply font-medium px-2 py-0.5 rounded text-xs uppercase tracking-wider;
    color: var(--color-text-secondary);
  }

  .label.new {
    @apply text-green-700 dark:text-green-300;
  }

  .value {
    @apply truncate flex-1 font-mono;
    color: var(--color-text-primary);
  }

  .footer {
    @apply p-4 flex justify-end gap-3;
  }
</style>
