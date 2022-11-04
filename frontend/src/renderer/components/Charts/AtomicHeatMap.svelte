
<script>
    import { chart } from "svelte-apexcharts";

    export let data;

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

    const options = {
        series: getData(),
        chart: {
            height: '600px',
            width: '600px',
            type: 'heatmap',
        },
        legend: {
            show: false
        },
        tooltip: {
            enabled: true,
            theme: 'dark',
            y: {
                show: false
            }
            //https://apexcharts.com/docs/options/tooltip/
        },
        toolbar: {
            show: true
        },
        plotOptions: {
            heatmap: {
                shadeIntensity: 0.5,
                radius: 0,
                useFillColorAsStroke: true,
                colorScale: {
                    ranges: [{
                        from: 0,
                        to: 5,
                        color: '#0a8a0a',
                        name: 'low',
                    },
                        {
                            from: 5,
                            to: 10,
                            color: '#d9d612',
                            name: 'medium',
                        },
                        {
                            from: 10,
                            to: 100,
                            color: '#ff1e00',
                            name: 'high',
                        }
                    ]
                }
            }
        },
        dataLabels: {
            enabled: true
        }
    };

</script>
<main>
    <div>
        <div use:chart={options} />
    </div>
</main>

<style>
</style>
