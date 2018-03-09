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

  // Get usage stats going back six months, grouped by month.
  $six_months_ago = $six_monts_ago = strtotime('-6 months', time());
  $views_result = db_query("SELECT year(FROM_UNIXTIME(time)) AS year, month(FROM_UNIXTIME(time)) AS month, count(*) AS count FROM {islandora_usage_stats_object_access_log} WHERE time BETWEEN :ago AND NOW() AND pid_id = :pid_id GROUP BY month(FROM_UNIXTIME(time))", array(':ago' => $six_months_ago, ':pid_id' => $access_log_id));
  $views_data = array();
  foreach ($views_result as $row) {
    $month = str_pad($row->month, 2, '0', STR_PAD_LEFT);
    $label = $row->year . '-' . $month;
    $views_data[$label] = $row->count;
  }

  // Define an alter hook to allow third-party modules to add data from other
  // sources (e.g. gathered outside of Islandora) and merge them with $output.
  // drupal_alter('islandora_usage_stats_charts_usage', $views_data, $pid, 'views');
  ksort($views_data, SORT_NATURAL);

  // Get the downloads stats going back six months, grouped by month, same
  // as the usage data.
  $downloads_chart_values = array();
  if (variable_get('islandora_usage_stats_charts_show_datastreams_downloads', 1)) {
    $downloads_result = db_query("SELECT year(FROM_UNIXTIME({islandora_usage_stats_object_ds_access_log}.time)) AS year, month(FROM_UNIXTIME({islandora_usage_stats_object_ds_access_log}.time)) AS month, COUNT(*) AS count FROM {islandora_usage_stats_datastreams}, {islandora_usage_stats_object_ds_access_log} WHERE {islandora_usage_stats_object_ds_access_log}.time BETWEEN :ago AND NOW() AND {islandora_usage_stats_datastreams}.pid_id = :pid_id GROUP BY month(FROM_UNIXTIME({islandora_usage_stats_object_ds_access_log}.time))", array(':ago' => $six_months_ago, ':pid_id' => $access_log_id));
    $downloads_data = array();
    foreach ($downloads_result as $row) {
      $month = str_pad($row->month, 2, '0', STR_PAD_LEFT);
      $label = $row->year . '-' . $month;
      $downloads_data[$label] = $row->count;
    }

    // Define an alter hook to allow third-party modules to add data from other
    // sources (e.g. gathered outside of Islandora) and merge them with $output.
    // drupal_alter('islandora_usage_stats_charts_usage', $downloads_data, $pid, 'downloads');
    ksort($downloads_data, SORT_NATURAL);
  }

  // First, assemble the labels from the keys of the views and downloads arrays.
  // If downloads are turned off, just use the keys from the views data.
  if (variable_get('islandora_usage_stats_charts_show_datastreams_downloads', 1)) {
    $chart_labels = array_merge(array_keys($views_data), array_keys($downloads_data));
    $chart_labels = array_unique($chart_labels);
    sort($chart_labels, SORT_NATURAL);
  }
  else {
    $chart_labels = array_keys($views_data);
    sort($chart_labels, SORT_NATURAL);
  }

  // Then, create two arrays for the values, paralleling the list of month keys,
  // to pass into Chart.js. If downloads are turned off, just use the values
  // from the views data.
  if (variable_get('islandora_usage_stats_charts_show_datastreams_downloads', 1)) {
    $views_chart_values = array();
    foreach ($chart_labels as $label) {
      if (array_key_exists($label, $views_data)) {
        $views_chart_values[] = $views_data[$label];
      }
      else {
        $views_chart_values[] = 0;
      }
    }
    $download_chart_values = array();
    foreach ($chart_labels as $label) {
      if (array_key_exists($label, $downloads_data)) {
        $downloads_chart_values[] = $downloads_data[$label];
      }
      else {
        $downloads_chart_values[] = 0;
      }
    }
  }
  else {
    $views_chart_values = array_values($views_data);
  }

  drupal_add_js(drupal_get_path('module', 'islandora_usage_stats_charts') . '/js/usageStatsChart.js', array('scope' => 'footer'));

  $config_settings = array(
    'viewsColor' => variable_get('islandora_usage_stats_charts_views_color', '#002db3'),
    'downloadsColor' => variable_get('islandora_usage_stats_charts_downloads_color', '#99b3ff'),
    'showDownloads' => variable_get('islandora_usage_stats_charts_show_datastreams_downloads', 0),
  );
  drupal_add_js(array('islandora_usage_stats_charts' => $config_settings), 'setting');
  drupal_add_js(
    array(
      'islandora_usage_stats_charts' => array(
        'chartLabels' => $chart_labels,
        'viewsChartValues' => $views_chart_values,
        'downloadsChartValues' => $downloads_chart_values,
      )), 'setting');

  $collapsed = variable_get('islandora_usage_stats_charts_collapsed', 1) ? 'collapsed' : '';
  $markup = theme('islandora_usage_stats_charts', array(
    'collapsed' => $collapsed,
  ));
  return $markup;
}

