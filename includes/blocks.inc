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
    $form['display_summary_only'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display usage summary only (no chart).'),
      '#default_value' => variable_get('display_summary_only', 0),
    );
    return $form;
  }
}

/**
 * Implements hook_block_save().
 */
function islandora_usage_stats_charts_block_save($delta = '', $edit = array()) {
  if ($delta == 'object') {
    variable_set('display_summary_only', (int) $edit['display_summary_only']);
  }
}

/**
 * Populate the usage statistics block for non-collection objects.
 */
function islandora_usage_stats_charts_generate_stats($pid) {
  $access_log_id = islandora_usage_stats_charts_get_object_access_id($pid);

  if (variable_get('display_summary_only')) {
    $result = db_select('islandora_usage_stats_object_access_log')
      ->fields('islandora_usage_stats_object_object_log', array('id'))
      ->condition('pid_id', $access_log_id)
      ->countQuery()
      ->execute();
    $count = $result->fetchField();

    $collapsed = variable_get('islandora_usage_stats_charts_collapsed', 1) ? 'collapsed' : '';
    $markup = theme('islandora_usage_stats_charts', array(
      'collapsed' => $collapsed,
      'content' => "This item has been accessed " . $count . " times.",
    ));
    return $markup;
  }

  $js_url = variable_get('islandora_usage_stats_charts_javascript_url', 'https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.2/Chart.bundle.js');
  drupal_add_js($js_url, 'external');

  // Query the object access table for rows for the current object.
  $result = db_select('islandora_usage_stats_object_access_log')
    ->fields('islandora_usage_stats_object_access_log',
      array('id', 'time', 'ip'))
    ->condition('pid_id', $access_log_id)
    ->execute();
  $uses = $result->fetchAll();

  foreach ($uses as $use) {
    $output[$use->id] = array('time' => $use->time, 'ip' => $use->ip);
  }

  // Define an alter hook to allow third-party modules to add data from other
  // sources (e.g. gathered outside of Islandora) and merge them with $output.
  // 'time' values should be in Unix epoch format.
  drupal_alter('islandora_usage_stats_charts_usage', $output, $pid);

  if (variable_get('islandora_usage_stats_charts_show_datastreams_downloads', 1)) {
    // @todo: Render datastream download stats and append the output to the
    // 'content' theme variable so all graphs are in the same block.
  }

  // @todo: Pass in $output via Drupal.settings.
  drupal_add_js(drupal_get_path('module', 'islandora_usage_stats_charts') . '/js/usageStatsChart.js', array('scope' => 'footer'));

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
