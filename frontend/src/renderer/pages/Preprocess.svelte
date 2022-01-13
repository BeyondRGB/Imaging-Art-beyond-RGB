<script lang="ts">
  import { currentPage } from "@util/stores";
  import Settings from "@components/Process/Settings.svelte";
  import ColorTarget from "@components/Process/ColorTarget.svelte";
  import ImportImages from "@components/Preprocess/Tabs/ImportImages.svelte";
  import SelectDest from "@components/Preprocess/Tabs/SelectDest.svelte";
  import SpecFileRoles from "@components/Preprocess/Tabs/SpecFileRoles.svelte";
  import AdvOpts from "@components/Preprocess/Tabs/AdvOpts.svelte";
  import { fade } from "svelte/transition";
  let currentTab: number = 0;
  let currName: string = "Settings";

  let tabs: any = [
    { name: "Import Images", component: ImportImages },
    { name: "Select Destination", component: SelectDest },
    { name: "Specifiy File Roles", component: SpecFileRoles },
    { name: "Advanced Options", component: AdvOpts },
  ];

  function nextTab() {
    if (currentTab !== tabs.length - 1) {
      currentTab += 1;
    } else {
      console.log("Error overflow");
    }
  }

  function prevTab() {
    if (currentTab !== 0) {
      currentTab -= 1;
    } else {
      console.log("Error overflow");
    }
  }
</script>

<main>
  <nav class="dark:bg-gray-800/25">
    <button id="backBtn" on:click={prevTab}>Back</button>
    <tabs>
      {#each tabs as tab}
        <div
          class="tab {tabs[currentTab].name !== tab.name ? 'none' : ''}"
          id={tab.name}
        />
      {/each}
      <!-- <div class="tab" id="tab1" />
      <div class="tab {currentTab === 0 ? 'none' : ''}" id="tab2" /> -->
    </tabs>
  </nav>
  <div class="page">
    {#key currentTab}
      <div in:fade={{ duration: 200, delay: 200 }} out:fade={{ duration: 200 }}>
        <svelte:component this={tabs[currentTab].component} />
      </div>
    {/key}
  </div>
  <botnav class="dark:bg-transparent">
    <button on:click={nextTab} class="nextBtn">Next</button>
  </botnav>
</main>

<style lang="postcss" local>
  main {
    @apply relative h-full w-full overflow-hidden flex flex-col min-h-0;
  }
  nav {
    @apply flex h-12;
  }
  botnav {
    @apply flex h-10 justify-end;
  }
  tabs {
    @apply flex-grow justify-center flex mr-[5vw];
  }
  .nextBtn {
    @apply m-4;
  }
  .page {
    overflow: overlay;
    @apply w-full h-full;
  }
  div {
    @apply w-full h-full;
  }
  .tab {
    @apply w-16 h-1 rounded-full bg-blue-400 self-center mx-2;
  }
  #backBtn {
    @apply absolute h-8 py-0 ml-2 my-2;
  }
  .tab.none {
    @apply bg-gray-600;
  }
</style>
