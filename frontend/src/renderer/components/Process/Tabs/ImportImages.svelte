<script lang="ts">
  import { processState } from "@util/stores";
  import FileSelector from "@components/FileSelector.svelte";

  let filePaths = [];
  $: console.log(filePaths);
  $: if (filePaths) {
    $processState.imageFilePaths = filePaths.map((path) => {
      return {
        id: (
          path.split("").reduce((a, b) => {
            a = (a << 5) - a + b.charCodeAt(0);
            return a & a;
          }, 0) + Math.pow(2, 31)
        ).toString(16),
        name: path,
      };
    });
  }
</script>

<main>
  <left>
    <h1>Import Images</h1>
    <p>
      Select the folder or all images you would like to import into the program
    </p>
  </left>
  <right>
    <div class="fileSelector">
      <FileSelector bind:filePaths />
    </div>
    <article>
      {filePaths ? filePaths.length : 0} / 6
      <ul>
        {#if filePaths?.length > 0}
          {#each filePaths as filePath}
            <li>
              {filePath.length > 60
                ? `${filePath.substring(0, 20)}...${filePath.substring(
                    filePath.length - 40
                  )}`
                : filePath}
            </li>
          {/each}
        {/if}
      </ul>
    </article></right
  >
</main>

<style lang="postcss">
  main {
    @apply flex justify-between h-full w-full overflow-hidden;
  }
  left {
    @apply bg-gray-600 w-full h-full p-6 flex-col overflow-auto;
  }
  right {
    @apply bg-gray-700 w-full h-full p-6 flex flex-col justify-center items-center;
  }
  h1 {
    @apply text-3xl;
  }
  p {
    @apply text-center pt-[30vh] bg-gray-500/25 m-4 h-[90%] rounded-lg;
  }
  article {
    @apply bg-gray-800 h-full w-full m-6 overflow-auto;
  }
  li {
    @apply bg-gray-700 m-1 p-2;
  }
</style>
