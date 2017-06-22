cp scripts/sites.php ../html/sites/
cd ../html
export PHP_OPTIONS="-d sendmail_path=`which true`"
../behat/bin/drush site-install standard --db-url=sqlite://sites/all/test.db --sites-subdir=test --account-pass=admin -y
cd sites/test
../../../behat/bin/drush en events_config -y
../../../behat/bin/drush en events_event -y
../../../behat/bin/drush fra -y
../../../behat/bin/drush search-index
