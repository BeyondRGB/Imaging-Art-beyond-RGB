<script lang="ts">
  import { sendMessage, messageStore, processState } from "@util/stores";
  import { stringify } from "postcss";

  let textValue;
  let messageList = [];
  $: messageList = [
    [$messageStore[0], getTime($messageStore[1])],
    ...messageList,
  ];

  $: console.log(messageList);
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

  function getTime(time = new Date()) {
    let minutes = time.getMinutes();
    let seconds = time.getSeconds();
    let mili = time.getMilliseconds();
    let hours = time.getHours();
    let ofDay = "AM";
    if (hours > 12) {
      hours -= 12;
      ofDay = "PM";
    }
    return `${hours < 10 ? "0" : ""}${hours}:${
      minutes < 10 ? "0" : ""
    }${minutes}:${seconds < 10 ? "0" : ""}${seconds}:${
      mili < 100 ? "00" : ""
    }${mili} ${ofDay}`;
  }
</script>

<main>
  <div class="state">
    <h3>Current State:</h3>
    <div class="box">
      <p>RequestType: "processImg",</p>
      <p>RequestID: {jsonTest.RequestID}</p>
      <p>RequestData:</p>
      <p class="px-2">
        Images:

        {#each jsonTest.RequestData.Images as image, index}
          <li>
            Image {String.fromCharCode(65 + index)}
            {#each Object.keys(image) as key}
              <li>
                {key}:
                <span>{image[key]}</span>
              </li>
            {/each}
            <br />
          </li>
        {/each}
      </p>
      <p class="px-2">
        Target Loation:

        {#each Object.keys(jsonTest.RequestData.TargetLocation) as key}
          <li>
            {key}: <span>{jsonTest.RequestData.TargetLocation[key]}</span>
          </li>
        {/each}
      </p>
      <p class="px-2">
        Reference Data:

        {#each Object.keys(jsonTest.RequestData.RefData) as key}
          <li>{key}: <span>{jsonTest.RequestData.RefData[key]}</span></li>
        {/each}
      </p>
    </div>
    <button on:click={() => sendMessage(JSON.stringify(jsonTest))}
      >Send to Server</button
    >
  </div>
  <article>
    <div class="inputGroup">
      <textarea placeholder="Enter command here" bind:value={textValue} />
      <button
        class="termSend"
        on:click={() => sendMessage(JSON.stringify(textValue))}>Send It!</button
      >
    </div>
    <div class="outputGroup">
      <h4>Server Response</h4>
      <div class="term">
        {#each messageList as message}
          <div class="msg">({message[1]}) > {message[0]}</div>
          <br />
        {/each}
      </div>
    </div>
  </article>
</main>

<style lang="postcss">
  main {
    @apply w-full h-full flex justify-center items-center;
  }
  .state {
    @apply w-full h-full;
  }
  article {
    @apply w-full h-[70%] flex flex-col justify-center items-center m-2;
  }
  h4 {
    @apply bg-gray-700/25 text-gray-400 bg-gray-900 absolute top-0;
  }
  span {
    @apply bg-gray-700 text-gray-200 font-semibold whitespace-nowrap select-text;
  }
  h3 {
    @apply text-lg;
  }
  textarea {
    overflow-wrap: break-word;
    @apply bg-gray-500/25 w-full h-[30%] text-white border-2 border-green-600 max-h-80 min-h-[4rem];
  }
  .term {
    overflow-wrap: break-word;
    @apply overflow-y-scroll h-full w-[50vw] bg-gray-900 p-2 flex flex-col-reverse;
  }
  .box {
    @apply overflow-y-scroll w-full h-[80%] bg-gray-900 text-gray-100 text-sm;
  }
  li {
    @apply px-2 p-1 list-none;
  }
  .termSend {
    @apply bg-green-600 h-full rounded-l-none;
  }
  .inputGroup {
    @apply flex w-full justify-center items-center p-2;
  }
  .outputGroup {
    @apply flex w-full h-full justify-center items-center relative flex-grow;
  }
  .msg {
    @apply bg-gray-700 rounded-xl p-2 select-text;
  }
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
