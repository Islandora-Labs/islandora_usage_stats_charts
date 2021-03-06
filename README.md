# Islandora Usage Stats Charts

## Overview

Displays object (and collection) usage stats collected by [Islandora Usage Stats](https://github.com/Islandora/islandora_usage_stats) as charts, which are rendered within standard Drupal blocks. An example chart looks like:

![Example chart](usage_stats_example.png)

By default, only the last 6 months of data is shown in the chart, although the number of months can be configured.

## Dependencies

* [Islandora](https://github.com/Islandora/islandora)
* [Islandora Usage Stats](https://github.com/Islandora/islandora_usage_stats)

[Chart.js](http://www.chartjs.org/) is not a local requirement, since it is called from a CDN.

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

1. Set the number of months' data, colors for your charts' "views" and "downloads" bars, etc. at `admin/islandora/tools/islandora_usage_stats_charts`.
1. Enable and configure the "Islandora Usage Stats Charts: object-level report" and/or "Islandora Usage Stats Charts: collection-level report" blocks.
1. Select the content models you want to show the usage stats blocks for.

If you want to provide a CSV containing all of the usage data for an object, install and configure [Islandora Usage Stats CSV](https://github.com/mjordan/islandora_usage_stats_csv). If that module is installed, and if the "Show link to download stats CSV file" admin setting is checked, a link to the CSV will appear beneath the chart generated by this module (provided the user has permission to download the CSV file).

## Altering usage data

This module defines a Drupal alter hook that allows third-party modules to alter usage data for objects, for example to combine usage data from an external source with the data collected in Islandora Usage Stats database, or to filter out unwanted data. See `islandora_usage_stats_charts.api.php` for more information.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

We welcome contributions! [CONTRIBUTING.md](CONTRIBUTING.md) covers everthing you need to know.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
