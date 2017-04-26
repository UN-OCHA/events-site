<?php
/**
 * @file
 * Displays the site header.
 */

?>
<header role="banner" class="site-header">
	<div class="site-header-top">
		<div class="container">
      <a href="#main" class="skip-link">Skip to content</a>
			<?php if ($secondary_menu): ?>
				<?php print theme('links__system_secondary_menu',
          array(
            'links' => $secondary_menu,
            'attributes' => array(
              'id' => 'secondary-menu',
              'class' => array('site-header-top__nav'),
            ),
          )
        ); ?>
			<?php
endif; ?>
		</div>
	</div>
  <div class="container">
    <div class="site-header__inner">

      <?php if($is_front): ?>
      <h1 class="site-header__logo">
        <a href="<?php print $front_page; ?>">
          <img src="<?php print $base_path . $directory . '/assets/images/humanitarian-events-logo.png'; ?>" alt="<?php print t('Humanitarian Events') ?>">
        </a>
      </h1>
      <?php
else: ?>
        <a href="<?php print $front_page; ?>" class="site-header__logo">
          <img src="<?php print $base_path . $directory . '/assets/images/humanitarian-events-logo.png'; ?>" alt="<?php print t('Humanitarian Events') ?>">
        </a>
      <?php
endif;
      ?>
    </div>
  </div>
</header>
