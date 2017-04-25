<?php
/**
 * @file
 * events_event.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function events_event_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['views-10203b0dc686fbffa982397de03138aa'] = array(
    'cache' => -1,
    'custom' => 0,
    'delta' => '10203b0dc686fbffa982397de03138aa',
    'module' => 'views',
    'node_types' => array(),
    'pages' => '<front>',
    'roles' => array(),
    'themes' => array(
      'bartik' => array(
        'region' => 'sidebar_first',
        'status' => 1,
        'theme' => 'bartik',
        'weight' => 0,
      ),
      'ochabasic' => array(
        'region' => 'content',
        'status' => 1,
        'theme' => 'ochabasic',
        'weight' => -15,
      ),
      'seven' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'seven',
        'weight' => 0,
      ),
    ),
    'title' => 'Clusters',
    'visibility' => 1,
  );

  $export['views-72ae41f893bf853ab652d0c5379f5e4b'] = array(
    'cache' => -1,
    'custom' => 0,
    'delta' => '72ae41f893bf853ab652d0c5379f5e4b',
    'module' => 'views',
    'node_types' => array(),
    'pages' => '<front>',
    'roles' => array(),
    'themes' => array(
      'bartik' => array(
        'region' => 'sidebar_first',
        'status' => 1,
        'theme' => 'bartik',
        'weight' => 0,
      ),
      'ochabasic' => array(
        'region' => 'content',
        'status' => 1,
        'theme' => 'ochabasic',
        'weight' => -14,
      ),
      'seven' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'seven',
        'weight' => 0,
      ),
    ),
    'title' => 'Organizations',
    'visibility' => 1,
  );

  $export['views-bb7f2f19fb4857fac48b5b01f528920a'] = array(
    'cache' => -1,
    'custom' => 0,
    'delta' => 'bb7f2f19fb4857fac48b5b01f528920a',
    'module' => 'views',
    'node_types' => array(),
    'pages' => '<front>',
    'roles' => array(),
    'themes' => array(
      'bartik' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'bartik',
        'weight' => 0,
      ),
      'ochabasic' => array(
        'region' => 'content',
        'status' => 1,
        'theme' => 'ochabasic',
        'weight' => -13,
      ),
      'seven' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'seven',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  return $export;
}
