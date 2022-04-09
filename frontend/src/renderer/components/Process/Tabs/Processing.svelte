<script lang="ts">
  import {
    viewState,
    processState,
    connectionState,
    connect,
    currentPage,
    messageStore,
  } from "@util/stores";
  import ImageViewer from "@components/ImageViewer.svelte";
  import { each } from "svelte/internal";

  let notConnectedMode = false;
  let info = { sender: "Waiting...", value: 0 };

  function reset() {
    currentPage.set("Process");
    processState.set({
      currentTab: 0,
      pipelineComponents: [],
      pipelineProgress: null,
      destDir: "",
      imageFilePaths: [],
      thumbnailID: null,
      colorTargetID: null,
      imageThumbnails: {},
      outputImage: { dataURL: "", name: "Waiting..." },
      artStacks: [
        {
          id: 1,
          name: "Art 1",
          colorTargetImage: { dataURL: "", filename: "" },
          verificationTargetImage: { dataURL: "", filename: "" },
          colorTarget: {},
          verificationTarget: null,
          fields: {
            images: [],
            whitefield: [],
            darkfield: [],
          },
        },
      ],
    });
  }

  let temp = {
    component: [
      {
        component: [
          {
            name: "ImageReader",
          },
          {
            name: "BitDepthScaler",
          },
          {
            name: "Flat Fielding",
          },
          {
            name: "PixelRegestor",
          },
        ],
        name: "PreProcessor",
      },
      {
        component: [
          {
            name: "ColorManagedCalibrator",
          },
          {
            name: "SpectralCalibration",
          },
        ],
        name: "ImageCalibrator",
      },
      {
        name: "ResultsProcsessor",
      },
    ],
    name: "ImageProcessor",
  };

  $: console.log([
    $processState.pipelineProgress,
    $processState.pipelineComponents,
  ]);
  console.log(temp);
</script>

<main>
  {#if $viewState.projectKey?.length > 1}
    COMPLETED
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
    {#each Object.keys(temp) as component1, i1}
      {#if component1 !== "name"}
        {#each Object.keys(temp[component1]) as component2, i2}
          <div class="stepGroup">
            <div class="stepTitle">{temp[component1][component2]["name"]}</div>
            <div class="steps">
              {#if component2 !== "name"}
                {#each Object.keys(temp[component1][component2]) as component3, i3}
                  {#if component3 !== "name"}
                    {#each Object.keys(temp[component1][component2][component3]) as component4, i4}
                      <div
                        class="progress-circle"
                        style="--progress:{$processState.pipelineProgress[
                          temp[component1][component2][component3][component4][
                            'name'
                          ]
                        ] * 100}"
                      >
                        <div class="stepper">
                          <span class="sender"
                            >{temp[component1][component2][component3][
                              component4
                            ]["name"]}</span
                          >
                          <span class="value"
                            >{$processState.pipelineProgress[
                              temp[component1][component2][component3][
                                component4
                              ]["name"]
                            ]}</span
                          >
                        </div>
                      </div>
                    {/each}
                  {/if}
                {/each}
              {/if}
              {#if temp[component1][component2]["name"].includes("Results")}
                <div
                  class="progress-circle"
                  style="--progress:{$processState.pipelineProgress[
                    temp[component1][component2]['name']
                  ] * 100}"
                >
                  <div class="stepper">
                    <span class="sender"
                      >{temp[component1][component2]["name"]}</span
                    >
                    <span class="value"
                      >{$processState.pipelineProgress[
                        temp[component1][component2]["name"]
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
    @apply w-full h-full flex flex-col relative p-2;
  }
  .bottom {
    @apply w-full h-full flex justify-center items-center relative overflow-auto gap-2;
  }
  .stepper {
    @apply w-[20vh] h-[20vh] rounded-full bg-gray-500 flex flex-col justify-center items-center;
  }
  .progress-circle {
    background: conic-gradient(
      rgb(5 150 105) calc(var(--progress) * 1%),
      #0000 0
    );
    @apply bg-gray-600 rounded-full p-1;
  }
  .sender {
    @apply bg-gray-500 text-lg rounded-lg;
  }
  .message {
    @apply bg-gray-500 rounded-lg p-0.5;
  }
  .stepTitle {
    @apply bg-red-500;
  }
  .stepGroup {
    @apply bg-green-300 flex flex-col;
  }
  .steps {
    @apply bg-blue-400 flex;
  }
  .top {
    @apply w-full h-full flex justify-center items-center overflow-hidden;
  }
  .left {
    @apply w-[30%] h-full flex justify-center items-center ml-2;
  }
  .right {
    @apply w-full h-full flex justify-center items-center ml-2;
  }
  .state {
    @apply w-full h-full py-2 flex flex-col;
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
  .letter {
    @apply bg-transparent font-bold text-blue-500 select-none;
  }
  .cont {
    @apply bg-red-500;
  }
  .key {
    @apply bg-transparent font-bold text-red-500/75 select-none;
  }
  h3 {
    @apply text-lg;
  }
  /* .imgTitle {
    @apply absolute top-0 z-50 bg-gray-900/75 pb-1 px-2 pt-0 rounded-lg text-gray-200;
  } */
  .box {
    @apply overflow-y-auto w-full h-full bg-gray-900 text-gray-100 text-sm;
  }
  li {
    @apply px-2 p-1 list-none;
  }
  .stateSend {
    @apply bg-blue-500/50 h-full;
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
