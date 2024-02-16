<script lang="ts">
  import Dropdown from "@root/components/Dropdown.svelte";
  import { processState } from "@util/stores";
  let sharpingSettings = ["None", "Low", "Medium", "High"];

  let selected = "None";

  $: if ($processState.currentTab === 4 && !$processState.completedTabs[4]) {
    $processState.completedTabs[4] = true;
  }

  $: if (selected) {
    $processState.artStacks[0].sharpenString = selected.slice(0, 1);
  }
</script>

<main>
  <left>
    <h1>Advanced Options</h1>
  </left>
  <right>
    <div class="settings">
      <div class="sharpness">
        <p>Sharpening Level</p>
        <Dropdown values={sharpingSettings} bind:selected isFile={false} />
      </div>
    </div>
  </right>
</main>

<style lang="postcss">
  main {
    @apply flex justify-between h-full w-full overflow-hidden;
  }
  left {
    @apply bg-gray-600 w-full h-full p-6 flex-col;
  }
  right {
    @apply bg-gray-700 w-full h-full p-6 flex justify-center;
  }
  h1 {
    @apply text-3xl;
  }
  .settings {
    @apply w-[75%] flex justify-center items-start;
  }
  .settings .sharpness {
    @apply flex w-full max-w-lg justify-between bg-gray-600 rounded-xl p-2;
  }

  .sharpness p {
    @apply text-xl;
  }
</style>
