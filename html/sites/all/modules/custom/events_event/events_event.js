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
        'field_event_category': undefined,
        'field_event_coordination_hub': undefined,
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
        },
        viewRender: function(view) {
          if (view.name === 'upcoming') {
            if ($calendar.fullCalendar('getDate').unix() < moment().unix()) {
              $calendar.fullCalendar('gotoDate', moment());
            }
          }
          else if (view.name === 'past') {
            if ($calendar.fullCalendar('getDate').toISOString() >= moment().format('Y-MM-DD')) {
              $calendar.fullCalendar('gotoDate', moment().add(-1, 'days'));
              window.setTimeout(function () {
                $calendar.fullCalendar('prev');
              }, 250);
            }
          }
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
          'type': 'listrev',
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
              end: currentDate.clone().add(-1, 'days')
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

      var buildIcalButton = function () {
        var button = document.createElement('button');
        button.innerHTML = Drupal.t('ICAL');
        button.addEventListener('click', handleICal);
        return button;
      }

      var buildExportOptions = function () {
        var container = document.createElement('div');
        container.className += ' calendar-export';

        var exportButton = document.createElement('button');
        exportButton.className = 'btn-primary calendar-export__button';
        exportButton.innerHTML = Drupal.t('Export');
        exportButton.id = 'export-dropdown';
        exportButton.setAttribute('data-toggle', 'dropdown');
        exportButton.setAttribute('aria-haspopup', 'true');
        exportButton.setAttribute('aria-expanded', 'false');

        container.appendChild(exportButton);

        var exportOptionsList = document.createElement('ul');
        exportOptionsList.className = 'dropdown-menu';
        exportOptionsList.setAttribute('aria-labelledby', 'export-dropdown');

        var icalButton = buildIcalButton();

        var exportListItem = document.createElement('li');
        exportListItem.appendChild(icalButton);
        exportOptionsList.appendChild(exportListItem);
        container.appendChild(exportOptionsList);
        return container;
      }

      var wrapFilters = function (filters) {
        var content = document.querySelector('.region-content');
        var filtersWrapper = document.createElement('div');
        filtersWrapper.className = 'calendar-filters';
        for (var i = 0; i < filters.length; i++) {
          filtersWrapper.appendChild(filters[i]);
        }
        content.insertBefore(filtersWrapper, document.querySelector('#block-system-main'));
      }

      var filters = document.querySelectorAll('.block-views');
      if (filters) {

        wrapFilters(filters);

        for (var i = 0; i < filters.length; i++) {
          var filter = filters[i];

          // Construct label.
          var newLabel = document.createElement('label');
          newLabel.innerText = filter.querySelector('h2').innerText;
          newLabel.setAttribute('for', 'filter-' + i);
          var heading = filter.querySelector('h2');
          heading.className = 'hidden';

          // Construct select.
          var newSelect = document.createElement('select');
          newSelect.className += ' chosen-enable';
          newSelect.id = 'filter-' + i;
          var options = filter.querySelectorAll('.block-views .content li a');

          if (options.length === 0) {
            filter.className += ' invisible';
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
            filter.className += ' processed';
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
        tzDiv.className += 'calendar-settings';

        var tzTitle = document.createElement('h2');
        tzTitle.innerHTML = Drupal.t('Calendar settings');
        tzDiv.appendChild(tzTitle);

        var tzSubTitle = document.createElement('h3');
        tzSubTitle.innerHTML = Drupal.t('Time zone');
        tzDiv.appendChild(tzSubTitle);

        var tzCurrent = document.createElement('p');
        tzCurrent.innerHTML = Drupal.t('Current time zone: ');

        tzDiv.appendChild(tzCurrent);

        var tzLabel = document.createElement('label');
        tzLabel.setAttribute('for', 'timezone-selector');
        tzLabel.innerHTML = Drupal.t('Display times from the following time zone');
        tzDiv.appendChild(tzLabel);


        var tzSelect = document.createElement('select');
        tzSelect.id = 'timezone-selector';

        tzDiv.appendChild(tzSelect);
        document.querySelector('#fullcalendar').insertBefore(tzDiv, null);

        $.getJSON('api/v0/timezones', function(timezones) {
          var $tz = $('#timezone-selector');
          var $newtz;
          var currentTz;

          $tz.append(
            $("<option/>").text('local').attr('value', '')
          );
          for (var tz in timezones) {
            if (Drupal.settings.fullcalendar_api.calendarSettings.timezone === tz) {
              $newtz = $("<option selected/>").text(timezones[tz]).attr('value', tz);
              currentTz = timezones[tz];
            }
            else {
              $newtz = $("<option/>").text(timezones[tz]).attr('value', tz);
            }
            $tz.append($newtz);
          }

          tzCurrent.innerHTML += currentTz;

          if (Drupal.behaviors && Drupal.behaviors.chosen) {
            $tz.chosen('destroy');
            $tz.addClass('chosen-enable');
            Drupal.behaviors.chosen.attach($tz, Drupal.settings);
            $tz.chosen().change(function(e) {
              handleTimezone(e);
            });
          }
        });

        var exportDiv = buildExportOptions();
        document.querySelector('.region-content').insertBefore(exportDiv, document.querySelector('.calendar-filters'));
      }
    }
  }
})(jQuery);
