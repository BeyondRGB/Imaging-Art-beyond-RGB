<script lang="ts">
    import TextLogo from "$lib/svgs/TextLogo.svg";
    import About from "$lib/components/About.svelte";
    import { fade } from "svelte/transition";
    import {
        CrosshairIcon,
        ApertureIcon,
        InfoIcon,
        XCircleIcon,
        CopyIcon
    } from "svelte-feather-icons";
    import Modal from "$lib/components/Modal.svelte";

    let showAbout = $state(false);

    //Open a new electron window of the BeyondRGB application using IPC
    const openNewWindow = () => {
        // window.electron.openNewWindow();
    }

    function toggleAbout() {
        showAbout = !showAbout;
    }
</script>

{#if showAbout}
    <Modal bind:open={showAbout}>
        <About />
    </Modal>
{/if}

<div class="homeContent">
    <div class="welcome">
        <h1>Welcome to</h1>
        <img src={TextLogo}  alt="app-logo" />
    </div>
    <div class="buttons">
        <a href="/process" class="homeBtn">
            <div class="buttonTitle">
                <ApertureIcon size="1.25x" />
                <h2>Process</h2>
            </div>

            <span> Process a new RAW image set </span>
        </a>

        <a href="/viewer" class="homeBtn">
            <div class="buttonTitle">
                <CrosshairIcon size="1.25x" />
                <h2>View</h2>
            </div>

            <span> View a previously-processed imaged set </span>
        </a>

        <button on:click={toggleAbout} class="homeBtn">
            <div class="buttonTitle">
                <InfoIcon size="1.25x" />
                <h2>About</h2>
            </div>

            <span> About the program </span>
        </button>
    </div>
</div>

<style lang="postcss">
    @reference "tailwindcss";

    .homeContent {
        @apply flex flex-col items-center justify-between mb-[15vh];
    }

    .homeBtn {
        @apply w-full h-full flex flex-col justify-center items-center p-[2vh] text-lg
        bg-gray-900/25 ring-0 hover:bg-gray-600 text-gray-100 hover:text-white;
    }

    .buttons {
        @apply w-[60vw] flex flex-col justify-center items-center gap-4;
    }

    h1 {
        @apply text-4xl font-bold;
    }

    img {
        /* transform: scale(0.5); */
        pointer-events: none;
        @apply h-[10vh];
    }

    .buttonTitle {
        @apply w-full flex justify-center items-center gap-1 p-1 text-xl;
    }

    .welcome {
        @apply text-gray-100 flex flex-col justify-center
        items-center gap-2 pb-5;
    }

    .welcome h1 {
        @apply opacity-40;
    }

    .closeBtn {
        @apply absolute right-0 m-2 bg-transparent ring-0 hover:bg-red-400/50
        p-1;
    }
</style>