Configuration for automatic updates using the drupal-update-checker project.

To use this, clone git@github.com:UN-OCHA/drupal-update-checker.git to the
Drupal project root, then run `drupal-update-checker/run_upgrade.sh` with
appropriate options or environment variables.

The only available template variable is %%URI%%, which is replaced with whatever
URI is passed via the `drupal-update-checker/run_upgrade.sh` script.

See `drupal-update-checker/run_upgrade.sh -h` for help.
