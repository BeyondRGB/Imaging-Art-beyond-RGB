<script>
  import { currentPage, processState, modal } from "@util/stores";
  import ColorTargetViewer from "@components/Process/ColorTargetViewer.svelte";
  import { flip } from "svelte/animate";
  import Page from "@root/components/Page.svelte";
  import { PlusCircleIcon } from "svelte-feather-icons";
  import Dropdown from "@root/components/Dropdown.svelte";

  let refData = [
    "NGT_Reflectance_Data.csv",
    "APT_Reflectance_Data.csv",
    "CCSG_Reflectance_Data.csv",
    "CC_Classic_Reflectance_Data.csv",
    "Choose a custom file....csv",
  ];

  function update() {
    if (colorPos) {
      $processState.artStacks[0].colorTarget.top = colorPos.top;
      $processState.artStacks[0].colorTarget.left = colorPos.left;
      $processState.artStacks[0].colorTarget.bottom = colorPos.bottom;
      $processState.artStacks[0].colorTarget.right = colorPos.right;
      $processState.artStacks[0].colorTarget.rows = colorTarget.rows;
      $processState.artStacks[0].colorTarget.cols = colorTarget.cols;
      $processState.artStacks[0].colorTarget.size = colorTarget.size;
      $processState.artStacks[0].colorTarget.whitePatch =
        colorTarget.whitePatch;
      if (colorTarget.refData.name !== "CUSTOM DATA") {
        $processState.artStacks[0].colorTarget.refData = {
          name: colorTarget.refData.name,
        };
      }
    }

    if (verifyPos) {
      $processState.artStacks[0].verificationTarget.top = verifyPos.top;
      $processState.artStacks[0].verificationTarget.left = verifyPos.left;
      $processState.artStacks[0].verificationTarget.bottom = verifyPos.bottom;
      $processState.artStacks[0].verificationTarget.right = verifyPos.right;
      $processState.artStacks[0].verificationTarget.rows = verifyTarget.rows;
      $processState.artStacks[0].verificationTarget.cols = verifyTarget.cols;
      $processState.artStacks[0].verificationTarget.size = verifyTarget.size;
      $processState.artStacks[0].verificationTarget.whitePatch =
        verifyTarget.whitePatch;
      if (verifyTarget.refData.name !== "CUSTOM DATA") {
        $processState.artStacks[0].verificationTarget.refData = {
          name: verifyTarget.refData.name,
        };
      }
    } else {
      $processState.artStacks[0].verificationTarget = null;
    }
  }

  $: if ($processState.currentTab === 5) {
    console.log("Update");
    console.log($processState);
    update();
    console.log($processState);
  }

  let colorTarget;
  let colorPos;
  let verifyTarget;
  let verifyPos;

  function addTarget() {
    if (!colorTarget) {
      colorTarget = {
        name: "Color Target",
        rows: 10,
        cols: 10,
        refData: null,
        color: 50,
        size: 0.5,
        whitePatch: {
          row: 1,
          col: 1,
        },
        refData: {
          name: "NGT_Reflectance_Data.csv",
          standardObserver: 1931,
          illuminants: "D50",
        },
      };
      colorPos = { top: 0.25, left: 0.25, bottom: 0.5, right: 0.5 };
    } else if (!verifyTarget) {
      verifyTarget = {
        name: "Verification Target",
        rows: 10,
        cols: 10,
        refData: null,
        color: 100,
        size: 0.5,
        whitePatch: {
          row: 1,
          col: 1,
        },
        refData: {
          name: "NGT_Reflectance_Data.csv",
          standardObserver: 1931,
          illuminants: "D50",
        },
      };
      verifyPos = { top: 0.5, left: 0.5, bottom: 0.75, right: 0.75 };
    }
  }

  function removeTarget(id) {
    console.log("Remove");
    if (id === 0) {
      console.log("Removeing Color Target");
      colorTarget = null;
    } else if (id === 1) {
      console.log("Removeing Verify Target");
      verifyTarget = null;
    }
  }

  $: if (colorTarget) {
    let root = document.documentElement;
    root.style.setProperty("--color_hue", `${colorTarget.color}`);
  }

  $: if (verifyTarget) {
    let root = document.documentElement;
    root.style.setProperty("--verfiy_hue", `${verifyTarget}`);
  }

  let targetArray;
  $: {
    targetArray = [];
    if (colorTarget) {
      targetArray = [colorTarget];
    }
    if (verifyTarget) {
      targetArray = [...targetArray, verifyTarget];
    }
  }

  // $: console.log(targetArray);

  $: if (colorTarget?.refData?.name === "Choose a custom file....csv") {
    console.log("OPENING CUSTOM REF MODAL");
    modal.set("CustomRefData");
    colorTarget.refData.name = "CUSTOM DATA";
  }
  $: if (verifyTarget?.refData?.name === "Choose a custom file....csv") {
    console.log("OPENING CUSTOM REF MODAL VER");
    modal.set("CustomRefDataVer");
    verifyTarget.refData.name = "CUSTOM DATA";
  }
</script>

