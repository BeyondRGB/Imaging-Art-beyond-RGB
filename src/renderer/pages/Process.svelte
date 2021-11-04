<script>
  import { currentPage } from "../stores";
  import Stepper from "../components/Stepper.svelte";

  import { getContext } from "svelte";
  import ExportOptions from "@components/ExportOptions.svelte";
  import PageTransitions from "@components/PageTransitions.svelte";
  import CanvasImage from "@components/CanvasImage.svelte";
  let steps = [
    {
      title: "Preprocess",
      status: "pending",
    },
    {
      title: "Process",
      status: "pending",
    },
    {
      title: "Export",
      status: "pending",
    },
  ];
  let activeStep = 0;

  const { open } = getContext("simple-modal");

  const openModal = () => {
    open(ExportOptions);
  };

  $: if (activeStep === 3) {
    openModal();
  }
</script>

<PageTransitions>
  <main>
    <div id="image">
      <CanvasImage />
    </div>
    <button
      on:click={() => (steps[activeStep++].status = "success")}
      class="bg-blue-500 rounded-md px-2 py-1">[Next]</button
    >
    <Stepper bind:steps />
  </main>
</PageTransitions>

<style lang="postcss">
  main {
    @apply flex flex-col w-full;
  }
  #image {
    @apply self-center w-[110vh];
  }
</style>
