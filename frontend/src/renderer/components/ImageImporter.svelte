<script lang="ts">
    import { FilePlusIcon } from "svelte-feather-icons";
    import { processState, sendMessage} from "@util/stores";
    import { forEach, find } from "lodash";
    import ImageBubble from "@components/Process/ImageBubble.svelte";
    import Dropzone from "svelte-file-dropzone";
    export let label = "Select Files";
    export let icon = FilePlusIcon;
    export let largeText = false;
    let filePaths = [];
    let files = {
        accepted: [],
        rejected: []
    };
    let MAX_FILES = 8

    function getThumbnails() {
        $processState.thumbnailID = Math.floor(Math.random() * 999999999);
        let msg = {
            RequestID: $processState.thumbnailID,
            RequestType: "Thumbnails",
            RequestData: {
                names: filePaths,
            },
        };
        console.log(msg);
        sendMessage(JSON.stringify(msg));
        filePaths = [];
    }

    $: if (filePaths?.length > 0) {
        getThumbnails();
    }

    $: if (
        $processState.imageFilePaths.length >= 6 &&
        !$processState.completedTabs[0]
    ) {
        $processState.completedTabs[0] = true;
    }

    function handleFilesSelect(e) {
        if (files.accepted.length < MAX_FILES){
            files.accepted = [];
            const {acceptedFiles, fileRejections} = e.detail;
            files.accepted = [...files.accepted, ...acceptedFiles];
            files.rejected = [...files.rejected, ...fileRejections];

            forEach(files.accepted, (f) => {
                if (!find(filePaths, {id: f.path, name: f.name})) {
                    filePaths.push({
                        id: f.path,
                        name: f.path
                    });
                }
            });
            $processState.imageFilePaths = [...filePaths];
        }
    }
</script>

<main>
    <br>
    <Dropzone
            on:drop={handleFilesSelect}

            containerStyles="flex: 1;
                            display: flex;
                            flex-direction: column;
                            align-items: center;
                            padding: 20px;
                            border-width: 2px;
                            border-radius: 2px;
                            border-color: #eeeeee;
                            border-style: dashed;
                            background-color: #1D1C1E;
                            color: #bdbdbd;
                            outline: none;
                            transition: border 0.24s ease-in-out;
                            background-color: #1D1C1E;
                            border-radius: 10px;
                            font-size: 30px;
                            text-align: center;"
            disableDefaultStyles
            containerClasses="custom-dropzone">
        <br>
        Click or Drag and Drop Files Here
        <br>
        <br>
        <article>
            <ul>
                {#if $processState.imageFilePaths?.length > 0}
                    {#each $processState.imageFilePaths as filePath}
                        <ImageBubble filename={filePath.name} />
                    {/each}
                {/if}
            </ul>
        </article>
    </Dropzone>
    <br>

</main>

<style lang="postcss">
    main {
        @apply h-full;
    }
    button {
        @apply flex justify-between items-center gap-2 p-0 pl-2 whitespace-nowrap;
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
</style>
