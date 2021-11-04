<script>
  import { fade } from "svelte/transition";
</script>

<div in:fade={{ duration: 300, delay: 300 }} out:fade={{ duration: 300 }}>
  <slot />
</div>

<style lang="postcss">
  div {
    @apply w-full h-full;
  }
</style>