<main>
  <div class="left">
    <ColorTargetViewer
      bind:colorTarget
      bind:verifyTarget
      bind:colorPos
      bind:verifyPos
    />
  </div>
  <div class="right">
    <div class="boxHead">Targets</div>
    <div class="cardBox">
      {#each targetArray as target, i (target)}
        <div
          animate:flip={{ duration: 250 }}
          class={`card ${i === 0 ? "colorTarget" : "verificationTarget"}`}
        >
          <h2>{target.name}</h2>
          <input
            type="range"
            class="colorSlider"
            bind:value={target.color}
            max="360"
          />

          <div class="rowcol">
            <div class="inputGroup">
              <span>Rows:</span>
              <input
                placeholder="1..26 [a-z]"
                type="number"
                min="1"
                bind:value={target.rows}
              />
            </div>
            <div class="inputGroup">
              <span>Columns:</span>
              <input
                placeholder="1..26 [a-z]"
                type="number"
                min="1"
                bind:value={target.cols}
              />
            </div>
          </div>
          <div class="refDataDiv">
            <span>Reference Data</span>
            <Dropdown
              values={refData}
              bind:selected={target.refData.name}
              spaceLast
            />
          </div>
          <div class="sizeDiv">
            <span>Selection Area Size:</span>
            {Math.round(target.size * 100)}%
            <input
              type="range"
              bind:value={target.size}
              min=".3"
              max=".7"
              step=".01"
            />
          </div>
          <div class="break" />
          <span>White Patch Location</span>
          <div class="whitePatch">
            <div class="inputGroup">
              <span>Row: </span>
              <input
                placeholder="1..26 [a-z]"
                type="number"
                min="1"
                bind:value={target.whitePatch.row}
              />
            </div>
            <span class="and">&</span>
            <div class="inputGroup">
              <span>Col: </span>
              <input
                placeholder="1..26 [a-z]"
                type="number"
                min="1"
                bind:value={target.whitePatch.col}
              />
            </div>
          </div>
          <button class="close" on:click={() => removeTarget(i)}>X</button>
        </div>
      {/each}
    </div>
    <button
      class="addTarget"
      class:removeButton={targetArray.length === 2}
      on:click={() => addTarget()}><PlusCircleIcon size="1.75x" /></button
    >
  </div>
</main>

<style lang="postcss">
  :root {
    --color_hue: 50;
    --verfiy_hue: 100;
  }

  main {
    @apply flex w-full h-full overflow-hidden;
  }

  .left {
    @apply w-full h-full flex items-center m-1 bg-gray-600 p-2;
  }

  .right {
    @apply w-[40vw] h-full flex flex-col m-1 bg-gray-700 pt-[5vh] items-center;
  }

  .cardBox {
    @apply bg-gray-800 min-h-[60vh] w-[85%] p-2 gap-2 flex flex-col items-center
            rounded-2xl;
  }

  .card {
    @apply rounded-lg w-full min-h-[4rem] p-4 flex flex-col gap-1 relative;
  }

  .colorTarget {
    background-color: hsl(var(--color_hue), 100%, 30%);
  }

  .verificationTarget {
    background-color: hsl(var(--verfiy_hue), 100%, 30%);
  }

  h2 {
    @apply text-lg justify-center flex items-center font-semibold;
  }

  .rowcol {
    @apply flex flex-col justify-between items-center;
  }

  .whitePatch {
    @apply flex justify-between items-center gap-1 text-base;
  }

  .addTarget {
    @apply bg-green-500 w-16 h-16 transition-all rounded-full flex items-center justify-center
            text-gray-100;
  }

  .removeButton {
    @apply hidden;
  }

  .rowcol input {
    text-align: center;
    @apply p-0.5 bg-gray-900 border-2 border-gray-800 rounded-lg
          focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2
            h-full w-12 min-w-[1rem];
  }

  .whitePatch input {
    @apply p-0.5 bg-gray-900 border-2 border-gray-800 rounded-lg
          focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2
            h-full w-full;
  }

  .extra {
    @apply bg-gray-700 p-2;
  }

  .rowcol .inputGroup {
    @apply flex justify-between items-center gap-2 w-full;
  }

  .whitePatch .inputGroup {
    @apply flex items-center gap-2;
  }

  .inputGroup > span {
    @apply font-semibold;
  }
  .times {
    @apply text-xl;
  }

  .close {
    @apply absolute top-0 right-0 bg-transparent text-white
            hover:bg-red-600/50 hover:text-red-300;
  }
  .refDataDiv {
    @apply flex justify-between items-center;
  }
  .break {
    @apply w-full h-1 bg-black border-2;
  }
  .colorSlider {
    -webkit-appearance: none;
    appearance: none;
    background: linear-gradient(
      to right,
      hsl(0, 100%, 40%),
      hsl(20, 100%, 40%),
      hsl(40, 100%, 40%),
      hsl(60, 100%, 40%),
      hsl(80, 100%, 40%),
      hsl(100, 100%, 40%),
      hsl(120, 100%, 40%),
      hsl(140, 100%, 40%),
      hsl(160, 100%, 40%),
      hsl(180, 100%, 40%),
      hsl(200, 100%, 40%),
      hsl(220, 100%, 40%),
      hsl(240, 100%, 40%),
      hsl(260, 100%, 40%),
      hsl(280, 100%, 40%),
      hsl(300, 100%, 40%),
      hsl(320, 100%, 40%),
      hsl(340, 100%, 40%),
      hsl(360, 100%, 40%)
    );
    @apply w-full h-2 rounded-xl;
  }
  .colorSlider::-webkit-slider-thumb {
    -webkit-appearance: none; /* Override default look */
    appearance: none;
    @apply w-4 h-4 bg-gray-600 cursor-pointer rounded-full outline outline-1
          outline-gray-200;
  }
  .sizeDiv {
    @apply flex justify-between items-center;
  }
  .sizeDiv input {
    @apply w-1/2;
  }
</style>
