<?php

/**
 * @file
 * Block-related functions.
 */

/**
 * Implements hook_block_info().
 */
function islandora_usage_stats_charts_block_info() {
  $blocks = array();
  $blocks['object'] = array(
    'info' => t('Islandora Usage Stats Charts: object-level report'),
    'cache' => DRUPAL_CACHE_PER_USER,
  );
  $blocks['collection'] = array(
    'info' => t('Islandora Usage Stats Charts: collection-level report'),
    'cache' => DRUPAL_CACHE_PER_USER,
  );
  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function islandora_usage_stats_charts_block_view($delta = '') {
  $object = menu_get_object('islandora_object', 2);
  if ($delta == 'object') {
    if ($object && !in_array('islandora:collectionCModel', $object->models)) {
      $block['content'] = islandora_usage_stats_charts_generate_stats($object->id);
      return $block;
    }
  }
  if ($delta == 'collection') {
    if ($object && in_array('islandora:collectionCModel', $object->models)) {
      $block['content'] = islandora_usage_stats_charts_generate_collection_stats($object->id);
      return $block;
    }
  }
}

/**
 * Implements hook_block_configure().
 */
function islandora_usage_stats_charts_block_configure($delta) {
  if ($delta == 'object') {
    $form['islandora_usage_stats_charts_collapsed'] = array(
      '#type' => 'checkbox',
      '#title' => t('Collapse usage stats chart block by default (user can toggle)'),
      '#default_value' => variable_get('islandora_usage_stats_charts_collapsed', 1),
    );
    return $form;
  }
}

/**
 * Implements hook_block_save().
 */
function islandora_usage_stats_charts_block_save($delta = '', $edit = array()) {
  if ($delta == 'object') {
    variable_set('islandora_usage_stats_charts_collapsed', (int) $edit['islandora_usage_stats_charts_collapsed']);
  }
}

/**
 * Populate the usage statistics block for non-collection objects.
 */
function islandora_usage_stats_charts_generate_stats($pid) {
  $access_log_id = islandora_usage_stats_charts_get_object_access_id($pid);

  $js_url = variable_get('islandora_usage_stats_charts_javascript_url', 'https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.2/Chart.bundle.js');
  drupal_add_js($js_url, 'external');

  // Get usage stats going back six months, grouped by month. This is not
  // portable across RDBMSs, but is also probably not possible using
  // Drupal's database API.
  $six_months_ago = $six_monts_ago = strtotime('-6 months', time());
  $dev_result = db_query("select year(from_unixtime(time)) as year, month(from_unixtime(time)) as month, count(*) as count from {islandora_usage_stats_object_access_log} where time between :ago and now() and pid_id = :pid_id group by month(from_unixtime(time))", array(':ago' => $six_months_ago, ':pid_id' => $access_log_id));
  $data = array();
  foreach ($dev_result as $row) {
    $month = str_pad($row->month, 2, '0', STR_PAD_LEFT);
    $label = $row->year . '-' . $month;
    $data[$label] = $row->count;
  }
  ksort($data, SORT_NATURAL);
  $chart_labels = array_keys($data);
  $chart_values = array_values($data);

  // Define an alter hook to allow third-party modules to add data from other
  // sources (e.g. gathered outside of Islandora) and merge them with $output.
  // 'time' values should be in Unix epoch format.
  drupal_alter('islandora_usage_stats_charts_usage', $output, $pid);

  if (variable_get('islandora_usage_stats_charts_show_datastreams_downloads', 1)) {
    // @todo: Get datastream download stats and pass to Chart.js.
  }

  drupal_add_js(drupal_get_path('module', 'islandora_usage_stats_charts') . '/js/usageStatsChart.js', array('scope' => 'footer'));

  $bar_colors = array(
    'viewsColor' => variable_get('islandora_usage_stats_charts_views_color', '#002db3'),
    'downloadsColor' => variable_get('islandora_usage_stats_charts_downloads_color', '#99b3ff'),
  );
  drupal_add_js(array('islandora_usage_stats_charts' => $bar_colors), 'setting');
  drupal_add_js(array('islandora_usage_stats_charts' => array('chartLabels' => $chart_labels, 'chartValues' => $chart_values)), 'setting');

  $collapsed = variable_get('islandora_usage_stats_charts_collapsed', 1) ? 'collapsed' : '';
  $markup = theme('islandora_usage_stats_charts', array(
    'collapsed' => $collapsed,
    // The 'content' theme variable is inserted into a div with ID
    // "islandora-usage-stats-charts-content".
    'content' => '<canvas id="islandoraUsageStatsChart" width="400" height="400"></canvas>',
  ));
  return $markup;
}

/**
 * Populate the usage statistics block for collection objects.
 */
function islandora_usage_stats_charts_generate_collection_stats($pid) {
  // drupal_add_js('misc/collapse.js');
  $collection_access_log_id = islandora_usage_stats_charts_get_object_access_id($pid);

  // Query the collection access table for rows for the current object.
  $result = db_select('islandora_usage_stats_collection_access_log')
    ->fields('islandora_usage_stats_object_access_log', array('id'))
    ->condition('collection', $collection_access_log_id)
    ->countQuery()
    ->execute();
  $count = $result->fetchField();

  $collapsed = variable_get('islandora_usage_stats_charts_collapsed', 1) ? 'collapsed' : '';
  $markup = theme('islandora_usage_stats_charts', array(
    'collapsed' => $collapsed,
    // The 'content' theme variable is inserted into a div with ID
    // "islandora-usage-stats-charts-content".
    'content' => "Objects in this collection have been accessed " . $count . " times.",
  ));
  return $markup;
}

/**
 * Get the ID in the object access table corresponding to the incoming PID.
 *
 * @param string $pid
 *   The object's PID.
 *
 * @return string
 *   The corresponding ID from the islandora_usage_stats_objects table.
 */
function islandora_usage_stats_charts_get_object_access_id($pid) {
  $result = db_select('islandora_usage_stats_objects')
    ->fields('islandora_usage_stats_objects', array('id'))
    ->condition('pid', $pid)
    ->execute();
  return $result->fetchAssoc();
}
