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

  public function configurationForm() {
    $form['note'] = array(
      '#markup' => '<p>' . t('No configuration needed.') . '</p>',
    );
    return $form;
  }

  public function alterItems(array &$items) {
    // This only works with solr backend.
    foreach ($items as &$item) {
      try {
        $data = array(
          'field_event_date' => $item->field_event_date['und'],
        );
        if (!$data) {
          $item->search_api_viewed = NULL;
          continue;
        }
        $item->search_api_viewed = $data;
      }
      catch (Exception $e) {
        $item->search_api_viewed = NULL;
      }
    }

  }

  public function propertyInfo() {
    return array(
      'search_api_event_data' => array(
        'label' => t('Event data'),
        'description' => t('Data for the event API.'),
        'type' => 'text',
      ),
    );
  }

}
