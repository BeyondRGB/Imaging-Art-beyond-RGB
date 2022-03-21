<script lang="ts">
  import "@carbon/charts/styles.min.css";
  import "carbon-components/css/carbon-components.min.css";
  import { ScatterChart } from "@carbon/charts-svelte";
  import { text } from "svelte/internal";

  function lab2rgb(lab) {
    var y = (lab[0] + 16) / 116,
      x = lab[1] / 500 + y,
      z = y - lab[2] / 200,
      r,
      g,
      b;

    x = 0.95047 * (x * x * x > 0.008856 ? x * x * x : (x - 16 / 116) / 7.787);
    y = 1.0 * (y * y * y > 0.008856 ? y * y * y : (y - 16 / 116) / 7.787);
    z = 1.08883 * (z * z * z > 0.008856 ? z * z * z : (z - 16 / 116) / 7.787);

    r = x * 3.2406 + y * -1.5372 + z * -0.4986;
    g = x * -0.9689 + y * 1.8758 + z * 0.0415;
    b = x * 0.0557 + y * -0.204 + z * 1.057;

    r = r > 0.0031308 ? 1.055 * Math.pow(r, 1 / 2.4) - 0.055 : 12.92 * r;
    g = g > 0.0031308 ? 1.055 * Math.pow(g, 1 / 2.4) - 0.055 : 12.92 * g;
    b = b > 0.0031308 ? 1.055 * Math.pow(b, 1 / 2.4) - 0.055 : 12.92 * b;

    return [
      Math.max(0, Math.min(1, r)) * 255,
      Math.max(0, Math.min(1, g)) * 255,
      Math.max(0, Math.min(1, b)) * 255,
    ];
  }

  let inputData = [
    {
      a: 56,
      b: 65,
      l: 100,
    },
    {
      a: 117,
      b: -115,
      l: 38,
    },
    {
      a: 71,
      b: -122,
      l: 7,
    },
    {
      a: -40,
      b: -90,
      l: 50,
    },
    {
      a: 78,
      b: 68,
      l: 91,
    },
    {
      a: 66,
      b: 25,
      l: 59,
    },
    {
      a: 65,
      b: 121,
      l: 18,
    },
    {
      a: -71,
      b: 127,
      l: 5,
    },
    {
      a: -103,
      b: -127,
      l: 93,
    },
    {
      a: -67,
      b: 127,
      l: 85,
    },
    {
      a: 117,
      b: -35,
      l: 71,
    },
    {
      a: -61,
      b: -128,
      l: 99,
    },
    {
      a: -59,
      b: 88,
      l: 6,
    },
    {
      a: 57,
      b: -100,
      l: 68,
    },
    {
      a: 32,
      b: 22,
      l: 56,
    },
    {
      a: -33,
      b: 96,
      l: 86,
    },
    {
      a: 49,
      b: -61,
      l: 99,
    },
    {
      a: -110,
      b: 61,
      l: 93,
    },
    {
      a: 24,
      b: 102,
      l: 25,
    },
    {
      a: 25,
      b: -96,
      l: 99,
    },
    {
      a: 83,
      b: 6,
      l: 5,
    },
    {
      a: 39,
      b: -10,
      l: 30,
    },
    {
      a: 36,
      b: -22,
      l: 54,
    },
    {
      a: -55,
      b: 80,
      l: 91,
    },
    {
      a: -5,
      b: -115,
      l: 22,
    },
    {
      a: -117,
      b: 124,
      l: 34,
    },
    {
      a: -125,
      b: -57,
      l: 28,
    },
    {
      a: -103,
      b: -47,
      l: 87,
    },
    {
      a: -58,
      b: -124,
      l: 71,
    },
    {
      a: 68,
      b: 94,
      l: 26,
    },
    {
      a: 60,
      b: 71,
      l: 38,
    },
    {
      a: 18,
      b: -91,
      l: 26,
    },
    {
      a: -67,
      b: 112,
      l: 62,
    },
    {
      a: -43,
      b: -12,
      l: 37,
    },
    {
      a: 25,
      b: -41,
      l: 55,
    },
    {
      a: 19,
      b: -15,
      l: 88,
    },
    {
      a: 45,
      b: -125,
      l: 64,
    },
    {
      a: 35,
      b: 15,
      l: 67,
    },
    {
      a: -73,
      b: 48,
      l: 49,
    },
    {
      a: -42,
      b: -1,
      l: 93,
    },
    {
      a: -61,
      b: 80,
      l: 14,
    },
    {
      a: -29,
      b: -89,
      l: 91,
    },
    {
      a: -17,
      b: -15,
      l: 68,
    },
    {
      a: -78,
      b: -69,
      l: 25,
    },
    {
      a: 36,
      b: -99,
      l: 74,
    },
    {
      a: 103,
      b: -119,
      l: 50,
    },
    {
      a: 59,
      b: 21,
      l: 18,
    },
    {
      a: -39,
      b: -15,
      l: 33,
    },
    {
      a: 94,
      b: -47,
      l: 85,
    },
    {
      a: -63,
      b: -92,
      l: 82,
    },
  ];

  let testAB = [];
  let colorsAB = {};
  inputData.map((value) => {
    let rgb = lab2rgb([value.l, value.a, value.b]);
    testAB.push({
      group: `R(${Math.round(rgb[0])})-G(${Math.round(rgb[1])})-B(${Math.round(
        rgb[2]
      )})`,
      a: value.a,
      b: value.b,
    });
    colorsAB[
      `R(${Math.round(rgb[0])})-G(${Math.round(rgb[1])})-B(${Math.round(
        rgb[2]
      )})`
    ] = `rgb(${rgb[0]}, ${rgb[1]}, ${rgb[2]})`;
  });
  console.log(testAB);
  console.log(colorsAB);

  let testLC = [];
  let colorsLC = {};
  inputData.map((value) => {
    let rgb = lab2rgb([value.l, value.a, value.b]);
    testLC.push({
      group: `R(${Math.round(rgb[0])})-G(${Math.round(rgb[1])})-B(${Math.round(
        rgb[2]
      )})`,
      l: value.l,
      c: Math.sqrt(value.a * value.a + value.b * value.b),
    });
    colorsLC[
      `R(${Math.round(rgb[0])})-G(${Math.round(rgb[1])})-B(${Math.round(
        rgb[2]
      )})`
    ] = `rgb(${rgb[0]}, ${rgb[1]}, ${rgb[2]})`;
  });
