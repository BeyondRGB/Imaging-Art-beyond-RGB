<script lang="ts">
  import {
    currentPage,
    messageStore,
    processState,
    sendMessage,
    viewState,
  } from "@util/stores";

  import Settings from "@components/Process/Settings.svelte";
  import ColorTarget from "@root/components/Process/Tabs/ColorTarget.svelte";
  import ImportImages from "@components/Process/Tabs/ImportImages.svelte";
  import SelectDest from "@components/Process/Tabs/SelectDest.svelte";
  import SpecFileRoles from "@components/Process/Tabs/SpecFileRoles.svelte";
  import AdvOpts from "@components/Process/Tabs/AdvOpts.svelte";
  import Processing from "@root/components/Process/Tabs/Processing.svelte";
  import Layout from "@components/Process/Layout.svelte";
  import { time_ranges_to_array } from "svelte/internal";
  let tabList;

  let showDialog = false;
  let binaryType = null;
  let binaryName = null;
  let binaryFor = null;
  let binaryID = null;

  let tabs: any = [
    { name: "Import Images", component: ImportImages },
    { name: "Select Destination", component: SelectDest },
    { name: "Specifiy File Roles", component: SpecFileRoles },
    { name: "Advanced Options", component: AdvOpts },
    { name: "Color Target", component: ColorTarget },
    { name: "Processing", component: Processing, hidden: true },
  ];

  function nextTab() {
    if ($processState.currentTab !== tabs.length - 1) {
      $processState.currentTab += 1;
    } else {
      console.log("Error overflow");
    }
  }

  function prevTab() {
    if ($processState.currentTab !== 0) {
      $processState.currentTab -= 1;
    } else {
      console.log("Error overflow");
    }
  }

  $: if (tabList) {
    let width = tabList.scrollWidth;

    tabList.scroll({
      top: 0,
      left: $processState.currentTab * (width / tabs.length),
      behavior: "smooth",
    });
  }

  $: if ($messageStore.length > 1 && !($messageStore[0] instanceof Blob)) {
    console.log($messageStore[0]);
    console.log("New Message");
    try {
      let temp = JSON.parse($messageStore[0]);
      if (temp["ResponseType"] === "CalibrationComplete") {
        // Project Key handler
        console.log("CalibrationComplete Project Key From Server");
        $viewState.projectKey = temp["ResponseData"]["path"];
      } else if (
        // Thumbnail Binary Handler
        temp["ResponseType"] === "ImageBinary" &&
        $viewState.colorManagedImages.hasOwnProperty(temp["RequestID"])
      ) {
        console.log("Color Managed Binary From Server");
        binaryType = temp["ResponseData"]["type"];
        binaryName = temp["ResponseData"]["name"];
        binaryID = temp["RequestID"];
        binaryFor = "ColorManaged";
      } else if (
        // Thumbnail Binary Handler
        temp["ResponseType"] === "ImageBinary" &&
        temp["RequestID"] === $processState.thumbnailID
      ) {
        console.log("Thumbnail Binary From Server");
        binaryType = temp["ResponseData"]["type"];
        binaryName = temp["ResponseData"]["name"];
        binaryFor = "Thumbnail";
      } else if (temp["ResponseType"] === "ImageBinary") {
        // Color target and output binary handler
        console.log("Binary From Server");
        binaryType = temp["ResponseData"]["type"];
        binaryName = temp["ResponseData"]["name"];
        binaryFor = "Output";
      } else if (temp["RequestID"] === $processState.colorTargetID) {
        // Base64 Halfsize handler
        console.log("HalfSizedPreview From Server");
        $processState.artStacks[0].colorTargetImage = temp["ResponseData"];
        $processState.colorTargetID = null;
      } else if (
        // Base64 Thumbnail Handler
        temp["ResponseType"] === "ImageBase64" &&
        temp["RequestID"] === $processState.thumbnailID
      ) {
        console.log("Thumbnail Base64 From Server");
        $processState.imageThumbnails[temp["ResponseData"]["name"]] =
          temp["ResponseData"]["dataURL"];
      } else if (temp["ResponseType"] === "ImageBase64") {
        // base64 output handler
        console.log("Base64 From Server");
        $processState.outputImage = temp["ResponseData"];
      }
    } catch (e) {
      console.log(e);
    }
  }

  $: if ($messageStore.length > 1 && $messageStore[0] instanceof Blob) {
    console.log("creating blob");
    let blob = $messageStore[0].slice(0, $messageStore[0].size, binaryType);
    let temp = new Image();
    temp.src = URL.createObjectURL(blob);

    if (binaryFor === "Thumbnail") {
      $processState.imageThumbnails[binaryName] = temp.src;
    } else if (binaryFor === "Output") {
      $processState.artStacks[0].colorTargetImage = {
        dataURL: temp.src,
        filename: binaryName,
      };
      $processState.outputImage = {
        dataURL: temp.src,
        name: binaryName,
      };
    } else if (binaryFor === "ColorManaged") {
      $viewState.colorManagedImages[binaryID]["dataURL"] = temp.src;
      $viewState.colorManagedImages[binaryID]["filename"] = binaryName;
    }
    binaryType = null;
    binaryName = null;
    binaryID = null;
  }

  $: processRequest = {
    RequestType: "Process",
    RequestID: Date.now(),
    RequestData: {
      images: [
        {
          art: $processState.artStacks[0].fields.images[0]?.name,
          white: $processState.artStacks[0].fields.whitefield[0]?.name,
          dark: $processState.artStacks[0].fields.darkfield[0]?.name,
        },
        {
          art: $processState.artStacks[0].fields.images[1]?.name,
          white: $processState.artStacks[0].fields.whitefield[1]?.name,
          dark: $processState.artStacks[0].fields.darkfield[1]?.name,
        },
      ],
      destinationDirectory: $processState.destDir,
      targetLocation: $processState.artStacks[0].colorTarget,
    },
  };

  $: console.log($processState.artStacks[0].colorTarget);
  $: console.log({ processRequest });

  $: {
    if (processRequest.RequestData.targetLocation["refData"] !== undefined) {
      processRequest.RequestData.targetLocation["refData"][
        "standardObserver"
      ] = 1931;
      processRequest.RequestData.targetLocation["refData"]["illuminants"] =
        "D50";
    }
    if ($processState.artStacks[0].verificationTarget !== null) {
      console.log("Adding Verification to Process Request");
      console.log($processState.artStacks[0].verificationTarget);
      processRequest.RequestData["verificationLocation"] =
        $processState.artStacks[0].verificationTarget;
      if (processRequest.RequestData.targetLocation["refData"] !== undefined) {
        processRequest.RequestData["verificationLocation"]["refData"][
          "standardObserver"
        ] = 1931;
        processRequest.RequestData["verificationLocation"]["refData"][
          "illuminants"
        ] = "D50";
      }
    }
  }

  function handleConfirm() {
    showDialog = false;

    if ($processState.currentTab !== tabs.length - 1) {
      $processState.currentTab += 1;
      console.log("Sening Process Request");
      console.log(processRequest);
      setTimeout(() => {
        sendMessage(JSON.stringify(processRequest));
      }, 150);
    } else {
      console.log("Error overflow");
    }
  }
