<script>
  import { currentPage, processState } from "@util/stores";
  import ColorTargetViewer from "@components/Process/ColorTargetViewer.svelte";
  function update() {
    if (colorTarget?.currentPos) {
      $processState.artStacks[0].colorTargets[0].top =
        colorTarget.currentPos.top;
      $processState.artStacks[0].colorTargets[0].left =
        colorTarget.currentPos.left;
      $processState.artStacks[0].colorTargets[0].bottom =
        colorTarget.currentPos.bottom;
      $processState.artStacks[0].colorTargets[0].right =
        colorTarget.currentPos.right;
      $processState.artStacks[0].colorTargets[0].rows = colorTarget.rows;
      $processState.artStacks[0].colorTargets[0].cols = colorTarget.cols;
    }
  }

  $: if ($processState.currentTab === 5) {
    console.log("Update");
    update();
  }

  let colorTarget;
  let verifyTarget;

  function addTarget() {
    if (!colorTarget) {
      colorTarget = {
        name: "Color Target",
        currentPos: {},
        rows: 10,
        cols: 10,
        refData: null,
        color: 50,
      };
      $processState.artStacks[0].colorTargets[0] = {
        top: 0.25,
        left: 0.25,
        bottom: 0.5,
        right: 0.5,
        cols: 10,
        rows: 10,
      };
    } else if (!verifyTarget) {
      verifyTarget = {
        name: "Verification Target",
        currentPos: {},
        rows: 10,
        cols: 10,
        refData: null,
        color: 100,
      };
      $processState.artStacks[0].colorTargets[1] = {
        top: 0.25,
        left: 0.25,
        bottom: 0.5,
        right: 0.5,
        cols: 10,
        rows: 10,
      };
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
  $: console.log($processState.artStacks[0].colorTargets);

  $: console.log([colorTarget, verifyTarget]);
</script>

<main>
  <div class="left">
    <ColorTargetViewer bind:colorTarget bind:verifyTarget />
  </div>
  <div class="right">
    <div class="cardBox">
      {#each [colorTarget, verifyTarget] as target, i}
        {#if target}
          <div class={`card ${i === 0 ? "colorTarget" : "verificationTarget"}`}>
            <h2>{target.name}</h2>
            <div class="rowcol">
              <div class="inputGroup">
                <span>Rows</span>
                <input
                  placeholder="1..26 [a-z]"
                  type="number"
                  bind:value={target.rows}
                />
              </div>
              <span class="times">x</span>
              <div class="inputGroup">
                <span>Cols</span>
                <input
                  placeholder="1..26 [a-z]"
                  type="number"
                  bind:value={target.cols}
                />
              </div>
            </div>
            <div class="extra">
              <button>RefData</button>
              <input type="range" bind:value={target.color} max="360" />
              {target.color}
            </div>
            <button class="close" on:click={() => removeTarget(i)}>X</button>
          </div>
        {/if}
      {/each}
      <button class="addTarget" on:click={() => addTarget()}>+</button>
    </div>
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
    @apply w-[40vw] h-full flex justify-center bg-gray-700;
  }

  .cardBox {
    @apply bg-gray-800 w-full m-6 p-2 gap-2 flex flex-col items-center;
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
    @apply flex justify-between items-center;
  }

  .addTarget {
    @apply bg-green-500 w-full h-12;
  }

  .rowcol input {
    @apply p-0.5 bg-gray-900 border-2 border-gray-800 rounded-lg
          focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2
            h-full w-full;
  }

  .extra {
    @apply bg-gray-700 p-2;
  }

  .inputGroup {
    @apply flex flex-col items-center;
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
</style>
