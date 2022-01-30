<script>
  export let steps = [];

  $: console.log(steps);
</script>

<main>
  {#each steps as step, index}
    <div id="step" class={step.status}>
      {#if step.subTitle === "none"}
        <h3>{step.title}</h3>
      {:else}
        <h3>{step.subTitle}</h3>
      {/if}
    </div>
  {/each}
</main>

<style lang="postcss">
  main {
    @apply bg-red-400 flex justify-center gap-2;
  }
  div {
    @apply bg-gray-50 rounded-full flex justify-center w-24 h-24;
  }
  h3 {
    @apply text-xl my-auto;
  }
  .success {
    @apply bg-green-300;
  }
</style>
