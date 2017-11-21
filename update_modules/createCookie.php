<?php

require 'vendor/autoload.php';

$client = new \GuzzleHttp\Client();
$jar = new \GuzzleHttp\Cookie\CookieJar;
$url = trim(file_get_contents('login_link.txt'));
$cookie_file = 'backstop_data/engine_scripts/cookies.json';

$res = $client->request('GET', $url, [
  'cookies' => $jar,
]);

if ($res->getStatusCode() != 200) {
  exit(1);
}

// Convert cookie keys to lowercase.
$cookies = $jar->toArray();
$cookies = lowerCaseKeys($cookies);

print_r($cookies);

file_put_contents($cookie_file, json_encode($cookies));

function lowerCaseKeys($array) {
  $new = [];
  foreach ($array as $key => $value) {
    if (is_array($value)) {
        $new[lcfirst($key)] = lowerCaseKeys($value);
    }
    elseif (is_string($key)) {
      $new[lcfirst($key)] = $value;
    }
    else {
      $new[$key] = $value;
    }
  }

  return $new;
}
