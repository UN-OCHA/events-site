# Behat instructions

## Travis

See root .travis.yml file

## Locally

### Site install

```
cp scripts/sites.php ../html/sites/
cd ../html
rm -f sites/all/test.db
export PHP_OPTIONS="-d sendmail_path=`which true`"
../behat/bin/drush site-install behat --db-url=sqlite://sites/all/test.db --sites-subdir=test --account-pass=admin -y
cd sites/test
../../../behat/bin/drush en events_config -y
../../../behat/bin/drush en events_event -y
../../../behat/bin/drush en events_page -y
../../../behat/bin/drush cc all
../../../behat/bin/drush fra -y
../../../behat/bin/drush search-api-index
../../../behat/bin/drush vset -y events_event_page_cache 0
```

### Webserver

Use `localhost`, not `127.0.0.1` so it doesn't use the default sites directory

```
cd ../html/sites/test
../../../behat/bin/drush runserver localhost:8888
```

### Chromedriver

```
wget -N https://chromedriver.storage.googleapis.com/2.36/chromedriver_linux64.zip
unzip chromedriver_linux64.zip
./chromedriver
```

### Behat

```
bin/behat --config behat.local.yml --format pretty
```
