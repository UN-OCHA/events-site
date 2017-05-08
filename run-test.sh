#!/bin/sh -eu

# Do not run if there is no phpcs.
test -x vendor/bin/phpcs || (echo "You need to do some composering" && exit 1)

# Ensure the Drupal standard is available.
ln -sf $(pwd)/vendor/drupal/coder/coder_sniffer/Drupal \
  $(pwd)/vendor/squizlabs/php_codesniffer/CodeSniffer/Standards/

# Sniff!
./vendor/bin/phpcs --extensions=info,module,install,inc,php \
                   --report=full --standard=Drupal \
                   html/sites/all/modules/custom
