<script lang="ts">
  /**
   * Generic Empty State Component
   * Displays a centered message with optional action component
   */
  export let title: string = "No File Selected";
  export let description: string = "";
</script>

<div class="empty-state">
  <div class="empty-state-box">
    {#if title}
      <h2>{title}</h2>
    {/if}
    {#if description}
      <p>{description}</p>
    {/if}
    <slot />
  </div>
</div>

<style lang="postcss">
  .empty-state {
    background-color: var(--color-surface-base);
    @apply absolute w-full h-full z-[99] flex justify-center items-center;
  }
  
  .empty-state-box {
    background-color: var(--color-surface);
    @apply w-auto h-auto flex flex-col gap-2 justify-center items-center
          p-8 rounded-2xl;
  }
  
  .empty-state-box h2 {
    @apply text-xl;
  }
  
  .empty-state-box p {
    @apply text-sm text-center;
    color: var(--color-text-secondary);
  }
</style>

