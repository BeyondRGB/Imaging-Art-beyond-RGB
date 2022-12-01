<script>
    import { chart } from "svelte-apexcharts";

    export let data;

    // Colorblind option
    export let visionDeficiencyMode = false;

    // Parse the needed data for the heatmap
    function getData() {
        let formattedData = [];
        let tempData = [];
        for (let i = 0; i < data.length; i++) {
            tempData[i] = [];
            for (let j = 0; j < data[i].length; j++) {
                tempData[i][j] = {
                    y: data[i][j].toFixed(2),
                    x: String.fromCharCode(j + 65)
                };
            }
            formattedData.push({
                name: i,
                data: tempData[i]
            });
        }

        return formattedData;
    }

    const colors = [
        "#0a9400",
        "#199a00",
        "#2a9f00",
        "#3ca500",
        "#4eab00",
        "#62b000",
        "#77b600",
        "#8dbb00",
        "#a4c100",
        "#bdc700",
        "#ccc300",
        "#d2b300",
        "#d8a300",
        "#dd9200",
        "#e37f00",
        "#e86c00",
        "#ee5700",
        "#f44100",
        "#f92a00",
        "#ff1200"
    ];

    const grayScale = [
        "#ffffff",
        "#efefef",
        "#d3d3d3",
        "#c1c1c1",
        "#b5b5b5",
        "#9e9e9e",
        "#898989",
        "#767676",
        "#585858",
        "#4c4c4c",
        "#4a4a4a",
        "#4c4c4c",
        "#383838",
        "#383838",
        "#313131",
        "#2a2a2a",
        "#1c1c1c",
        "#191919",
        "#171717",
        "#000000"
    ];

    function ranges() {
        const result = [];
        for (let i = 0; i < 20; i++) {
            result.push({
                from: i/2,
                to: i/2 + .5,
                color: visionDeficiencyMode ? grayScale[i] : colors[i]
            });
        }
        result[result.length - 1].to = Number.MAX_SAFE_INTEGER;
        return result;
    }

    const getOptions = function() {
        return {
            series: getData(),
            chart: {
                height: '700px',
                width: '700px',
                type: 'heatmap',
                toolbar: {
                    // Hamburger menu which has exports such as CSV etc.
                    // I have had issues displaying this, I believe some unrelated global CSS is causing issues
                    show: false
                },
                selection: {
                    enabled: false
                }
            },
            legend: {
                show: false
            },
            tooltip: {
                enabled: true,
                theme: 'dark',
                y: {
                    show: false,
                    title: {
                        // Displayed before the value on the tooltip, unnecessary
                        formatter: () => ""
                    }
                }
            },
            yaxis: {
                labels: {
                    formatter: function (value, info) {
                        if(info === data?.length) {
                            // This gets hit before we even get to the heatmap and displays a random axisLabel
                            return "";
                        } else if(isFinite(info)){
                            // This gets hit for each row on the heatmap (displayed as y-axis)
                            return data?.length - info;
                        } else {
                            // This gets hit for each individual square on the heatmap (displayed as tooltip value)
                            return value;
                        }
                    }
                },
            },
            plotOptions: {
                heatmap: {
                    shadeIntensity: 0.5,
                    radius: 0,
                    useFillColorAsStroke: true,
                    // EnableShades ends up shading higher-value greens darker shades, which is not what we want
                    enableShades: false,
                    colorScale: {
                        ranges: ranges()
                    }
                }
            },
            dataLabels: {
                enabled: true,
                style: {
                    colors: visionDeficiencyMode ? ['#e60b0b'] : ['#ffffff']
                }
            }
        }
    };

</script>

<main>
    {#key visionDeficiencyMode}
        <div use:chart={getOptions()}></div>
    {/key}
</main>
