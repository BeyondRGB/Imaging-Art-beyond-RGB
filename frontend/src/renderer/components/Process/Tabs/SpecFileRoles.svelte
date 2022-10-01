<script lang="ts">
  import { currentPage, processState } from "@util/stores";
  import Dropbox from "@components/Process/Dropbox.svelte";
  $: if ($processState.artStacks[0].fields) {
    let done = true;
    Object.keys($processState.artStacks[0].fields).map((ele) => {
      if (
        $processState.artStacks[0].fields[ele].length === 0 &&
        !ele.includes("target")
      ) {
        done = false;
      }
    });
    if (done && !$processState.completedTabs[2]) {
      $processState.completedTabs[2] = true;
    }
  }
  let items = [];
</script>

<main>
  <panel>
      <h1>Specify Image Roles</h1>
      <p>Drag and drop each image into its appropriate role</p>
    <div id="mainImageBox">
      <Dropbox items={$processState.imageFilePaths} singleItem={false}/>
    </div>
  </panel>
  <right>
          <div id="imageStack">
              <div class="inputGroup">
                  <div class="imageLetter">A</div>
                  <div class="imageLetter">B</div>
              </div>
              <div class="text">Object</div>
              <div class="inputGroup">
                  <div class="cell"><Dropbox items={items} singleItem={true}/></div>
                  <div class="cell"><Dropbox items={items} singleItem={true}/></div>
              </div>
              <div class="text">Target</div>
              <div class="inputGroup">
                  <div class="cell"><Dropbox items={items} singleItem={true}/></div>
                  <div class="cell"><Dropbox items={items} singleItem={true}/></div>
              </div>
              <div class="text">FlatField</div>
              <div class="inputGroup">
                  <div class="cell"><Dropbox items={items} singleItem={true}/></div>
                  <div class="cell"><Dropbox items={items} singleItem={true}/></div>
              </div>
              <div class="text">DarkField</div>
              <div class="inputGroup">
                  <div class="cell"><Dropbox items={items} singleItem={true}/></div>
                  <div class="cell"><Dropbox items={items} singleItem={true}/></div>
              </div>
          </div>
  </right>
</main>

<style lang="postcss">
  main {
    @apply flex justify-between h-full w-full overflow-hidden;
  }
  panel {
    width: 60%;
    background-color: #3a3a3c;
  }
  right {
    @apply bg-gray-700 w-full h-full p-6 flex justify-center overflow-auto;
  }
  Heapbox {
  }
  h1 {
    margin: 25px;
    font-size: 35px;
    width: 100%;
  }
  p {
    margin: 25px;
    font-size: 18px;
    width: 100%;
  }
  #mainImageBox {
      display: block;
      margin-left: auto;
      margin-right: auto;
      width: 75%;
      margin-top: 20%;
      flex-basis: auto;
      background-color: #2c2c2f;
      border-radius: 20px;
      padding-top: 20px;
      padding-bottom: 20px;
  }
  #imageStack {
      width: 65%;
      margin: auto;
      flex-basis: auto;
      background-color: #2c2c2f;
      border-radius: 20px;
      height: auto;
      display: flex;
      flex-wrap: wrap;
      padding: 20px;
      background-color: #3a3a3d;
      justify-content: center;
  }
  .inputGroup {
      display: flex;
      flex-direction: row;
      height: auto;
      gap: 20px;
      width: 100%;
      justify-content: center;
  }
  .cell {
      width: 50%;
  }
  .text {
      margin-top: 20px;
      text-align: center;
  }
  .imageLetter {
      width: 50%;
      margin-top: 20px;
      text-align: center;
      font-size: 30px;
  }

</style>
