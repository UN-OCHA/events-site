<?php
/**
 * @file
 * Code for Events migration from HRInfo.
 */

/**
 * Migrate events.
 */
function ev_migrate_events_from_hrinfo() {
  if (variable_get('ev_migrate_stop_processing_hrinfo', FALSE)) {
    ev_migrate_log('Kill switch is enabled!');
    return;
  }

  ev_migrate_log('Getting page ' . variable_get('ev_migrate_page_hrinfo', 0));

  $page = variable_get('ev_migrate_page_hrinfo', 0);
  $api_url = variable_get('ev_migrate_api_url_hrinfo', '');
  $shared_secret = variable_get('ev_migrate_shared_secret_hrinfo', 'ThisIsMySecret');
  $timestamp = REQUEST_TIME;

  // Build hash.
  $hash = md5($shared_secret . '_' . $page . '_' . $timestamp);

  // Build URL;
  $url = $api_url;
  $url .= '/' . $page;
  $url .= '/' . $timestamp;
  $url .= '/' . $hash;

  // Get data.
  $request = drupal_http_request($url);
  if (isset($request->data)) {
    $data = drupal_json_decode($request->data);
    if (count($data)) {
      foreach ($data as $row) {
        ev_migrate_process_row_from_hrinfo($row, 'hrinfo-');
      }

      // Increment page.
      variable_set('ev_migrate_page_hrinfo', $page + 1);

      // Update last sync.
      $source_term = ev_migrate_get_term('HumanitarianResponse.info', 'ev_source');
      if ($source_term) {
        $source_term->field_source_last_sync[LANGUAGE_NONE][0]['value'] = REQUEST_TIME;
        taxonomy_term_save($source_term);
      }

      return;
    }
    else {
      _ev_migrate_send_chat_message_line('No more events to import, sleeping for a while', array('hrinfo'));
    }
  }

  variable_set('ev_migrate_stop_processing_hrinfo', TRUE);
};

/**
 * Process event data.
 *
 * @SuppressWarnings(PHPMD)
 */
