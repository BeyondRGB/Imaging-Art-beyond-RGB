<script>
  import { currentPage } from "../stores";
  import { angleDoubleDown } from "svelte-awesome/icons";
  import Icon from "svelte-awesome";
  import PageTransitions from "@components/PageTransitions.svelte";
</script>

<PageTransitions>
  <main>
    <div>
      <h2>Welcome to</h2>
      <h1>Beyond RGB</h1>
      <h3>Get started below</h3>
      <Icon data={angleDoubleDown} scale="2" />
    </div>
    <button on:click={() => currentPage.set("Preprocessing")}>
      Proccess
      <span> Process a new Spectral Image </span>
    </button>
    <button on:click={() => currentPage.set("SpecOverlay")}>
      View
      <span> View already processed Image </span>
    </button>
  </main>
</PageTransitions>

<style lang="postcss">
  main {
    background-color: var(--box-color);
    @apply w-full h-full flex flex-col items-center justify-center gap-4;
  }

  button {
    @apply bg-gray-50 w-[60%] h-[12%] rounded-lg text-2xl active:bg-blue-500 select-none
						flex flex-col justify-center items-center border-2 hover:bg-blue-200 transition-all duration-500
            hover:rounded-2xl;
  }

  h2 {
    @apply text-3xl;
  }
  h1 {
    @apply text-4xl;
  }
  h3 {
    @apply text-base pt-4;
  }

  div {
    @apply text-black text-opacity-40 absolute mb-[30%] flex flex-col justify-center items-center gap-1;
  }

  span {
    @apply text-sm;
  }
</style>
