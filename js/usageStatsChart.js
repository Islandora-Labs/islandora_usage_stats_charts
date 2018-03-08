var islandoraUsageChart = document.getElementById('islandora-usage-stats-chart');

var usageData = {
  labels: Drupal.settings.islandora_usage_stats_charts.chartLabels,
  datasets: [{
    label: "Views",
    backgroundColor: Drupal.settings.islandora_usage_stats_charts.viewsColor,
    data: Drupal.settings.islandora_usage_stats_charts.chartValues,
  },
/*
  {
    label: "Downloads",
    backgroundColor: Drupal.settings.islandora_usage_stats_charts.downloadsColor,
    data: [10, 6, 5, 8, 14, 5],
  },*/ ]
};

var usageChart = new Chart(islandoraUsageChart, {
    type: 'bar',
    data: usageData,
    options: {
        title: {
            display: true,
            text: 'Note: This is sample data'
        },
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true,
                     stepSize: 1 
                }
            }]
        }
    }
});

