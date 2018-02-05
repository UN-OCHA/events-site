<?php
/**
 * @file
 * Predefined environment indicators.
 */

/**
 * Implements hook_default_environment_indicator_environment().
 */
function events_config_default_environment_indicator_environment() {
  $export = array();

  $environment = new stdClass();
  $environment->disabled = FALSE; /* Edit this to true to make a default environment disabled initially */
  $environment->api_version = 1;
  $environment->machine = 'default_environment';
  $environment->name = 'Default environment';
  $environment->regexurl = '.*';
  $environment->settings = array(
    'weight' => 10,
    'color' => '#000000',
    'text_color' => '#ffffff',
    'weight' => 99,
    'position' => 'top',
    'fixed' => 0,
  );
  $export['default_environment'] = $environment;

  $environment = new stdClass();
  $environment->disabled = FALSE; /* Edit this to true to make a default environment disabled initially */
  $environment->api_version = 1;
  $environment->machine = 'local';
  $environment->name = 'Local';
  $environment->regexurl = '.*local.*';
  $environment->settings = array(
    'weight' => 10,
    'color' => '#009933',
    'text_color' => '#ffffff',
    'weight' => 10,
    'position' => 'top',
    'fixed' => 0,
  );
  $export['local'] = $environment;

  $environment = new stdClass();
  $environment->disabled = FALSE; /* Edit this to true to make a default environment disabled initially */
  $environment->api_version = 1;
  $environment->machine = 'staging';
  $environment->name = 'Staging';
  $environment->regexurl = '.*staging.*';
  $environment->settings = array(
    'weight' => 10,
    'color' => '#cb00cd',
    'text_color' => '#ffffff',
    'weight' => 50,
    'position' => 'top',
    'fixed' => 0,
  );
  $export['staging'] = $environment;

  return $export;
}
