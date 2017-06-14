<?php
/**
 * @file
 * events_event.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function events_event_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|ev_event|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'ev_event';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'field_event_organization' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Organization(s)',
        ),
      ),
    ),
    'field_event_cluster' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Cluster(s)',
        ),
      ),
    ),
    'field_event_info' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Additional information',
        ),
      ),
    ),
  );
  $export['node|ev_event|default'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function events_event_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|ev_event|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'ev_event';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'ev_page_heading',
      ),
      'left' => array(
        1 => 'ev_display_date_time',
        2 => 'field_event_cluster',
        3 => 'field_event_location',
        4 => 'field_event_location_details',
        5 => 'field_event_info',
      ),
      'right' => array(
        6 => 'field_event_category',
        7 => 'field_event_documents',
        8 => 'field_event_organization',
        9 => 'field_event_coordination_hub',
        10 => 'field_event_disasters',
      ),
    ),
    'fields' => array(
      'ev_page_heading' => 'header',
      'ev_display_date_time' => 'left',
      'field_event_cluster' => 'left',
      'field_event_location' => 'left',
      'field_event_location_details' => 'left',
      'field_event_info' => 'left',
      'field_event_category' => 'right',
      'field_event_documents' => 'right',
      'field_event_organization' => 'right',
      'field_event_coordination_hub' => 'right',
      'field_event_disasters' => 'right',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => 1,
  );
  $export['node|ev_event|default'] = $ds_layout;

  return $export;
}
