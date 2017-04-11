<?php
/**
 * @file
 * events_event..inc
 */

/**
 * Implements hook_multifield_default_multifield().
 */
function events_event_multifield_default_multifield() {
  $export = array();

  $multifield = new stdClass();
  $multifield->disabled = FALSE; /* Edit this to true to make a default multifield disabled initially */
  $multifield->machine_name = 'field_event_documents';
  $multifield->label = 'field_event_documents';
  $multifield->description = '';
  $export['field_event_documents'] = $multifield;

  return $export;
}
