<?php
/**
 * @file
 * events_event.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function events_event_taxonomy_default_vocabularies() {
  return array(
    'ev_document_type' => array(
      'name' => 'Document type',
      'machine_name' => 'ev_document_type',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'ev_list' => array(
      'name' => 'List',
      'machine_name' => 'ev_list',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'ev_organization' => array(
      'name' => 'Organization',
      'machine_name' => 'ev_organization',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
