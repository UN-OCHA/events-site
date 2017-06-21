cd sites
/usr/bin/env PHP_OPTIONS="-d sendmail_path=`which true`" drush site-install standard --db-url=sqlite:///tmp/test.db --sites-subdir=test --account-pass=admin -y
cd test
drush en events_config -y
drush en events_event -y
drush fra -y
drush search-index
cd ../..
