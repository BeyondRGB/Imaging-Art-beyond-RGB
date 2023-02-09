<script lang="ts">
    import ImageImporter from "@components/ImageImporter.svelte";
    import SelectDestination from "@components/Process/SelectDestination.svelte";
    import FileRoles from "@components/Process/FileRoles.svelte";

</script>

<main>
    <left>
        <h1>Import Images</h1>
        <ImageImporter/>
        <br>
        <br>
        <br>
        <br>
        <br>
        <SelectDestination/>
    </left>
    <right>
        <h1>Specify Image Roles</h1>
        <p>Drag and drop each image into its appropriate role</p>
        <FileRoles/>
    </right>

</main>

<style lang="postcss">
    main {
        @apply flex justify-between h-full w-full overflow-hidden;
    }
    left {
        @apply bg-gray-600 w-full h-full p-6 flex-col overflow-hidden;
    }
    right {
        @apply bg-gray-700 w-full p-6 flex flex-col justify-center items-center gap-4 overflow-auto;
    }
    h1 {
        font-size: 35px;
        width: 100%;
    }
    p {
        font-size: 18px;
        width: 100%;
    }
    article {
        @apply bg-gray-800 w-full min-h-[12rem] overflow-auto rounded-[32px] py-2 px-6;
    }
</style>