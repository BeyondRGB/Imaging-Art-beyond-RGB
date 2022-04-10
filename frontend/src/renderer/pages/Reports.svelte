<script>
  import {
    processState,
    viewState,
    sendMessage,
    messageStore,
    currentPage,
  } from "@util/stores";
  import Heatmap from "@components/Charts/Heatmap.svelte";
  import ScatterCharts from "@root/components/Charts/ScatterCharts.svelte";
  import LinearChart from "@root/components/Charts/LinearChart.svelte";
  import FileSelector from "@components/FileSelector.svelte";
  import { ChevronsDownIcon, ChevronsUpIcon } from "svelte-feather-icons";
  let open = false;

  function getReports() {
    let rand = Math.floor(Math.random() * 99999999);
    let msg = {
      RequestID: rand,
      RequestType: "Reports",
      RequestData: {
        name: $viewState.projectKey,
      },
    };

    console.log("Fetching Reports");
    console.log(msg);
    sendMessage(JSON.stringify(msg));
  }

  let toggle = false;
  $: if (
    $currentPage === "Reports" &&
    $viewState.projectKey !== null &&
    !toggle
  ) {
    console.log("Getting FIRST Report");
    // CALL FOR REPORTS
    toggle = true;
    getReports();
  }
  let mainfilePath;
  $: if (mainfilePath?.length > 0) {
    console.log("New Project Key");
    $viewState.projectKey = mainfilePath[0];
  }

  $: if ($messageStore.length > 1 && !($messageStore[0] instanceof Blob)) {
    try {
      let temp = JSON.parse($messageStore[0]);
      console.log(temp);
      if (temp["ResponseType"] === "Report") {
        // Report handler
        console.log("Report From Server");
        if (temp["ResponseData"]["reportType"] === "Calibration") {
          $viewState.reports.calibration = temp["ResponseData"]["reports"];
        } else if (temp["ResponseData"]["reportType"] === "Verification") {
          $viewState.reports.verification = temp["ResponseData"]["reports"];
        }
      }
    } catch (e) {
      console.log(e);
    }
  }

  $: console.log($viewState.reports);
</script>

<main class="reports-main">
  {#if $viewState.projectKey === null}
    <div class="noFile">
      <div class="inputBox">
        <h2>Select a project file to import into BeyondRGB</h2>
        <FileSelector bind:filePaths={mainfilePath} filter="project" />
      </div>
    </div>
  {/if}
  <div class="art">
    <div class="report-header" class:show={$currentPage === "Reports"}>
      <div class="report-name">
        {$viewState.projectKey.split("\\").length > 2
          ? $viewState.projectKey.split("\\").at(-1)
          : $viewState.projectKey.split("/").at(-1)}
      </div>
      <div class="report-info">
        Mean ΔE: {parseFloat(
          $viewState.reports.calibration["double_values"][0]["data"]
        ).toFixed(4)}
      </div>
    </div>
    <div class="reports">
      <div class="report-item">
        <Heatmap bind:data={$viewState.reports.calibration} />
      </div>
      <div class="report-item">
        <ScatterCharts bind:data={$viewState.reports.calibration} />
      </div>
      <div class="report-item">
        <LinearChart bind:data={$viewState.reports.calibration} />
      </div>
    </div>
  </div>
</main>

<style lang="postcss" global>
  .reports-main {
    @apply flex flex-col w-full h-full overflow-y-scroll pt-[15vh];
  }
  .art {
    @apply flex flex-col relative;
  }
  .reports {
    transform-origin: left top;
    @apply w-full h-full transition-all duration-300
          bg-gray-700 rounded-b-2xl p-4 flex flex-col items-center justify-center;
  }
  .report-item {
    @apply w-full h-full flex justify-center items-center;
  }
  .dropdown-report-btn {
    @apply w-full flex flex-col justify-center items-center z-50;
  }

  .noFile {
    @apply absolute w-full h-full z-[99] flex justify-center bg-gray-800
            items-center;
  }
  .inputBox {
    @apply w-auto h-auto bg-gray-700 flex flex-col gap-2 justify-center items-center
          p-8 rounded-2xl;
  }
  .inputBox h2 {
    @apply text-xl;
  }
  .report-header {
    @apply fixed w-full top-0 bg-gray-800 z-[10000] flex flex-col p-4 rounded-b-xl scale-0;
  }
  .report-header.show {
    @apply scale-100;
  }
  .report-name {
    @apply text-4xl;
  }
</style>
