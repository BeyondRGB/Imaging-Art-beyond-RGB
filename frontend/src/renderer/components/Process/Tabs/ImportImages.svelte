<script lang="ts">
  import { processState, sendMessage, messageStore } from "@util/stores";
  import FileSelector from "@components/FileSelector.svelte";
  import ImageBubble from "@components/Process/ImageBubble.svelte";

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

  function getThumbnails() {
    console.log("Getting Thumbnails");
    $processState.thumbnailID = Math.floor(Math.random() * 999999999);
    let msg = {
      RequestID: $processState.thumbnailID,
      RequestType: "Thumbnails",
      RequestData: {
        names: filePaths,
      },
    };
    console.log(msg);
    sendMessage(JSON.stringify(msg));
    filePaths = [];
  }

  $: if (filePaths?.length > 0) {
    console.log("Fetching Thumbnails");
    getThumbnails();
  }

  $: if (
    $processState.imageFilePaths.length >= 6 &&
    !$processState.completedTabs[0]
  ) {
    $processState.completedTabs[0] = true;
  }
</script>

<main>
  <left>
    <h1>Import Images</h1>
    <p>Select the image set you would like to process</p>
  </left>
  <right>
    <div class="fileSelector">
      <FileSelector bind:filePaths filter="raws" largeText />
    </div>
    <article>
      <ul>
        {#if $processState.imageFilePaths?.length > 0}
          {#each $processState.imageFilePaths as filePath}
            <ImageBubble filename={filePath.name} />
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
    @apply bg-gray-700 w-full h-full p-6 flex flex-col justify-center items-center gap-4;
  }
  h1 {
    @apply text-3xl;
  }
  p {
    @apply text-center pt-[30vh] bg-gray-500/25 m-4 h-[90%] rounded-lg;
  }
  article {
    @apply bg-gray-800 w-full min-h-[12rem] overflow-auto rounded-[32px] py-2 px-6;
  }
  .number {
    @apply flex justify-center bg-gray-700/50;
  }
  ul {
    @apply flex flex-col gap-2 w-full justify-center items-center;
  }
</style>
