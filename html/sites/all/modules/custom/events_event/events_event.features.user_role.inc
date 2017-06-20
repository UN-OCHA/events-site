<?php
/**
 * @file
 * events_event.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function events_event_user_default_roles() {
  $roles = array();

  // Exported role: HID verified.
  $roles['HID verified'] = array(
    'name' => 'HID verified',
    'weight' => 3,
  );

  return $roles;
}
