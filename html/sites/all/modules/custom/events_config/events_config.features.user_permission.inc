<?php
/**
 * @file
 * events_config.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function events_config_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'use hybridauth'.
  $permissions['use hybridauth'] = array(
    'name' => 'use hybridauth',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'hybridauth',
  );

  return $permissions;
}
