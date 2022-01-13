<script lang="ts">
</script>

<main>
  <left>
    <h1>Advanced Options</h1>
    <p>Warning: Only select Advanced Options if you know what you are doing!</p>
  </left>
  <right> [PLACEHOLDER] </right>
</main>

<style lang="postcss">
  main {
    @apply flex justify-between h-full w-full overflow-hidden;
  }
  left {
    @apply bg-gray-600 w-full h-full p-6 flex-col;
  }
  right {
    @apply bg-gray-700 w-full h-full p-6 flex justify-center;
  }
  h1 {
    @apply text-3xl;
  }
  p {
    @apply text-center pt-[30vh] bg-gray-500/25 m-6 h-[90%] rounded-lg;
  }
  input {
    @apply translate-y-[35vh];
  }
</style>
