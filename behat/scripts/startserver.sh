cd sites/test
drush runserver 8888  > /dev/null &
phantomjs --webdriver=4444  > /dev/null &
