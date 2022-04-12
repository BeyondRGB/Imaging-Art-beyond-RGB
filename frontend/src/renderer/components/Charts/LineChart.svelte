<script lang="ts">
  import "@carbon/charts/styles.min.css";
  import "carbon-components/css/carbon-components.min.css";
  import { LineChart } from "@carbon/charts-svelte";
  import { ImageIcon } from "svelte-feather-icons";
  import html2canvas from "html2canvas";
  import { element } from "svelte/internal";

  export let data = [];
  export let wavelengthArray;
  export let trueShadowPos;

  let inputData = [];

  $: if (data.length > 1) {
    console.log("Creating InputData");
    inputData = [];
    wavelengthArray.forEach((element, i) => {
      inputData.push({
        intensity: data[i] * 100,
        wavelength: element,
      });
    });
    inputData = inputData;
  }

  let imageDataURL;

  function savePNG() {
    html2canvas(document.querySelector("div.line-chart .bx--chart-holder"), {
      backgroundColor: "#3a3a3c",
      scale: window.devicePixelRatio * 2,
      windowWidth: window.innerWidth,
      windowHeight: window.innerHeight,
    }).then(function (canvas) {
      console.log("Created Canvas");
      imageDataURL = canvas.toDataURL("image/png");

      var anchor = document.createElement("a");
      anchor.href = imageDataURL;
      anchor.target = "_blank";
      anchor.download = `Estimate_Spectrum_(${parseFloat(
        trueShadowPos?.left
      ).toFixed(1)}, ${parseFloat(trueShadowPos?.top).toFixed(1)}).png`;
      document.body.appendChild(anchor);
      anchor.click();
      document.body.removeChild(anchor);
    });
  }
</script>

<div class="line-chart" id="EstSpecChart">
  <button class="saveBtn" on:click={savePNG}><ImageIcon size="1.5x" /></button>
  <LineChart
    data={inputData}
    options={{
      title: `Estimate Spectrum (${parseFloat(trueShadowPos?.left).toFixed(
        1
      )}, ${parseFloat(trueShadowPos?.top).toFixed(1)})`,
      axes: {
        bottom: {
          title: "Wavelength (nm)",
          mapsTo: "wavelength",

          limitDomainToBins: true,
        },
        left: {
          title: "",
          mapsTo: "intensity",
          percentage: true,
        },
      },
      legend: {
        enabled: false,
      },
      toolbar: {
        enabled: true,
        controls: [{ type: "Export as CSV" }],
        // numberOfIcons: 1,
      },
      // width: "30vw",
      resizable: true,
    }}
  />
</div>

<style lang="postcss" global>
  .line-chart {
    @apply relative;
  }
  .line-chart .bx--chart-holder {
    @apply w-auto;
  }
  .line-chart .saveBtn {
    @apply flex w-fit p-1 rounded-lg absolute top-0 right-10 z-50;
  }
  .line-chart .bx--cc--scatter circle {
    fill-opacity: 1;
    r: 5;
    stroke: none;
  }

  .line-chart .bx--cc--line path.line {
    stroke-width: 1%;
  }

  .line-chart .bx--chart-holder.filled,
  .bx--chart-holder.fullscreen {
    @apply bg-transparent;
  }

  .line-chart .bx--chart-holder.filled .bx--cc--chart-wrapper,
  .bx--chart-holder.fullscreen .bx--cc--chart-wrapper {
    @apply bg-transparent;
  }

  .line-chart .bx--chart-holder.fullscreen,
  .bx--chart-holder:-webkit-full-screen {
    width: 0;
    height: 0;
    padding: 0;
  }

  /* .line-chart .bx--cc--chart-wrapper .layout-child {
    @apply bg-indigo-600 overflow-hidden w-[50%];
  } */
  /* .line-chart .bx--cc--spacer {
    @apply bg-red-600;
  }
  .line-chart .bx--cc--layout-column {
    @apply bg-green-400 my-6;
  } */

  /* .line-chart .bx--cc--scatter circle:first-child {
    fill-opacity: 1;
    fill: #8300b5;
    r: 6;
  } */

  .line-chart .bx--cc--chart-wrapper .bx--cc--line > .stroke-1-1-1 {
    stroke: white;
  }

  .line-chart .bx--cc--scatter circle:nth-child(1) {
    fill: #610061;
  }

  .line-chart .bx--cc--scatter circle:nth-child(2) {
    fill: #79008d;
  }

  .line-chart .bx--cc--scatter circle:nth-child(3) {
    fill: #8300b5;
  }

  .line-chart .bx--cc--scatter circle:nth-child(4) {
    fill: #7e00db;
  }

  .line-chart .bx--cc--scatter circle:nth-child(5) {
    fill: #6a00ff;
  }

  .line-chart .bx--cc--scatter circle:nth-child(6) {
    fill: #3d00ff;
  }

  .line-chart .bx--cc--scatter circle:nth-child(7) {
    fill: #0000ff;
  }

  .line-chart .bx--cc--scatter circle:nth-child(8) {
    fill: #0046ff;
  }

  .line-chart .bx--cc--scatter circle:nth-child(9) {
    fill: #007bff;
  }

  .line-chart .bx--cc--scatter circle:nth-child(10) {
    fill: #00a9ff;
  }

  .line-chart .bx--cc--scatter circle:nth-child(11) {
    fill: #00d5ff;
  }

  .line-chart .bx--cc--scatter circle:nth-child(12) {
    fill: #00ffff;
  }

  .line-chart .bx--cc--scatter circle:nth-child(13) {
    fill: #00ff92;
  }

  .line-chart .bx--cc--scatter circle:nth-child(14) {
    fill: #00ff00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(15) {
    fill: #36ff00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(16) {
    fill: #5eff00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(17) {
    fill: #81ff00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(18) {
    fill: #a3ff00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(19) {
    fill: #c3ff00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(20) {
    fill: #ffff00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(21) {
    fill: #ffdf00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(22) {
    fill: #ffbe00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(23) {
    fill: #ff9b00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(24) {
    fill: #ff7700;
  }

  .line-chart .bx--cc--scatter circle:nth-child(25) {
    fill: #ff4f00;
  }

  .line-chart .bx--cc--scatter circle:nth-child(26) {
    fill: #ff2100;
  }

  .line-chart .bx--cc--scatter circle:nth-child(27) {
    fill: #ff0000;
  }

  .line-chart .bx--cc--scatter circle:nth-child(28) {
    fill: #ff0000;
  }

  .line-chart .bx--cc--scatter circle:nth-child(29) {
    fill: #ff0000;
  }

  .line-chart .bx--cc--scatter circle:nth-child(30) {
    fill: #ff0000;
  }

  .line-chart .bx--cc--scatter circle:nth-child(31) {
    fill: #ff0000;
  }

  .line-chart .bx--cc--scatter circle:nth-child(32) {
    fill: #ff0000;
  }

  .line-chart .bx--cc--scatter circle:nth-child(33) {
    fill: #ff0000;
  }

  .line-chart .bx--cc--scatter circle:nth-child(34) {
    fill: #ff0000;
  }
  .line-chart .bx--cc--scatter circle:nth-child(35) {
    fill: #ff0000;
  }
</style>
