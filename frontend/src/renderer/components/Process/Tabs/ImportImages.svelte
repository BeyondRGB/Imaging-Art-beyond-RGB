<script lang="ts">
  import { processState } from "@util/stores";
  import ImageImporter from "@components/ImageImporter.svelte";
  import {get} from "lodash";
  import ImageRoleGrid from "@components/ImageRoleGrid.svelte";
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
        <ImageRoleGrid 
          bind:imageStack={imageStack}
          roleType="object"
          artImageCount={artImageCount}
          showHeaders={true}
        />
        <h3 class="text">Object</h3>
        
        <h3 class="text">Target</h3>
        <ImageRoleGrid 
          bind:imageStack={imageStack}
          roleType="target"
          showHeaders={false}
        />
        
        <h3 class="text">FlatField</h3>
        <ImageRoleGrid 
          bind:imageStack={imageStack}
          roleType="flatfield"
          showHeaders={false}
        />
        
        <h3 class="text">DarkField</h3>
        <ImageRoleGrid 
          bind:imageStack={imageStack}
          roleType="darkfield"
          showHeaders={false}
        />
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
    
  
    
</style>
