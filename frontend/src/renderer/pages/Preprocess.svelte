<script>
  let files;
  let outputFiles;

  import { currentPage } from "@util/stores";

  import Sorter from "@components/Sorter.svelte";
  import FileSelector from "@components/FileSelector.svelte";

  $: console.log(files);
</script>

<main class="dark:text-white">
  <div class="settings">
    <FileSelector title="Select Files:" />
    <FileSelector title="Export Destination:" directory />
    <div>
      Specify File Roles:
      <Sorter />
    </div>
    <br />
    <div>Advanced: [WIP]</div>
    <button class="next" on:click={() => currentPage.set("ColorTarget")}
      >Next</button
    >
  </div>
</main>

<style lang="postcss">
  main {
    @apply w-full px-[10%] pt-[5%] items-start justify-center;
  }

  .next {
    @apply mb-12;
  }
</style>
