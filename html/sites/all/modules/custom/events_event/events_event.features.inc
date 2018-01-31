<?php
/**
 * @file
 * events_event.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function events_event_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "facetapi" && $api == "facetapi_defaults") {
    return array("version" => "1");
  }
  if ($module == "multifield" && $api == "") {
    return array("version" => "");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function events_event_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function events_event_node_info() {
  $items = array(
    'ev_event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_default_search_api_index().
 */
function events_event_default_search_api_index() {
  $items = array();
  $items['un_events'] = entity_import('search_api_index', '{
    "name" : "UN Events",
    "machine_name" : "un_events",
    "description" : null,
    "server" : "local",
    "item_type" : "node",
    "options" : {
      "datasource" : { "bundles" : [ "ev_event" ] },
      "index_directly" : 1,
      "cron_limit" : "50",
      "fields" : {
        "created" : { "type" : "date" },
        "field_event_category" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "field_event_cluster" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_event_coordination_hub" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "field_event_date:duration" : { "type" : "list\\u003Cduration\\u003E" },
        "field_event_date:value" : { "type" : "list\\u003Cdate\\u003E" },
        "field_event_date:value2" : { "type" : "list\\u003Cdate\\u003E" },
        "field_event_disasters" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_event_documents" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "multifield" },
        "field_event_documents:field_document_type" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_event_info:value" : { "type" : "text" },
        "field_event_location" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_event_location_details" : { "type" : "text" },
        "field_event_organization" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_event_theme" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "search_api_language" : { "type" : "string" },
        "title" : { "type" : "text" }
      },
      "data_alter_callbacks" : {
        "events_event_search_api_store_data" : { "status" : 1, "weight" : "-50", "settings" : [] },
        "search_api_alter_bundle_filter" : {
          "status" : 0,
          "weight" : "-10",
          "settings" : { "default" : "1", "bundles" : [] }
        },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "0", "settings" : { "mode" : "full" } },
        "search_api_alter_add_hierarchy" : {
          "status" : 1,
          "weight" : "0",
          "settings" : { "fields" : { "field_event_location:parent" : "field_event_location:parent" } }
        },
        "search_api_alter_language_control" : {
          "status" : 0,
          "weight" : "0",
          "settings" : { "lang_field" : "", "languages" : [] }
        },
        "search_api_alter_node_access" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_node_status" : { "status" : 1, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 1,
          "weight" : "0",
          "settings" : { "fields" : { "title" : true, "field_event_info:value" : true } }
        },
        "search_api_html_filter" : {
          "status" : 0,
          "weight" : "10",
          "settings" : {
            "fields" : { "title" : true },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_transliteration" : {
          "status" : 1,
          "weight" : "15",
          "settings" : { "fields" : {
              "title" : true,
              "field_event_cluster" : true,
              "field_event_organization" : true,
              "field_event_info:value" : true
            }
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : {
            "fields" : { "title" : true },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : { "title" : true },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        },
        "search_api_porter_stemmer" : {
          "status" : 0,
          "weight" : "35",
          "settings" : { "fields" : { "title" : true }, "exceptions" : "texan=texa" }
        },
        "search_api_highlighting" : {
          "status" : 0,
          "weight" : "40",
          "settings" : {
            "prefix" : "\\u003Cstrong\\u003E",
            "suffix" : "\\u003C\\/strong\\u003E",
            "excerpt" : 1,
            "excerpt_length" : "256",
            "exclude_fields" : [],
            "highlight" : "always",
            "highlight_partial" : 0
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0"
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function events_event_default_search_api_server() {
  $items = array();
  $items['local'] = entity_import('search_api_server', '{
    "name" : "local",
    "machine_name" : "local",
    "description" : "",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "1",
      "partial_matches" : 1,
      "indexes" : { "un_events" : {
          "title" : {
            "table" : "search_api_db_un_events_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_event_organization" : {
            "table" : "search_api_db_un_events_field_event_organization",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_event_cluster" : {
            "table" : "search_api_db_un_events_field_event_cluster",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_un_events",
            "column" : "search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_event_date:value" : {
            "table" : "search_api_db_un_events_field_event_date_value",
            "column" : "value",
            "type" : "list\\u003Cdate\\u003E",
            "boost" : "1.0"
          },
          "field_event_date:value2" : {
            "table" : "search_api_db_un_events_field_event_date_value2",
            "column" : "value",
            "type" : "list\\u003Cdate\\u003E",
            "boost" : "1.0"
          },
          "field_event_date:duration" : {
            "table" : "search_api_db_un_events_field_event_date_duration",
            "column" : "value",
            "type" : "list\\u003Cduration\\u003E",
            "boost" : "1.0"
          },
          "created" : {
            "table" : "search_api_db_un_events",
            "column" : "created",
            "type" : "date",
            "boost" : "1.0"
          },
          "field_event_category" : {
            "table" : "search_api_db_un_events",
            "column" : "field_event_category",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_event_coordination_hub" : {
            "table" : "search_api_db_un_events",
            "column" : "field_event_coordination_hub",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_event_disasters" : {
            "table" : "search_api_db_un_events_field_event_disasters",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_event_location" : {
            "table" : "search_api_db_un_events_field_event_location",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_event_documents" : {
            "table" : "search_api_db_un_events_field_event_documents",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_event_documents:field_document_type" : {
            "table" : "search_api_db_un_events_field_event_documents_field_document_t",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_event_theme" : {
            "table" : "search_api_db_un_events_field_event_theme",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_event_info:value" : {
            "table" : "search_api_db_un_events_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_event_location_details" : {
            "table" : "search_api_db_un_events_text",
            "type" : "text",
            "boost" : "1.0"
          }
        }
      }
    },
    "enabled" : "1"
  }');
  return $items;
}
