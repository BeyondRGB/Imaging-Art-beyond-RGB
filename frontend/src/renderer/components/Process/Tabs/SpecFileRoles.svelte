<script lang="ts">
    import { processState } from "@util/stores";
    import Dropbox from "@components/Process/Dropbox.svelte";
    import {get, isEmpty, each, includes} from "lodash";
    import { autoSortImages } from "@util/autoSortStandards.svelte";
    import { TRIGGERS } from "svelte-dnd-action";

    enum FileRoles {
        ROLES = "roles",
        OBJECT_A = "objectA",
        OBJECT_B = "objectB",
        TARGET_A = "targetA",
        TARGET_B = "targetB",
        FLAT_A = "flatA",
        FLAT_B = "flatB",
        DARK_A = "darkA",
        DARK_B = "darkB",
    }

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

    function listForLocation(location) {
        switch (location) {
            case FileRoles.ROLES:
                return $processState.imageFilePaths;
            case FileRoles.OBJECT_A:
                return imageStack.imageA[0];
            case FileRoles.OBJECT_B:
                return imageStack.imageB[0];
            case FileRoles.TARGET_A:
                return imageStack.targetA;
            case FileRoles.TARGET_B:
                return imageStack.targetB;
            case FileRoles.FLAT_A:
                return imageStack.flatfieldA;
            case FileRoles.FLAT_B:
                return imageStack.flatfieldB;
            case FileRoles.DARK_A:
                return imageStack.darkfieldA;
            case FileRoles.DARK_B:
                return imageStack.darkfieldB;
        }
    }


    function appendToList(item, location) {
        switch (location) {
            case FileRoles.ROLES:
                $processState.imageFilePaths.push(item);
                break;
            case FileRoles.OBJECT_A:
                imageStack.imageA[0].push(item);
                break;
            case FileRoles.OBJECT_B:
                imageStack.imageB[0].push(item);
                break;
            case FileRoles.TARGET_A:
                imageStack.targetA.push(item);
                break;
            case FileRoles.TARGET_B:
                imageStack.targetB.push(item);
                break;
            case FileRoles.FLAT_A:
                imageStack.flatfieldA.push(item);
                break;
            case FileRoles.FLAT_B:
                imageStack.flatfieldB.push(item);
                break;
            case FileRoles.DARK_A:
                imageStack.darkfieldA.push(item);
                break;
            case FileRoles.DARK_B:
                imageStack.darkfieldB.push(item);
                break;
        }
    }

    function moveFirstElement(fromLocation, toLocation) {
        let firstItem = null;

        switch (fromLocation) {
            case FileRoles.ROLES:
                console.log("Moving from roles");
                firstItem = $processState.imageFilePaths.shift();
                break;
            case FileRoles.OBJECT_A:
                console.log("Moving from objectA");
                firstItem = imageStack.imageA[0].shift();
                break;
            case FileRoles.OBJECT_B:
                console.log("Moving from objectB");
                firstItem = imageStack.imageB[0].shift();
                break;
            case FileRoles.TARGET_A:
                console.log("Moving from targetA");
                firstItem = imageStack.targetA[0].shift();
                break;
            case FileRoles.TARGET_B:
                console.log("Moving from targetB");
                firstItem = imageStack.targetB[0].shift();
                break;
            case FileRoles.FLAT_A:
                console.log("Moving from flatA");
                firstItem = imageStack.flatfieldA[0].shift();
                break;
            case FileRoles.FLAT_B:
                console.log("Moving from flatB");
                firstItem = imageStack.flatfieldB[0].shift();
                break;
            case FileRoles.DARK_A:
                console.log("Moving from darkA");
                firstItem = imageStack.darkfieldA[0].shift();
                break;
            case FileRoles.DARK_B:
                console.log("Moving from darkB");
                firstItem = imageStack.darkfieldB[0].shift();
                break;
        }

        if (firstItem === null) {
            console.log("No item to move");
            return;
        }

        switch (toLocation) {
            case FileRoles.ROLES:
                $processState.imageFilePaths.push(firstItem);
                console.log("Moving to roles");
                break;
            case FileRoles.OBJECT_A:
                imageStack.imageA[0].push(firstItem);
                console.log("Moving to objectA");
                break;
            case FileRoles.OBJECT_B:
                imageStack.imageB[0].push(firstItem);
                console.log("Moving to objectB");
                break;
            case FileRoles.TARGET_A:
                imageStack.targetA.push(firstItem);
                console.log("Moving to targetA");
                break;
            case FileRoles.TARGET_B:
                imageStack.targetB.push(firstItem);
                console.log("Moving to targetB");
                break;
            case FileRoles.FLAT_A:
                imageStack.flatfieldA.push(firstItem);
                console.log("Moving to flatA");
                break;
            case FileRoles.FLAT_B:
                imageStack.flatfieldB.push(firstItem);
                console.log("Moving to flatB");
                break;
            case FileRoles.DARK_A:
                imageStack.darkfieldA.push(firstItem);
                console.log("Moving to darkA");
                break;
            case FileRoles.DARK_B:
                imageStack.darkfieldB.push(firstItem);
                console.log("Moving to darkB");
                break;
        }
    }

    const dropFunction = function (event, location) {
        console.log(`Drop: ${event.detail.info.trigger}`)

        if (event.detail.info.trigger == TRIGGERS.DROPPED_INTO_ZONE) {
            // The event we are looking at, is a drop into this zone. Check if the list has more items than needed. If so, move that item into lastDragSource
            console.log(`Dropped into ${location}`)

            let list = listForLocation(location);
            console.log(`List: ${JSON.stringify(list)}`)
            if (list.length > 1) {
                console.log(`Moving ${JSON.stringify(list[0])} from ${location} to ${lastDragLocation}`)
                moveFirstElement(location, lastDragLocation);
            }
        }
    }

    const dragMonitor = function (event, location) {
        console.log(`Monitor: ${event.detail.info.trigger}`)

        const trigger = event.detail.info.trigger;
        if (trigger == TRIGGERS.DRAG_STARTED) {
            console.log(`Drag started from ${location}`)
            lastDragLocation = location;
        } else if (trigger == TRIGGERS.DROPPED_INTO_ANOTHER || trigger == TRIGGERS.DROPPED_INTO_ZONE || trigger == TRIGGERS.DROPPED_OUTSIDE_OF_ANY) {
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
            <Dropbox id={FileRoles.ROLES} bind:items={$processState.imageFilePaths} type="image" singleItem={false} dragMonitor={dragMonitor}/>
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
                        <div class="cell"><Dropbox id={FileRoles.OBJECT_A} type="image" bind:items={imageStack.imageA[0]} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                        <div class="cell"><Dropbox id={FileRoles.OBJECT_B} type="image" bind:items={imageStack.imageB[0]} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                    </div>
                    {#if $processState.imageFilePaths && showTargetDropZones()}
                        <div class="text">Target</div>
                        <div class="inputGroup">
                            <div class="cell"><Dropbox id={FileRoles.TARGET_B} type="image" bind:items={imageStack.targetA} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                            <div class="cell"><Dropbox id={FileRoles.TARGET_A} type="image" bind:items={imageStack.targetB} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                        </div>
                    {/if}
                    <div class="text">FlatField</div>
                    <div class="inputGroup">
                        <div class="cell"><Dropbox id={FileRoles.FLAT_A} type="image" bind:items={imageStack.flatfieldA} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                        <div class="cell"><Dropbox id={FileRoles.FLAT_B} type="image" bind:items={imageStack.flatfieldB} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                    </div>
                    <div class="text">DarkField</div>
                    <div class="inputGroup">
                        <div class="cell"><Dropbox id={FileRoles.DARK_A} type="image" bind:items={imageStack.darkfieldA} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
                        <div class="cell"><Dropbox id={FileRoles.DARK_B} type="image" bind:items={imageStack.darkfieldB} singleItem={true} showError={!!validationError} dropFunction={dropFunction} dragMonitor={dragMonitor}/></div>
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