/**
 * Populate the usage statistics block for collection objects.
 */
function islandora_usage_stats_charts_generate_collection_stats($pid) {
  $collection_access_log_id = islandora_usage_stats_charts_get_object_access_id($pid);

  $js_url = variable_get('islandora_usage_stats_charts_javascript_url', 'https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.2/Chart.bundle.js');
  drupal_add_js($js_url, 'external');

  // Get usage stats going back six months, grouped by month.
  $six_months_ago = $six_monts_ago = strtotime('-6 months', time());
  $result = db_query("SELECT year(FROM_UNIXTIME({islandora_usage_stats_object_access_log}.time)) AS year, month(FROM_UNIXTIME({islandora_usage_stats_object_access_log}.time)) AS month, COUNT(*) AS count FROM {islandora_usage_stats_object_access_log}, {islandora_usage_stats_collection_access_log} WHERE {islandora_usage_stats_object_access_log}.time between :ago AND NOW() AND {islandora_usage_stats_collection_access_log}.collection = :collection_id AND {islandora_usage_stats_collection_access_log}.object_access_id = {islandora_usage_stats_object_access_log}.id GROUP BY month(from_unixtime({islandora_usage_stats_object_access_log}.time))", array(':ago' => $six_months_ago, ':collection_id' => $collection_access_log_id));
  $data = array();
  foreach ($result as $row) {
    $month = str_pad($row->month, 2, '0', STR_PAD_LEFT);
    $label = $row->year . '-' . $month;
    $data[$label] = $row->count;
  }

  // Define an alter hook to allow third-party modules to add data from other
  // sources (e.g. gathered outside of Islandora) and merge them with $output.
  // drupal_alter('islandora_usage_stats_charts_usage', $data, $pid, 'collections');
  ksort($data, SORT_NATURAL);

  $chart_labels = array_keys($data);
  $views_chart_values = array_values($data);

  drupal_add_js(drupal_get_path('module', 'islandora_usage_stats_charts') . '/js/usageStatsChart.js', array('scope' => 'footer'));

  $config_settings = array(
    'viewsColor' => variable_get('islandora_usage_stats_charts_views_color', '#002db3'),
    'downloadsColor' => variable_get('islandora_usage_stats_charts_downloads_color', '#99b3ff'),
    'showDownloads' => variable_get('islandora_usage_stats_charts_show_datastreams_downloads', 0),
  );
  drupal_add_js(array('islandora_usage_stats_charts' => $config_settings), 'setting');
  drupal_add_js(
    array(
      'islandora_usage_stats_charts' => array(
        'chartLabels' => $chart_labels,
        'viewsChartValues' => $views_chart_values,
      )), 'setting');

  $collapsed = variable_get('islandora_usage_stats_charts_collapsed', 1) ? 'collapsed' : '';
  $markup = theme('islandora_usage_stats_charts', array(
    'collapsed' => $collapsed,
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
