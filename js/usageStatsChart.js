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

// Adapt the Chart.js Y axis 'stepSize' to the range of values in our data. First, get 
// the maximum values in either the views or downloads data. We'll base our stepSize on
// the highest value we have.
var maxViewsCount = Math.max.apply(Math, Drupal.settings.islandora_usage_stats_charts.viewsChartValues);
var maxDownloadsCount = Math.max.apply(Math, Drupal.settings.islandora_usage_stats_charts.downloadsChartValues);
var higherCount = Math.max.apply(Math, [maxViewsCount, maxDownloadsCount]);
// If the number of views or downloads is large, adjust the stepSize so that it's 1/10 of the number.
if (higherCount > 50) {
  var stepSizeValue = higherCount / 10;
// By default, the Y axis shows decimal point if the value is less than 10. We don't want that.
} else if (higherCount < 10) {
  var stepSizeValue = 1;
} else {
  // Use the default.
  var stepSizeValue = null;
}

// Render the chart using the Chart.js API.
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
