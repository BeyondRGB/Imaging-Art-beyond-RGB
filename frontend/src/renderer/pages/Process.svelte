<script lang="ts">
  import { currentPage, processState } from "@util/stores";

  import Settings from "@components/Process/Settings.svelte";
  import ColorTarget from "@root/components/Process/Tabs/ColorTarget.svelte";
  import ImportImages from "@components/Process/Tabs/ImportImages.svelte";
  import SelectDest from "@components/Process/Tabs/SelectDest.svelte";
  import SpecFileRoles from "@components/Process/Tabs/SpecFileRoles.svelte";
  import AdvOpts from "@components/Process/Tabs/AdvOpts.svelte";
  import Layout from "@components/Process/Layout.svelte";
  let tabList;

  let tabs: any = [
    { name: "Import Images", component: ImportImages },
    { name: "Select Destination", component: SelectDest },
    { name: "Specifiy File Roles", component: SpecFileRoles },
    { name: "Advanced Options", component: AdvOpts },
    { name: "Color Target", component: ColorTarget },
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
</script>

<main>
  <nav class="dark:bg-gray-800/25">
    <button id="backBtn" on:click={prevTab}>Back</button>
    <tabs>
      {#each tabs as tab}
        <div
          class="tab {tabs[$processState.currentTab].name !== tab.name
            ? 'none'
            : ''}"
          id={tab.name}
        />
      {/each}
    </tabs>
  </nav>
  <Layout {tabs} bind:tabList />
  <botnav class="dark:bg-transparent">
    {#if tabs[$processState.currentTab + 1]?.name === "Advanced Options"}
      <button on:click={nextTab}>Go to Advanced Options</button>
      <button on:click={() => ($processState.currentTab += 2)} class="nextBtn"
        >Next: Skip Advanced Options</button
      >
    {:else}
      <button on:click={nextTab} class="nextBtn">Next</button>
    {/if}
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
  .component {
    @apply relative float-left flex-shrink-0 block;
  }
  .component:last-of-type {
    @apply z-0;
  }
</style>
