<?php
/**
 * @file
 * Code for Events migration from ReliefWeb.
 */

/**
 * Fetch documents from Reliefweb.
 */
function _ev_migrate_reliefweb_fetch($limit = 0, $offset = 0) {
  $endpoint = 'https://api.reliefweb.int/v1/training';
  $params = array(
    'appname' => 'hrinfo',
    'offset' => $offset,
    'limit' => $limit,
    'preset' => 'latest',

    'fields[include]' => array(
      'id',
      'url',
      'title',
      'date',
      'body',
      'country',
      'file',
      'language.code',
      'source',
      'theme',
      'type',
      'city',
    ),
  );

  $url = $endpoint . '?' . drupal_http_build_query($params);

  ev_migrate_log('Getting data from ' . $url);

  $request = drupal_http_request($url);
  if (isset($request->data)) {
    $response = drupal_json_decode($request->data);
    return $response['data'];
  }

  return array();
}

/**
 * Process event data.
 */
function ev_migrate_process_row_reliefweb($row, $prefix = '') {
  $needs_update = FALSE;

  ev_migrate_log('Migrating ' . $row['href']);

  // See if the node already exists.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'ev_event', '=');
  $query->fieldCondition('field_event_external_id', 'value', $prefix . $row['id'], '=');
  $result = $query->execute();

  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
    $nid = reset($nids);
    $event = node_load($nid);
    $event->log = 'Event imported from ' . $row['href'];
    $event->do_not_send_email = TRUE;
  }
  else {
    // New events always needs an update.
    $needs_update = TRUE;

    // Create a new event.
    $event = new stdClass();
    $event->type = 'ev_event';
    $event->status = 1;
    node_object_prepare($event);

    $event->language = 'en';
    $event->uid = 0;
    $event->status = TRUE;
    $event->promote = FALSE;
    $event->comment = FALSE;
    $event->log = 'Event imported from ' . $row['href'];
    $event->field_event_source_created[LANGUAGE_NONE][0]['value'] = REQUEST_TIME;
  }

  $event->revision = TRUE;

  $needs_update = $needs_update || $event->title != $row['fields']['title'];
  $event->title = $row['fields']['title'];

  $needs_update = $needs_update || $event->created != strtotime($row['fields']['date']['created']);
  $event->field_event_source_created[LANGUAGE_NONE][0]['value'] = strtotime($row['fields']['date']['created']);
  $event->created = strtotime($row['fields']['date']['created']);

  $needs_update = $needs_update || $event->changed != strtotime($row['fields']['date']['changed']);
  $event->field_event_source_updated[LANGUAGE_NONE][0]['value'] = strtotime($row['fields']['date']['changed']);
  $event->changed = strtotime($row['fields']['date']['changed']);

  $needs_update = $needs_update || !isset($event->field_event_source_url[LANGUAGE_NONE][0]['value']) || $event->field_event_source_url[LANGUAGE_NONE][0]['value'] != $row['fields']['url'];
  $event->field_event_source_url[LANGUAGE_NONE][0]['value'] = $row['fields']['url'];

  $source_term = ev_migrate_get_term('ReliefWeb.int', 'ev_source');
  $event->field_event_source[LANGUAGE_NONE][0]['tid'] = $source_term->tid;

  $event->ev_migrate_created = $event->created;
  $event->ev_migrate_changed = $event->changed;

  $needs_update = $needs_update || $event->field_event_date[LANGUAGE_NONE][0]['value'] != substr($row['fields']['date']['start'], 0, 19);
  $event->field_event_date[LANGUAGE_NONE][0]['value'] = substr($row['fields']['date']['start'], 0, 19);
  $needs_update = $needs_update || $event->field_event_date[LANGUAGE_NONE][0]['value2'] != substr($row['fields']['date']['end'], 0, 19);
  $event->field_event_date[LANGUAGE_NONE][0]['value2'] = substr($row['fields']['date']['end'], 0, 19);

  // Store external nid.
  $needs_update = $needs_update || $event->field_event_external_id[LANGUAGE_NONE][0]['value'] != $prefix . $row['id'];
  $event->field_event_external_id[LANGUAGE_NONE][0]['value'] = $prefix . $row['id'];

  // They are all trainings.
  if ($category = ev_migrate_get_term('Training', 'ev_category')) {
    $needs_update = $needs_update || $event->field_event_category[LANGUAGE_NONE][0]['tid'] != $category->tid;
    $event->field_event_category[LANGUAGE_NONE][0]['tid'] = $category->tid;
  }

  if (isset($row['fields']['body'])) {
    $needs_update = $needs_update || $event->field_event_info[LANGUAGE_NONE][0]['value'] != $row['fields']['body'];
    $event->field_event_info[LANGUAGE_NONE][] = array(
      'value' => $row['fields']['body'],
      'format' => 'markdown',
    );
  }

  if (isset($row['fields']['city'][0]['name'])) {
    $needs_update = $needs_update || $event->field_event_location_details[LANGUAGE_NONE][0]['value'] != $row['fields']['city'][0]['name'];
    $event->field_event_location_details[LANGUAGE_NONE][]['value'] = $row['fields']['city'][0]['name'];
  }

  if (isset($row['fields']['country'][0]['name']) && $location = ev_migrate_get_term($row['fields']['country'][0]['name'], 'ev_location')) {
    $needs_update = $needs_update || $event->field_event_location[LANGUAGE_NONE][0]['tid'] != $location->tid;
    $event->field_event_location[LANGUAGE_NONE][]['tid'] = $location->tid;
  }

  if (isset($row['fields']['theme']) && is_array($row['fields']['theme'])) {
    $current_original_theme = isset($event->field_event_original_theme[LANGUAGE_NONE][0]['value']) ? $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] : '';

    $event->field_event_theme[LANGUAGE_NONE] = array();
    $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] = '';
    foreach ($row['fields']['theme'] as $name) {
      if ($theme = ev_migrate_get_term($name['name'], 'ev_theme')) {
        $event->field_event_theme[LANGUAGE_NONE][]['tid'] = $theme->tid;
      }
      else {
        if (!isset($event->field_event_original_theme[LANGUAGE_NONE][0]['value'])) {
          $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] = $name['name'];
        }
        else {
          $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] .= ', ' . $name['name'];
        }
      }
    }

    // Limit to 250 characters.
    if (isset($event->field_event_original_theme[LANGUAGE_NONE][0]['value'])) {
      $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] = substr($event->field_event_original_theme[LANGUAGE_NONE][0]['value'], 0, 250);
    }

    $needs_update = $needs_update || $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] != $current_original_theme;
  }

  if ($needs_update) {
    $event = node_submit($event);
    node_save($event);

    ev_migrate_log('Events saved with id ' . $event->nid);
  }
  else {
    ev_migrate_log('No need to save event with id ' . $event->nid);
  }

}
