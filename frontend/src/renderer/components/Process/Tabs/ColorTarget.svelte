<script>
  let rows;
  let cols;

  import { currentPage, processState } from "@util/stores";
  import ColorTargetViewer from "@components/Process/ColorTargetViewer.svelte";
  import SelectBtn from "@components/SelectBtn.svelte";
  import Loader from "@root/components/Loader.svelte";
  import { draggable } from "svelte-drag";
  import { bullhorn } from "svelte-awesome/icons";
  let currentPos;
  function update() {
    if (currentPos) {
      $processState.artStacks[0].colorTarget.top = currentPos.top;
      $processState.artStacks[0].colorTarget.left = currentPos.left;
      $processState.artStacks[0].colorTarget.bottom = currentPos.bottom;
      $processState.artStacks[0].colorTarget.right = currentPos.right;
      $processState.artStacks[0].colorTarget.rows = rows;
      $processState.artStacks[0].colorTarget.cols = cols;
    }
  }

  let colorTargets = [
    {
      name: "Color Target",
      rows: null,
      cols: null,
      refData: null,
      color: "",
    },
  ];
</script>

<main>
  <!-- <img src="placeholder.jpg" alt="background image" /> -->
  <div class="left">
    <ColorTargetViewer
      bind:rows
      bind:cols
      bind:currentPos
      bind:hue={colorTargets[0].color}
    />
  </div>
  <div class="right">
    <div class="cardBox">
      {#each colorTargets as target}
        <div class="card">
          <h2>{target.name}</h2>
          <div class="rowcol">
            <div class="inputGroup">
              <span>Rows</span>
              <input placeholder="1..26 [a-z]" />
            </div>
            <span class="times">x</span>
            <div class="inputGroup">
              <span>Cols</span>
              <input placeholder="1..26 [a-z]" />
            </div>
          </div>
          <div class="extra">
            <button>RefData</button>
            <input type="range" bind:value={colorTargets[0].color} max="360" />
            {colorTargets[0].color}
          </div>
        </div>
      {/each}
    </div>

    <!-- <button on:click={() => update()}>SAVE TARGET INFO</button>
    <p>
      top: {$processState.artStacks[0].colorTarget.top.toFixed(4)} | left: {$processState.artStacks[0].colorTarget.left.toFixed(
        4
      )} | bottom:
      {$processState.artStacks[0].colorTarget.bottom.toFixed(4)} | right: {$processState.artStacks[0].colorTarget.right.toFixed(
        4
      )}
    </p> -->
  </div>
</main>

<style lang="postcss">
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
    @apply bg-gray-800 w-full m-6 p-2;
  }

  .card {
    @apply bg-gray-600 rounded-lg w-full min-h-[4rem] p-4 flex flex-col gap-1;
  }

  h2 {
    @apply text-lg justify-center flex items-center font-semibold;
  }

  .rowcol {
    @apply flex justify-between items-center;
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
</style>
