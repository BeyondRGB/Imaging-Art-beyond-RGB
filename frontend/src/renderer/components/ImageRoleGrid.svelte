<script lang="ts">
  /**
   * Generic Image Role Grid Component
   * Handles A/B image pair layouts for different image roles
   */
  import Dropbox from "./Process/Dropbox.svelte";
  import ScrollContainer from "./ScrollContainer.svelte";
  
  export let imageStack: any;
  export let roleType: 'object' | 'target' | 'flatfield' | 'darkfield' = 'object';
  export let artImageCount: number = 1;
  export let showHeaders: boolean = true;
  
  // Map role types to imageStack field names
  const roleFieldMap = {
    'object': { a: 'imageA', b: 'imageB' },
    'target': { a: 'targetA', b: 'targetB' },
    'flatfield': { a: 'flatfieldA', b: 'flatfieldB' },
    'darkfield': { a: 'darkfieldA', b: 'darkfieldB' }
  };
  
  $: fields = roleFieldMap[roleType];
</script>

{#if showHeaders}
  <div class="input-group">
    <div class="image-letter">A</div>
    <div class="image-letter">B</div>
  </div>
{/if}

{#if roleType === 'object'}
  <ScrollContainer maxHeight="210px" className="art-image-objects">
    {#each Array(artImageCount) as count, index (index)}
      <div class="input-group">
        <div class="cell">
          <Dropbox 
            type="image" 
            bind:items={imageStack[fields.a][index]} 
            singleItem={true} 
            dragDisabled={true}
          />
        </div>
        <div class="cell">
          <Dropbox 
            type="image" 
            bind:items={imageStack[fields.b][index]} 
            singleItem={true} 
            dragDisabled={true}
          />
        </div>
      </div>
      <br>
    {/each}
  </ScrollContainer>
{:else}
  <div class="input-group">
    <div class="cell">
      <Dropbox 
        type="image" 
        bind:items={imageStack[fields.a]} 
        singleItem={true} 
        dragDisabled={true}
      />
    </div>
    <div class="cell">
      <Dropbox 
        type="image" 
        bind:items={imageStack[fields.b]} 
        singleItem={true} 
        dragDisabled={true}
      />
    </div>
  </div>
{/if}

<style lang="postcss">
  .input-group {
    display: flex;
    flex-direction: row;
    height: auto;
    gap: 20px;
    width: 100%;
    justify-content: center;
  }
  
  .image-letter {
    width: 50%;
    text-align: center;
    font-size: 20px;
    margin-top: 10px;
  }
  
  .cell {
    width: 50%;
  }
  
  :global(.art-image-objects) {
    scrollbar-width: 10px;
  }
</style>

