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

var usageChart = new Chart(islandoraUsageChart, {
    type: 'bar',
    data: usageData,
    options: {
        title: {
            display: true,
            text: 'Usage stats for the last ' + Drupal.settings.islandora_usage_stats_charts.numMonthsData + ' months'
        },
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true,
                    // @todo: change stepSize to 1 if there are fewer than 10 usages.
                    // stepSize: 1 
                }
            }]
        }
    }
});
