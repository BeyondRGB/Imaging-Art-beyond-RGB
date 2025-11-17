<script>
  import {
    customRefData,
    persistentCustomRefData,
    processState,
    modal,
    sendMessage,
  } from "@util/stores";
  import ColorTargetViewer from "@components/Process/ColorTargetViewer.svelte";

  import {
    PlusCircleIcon,
    XIcon,
    AlertTriangleIcon,
  } from "svelte-feather-icons";
  import Dropdown from "@root/components/Dropdown.svelte";
  import Card from "@components/Card.svelte";
  import ScrollContainer from "@components/ScrollContainer.svelte";
  import TextInputRow from "@components/TextInputRow.svelte";

  let colorTarget;
  let colorPos;
  let verifyTarget;
  let verifyPos;
  let viewerOpen;

  let colorTargetViewer;

  let targetArray;

  let loading = false;

  let staticRefData = [
    {
      name: "NGT",
      fileName: "NGT_Reflectance_Data.csv",
      standardObserver: 1931,
      illuminants: "D50",
    },
    {
      name: "APT",
      fileName: "APT_Reflectance_Data.csv",
      standardObserver: 1931,
      illuminants: "D50",
    },
    {
      name: "CCSG",
      fileName: "CCSG_Reflectance_Data.csv",
      standardObserver: 1931,
      illuminants: "D50",
    },
    {
      name: "CC Classic",
      fileName: "CC_Classic_Reflectance_Data.csv",
      standardObserver: 1931,
      illuminants: "D50",
    }
  ]

  let refDataMeta = [
    { rows: 10, cols: 13 },
    { rows: 4, cols: 6 },
    { rows: 10, cols: 14 },
    { rows: 4, cols: 6 },
  ];

  let calibrationTargetRotationAngle = 0; // Default rotation angle in degrees

  function update() {
    processState.update(state => {
      const updatedStack = { ...state.artStacks[0] };
      
      // Update color target if present
      if (colorPos && colorTarget) {
        const refDataName = colorTarget.refData.name !== "CUSTOM DATA" 
          ? colorTarget.refData.name 
          : $customRefData.calibration.name;
          
        updatedStack.colorTarget = {
          ...updatedStack.colorTarget,
          top: colorPos.top,
          left: colorPos.left,
          bottom: colorPos.bottom,
          right: colorPos.right,
          rows: colorTarget.rows,
          cols: colorTarget.cols,
          size: colorTarget.size,
          resolution: colorTargetViewer.getResolution(),
          whitePatch: colorTarget.whitePatch,
          refData: {
            ...colorTarget.refData,
            name: refDataName
          },
          calibrationTargetRotationAngle: calibrationTargetRotationAngle
        };
      }
      
      // Update verification target if present
      if (verifyPos && verifyTarget) {
        const verifyRefDataName = verifyTarget.refData.name !== "CUSTOM DATA"
          ? verifyTarget.refData.name
          : verifyTarget.refData.name;
          
        updatedStack.verificationTarget = {
          top: verifyPos.top,
          left: verifyPos.left,
          bottom: verifyPos.bottom,
          right: verifyPos.right,
          rows: verifyTarget.rows,
          cols: verifyTarget.cols,
          size: verifyTarget.size,
          resolution: colorTargetViewer.getResolution(),
          whitePatch: verifyTarget.whitePatch,
          refData: {
            ...verifyTarget.refData,
            name: verifyRefDataName
          },
          calibrationTargetRotationAngle: 0
        };
      } else {
        updatedStack.verificationTarget = null;
      }
      
      return {
        ...state,
        artStacks: [updatedStack, ...state.artStacks.slice(1)]
      };
    });

    // Clear local state after updating store
    colorTarget = null;
    colorPos = null;
    verifyTarget = null;
    verifyPos = null;
    targetArray = null;
    loading = false;
  }

  function colorTargetPrev() {
    processState.update(state => ({
      ...state,
      colorTargetID: Math.floor(Math.random() * 999999999)
    }));
    console.log($processState);

    let targetImage = "";
    


    if ($processState.artStacks[0].fields.targetA.length !== 0) {
      console.log("Found Target");
      targetImage = $processState.artStacks[0].fields.targetA[0].name;
    }
    else{
      console.log("Didnt Find Target");
      targetImage = $processState.artStacks[0].fields.imageA[0][0].name; 
    }

    let msg = {
      RequestID: $processState.colorTargetID,
      RequestType: "HalfSizePreview",
      RequestData: {
        names: [targetImage],
      },
    };
    if (
     // $processState.artStacks[0].fields.imageA[0].name.length > 2 &&
      !loading
    ) {
      console.log("Getting Color Target Preview");
      console.log(msg);
      loading = true;
      sendMessage(JSON.stringify(msg));
    }
  }

  $: if ($processState.currentTab === 6) {
    console.log("Update");
    console.log($processState);
    update();
    console.log($processState);
  }

  $: if (
    $processState.currentTab === 5 &&
    $processState.artStacks[0].colorTargetImage?.filename?.length === 0 &&
    $processState.artStacks[0].fields.imageA[0] != null
  ) {
    console.log("Getting Color Target Preview");
    console.log($processState);
    colorTargetPrev();
  }

  $: console.log($processState);

  function buildCalibrationRefData() {
    return [
      ...staticRefData,
      ...$persistentCustomRefData.calibration,
      {
        name: "Choose a Custom CSV File",
        fileName: "Choose a custom file....csv",
      },
    ];
  }

  function buildVerificationRefData() {
    return [
      ...staticRefData,
      ...$persistentCustomRefData.verification,
      {
        name: "Choose a Custom CSV File",
        fileName: "Choose a custom file....csv",
      },
    ];
  }

  function addTarget() {
    if (!colorTarget) {
      colorTarget = {
        name: "Calibration Target",
        rows: 10,
        cols: 10,
        color: Math.floor(Math.random() * (360 - 0 + 1) + 0),
        size: 0.5,
        whitePatch: {
          row: null,
          col: null,
        },
        refData: {
          name: "---None---",
          fileName: "---None---.csv",
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
        color: Math.floor(Math.random() * (360 - 0 + 1) + 0),
        size: 0.5,
        whitePatch: {
          row: 1,
          col: 1,
        },
        refData: {
          name: "---None---",
          fileName: "---None---.csv",
          standardObserver: 1931,
          illuminants: "D50",
        },
      };
      verifyPos = { top: 0.25, left: 0.25, bottom: 0.5, right: 0.5 };
    }
  }

  function removeTarget(id) {
    console.log("Remove");
    if (id === 0) {
      if (verifyTarget == null) {
        console.log("Removing Color Target");
        colorTarget = null;
        colorPos = null;
      }
    } else if (id === 1) {
      console.log("Removing Verify Target");
      verifyTarget = null;
      verifyPos = null;
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

  $: {
    targetArray = [];
    if (colorTarget) {
      targetArray = [colorTarget];
    }
    if (verifyTarget) {
      targetArray = [...targetArray, verifyTarget];
    }
  }

  // If processing is terminated by a server error or something else, then re-enter the previous data
  $: if ($processState.returnedFromProcessing) {
    resetTargets();
  }

  function resetTargets() {
    if ($processState.artStacks[0].colorTarget && Object.keys($processState.artStacks[0].colorTarget).length !== 0) {
      addDataToTarget()
    }
    if ($processState.artStacks[0].verificationTarget && Object.keys($processState.artStacks[0].verificationTarget).length !== 0) {
      addDataToTarget();
    }
    processState.update(state => ({
      ...state,
      returnedFromProcessing: false
    }));
  }

  // Add data to targets from $processState
  function addDataToTarget() {
    if (!colorTarget) {
      colorTarget = {
        name: "Calibration Target",
        rows: $processState.artStacks[0].colorTarget.rows,
        cols: $processState.artStacks[0].colorTarget.cols,
        color: Math.floor(Math.random() * (360 - 0 + 1) + 0),
        size: $processState.artStacks[0].colorTarget.size,
        whitePatch: {
          row: $processState.artStacks[0].colorTarget.whitePatch.row,
          col: $processState.artStacks[0].colorTarget.whitePatch.col,
        },
        refData: {
          name: $processState.artStacks[0].colorTarget.refData.name,
          fileName: $processState.artStacks[0].colorTarget.refData.fileName,
          standardObserver: $processState.artStacks[0].colorTarget.refData.standardObserver,
          illuminants: $processState.artStacks[0].colorTarget.refData.illuminants,
        },
      };
      colorPos = { top: $processState.artStacks[0].colorTarget.top, left: $processState.artStacks[0].colorTarget.left,
         bottom: $processState.artStacks[0].colorTarget.bottom, right: $processState.artStacks[0].colorTarget.right };
    } else if (!verifyTarget) {
      verifyTarget = {
        name: "Verification Target",
        rows: $processState.artStacks[0].verificationTarget.rows,
        cols: $processState.artStacks[0].verificationTarget.cols,
        color: Math.floor(Math.random() * (360 - 0 + 1) + 0),
        size: $processState.artStacks[0].verificationTarget.size,
        whitePatch: {
          row: $processState.artStacks[0].verificationTarget.whitePatch.row,
          col: $processState.artStacks[0].verificationTarget.whitePatch.col,
        },
        refData: {
          name: $processState.artStacks[0].verificationTarget.refData.name,
          fileName: $processState.artStacks[0].verificationTarget.refData.fileName,
          standardObserver: $processState.artStacks[0].verificationTarget.refData.standardObserver,
          illuminants: $processState.artStacks[0].verificationTarget.refData.illuminants,
        },
      };
      verifyPos = { top: $processState.artStacks[0].verificationTarget.top, left: $processState.artStacks[0].verificationTarget.left, 
        bottom: $processState.artStacks[0].verificationTarget.bottom, right: $processState.artStacks[0].verificationTarget.right };
    }
  }

  $: if (colorTarget?.refData?.fileName === "Choose a custom file....csv") {
    console.log("OPENING CUSTOM REF MODAL");
    modal.set("CustomRefData");
    colorTarget.refData.name = "CUSTOM DATA";
  }
  $: if (verifyTarget?.refData?.fileName === "Choose a custom file....csv") {
    console.log("OPENING CUSTOM REF MODAL VER");
    modal.set("CustomRefDataVer");
    verifyTarget.refData.name = "CUSTOM DATA";
  }

  $: if (colorTarget?.refData?.name === "CUSTOM DATA" && $customRefData.calibration !== null) {
    console.log("Resetting Custom Calibration Ref Data");
    colorTarget.refData = {
      name: $customRefData.calibration.name,
      fileName: $customRefData.calibration.fileName,
      standardObserver: $customRefData.calibration.standardObserver,
      illuminants: $customRefData.calibration.illuminants,
    };
    $customRefData.calibration = null;
  }

  $: if (verifyTarget?.refData?.name === "CUSTOM DATA" && $customRefData.verification !== null) {
    console.log("Resetting Custom Verification Ref Data");
    verifyTarget.refData = {
      name: $customRefData.verification.name,
      fileName: $customRefData.verification.fileName,
      standardObserver: $customRefData.verification.standardObserver,
      illuminants: $customRefData.verification.illuminants,
    }
    $customRefData.verification = null;
  }

  $: if (staticRefData.includes(colorTarget?.refData)) {
    console.log("Setting Refdata ROW/COl");
    let index = staticRefData.findIndex((x) => x === colorTarget.refData);

    colorTarget.rows = refDataMeta[index].rows;
    colorTarget.cols = refDataMeta[index].cols;
  }
  $: if (staticRefData.includes(verifyTarget?.refData)) {
    let index = staticRefData.findIndex((x) => x === verifyTarget.refData);

    verifyTarget.rows = refDataMeta[index].rows;
    verifyTarget.cols = refDataMeta[index].cols;
  }

  $: console.log({ LOADING: loading });

  $: if (
    colorTarget &&
    colorTarget.refData.fileName !== "---None---.csv" &&
    (!verifyTarget || verifyTarget.refData?.fileName !== "---None---.csv") &&
    colorTarget?.whitePatch?.row &&
    colorTarget?.whitePatch?.col
  ) {
    processState.update(state => ({
      ...state,
      completedTabs: state.completedTabs.map((completed, i) => i === 5 ? true : completed)
    }));
  } else {
    processState.update(state => ({
      ...state,
      completedTabs: state.completedTabs.map((completed, i) => i === 5 ? false : completed)
    }));
  }

  $: if (colorTarget) {
    processState.update(state => ({
      ...state,
      whitePatchFilled: colorTarget.whitePatch?.row && colorTarget.whitePatch?.col
    }));
  }

  // Clamp the white patch rows to the number of rows in the target
  $: if (
      colorTarget !== undefined &&
      colorTarget !== null &&
      colorTarget.whitePatch !== undefined &&
      colorTarget.whitePatch.row !== null && // The row is null when the user deletes all content (usually when typing).
      colorTarget.whitePatch.row > colorTarget.rows
  ) {
    colorTarget.whitePatch.row = colorTarget.rows;
  } else if (
      colorTarget !== undefined &&
      colorTarget !== null &&
      colorTarget.whitePatch !== undefined &&
      colorTarget.whitePatch.row !== null && // The row is null when the user deletes all content (usually when typing).
      colorTarget.whitePatch.row < 1
  ) {
    colorTarget.whitePatch.row = 1;
  }

  // Clamp the white patch columns to the number of columns in the target
  $: if (
      colorTarget !== undefined &&
      colorTarget !== null &&
      colorTarget.whitePatch !== undefined &&
      colorTarget.whitePatch.col !== null && // The column is null when the user deletes all content (usually when typing).
      colorTarget.whitePatch.col > colorTarget.cols
  ) {
      colorTarget.whitePatch.col = colorTarget.cols;
  } else if (
      colorTarget !== undefined &&
      colorTarget !== null &&
      colorTarget.whitePatch !== undefined &&
      colorTarget.whitePatch.col !== null && // The column is null when the user deletes all content (usually when typing).
      colorTarget.whitePatch.col < 1
  ) {
      colorTarget.whitePatch.col = 1;
  }
</script>

<main>
  <div class="left">
    <div class="title">Target Patch Selection</div>
    <div class="left-content">
      <div class="image-container">
        {#if loading}
          <div class="loading">
            <div class="loading-box">Loading<span class="loader" /></div>
          </div>
        {/if}
        <ColorTargetViewer
          bind:colorTarget
          bind:verifyTarget
          bind:colorPos
          bind:verifyPos
          bind:loading
          bind:viewerOpen
          bind:calibrationTargetRotationAngle={calibrationTargetRotationAngle}
          bind:this={colorTargetViewer}
        />
      </div>
    </div>
  </div>
  <div class="right">
    <!-- <div class="boxHead">Targets</div> -->
    <Card variant="dark" padding="sm" rounded={true} className="cardBox">
      <ScrollContainer maxHeight="none" className="scroll-content">
      {#if viewerOpen}
      {#each [...targetArray, "Add"] as target, i (target)}
        {#if target === "Add" && i < 2}
            <Card
              className="addCard {i === 1 ? 'verificationAdd' : ''}"
            on:click={() => addTarget()}
              interactive={true}
              padding="md"
              rounded={true}
          >
            <div class="addCard-content">
              <PlusCircleIcon size="1.5x" class="addCard-icon" />
              <span class="addCard-text">
                {#if i === 0}
                  Add Calibration Target
                {:else}
                  Add Verification Target
                {/if}
              </span>
            </div>
            </Card>
        {:else if target !== "Add"}
            <Card
              className="card {i === 0 ? 'colorTarget' : ''} {i !== 0 ? 'verificationTarget' : ''}"
              variant="default"
              padding="none"
              rounded={true}
          >
            <div class="card-content">
            <h2>{target.name}</h2>
            <input
              type="range"
              class="colorSlider"
              bind:value={target.color}
              max="360"
            />

            <div class="rowcol">
              <TextInputRow
                label="Rows"
                type="number"
                min={1}
                bind:value={target.rows}
                placeholder="1-26"
              />
              <TextInputRow
                label="Columns"
                type="number"
                min={1}
                bind:value={target.cols}
                placeholder="1-26"
              />
            </div>
            <div class="refDataDiv">
              <span class="validatedTitle">
                {#if target.refData.fileName === "---None---.csv"}
                  <span class="invalid"
                    ><AlertTriangleIcon size="1.5x" />
                  </span>
                {/if}
                Reference Data:</span
              >
              <Dropdown
                values={i === 0 ? buildCalibrationRefData() : buildVerificationRefData() }
                bind:selected={target.refData}
                invalid={target.refData.fileName === "---None---.csv"}
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
            {#if i !== 1}
              <div class="break" />
              <div class="whitePatchBox">
                {#if target.whitePatch.row === null || target.whitePatch.col === null}
                  <span class="invalid"
                    ><AlertTriangleIcon size="1.5x" />
                  </span>
                {/if}
                <div class="whitePatchInfo">
                  <span> White Patch Location</span>
                  <div class="whitePatch">
                    <div class="inputGroup">
                      <span>Row: </span>
                      <input
                        placeholder="Row #"
                        type="number"
                        min="1"
                        max={target.rows}
                        bind:value={target.whitePatch.row}
                      />
                    </div>
                    <span class="and">&</span>
                    <div class="inputGroup">
                      <span>Col: </span>
                      <input
                        placeholder="Col #"
                        type="number"
                        min="1"
                        max={target.cols}
                        bind:value={target.whitePatch.col}
                      />
                    </div>
                  </div>
                </div>
              </div>
              <div class="target-coordinates">
                <h3>Color Target Coordinates</h3>
                <div class="inputGroup">
                  <span>Top:</span>
                  <input
                    type="number"
                    step="1"
                    value={colorPos.top * colorTargetViewer.getResolution()}
                    on:change={() => {
                      colorPos.top =
                        event.target.value / colorTargetViewer.getResolution();
                      colorTargetViewer.updateCoords();
                    }}
                  />
                </div>
                <div class="inputGroup">
                  <span>Bottom:</span>
                  <input
                    type="number"
                    step="1"
                    value={colorPos.bottom * colorTargetViewer.getResolution()}
                    on:change={() => {
                      colorPos.bottom =
                        event.target.value / colorTargetViewer.getResolution();
                      colorTargetViewer.updateCoords();
                    }}
                  />
                </div>
                <div class="inputGroup">
                  <span>Right:</span>
                  <input
                    type="number"
                    step="1"
                    value={colorPos.right * colorTargetViewer.getResolution()}
                    on:change={() => {
                      colorPos.right =
                        event.target.value / colorTargetViewer.getResolution();
                      colorTargetViewer.updateCoords();
                    }}
                  />
                </div>
                <div class="inputGroup">
                  <span>Left:</span>
                  <input
                    type="number"
                    step="1"
                    value={colorPos.left * colorTargetViewer.getResolution()}
                    on:change={() => {
                      colorPos.left =
                        event.target.value / colorTargetViewer.getResolution();
                      colorTargetViewer.updateCoords();
                    }}
                  />
                </div>
                <div>
                  <label for="rotation-slider">Rotation Angle (degrees):</label>
                  <input
                    id="rotation-slider"
                    type="range"
                    min="-180"
                    max="180"
                    step="0.01"
                    value="{calibrationTargetRotationAngle}"
                    on:input="{(e) => {
                      calibrationTargetRotationAngle = parseFloat(event.target.value);  
                    }}"
                  />
                  <span>{calibrationTargetRotationAngle}°</span>
                </div>                
              </div>
            {:else if target !== "Add"}
              <div class="target-coordinates">
                <h3>Color Target Coordinates</h3>
                <div class="inputGroup">
                  <span>Top:</span>
                  <input
                    type="number"
                    step="1"
                    value={verifyPos.top * colorTargetViewer.getResolution()}
                    on:change={() => {
                      verifyPos.top =
                        event.target.value / colorTargetViewer.getResolution();
                      colorTargetViewer.updateVerifyCoords();
                    }}
                  />
                </div>
                <div class="inputGroup">
                  <span>Bottom:</span>
                  <input
                    type="number"
                    step="1"
                    value={verifyPos.bottom * colorTargetViewer.getResolution()}
                    on:change={() => {
                      verifyPos.bottom =
                        event.target.value / colorTargetViewer.getResolution();
                      colorTargetViewer.updateVerifyCoords();
                    }}
                  />
                </div>
                <div class="inputGroup">
                  <span>Right:</span>
                  <input
                    type="number"
                    step="1"
                    value={verifyPos.right * colorTargetViewer.getResolution()}
                    on:change={() => {
                      verifyPos.right =
                        event.target.value / colorTargetViewer.getResolution();
                      colorTargetViewer.updateVerifyCoords();
                    }}
                  />
                </div>
                <div class="inputGroup">
                  <span>Left:</span>
                  <input
                    type="number"
                    step="1"
                    value={verifyPos.left * colorTargetViewer.getResolution()}
                    on:change={() => {
                      verifyPos.left =
                        event.target.value / colorTargetViewer.getResolution();
                      colorTargetViewer.updateVerifyCoords();
                    }}
                  />
                </div>
              </div>
            {/if}
            <button
              class="close"
              disabled={i === 0 &&
                typeof verifyTarget != "undefined" &&
                verifyTarget != null}
              on:click={() => removeTarget(i)}
              ><XIcon size="1.25x" /></button
            >
          </div>
            </Card>
        {/if}
      {/each}
      {/if}
      </ScrollContainer>
    </Card>
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
    background-color: var(--color-surface-elevated);
    @apply w-full h-full flex flex-col justify-start p-2 m-1 gap-2;
  }

  .title {
    @apply text-3xl p-1;
  }

  .right {
    background-color: var(--color-surface);
    @apply w-[40vw] h-full flex flex-col m-1 py-2 items-center overflow-hidden;
  }

  :global(.cardBox) {
    @apply flex-1 w-[90%] gap-2 flex flex-col items-center overflow-hidden !important;
  }
  
  :global(.scroll-content) {
    @apply w-full h-full flex flex-col gap-2 overflow-y-auto;
  }

  :global(.card) {
    @apply w-full h-auto flex flex-col gap-2 relative font-semibold !important;
    padding-top: 0.5rem;
    padding-right: 0.5rem;
  }
  
  :global(.colorTarget) {
    border: 3px solid hsl(var(--color_hue), 100%, 50%) !important;
    box-shadow: 0 0 15px hsla(var(--color_hue), 100%, 50%, 0.4) !important;
  }
  
  :global(.verificationTarget) {
    border: 3px solid hsl(var(--verfiy_hue), 100%, 50%) !important;
    box-shadow: 0 0 15px hsla(var(--verfiy_hue), 100%, 50%, 0.4) !important;
  }
  .invalid {
    @apply text-red-600;
  }
  .whitePatchBox {
    @apply w-full flex justify-center items-center gap-2;
  }
  .whitePatchInfo {
    @apply flex flex-col w-full;
  }

  :global(.addCard) {
    background: linear-gradient(135deg, rgba(34, 197, 94, 0.08) 0%, rgba(34, 197, 94, 0.12) 100%);
    border: 2px dashed rgba(34, 197, 94, 0.3);
    @apply w-full h-auto min-h-[4rem] max-h-[5rem] flex flex-col gap-2 relative
	justify-center items-center transition-all duration-200 cursor-pointer
	hover:bg-green-500/15 hover:border-green-500/50 hover:shadow-md
	active:scale-[0.99] active:bg-green-500/20 !important;
  }
  :global(.verificationAdd) {
    background: linear-gradient(135deg, rgba(34, 197, 94, 0.05) 0%, rgba(34, 197, 94, 0.1) 100%);
    border: 2px dashed rgba(34, 197, 94, 0.25);
  }
  :global(.verificationAdd:hover) {
    background-color: rgba(34, 197, 94, 0.12);
    border-color: rgba(34, 197, 94, 0.4);
  }
  .addCard-content {
    @apply flex flex-row items-center justify-center gap-3 px-4 py-2 w-full;
  }
  .addCard-icon {
    color: rgba(34, 197, 94, 0.9);
    @apply transition-transform duration-200 flex-shrink-0;
  }
  :global(.addCard:hover) .addCard-icon {
    @apply scale-110;
    color: rgba(34, 197, 94, 1);
  }
  .addCard-text {
    @apply text-base font-medium;
    color: var(--color-text-primary);
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
  }
  .left-content {
    @apply w-full h-auto flex items-center;
  }
  .image-container {
    background-color: var(--color-surface-base);
    @apply relative w-full h-auto overflow-visible;
  }
  .validatedTitle {
    @apply flex items-center justify-center gap-2;
  }

  .card-content {
    @apply p-4 flex flex-col gap-3;
  }

  h2 {
    @apply text-lg justify-center flex items-center font-semibold;
  }

  .rowcol {
    @apply flex flex-col justify-between items-center gap-2;
  }

  .whitePatch {
    @apply flex justify-between items-center gap-1 text-base;
  }

  .addTarget {
    color: var(--color-text-primary);
    @apply bg-green-500 w-16 h-16 transition-all rounded-full flex items-center justify-center;
  }

  .removeButton {
    @apply hidden;
  }

  .rowcol input {
    text-align: center;
    background-color: var(--color-surface-sunken);
    border: 2px solid var(--color-border);
    @apply p-0.5 rounded-lg
	focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2
	h-full w-12 min-w-[1rem];
  }

  .whitePatch input {
    background-color: var(--color-surface-sunken);
    border: 2px solid var(--color-border);
    @apply p-0.5 rounded-lg
	focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2
	h-full w-full;
  }

  .extra {
    background-color: var(--color-surface);
    @apply p-2;
  }

  .rowcol .inputGroup {
    @apply flex justify-between items-center gap-2 w-full;
  }

  .whitePatch .inputGroup {
    @apply flex items-center gap-2 w-full;
  }

  .inputGroup > span {
    @apply font-semibold;
  }
  .times {
    @apply text-xl;
  }

  .close {
    background-color: var(--color-surface);
    color: var(--color-text-secondary);
    border: 1px solid var(--color-border);
    @apply absolute top-2 right-2 z-10 w-8 h-8 rounded-full 
           flex items-center justify-center transition-all duration-200
           shadow-md hover:shadow-lg hover:scale-110 focus:outline-none;
  }
  
  .close:focus {
    outline: 2px solid var(--color-border-focus);
    outline-offset: 2px;
  }
  
  .close:hover {
    background-color: var(--color-interactive-hover);
    color: var(--color-text-primary);
  }
  
  .close:active {
    transform: scale(1.05);
  }
  
  .close:disabled {
    opacity: 0.5;
    cursor: not-allowed;
  }
  
  .close:disabled:hover {
    background-color: var(--color-surface);
    color: var(--color-text-secondary);
    transform: none;
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
    background-color: var(--color-interactive);
    @apply w-4 h-4 cursor-pointer rounded-full outline outline-1;
    outline-color: var(--color-border);
  }
  .sizeDiv {
    @apply flex justify-between items-center;
  }
  .sizeDiv input {
    @apply w-1/2;
  }

  .loading {
    background-color: var(--color-surface);
    @apply absolute w-full h-full z-[49] flex justify-center items-center;
  }
  .loading-box {
    @apply h-full flex flex-col gap-2 justify-center items-center
	text-2xl;
  }
  .loader {
    /* position: absolute; */
    width: 48px;
    height: 48px;
    background: #11ff00;
    transform: rotateX(65deg) rotate(45deg);
    /* remove bellows command for perspective change */
    transform: perspective(200px) rotateX(65deg) rotate(45deg);
    color: rgb(255, 0, 0);
    animation: layers1 1s linear infinite alternate;
    @apply z-50;
  }
  .loader:after {
    content: "";
    position: absolute;
    inset: 0;
    background: rgb(0, 0, 255);
    animation: layerTr 1s linear infinite alternate;
  }

  @keyframes layers1 {
    0% {
      box-shadow: 0px 0px 0 0px;
    }
    90%,
    100% {
      box-shadow: 20px 20px 0 -4px;
    }
  }
  @keyframes layerTr {
    0% {
      transform: translate(0, 0) scale(1);
    }
    100% {
      transform: translate(-25px, -25px) scale(1);
    }
  }

  .rowcol input {
    text-align: center;
    background-color: var(--color-surface-sunken);
    border: 2px solid var(--color-border);
    @apply p-0.5 rounded-lg
	focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2
	h-full w-12 min-w-[1rem];
  }

  .target-coordinates input {
    background-color: var(--color-surface-sunken);
    border: 2px solid var(--color-border);
    @apply p-0.5 rounded-lg
	focus-visible:outline-blue-700 focus-visible:outline focus-visible:outline-2
	h-full w-full;
  }
</style>
