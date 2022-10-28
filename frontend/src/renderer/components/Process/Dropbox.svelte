
<script>
    import {dndzone} from 'svelte-dnd-action';
    import {flip} from 'svelte/animate';
    import ImageBubble from "@components/Process/ImageBubble.svelte";
    import { isEmpty } from "lodash";
    const flipDurationMs = 200;

    export let items = [];
    export let type;
    export let singleItem = true;
    export let showError = false;

    function handleSort(e) {
        items = e.detail.items;
    }

</script>
<main>
    <div class="sectionStyle {showError && singleItem && isEmpty(items) ? 'errorStyle' : ''}">
        <section use:dndzone={{items, flipDurationMs, type, dropFromOthersDisabled: singleItem && items.length > 0}}
            on:consider={handleSort}
            on:finalize={handleSort}
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
