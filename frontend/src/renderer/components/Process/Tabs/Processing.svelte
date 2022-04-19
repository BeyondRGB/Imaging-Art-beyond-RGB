<script lang="ts">
  import {
    viewState,
    processState,
    connectionState,
    connect,
    currentPage,
    messageStore,
    resetProcess,
  } from "@util/stores";
  import ImageViewer from "@components/ImageViewer.svelte";

  let notConnectedMode = false;

  let pipelineComponents = {};
  let pipelineProgress = {};

  function reset() {
    pipelineComponents = {};
    pipelineProgress = {};
    resetProcess();
  }

  $: if ($messageStore.length > 1 && !($messageStore[0] instanceof Blob)) {
    //console.log($messageStore[0]);
    console.log("New Message PROCESSING");
    try {
      let temp = JSON.parse($messageStore[0]);
      if (temp["ResponseType"] === "CalibrationComplete") {
        // Project Key handler
        console.log("CalibrationComplete Project Key From Server");
        $viewState.projectKey = temp["ResponseData"]["path"];
        $processState.pipelineComplete = true;
      } else if (temp["ResponseType"] === "Progress") {
        // Progress Update
        console.log("Progress From Server");
        pipelineProgress[temp["ResponseData"]["sender"]] =
          temp["ResponseData"]["value"];
      } else if (temp["ResponseType"] === "PipelineComponents") {
        // Pipeline Components
        console.log("Pipeline Components From Server");
        pipelineComponents = temp["ResponseData"]["component_json"];
      }
    } catch (e) {
      console.log(e);
    }
  }

  function handleComplete(id) {
    if (id === 0) {
      reset();
      currentPage.set("SpecPicker");
      console.log({ RESETING: $currentPage });
    } else if (id === 1) {
      // open in election
    } else if (id === 2) {
      reset();
    }
  }
</script>

