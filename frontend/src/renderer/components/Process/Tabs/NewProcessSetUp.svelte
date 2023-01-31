<script lang="ts">
    import {
        messageStore,
        processState,
        sendMessage,
        viewState,
        serverError,
    } from "@util/stores";
    // Import Images
    import ImageImporter from "@components/ImageImporter.svelte";
    // Output Destination
    import SelectDestination from "@components/Process/SelectDestination.svelte";
    import FileRoles from "@components/Process/FileRoles.svelte";
</script>

<main>
    <left>
        <h1>Import Images</h1>
        <ImageImporter/>
        <SelectDestination/>
    </left>
    <right>
        <FileRoles/>
    </right>
</main>

<style lang="postcss">
    main {
        @apply flex justify-between h-full w-full overflow-hidden;
    }
    left {
        @apply bg-gray-600 w-full h-full p-6 flex-col overflow-auto;
    }
    right {
        @apply bg-gray-700 w-full h-full p-6 flex flex-col justify-center items-center gap-4;
    }
    h1 {
        @apply text-3xl;
    }
    p {
        @apply text-center pt-[30vh] bg-gray-500/25 m-4 h-[90%] rounded-lg;
    }
    article {
        @apply bg-gray-800 w-full min-h-[12rem] overflow-auto rounded-[32px] py-2 px-6;
    }
    ul {
        @apply flex flex-col gap-2 w-full justify-center items-center;
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
        @apply h-[70%] flex flex-col justify-center items-center gap-2;
    }
</style>