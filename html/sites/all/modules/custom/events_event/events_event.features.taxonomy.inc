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
    'ev_category' => array(
      'name' => 'Category',
      'machine_name' => 'ev_category',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'ev_coordination_hub' => array(
      'name' => 'Coordination Hub',
      'machine_name' => 'ev_coordination_hub',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'ev_disaster' => array(
      'name' => 'Disaster',
      'machine_name' => 'ev_disaster',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
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
    'ev_location' => array(
      'name' => 'Location',
      'machine_name' => 'ev_location',
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
    'ev_theme' => array(
      'name' => 'Theme',
      'machine_name' => 'ev_theme',
      'description' => 'Themes from ReliefWeb',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
