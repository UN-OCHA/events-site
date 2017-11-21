# Setup.
composer install
npm i

# Fetch a login link and create the cookie.
docker exec -i events-local-php drush --root=/srv/www/html uli > login_link.txt
php createCookie.php

# Create baseline images.
npm run reference

# Run contrib updates, exclude core.
docker exec -i events-local-php drush --root=/srv/www/html up --no-core -y > update.txt

# Nothing updated, exit.
[[ -z $(git status -uno --porcelain ../html) ]] && echo "no updates needed" && exit 0

# Create screenshots
npm run reference-anon

if [ ! $? -eq 0 ]; then
  echo "Errors in anonymous screenshots"
  exit 11
fi

npm run reference-auth

if [ ! $? -eq 0 ]; then
  echo "Errors in authenticated screenshots"
  exit 12
fi

## Clean up pre commit
git checkout .
rm package-lock.json

# No regressions, create branch.
branchname=`date +%Y-%m-%d`-update-contrib
git checkout -b $branchname

# Add changed/added contrib files.
git add ../html/sites/all/modules/contrib/
git commit -m 'Update contrib modules'
git push origin $branchname
