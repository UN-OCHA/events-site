# Events API

UN Events API.

Run with `silk -silk.url="https://events.staging.humanitarianresponse.info/api/v0" api.md`

Silk can be installed from https://github.com/matryer/silk

Debugging tip: `silk -silk.url="https://events.staging.humanitarianresponse.info/api/v0" -test.v api.md`

## `GET /fetch-events`

Gets a list of events, all parameters are optional.

  * `?start=` // Start date of the range of events, default first of the month, example `2017-03-01`
  * `?end=` // End date of the range of events, default last of the month, example `2017-03-31`
  * `?field_event_organization=` // Filter by a certain organization, default all, example `123`
  * `?field_event_cluster=` // Filter by a certain cluster, default all, example `123`
  * `?field_event_category=` // Filter by a certain category, default all, example `123`
  * `?field_event_coordination_hub=` // Filter by a certain coordination hub, default all, example `123`


Example output.

```json
[
  {
    "id": "11",
    "title": "Two documents",
    "start": "2017-04-20 00:00:00",
    "end": "2017-04-20 00:00:00",
    "allDay": true,
    "url": "\/node\/11",
    "backgroundColor": "#ffeecc",
    "field_event_organization": "123",
    "field_event_cluster": "123",
    "field_event_category": "123",
    "field_event_coordination_hub": "123"
  }
]
```

===

### Response

* Status: `200`
* Content-Type: "application/json"
* Data[0].id: /^[0-9]+$/
* Data[0].title: /./
* Data[0].start: /^[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}$/
* Data[0].end: /^[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}$/
* Data[0].allDay: /true|false/
* Data[0].url: /^\/node\/[0-9]+$/
* Data[0].field_event_organization: /^[0-9]+|$/
* Data[0].field_event_cluster: /^[0-9]+|$/
* Data[0].field_event_category: /^[0-9]+|$/
* Data[0].field_event_coordination_hub: /^[0-9]+|$/


## `GET /fetch-events`

Start date has to be smaller than end date.

  * `?start=2017-08-27`
  * `?end=2017-05-08`
  * `?field_event_organization=`
  * `?field_event_cluster=`
  * `?field_event_category=`
  * `?field_event_coordination_hub=`

===

### Response

* Status: `400`

## `GET /fetch-events`

field_event_organization is an integer.

  * `?start=2017-03-27`
  * `?end=2017-05-08`
  * `?field_event_organization=xyzzy`
  * `?field_event_cluster=`
  * `?field_event_category=`
  * `?field_event_coordination_hub=`

===

### Response

* Status: `400`

## `GET /fetch-events`

field_event_cluster is an integer.

  * `?start=2017-03-27`
  * `?end=2017-05-08`
  * `?field_event_organization=`
  * `?field_event_cluster=xyzzy`
  * `?field_event_category=`
  * `?field_event_coordination_hub=`

===

### Response

* Status: `400`

## `GET /fetch-events`

field_event_category is an integer.

  * `?start=2017-03-27`
  * `?end=2017-05-08`
  * `?field_event_organization=`
  * `?field_event_cluster=`
  * `?field_event_category=xyzzy`
  * `?field_event_coordination_hub=`

===

### Response

* Status: `400`

## `GET /fetch-events`

field_event_coordination_hub is an integer.

  * `?start=2017-03-27`
  * `?end=2017-05-08`
  * `?field_event_organization=`
  * `?field_event_cluster=`
  * `?field_event_category=`
  * `?field_event_coordination_hub=xyzzy`

===

### Response

* Status: `400`


## `GET /organisations`

Gets a list of organisations.

Example output.

```json
[
  {
    "tid": "7",
    "name": "OCHA",
  }
]
```

===

### Response

* Status: `200`
* Content-Type: "application/json"
* Data[0].tid: /^[0-9]+$/
* Data[0].name: /./

## `GET /clusters`

Gets a list of clusters.

Example output.

```json
[
  {
    "tid": "5577",
    "name": "Afganistan: Aviation",
  }
]
```

===

### Response

