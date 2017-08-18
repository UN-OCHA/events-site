<?php
/**
 * @file
 * events_event.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function events_event_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: user-menu_add-event:node/add/ev-event.
  $menu_links['user-menu_add-event:node/add/ev-event'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'node/add/ev-event',
    'router_path' => 'node/add/ev-event',
    'link_title' => 'Add event',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'user-menu_add-event:node/add/ev-event',
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'en',
    'menu_links_customized' => 1,
  );
  // Exported menu link: user-menu_log-out:user/logout.
  $menu_links['user-menu_log-out:user/logout'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user/logout',
    'router_path' => 'user/logout',
    'link_title' => 'Log out',
    'options' => array(
      'identifier' => 'user-menu_log-out:user/logout',
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'language' => 'en',
    'menu_links_customized' => 1,
  );
  // Exported menu link: user-menu_login:login.
  $menu_links['user-menu_login:login'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'login',
    'router_path' => 'login',
    'link_title' => 'Login',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'user-menu_login:login',
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'language' => 'en',
    'menu_links_customized' => 1,
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Add event');
  t('Log out');
  t('Login');

  return $menu_links;
}
