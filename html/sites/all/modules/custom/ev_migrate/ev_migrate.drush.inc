<?php
/**
 * @file
 * Code for Events migration.
 */

/**
 * Implements hook_drush_help().
 */
function ev_migrate_drush_help($section) {
  switch ($section) {
    case 'drush:ev-migrate':
      return dt('Migrate events from HR Info.');
  }
}

/**
 * Implements hook_drush_command().
 */
function ev_migrate_drush_command() {
  $items = array();

  $items['ev-migrate'] = array(
    'description' => 'Migrate events from HR Info.',
    'callback' => 'ev_migrate_events',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}
