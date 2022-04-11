<script lang="ts">
  import {
    computePosition,
    autoPlacement,
    shift,
    offset,
    flip,
    inline,
    autoUpdate,
  } from "@floating-ui/dom";
  import { processState } from "@util/stores";
  import { onDestroy, onMount } from "svelte";
  import { ImageIcon } from "svelte-feather-icons";

  export let filename = "Empty...";
  export let minimal = false;

  let show = false;
  let btnRef;
  let popRef;

  // async function update() {
  //   const { x, y } = await computePosition(btnRef, popRef);
  // }

  // function cleanup() {
  //   autoUpdate(btnRef, popRef, update);
  // }

  // onDestroy(() => {
  //   console.log("Cleaing Up Autoupdate");
  //   cleanup();
  // });

  // onMount(() => {
  //   update();
  // });

  const toggleDropdown = () => {
    if (show) {
      show = false;
    } else {
      show = true;
      computePosition(btnRef, popRef, {
        strategy: "fixed",
        placement: "bottom",
        middleware: [
          offset({
            mainAxis: 4,
            // crossAxis: -50,
          }),
          inline(),
          autoPlacement(),
        ],
      }).then(({ x, y }) => {
        Object.assign(popRef.style, {
          left: "0",
          top: "0",
          transform: `translate(${Math.round(x)}px,${Math.round(y)}px)`,
        });
      });
    }
  };
</script>

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

<div class="body" class:small={minimal} bind:this={popRef} class:shown={show}>
  <img src={$processState.imageThumbnails[filename]} alt={filename} />
</div>

<style lang="postcss">
  .bubble {
    @apply bg-gray-600 py-2 rounded-3xl hover:bg-gray-500/25
            flex justify-between items-center text-[1.05rem] px-4 gap-1;
  }
  .body {
    @apply bg-gray-600 z-50 hidden fixed max-w-[35vw] p-1;
  }
  .small {
    @apply max-w-[25vw];
  }
  img {
    @apply w-full h-full;
  }
  .shown {
    @apply block rounded-md fixed bg-gray-700/75;
  }
</style>
