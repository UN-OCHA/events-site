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
      </div>
      <?php print render($page['lang_switch']); ?>
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
      <?php
else: ?>
        <a href="<?php print $front_page; ?>" class="cd-site-header__logo">
          <span class="sr-only"><?php print t('Humanitarian Events') ?></span>
        </a>
      <?php
endif;
      ?>

      <!-- <div class="cd-site-header__search">
        <form role="search" class="cd-header-search">
          <label for="cd-search" class="sr-only">Search for Events</label>
          <input type="search" placeholder="Search for Events" name="cd-search" id="cd-search" class="cd-header-search__input">
          <button type="submit" class="cd-header-search__button">
            <svg version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="16" height="16" viewBox="0 0 16 16" aria-hidden="true">
            <path d="M6.344 9.344q1.25 0 2.125-0.875t0.875-2.125-0.875-2.125-2.125-0.875-2.125 0.875-0.875 2.125 0.875 2.125 2.125 0.875zM10.344 9.344l3.313 3.313-1 1-3.313-3.313v-0.531l-0.188-0.188q-1.188 1.031-2.813 1.031-1.813 0-3.078-1.25t-1.266-3.063 1.266-3.078 3.078-1.266 3.063 1.266 1.25 3.078q0 1.625-1.031 2.813l0.188 0.188h0.531z"></path>
            </svg>
            <span class="sr-only">Search</span>
          </button>
          <span class="cd-header-search__active"></span>
        </form>
      </div> -->

    </div>
  </div>
</header>
