<script lang="ts">
    import { FilePlusIcon } from "svelte-feather-icons";
    import { processState, sendMessage} from "@util/stores";
    import { forEach, find } from "lodash";
    import { testStyle } from "@util/styles";
    import ImageBubble from "@components/Process/ImageBubble.svelte";
    import Dropzone from "svelte-file-dropzone";
    export let label = "Select Files";
    export let icon = FilePlusIcon;
    export let largeText = false;
    export let removeButton = false;
    let filePaths = [];
    let files = {
        accepted: [],
        rejected: []
    };

    function getThumbnails() {
        $processState.thumbnailID = Math.floor(Math.random() * 999999999);
        let msg = {
            RequestID: $processState.thumbnailID,
            RequestType: "Thumbnails",
            RequestData: {
                names: filePaths,
            },
        };
        sendMessage(JSON.stringify(msg));
        filePaths = [];
    }

    $: if (
        $processState.imageFilePaths.length >= 6 &&
        !$processState.completedTabs[0]
    ) {
        $processState.completedTabs[0] = true;
    }

    function handleFilesSelect(e) {
        files.accepted = [];
        if (e.detail == null){
            /* handles files being added from the select files button */
            files.accepted = [...files.accepted, ...e.target.files];
        }
        else {
            /* handles files being added from dragging and dropping */
            files.accepted = [...e.detail?.acceptedFiles];
        }

        forEach(files.accepted, (f) => {
            if (!find($processState.imageFilePaths, {id: f.path, name: f.name})) {
                $processState.imageFilePaths.push({
                    id: f.path,
                    name: f.path
                });
            }
        });
        forEach($processState.imageFilePaths, function (f){
           filePaths.push(f.name);
        });
        getThumbnails();
    }

    const remove = (item) => {
        $processState.imageFilePaths = $processState.imageFilePaths.filter((value) => value.id !== item.id);
    };

    const removeAll = () => {
        $processState.imageFilePaths = [];
    };

    function openAttachment() {
        document.getElementById('attachment').click();
    }


</script>

<main>
    <br>
    <Dropzone
            on:drop={handleFilesSelect}
            noClick
            containerStyles={testStyle}
            disableDefaultStyles
            containerClasses="custom-dropzone">
        {#if $processState.imageFilePaths?.length > 0}
            <div class="two-col">
                <div class="col1">
                    <input type="file" class="file" id="attachment" style="display: none;" on:change={handleFilesSelect} multiple/>
                    <button
                            type="button"
                            class="file"
                            class:largeText
                            id="btnAttachment"
                            on:click={openAttachment}
                    >{label}
                        <div class="icon">
                            <svelte:component this={icon} size="1.5x" />
                        </div>
                    </button>
                </div>
                <div class="col2">
                    <button on:click={removeAll} class="remove-button" class:largeText> Remove All</button>
                </div>
            </div>
        {:else}
            <input type="file" class="file" id="attachment" style="display: none;" on:change={handleFilesSelect} multiple/>
            <button
                    type="button"
                    class="file"
                    class:largeText
                    id="btnAttachment"
                    on:click={openAttachment}
            >{label}
                <div class="icon">
                    <svelte:component this={icon} size="1.5x" />
                </div>
            </button>
        {/if}

        <br>
        Drag and Drop Files Here
        <br>
        <br>
        <article>
            <ul>
                {#if $processState.imageFilePaths?.length > 0}
                    {#each $processState.imageFilePaths as filePath}
                        <ImageBubble filename={filePath.name} on:remove={remove(filePath)}/>
                    {/each}
                {/if}
            </ul>
        </article>
    </Dropzone>
    <br>

</main>

<style lang="postcss">
    main {
        @apply h-full ;
    }
    button {
        @apply flex justify-between items-center gap-2 p-0 pl-2;
    }
    .largeText {
        @apply text-lg;
    }
    .icon {
        @apply bg-gray-500 p-1 group-hover:bg-blue-400 transition-all rounded-r-lg;
    }
    ul {
        @apply flex flex-col gap-2 w-full justify-center items-center;
    }
    article {
        @apply bg-gray-800 w-full min-h-[12rem] max-h-[30rem] overflow-auto rounded-[32px] py-2 px-6;
    }
    button {
        @apply flex justify-between items-center gap-2 p-0 pl-2 whitespace-nowrap;
    }
    .icon {
        @apply bg-gray-500 p-1 group-hover:bg-blue-400 transition-all rounded-r-lg;
    }
    .two-col {
        overflow: hidden;/* Makes this div contain its floats */
        width: 100%
    }

    .two-col .col1,
    .two-col .col2 {
        /*width: 80%;*/
    }

    .two-col .col1 {
        float: left;
        width: 60%;
        padding-left: 40%;
    }

    .two-col .col2 {
        float: right;
        align-items: flex-end;
        vertical-align: center;
    }

    .two-col label {
        display: block;
    }

    .remove-button:hover {
        background-color: red;
    }

</style>
