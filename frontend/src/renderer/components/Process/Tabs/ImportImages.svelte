<script lang="ts">
  import { processState } from "@util/stores";
  import ImageImporter from "@components/ImageImporter.svelte";
  import {get} from "lodash";
  import Dropbox from "../Dropbox.svelte";
  import { countFields } from "@root/util/storesUtil";
  let imageStack = get($processState, 'artStacks[0].fields');
  let sortedImageCount=0;
  let artImageCount = 0;

  // this helps force a rerender once the imageStack has been updated
  $: if ($processState.currentTab === 1) {
        imageStack = get($processState, 'artStacks[0].fields');
        artImageCount = $processState.artImageCount;
        console.log(artImageCount);
        sortedImageCount = countFields(imageStack);
  }

</script>

<main>
  <left>
    <h1>Import Images</h1>
    <br><br>
    <div>
      {#if sortedImageCount>0}
      <h2>Already Sorted Images</h2>
      <div >
        <div class="inputGroup">
            <div class="imageLetter">A</div>
            <div class="imageLetter">B</div>
        </div>
        <h3 class="text">Object</h3>
        <div class="artImageObjects">
          {#each Array(artImageCount) as count, index (index)}
          <div class="inputGroup">
              <div class="cell"><Dropbox type="image" bind:items={imageStack.imageA[index]} singleItem={true} dragDisabled={true}/></div>
              <div class="cell"><Dropbox type="image" bind:items={imageStack.imageB[index]} singleItem={true} dragDisabled={true}/></div>
          </div>
          <br>
          {/each}
        </div>
      </div>
        <h3 class="text">Target</h3>
        <div class="inputGroup">
          <div class="cell"><Dropbox type="image" bind:items={imageStack.targetA} singleItem={true} dragDisabled={true}/></div>
          <div class="cell"><Dropbox type="image" bind:items={imageStack.targetB} singleItem={true} dragDisabled={true}/></div>
        </div>
        <h3 class="text">FlatField</h3>
        <div class="inputGroup">
          <div class="cell"><Dropbox type="image" bind:items={imageStack.flatfieldA} singleItem={true} dragDisabled={true}/></div>
          <div class="cell"><Dropbox type="image" bind:items={imageStack.flatfieldB} singleItem={true} dragDisabled={true}/></div>
        </div>
        <h3 class="text">DarkField</h3>

        <div class="inputGroup">
          <div class="cell"><Dropbox type="image" bind:items={imageStack.darkfieldA} singleItem={true} dragDisabled={true}/></div>
          <div class="cell"><Dropbox type="image" bind:items={imageStack.darkfieldB} singleItem={true} dragDisabled={true}/></div>
        </div>          
      {/if}
    </div>
  </left>
  <right>
    <ImageImporter/>
  </right>
</main>

<style lang="postcss">
  main {
    @apply flex justify-between h-full w-full overflow-hidden;
  }
  left {
    display:flex;
    flex-direction: column;
    justify-content:start;
    background-color: var(--color-surface-elevated);
    @apply w-full h-full p-6 flex-col overflow-auto;
  }
  right {
    background-color: var(--color-surface);
    @apply w-full h-full p-6 flex flex-col justify-center items-center gap-4;
  }
  h1 {
    @apply text-3xl;
  }
  p {
    background-color: var(--color-overlay-light);
    @apply text-center pt-[30vh] m-4 h-[90%] rounded-lg;
  }
  article {
    background-color: var(--color-surface-sunken);
    @apply w-full min-h-[12rem] overflow-auto rounded-[32px] py-2 px-6;
  }
  .number {
    background-color: var(--color-overlay-light);
    @apply flex justify-center;
  }
  ul {
    @apply flex flex-col gap-2 w-full justify-center items-center;
  }
  .inputGroup {
    display: flex;
    flex-direction: row;
    height: auto;
    gap: 20px;
    width: 100%;
    justify-content: center;
  }
  card {
    width: auto;
    padding-right: 10px;
    padding-left: 10px;
    height: 1.8em;
    border-radius: 10px;
    color: white;
    text-align: center;
    @apply bg-green-400 text-black dark:text-white;
  }
  .text{
    text-align: center;
    margin-top:5px;
    margin-bottom:10px;
  }
  .imageLetter{
    width: 50%;
    text-align: center;
    font-size: 20px;
    margin-top:10px;
  }
  .cell {
    width: 50%;
  }
  section {
    background-color: #2c2c2f;
    height: auto;
    width: 80%;
    min-height: 3.25em;
    margin: auto;
    border-radius: 10px;
    justify-content: center;
    display: flex;
    flex-wrap: wrap;
    gap: 10px;
    align-items: center;
    padding: 10px;
  }
  .artImageObjects{
    max-height:210px;
    overflow-y:auto;
    scrollbar-width: 10px;
  }

  /* width */
  .artImageObjects::-webkit-scrollbar {
    @apply relative transition-all w-1;
  }

  /* Track */
  .artImageObjects::-webkit-scrollbar-track {
    @apply bg-transparent;
  }

  /* Handle */
  .artImageObjects::-webkit-scrollbar-thumb {
    background-color: var(--color-border);
    @apply rounded-full;
  }

  /* Handle on hover */
  .artImageObjects::-webkit-scrollbar-thumb:hover {
    background-color: var(--color-interactive-hover);
  }
    
  
    
</style>
