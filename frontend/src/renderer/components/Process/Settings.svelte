<script lang="ts">
  let files: string[];
  let outputDir: string;
  export let nextTab: any;

  import Sorter from "@components/Preprocess/Sorter.svelte";
  // import FileSelector from "@components/Preprocess/FileSelector.svelte";

  $: console.log(files);
</script>

<main class="dark:text-white">
  <div class="settings">
    <!-- <FileSelector title="Select Files:" />
    <FileSelector title="Export Destination:" directory /> -->
    <div>
      Specify File Roles:
      <Sorter />
    </div>
    <br />
    <div>Advanced: [WIP]</div>
    <button class="next" on:click={() => nextTab("Settings")}>Next</button>
  </div>
</main>

<style lang="postcss">
  main {
    @apply w-full h-full px-[10%] pt-[5%] items-start justify-center;
  }
  .next {
    @apply mb-12;
  }
</style>
