<script lang="ts">
  import "@carbon/charts/styles.min.css";
  import "carbon-components/css/carbon-components.min.css";
  import { LineChart } from "@carbon/charts-svelte";
  import html2canvas from "html2canvas";

  let inputData = [
    {
      group: "1",
      intensity: 2,
      wavelength: 440,
    },
    {
      group: "1",
      intensity: 3.2,
      wavelength: 480,
    },
    {
      group: "1",
      intensity: 2.7,
      wavelength: 520,
    },
    {
      group: "1",
      intensity: 2.3,
      wavelength: 540,
    },
    {
      group: "1",
      intensity: 3.5,
      wavelength: 580,
    },
    {
      group: "1",
      intensity: 3.0,
      wavelength: 630,
    },
    {
      group: "1",
      intensity: 5.3,
      wavelength: 730,
    },
    {
      group: "1",
      intensity: 5.0,
      wavelength: 780,
    },
    {
      group: "1",
      intensity: 4.4,
      wavelength: 830,
    },
    {
      group: "1",
      intensity: 5.9,
      wavelength: 870,
    },
    {
      group: "1",
      intensity: 35,
      wavelength: 940,
    },
  ];

  let imageDataURL;

  function savePNG() {
    html2canvas(
      document
        .getElementById("EstSpecChart")
        .querySelector(".bx--chart-holder"),
      {
        backgroundColor: "#3a3a3c",
        scale: window.devicePixelRatio * 2,
        windowWidth: window.innerWidth,
        windowHeight: window.innerHeight,
      }
    ).then(function (canvas) {
      console.log("Created Canvas");
      console.log(document.querySelector(".bx--chart-holder"));
      // document.getElementById("EstSpecChart").appendChild(canvas);
      imageDataURL = canvas.toDataURL("image/png");

      var anchor = document.createElement("a");
      anchor.href = imageDataURL;
      anchor.target = "_blank";
      anchor.download = "Estimate Spectrum.png";
      document.body.appendChild(anchor);
      anchor.click();
      document.body.removeChild(anchor);
    });
  }

  // let testAB = [];
  // let colorsAB = {};
  // inputData.map((value) => {
  //   let rgb = lab2rgb([(value.intensity + value.wavelength) / 2, 0, 0]);
  //   testAB.push({
  //     group: `L*ref(${value.wavelength})-L*cam(${value.intensity})`,
  //     wavelength: value.wavelength,
  //     intensity: value.intensity,
  //   });
  //   colorsAB[
  //     `L*ref(${value.wavelength})-L*cam(${value.intensity})`
  //   ] = `rgb(${rgb[0]}, ${rgb[1]}, ${rgb[2]})`;
  // });
  // console.log(testAB);
  // console.log(colorsAB);
</script>

<button on:click={savePNG}>dl</button>
<div class="line-chart" id="EstSpecChart">
  <LineChart
    data={inputData}
    options={{
      title: "Estimate Spectrum ",
      axes: {
        bottom: {
          title: "Wavelength (nm)",
          mapsTo: "wavelength",
          ticks: {
            min: 400,
            max: 1000,
          },
          limitDomainToBins: true,
          domain: [400, 1000],
        },
        left: {
          title: "",
          mapsTo: "intensity",
          percentage: true,
          // ticks: {
          //   values: [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100],
          // },
          // domain: [0, 100],
        },
      },
      legend: {
        enabled: false,
      },
      toolbar: {
        enabled: false,
        // controls: [
        //   { type: "Show as table" },
        //   { type: "Make Fullscrean" },
        //   { type: "Zoom out" },
        // ],
        numberOfIcons: 1,
      },
      // width: "30vw",
      resizable: true,
      color: {
        scale: {
          1: "rgb(59 130 246)",
        },
      },
    }}
  />
</div>

<style lang="postcss" global>
  .line-chart .bx--chart-holder {
    @apply w-auto;
  }
  .line-chart .bx--cc--scatter circle {
    fill-opacity: 1;
    r: 4;
  }
  .line-chart .bx--cc--chart-wrapper .stroke-1-1-1 {
    stroke: rgb(59 130 246);
  }

  .line-chart .bx--cc--line path.line {
    stroke-width: 2%;
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
</style>
