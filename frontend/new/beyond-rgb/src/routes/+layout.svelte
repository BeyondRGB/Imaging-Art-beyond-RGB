<script>
    import "../app.css";
    
    let theme = "dark";

    document.documentElement.classList.add(theme);
    document.body.classList.add(theme);
</script>

<svelte:head>
    <link rel="icon" href="/favicon.png" />
</svelte:head>

<!-- Add sidebar here -->
<main>
<!--    class:sideMenu={$appSettings.sideNav} -->
    <div class="app">
        <slot></slot>
    </div>
</main>

<style global lang="postcss">
    @reference "tailwindcss";

    :global(html), :global(body) {
        width: 100%;
        height: 100%;
        margin: 0 auto;

        @apply bg-gray-800 dark:border-gray-600 border-red-600 select-none dark:text-gray-50 text-base;
    }

    main {
        height: 100%;
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto,
        Helvetica, Arial, sans-serif;
    }

    .app {
        width: 100%;
        height: 100%;
        margin: 0 auto;

        @apply flex flex-col-reverse relative overflow-hidden justify-center items-center;
    }

    /* width */
    ::-webkit-scrollbar {
        @apply relative transition-all w-1;
    }

    /* Track */
    ::-webkit-scrollbar-track {
        @apply bg-transparent;
    }

    /* Handle */
    ::-webkit-scrollbar-thumb {
        @apply bg-gray-600 rounded-full;
    }

    /* Handle on hover */
    ::-webkit-scrollbar-thumb:hover {
        @apply bg-gray-500;
    }
</style>

