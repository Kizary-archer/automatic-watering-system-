/* eslint-disable */

(function () {
  window.renderChart = function renderChart(data) {
    const series = [];

    let interval = 86400000;
    if (data.wateringMode) interval = 3600000;

    data.dataHumidity.forEach((element, i) => {
      series.push([data.dateWatering + (i * interval), element]);
    });

    return Highcharts.stockChart('containerChart', {

      redraw: true,

      rangeSelector: {
        buttons: [{
          type: 'hour',
          count: 3,
          text: '3h',
        },
        {
          type: 'hour',
          count: 5,
          text: '5h',
        },
        {
          type: 'hour',
          count: 12,
          text: '12h',
        },
        {
          type: 'day',
          count: 1,
          text: '1d',
        },
        {
          type: 'day',
          count: 3,
          text: '3d',
        },
        {
          type: 'day',
          count: 7,
          text: '7d',
        },
        {
          type: 'all',
          text: 'All',
        },
        ],
        inputEnabled: false,
        selected: 0,
      },

      tooltip: {
        valueSuffix: '%',
      },

      yAxis: {
        plotLines: [{
          value: data.humidity,
          color: 'red',
          dashStyle: 'solid',
          width: 2,
          label: {
            text: 'Humidity minimum',
          },
        }],
      },

      xAxis: {
        type: 'datetime',
        tickPixelInterval: 150,
      },

      series: [{
        name: 'Humidity',
        type: 'spline',
        data: series,
      }],
    });
  };

  window.updateChartPlotLines = function updateChartPlotLines(chart, data) {
    chart.update(
      {
        yAxis: {
          plotLines: [{
            value: store.humidity,
            color: 'red',
            dashStyle: 'solid',
            width: 2,
            label: {
              text: 'Humidity minimum',
            },
          }],
        },
      },
      true,
    );
  };
}());
