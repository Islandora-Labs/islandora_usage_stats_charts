var islandoraUsageChart = document.getElementById('islandora-usage-stats-chart');

var usageData = {
  labels: ["Nov 2017", "Dec 2017", "Jan 2018", "Feb 2018", "March 2018", "April 2018"],
  datasets: [{
    label: "Views",
    backgroundColor: Drupal.settings.islandora_usage_stats_charts.viewsColor,
    data: [15, 12, 10, 5, 10, 13, 15],
  },
  {
    label: "Downloads",
    backgroundColor: Drupal.settings.islandora_usage_stats_charts.downloadsColor,
    data: [10, 6, 5, 8, 14, 5],
  },]
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
                    beginAtZero: true
                }
            }]
        }
    }
});

