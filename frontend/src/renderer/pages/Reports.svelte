<script>
  import {
    viewState,
    sendMessage,
    messageStore,
    currentPage,
    processState,
  } from "@util/stores";
  import Heatmap from "@components/Charts/HeatMap.svelte";
  import LinearChart from "@root/components/Charts/LinearChart.svelte";
  import FileSelector from "@components/FileSelector.svelte";
  import VectorChart from "@components/Charts/VectorChart.svelte";
  import ImageViewer from "@root/components/ImageViewer.svelte";
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
    sendMessage(JSON.stringify(msg));
  }

  function colorManagedTargetImage() {
    let randID = Math.floor(Math.random() * 999999);
    $processState.CMTID = randID;
    let msg = {
      RequestID: randID,
      RequestType: "ColorManagedImage",
      RequestData: {
        name: $viewState.projectKey,
        target_requested: true,
      },
    };

    console.log("Fetching Color Managed Target Image");
    console.log(msg);
    //loading = true;
    sendMessage(JSON.stringify(msg));
  }

  let toggle = false;
  $: if (
    $currentPage === "Reports" &&
    $viewState.projectKey !== null &&
    !toggle
  ) {
    // CALL FOR REPORTS
    toggle = true;
    colorManagedTargetImage();
    getReports();
  }
  let mainfilePath;
  $: if (mainfilePath?.length > 0) {
    console.log("New Project Key");
    $viewState.projectKey = mainfilePath[0];
  }

  $: if ($messageStore.length > 1 && !($messageStore[0] instanceof Blob)) {
    console.log("New Message REPORTS");
    try {
      let temp = JSON.parse($messageStore[0]);
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

  $: isVerification =
    $viewState.reports.verification != null &&
    $viewState.reports.verification["double_values"].length > 0;

  let showVerification = false;

  function handleCloseReport() {
    $viewState.projectKey = null;
    $viewState.reports = {
      calibration: null,
      verification: null,
    };
    $viewState.colorManagedTargetImage = { dataURL: "", name: "Waiting..." };
    toggle = false;
    mainfilePath = null;
  }

  function detectZoom(event){ 
   // Zoom in or out based on the scroll direction 
    let direction = event.deltaY > 0 ? -1 : 1; 
    zoomImage(direction); 
  }

  let currentZoom=1;let stepSize=0.05;
  function zoomImage(direction) { 
      let newZoom = currentZoom + direction * stepSize; 
      // Limit the zoom level to the minimum and maximum values 
      if (newZoom < 1 || newZoom > 3) { 
          return; 
      } 
    
      currentZoom = newZoom; 
    
      // Update the CSS transform of the image to scale it 
      let image = document.querySelector("#cm-target-image"); 
      image.style.transform = "scale(" + currentZoom + ')'; 
  }
</script>

<main class="reports-main">
  {#if $viewState.projectKey === null}
    <div class="noFile">
      <div class="inputBox">
        <h2>Select a project file to import into BeyondRGB</h2>
        <FileSelector bind:filePaths={mainfilePath} filter="project" />
      </div>
    </div>
  {:else}
    <div class="art">
      <div class="report-header" class:show={$currentPage === "Reports"}>
        <button class="close-report" on:click={handleCloseReport}>X</button>
        <div class="report-left">
          <div class="report-name">
            {$viewState.projectKey?.split("\\").length > 2
              ? $viewState.projectKey?.split("\\").at(-1)
              : $viewState.projectKey?.split("/").at(-1)}
          </div>
          <div class="report-info">
            Mean ΔE: {parseFloat(
              $viewState.reports.calibration?.["double_values"]?.[0]?.["data"]
            ).toFixed(4)}
          </div>
          <button class="report-info new-window-button" on:click={() => { window.electron.openNewWindow() }}>View Another Report</button>

          {#if isVerification}
            <div class="report-info">
              Verification Mean ΔE: {parseFloat(
                $viewState.reports.verification?.["double_values"]?.[0]?.[
                  "data"
                ]
              ).toFixed(4)}
            </div>
          {/if}
        </div>
      </div>
      <div class="reports">
        <div class="reportBody">
          <div class="report-item">
            <Heatmap
              data={$viewState.reports.calibration}
              matrixName={"CM DeltaE Values"}
            />
            <div class="target-image-container">
              <ImageViewer srcUrl={$viewState.colorManagedTargetImage.dataURL} identifier="CM_target"/>
            </div>
          </div>
          <div class="report-item">
            <!-- AB vector chart -->
            <VectorChart data={$viewState.reports.calibration} matrix={"CM"} ab={true}></VectorChart>
          </div>
          <div class="report-item">
            <!-- LC vector chart -->
            <VectorChart data={$viewState.reports.calibration} matrix={"CM"} ab={false}></VectorChart>
          </div>
          <div class="report-item">
            <LinearChart
              data={$viewState.reports.calibration}
              matrix={"CM"}
            />
          </div>
        </div>
      </div>
    </div>
  {/if}
</main>

<style lang="postcss" global>
  .reports-main {
    @apply flex flex-col w-full h-full overflow-y-scroll ;
  }
  .art {
    @apply flex flex-col relative;
  }
  .reports {
    @apply w-full h-full flex flex-col gap-2;
  }
  .reportBody {
    transform-origin: left top;
    @apply w-full h-full transition-all duration-300
          bg-gray-700 rounded-b-2xl p-4 flex flex-col items-center justify-center
          gap-2;
  }
  .report-item {
    @apply w-auto h-full flex justify-center items-center bg-gray-800 ring-1 ring-gray-500
          p-4 pr-8 rounded-xl;
  }
  .dropdown-report-btn {
    @apply w-full flex flex-col justify-center items-center z-50;
  }

  .noFile {
    @apply absolute w-full h-full z-[99] flex justify-center bg-gray-800
            items-center top-0 left-0;
  }
  .inputBox {
    @apply w-auto h-auto bg-gray-700 flex flex-col gap-2 justify-center items-center
          p-8 rounded-2xl;
  }
  .inputBox h2 {
    @apply text-xl;
  }
  .report-header {
    width: 100%;
    height: 20vh;
    @apply sticky top-0 bg-gray-800 z-[9999] flex px-[1vw] pr-[5vw] py-4 rounded-b-xl -translate-y-full
            transition-all delay-150 duration-300 ease-in justify-between;
  }
  .close-report {
    @apply absolute top-5 right-0;
  }
  .report-header.show {
    @apply translate-y-0;
  }
  .report-left {
    @apply w-full flex flex-col justify-center;
  }

  .report-right {
    @apply w-full rounded-xl overflow-auto;
  }
  .report-name {
    @apply text-4xl;
  }
  .reports h2 {
    @apply w-full text-3xl;
  }
  .verificationBar {
    @apply w-full h-full bg-gray-500 flex flex-col p-2 rounded-xl;
  }
  .new-window-button{
    align-self: baseline;margin-top: 5px;
  }
  .target-image-container {
    height:450px;
    @apply w-full bg-blue-600/25 relative items-center flex justify-center;
  }
</style>
