<script>
    import { chart } from "svelte-apexcharts";
    import {find, forEach} from "lodash";

    export let dataAB;
    export let dataLC;

    const colors = [];
    let data = [];

    const getData = function () {
        forEach(dataAB || dataLC, function (item) {
            let entry = find(data, {'series': item.group});
            if(!entry) {
                entry = {
                    name: item.group,
                    series: item.group,
                    data: []
                };
                data.push(entry);
                colors.push(item.color);
            }
            entry.data.push({
                y: item.b || item.l,
                x: item.a || item.c,
            });
        });
        return data;
    };

    const getOptions = function() {
        return {
            series: getData(),
            markers: {
                size: 5,
                colors: colors,
                strokeColor: '#000',
                strokeWidth: 1
            },
            chart: {
                animations: {
                    enabled: false
                },
                height: '700px',
                width: '700px',
                type: 'line',
                stroke: {
                    curve: 'straight',
                },
                toolbar: {
                    // Hamburger menu which has exports such as CSV etc.
                    // I have had issues displaying this, I believe some unrelated global CSS is causing issues
                    show: false
                },
                selection: {
                    enabled: false
                }
            },
            tooltip: {
                shared: false,
                intersect: true,
                theme: 'dark',
                x: {
                    show: false
                },
                y: {
                    formatter: undefined,
                    title: {
                        formatter: function (seriesName) {
                            return seriesName;
                        },
                    }
                },
                z: {
                    show: false
                }
            },
            legend: {
                show: false
            },
            xaxis: {
                labels: {
                    style: {
                        colors: '#fff',
                    }
                },
                title: {
                    text: dataAB ? 'a*' : 'C*',
                    offsetY: 75,
                    style: {
                        color: '#fff'
                    }
                }
            },
            yaxis: {
                labels: {
                    style: {
                        colors: '#fff',
                    },
                    formatter: function (value, info) {
                        if(isFinite(info)){
                            // This gets hit for each row (displayed as y-axis)
                            return value;
                        } else if(isFinite(info?.dataPointIndex)){
                            // This gets hit for each individual point on the heatmap (displayed as tooltip value)
                            return info?.dataPointIndex % 2 ? "Expected" : "Actual";
                        }
                    }
                },
                title: {
                    text: dataAB ? 'b*' : 'L*',
                    style: {
                        color: '#fff'
                    }
                },
                decimalsInFloat: 0
            },
            colors: colors
        }
    };

</script>

<main>
    <div use:chart={getOptions()}></div>
</main>
