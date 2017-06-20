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
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own ev_event content'.
  $permissions['delete own ev_event content'] = array(
    'name' => 'delete own ev_event content',
    'roles' => array(
      'HID verified' => 'HID verified',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own ev_event content'.
  $permissions['edit own ev_event content'] = array(
    'name' => 'edit own ev_event content',
    'roles' => array(
      'HID verified' => 'HID verified',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view own unpublished content'.
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      'HID verified' => 'HID verified',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
