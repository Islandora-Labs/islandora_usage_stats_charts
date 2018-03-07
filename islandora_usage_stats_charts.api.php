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
 *   The array of usage entries managed by Islandora Usage Stats. Each entry has
 *   two keys, 'time' and 'ip'; 'time' must be in Unix epoch format.
 * @param string $pid
 *   The PID of the object the usage data applies to.
 */
function mymodule_islandora_usage_stats_charts_usage_alter(&$usage, &$pid) {
  // Add a view data point.
  $usage[] = array('time' => '1520169807', 'ip' => '123.456.789.987');
}
