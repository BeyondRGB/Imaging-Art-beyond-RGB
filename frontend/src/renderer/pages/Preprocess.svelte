<script lang="ts">
  import { currentPage } from "@util/stores";
  import Settings from "@components/Process/Settings.svelte";
  import ColorTarget from "@components/Process/ColorTarget.svelte";
  import { fade } from "svelte/transition";
  let currentTab: any = Settings;
  let currName: string = "Settings";

  function nextTab(current: string) {
    if (current === "ColorTarget") {
      // Set Page
    } else {
      console.log("next tab");
      currentTab = ColorTarget;
      currName = "ColorTarget";
    }
  }
  function prevTab(current: string) {
    if (current === "ColorTarget") {
      currentTab = Settings;
      currName = "Settings";
    } else {
      // Set Page
      currentPage.set("Home");
    }
  }
</script>

<main>
  <nav class="dark:bg-gray-800/25">
    <button id="backBtn" on:click={() => prevTab(currName)}>Back</button>
    <tabs>
      <div class="tab" id="tab1" />
      <div class="tab {currentTab === Settings ? 'none' : ''}" id="tab2" />
    </tabs>
  </nav>
  {#key currentTab}
    <div
      class="page"
      in:fade={{ duration: 200, delay: 200 }}
      out:fade={{ duration: 200 }}
    >
      <svelte:component this={currentTab} {nextTab} />
    </div>
  {/key}
</main>

<style lang="postcss" local>
  main {
    @apply relative h-full w-full overflow-hidden;
  }
  nav {
    @apply flex h-12;
  }
  tabs {
    @apply flex-grow justify-center flex mr-[5vw];
  }
  .page {
    overflow: overlay;
    @apply w-full h-full;
  }
  .tab {
    @apply w-16 h-2 rounded-full bg-green-400 self-center mx-2 ring-1 ring-gray-600;
  }
  #backBtn {
    @apply absolute h-8 py-0 ml-2 my-2;
  }
  .tab.none {
    @apply bg-gray-600;
  }
</style>
