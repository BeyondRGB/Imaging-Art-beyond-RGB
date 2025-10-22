<script lang="ts">
  /**
   * Generic Confirmation Dialog Component
   * Provides a modal overlay for yes/no confirmations
   */
  import Button from "./Button.svelte";
  
  export let show: boolean = false;
  export let message: string = "Are you sure?";
  export let confirmLabel: string = "Confirm";
  export let cancelLabel: string = "Cancel";
  export let type: 'info' | 'warning' | 'error' = 'info';
  export let onConfirm: () => void = () => {};
  export let onCancel: () => void = () => {};
  
  function handleConfirm() {
    onConfirm();
    show = false;
  }
  
  function handleCancel() {
    onCancel();
    show = false;
  }
</script>

<div class="confirm-modal" class:show>
  <div class="confirm-dialog" class:warning={type === 'warning'} class:error={type === 'error'}>
    <p class="message">{message}</p>
    <div class="button-group">
      {#if cancelLabel}
        <Button variant="secondary" onClick={handleCancel}>{cancelLabel}</Button>
      {/if}
      <Button 
        variant={type === 'error' ? 'danger' : 'success'} 
        onClick={handleConfirm}
      >
        {confirmLabel}
      </Button>
    </div>
  </div>
</div>

<style lang="postcss">
  .confirm-modal {
    background-color: var(--color-overlay-heavy);
    @apply absolute z-50 items-center justify-center w-full h-full hidden;
  }
  
  .show {
    @apply flex;
  }
  
  .confirm-dialog {
    background-color: var(--color-surface);
    @apply w-1/2 h-auto min-h-[200px] text-xl rounded-xl p-4 
           flex flex-col justify-between;
  }
  
  .confirm-dialog.warning {
    @apply border-2 border-yellow-500;
  }
  
  .confirm-dialog.error {
    @apply border-2 border-red-500;
  }
  
  .message {
    background-color: var(--color-surface-sunken);
    @apply rounded-md flex justify-center items-center p-4 text-lg mt-4 mb-4;
  }
  
  .button-group {
    @apply flex justify-end gap-2;
  }
</style>

