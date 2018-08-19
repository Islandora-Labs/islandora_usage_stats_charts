<?php

/**
 * @file
 * Utility functions used by the Islandora Usage Stats Charts module.
 */

/**
 * Query the resource index to get a list of all content model objects.
 *
 * @return array
 *   Associative array of pid => label pairs.
 */
function islandora_usage_stats_charts_get_cmodels() {
  $skip_these = array(
    'fedora-system:FedoraObject-3.0',
    'fedora-system:ServiceDefinition-3.0',
    'fedora-system:ContentModel-3.0',
    'fedora-system:ServiceDeployment-3.0',
  );

  // Query the rindex to get all cmodels.
  $tuque = islandora_get_tuque_connection();
  $ri_query = 'PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
select $object $label from <#ri>
     where { $object fedora-model:hasModel <info:fedora/fedora-system:ContentModel-3.0> ;
      fedora-model:label $label .}';
  $results = $tuque->repository->ri->sparqlQuery($ri_query, 'unlimited');
  $cmodels = array();
  foreach ($results as $member) {
    if (!in_array($member['object']['value'], $skip_these)) {
      $pid = preg_replace('/info:fedora\//', '', $member['object']['value']);
      $label = $member['label']['value'];
      $cmodels[$pid] = $label;
    }
  }
  return $cmodels;
}

/**
 * Checks to see if the current object has any of the allowed content models.
 *
 * @return array
 *   An array containing the content models that the object has that
 *   are allowed.
 */
function islandora_usage_stats_charts_check_for_cmodels($object) {
  $object_models = $object->models;
  $allowed_models = variable_get('islandora_usage_stats_charts_allowed_cmodels', array());
  $allowed_models_filtered = array();
  foreach ($allowed_models as $key => $value) {
    if ($value != '0') {
      $allowed_models_filtered[] = $value;
    }
  }
  return array_intersect($object_models, $allowed_models_filtered);
}
