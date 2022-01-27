<script>
  import { createEventDispatcher, onDestroy } from "svelte";

  import { appSettings } from "@util/stores";
  const dispatch = createEventDispatcher();
  const close = () => dispatch("close");

  let modal;

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

<div
  class="{theme} modal dark:bg-gray-600 bg-gray-200"
  role="dialog"
  aria-modal="true"
  bind:this={modal}
>
  <slot name="header" />
  <hr />
  <slot />
  <hr />

  <!-- svelte-ignore a11y-autofocus -->
  <button autofocus on:click={close}>close modal</button>
</div>

<style lang="postcss">
  .modal-background {
    background: rgba(0, 0, 0, 0.6);
    @apply fixed top-0 left-0 w-full h-full z-50;
  }

  .modal {
    @apply absolute left-1/2 top-1/2 w-[calc(100vw-4em)] z-50 max-w-[32em]
            max-h-[calc(100vh-4em)] overflow-auto -translate-x-1/2 -translate-y-1/2 p-[1em];
  }

  button {
    @apply mt-2;
  }
</style>
