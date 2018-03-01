<?php
/**
 * @file
 * Contains all functions from locale module that had to be rewritten.
 */

/**
 * Process result from locale translation filter form.
 *
 * Rewrites locale_translation_filter_form_submit().
 */
function locale_translation_context_locale_translation_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = locale_translation_filters();
  $filters['context'] = t('Context');
  switch ($op) {
    case t('Filter'):
      foreach ($filters as $name => $filter) {
        if (isset($form_state['values'][$name])) {
          $_SESSION['locale_translation_filter'][$name] = $form_state['values'][$name];
        }
      }
      break;

    case t('Reset'):
      $_SESSION['locale_translation_filter'] = array();
      break;
  }

  $form_state['redirect'] = 'admin/config/regional/translate/translate';
}

/**
 * String search screen.
 *
 * Rewrites locale_translate_seek_screen().
 */
function locale_translation_context_locale_translate_seek_screen() {
  // Add CSS.
  drupal_add_css(drupal_get_path('module', 'locale') . '/locale.css');

  $elements = drupal_get_form('locale_translation_filter_form');
  $output = drupal_render($elements);
  $output .= locale_translation_context_locale_translate_seek();
  return $output;
}

/**
 * Perform a string search and display results in a table.
 *
 * Rewrites _locale_translate_seek().
 */
function locale_translation_context_locale_translate_seek() {
  $output = '';

  // We have at least one criterion to match.
  if (!($query = locale_translation_context_locale_translate_seek_query())) {
    $query = array(
      'translation' => 'all',
      'group' => 'all',
      'language' => 'all',
      'string' => '',
      'context' => 'all',
    );
  }

  $sql_query = db_select('locales_source', 's');

  $limit_language = NULL;
  if ($query['language'] != 'en' && $query['language'] != 'all') {
    $sql_query->leftJoin('locales_target', 't', "t.lid = s.lid AND t.language = :langcode", array(':langcode' => $query['language']));
    $limit_language = $query['language'];
  }
  else {
    $sql_query->leftJoin('locales_target', 't', 't.lid = s.lid');
  }

  $sql_query->fields('s', array(
    'source',
    'location',
    'context',
    'lid',
    'textgroup',
  ));
  $sql_query->fields('t', array('translation', 'language'));

  // Compute LIKE section.
  switch ($query['translation']) {
    case 'translated':
      $sql_query->condition('t.translation', '%' . db_like($query['string']) . '%', 'LIKE');
      $sql_query->orderBy('t.translation', 'DESC');
      break;

    case 'untranslated':
      $sql_query->condition(db_and()
        ->condition('s.source', '%' . db_like($query['string']) . '%', 'LIKE')
        ->isNull('t.translation')
      );
      $sql_query->orderBy('s.source');
      break;

    case 'all':
    default:
      $condition = db_or()
        ->condition('s.source', '%' . db_like($query['string']) . '%', 'LIKE');
      if ($query['language'] != 'en') {
        // Only search in translations if the language is not forced to English.
        $condition->condition('t.translation', '%' . db_like($query['string']) . '%', 'LIKE');
      }
      $sql_query->condition($condition);
      break;
  }

  // Add a condition on the text group.
  if (!empty($query['group']) && $query['group'] != 'all') {
    $sql_query->condition('s.textgroup', $query['group']);
  }

  // Add a condition on the context.
  if (!empty($query['context']) && $query['context'] != 'all') {
    $sql_query->condition('s.context', $query['context']);
  }

  $sql_query = $sql_query->extend('PagerDefault')->limit(50);
  $locales = $sql_query->execute();

  $groups = module_invoke_all('locale', 'groups');
  $header = array(
    t('Text group'),
    t('String'),
    t('Context'),
    ($limit_language) ? t('Language') : t('Languages'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );

  $strings = array();
  foreach ($locales as $locale) {
    if (!isset($strings[$locale->lid])) {
      $strings[$locale->lid] = array(
        'group' => $locale->textgroup,
        'languages' => array(),
        'location' => $locale->location,
        'source' => $locale->source,
        'context' => $locale->context,
      );
    }
    if (isset($locale->language)) {
      $strings[$locale->lid]['languages'][$locale->language] = $locale->translation;
    }
  }

  $rows = array();
  foreach ($strings as $lid => $string) {
    $rows[] = array(
      $groups[$string['group']],
      array(
        'data' => check_plain(truncate_utf8($string['source'], 150, FALSE, TRUE)) . '<br /><small>' . $string['location'] . '</small>',
      ),
      $string['context'],
      array(
        'data' => _locale_translate_language_list($string, $limit_language),
        'align' => 'center',
      ),
      array(
        'data' => l(t('edit'), "admin/config/regional/translate/edit/$lid", array('query' => drupal_get_destination())),
        'class' => array('nowrap'),
      ),
      array(
        'data' => l(t('delete'), "admin/config/regional/translate/delete/$lid", array('query' => drupal_get_destination())),
        'class' => array('nowrap'),
      ),
    );
  }

  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No strings available.'),
  ));
  $output .= theme('pager');

  return $output;
}

