/*
@file
Javascript that renders a Chart.js chart of monthly usage data from Islandora Usage Statistics.
*/

var islandoraUsageChart = document.getElementById('islandora-usage-stats-chart');

var usageData = {
  labels: Drupal.settings.islandora_usage_stats_charts.chartLabels,
  datasets: [{
    label: "Views",
    backgroundColor: Drupal.settings.islandora_usage_stats_charts.viewsColor,
    data: Drupal.settings.islandora_usage_stats_charts.viewsChartValues,
  }]
};

if (Drupal.settings.islandora_usage_stats_charts.showDownloads) {
  var downloadsData = [];
  downloadsData['label'] = 'Downloads';
  downloadsData['backgroundColor'] = Drupal.settings.islandora_usage_stats_charts.downloadsColor; 
  downloadsData['data'] = Drupal.settings.islandora_usage_stats_charts.downloadsChartValues;
  usageData.datasets.push(downloadsData);
}

// Override the Drupal admin setting that defines how many months' worth of data
// to include in the chart, if the number of months available is lower. The number
// of months is also used in the chart's title.
if (usageData.labels.length < Drupal.settings.islandora_usage_stats_charts.numMonthsData) {
    var numMonths = usageData.labels.length;
} else {
    var numMonths = Drupal.settings.islandora_usage_stats_charts.numMonthsData;
}

var maxViewsCount = Math.max.apply(Math, Drupal.settings.islandora_usage_stats_charts.viewsChartValues);
var maxDownloadsCount = Math.max.apply(Math, Drupal.settings.islandora_usage_stats_charts.downloadsChartValues);
// If the number of hits is large, make the step size large. We want at most 10 steps.
if (maxViewsCount > 60 || maxDownloadsCount > 60) {
  var stepSizeValue = maxViewsCount / 10;
// By default, Chart.js's Y axis shows decimal point if the value is less than 10. We don't want that.
} else if (maxViewsCount < 10 || maxDownloadsCount < 10) {
  var stepSizeValue = 1;
} else {
  // Use the default.
  var stepSizeValue = null;
}

console.log(stepSizeValue);

// Render the chart.
usageChart = new Chart(islandoraUsageChart, {
    type: 'bar',
    data: usageData,
    options: {
        title: {
            display: true,
            text: 'Usage stats for the last ' + numMonths + ' months'
        },
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true,
                    stepSize: stepSizeValue,
                }
            }]
        }
    }
});

// Size controls.
if (Drupal.settings.islandora_usage_stats_charts.chartWidth.length) {
    usageChart.canvas.parentNode.style.width = Drupal.settings.islandora_usage_stats_charts.chartWidth;
}
if (Drupal.settings.islandora_usage_stats_charts.limitChartHeight) {
    usageChart.canvas.parentNode.style.height = usageChart.canvas.parentNode.style.width;
}
