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

        $events = events_event_build_api_structure($item);

        db_merge('events_event_data')
          ->key(array('nid' => $node->nid))
          ->fields(array(
            'data' => serialize($events),
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