/**
 * Build array out of search criteria specified in request variables.
 *
 * Rewrites _locale_translate_seek_query().
 */
function locale_translation_context_locale_translate_seek_query() {
  $query = &drupal_static(__FUNCTION__);
  if (!isset($query)) {
    $query = array();
    $fields = array('string', 'language', 'translation', 'group', 'context');
    foreach ($fields as $field) {
      if (isset($_SESSION['locale_translation_filter'][$field])) {
        $query[$field] = $_SESSION['locale_translation_filter'][$field];
      }
    }
  }

  return $query;
}

/**
 * Process a translation (or template) export form submission.
 *
 * Rewrites locale_translate_export_po_form_submit().
 */
function locale_translation_context_locale_translate_export_po_form_submit($form, &$form_state) {
  // If template is required, language code is not given.
  $language = NULL;
  if (isset($form_state['values']['langcode'])) {
    $languages = language_list();
    $language = $languages[$form_state['values']['langcode']];
  }

  $context = NULL;
  if (isset($form_state['values']['context']) && $form_state['values']['context'] != 'all') {
    $context = $form_state['values']['context'];
  }

  locale_translation_context_locale_export_po(
    $language, $context,
    _locale_export_po_generate($language, locale_translation_context_locale_export_get_strings($language, $form_state['values']['group'], $context)));
}

/**
 * Generates a structured array of strings with translations.
 *
 * Generates a structured array of all strings with translations in
 * $language, if given. This array can be used to generate an export
 * of the string in the database.
 *
 * @param mixed $language
 *   Language object to generate the output for, or NULL if generating
 *   translation template.
 * @param string $group
 *   Text group to export PO file from (eg. 'default' for interface
 *   translations).
 * @param string $context
 *   Context to filter the strings to export.
 *
 * @return array
 *   An array of translation strings.
 */
function locale_translation_context_locale_export_get_strings($language = NULL, $group = 'default', $context = NULL) {
  $query = db_select('locales_source', 'ls');
  $query
    ->fields('ls', array('lid', 'source', 'context', 'location'))
    ->condition('ls.textgroup', $group)
    ->orderBy('lt.plid')
    ->orderBy('lt.plural');

  if ($context) {
    $query->condition('ls.context', $context);
  }

  if (isset($language)) {
    $query->fields('lt', array('translation', 'plid', 'plural'));
    $query->leftjoin('locales_target', 'lt', "ls.lid = lt.lid AND lt.language = '" . $language->language . "'");
  }
  else {
    $query->fields('lt', array('plid', 'plural'));
    $query->leftjoin('locales_target', 'lt', "ls.lid = lt.lid");
  }

  if (isset($context)) {
    $query->condition('ls.context', $context);
  }

  $results = $query->execute();

  $strings = array();

  while ($child = $results->fetchAssoc()) {
    $string = array(
      'comment'     => $child['location'],
      'source'      => $child['source'],
      'context'     => $child['context'],
      'translation' => isset($child['translation']) ? $child['translation'] : '',
    );
    if ($child['plid']) {
      // Has a parent lid. Since we process in the order of plids,
      // we already have the parent in the array, so we can add the
      // lid to the next plural version to it. This builds a linked
      // list of plurals.
      $string['child'] = TRUE;
      $strings[$child['plid']]['plural'] = $child['lid'];
    }
    $strings[$child['lid']] = $string;
  }

  return $strings;
}

/**
 * Write a generated PO or POT file to the output.
 *
 * @param mixed $language
 *   Language object to generate the output for, or NULL if generating
 *   translation template.
 * @param string $context
 *   The translation string context generate the output for, or NULL if
 *   generating all contexts are exported.
 * @param string $output
 *   The PO(T) file to output as a string. See _locale_export_generate_po()
 *   on how it can be generated.
 */
function locale_translation_context_locale_export_po($language = NULL, $context = NULL, $output = NULL) {
  // Generate filename and log the export event.
  $filename = '';
  if (isset($context)) {
    $filename = '_' . $context;
  }

  if (isset($language)) {
    $filename = $language->language . $filename . '.po';
    watchdog('locale', 'Exported %locale translation file: %filename.', array('%locale' => $language->name, '%filename' => $filename));
  }
  else {
    $filename = 'drupal' . $filename . '.pot';
    watchdog('locale', 'Exported translation file: %filename.', array('%filename' => $filename));
  }

  // Download the file for the client.
  header("Content-Disposition: attachment; filename=$filename");
  header("Content-Type: text/plain; charset=utf-8");
  print $output;
  drupal_exit();
}
