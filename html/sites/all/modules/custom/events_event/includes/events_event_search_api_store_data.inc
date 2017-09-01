<?php

/**
 * @file
 * Contains EventsEventSearchApiStoreData.
 */

/**
 * Search API data alteration to store all data for API.
 */
class EventsEventSearchApiStoreData extends SearchApiAbstractAlterCallback {

  /**
   * Only support indexes containing entities.
   *
   * @see SearchApiAlterCallbackInterface::supportsIndex()
   */
  public function supportsIndex(SearchApiIndex $index) {
    return $index->machine_name === 'un_events';
  }

  /**
   * Configuration form.
   *
   * @see SearchApiAlterCallbackInterface::configurationForm()
   */
  public function configurationForm() {
    $form['note'] = array(
      '#markup' => '<p>' . t('No configuration needed.') . '</p>',
    );
    return $form;
  }

  /**
   * Alter items during indexing.
   *
   * @see SearchApiAlterCallbackInterface::alterItems()
   */
  public function alterItems(array &$items) {
    foreach ($items as &$item) {
      try {

        $node = $item;
        $timezone = 'UTC';

        foreach ($node->field_event_date['und'] as $date) {
          $date_start = new DateTime($date['value'], timezone_open($date['timezone_db']));
          $date_start->setTimezone(timezone_open($timezone));
          $date_end = new DateTime($date['value2'], timezone_open($date['timezone_db']));
          $date_end->setTimezone(timezone_open($timezone));

          $event = array(
            'id' => $node->nid,
            'title' => $node->title,
            'start' => $date_start->format(DATE_FORMAT_DATETIME),
            'end' => $date_end->format(DATE_FORMAT_DATETIME),
            'start_raw' => $date['value'],
            'end_raw' => $date['value2'],
            'timezone_db' => $date['timezone_db'],
            'allDay' => $date['value'] == $date['value2'],
            'url' => url('node/' . $node->nid, array('absolute' => TRUE)),
            'backgroundColor' => events_event_get_event_color($node),
            'location' => events_event_get_event_locations($node),
            'locationDetails' => isset($node->field_event_location_details['und'][0]['value']) ? $node->field_event_location_details['und'][0]['value'] : '',
            'description' => isset($node->field_event_info['und'][0]['safe_value']) ? $node->field_event_info['und'][0]['safe_value'] : '',
          );

          foreach (events_event_exported_term_fields() as $term_field => $term_field_info) {
            $event[$term_field_info['short_key']] = array();
            if (isset($node->{$term_field}['und'][0]['tid'])) {
              foreach ($node->{$term_field}['und'] as $child) {
                // Add parent tid.
                if ($term_field_info['has_parents']) {
                  $term = taxonomy_term_load($child['tid']);
                  $parents = explode('|', $term->field_parent_tids['und'][0]['value']);
                  foreach ($parents as $parent) {
                    $event[$term_field_info['short_key']][] = $parent;
                  }
                }
                else {
                  $event[$term_field_info['short_key']][] = $child['tid'];
                }
              }
              $event[$term_field_info['short_key']] = array_unique($event[$term_field_info['short_key']]);
            }
            elseif (isset($term_field_info['node_field'])) {
              $document_types = events_event_document_types();
              $node_field = $term_field_info['node_field'];
              $multifield_field = $term_field_info['multifield_field'];
              if (isset($node->{$node_field}['und'][0][$multifield_field]['und'])) {
                foreach ($node->{$node_field}['und'][0][$multifield_field]['und'] as $key => $multifield) {
                  $event[$term_field_info['short_key']][] = $multifield['tid'];
                }
                // Avoid duplicates.
                $event[$term_field_info['short_key']] = array_unique($event[$term_field_info['short_key']]);

                // Add documents.
                foreach ($node->{$node_field}['und'][0]['field_document_file']['und'] as $key => $multifield_file) {
                  $event['files'][] = array(
                    'uri' => file_create_url($multifield_file['uri'], array('absolute' => TRUE)),
                    'name' => !empty($multifield_file['description']) ? $multifield_file['description'] : $multifield_file['origname'],
                    'type' => $node->{$node_field}['und'][0][$multifield_field]['und'][$key]['tid'],
                    'type_human' => isset($node->{$node_field}['und'][0][$multifield_field]['und'][$key]['tid']) ? $document_types[$node->{$node_field}['und'][0][$multifield_field]['und'][$key]['tid']]['name'] : '',
                  );
                }
              }
            }
          }
        }

        db_merge('events_event_data')
          ->key(array('nid' => $node->nid))
          ->fields(array(
            'data' => serialize($event),
          ))
          ->execute();

        $item->search_api_event_data = TRUE;
      }
      catch (Exception $e) {
        $item->search_api_event_data = FALSE;
      }
    }
  }

  /**
   * Property Info.
   *
   * @see SearchApiAlterCallbackInterface::propertyInfo()
   */
  public function propertyInfo() {
    return array(
      'search_api_event_data' => array(
        'label' => t('Event data'),
        'description' => t('Flag for the event API.'),
        'type' => 'boolean',
      ),
    );
  }

}
