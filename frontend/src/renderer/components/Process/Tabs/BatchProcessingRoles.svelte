
<script lang="ts">
    import { processState, batchProcessState, batchImagesA, batchImagesB } from "@util/stores";
    import Dropbox from "@components/Process/Dropbox.svelte";
    import Button from "@components/Button.svelte";
    import {get, isEmpty, each, includes} from "lodash";
    import { autoSortBatchImages } from "@util/autoSortStandards.svelte";
    import { countFields } from "@root/util/storesUtil";

    let imageStack = get($processState, 'artStacks[0].fields');
    let artImageStackA = get($batchImagesA);
    let artImageStackB = get($batchImagesB, 'artStacks[0].fields.imageB');
    let artImageCount = 1;

    let rerenderToggle = false;
    let validationError = null;

    // this helps force a rerender once the imageStack has been reset
    $:if ($processState.currentTab === 3) {
        imageStack = get($processState, 'artStacks[0].fields');
        
    }
    // get art image count after images are updated but before we render this screen.
    $:if ($processState.currentTab ===2){
        let totalImageCount = $processState.imageFilePaths.length + countFields($processState.artStacks[0].fields); 
        if(totalImageCount >= 6 && totalImageCount <=8  ){
            artImageCount = 1;
        }else if ( totalImageCount > 8){
            artImageCount = Math.ceil((totalImageCount - 6) / 2);
        }
    }

    const getAllImages = function () {
        let allImages = [];
        each(imageStack, function (field) {
            if(!isEmpty(field) && !isEmpty(field[0])) {
                allImages.push(field[0]);
            }
        });
        each($processState.imageFilePaths, function (image) {
            if(!includes(allImages, image)) {
                allImages.push(image);
            }
        });
        return allImages;
    };

    const showTargetDropZones = function () {
        return getAllImages().length > 6;
    };

    const autoSort = function () {
        processState.update(state => ({
          ...state,
          imageFilePaths: autoSortBatchImages(getAllImages(), imageStack)
        }));
        rerenderToggle = !rerenderToggle;
    };

    const validate = function () {
        console.log($batchProcessState)
        console.log(imageStack)
        if(showTargetDropZones() && (isEmpty(imageStack?.targetA) || isEmpty(imageStack?.targetB)) ||
            isEmpty(imageStack?.imageA) || isEmpty(imageStack?.imageB) ||
            isEmpty(imageStack?.flatfieldA) || isEmpty(imageStack?.flatfieldB) ||
            isEmpty(imageStack?.darkfieldA) || isEmpty(imageStack?.darkfieldB)) {
            return  "Please ensure each dropzone is assigned an image.";
        } else {
            return null;
        }
        return validationError;
    };

    const submitSpecFileRoles = function (skipOptionalFiltering) {
        validate()
        if(validationError = validate()) {
            return;
        }
        processState.update(state => ({
          ...state,
          imageFilePaths: [],
          completedTabs: state.completedTabs.map((completed, i) => i === 3 ? true : completed),
          currentTab: skipOptionalFiltering ? state.currentTab + 2 : state.currentTab + 1
        }));
    };

</script>

