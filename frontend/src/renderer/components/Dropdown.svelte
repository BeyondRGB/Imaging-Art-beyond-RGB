<script lang="ts">
  import {
    computePosition,
    autoPlacement,
    shift,
    offset,
    flip,
    inline,
  } from "@floating-ui/dom";
  export let values: string[];
  export let selected;

  let show = false;

  let btnRef;
  let popRef;

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

  $: console.log({ SHOW: show });
</script>

{#if show}
  <div class="background" on:click={() => (show = false)} />
{/if}
<main>
  <button on:click={toggleDropdown} bind:this={btnRef}
    >{selected.slice(0, -4)}</button
  >
  <hr />
  <div class="body" bind:this={popRef} class:shown={show}>
    {#each values as value}
      <li
        on:click={() => {
          selected = value;
          show = false;
        }}
      >
        {value.slice(0, -4)}
      </li>
    {/each}
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex justify-center;
  }
  button {
    @apply bg-blue-600/50 z-50 text-sm w-full;
  }
  .body {
    @apply bg-gray-600 z-50 hidden fixed;
  }
  .shown {
    @apply block bg-gray-600 rounded-md fixed;
  }
  li {
    @apply list-none hover:text-blue-300 px-2 py-1 hover:bg-blue-800/25  rounded-md;
  }
  .background {
    @apply fixed top-0 left-0 w-full h-full z-10 block float-none;
  }
</style>
