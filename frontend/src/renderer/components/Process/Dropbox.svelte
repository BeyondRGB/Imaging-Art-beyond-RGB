
<script>
    import {dndzone} from 'svelte-dnd-action';
    import {flip} from 'svelte/animate';
    import ImageBubble from "@components/Process/ImageBubble.svelte";
    import { isEmpty } from "lodash";
    import {drag} from "d3";
    const flipDurationMs = 200;

    // String ID representing the dropbox.
    export let id;

    export let items = [];
    export let type;
    export let singleItem = true;
    export let showError = false;
    export let dragDisabled = false;

    export let dragMonitor = undefined;
    export let dropFunction = undefined;

    $: console.log(JSON.stringify(items))

    function handleConsider(e) {
        items = e.detail.items

        if (dragMonitor) {
            dragMonitor(e, id)
        }
    }

    function handleFinalize(e) {
        items = e.detail.items

        if (dropFunction) {
            dropFunction(e, id)
        }
    }

</script>
<main>
<!--    dropFromOthersDisabled: singleItem && items.length > 0 -->
    <div class="sectionStyle {showError && singleItem && isEmpty(items) ? 'errorStyle' : ''}">
        <section use:dndzone={{items, flipDurationMs, type, dragDisabled}}
            on:consider={handleConsider}
            on:finalize={handleFinalize}
        >
            {#each items as item (item?.id)}
                <card animate:flip={{ duration: flipDurationMs }} class="{singleItem ? 'verified' : ''}">
                    <ImageBubble filename={item.name} minimal />
                </card>
            {/each}
        </section>
    </div>
</main>

<style>
    section {
        min-height: 3.25em;
        margin: auto;
        border-radius: 10px;
        justify-content: center;
        display: flex;
        flex-wrap: wrap;
        gap: 10px;
        align-items: center;
        padding: 10px;
    }
    .sectionStyle {
        background-color: #2c2c2f;
        height: auto;
        width: 80%;
        border-radius: 10px;
        margin: auto;
    }
    .errorStyle {
        background-color: red;
    }
    card {
        background-color: gray;
        width: auto;
        padding-right: 10px;
        padding-left: 10px;
        height: 1.8em;
        border-radius: 10px;
        color: white;
        text-align: center;
    }
    .verified {
        @apply bg-green-400 text-black dark:bg-green-800 dark:text-white;
    }
</style>
