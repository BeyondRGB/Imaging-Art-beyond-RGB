<script lang="ts">
  import {
    currentPage,
    messageStore,
    processState,
    sendMessage,
  } from "@util/stores";

  import Settings from "@components/Process/Settings.svelte";
  import ColorTarget from "@root/components/Process/Tabs/ColorTarget.svelte";
  import ImportImages from "@components/Process/Tabs/ImportImages.svelte";
  import SelectDest from "@components/Process/Tabs/SelectDest.svelte";
  import SpecFileRoles from "@components/Process/Tabs/SpecFileRoles.svelte";
  import AdvOpts from "@components/Process/Tabs/AdvOpts.svelte";
  import Processing from "@root/components/Process/Tabs/Processing.svelte";
  import Layout from "@components/Process/Layout.svelte";
  let tabList;

  let showDialog = false;

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
  $: if ($messageStore.length > 1) {
    try {
      let temp = JSON.parse($messageStore[0]);
      console.log(temp);
      if (temp["RequestType"] === "HalfSizePreview") {
        console.log("HalfSizedPreview From Server");
        $processState.artStacks[0].colorTargetImage = temp.RequestData;
      } else if (temp["ResponseType"] === "image") {
        $processState.outputImage = temp["ResponseData"];
      }
    } catch (e) {
      console.log(e);
    }
  }

  function colorTargetPrev() {
    let msg = {
      RequestID: 2434234,
      RequestType: "HalfSizePreview",
      RequestData: {
        filenames: [$processState.artStacks[0].fields.images[0].name],
      },
    };
    if ($processState.artStacks[0].fields.images[0].name.length > 2) {
      console.log("Getting Color Target Preview");
      sendMessage(JSON.stringify(msg));
    }
  }

  function handleConfirm() {
    showDialog = false;

    if ($processState.currentTab !== tabs.length - 1) {
      $processState.currentTab += 1;
    } else {
      console.log("Error overflow");
    }
  }
</script>

<main>
  <nav class="dark:bg-gray-800/25">
    <button id="backBtn" on:click={prevTab}>Back</button>
    {#if !tabs[$processState.currentTab].hidden}
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
    {/if}
  </nav>

  <Layout {tabs} bind:tabList />
  <botnav class="dark:bg-transparent">
    {#if tabs[$processState.currentTab + 1]?.name === "Advanced Options"}
      <button on:click={nextTab}>Go to Advanced Options</button>
      <button
        on:click={() => {
          colorTargetPrev();
          $processState.currentTab += 2;
        }}
        class="nextBtn">Next: Skip Advanced Options</button
      >
    {:else if tabs[$processState.currentTab + 1]?.name === "Processing"}
      <button on:click={() => (showDialog = true)} class="nextBtn"
        >Confirm</button
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
    @apply absolute bg-black/25 z-50 items-center justify-center w-full h-full hidden;
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
    @apply w-16 h-1 rounded-full bg-blue-400 self-center mx-2;
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
