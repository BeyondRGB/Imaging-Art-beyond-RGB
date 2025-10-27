<script>
  import { createEventDispatcher, onDestroy } from "svelte";
  import { fly } from "svelte/transition";
  import { appSettings } from "@util/stores";
  import { XIcon, ChevronDownIcon } from "svelte-feather-icons";
  const dispatch = createEventDispatcher();
  const close = () => dispatch("close");

  let modal;

  export let minimal = false;
  export let customExit = false;
  export let component;

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

{#if !minimal}
  <div class="modal-background" on:click={close} />
{/if}

{#if !minimal}
  <div
    class="{theme} modal-container"
    role="dialog"
    aria-modal="true"
    bind:this={modal}
    transition:fly={{ y: window.innerHeight, duration: 250, opacity: 0 }}
  >
    <div class="modal-content">
      {#if !customExit}
        <button class="close-button" on:click={close} aria-label="Close">
          <XIcon size="1.25x" />
        </button>
      {/if}
      <svelte:component this={component} closeModal={close} />
    </div>
  </div>
{:else}
  <div
    class="{theme} modal-container"
    bind:this={modal}
    role="dialog"
    transition:fly={{ y: window.innerHeight, duration: 400, opacity: 1 }}
  >
    <button class="close-home" on:click={close} aria-label="Close">
      <ChevronDownIcon size="2x" />
    </button>
    <div class="minimal-content">
      <svelte:component this={component} />
    </div>
  </div>
{/if}

<style lang="postcss">
  .modal-background {
    @apply fixed top-0 left-0 w-full h-full z-[9999] bg-black/60 backdrop-blur-sm;
  }

  .modal-container {
    @apply absolute flex flex-col items-center justify-center z-[9999] w-full h-full pointer-events-none;
  }

  .modal-content {
    @apply relative pointer-events-auto;
  }

  .minimal-content {
    @apply pointer-events-auto w-full h-full;
  }

  .close-button {
    @apply absolute -top-3 -right-3 z-10 w-9 h-9 rounded-full 
           bg-gray-700 hover:bg-gray-600 text-gray-300 hover:text-gray-100
           flex items-center justify-center transition-all duration-200
           shadow-lg hover:shadow-xl hover:scale-110 ring-1 ring-gray-600;
  }

  .close-home {
    @apply absolute top-0 left-0 bg-gray-800/75 w-full h-[6%] rounded-none
           border-2 border-gray-700 text-gray-200/50 hover:bg-gray-600/75
           m-0 flex items-center justify-center active:scale-100 
           active:bg-gray-400/75 transition-all pointer-events-auto;
  }
</style>
