<script>
  let files;
  let outputFiles;

  import { currentPage } from "../stores";

  import Sorter from "@components/Sorter.svelte";
  import FileSelector from "@components/FileSelector.svelte";
  import PageTransitions from "@components/PageTransitions.svelte";

  $: console.log(files);
</script>

<PageTransitions>
  <main>
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
</PageTransitions>

<style lang="postcss">
  main {
    @apply w-full px-[10%] pt-[5%] items-start justify-center;
  }

  .next {
    @apply mb-12 bg-blue-500 rounded-md self-center text-gray-200 text-lg px-4 py-2 mt-auto;
  }
</style>
