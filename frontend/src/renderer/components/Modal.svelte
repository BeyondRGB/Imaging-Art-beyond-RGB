<script lang="ts">
  import { createEventDispatcher, onDestroy } from "svelte";
  import { fly, fade } from "svelte/transition";
  import { appSettings } from "@util/stores";
  import { XIcon, ChevronDownIcon } from "svelte-feather-icons";
  const dispatch = createEventDispatcher();
  const close = () => dispatch("close");

  let modal;

  export let minimal = false;
  export let customExit = false;
  export let component;
  export let size: 'small' | 'medium' | 'large' | 'fullscreen' = 'medium';
  export let backdropBlur: 'none' | 'sm' | 'md' | 'lg' = 'sm';
  export let backdropOpacity: 'light' | 'medium' | 'heavy' = 'medium';

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

<div 
  class="modal-background"
  class:blur-none={backdropBlur === 'none'}
  class:blur-sm={backdropBlur === 'sm'}
  class:blur-md={backdropBlur === 'md'}
  class:blur-lg={backdropBlur === 'lg'}
  class:opacity-light={backdropOpacity === 'light'}
  class:opacity-medium={backdropOpacity === 'medium'}
  class:opacity-heavy={backdropOpacity === 'heavy'}
  on:click={close}
  transition:fade={{ duration: 200 }}
/>

{#if !minimal}
  <div
    class="{theme} modal-container"
    role="dialog"
    aria-modal="true"
    bind:this={modal}
    transition:fly={{ y: window.innerHeight, duration: 300, opacity: 0, easing: (t) => 1 - Math.pow(1 - t, 3) }}
  >
    <div 
      class="modal-content"
      class:size-small={size === 'small'}
      class:size-medium={size === 'medium'}
      class:size-large={size === 'large'}
      class:size-fullscreen={size === 'fullscreen'}
    >
      {#if !customExit}
        <button class="close-button" on:click={close} aria-label="Close">
          <XIcon size="1.25x" />
        </button>
      {/if}
      <svelte:component this={component} closeModal={close} {...$$restProps} />
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
    @apply fixed top-0 left-0 w-full h-full z-[9999];
  }
  
  /* Backdrop blur variants */
  .blur-none {
    backdrop-filter: none;
  }
  
  .blur-sm {
    @apply backdrop-blur-sm;
  }
  
  .blur-md {
    @apply backdrop-blur-md;
  }
  
  .blur-lg {
    @apply backdrop-blur-lg;
  }
  
  /* Backdrop opacity variants */
  .opacity-light {
    background-color: var(--color-overlay-light);
  }
  
  .opacity-medium {
    background-color: var(--color-overlay-medium);
  }
  
  .opacity-heavy {
    background-color: var(--color-overlay-heavy);
  }

  .modal-container {
    @apply absolute flex flex-col items-center justify-center z-[9999] w-full h-full pointer-events-none;
  }

  .modal-content {
    @apply relative pointer-events-auto bg-transparent;
    max-height: 90vh;
    max-width: 90vw;
    position: relative;
  }
  
  /* Size variants */
  .size-small {
    @apply max-w-md w-full;
  }
  
  .size-medium {
    @apply max-w-2xl w-full;
  }
  
  .size-large {
    @apply max-w-4xl w-full;
  }
  
  .size-fullscreen {
    @apply max-w-full max-h-full w-full h-full;
  }

  .close-button {
    background-color: var(--color-surface);
    color: var(--color-text-secondary);
    border: 1px solid var(--color-border);
    @apply absolute -top-3 -right-3 z-10 w-9 h-9 rounded-full 
           flex items-center justify-center transition-all duration-200
           shadow-lg hover:shadow-xl hover:scale-110 focus:outline-none;
  }
  
  .close-button:focus {
    outline: 2px solid var(--color-border-focus);
    outline-offset: 2px;
  }
  
  .close-button:hover {
    background-color: var(--color-interactive-hover);
    color: var(--color-text-primary);
  }
  
  .close-button:active {
    transform: scale(1.05);
  }

  .close-home {
    background-color: var(--color-overlay-medium);
    color: var(--color-text-tertiary);
    border: 2px solid var(--color-border);
    @apply absolute top-0 left-0 w-full h-[6%] rounded-none
           m-0 flex items-center justify-center active:scale-100 transition-all pointer-events-auto
           focus:outline-none;
  }
  
  .close-home:focus {
    outline: 2px solid var(--color-border-focus);
    outline-offset: 2px;
  }
  
  .close-home:hover {
    background-color: var(--color-overlay-heavy);
  }
  
  .close-home:active {
    background-color: var(--color-interactive-active);
  }
</style>
