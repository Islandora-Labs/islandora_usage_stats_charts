<?php

/**
 * @file
 * Documents the hooks this module defines.
 */

/**
 * Allows modules to alter the array containing object usage entries.
 *
 * Useful for integrating usage stats not managed by Islandora Usage Stats.
 * Could also be used to filter out unwanted entries.
 *
 * @param array $usage
 *   The array of usage entries managed by Islandora Usage Stats. Entries
 *   looks like this:
 *     Array
 *      (
 *         [2017-12] => 1
 *         [2018-03] => 2
 *      )
 *    The keys are months in yyyy-mm format and the values are the number of
 *    hits in that month.
 * @param string $pid
 *   The PID of the object the usage data applies to.
 */
function mymodule_islandora_usage_stats_charts_usage_alter(&$usage, &$pid) {
  // Add a view data point.
  $usage[] = array('time' => '1520169807', 'ip' => '123.456.789.987');
}