<main>
    {#key rerenderToggle}
        <panel>
            <div class="leftHeader">
                <h1>Specify Image Roles Batch</h1>
                <p>Drag and drop each image into its appropriate role</p>
                <div class="leftStartBox">
                    <Dropbox bind:items={$processState.imageFilePaths} type="image" singleItem={false}/>
                </div>
                <div class="btnGroup" style="margin: 50px 65px 2px 0;">
                    <Button variant="secondary" size="md" onClick={autoSort}>Auto-sort images</Button>
                </div>
                
            </div>
            <div class="leftBoxes">

                <div class="centerFlexBox">
                <div id="imageStack">
                    <div class="inputGroup">
                        <div class="imageLetter">A</div>
                        <div class="imageLetter">B</div>
                    </div>
                    <div class='objectDropBoxes'>
                    <div class="text">Object</div>
                    {#each Array(artImageCount) as _, index (index)}
                    <div class="inputGroup">
                        <div class="cell"><Dropbox type="image" bind:items={imageStack.imageA[index]} singleItem={true} showError={!!validationError}/></div>
                        <div class="cell"><Dropbox type="image" bind:items={imageStack.imageB[index]} singleItem={true} showError={!!validationError}/></div>
                    </div>
                    {/each}
                    </div>
                    <br>
                </div>
                
            </div>
            </div>
        </panel>
        <right>
            <div class="centerFlexBox">
                <div id="imageStack">
                    <div class="inputGroup">
                        <div class="imageLetter">A</div>
                        <div class="imageLetter">B</div>
                    </div>
                    <!-- <div class="text">Object</div>
                    <div class="inputGroup">
                        <div class="cell"><Dropbox type="image" bind:items={imageStack.imageA} singleItem={true} showError={!!validationError}/></div>
                        <div class="cell"><Dropbox type="image" bind:items={imageStack.imageB} singleItem={true} showError={!!validationError}/></div>
                    </div> -->
                        <div class="text">Target</div>
                        <div class="inputGroup">
                            <div class="cell"><Dropbox type="image" bind:items={imageStack.targetA} singleItem={true} showError={!!validationError}/></div>
                            <div class="cell"><Dropbox type="image" bind:items={imageStack.targetB} singleItem={true} showError={!!validationError}/></div>
                        </div>
                    <div class="text">FlatField</div>
                    <div class="inputGroup">
                        <div class="cell"><Dropbox type="image" bind:items={imageStack.flatfieldA} singleItem={true} showError={!!validationError}/></div>
                        <div class="cell"><Dropbox type="image" bind:items={imageStack.flatfieldB} singleItem={true} showError={!!validationError}/></div>
                    </div>
                    <div class="text">DarkField</div>
                    <div class="inputGroup">
                        <div class="cell"><Dropbox type="image" bind:items={imageStack.darkfieldA} singleItem={true} showError={!!validationError}/></div>
                        <div class="cell"><Dropbox type="image" bind:items={imageStack.darkfieldB} singleItem={true} showError={!!validationError}/></div>
                    </div>
                    {#if validationError && imageStack && validate()}
                        <div class="errorText">
                            {validationError}
                        </div>
                    {/if}
                    <br>
                </div>
                <div class="btnGroup">
                    <Button variant="secondary" size="md" onClick={() => submitSpecFileRoles(false)}>Optional filtering</Button>
                    <Button variant="success" size="md" onClick={() => submitSpecFileRoles(true)}>Next: Skip optional filtering</Button>
                </div>
            </div>
        </right>
    {/key}
</main>

<style lang="postcss">
    main {
        @apply flex justify-between h-full w-full overflow-hidden;
    }
    panel {
        width: 100%;
        background-color: var(--color-surface-elevated);
    }
    right {
        background-color: var(--color-surface);
        @apply w-full h-full p-6 flex justify-center overflow-auto;
    }
    h1 {
        color: var(--color-text-primary);
        margin: 25px;
        font-size: 35px;
        width: 100%;
    }
    p {
        color: var(--color-text-secondary);
        margin: 25px;
        font-size: 18px;
        width: 100%;
    }
    #imageStack {
        background-color: var(--color-surface-base);
        border: 1px solid var(--color-border);
        margin-top: auto;
        margin-bottom: auto;
        border-radius: 30px;
        display: flex;
        flex-direction: column;
        gap:20px;
    }
    .centerFlexBox {
        display: flex;
        flex-direction: column;
        width: 80%;
    }
    .inputGroup {
        display: flex;
        flex-direction: row;
        height: auto;
        gap: 20px;
        width: 100%;
        justify-content: center;
        padding-top: 20px;
    }
    .cell {
        width: 50%;
    }
    .text {
        color: var(--color-text-secondary);
        text-align: center;
    }
    .imageLetter {
        color: var(--color-text-primary);
        width: 50%;
        margin-top: 20px;
        text-align: center;
        font-size: 30px;
    }
    .errorText {
        text-align: center;
        color: var(--color-error);
    }
    .btnGroup {
        @apply flex justify-end gap-2;
    }
    .leftHeader {
        max-height:300px;
    }
    .leftBoxes {
        margin-top: 5px;
        max-height: calc(100% - 300px);
        overflow-y:auto;
        scrollbar-width: 10px;
        padding-left: 20px;
    }
    .leftStartBox {
        max-height: 100px;
        overflow-y:auto;
        scrollbar-width: 5px;
    }
</style>
