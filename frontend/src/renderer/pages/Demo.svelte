<script lang="ts">
  import { sendMessage, messageStore } from "@util/stores";
  import { connectionState } from "@util/stores";

  let messageList: String[] = [];
  $: messageList = [...messageList, $messageStore];
  let files;
</script>

<main>
  <status
    class={$connectionState !== "Connected" ? "text-red-500" : "text-green-500"}
    >{$connectionState}</status
  >
  <h1>Demo Backend Connection</h1>
  <!-- <div id="fileBlock">
    <div><p>Select Tiff File</p></div>
    <div><input id="fileSelect" type="file" bind:files actcept="*" /></div>
  </div> -->
  <div id="io">
    <article>
      <ul>
        {#each messageList as message}
          <li>{message}</li>
        {/each}
      </ul>
    </article>
    <button on:click={() => sendMessage("processImg")}>Process</button>
  </div>
</main>

<style lang="postcss">
  main {
    @apply flex flex-col items-center pt-6 w-[80%] self-center overflow-auto;
  }
  h1 {
    @apply text-3xl font-semibold;
  }
  #fileBlock {
    justify-content: space-between;
    @apply flex justify-between w-[80%] pt-6;
  }
  status {
    @apply ml-auto font-semibold;
  }
  #io {
    @apply flex w-[60%] pt-6;
  }
  button {
    @apply ml-auto;
  }
  #image {
    @apply h-[80vh] ring-1 ring-gray-800 w-full;
  }
  article {
    @apply h-60 bg-gray-800 overflow-auto flex flex-col-reverse mx-2 my-1 w-full;
  }
</style>
