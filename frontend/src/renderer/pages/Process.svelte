<script>
  import { sendMessage, messageStore } from "@util/stores";
  import Stepper from "@components/Stepper.svelte";
  import { getContext } from "svelte";
  import ExportOptions from "@components/ExportOptions.svelte";
  import CanvasImage from "@components/CanvasImage.svelte";

  let messageList = [];
  $: messageList = [...messageList, $messageStore];
  $: {
    const re = /->([a-z A-Z]+)->([a-z A-Z]+)/;
    const array = re.exec($messageStore);
    console.log(array);
    if (array) {
      const stepName =
        array[1] === "ImageCalibrator" ? "Process" : "Preprocess";
      const stepId = steps.findIndex((c) => c.title === stepName);
      console.log(stepId);
      steps[stepId].subTitle = array[2];
      steps = [...steps];
    } else if ($messageStore.includes("PreProcessor Done")) {
      steps[0].status = "success";
      steps = [...steps];
    } else if ($messageStore.includes("Image Calibration Done")) {
      steps[1].status = "success";
      steps = [...steps];
    } else if ($messageStore.includes("Image Processing Done")) {
      steps[2].status = "success";
      steps = [...steps];
    }
  }

  let steps = [
    {
      title: "Preprocess",
      subTitle: "none",
      status: "pending",
    },
    {
      title: "Process",
      subTitle: "none",
      status: "pending",
    },
    {
      title: "Export",
      subTitle: "none",
      status: "pending",
    },
  ];
  let activeStep = 0;

  const { open } = getContext("simple-modal");

  const openModal = () => {
    //modal.set(bind(ExportOptions), {}, { closeButton: false });
    open(ExportOptions, {}, { closeButton: false });
  };

  $: if (activeStep === 3) {
    openModal();
  }
</script>

<main>
  <div id="image">
    <CanvasImage />
  </div>
  <button
    on:click={() => (steps[activeStep++].status = "success")}
    class="bg-blue-500 rounded-md px-2 py-1">[Next]</button
  >
  <div>
    <button id="send" on:click={() => sendMessage("processImg")}
      >Send to Server</button
    >
    <div id="listbox">
      <ul>
        {#each messageList as message}
          <li>{message}</li>
        {/each}
      </ul>
    </div>
  </div>
  <Stepper bind:steps />
</main>

<style lang="postcss">
  main {
    @apply flex flex-col w-full;
  }
  #image {
    @apply self-center w-[110vh];
  }

  #send {
    @apply bg-gray-50 w-full h-8 rounded-lg text-sm active:bg-blue-500 select-none
          flex flex-col justify-center items-center border-2 hover:bg-blue-200 transition-all duration-500
          hover:rounded-2xl text-gray-700;
  }
  #listbox {
    @apply h-20 bg-gray-400 overflow-auto flex flex-col-reverse mx-2 my-1;
  }
</style>
