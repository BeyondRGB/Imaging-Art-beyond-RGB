<script lang="ts">
  import Button from "@components/Button.svelte";
  
  export let closeModal: () => void;
  export let onViewImage: () => void;
  export let onOpenFileLocation: () => void;
  export let onProcessAnother: () => void;
</script>

<main>
  <h2>Processing Complete</h2>
  <div class="modal-content">
    <div class="button-group">
      <Button 
        variant="default" 
        size="lg" 
        onClick={() => {
          onViewImage();
          closeModal();
        }}
      >
        View Image
      </Button>
      <Button 
        variant="secondary" 
        size="lg" 
        onClick={() => {
          onOpenFileLocation();
        }}
      >
        Open File Location
      </Button>
      <Button 
        variant="default" 
        size="lg" 
        onClick={() => {
          onProcessAnother();
          closeModal();
        }}
      >
        Process Another Image
      </Button>
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    background-color: var(--color-surface-elevated);
    @apply w-[500px] rounded-xl shadow-2xl overflow-hidden;
  }

  h2 {
    color: var(--color-text-primary);
    @apply text-2xl font-semibold mb-4 text-center pt-8 px-8;
  }

  .modal-content {
    @apply flex flex-col gap-6 p-8 pt-0;
  }

  p {
    color: var(--color-text-secondary);
    @apply text-center text-lg;
  }

  .button-group {
    @apply flex flex-col gap-3;
  }

  .button-group :global(button) {
    @apply w-full;
  }
</style>

