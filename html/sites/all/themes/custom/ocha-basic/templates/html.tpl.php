<?php

/**
 * @file
 * Returns the HTML for the basic html structure of a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728208
 */
?>
<!DOCTYPE html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" <?php print $html_attributes; ?>>
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <link href="https://fonts.googleapis.com/css?family=Open+Sans:300,400,600" rel="stylesheet">
  <!--[if lt IE 9]>
    <script src="https://cdn.jsdelivr.net/html5shiv/3.7.3/html5shiv.min.js"></script>
  <![endif]-->
</head>
<body class="<?php print $classes; ?>" <?php print $attributes; ?>>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
  <script>
    (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
    (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
    m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
    })(window,document,'script','https://www.google-analytics.com/analytics.js','ga');
    ga('create', 'UA-101476383-1', 'auto');
    ga('send', 'pageview');
  </script>
</body>
</html>
