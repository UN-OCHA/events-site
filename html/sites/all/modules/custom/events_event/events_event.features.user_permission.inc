<?php
/**
 * @file
 * events_event.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function events_event_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create ev_event content'.
  $permissions['create ev_event content'] = array(
    'name' => 'create ev_event content',
    'roles' => array(
      'HID verified' => 'HID verified',
      'IMO' => 'IMO',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own ev_event content'.
  $permissions['delete own ev_event content'] = array(
    'name' => 'delete own ev_event content',
    'roles' => array(
      'HID verified' => 'HID verified',
      'IMO' => 'IMO',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any ev_event content'.
  $permissions['edit any ev_event content'] = array(
    'name' => 'edit any ev_event content',
    'roles' => array(
      'IMO' => 'IMO',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own ev_event content'.
  $permissions['edit own ev_event content'] = array(
    'name' => 'edit own ev_event content',
    'roles' => array(
      'HID verified' => 'HID verified',
      'IMO' => 'IMO',
    ),
    'module' => 'node',
  );

  // Exported permission: 'use text format filtered_html'.
  $permissions['use text format filtered_html'] = array(
    'name' => 'use text format filtered_html',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format full_html'.
  $permissions['use text format full_html'] = array(
    'name' => 'use text format full_html',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format markdown'.
  $permissions['use text format markdown'] = array(
    'name' => 'use text format markdown',
    'roles' => array(
      'HID verified' => 'HID verified',
      'IMO' => 'IMO',
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'view own unpublished content'.
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      'HID verified' => 'HID verified',
      'IMO' => 'IMO',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
