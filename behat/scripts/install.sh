cd ../html
export PHP_OPTIONS="-d sendmail_path=`which true`"
../behat/bin/drush site-install behat --debug --root=$PWD --db-url=sqlite://sites/all/test.db --uri=http://127.0.0.1:8888 --sites-subdir=8888.127.0.0.1 --account-pass=admin -y

cd sites/8888.127.0.0.1
../../../behat/bin/drush en events_config -y
../../../behat/bin/drush en events_event -y
../../../behat/bin/drush en events_page -y
../../../behat/bin/drush cc all
../../../behat/bin/drush fra -y
../../../behat/bin/drush search-api-index
../../../behat/bin/drush vset -y events_event_page_cache 0