</script>

<div class="scatter-charts">
  <ScatterChart
    data={testAB}
    options={{
      title: "a* vs b*",
      axes: {
        bottom: {
          title: "a*",
          mapsTo: "a",
          ticks: {
            values: [
              -120, -100, -80, -60, -40, -20, 0, 20, 40, 60, 80, 100, 120,
            ],
          },
          domain: [-128, 128],
        },
        left: {
          title: "b*",
          mapsTo: "b",
          ticks: {
            values: [
              -120, -100, -80, -60, -40, -20, 0, 20, 40, 60, 80, 100, 120,
            ],
          },
          domain: [-128, 128],
        },
      },
      legend: {
        enabled: false,
      },
      toolbar: {
        enabled: true,
        // controls: [
        //   { type: "Show as table" },
        //   { type: "Make Fullscrean" },
        //   { type: "Zoom out" },
        // ],
        numberOfIcons: 1,
      },
      height: "70vh",
      resizable: true,
      color: {
        scale: colorsAB,
      },
    }}
  />
  <ScatterChart
    data={testLC}
    options={{
      title: "L* vs C*",
      axes: {
        bottom: {
          title: "c*",
          mapsTo: "c",
          ticks: {
            values: [0, 20, 40, 60, 80, 100],
          },
          domain: [0, 100],
        },
        left: {
          title: "L*",
          mapsTo: "l",
          ticks: {
            values: [0, 20, 40, 60, 80, 100],
          },
          domain: [0, 100],
        },
      },
      legend: {
        enabled: false,
      },
      toolbar: {
        enabled: true,
        // controls: [
        //   { type: "Show as table" },
        //   { type: "Make Fullscrean" },
        //   { type: "Zoom out" },
        // ],
        numberOfIcons: 1,
      },
      height: "70vh",
      resizable: true,
      color: {
        scale: colorsLC,
      },
    }}
  />
</div>

<!-- <div class="heatmap-number-grid">
  {#each valueGrid.reverse() as row, i}
    {#each row as col, i}
      <p class="heatmap-value">
        {col.value}
      </p>
    {/each}
  {/each}
</div> -->
<style lang="postcss" global>
  .scatter-charts {
    @apply w-full flex justify-center;
  }
  .bx--chart-holder {
    @apply aspect-square w-auto;
  }
  .bx--cc--scatter circle {
    fill-opacity: 1;
    r: 7;
  }
</style>
