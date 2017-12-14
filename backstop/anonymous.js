const fs = require('fs');
const URL = require('url').URL;
const extend = require('util')._extend;

// Read base config from file, check parent dir first.
let config;
let urls;
if (fs.existsSync('../backstop_anon.json')) {
  config = JSON.parse(fs.readFileSync('../backstop_anon.json'));
  urls = fs.readFileSync('../urls_anon.txt').toString().split("\n");
}
else {
  config = JSON.parse(fs.readFileSync('backstop_anon.json'));
  urls = fs.readFileSync('urls_anon.txt').toString().split("\n");
}

// Read first scenario for options.
let baseScenario = config.scenarios[0];
delete baseScenario.label;
delete baseScenario.url;
config.scenarios = [];

for (i in urls) {
  let urlString = urls[i];
  if (urlString) {
    let url = new URL(urlString);
    let scenario = Object.assign({}, baseScenario);

    let label = url.pathname + url.search + url.hash;
    if (label.length > 1) {
      label = label.replace(/^[^a-z]+|[^\w:.-]+/gi, '-');
      label = label.replace(/^-+/gi, '');
      label = label.replace(/-+$/gi, '');
    }
    else {
      label = 'home';
    }

    url.port = process.env.DUC_PORT || url.port;
    url.hostname = process.env.DUC_HOSTNAME || url.hostname;
    url.protocol = process.env.DUC_PROTOCOL || url.protocol;

    scenario.url = url.href;
    scenario.label = label;
    config.scenarios.push(scenario);
  }
}

module.exports = config;
