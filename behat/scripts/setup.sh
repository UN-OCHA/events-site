composer install
ln -s ../html/sites sites
wget -N https://chromedriver.storage.googleapis.com/2.36/chromedriver_linux64.zip
unzip chromedriver_linux64.zip
cd ../html/sites/all/themes/custom/ocha-basic
export NODE_ENV='production'
npm install
gulp build
