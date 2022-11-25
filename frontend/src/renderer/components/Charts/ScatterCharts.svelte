<script lang="ts">
  import "@carbon/charts/styles.min.css";
  import "carbon-components/css/carbon-components.min.css";
  import { ScatterChart } from "@carbon/charts-svelte";

  // https://github.com/antimatter15/rgb-lab/blob/master/color.js
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

  export let data;
  export let matrix = "CM";

  $: if (data?.matrix_values) {
    dataLC = [];
    colorsLC = {};
    dataAB = [];
    colorsAB = {};
    let chartData = [];
    data?.matrix_values
      .find((ele) => ele.name === `${matrix} L*_camera`)
      ["data"].map((row, i) => {
        row.map((colData, k) => {
          chartData = [
            ...chartData,
            {
              col: String.fromCharCode(k + 65),
              row: i + 1,
              l: data.matrix_values.find(
                (ele) => ele.name === `${matrix} L*_camera`
              )["data"][i][k],
              a: data.matrix_values.find(
                (ele) => ele.name === `${matrix} a*_camera`
              )["data"][i][k],
              b: data.matrix_values.find(
                (ele) => ele.name === `${matrix} b*_camera`
              )["data"][i][k],
            },
          ];
        });
      });

    chartData.map((value) => {
      let rgb = lab2rgb([value.l, value.a, value.b]);
      dataAB.push({
        group: `${value.col}:${value.row}`,
        a: value.a,
        b: value.b,
      });
      colorsAB[
        `${value.col}:${value.row}`
      ] = `rgb(${rgb[0]}, ${rgb[1]}, ${rgb[2]})`;
    });

    chartData.map((value) => {
      let rgb = lab2rgb([value.l, value.a, value.b]);
      dataLC.push({
        group: `${value.col}:${value.row}`,
        l: value.l,
        c: Math.sqrt(value.a * value.a + value.b * value.b),
      });
      colorsLC[
        `${value.col}:${value.row}`
      ] = `rgb(${rgb[0]}, ${rgb[1]}, ${rgb[2]})`;
    });
  }

  let dataLC = [];
  let colorsLC = {};
  let dataAB = [];
  let colorsAB = {};
</script>

<div class="scatter-charts">
  <ScatterChart
    data={dataAB}
    options={{
      title: "a* vs b*",
      axes: {
        bottom: {
          title: "a*",
          mapsTo: "a",
          // ticks: {
          //   values: [
          //     -120, -100, -80, -60, -40, -20, 0, 20, 40, 60, 80, 100, 120,
          //   ],
          // },
          // domain: [-128, 128],
          ticks: {
            min: -120,
            max: 120,
          },
        },
        left: {
          title: "b*",
          mapsTo: "b",
          // ticks: {
          //   values: [
          //     -120, -100, -80, -60, -40, -20, 0, 20, 40, 60, 80, 100, 120,
          //   ],
          // },
          // domain: [-128, 128],
          ticks: {
            min: -120,
            max: 120,
          },
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
    data={dataLC}
    options={{
      title: "L* vs C*",
      axes: {
        bottom: {
          title: "c*",
          mapsTo: "c",
          ticks: {
            min: 0,
            max: 100,
          },
        },
        left: {
          title: "L*",
          mapsTo: "l",
          ticks: {
            min: 0,
            max: 100,
          },
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

<style lang="postcss" global>
  .scatter-charts {
    @apply w-full flex justify-center gap-10;
  }
  .bx--chart-holder {
    @apply aspect-square w-auto;
  }
  .bx--cc--scatter circle {
    fill-opacity: 1;
    r: 7;
  }

  .scatter-charts .bx--cc--scatter circle {
    stroke: black !important;
  }
</style>
