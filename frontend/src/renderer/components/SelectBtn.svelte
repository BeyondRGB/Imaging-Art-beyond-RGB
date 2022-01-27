<script lang="ts">
  import {
    computePosition,
    autoPlacement,
    shift,
    offset,
  } from "@floating-ui/dom";
  import { appSettings } from "@util/stores";

  $: theme = $appSettings.theme ? "dark" : "";
  let show = false;

  let btnRef;
  let popRef;

  const toggleDropdown = () => {
    if (show) {
      show = false;
    } else {
      show = true;
      computePosition(btnRef, popRef, {
        placement: "bottom",
        middleware: [offset(4), autoPlacement({ crossAxis: true })],
      }).then(({ x, y }) => {
        Object.assign(popRef.style, {
          left: "0",
          top: "0",
          transform: `translate(${Math.round(x)}px,${Math.round(y)}px)`,
        });
      });
    }
  };

  let temp;
  $: console.log(popRef);
</script>

{#if show}
  <div class="background" on:click={() => (show = false)} />
{/if}
<main class={theme}>
  <button on:click={toggleDropdown} bind:this={btnRef}>Dropdown</button>
  <hr />
  <div bind:this={popRef} class={show ? "shown" : ""}>
    <li>Float</li>
    <li>Trust</li>
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex flex-wrap;
  }
  button {
    @apply bg-blue-600/50 z-20;
  }
  div {
    @apply z-50 float-left hidden;
  }
  .shown {
    @apply block w-36 dark:bg-gray-700 bg-gray-50 rounded-md fixed;
  }
  li {
    @apply list-none hover:text-blue-400 px-2 py-1;
  }
  .background {
    @apply fixed top-0 left-0 w-full h-full z-10 block float-none;
  }
</style>
