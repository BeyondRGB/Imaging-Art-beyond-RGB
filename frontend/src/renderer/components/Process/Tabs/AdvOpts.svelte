<script lang="ts">
  import Dropdown from "@root/components/Dropdown.svelte";
  import { processState } from "@util/stores";
  // let sharpingSettings = ["None", "Low", "Medium", "High"];
  let sharpingSettings = [
    {
      name: "None",
      value: "N",
    },
    {
      name: "Low",
      value: "L",
    },
    {
      name: "Medium",
      value: "M",
    },
    {
      name: "High",
      value: "H",
    },
  ]

  let selected = sharpingSettings[0];

  $: if ($processState.currentTab === 4 && !$processState.completedTabs[4]) {
    $processState.completedTabs[4] = true;
  }

  $: if (selected) {
    $processState.artStacks[0].sharpenString = selected.value;
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
        <Dropdown values={sharpingSettings} bind:selected />
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
