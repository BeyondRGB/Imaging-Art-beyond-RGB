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

<div class="modal-background" on:click={close} />

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
    <svelte:component this={component} />
  </div>
{/if}

<style lang="postcss">
  .modal-background {
    background-color: var(--color-overlay-medium);
    @apply fixed top-0 left-0 w-full h-full z-[9999] backdrop-blur-sm;
  }

  .modal-container {
    @apply absolute flex flex-col items-center justify-center z-[9999] w-full h-full pointer-events-none;
  }

  .modal-content {
    @apply relative pointer-events-auto;
  }

  .close-button {
    background-color: var(--color-surface);
    color: var(--color-text-secondary);
    border: 1px solid var(--color-border);
    @apply absolute -top-3 -right-3 z-10 w-9 h-9 rounded-full 
           flex items-center justify-center transition-all duration-200
           shadow-lg hover:shadow-xl hover:scale-110;
  }
  
  .close-button:hover {
    background-color: var(--color-interactive-hover);
    color: var(--color-text-primary);
  }

  .close-home {
    background-color: var(--color-overlay-medium);
    color: var(--color-text-tertiary);
    border: 2px solid var(--color-border);
    @apply absolute top-0 left-0 w-full h-[6%] rounded-none
           m-0 flex items-center justify-center active:scale-100 transition-all pointer-events-auto;
  }
  
  .close-home:hover {
    background-color: var(--color-overlay-heavy);
  }
  
  .close-home:active {
    background-color: var(--color-interactive-active);
  }
</style>
