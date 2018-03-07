# Islandora Usage Stats Charts

## Overview

Displays object (and collection) usage stats collected by [Islandora Usage Stats](https://github.com/Islandora/islandora_usage_stats) as charts, which are rendered within standard Drupal blocks. An example chart looks like:

![Example chart](usage_stats_example.png)

## Dependencies

* [Islandora](https://github.com/Islandora/islandora)
* [Islandora Usage Stats](https://github.com/Islandora/islandora_usage_stats)

[Chart.js](http://www.chartjs.org/) is not a local requirement, since it is called from CDNJS.

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

1. Go to `admin/islandora/tools/islandora_usage_stats_charts`.
1. Enable and configure the 'Islandora Usage Stats Charts: object-level report' and 'Islandora Usage Stats Charts: collection-level report' blocks.
1. In the object-level block, you have the option of displaying only a summary of the usage. This setting can be found within the block's 'configure' options.

## Altering usage data

This module defines a Drupal alter hook that allows third-party modules to alter usage data for objects, for example to combine usage data from an external source with the data collected in Islandora Usage Stats database, or to filter out unwanted data. See `islandora_usage_stats_objects.api.php` for more information.

## To do

* Functionality
  * Provide admin option to not to use collapsible blocks.
  * Add downloads numbers. Maybe use mappings defined by [Islandora Downloadable Datastreams](https://github.com/bondjimbond/islandora_downloadable_datastreams), and make this module an optional depenedency (i.e., if it isn't installed, you've got to implement your own DS->downloadable file mapping, maybe through a hook we would define?).
  * Make object-level summary output themeable.
* Scalability and performance
  * Use Ajax to generate block content so that it doesn't slow down page rendering.
  * Add admin options to only show stats since a specific date (e.g., a day, week, month, year?)
* Nice to have
  * Provide admin option to use a local copy of Chart.js.
  * Provide a way to have object owner get an email on a monthly basis summarizing usage, maybe via a cron job (but this could probably be completely separate from this module).

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Bug reports, use cases, feature requests, and pull requests are welcome. If you want to open a pull request, please open an issue first, and use the pull request template.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