</script>

<main>
  {#if !tabs[$processState.currentTab].hidden}
    <nav class="dark:bg-gray-800/25">
      <button id="backBtn" on:click={prevTab}>Back</button>

      <tabs>
        {#each tabs as tab}
          {#if !tab.hidden}
            <div
              class="tab {tabs[$processState.currentTab].name !== tab.name
                ? 'none'
                : ''}"
              id={tab.name}
            />
          {/if}
        {/each}
      </tabs>
    </nav>
  {/if}
  <Layout {tabs} bind:tabList />
  <botnav class="dark:bg-transparent">
    {#if tabs[$processState.currentTab + 1]?.name === "Advanced Options"}
      <button on:click={nextTab}>Go to Advanced Options</button>
      <button
        on:click={() => {
          $processState.currentTab += 2;
        }}
        class="nextBtn">Next: Skip Advanced Options</button
      >
    {:else if tabs[$processState.currentTab + 1]?.name === "Processing"}
      <button on:click={() => (showDialog = true)} class="nextBtn"
        >Begin Processing</button
      >
    {:else if tabs[$processState.currentTab].hidden}
      <br />
    {:else}
      <button on:click={nextTab} class="nextBtn">Next</button>
    {/if}
  </botnav>

  <div class={`confirmModal ${showDialog ? "show" : ""}`}>
    <div class="confirmDialog">
      <span class="warning">Warning:</span>
      <p>
        Are you sure all information is accurate, you will not be able to go
        back
      </p>
      <div class="btnGroup">
        <button class="cancel" on:click={() => (showDialog = false)}
          >Cancel</button
        >
        <button class="confirm" on:click={handleConfirm}>Confirm</button>
      </div>
    </div>
  </div>
</main>

<style lang="postcss" local>
  main {
    @apply relative h-full w-full overflow-hidden flex flex-col min-h-0;
  }
  nav {
    @apply flex h-12;
  }
  botnav {
    @apply absolute h-10 bottom-2 flex items-center right-0;
  }
  tabs {
    @apply flex-grow justify-center flex mr-[5vw];
  }
  .nextBtn {
    @apply m-4 bg-green-700 hover:bg-green-600 focus:ring-green-600 transition-all;
  }
  .page {
    overflow: overlay;
    @apply w-full h-full flex overflow-x-auto;
  }

  .confirmModal {
    @apply absolute bg-black/75 z-50 items-center justify-center w-full h-full hidden;
  }

  .show {
    @apply flex;
  }

  .confirmDialog {
    @apply bg-gray-700 w-1/2 h-1/3 text-xl rounded-xl p-4 flex flex-col justify-between;
  }

  .confirmDialog p {
    @apply bg-gray-800/25 rounded-md flex justify-center p-2;
  }

  .warning {
    @apply text-red-500 font-bold text-2xl flex items-center justify-center;
  }

  .btnGroup {
    @apply flex justify-end gap-2;
  }

  .confirm {
    @apply bg-green-600 hover:bg-green-500 focus:ring-green-600 transition-all;
  }

  .cancel {
    @apply bg-gray-500 hover:bg-blue-500/50 focus:ring-green-600 transition-all;
  }
  /* div {
    @apply w-full h-full;
  } */
  .tab {
    @apply w-16 h-2 rounded-full bg-blue-400 self-center mx-2 ring-1 ring-blue-400
          transition-all duration-700 ease-out;
  }
  #backBtn {
    @apply absolute h-8 py-0 ml-2 my-2;
  }
  .tab.none {
    @apply bg-gray-600;
  }
  .component {
    @apply relative float-left flex-shrink-0 block;
  }
  .component:last-of-type {
    @apply z-0;
  }
</style>
