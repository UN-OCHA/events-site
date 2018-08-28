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
  // Bail out if function is not available.
  if (!function_exists('language_negotiation_get_switch_links')) {
    return;
  }

  // Add language links.
  global $language;
  $path = drupal_is_front_page() ? '<front>' : $_GET['q'];
  $links = language_negotiation_get_switch_links('language', $path);

  // Bail out if links is not enumerable.
  if (!$links) {
    return;
  }

  $render = array(
    'links' => $links->links,
    'attributes' => array(
      'class' => [
        'cd-global-header__dropdown',
        'cd-dropdown',
        'cd-user-menu__dropdown',
      ],
      'role' => 'menu',
      'id' => 'cd-language',
      'aria-labelledby' => 'cd-language-toggle',
    ),
  );

  $output = '';
  $output .= '<div class="cd-language-switcher">';
  $output .= '<button type="button" class="cd-user-menu__item cd-user-menu__item--small cd-global-header__dropdown-btn" data-toggle="dropdown" id="cd-language-toggle">';
  $output .= $language->language;
  $output .= '<svg width="32" height="32" viewBox="0 0 32 32" class="arrow-down" aria-hidden="true"><path d="M26.2 11.7c0 0.4-0.2 0.6-0.3 0.7l-8.4 8.4c-0.4 0.4-0.9 0.6-1.5 0.6s-1.1-0.2-1.5-0.6l-8.4-8.4c-0.2-0.2-0.3-0.4-0.3-0.7s0.1-0.5 0.3-0.7c0.2-0.2 0.4-0.3 0.7-0.3s0.5 0.1 0.7 0.3l8.4 8.4c0 0 0 0 0 0s0 0 0.1 0 0.1 0 0.2 0l8.3-8.4c0.4-0.4 1-0.4 1.4 0 0.1 0.1 0.3 0.3 0.3 0.7z"></path></svg>';
  $output .= '</button>';
  $output .= theme('links__locale_block', $render);
  $output .= '</div>';

  $vars['page']['language_switcher'] = $output;
}

/**
 * Implements hook_pwa_manifest_alter().
 */
function ochabasic_pwa_manifest_alter(&$manifest) {
  // Hard-code a theme-color into the manifest.
  $manifest['theme_color'] = '#026CB6';

  // Override the PWA default icons with OCHA defaults.
  //
  // If you are using this theme as a starterkit feel free to manually adjust
  // this code block, otherwise copy this hook into your subtheme and customize
  // to your heart's content.
  $manifest['icons'] = [
    [
      'src' => url(drupal_get_path('theme', 'ochabasic') . '/img/android-chrome-512x512.png'),
      'sizes' => '512x512',
      'type' => 'image/png',
    ],
    [
      'src' => url(drupal_get_path('theme', 'ochabasic') . '/img/android-chrome-192x192.png'),
      'sizes' => '192x192',
      'type' => 'image/png',
    ],
  ];
}
