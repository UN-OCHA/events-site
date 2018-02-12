<?php

/**
 * @file
 * Theme implementation to display a single Drupal page.
 */
?>
<div class="page-wrapper">
  <?php include 'header.inc'; ?>

  <?php if($messages): ?>
    <div id="messages" class="container">
      <?php print $messages; ?>
    </div>
  <?php endif; ?>

  <div id="main" class="container">

    <?php if ($tabs): ?>
      <div class="tabs"><?php print render($tabs); ?></div>
    <?php endif; ?>

    <?php if (!drupal_is_front_page() && $title && isset($node) && $node->type !== 'ev_event'): ?>
      <h1>
        <?php print $title; ?>
      </h1>
    <?php endif; ?>

     <?php print render($page['content']); ?>
  </div>
</div>
<?php include 'footer.inc'; ?>
