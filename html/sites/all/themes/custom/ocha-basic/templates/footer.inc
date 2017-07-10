<?php
/**
 * @file
 * Displays the site footer.
 */

?>
<footer role="contentinfo" class="footer">
  <div class="container">
    <div class="row">
      <div class="col-md-8">
        <?php if ($secondary_menu): ?>
          <?php print theme('links__menu_footer_menu',
            array(
              'links' => menu_navigation_links('menu-footer-menu'),
              'attributes' => array(
                'id' => 'footer-menu',
                'class' => array('footer__nav'),
              ),
            )
          ); ?>
        <?php
endif; ?>
        </div>
      </div>

      <div class="row">
        <div class="col-md-6">
         <div class="footer__logo">
          <?php print t('Service provided by'); ?>
          <a href="http://www.unocha.org/">
           <img src="<?php print $base_path . $directory . '/assets/images/ocha-logo.svg'; ?>" alt="<?php print t('OCHA') ?>">
         </a>
       </div>
     </div>
     <div class="col-md-6">
       <p class="footer__licence">Except where otherwise noted, content on this site is licensed under a <a href="https://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0</a> International license. <span class="icon-creative-commons" aria-hidden="true"></span></p>
     </div>
   </div>

 </div>
</footer>
