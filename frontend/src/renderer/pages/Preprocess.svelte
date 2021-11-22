<script>
  import Settings from "@components/Process/Settings.svelte";
  import ColorTarget from "@components/Process/ColorTarget.svelte";
  let currentTab = Settings;

  function nextTab(current) {
    if (current === "ColorTarget") {
      // Set Page
    } else {
      console.log("next tab");
      currentTab = ColorTarget;
    }
  }
  function prevTab(current) {
    if (current === "ColorTarget") {
      currentTab = ColorTarget;
    } else {
      // Set Page
    }
  }

  $: console.log();
</script>

<main>
  <nav>
    <button id="backBtn">Back</button>
    <tabs>
      <div class="tab" id="tab1" />
      <div class="tab {currentTab === Settings ? 'none' : ''}" id="tab2" />
    </tabs>
  </nav>
  {#key currentTab}
    <div class="page">
      <svelte:component this={currentTab} {nextTab} />
    </div>
  {/key}
</main>

<style lang="postcss" local>
  main {
    @apply relative h-full overflow-hidden;
  }
  nav {
    @apply flex h-12 border-2;
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
    @apply absolute h-8 py-0 ml-1;
  }
  .tab.none {
    @apply bg-gray-600;
  }
</style>
