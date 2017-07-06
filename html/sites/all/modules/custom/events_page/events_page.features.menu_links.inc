<?php
/**
 * @file
 * events_page.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function events_page_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-footer-menu_about:about.
  $menu_links['menu-footer-menu_about:about'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => 'about',
    'router_path' => '',
    'link_title' => 'About',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-footer-menu_about:about',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: menu-footer-menu_code-of-conduct:code-of-conduct.
  $menu_links['menu-footer-menu_code-of-conduct:code-of-conduct'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => 'code-of-conduct',
    'router_path' => '',
    'link_title' => 'Code of conduct',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-footer-menu_code-of-conduct:code-of-conduct',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-footer-menu_privacy-policy:https://www.un.org/en/sections/about-website/privacy-notice.
  $menu_links['menu-footer-menu_privacy-policy:https://www.un.org/en/sections/about-website/privacy-notice'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => 'https://www.un.org/en/sections/about-website/privacy-notice',
    'router_path' => '',
    'link_title' => 'Privacy policy',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-footer-menu_privacy-policy:https://www.un.org/en/sections/about-website/privacy-notice',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('About');
  t('Code of conduct');
  t('Privacy policy');

  return $menu_links;
}
