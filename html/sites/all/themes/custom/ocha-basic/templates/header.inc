<?php
/**
 * @file
 * Displays the site header.
 */

?>
<header role="banner" class="site-header">
	<div class="site-header-top">
		<div class="container">
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
      <a href="#main" class="skip-link">Skip to content</a>
      <a href="<?php print $front_page; ?>" class="site-header__logo">
        <img src="<?php print $base_path . $directory . '/assets/images/ocha-logo.svg'; ?>" alt="UN-OCHA">
      </a>
    </div>
  </div>
</header>
