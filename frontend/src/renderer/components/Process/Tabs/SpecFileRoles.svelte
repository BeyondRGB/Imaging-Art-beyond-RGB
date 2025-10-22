<script lang="ts">
    import { processState } from "@util/stores";
    import Dropbox from "@components/Process/Dropbox.svelte";
    import {get, isEmpty, each, includes} from "lodash";
    import { autoSortImages } from "@util/autoSortStandards.svelte";

    let imageStack = get($processState, 'artStacks[0].fields');
    let rerenderToggle = false;
    let validationError = null;

    // this helps force a rerender once the imageStack has been reset
    $:if ($processState.currentTab === 3) {
        imageStack = get($processState, 'artStacks[0].fields');

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
        $processState.imageFilePaths = autoSortImages(getAllImages(), imageStack);
        rerenderToggle = !rerenderToggle;
    };

    const validate = function () {
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
        if(validationError = validate()) {
            return;
        }
        $processState.imageFilePaths = [];
        $processState.completedTabs[3] = true;
        if(skipOptionalFiltering) {
            $processState.currentTab += 2;
        } else {
            $processState.currentTab++;
        }
    };

</script>

<main>
    {#key rerenderToggle}
        <panel>
            <h1>Specify Image Roles</h1>
            <p>Drag and drop each image into its appropriate role</p>
            <div>
            <Dropbox bind:items={$processState.imageFilePaths} type="image" singleItem={false}/>
            <div class="btnGroup">
                <button class="autoSortButton" on:click={autoSort}>Auto-sort images</button>
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
                    <div class="text">Object</div>
                    <div class="inputGroup">
                        <div class="cell"><Dropbox type="image" bind:items={imageStack.imageA[0]} singleItem={true} showError={!!validationError}/></div>
                        <div class="cell"><Dropbox type="image" bind:items={imageStack.imageB[0]} singleItem={true} showError={!!validationError}/></div>
                    </div>
                    {#if $processState.imageFilePaths && showTargetDropZones()}
                        <div class="text">Target</div>
                        <div class="inputGroup">
                            <div class="cell"><Dropbox type="image" bind:items={imageStack.targetA} singleItem={true} showError={!!validationError}/></div>
                            <div class="cell"><Dropbox type="image" bind:items={imageStack.targetB} singleItem={true} showError={!!validationError}/></div>
                        </div>
                    {/if}
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
                    <button on:click={() => submitSpecFileRoles(false)}>Optional filtering</button>
                    <button on:click={() => submitSpecFileRoles(true)} class="nextBtn">Next: Skip optional filtering</button>
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
        background-color: #3a3a3c;
    }
    right {
        background-color: var(--color-surface);
        @apply w-full h-full p-6 flex justify-center overflow-auto;
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
    #imageStack {
        background-color: #3a3a3d;
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
    }
    .cell {
        width: 50%;
    }
    .text {
        text-align: center;
    }
    .imageLetter {
        width: 50%;
        margin-top: 20px;
        text-align: center;
        font-size: 30px;
    }
    .autoSortButton {
        margin: 50px 65px 0 0;
    }
    .errorText {
        text-align: center;
        color: red;
    }
    .btnGroup {
        @apply flex justify-end gap-2;
    }
    .nextBtn {
        @apply m-4 bg-green-700 hover:bg-green-600 focus:ring-green-600 transition-all;
    }
</style>
