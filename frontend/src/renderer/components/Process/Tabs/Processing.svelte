<script>
  import { sendMessage, messageStore, processState } from "@util/stores";
  import { stringify } from "postcss";

  let textValue;
  let messageList = [];
  $: messageList = [...messageList, $messageStore];
  $: console.log($processState.artStacks[0].colorTarget);
  $: jsonTest = {
    RequestType: "processImg",
    RequestID: Date.now(),
    RequestData: {
      Images: [
        {
          Art: $processState.artStacks[0].fields.images[0]?.name,
          White: $processState.artStacks[0].fields.whitefield[0]?.name,
          Dark: $processState.artStacks[0].fields.darkfield[0]?.name,
        },
        {
          Art: $processState.artStacks[0].fields.images[1]?.name,
          White: $processState.artStacks[0].fields.whitefield[1]?.name,
          Dark: $processState.artStacks[0].fields.darkfield[1]?.name,
        },
      ],
      TargetLocation: $processState.artStacks[0].colorTarget,
      RefData: {
        RefData: "NGT_Reflectance_Data.csv",
        StandardObserver: 1931,
        Illuminants: "D50",
      },
    },
  };
</script>

<main>
  <state>
    <h3>Current State:</h3>
    <p>RequestType: "processImg",</p>
    <p>RequestID: {jsonTest.RequestID}</p>
    <p>RequestData:</p>
    <p>
      Images:

      {#each jsonTest.RequestData.Images as image, index}
        <p>Image {String.fromCharCode(65 + index)}</p>
        {#each Object.keys(image) as key}
          <li>
            {key}:
            <span>{image[key]}</span>
          </li>
        {/each}
        <br />
      {/each}
    </p>
    <p>
      Target Loation:

      {#each Object.keys(jsonTest.RequestData.TargetLocation) as key}
        <li>{key}: {jsonTest.RequestData.TargetLocation[key]}</li>
      {/each}
    </p>
    <p>
      Reference Data:

      {#each Object.keys(jsonTest.RequestData.RefData) as key}
        <li>{key}: {jsonTest.RequestData.RefData[key]}</li>
      {/each}
    </p>
    <button on:click={() => sendMessage(JSON.stringify(jsonTest))}
      >Send to Server</button
    >
  </state>
  <article>
    <textarea placeholder="Enter command here" bind:value={textValue} />
    <button on:click={() => sendMessage(JSON.stringify(textValue))}
      >Send It!</button
    >
    <h4>Server Response</h4>
    {#each messageList as message}
      <message>({new Date().toLocaleTimeString()}) > {message}</message><br />
    {/each}
  </article>
</main>

<style lang="postcss">
  main {
    @apply w-full h-full flex justify-center items-center;
  }
  state {
    @apply w-full h-full list-disc p-4;
  }
  article {
    @apply bg-gray-900 w-full h-[70%] m-4;
  }
  h4 {
    @apply bg-gray-700/25 text-gray-400;
  }
  span {
    @apply bg-gray-700 text-gray-300 whitespace-nowrap select-text;
  }
  h3 {
    @apply text-lg;
  }
  textarea {
    @apply bg-gray-500/25 w-full text-white border-2 border-blue-500;
  }
</style>
