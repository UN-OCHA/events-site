# Behat instructions

```
cd behat
composer install
```

## Terminal 1

```
cd ../html
rm -f sites/all/test.db
export PHP_OPTIONS="-d sendmail_path=`which true`"
cd ../html
export PHP_OPTIONS="-d sendmail_path=`which true`"
../behat/bin/drush site-install behat --debug --root=$PWD --db-url=sqlite://sites/all/test.db --sites-subdir=8888.localhost --account-pass=admin -y

cd sites/8888.localhost
../../../behat/bin/drush en events_config -y
../../../behat/bin/drush en events_event -y
../../../behat/bin/drush en events_page -y
../../../behat/bin/drush cc all
../../../behat/bin/drush fra -y
../../../behat/bin/drush search-api-index
../../../behat/bin/drush vset -y events_event_page_cache 0
../../../behat/bin/drush runserver 8888
```

## Terminal 2

```
wget -N https://chromedriver.storage.googleapis.com/2.36/chromedriver_linux64.zip
unzip chromedriver_linux64.zip
./chromedriver
```

## Terminal 3

```
bin/behat --config behat.local.yml --format pretty
```
