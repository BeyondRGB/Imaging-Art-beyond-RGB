<script lang="ts">
    import { processState } from "@util/stores";
    import Dropbox from "@components/Process/Dropbox.svelte";
    import {get, isEmpty, each, includes} from "lodash";
    import { autoSortImages } from "@util/autoSortStandards.svelte";

    let imageStack = get($processState, 'artStacks[0].fields');
    let rerenderToggle = false;

    // this helps force a rerender once the imageStack has been reset
    $:if ($processState.currentTab === 0) {
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

    const resetImages = function () {
        each($processState.artStacks[0].fields, function (image) {
            $processState.imageFilePaths.push({name: image[0].name, id: image[0].id});
        });
        $processState.artStacks[0].fields = {
            imageA: [],
            imageB: [],
            targetA: [],
            targetB: [],
            flatfieldA: [],
            flatfieldB: [],
            darkfieldA: [],
            darkfieldB: [],
        };
        rerenderToggle = !rerenderToggle;
    };

</script>

{#key rerenderToggle}
    <Dropbox min bind:items={$processState.imageFilePaths} type="image" singleItem={false}/>
    <div class="btnGroup">
        {#if $processState.imageFilePaths.length !== 0}
            <button class="autoSortButton" on:click={autoSort}>Auto-sort images</button>
        {:else}
            <button class="autoSortButton" on:click={resetImages}>Reset images</button>
        {/if}

    </div>
    <div class="centerFlexBox">
        <div id="imageStack">
            <div class="inputGroup">
                <div class="imageLetter">A</div>
                <div class="imageLetter">B</div>
            </div>
            <div class="text">Object</div>
            <div class="inputGroup">
                <div class="cell"><Dropbox type="image" bind:items={imageStack.imageA} singleItem={true}/></div>
                <div class="cell"><Dropbox type="image" bind:items={imageStack.imageB} singleItem={true}/></div>
            </div>
            {#if $processState.imageFilePaths && showTargetDropZones()}
                <div class="text">Target</div>
                <div class="inputGroup">
                    <div class="cell"><Dropbox type="image" bind:items={imageStack.targetA} singleItem={true}/></div>
                    <div class="cell"><Dropbox type="image" bind:items={imageStack.targetB} singleItem={true}/></div>
                </div>
            {/if}
            <div class="text">FlatField</div>
            <div class="inputGroup">
                <div class="cell"><Dropbox type="image" bind:items={imageStack.flatfieldA} singleItem={true}/></div>
                <div class="cell"><Dropbox type="image" bind:items={imageStack.flatfieldB} singleItem={true}/></div>
            </div>
            <div class="text">DarkField</div>
            <div class="inputGroup">
                <div class="cell"><Dropbox type="image" bind:items={imageStack.darkfieldA} singleItem={true}/></div>
                <div class="cell"><Dropbox type="image" bind:items={imageStack.darkfieldB} singleItem={true}/></div>
            </div>
            <br>
        </div>
    </div>
    <br>
    <br>
    <br>
{/key}

<style lang="postcss">
    h1 {
        font-size: 35px;
        width: 100%;
    }
    p {
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
        overflow: scroll;
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
        align-self: auto;
    }
    .btnGroup {
        align-content: center;
    }
</style>
