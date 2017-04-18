<?php
/**
 * @file
 * Theme implementation to display a single Drupal page.
 */
?>
<?php include 'header.inc'; ?>

<?php if($messages): ?>
  <div id="messages" class="container">
    <?php print $messages; ?>
  </div>
<?php endif; ?>

<div id="main" class="container">

  <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>

  <?php if ($title): ?>
    <h1>
      <?php print $title; ?>
    </h1>
   <?php endif; ?>

	<?php print render($page['content']); ?>
</div>

<?php include 'footer.inc'; ?>
