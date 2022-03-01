<script>
  import { createEventDispatcher, onDestroy } from "svelte";
  import { fly } from "svelte/transition";
  import { appSettings } from "@util/stores";
  import { ChevronDownIcon } from "svelte-feather-icons";
  const dispatch = createEventDispatcher();
  const close = () => dispatch("close");

  let modal;

  export let minimal = false;

  $: theme = $appSettings.theme ? "dark" : "";

  const handle_keydown = (e) => {
    if (e.key === "Escape") {
      close();
      return;
    }

    if (e.key === "Tab") {
      // trap focus
      const nodes = modal.querySelectorAll("*");
      const tabbable = Array.from(nodes).filter((n) => n.tabIndex >= 0);

      let index = tabbable.indexOf(document.activeElement);
      if (index === -1 && e.shiftKey) index = 0;

      index += tabbable.length + (e.shiftKey ? -1 : 1);
      index %= tabbable.length;

      tabbable[index].focus();
      e.preventDefault();
    }
  };

  const previously_focused =
    typeof document !== "undefined" && document.activeElement;

  if (previously_focused) {
    onDestroy(() => {
      previously_focused.focus();
    });
  }
</script>

<svelte:window on:keydown={handle_keydown} />

<div class="modal-background" on:click={close} />

{#if !minimal}
  <div
    class="{theme} modal"
    role="dialog"
    aria-modal="true"
    bind:this={modal}
    transition:fly={{ y: window.innerHeight, duration: 250, opacity: 0 }}
  >
    <slot />

    <!-- svelte-ignore a11y-autofocus -->
    <button class="closeDia" autofocus on:click={close}>close modal</button>
  </div>
{:else}
  <div
    class="{theme} modal"
    bind:this={modal}
    role="dialog"
    transition:fly={{ y: window.innerHeight, duration: 400, opacity: 1 }}
  >
    <button class="closeHome" on:click={close}
      ><ChevronDownIcon size="2x" /></button
    >
    <slot />
  </div>
{/if}

<style lang="postcss">
  .modal-background {
    background: rgba(0, 0, 0, 0.6);
    @apply fixed top-0 left-0 w-full h-full z-50;
  }

  .modal {
    @apply absolute flex flex-col items-center justify-center z-50 w-full h-full;
  }

  .box {
    @apply bg-red-300 h-full w-full;
  }

  .closeHome {
    @apply absolute top-0 left-0 bg-gray-800/75 w-full h-[6%] rounded-none
          border-2 border-gray-700 text-gray-200/50 hover:bg-gray-600/75 hover:rounded-none
          m-0 flex items-center justify-center active:scale-100 active:bg-gray-400/75 transition-all;
  }

  .closeDia {
    @apply bg-gray-500 h-10 -translate-y-16;
  }

  button {
    @apply mt-2;
  }
</style>
