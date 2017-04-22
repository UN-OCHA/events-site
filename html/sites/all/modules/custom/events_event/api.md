# Events API

UN Events API.

Run with `silk -silk.url="https://events.staging.humanitarianresponse.info" api.md`  
Info on https://github.com/matryer/silk

## `GET /fetch-events`

Gets a list of events.

  * `?start=2017-03-27` // Start date of the range of events
  * `?end=2017-05-08` // End date of the range of events
  * `?field_event_organization=` // Filter by a certain organization
  * `?field_event_cluster=` // Filter by a certain cluster

Example output.

```json
[
  {
    "id": "11",
    "title": "Two documents",
    "start": "2017-04-20T00:00:00",
    "end": "2017-04-20T00:00:00",
    "allDay": true,
    "url": "\/node\/11",
    "backgroundColor": "#ffeecc",
    "field_event_organization": "",
    "field_event_cluster": "4500"
  }
]
```

===

### Response

* Status: `200`
* Content-Type: "application/json"
* Data[0].id: /^[0-9]+$/
* Data[0].title: /./
* Data[0].start: /^[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}$/
* Data[0].end: /^[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}$/
* Data[0].allDay: /true|false/
* Data[0].url: /^\/node\/[0-9]+$/
* Data[0].field_event_organization: /^[0-9]+|$/
* Data[0].field_event_cluster: /^[0-9]+|$/


## `GET /fetch-events`

Start date has to be smaller than end date.

  * `?start=2017-08-27`
  * `?end=2017-05-08`
  * `?field_event_organization=`
  * `?field_event_cluster=`

===

### Response

* Status: `500`

## `GET /fetch-events`

field_event_organization is an integer.

  * `?start=2017-03-27`
  * `?end=2017-05-08`
  * `?field_event_organization=xyzzy`
  * `?field_event_cluster=`

===

### Response

* Status: `500`

## `GET /fetch-events`

field_event_cluster is an integer.

  * `?start=2017-03-27`
  * `?end=2017-05-08`
  * `?field_event_organization=`
  * `?field_event_cluster=xyzzy`

===

### Response

* Status: `500`
