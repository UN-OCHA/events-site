<?php
/**
 * @file
 * Template overrides, preprocess, and alter hooks for the OCHA Basic theme.
 */

/**
 * Implements hook_preprocess_html().
 */
function ochabasic_preprocess_html(&$vars) {
  $viewport = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1.0',
    ),
  );

  drupal_add_html_head($viewport, 'viewport');
}


/**
 * Implements template_preprocess_page().
 */
function ochabasic_preprocess_page(&$vars) {
  // Assemble list of active languages in this installation.
  $lang_list = language_list('enabled');
  foreach($lang_list['1'] as $avail_lang) {
    $vars['available_languages'][] = $avail_lang;
  }
}
