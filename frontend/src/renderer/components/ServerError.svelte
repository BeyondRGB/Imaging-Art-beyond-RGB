<script lang="ts">
  import {
    serverError,
    resendMessage,
    messageStore,
    processState,
  } from "@util/stores";
  import { AlertCircleIcon } from "svelte-feather-icons";

  export let closeModal;

  function handleClose() {
    console.log("Close");
    $serverError = null;
    $messageStore = [];
    closeModal();
  }

  /**
   * When a server error occurs, go back to the most recent tab and 
   * reset fields in the processState
   */
  function returnToSetup() {
    for (let i = 0; i < $processState.completedTabs.length - 1; i++) {
      if ($processState.completedTabs[i+1] === false || i === $processState.completedTabs.length - 2) {
        $processState.currentTab = i;
        $processState.completedTabs[i] = false;
        break;
      }
    }
    $processState.whitePatchFilled = false;
    $processState.returnedFromProcessing = true;
    if ($processState.artStacks[0].verificationTarget === null) {
      $processState.artStacks[0].verificationTarget = {};
    }
  }
</script>

<main>
  <h2 class="title">
    <AlertCircleIcon size="1x" />
    <p>Error</p>
  </h2>
  <div class="body">
    <div class="info">
      <span>Encountered Server Error</span>
      <div class="err">
        <div class="sender">
          Sender: {$serverError?.sender}
        </div>
        <div class="msg">
          <p>Message:</p>
          {$serverError?.message}
        </div>
      </div>
    </div>
    <div class="btns">
      <button
        class="retry"
        on:click={() => {
          resendMessage();
          handleClose();
        }}>Retry</button
      >
      <button
        class="reset"
        on:click={() => {
          returnToSetup();
          handleClose();
        }}>Return to Setup</button
      >
    </div>
  </div>
</main>

<style lang="postcss">
  main {
    @apply max-w-[50%] min-w-[35%] max-h-[50%] bg-gray-700 rounded-2xl shadow-xl overflow-auto flex flex-col;
  }
  .body {
    @apply h-full flex flex-col justify-between p-1;
  }
  .title {
    @apply bg-gray-800 text-2xl p-2 flex items-center gap-2 text-red-600;
  }
  .title p {
    @apply text-2xl flex text-gray-100;
  }
  .info {
    @apply flex flex-col p-2;
  }
  .info span {
    @apply text-lg p-1 flex flex-col;
  }
  .err {
    @apply p-2;
  }
  .sender {
    @apply bg-gray-500 p-1;
  }
  .msg {
    @apply flex flex-col p-2 bg-gray-600;
  }
  .btns {
    @apply flex justify-end gap-2 p-1;
  }
  .btns button {
    @apply p-2 text-lg;
  }
  .reset {
    @apply bg-blue-600;
  }
</style>