* Status: `200`
* Content-Type: "application/json"
* Data[0].tid: /^[0-9]+$/
* Data[0].name: /./

## `GET /categories`

Gets a list of categories.

Example output.

```json
[
  {
    "tid": "123",
    "name": "Meeting",
  }
]
```

===

### Response

* Status: `200`
* Content-Type: "application/json"
* Data[0].tid: /^[0-9]+$/
* Data[0].name: /./

## `GET /disasters`

Gets a list of disasters.

Example output.

```json
[
  {
    "tid": "7",
    "name": "OCHA",
  }
]
```

===

### Response

* Status: `200`
* Content-Type: "application/json"
* Data[0].tid: /^[0-9]+$/
* Data[0].name: /./

## `GET /themes`

Gets a list of themes.

Example output.

```json
[
  {
    "tid": "7",
    "name": "OCHA",
  }
]
```

===

### Response

* Status: `200`
* Content-Type: "application/json"
* Data[0].tid: /^[0-9]+$/
* Data[0].name: /./

## `GET /timezones`

Gets a list of time zones.

Example output.

```json
{
  "Africa\/Abidjan": "Africa\/Abidjan: Monday, 24 April, 2017 - 10:54 +0000",
  "Africa\/Accra":"Africa\/Accra: Monday, 24 April, 2017 - 10:54 +0000"
}
```

===

### Response

* Status: `200`
* Content-Type: "application/json"


## `GET /facets`

Gets a list of events, all parameters are optional.

  * `?start=` // Start date of the range of events, default first of the month, example `2017-03-01`
  * `?end=` // End date of the range of events, default last of the month, example `2017-03-31`
  * `?field_event_organization=` // Filter by a certain organization, default all, example `123`
  * `?field_event_cluster=` // Filter by a certain cluster, default all, example `123`
  * `?field_event_category=` // Filter by a certain category, default all, example `123`
  * `?field_event_coordination_hub=` // Filter by a certain coordination hub, default all, example `123`


Example output.

```json
{
  "field_event_category": {
    "6670": "Meeting",
    "6671": "Test"
  },
  "field_event_cluster": {
    "5580": "Afganistan: Education in Emergencies Working Group"
  },
  "field_event_coordination_hub": {
    "6678": "Afganistan: South Eastern Region"
  },
  "field_event_organization": {
    "13": "A Single Drop for Safe Water (ASDSW)",
    "7": "(at)Fire"
  }
}
```

===

### Response

* Status: `200`
* Content-Type: "application/json"


## `GET /facets`

Start date has to be smaller than end date.

  * `?start=2017-08-27`
  * `?end=2017-05-08`
  * `?field_event_organization=`
  * `?field_event_cluster=`
  * `?field_event_category=`
  * `?field_event_coordination_hub=`

===

### Response

* Status: `400`

## `GET /facets`

field_event_organization is an integer.

  * `?start=2017-03-27`
  * `?end=2017-05-08`
  * `?field_event_organization=xyzzy`
  * `?field_event_cluster=`
  * `?field_event_category=`
  * `?field_event_coordination_hub=`

===

### Response

* Status: `400`

## `GET /facets`

field_event_cluster is an integer.

  * `?start=2017-03-27`
  * `?end=2017-05-08`
  * `?field_event_organization=`
  * `?field_event_cluster=xyzzy`
  * `?field_event_category=`
  * `?field_event_coordination_hub=`

===

### Response

* Status: `400`

## `GET /facets`

field_event_category is an integer.

  * `?start=2017-03-27`
  * `?end=2017-05-08`
  * `?field_event_organization=`
  * `?field_event_cluster=`
  * `?field_event_category=xyzzy`
  * `?field_event_coordination_hub=`

===

### Response

* Status: `400`

## `GET /facets`

field_event_coordination_hub is an integer.

  * `?start=2017-03-27`
  * `?end=2017-05-08`
  * `?field_event_organization=`
  * `?field_event_cluster=`
  * `?field_event_category=`
  * `?field_event_coordination_hub=xyzzy`

===

### Response

* Status: `400`
