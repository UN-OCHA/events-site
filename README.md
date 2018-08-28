This is the Events Drupal 7 website
-----------------------------------

The full Drupal 7 source tree lives in the html directory.

Add any new custom modules in the html/sites/all/modules/custom directory.

For any contrib modules or themes, please add them via `composer require` and
commit both the composer.json file and the downloaded them or module source
from sites/all/... to the repository.

You can add any patches for core or contrib to composer.patches.json, run
`composer update` and commit the result.
