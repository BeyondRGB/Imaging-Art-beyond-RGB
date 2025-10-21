<script lang="ts">

  type DropdownOption = {
    name: string,
    fileName: string,
    standardObserver: number,
    illuminants: string
  }

  import {
    computePosition,
    autoPlacement,
    shift,
    offset,
    flip,
    inline,
  } from "@floating-ui/dom";
  import { ChevronDownIcon, ChevronUpIcon } from "svelte-feather-icons";
  export let values: Array<DropdownOption>;
  export let selected: DropdownOption;
  export let invalid = false;
  //export let isFile = true;

  let show = false;
  export let spaceLast = false;
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
</script>

{#if show}
  <div class="background" on:click={() => (show = false)} />
{/if}

<button
  class="refDataBtn"
  class:invalid
  on:click={toggleDropdown}
  bind:this={btnRef}
  ><span class="btnText">{selected.name}</span>
  <div class="iconSqu">
    {#if show}
      <ChevronUpIcon size="1.5x" />
    {:else}
      <ChevronDownIcon size="1.5x" />
    {/if}
  </div></button
>

<div class="body" bind:this={popRef} class:shown={show}>
  {#each values as value, i}
    {#if spaceLast && i === values.length - 1}
      <div class="sep" />
    {/if}
    <li
      on:click={() => {
        selected = value;
        show = false;
      }}
    >
      {value.name}
    </li>
  {/each}
</div>

<style lang="postcss">
  .refDataBtn {
    @apply bg-gray-700 z-50 text-sm flex justify-between items-center
          gap-1 p-0 pl-3 rounded-lg hover:bg-gray-600 transition-colors;
  }
  .invalid {
    @apply ring-2 ring-red-500;
  }
  .body {
    @apply bg-gray-700 z-50 hidden fixed border border-gray-600;
  }
  .shown {
    @apply block bg-gray-700 rounded-lg fixed shadow-xl;
  }
  li {
    @apply list-none text-gray-200 hover:text-gray-50 px-3 py-2 hover:bg-gray-600 rounded-md cursor-pointer;
  }
  .background {
    @apply fixed top-0 left-0 w-full h-full z-10 block float-none;
  }
  .iconSqu {
    @apply bg-gray-600 h-full w-full rounded-r-lg p-1.5 py-2;
  }
  .invalid .iconSqu {
    @apply bg-red-600;
  }
  .btnText {
    @apply py-2 whitespace-nowrap text-gray-100;
  }
  .sep {
    @apply bg-gray-600 h-px flex justify-center items-center mx-2 my-1;
  }
</style>