function ev_migrate_process_row_from_hrinfo($row, $prefix = '') {
  $needs_update = FALSE;

  ev_migrate_log('Migrating ' . $row['url']);

  // See if the node already exists.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'ev_event', '=');
  $query->fieldCondition('field_event_external_id', 'value', $prefix . $row['nid'], '=');
  $result = $query->execute();

  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
    $nid = reset($nids);
    $event = node_load($nid);
    $event->log = 'Event imported from ' . $row['url'];
    $event->do_not_send_email = TRUE;
  }
  else {
    // New events alsways needs an update.
    $needs_update = TRUE;

    // Make sure we have a user.
    $account = user_load_by_mail($row['author']['email']);
    if (!$account) {
      // Check if there's a user by the same name.
      if (user_load_by_name(!empty($row['author']['name']) ? $row['author']['name'] : $row['author']['email'])) {
        $row['author']['name'] = !empty($row['author']['name']) ? $row['author']['name'] : $row['author']['email'];
        $row['author']['name'] .= '_' . REQUEST_TIME;
      }

      // Cleanup init.
      $row['author']['init'] = str_replace('oauthconnector_hid_oauth__', '', $row['author']['init']);
      if (empty($row['author']['init'])) {
        $row['author']['init'] = $row['author']['email'];
      }

      $row['author']['name'] = !empty($row['author']['name']) ? $row['author']['name'] : $row['author']['email'];
      if (strlen($row['author']['name']) > 60) {
        $row['author']['name'] = substr($row['author']['name'], 0, 60);
      }

      // Create user object.
      $account = array(
        'name' => $row['author']['name'],
        'mail' => $row['author']['email'],
        'status' => 1,
        'init' => $row['author']['init'],
        'language' => $row['author']['language'],
        'timezone' => $row['author']['timezone'],
      );
      $account = user_save(NULL, $account);

      // Save the HID association with hybridauth.
      $hdata = array(
        'provider' => 'HumanitarianId',
        'identifier' => $row['author']['init'],
      );

      // Ignore errors.
      try {
        _hybridauth_identity_save($hdata, $account->uid);
      }
      catch (Exception $e) {
        // Ignore it, not much we can do.
      }
    }

    // Create a new event.
    $event = new stdClass();
    $event->type = 'ev_event';
    $event->status = 1;
    node_object_prepare($event);

    $event->language = 'en';
    $event->uid = $account->uid;
    $event->status = TRUE;
    $event->promote = FALSE;
    $event->comment = FALSE;
    $event->log = 'Event imported from ' . $row['url'];
    $event->field_event_source_created[LANGUAGE_NONE][0]['value'] = REQUEST_TIME;
  }

  $event->revision = TRUE;

  $needs_update = $needs_update || $event->title != $row['title'];
  $event->title = $row['title'];

  $needs_update = $needs_update || $event->created != $row['created'];
  $event->field_event_source_created[LANGUAGE_NONE][0]['value'] = $row['created'];
  $event->created = $row['created'];

  $needs_update = $needs_update || $event->changed != $row['changed'];
  $event->field_event_source_updated[LANGUAGE_NONE][0]['value'] = $row['changed'];
  $event->changed = $row['changed'];

  if (strlen($row['url']) > 255) {
    $parsed_url = parse_url($row['url']);
    $row['url'] = $parsed_url['scheme'] . '://' . $parsed_url['host'] . '/node/' . $row['nid'];
  }
  $needs_update = $needs_update || !isset($event->field_event_source_url[LANGUAGE_NONE][0]['value']) || $event->field_event_source_url[LANGUAGE_NONE][0]['value'] != $row['url'];
  $event->field_event_source_url[LANGUAGE_NONE][0]['value'] = $row['url'];

  $source_term = ev_migrate_get_term('HumanitarianResponse.info', 'ev_source');
  $event->field_event_source[LANGUAGE_NONE][0]['tid'] = $source_term->tid;

  $event->ev_migrate_created = $event->created;
  $event->ev_migrate_changed = $event->changed;

  $date_data = array();
  if (!empty($row['date']['rrule'])) {
    $field_info = field_info_field('field_event_date');
    // Double check date format used.
    if (strpos(date_type_format($field_info['type']), 'T') !== FALSE) {
      $row['date']['value'] = str_replace(' ', 'T', $row['date']['value']);
      $row['date']['value2'] = str_replace(' ', 'T', $row['date']['value2']);
    }

    // Build all values.
    $date_data = date_repeat_build_dates($row['date']['rrule'], NULL, $field_info, $row['date']);
  }
  else {
    $date_data[0] = $row['date'];
  }

  if (isset($event->field_event_date[LANGUAGE_NONE][0]['value'])) {
    $needs_update = $needs_update || $event->field_event_date[LANGUAGE_NONE][0]['value'] != $date_data[0]['value'];
    $needs_update = $needs_update || $event->field_event_date[LANGUAGE_NONE][0]['value2'] != $date_data[0]['value2'];
    $needs_update = $needs_update || $event->field_event_date[LANGUAGE_NONE][0]['rrule'] != $date_data[0]['rrule'];
  }
  $event->field_event_date[LANGUAGE_NONE] = $date_data;

  // Store external nid.
  $needs_update = $needs_update || $event->field_event_external_id[LANGUAGE_NONE][0]['value'] != $prefix . $row['nid'];
  $event->field_event_external_id[LANGUAGE_NONE][0]['value'] = $prefix . $row['nid'];

  if (isset($row['description']['value'])) {
    $needs_update = $needs_update || $event->field_event_info[LANGUAGE_NONE][0]['value'] != $row['description']['value'];
    $event->field_event_info[LANGUAGE_NONE][] = array(
      'value' => $row['description']['value'],
      'format' => 'markdown',
    );
  }

  if (isset($row['address']) && isset($row['address']['locality'])) {
    $needs_update = $needs_update || $event->field_event_location_details[LANGUAGE_NONE][0]['value'] != $row['address']['locality'];
    $event->field_event_location_details[LANGUAGE_NONE][]['value'] = $row['address']['locality'];
  }

  if (isset($row['category']) && $category = ev_migrate_get_term($row['category'], 'ev_category')) {
    $needs_update = $needs_update || $event->field_event_category[LANGUAGE_NONE][0]['tid'] != $category->tid;
    $event->field_event_category[LANGUAGE_NONE][]['tid'] = $category->tid;
  }

  if (isset($row['location']) && $location = ev_migrate_get_term($row['location'], 'ev_location')) {
    $needs_update = $needs_update || $event->field_event_location[LANGUAGE_NONE][0]['tid'] != $location->tid;
    $event->field_event_location[LANGUAGE_NONE][]['tid'] = $location->tid;
  }

  if (isset($row['themes']) && is_array($row['themes'])) {
    $current_original_theme = isset($event->field_event_original_theme[LANGUAGE_NONE][0]['value']) ? $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] : '';

    $event->field_event_theme[LANGUAGE_NONE] = array();
    $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] = '';
    foreach ($row['themes'] as $name) {
      if ($theme = ev_migrate_get_term($name, 'ev_theme')) {
        $event->field_event_theme[LANGUAGE_NONE][]['tid'] = $theme->tid;
      }
      else {
        if (!isset($event->field_event_original_theme[LANGUAGE_NONE][0]['value'])) {
          $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] = $name;
        }
        else {
          $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] .= ', ' . $name;
        }
      }
    }

    // Limit to 250 characters.
    if (isset($event->field_event_original_theme[LANGUAGE_NONE][0]['value'])) {
      $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] = substr($event->field_event_original_theme[LANGUAGE_NONE][0]['value'], 0, 250);
    }

    $needs_update = $needs_update || $event->field_event_original_theme[LANGUAGE_NONE][0]['value'] != $current_original_theme;
  }

  if (isset($row['organizations']) && is_array($row['organizations'])) {
    $current_organization = isset($event->field_event_organization[LANGUAGE_NONE]) ? $event->field_event_organization[LANGUAGE_NONE] : array();
    $event->field_event_organization[LANGUAGE_NONE] = array();
    foreach ($row['organizations'] as $name) {
      if ($organization = ev_migrate_get_term($name, 'ev_organization')) {
        $event->field_event_organization[LANGUAGE_NONE][]['tid'] = $organization->tid;
      }
    }
    $needs_update = $needs_update || $event->field_event_organization[LANGUAGE_NONE] != $current_organization;
  }

  if (isset($row['disasters']) && is_array($row['disasters'])) {
    $current_disaster = isset($event->field_event_disasters[LANGUAGE_NONE]) ? $event->field_event_disasters[LANGUAGE_NONE] : array();
    $event->field_event_disasters[LANGUAGE_NONE] = array();
    foreach ($row['disasters'] as $name) {
      if ($disaster = ev_migrate_get_term($name, 'ev_disaster')) {
        $event->field_event_disasters[LANGUAGE_NONE][]['tid'] = $disaster->tid;
      }
    }
    $needs_update = $needs_update || $event->field_event_disasters[LANGUAGE_NONE] != $current_disaster;
  }

  $cluster_tids = array();
  if (isset($row['sectors']) && is_array($row['sectors'])) {
    foreach ($row['sectors'] as $name) {
      if ($sector = ev_migrate_get_term($name, 'ev_list')) {
        $cluster_tids[] = $sector->tid;
      }
      elseif (isset($row['location']) && $sector = ev_migrate_get_term($row['location'] . ': ' . $name, 'ev_list')) {
        $cluster_tids[] = $sector->tid;
      }
      elseif (isset($row['country']) && $sector = ev_migrate_get_term($row['country'] . ': ' . $name, 'ev_list')) {
        $cluster_tids[] = $sector->tid;
      }
    }
  }

  if (isset($row['bundles']) && is_array($row['bundles'])) {
    foreach ($row['bundles'] as $name) {
      if ($bundle = ev_migrate_get_term($name, 'ev_list')) {
        $cluster_tids[] = $bundle->tid;
      }
      elseif (isset($row['location']) && $bundle = ev_migrate_get_term($row['location'] . ': ' . $name, 'ev_list')) {
        $cluster_tids[] = $bundle->tid;
      }
      elseif (isset($row['country']) && $bundle = ev_migrate_get_term($row['country'] . ': ' . $name, 'ev_list')) {
        $cluster_tids[] = $bundle->tid;
      }
    }
  }

  if (!empty($cluster_tids)) {
    $current_cluster_tids = isset($event->field_event_cluster[LANGUAGE_NONE]) ? $event->field_event_cluster[LANGUAGE_NONE] : array();
    $event->field_event_cluster[LANGUAGE_NONE] = array();
    // Remove duplicates.
    $cluster_tids = array_unique($cluster_tids);
    foreach ($cluster_tids as $cluster_tid) {
      $event->field_event_cluster[LANGUAGE_NONE][]['tid'] = $cluster_tid;
    }
    $needs_update = $needs_update || $event->field_event_cluster[LANGUAGE_NONE] != $current_cluster_tids;
  }

  if (isset($row['coordination_hubs']) && is_array($row['coordination_hubs'])) {
    $current_coordination_hub = isset($event->field_event_coordination_hub[LANGUAGE_NONE]) ? $event->field_event_coordination_hub[LANGUAGE_NONE] : array();
    $event->field_event_coordination_hub[LANGUAGE_NONE] = array();
    foreach ($row['coordination_hubs'] as $name) {
      if ($coordination_hub = ev_migrate_get_term($name, 'ev_coordination_hub')) {
        $event->field_event_coordination_hub[LANGUAGE_NONE][]['tid'] = $coordination_hub->tid;
      }
    }
    $needs_update = $needs_update || $event->field_event_coordination_hub[LANGUAGE_NONE] != $current_coordination_hub;
  }

  $current_documents = isset($event->field_event_documents[LANGUAGE_NONE]) ? $event->field_event_documents[LANGUAGE_NONE] : array();
  if (isset($row['event_meeting_minutes']) && is_array($row['event_meeting_minutes'])) {
    $meeting_minute_term_info = events_config_get_term('Meeting Minutes', 'ev_document_type');
    $meeting_minute_tid = $meeting_minute_term_info['term']->tid;
    foreach ($row['event_meeting_minutes'] as $event_meeting_minute) {
      $fid = ev_migrate_fetch_and_save_file($event_meeting_minute['name'], $event_meeting_minute['url'], $event->created);
      if ($fid) {
        $event->field_event_documents[LANGUAGE_NONE][0]['field_document_type'][LANGUAGE_NONE][0]['tid'] = $meeting_minute_tid;
        $event->field_event_documents[LANGUAGE_NONE][0]['field_document_file'][LANGUAGE_NONE][0] = array(
          'fid' => $fid,
          'display' => 1,
          'description' => $event_meeting_minute['name'],
        );
      }
    }
  }

  if (isset($row['event_agenda']) && is_array($row['event_agenda'])) {
    $meeting_minute_term_info = events_config_get_term('Agenda', 'ev_document_type');
    $meeting_minute_tid = $meeting_minute_term_info['term']->tid;
    foreach ($row['event_agenda'] as $event_meeting_minute) {
      $fid = ev_migrate_fetch_and_save_file($event_meeting_minute['name'], $event_meeting_minute['url'], $event->created);
      if ($fid) {
        $event->field_event_documents[LANGUAGE_NONE][0]['field_document_type'][LANGUAGE_NONE][0]['tid'] = $meeting_minute_tid;
        $event->field_event_documents[LANGUAGE_NONE][0]['field_document_file'][LANGUAGE_NONE][0] = array(
          'fid' => $fid,
          'display' => 1,
          'description' => $event_meeting_minute['name'],
        );
      }
    }
  }

  if (isset($event->field_event_documents[LANGUAGE_NONE])) {
    $needs_update = $needs_update || $event->field_event_documents[LANGUAGE_NONE] != $current_documents;
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
