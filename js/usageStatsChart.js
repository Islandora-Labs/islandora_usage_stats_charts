var islandoraUsageChart = document.getElementById('islandora-usage-stats-chart');

var usageData = {
  labels: ["Oct 2017", "Nov 2017", "Dec 2017", "Jan 2018", "Feb 2018", "March 2018", "April 2018"],
  datasets: [{
    label: "Views",
    backgroundColor: '#002db3',
    data: [7, 15, 12, 10, 5, 10, 13, 15],
  },
  {
    label: "Downloads",
    backgroundColor: '#99b3ff',
    data: [5, 10, 6, 5, 8, 14, 5],
  },]
};

var usageChart = new Chart(islandoraUsageChart, {
    type: 'bar',
    data: usageData,
    options: {
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero:true
                }
            }]
        }
    }
});

