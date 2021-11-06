<script>
  export let steps = [];

  $: console.log(steps);
</script>

<main>
  {#each steps as step, index}
    <div id="step" class={step.status}><span>{step.title}</span></div>
  {/each}
</main>

<style lang="postcss">
  main {
    @apply bg-red-400 flex justify-center gap-2;
  }
  div {
    @apply bg-gray-50 rounded-full flex justify-center w-24 h-24;
  }
  span {
    @apply text-xl my-auto;
  }
  .success {
    @apply bg-green-300;
  }
</style>
