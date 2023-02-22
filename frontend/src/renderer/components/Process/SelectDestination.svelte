<script lang="ts">
    import FileSelector from "@components/FileSelector.svelte";
    import { FolderPlusIcon, FolderIcon } from "svelte-feather-icons";
    import { currentPage, processState } from "@util/stores";
    let filePaths = [];
    $: console.log(filePaths);

    $: if (filePaths) {
        $processState.destDir = filePaths[0];
    }

</script>


<div class="selectBox">
    <h1>Select Destination</h1>
    <div class="fileSelector">
        <FileSelector
                bind:filePaths
                type="Dir"
                label="Select Folder"
                icon={FolderPlusIcon}
                largeText
        />
    </div>
    {#if filePaths?.length > 0}
        <div class="folderDisp">
            <div class="destLabel">
                <FolderIcon size="3x" />
                Destination Folder:
            </div>
            <div class="folderLoc">
                {$processState.destDir}
            </div>
        </div>
    {/if}
</div>


<style lang="postcss">
    p {
        @apply text-center pt-[30vh] bg-gray-500/25 m-6 h-[90%] rounded-lg;
    }
    h1 {
        font-size: 35px;
        width: 100%;
    }
    .folderDisp {
        @apply flex justify-between m-1 text-base;
    }
    .destLabel {
        @apply bg-blue-500/50 flex justify-between items-center p-1 rounded-l-xl;
    }
    .folderLoc {
        @apply bg-gray-800 flex items-center justify-center rounded-r-xl p-1;
    }
    .selectBox {
        @apply h-[80%] flex flex-col items-center gap-2;
        padding-top: 5vh;
    }
</style>
