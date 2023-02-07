<script lang="ts">
    import { processState } from "@util/stores";
    import Dropbox from "@components/Process/Dropbox.svelte";
    import {get, isEmpty, each, includes} from "lodash";
    import { autoSortImages } from "@util/autoSortStandards.svelte";

    let imageStack = get($processState, 'artStacks[0].fields');
    let rerenderToggle = false;
    let validationError = null;

    // this helps force a rerender once the imageStack has been reset
    $:if ($processState.currentTab === 2) {
        imageStack = get($processState, 'artStacks[0].fields');
    }

    const getAllImages = function () {
        let allImages = [];
        each(imageStack, function (field) {
            if(!isEmpty(field)) {
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

</script>

{#key rerenderToggle}
        <h1>Specify Image Roles</h1>
<!--        <p>Drag and drop each image into its appropriate role</p>-->
        <Dropbox bind:items={$processState.imageFilePaths} type="image" singleItem={false}/>
        <div class="btnGroup">
            <button class="autoSortButton" on:click={autoSort}>Auto-sort images</button>
        </div>
        <div class="centerFlexBox">
            <div id="imageStack">
                <div class="inputGroup">
                    <div class="imageLetter">A</div>
                    <div class="imageLetter">B</div>
                </div>
                <div class="text">Object</div>
                <div class="inputGroup">
                    <div class="cell"><Dropbox type="image" bind:items={imageStack.imageA} singleItem={true} showError={!!validationError}/></div>
                    <div class="cell"><Dropbox type="image" bind:items={imageStack.imageB} singleItem={true} showError={!!validationError}/></div>
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
        </div>
    <br>
    <br>
    <br>

{/key}

<style lang="postcss">
    panel {
        width: 100%;
        background-color: #3a3a3c;
    }
    right {
        @apply bg-gray-700 w-full h-full p-6 flex justify-center overflow-auto;
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
</style>
