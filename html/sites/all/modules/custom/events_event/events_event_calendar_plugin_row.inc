<?php
/**
 * @file
 * Contains the Calendar row style plugin.
 */

// @codingStandardsIgnoreFile

/**
 * Plugin which creates a view on the resulting object
 * and formats it as a Calendar node.
 */
class events_event_calendar_plugin_row extends calendar_plugin_row {

  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);
  }

   /**
   * Create a stripe based on a taxonomy term.
   */
  function calendar_taxonomy_stripe(&$result) {
    $entity = $result->entity;
    $term_field_name = $this->options['colors']['taxonomy_field'];
    if ($terms_for_entity = field_get_items($this->view->base_table, $entity, $term_field_name)) {
      foreach ($terms_for_entity as $delta => $item) {
        $term_for_entity = taxonomy_term_load($item['tid']);
        if ($term_for_entity) {
          if (isset($term_for_entity->field_ev_legend_color['und'][0]['rgb'])) {
            $result->stripe[] = $term_for_entity->field_ev_legend_color['und'][0]['rgb'];
          }
          else {
            $result->stripe[] = sprintf('#%06X', mt_rand(0, 0xFFFFFF));
          }
          $result->stripe_label[] = $term_for_entity->name;
        }
      }
    }
  }

}
