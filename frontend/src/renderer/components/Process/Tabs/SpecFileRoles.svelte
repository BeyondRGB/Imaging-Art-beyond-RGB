<script lang="ts">
    import { processState } from "@util/stores";
    import Dropbox from "@components/Process/Dropbox.svelte";
    import {get, isEmpty, each, includes} from "lodash";
    import { autoSortImages } from "@util/autoSortStandards.svelte";
    import { TRIGGERS } from "svelte-dnd-action";
    import { ImageField } from "@util/ImageField";

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

    let lastDragLocation = null;

    function getList(location) {
        const mapping = {
            [ImageField.ROLES]: $processState.imageFilePaths,
            [ImageField.IMAGE_A]: imageStack.imageA[0],
            [ImageField.IMAGE_B]: imageStack.imageB[0],
            [ImageField.TARGET_A]: imageStack.targetA,
            [ImageField.TARGET_B]: imageStack.targetB,
            [ImageField.FLAT_A]: imageStack.flatfieldA,
            [ImageField.FLAT_B]: imageStack.flatfieldB,
            [ImageField.DARK_A]: imageStack.darkfieldA,
            [ImageField.DARK_B]: imageStack.darkfieldB
        };
        return mapping[location];
    }

    function moveItem(fromLocation, toLocation) {
        const sourceList = getList(fromLocation);

        if (sourceList === undefined) {
            console.log("Source list is undefined");
            return;
        }

        // Remove the item from the source list.
        const item = sourceList.shift();

        if (!item) {
            console.log("Item is undefined");
            return;
        }

        // Special processing is needed for the roles list, since it uses the imageFilePaths array.
        if (fromLocation === ImageField.ROLES) {
            // Force Svelte reactivity by reassigning the imageFilePaths.
            $processState.imageFilePaths = sourceList;
        } else {
            // Force Svelte reactivity by reassigning the imageStack
            imageStack = {
                ...imageStack,
                // Modify the targetList to remove the item. This uses a JavaScript modification syntax that looks up a field with a name.
                [fromLocation]: sourceList
            };
        }

        // Special processing is needed for the roles list, since it uses the imageFilePaths array.
        if (toLocation === ImageField.ROLES) {
            // Force Svelte reactivity by reassigning the imageFilePaths.
            $processState.imageFilePaths = [...$processState.imageFilePaths, item];
        } else {
            // Get the list we are moving items too
            const targetList = getList(toLocation);

            if (sourceList === undefined) {
                console.log("Source list is undefined");
                return;
            }

            // Add the item to the target list.
            targetList.push(item);

            // Force Svelte reactivity by reassigning the imageStack.
            imageStack = {
                ...imageStack,
                // Modify the targetList to include the item. This uses a JavaScript modification syntax that looks up a field with a name.
                [toLocation]: targetList
            };
        }
    }

    const dropFunction = function (event, location) {
        const trigger = event.detail.info.trigger;

        // When we receive a trigger that tells us an item was dropped into this zone.
        if (trigger === TRIGGERS.DROPPED_INTO_ZONE) {
            const list = getList(location);

            if (list === undefined) {
                console.log("List is undefined");
                return;
            }

            // If the list has more than one item, move the last item to the list that the new item came from.
            if (list.length > 1 && lastDragLocation) {
                moveItem(location, lastDragLocation);
            }
        }
    }

    const dragMonitor = function (event, location) {
        const trigger = event.detail.info.trigger;

        if (trigger === TRIGGERS.DRAG_STARTED) {
            // If the drag started, record the last drag location.
            lastDragLocation = location;
        } else if ([TRIGGERS.DROPPED_INTO_ANOTHER, TRIGGERS.DROPPED_INTO_ZONE, TRIGGERS.DROPPED_OUTSIDE_OF_ANY].includes(trigger)) {
            // If any of the ending drag triggers have been sent, reset the last drag location.
            lastDragLocation = null;
        }
    }
</script>

<main>
    {#key rerenderToggle}
        <panel>
            <h1>Specify Image Roles</h1>
            <p>Drag and drop each image into its appropriate role</p>
            <div>
            <Dropbox id={ImageField.ROLES} bind:items={$processState.imageFilePaths} type="image" singleItem={false} dragMonitor={dragMonitor}/>
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
                        <div class="cell"><Dropbox id={ImageField.IMAGE_A} type="image" bind:items={imageStack.imageA[0]} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                        <div class="cell"><Dropbox id={ImageField.IMAGE_B} type="image" bind:items={imageStack.imageB[0]} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                    </div>
                    {#if $processState.imageFilePaths && showTargetDropZones()}
                        <div class="text">Target</div>
                        <div class="inputGroup">
                            <div class="cell"><Dropbox id={ImageField.TARGET_B} type="image" bind:items={imageStack.targetA} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                            <div class="cell"><Dropbox id={ImageField.TARGET_A} type="image" bind:items={imageStack.targetB} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                        </div>
                    {/if}
                    <div class="text">FlatField</div>
                    <div class="inputGroup">
                        <div class="cell"><Dropbox id={ImageField.FLAT_A} type="image" bind:items={imageStack.flatfieldA} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                        <div class="cell"><Dropbox id={ImageField.FLAT_B} type="image" bind:items={imageStack.flatfieldB} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                    </div>
                    <div class="text">DarkField</div>
                    <div class="inputGroup">
                        <div class="cell"><Dropbox id={ImageField.DARK_A} type="image" bind:items={imageStack.darkfieldA} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                        <div class="cell"><Dropbox id={ImageField.DARK_B} type="image" bind:items={imageStack.darkfieldB} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
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
    .btnGroup {
        @apply flex justify-end gap-2;
    }
    .nextBtn {
        @apply m-4 bg-green-700 hover:bg-green-600 focus:ring-green-600 transition-all;
    }
</style>
