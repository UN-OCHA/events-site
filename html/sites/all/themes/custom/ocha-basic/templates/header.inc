<?php
/**
 * @file
 * Displays the site header.
 */

?>
<a href="#main" class="skip-link">Skip to content</a>
<header class="cd-header" role="banner">
  <div class="cd-global-header">
    <div class="container cd-global-header__inner">
      <div class="cd-global-header__sites cd-dropdown">
        <button type="button" class="cd-global-header__sites-btn" data-toggle="dropdown" id="cd-related-platforms-toggle">
          Related platforms
          <i class="icon-arrow-down" aria-hidden="true"></i>
        </button>
        <ul class="cd-dropdown-menu" role="menu" id="cd-related-platforms" aria-labelledby="cd-related-platforms-toggle">
          <li><a href="https://fts.unocha.org/">Financial Tracking Service</a></li>
          <li><a href="https://humdata.org/">Humanitarian Data Exchange</a></li>
          <li><a href="https://humanitarian.id/">Humanitarian ID</a></li>
          <li><a href="https://humanitarianresponse.info/">HumanitarianResponse.info</a></li>
          <li><a href="https://reliefweb.int/">ReliefWeb</a></li>
        </ul>
      </div>
      <div class="cd-global-header__nav">
        <?php if ($secondary_menu): ?>
          <?php print theme('links__system_secondary_menu',
            array(
              'links' => $secondary_menu,
              'attributes' => array(
                'id' => 'secondary-menu',
                'class' => array('cd-nav cd-nav--secondary'),
              ),
            )
          ); ?>
        <?php endif; ?>
        <div class="cd-global-header__language-switcher">
          <?php print $page['language_switcher']; ?>
        </div>
      </div>
    </div>
  </div>
  <div class="cd-site-header">
    <div class="container cd-site-header__inner">
      <?php if($is_front): ?>
        <h1>
          <a href="<?php print $front_page; ?>" class="cd-site-header__logo">
            <span class="sr-only"><?php print t('Humanitarian Events') ?></span>
          </a>
        </h1>
      <?php else: ?>
        <a href="<?php print $front_page; ?>" class="cd-site-header__logo">
          <span class="sr-only"><?php print t('Humanitarian Events') ?></span>
        </a>
      <?php endif; ?>
    </div>
  </div>
</header>
