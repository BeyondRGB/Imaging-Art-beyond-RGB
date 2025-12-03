<script lang="ts">
  /**
   * Generic Loading Overlay Component
   * Displays a loading indicator with optional message
   */
  export let show: boolean = false;
  export let message: string = "Loading";
</script>

{#if show}
  <div class="loading-overlay">
    <div class="loading-box">
      {message}
      <span class="loader" />
    </div>
  </div>
{/if}

<style lang="postcss">
  .loading-overlay {
    background-color: var(--color-surface);
    @apply absolute w-full h-full z-[49] flex justify-center items-center;
  }
  
  .loading-box {
    @apply h-full flex flex-col gap-2 justify-center items-center text-2xl;
  }
  
  .loader {
    width: 48px;
    height: 48px;
    background: #11ff00;
    transform: rotateX(65deg) rotate(45deg);
    /* remove bellows command for perspective change */
    transform: perspective(200px) rotateX(65deg) rotate(45deg);
    color: rgb(255, 0, 0);
    animation: layers1 1s linear infinite alternate;
    @apply z-50;
  }
  
  .loader:after {
    content: "";
    position: absolute;
    inset: 0;
    background: rgb(0, 0, 255);
    animation: layerTr 1s linear infinite alternate;
  }
  
  @keyframes layers1 {
    0% {
      box-shadow: 0px 0px 0 0px;
    }
    90%,
    100% {
      box-shadow: 20px 20px 0 -4px;
    }
  }
  
  @keyframes layerTr {
    0% {
      transform: translate(0, 0) scale(1);
    }
    100% {
      transform: translate(-25px, -25px) scale(1);
    }
  }
</style>