<main>
  {#if $processState.pipelineComplete}
    <div class="completedBox">
      <div class="completedOptions">
        <button on:click={() => handleComplete(0)}>View Image</button>
        <button disabled on:click={() => handleComplete(1)}
          >Open File Location</button
        >
        <button on:click={() => handleComplete(2)}>Process Another Image</button
        >
      </div>
    </div>
  {/if}
  <div class="top">
    <div class="right">
      <div class="image">
        <h4>{$processState.outputImage?.name}</h4>
        <ImageViewer />
      </div>
    </div>
  </div>
  <div class="bottom">
    <div class="stepBox">
      {#if Object.keys(pipelineComponents).length < 1}
        <div class="waitingBox">
          <div class="waitingMsg">Waiting</div>
        </div>
      {:else}
        {#each Object.keys(pipelineComponents) as component1, i1}
          {#if component1 !== "name"}
            {#each Object.keys(pipelineComponents[component1]) as component2, i2}
              <div class="stepGroup">
                <div class="stepTitle">
                  {pipelineComponents[component1][component2]["name"]}
                </div>
                <div class="steps">
                  {#if component2 !== "name"}
                    {#each Object.keys(pipelineComponents[component1][component2]) as component3, i3}
                      {#if component3 !== "name"}
                        {#each Object.keys(pipelineComponents[component1][component2][component3]) as component4, i4}
                          <div
                            class="progress-circle"
                            class:completed={pipelineProgress[
                              pipelineComponents[component1][component2][
                                component3
                              ][component4]["name"]
                            ] === 1}
                          >
                            <div
                              class="progress-overlay"
                              style="--progress:{pipelineComponents[component1][
                                component2
                              ][component3][component4]['name'] in
                              pipelineProgress
                                ? pipelineProgress[
                                    pipelineComponents[component1][component2][
                                      component3
                                    ][component4]['name']
                                  ] * 100
                                : 0}"
                            />
                            <div class="stepper">
                              <span class="sender"
                                >{pipelineComponents[component1][component2][
                                  component3
                                ][component4]["name"]}</span
                              >
                            </div>
                          </div>
                        {/each}
                      {/if}
                    {/each}
                  {/if}
                  {#if pipelineComponents[component1][component2]["name"].includes("Results") || pipelineComponents[component1][component2]["name"].includes("Verifi")}
                    <div
                      class="progress-circle"
                      class:completed={pipelineProgress[
                        pipelineComponents[component1][component2]["name"]
                      ] === 1}
                    >
                      <div
                        class="progress-overlay"
                        style="--progress:{pipelineComponents[component1][
                          component2
                        ]['name'] in pipelineProgress
                          ? pipelineProgress[
                              pipelineComponents[component1][component2]['name']
                            ] * 100
                          : 0}"
                      />
                      <div class="stepper">
                        <span class="sender"
                          >{pipelineComponents[component1][component2][
                            "name"
                          ]}</span
                        >
                      </div>
                    </div>
                  {/if}
                </div>
              </div>
            {/each}
          {/if}
        {/each}
      {/if}
    </div>
  </div>
  {#if $connectionState !== "Connected" && !notConnectedMode}
    <div class="notConnected">
      <card>
        WARNING YOU ARE NOT CONNECTED TO THE BACKEND
        <button class="" on:click={() => connect()}>Reconnect</button>
        <button on:click={() => (notConnectedMode = true)} class="cont"
          >Continue Anyway</button
        >
      </card>
    </div>
  {/if}
</main>

<style lang="postcss">
  .notConnected {
    @apply w-full h-full absolute bg-black/75 z-50 flex justify-center items-center;
  }
  card {
    @apply bg-gray-600 w-64 h-64 p-5 flex flex-col justify-between py-10 rounded-xl font-semibold;
  }
  main {
    @apply w-full h-full flex flex-col relative p-2 gap-2;
  }
  .bottom {
    @apply w-full h-auto flex flex-col justify-center items-center relative gap-2;
  }
  .stepBox {
    @apply w-[90%] flex justify-between items-center relative gap-2;
  }
  .stepper {
    margin-left: 5%;
    margin-top: 5%;
    overflow-wrap: break-word;
    @apply w-[90%] h-[90%] rounded-full bg-gray-600 flex flex-col justify-center items-center
            absolute top-0 left-0;
  }
  .stepTitle {
    @apply text-white;
  }
  .stepGroup {
    @apply gap-1 flex flex-col justify-items-center items-center
            text-lg ring-1 ring-gray-600 p-1;
  }
  .steps {
    @apply flex w-full justify-center gap-2;
  }
  .completedBox {
    @apply w-full h-full absolute bg-black/50 z-50 flex justify-center items-center;
  }
  .completedOptions {
    @apply w-1/2 bg-gray-700 flex flex-col p-2 rounded-lg gap-2;
  }
  .completedOptions button {
    @apply w-full h-full text-xl;
  }
  .sender {
    word-break: break-word;
    white-space: pre-line;
    @apply w-[90%] bg-gray-600 text-base flex rounded-full justify-center items-center
          text-center;
  }
  .progress-circle {
    background: linear-gradient(
      90deg,
      rgb(224, 62, 62) 0%,
      rgb(68, 217, 83) 50%,
      rgb(66, 86, 215) 100%
    );
    background-size: 400% 400%;
    animation: gradient 5s ease infinite;
    @apply bg-gray-600 h-[8vw] w-[8vw] rounded-full p-1 relative;
  }

  .progress-circle.completed {
    background: 0;
    @apply bg-green-500;
  }
  .progress-overlay {
    background: conic-gradient(
      #0000 calc(var(--progress) * 1%),
      rgb(125, 123, 123) 0
    );

    @apply w-full h-full absolute top-0 left-0 rounded-full;
  }

  .waitingBox {
    @apply w-full h-[10vw] flex justify-center items-center;
  }

  .waitingMsg {
    @apply text-xl;
  }

  .top {
    @apply w-full h-full flex justify-center items-center overflow-hidden;
  }
  .right {
    @apply w-full h-full flex justify-center items-center ml-2;
  }

  .image {
    @apply w-full h-full bg-blue-600/25 relative items-center flex justify-center;
  }
  h4 {
    @apply text-gray-200 bg-gray-900/90 absolute top-0 pb-1 pt-0 px-2 rounded-lg z-50;
  }
  span {
    @apply bg-gray-700 text-gray-200 font-semibold whitespace-nowrap select-text;
  }
  .cont {
    @apply bg-red-500;
  }

  @keyframes gradient {
    0% {
      background-position: 0% 50%;
    }
    25% {
      background-position: 50% 50%;
    }
    50% {
      background-position: 100% 50%;
    }
    75% {
      background-position: 50% 50%;
    }
    100% {
      background-position: 0% 50%;
    }
  }

  ::-webkit-scrollbar-track {
    @apply bg-transparent;
  }

  /* Handle */
  ::-webkit-scrollbar-thumb {
    @apply bg-gray-600 rounded-full;
  }

  /* Handle on hover */
  ::-webkit-scrollbar-thumb:hover {
    @apply bg-gray-500;
  }
</style>
