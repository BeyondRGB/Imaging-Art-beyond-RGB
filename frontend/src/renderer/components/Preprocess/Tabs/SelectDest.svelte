<script lang="ts">
</script>

<main>
  <left>
    <h1>Select Destination Files Location</h1>
    <p>
      Select the folder where you would like to save the spectral images after
      processing
    </p>
  </left>
  <right> <input type="file" /> </right>
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
