/**
 * @file
 * Calendar.
 */

(function($) {
  Drupal.behaviors.eventsEvent = {
    attach: function(context, settings) {
      if (!settings.fullcalendar_api.calendarSettings) {
        return;
      }
      var $calendarId = Drupal.settings.fullcalendar_api.calendarId;
      var $calendar = $('#' + $calendarId);
      if (!$calendar.length) {
        return;
      }

      var eventFilters = {
        'field_event_organization': undefined,
        'field_event_cluster': undefined,
        'timezone': 'UTC'
      };

      var $settings = settings.fullcalendar_api.calendarSettings;
      $.extend($settings, {
        'eventRender': function(event, element, view) {
          for (f in eventFilters) {
            if (eventFilters.hasOwnProperty(f) && event.hasOwnProperty(f) && typeof eventFilters[f] != 'undefined' && eventFilters[f] != event[f]) {
              return false;
            }
          }
          return true;
        }
      });

      $.extend($settings['views'], {
        'upcoming': {
          'type': 'list',
          'buttonText': 'Upcoming',
          'duration': {
            'days': 90
          },
          'visibleRange': function(currentDate) {
            return {
              start: currentDate.clone(),
              end: currentDate.clone().add(90, 'days')
            };
          },
          'validRange': function(currentDate) {
            return {
              start: currentDate.clone()
            };
          }
        },
        'past': {
          'type': 'list',
          'buttonText': 'Past events',
          'duration': {
            'days': 90
          },
          'visibleRange': function(currentDate) {
            return {
              start: currentDate.clone().add(-90, 'days'),
              end: currentDate.clone().add(1, 'days')
            };
          },
          'validRange': function(currentDate) {
            return {
              end: currentDate.clone().add(1, 'days')
            };
          }
        }
      });

      $calendar.fullCalendar($settings);

      var handleTimezone = function (e) {
        if (e.target.value) {
          data = e.target.value;
          eventFilters['timezone'] = data;
          $calendar.fullCalendar('option', 'timezone', data);
        }
      };

      var handleICal = function (e) {
        var url = '/ical?';
        url += $.param(eventFilters);
        window.location = url;
      };

      // Redirect to selected option.
      var handleSelect = function (e) {
        if (e.target.value) {
          data = e.target.value;
          data = data.split("?").pop();
          data = data.replace('f[0]=', '');
          parts = data.split('%3A');

          eventFilters[parts[0]] = parts[1];

          // Don't change the source.
          // $calendar.fullCalendar('getEventSources')[0].data[parts[0]] = parts[1];

          // Trigger rerender.
          $calendar.fullCalendar('rerenderEvents');
        }
      };

      var filters = document.querySelectorAll('.block-views');
      if (filters) {
        for (var i = 0; i < filters.length; i++) {
          var filter = filters[i];

          // Construct label.
          var newLabel = document.createElement('label');
          newLabel.innerText = filter.querySelector('h2').innerText;
          newLabel.classList.add('invisible');

          // Construct select.
          var newSelect = document.createElement('select');
          newSelect.classList.add('chosen-enable');
          var options = filter.querySelectorAll('.block-views .content li a');

          if (options.length === 0) {
            filter.classList.add('invisible');
          }
          else {
            // Add empty option.
            var newOption = document.createElement('option');
            data = options[0].href.split("%3A").shift();
            newOption.value = data;
            newOption.text = Drupal.t('- Any -');
            newSelect.appendChild(newOption);

            // Add options.
            for (var j = 0; j < options.length; j++) {
              var option = options[j];
              var newOption = document.createElement('option');
              newOption.value = option.href;
              newOption.text = option.innerHTML.replace(/<span.*/g, '');
              newSelect.appendChild(newOption);
            }

            // Hide filters.
            filter.classList.add('processed');
            filter.appendChild(newLabel);
            filter.appendChild(newSelect);

            if (Drupal.behaviors && Drupal.behaviors.chosen) {
              Drupal.behaviors.chosen.attach(newSelect, Drupal.settings);
              jQuery(newSelect).chosen().change(function(e) {
                handleSelect(e);
              });
            }
          }
        }

        // Add timezone selector.
        var tzDiv = document.createElement('div');
        tzDiv.classList.add('block-views');

        var tzTitle = document.createElement('h2');
        tzTitle.innerHTML = Drupal.t('Time zone');
        tzDiv.appendChild(tzTitle);

        var tzSelect = document.createElement('select');
        tzSelect.id = 'timezone-selector';

        tzDiv.appendChild(tzSelect);
        document.querySelector('.region-content').insertBefore(tzDiv, document.querySelector('.region-content').firstChild);

        $.getJSON('api/v0/timezones', function(timezones) {
          var $tz = $('#timezone-selector');
          var $newtz;
          $tz.append(
            $("<option/>").text('local').attr('value', '')
          );
          for (var tz in timezones) {
            if (Drupal.settings.fullcalendar_api.calendarSettings.timezone = tz) {
              $newtz = $("<option selected/>").text(timezones[tz]).attr('value', tz);
            }
            else {
              $newtz = $("<option/>").text(timezones[tz]).attr('value', tz);
            }
            $tz.append($newtz);
          }

          if (Drupal.behaviors && Drupal.behaviors.chosen) {
            $tz.chosen("destroy");
            $tz.addClass('chosen-enable');
            Drupal.behaviors.chosen.attach($tz, Drupal.settings);
            $tz.chosen().change(function(e) {
              handleTimezone(e);
            });
          }
        });

        // Add ical button.
        var icalDiv = document.createElement('div');
        icalDiv.classList.add('block-views');

        var icalTitle = document.createElement('h2');
        icalTitle.innerHTML = Drupal.t('Export');
        icalDiv.appendChild(icalTitle);

        var icalButton = document.createElement('button');
        icalButton.innerHTML = Drupal.t('ICAL');
        icalButton.addEventListener('click', handleICal);
        icalDiv.appendChild(icalButton);

        document.querySelector('.region-content').insertBefore(icalDiv, document.querySelector('#block-system-main'));
      }
    }
  }
})(jQuery);
