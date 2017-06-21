cd ../html/sites/test/
../../../behat/bin/drush runserver 8888 > /dev/null &
phantomjs --webdriver=4444  > /dev/null &
