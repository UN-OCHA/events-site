<?php
/**
 * @file
 * events_event.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function events_event_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'ev_event_filters';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_un_events';
  $view->human_name = 'Event filters';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    1 => '1',
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Indexed Node: Node ID */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'search_api_index_un_events';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';

  /* Display: Facets block */
  $handler = $view->new_display('search_api_views_facets_block', 'Facets block', 'search_api_views_facets_block_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['block_description'] = 'Organisations';
  $handler->display->display_options['facet_field'] = 'field_event_organization';

  /* Display: Facets block 2 */
  $handler = $view->new_display('search_api_views_facets_block', 'Facets block 2', 'search_api_views_facets_block_2');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['block_description'] = 'Clusters';
  $handler->display->display_options['facet_field'] = 'field_event_cluster';
  $translatables['ev_event_filters'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Node ID'),
    t('.'),
    t(','),
    t('Facets block'),
    t('Organisations'),
    t('Facets block 2'),
    t('Clusters'),
  );
  $export['ev_event_filters'] = $view;

  return $export;
}
