composer install
ln -s ../html/sites sites
wget -N https://chromedriver.storage.googleapis.com/2.30/chromedriver_linux64.zip
unzip chromedriver_linux64.zip
ls
cd ../html/sites/all/themes/custom/ocha-basic
npm install
grunt
