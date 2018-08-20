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
  if ($object) {
    module_load_include('inc', 'islandora_usage_stats_charts', 'includes/utilities');
    $allowed_cmodels = islandora_usage_stats_charts_check_for_cmodels($object);
    if ($delta == 'object') {
      if ($object && !in_array('islandora:collectionCModel', $object->models) && count($allowed_cmodels) > 0 && arg(3) != 'manage') {
        $block['content'] = islandora_usage_stats_charts_generate_stats($object->id);
        return $block;
      }
    }
    if ($delta == 'collection') {
      $url_params = drupal_get_query_parameters();
      if ($object && in_array('islandora:collectionCModel', $object->models) && count($allowed_cmodels) > 0 && arg(3) != 'manage' && !array_key_exists('page', $url_params)) {
        $block['content'] = islandora_usage_stats_charts_generate_collection_stats($object->id);
        return $block;
      }
    }
  }
}
