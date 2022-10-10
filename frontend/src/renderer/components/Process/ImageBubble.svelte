<script lang="ts">
  import {
    computePosition,
    autoPlacement,
    shift,
    offset,
    flip,
    inline,
  } from "@floating-ui/dom";
  import { processState } from "@util/stores";
  import { ImageIcon } from "svelte-feather-icons";

  export let filename = "Empty...";
  export let minimal = false;

  let show = false;
  let btnRef;
  let popRef;

  const toggleDropdown = () => {
    if (show) {
      show = false;
    } else {
      show = true;
    }
  };
</script>

<main class="group">
  <div
    class:bubble={!minimal}
    on:mouseenter={toggleDropdown}
    on:mouseleave={toggleDropdown}
    bind:this={btnRef}
  >
    {#if minimal}
      {filename.split("\\").length > 2
        ? filename.split("\\").at(-1)
        : filename.split("/").at(-1)}
    {:else}
      <ImageIcon size="1.5x" />
      {filename}
    {/if}
  </div>
  <!-- class:shown={show} -->
  <!-- bind:this={popRef} -->
  <div class="body">
    <img src={$processState.imageThumbnails[filename]} alt={filename} />
  </div>
</main>

<style lang="postcss">
  main {
    @apply w-full h-full;
  }
  .bubble {
    @apply bg-gray-600 py-2 rounded-3xl hover:bg-gray-500/25
            flex justify-between items-center text-[1.05rem] px-4 gap-1;
  }
  .body {
    @apply bg-gray-700/75 w-0 opacity-0 fixed max-w-[35vw] p-1
           group-hover:w-full group-hover:opacity-100 rounded-md transition-all 
           bottom-0 left-24 delay-100;
  }
  img {
    @apply w-full h-full;
  }
</style>
