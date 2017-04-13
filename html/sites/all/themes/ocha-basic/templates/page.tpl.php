<?php include('header.inc'); ?>

<?php if($messages): ?>
  <div id="messages">
    <div class="section clearfix">
      <?php print $messages; ?>
    </div>
  </div>
<?php endif; ?>

<div id="main" class="container">

  <h1><?php print $site_name; ?></h1>

  <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>

	<?php print render($page['content']); ?>
</div>

<?php include('footer.inc'); ?>
