<script lang="ts">
</script>

<main>
  <left>
    <h1>Import Images</h1>
    <p>
      Select the folder or all images you would like to import into the program
    </p>
  </left>
  <right>
    <input type="file" />
    <article>
      <ul>
        {#each [1, 2, 3, 4, 5] as i}
          <li>{i}</li>
        {/each}
      </ul>
    </article></right
  >
</main>

<style lang="postcss">
  main {
    @apply flex justify-between h-full w-full overflow-hidden;
  }
  left {
    @apply bg-gray-600 w-full h-full p-6 flex-col overflow-auto;
  }
  right {
    @apply bg-gray-700 w-full h-full p-6 flex justify-center;
  }
  h1 {
    @apply text-3xl;
  }
  p {
    @apply text-center pt-[30vh] bg-gray-500/25 m-4 h-[90%] rounded-lg;
  }
  input {
    @apply pt-[35vh];
  }
  article {
    @apply pt-[40vh] bg-gray-800 h-60 w-[80%] hidden;
  }
</style>
