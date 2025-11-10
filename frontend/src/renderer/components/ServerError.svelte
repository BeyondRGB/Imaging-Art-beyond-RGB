<script lang="ts">
  import {
    serverError,
    resendMessage,
    messageStore,
    processState,
  } from "@util/stores";
  import { AlertCircleIcon } from "svelte-feather-icons";
  import ScrollContainer from "@components/ScrollContainer.svelte";

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
    let targetTab = 0;
    for (let i = 0; i < $processState.completedTabs.length - 1; i++) {
      if ($processState.completedTabs[i+1] === false || i === $processState.completedTabs.length - 2) {
        targetTab = i;
        break;
      }
    }
    const finalTab = targetTab;
    processState.update(state => ({
      ...state,
      currentTab: finalTab,
      completedTabs: state.completedTabs.map((completed, i) => i === finalTab ? false : completed),
      whitePatchFilled: false,
      returnedFromProcessing: true,
      artStacks: state.artStacks.map((stack, i) => 
        i === 0 ? {
          ...stack,
          verificationTarget: stack.verificationTarget === null ? {} : stack.verificationTarget
        } : stack
      )
    }));
  }
</script>

<ScrollContainer maxHeight="50vh" className="main-container">
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
</ScrollContainer>

<style lang="postcss">
  :global(.main-container) {
    background-color: var(--color-surface);
    @apply max-w-[50%] min-w-[35%] rounded-2xl shadow-xl flex flex-col;
  }
  
  main {
    @apply flex flex-col;
  }
  .body {
    @apply h-full flex flex-col justify-between p-1;
  }
  .title {
    background-color: var(--color-surface-base);
    @apply text-2xl p-2 flex items-center gap-2 text-red-600;
  }
  .title p {
    color: var(--color-text-primary);
    @apply text-2xl flex;
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
    background-color: var(--color-surface-sunken);
    @apply p-1;
  }
  .msg {
    background-color: var(--color-surface-elevated);
    @apply flex flex-col p-2;
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
