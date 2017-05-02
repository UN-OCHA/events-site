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

      var defaultFilters = Drupal.settings.fullcalendar_api.calendarSettings.defaultFilters;
      var eventFilters = Drupal.settings.fullcalendar_api.calendarSettings.availableFilters;

      var updateEventFilters = function (filters) {
        eventFilters = filters;
        var path = '?';
        for (f in eventFilters) {
          if (eventFilters.hasOwnProperty(f) && typeof eventFilters[f] != 'undefined' && eventFilters[f] != '') {
            path += f + '=' + eventFilters[f] + '&';
          }
        }
        history.pushState(eventFilters, '', path);
      };

      var $settings = settings.fullcalendar_api.calendarSettings;
      $.extend($settings, {
        eventRender: function(event, element, view) {
          for (f in eventFilters) {
            if (eventFilters.hasOwnProperty(f) && event.hasOwnProperty(f) && typeof eventFilters[f] != 'undefined' && eventFilters[f] != '' && event[f].indexOf(eventFilters[f]) === -1) {
              return false;
            }
          }
          return true;
        },
        viewRender: function(view) {
          // Store view.name, view.start and view.end
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
        var url = $settings.base_url + '/ical?';
        url += $.param(eventFilters);
        window.location = url;
      };

      // Redirect to selected option.
      var handleSelect = function (e) {
        if (e.target.value) {
          data = e.target.value;
          parts = data.split(':');

          eventFilters[parts[0]] = parts[1];
          updateEventFilters(eventFilters);

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

      renderElements = function (elements, doc, margin_top) {
        html2canvas(elements.shift(), {
          onrendered: function(canvas) {
            var height = canvas.height + 30 > doc.internal.pageSize.height ? doc.internal.pageSize.height - 30 : canvas.height;
            var ratio = 1;
            if (height !== canvas.height) {
              ratio = height / canvas.height;
            }
            var width = canvas.width * ratio + 30 > doc.internal.pageSize.width ? doc.internal.pageSize.width - 30 : canvas.width * ratio;
            var new_ratio = width / canvas.width;
            if (new_ratio !== ratio) {
              height = canvas.height * new_ratio;
            }
            if (margin_top + 15 + height > doc.internal.pageSize.height) {
              doc.addPage();
              margin_top = 15;
            }
            doc.addImage(canvas.toDataURL('image/png'), 'PNG', 15, margin_top, width, height);
            if (!elements.length) {
              doc.save('export.pdf');
            }
            else {
              renderElements(elements, doc, margin_top + 15 + height);
            }
          }
        });
      }

      var buildPdfButton = function () {
        var button = document.createElement('button');
        button.innerHTML = Drupal.t('PDF');
        button.addEventListener('click', function (e) {
          e.preventDefault();
          var elements = [];
          elements.push($('.site-header'));
          elements.push($calendar.find('.fc-view-container'));

          var filtered_elements = [];
          for (var i = 0; i < elements.length; i++) {
            if (elements[i]) {
              filtered_elements.push(elements[i]);
            }
          }
          var doc = new jsPDF('portrait', 'px', 'A4', true);
          renderElements(filtered_elements, doc, 15);
        });
        return button;
      }

      var buildExportOptions = function () {
        var container = document.createElement('div');
        container.className = 'calendar-export';

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
        var pdfButton = buildPdfButton();

        var exportListItem = document.createElement('li');
        exportListItem.appendChild(icalButton);
        exportListItem.appendChild(pdfButton);
        exportOptionsList.appendChild(exportListItem);
        container.appendChild(exportOptionsList);
        return container;
      }

      $.getJSON($settings.base_url + '/api/v0/facets', function(facets) {
        var filtersWrapper = document.createElement('div');
        filtersWrapper.className = 'calendar-filters clearfix';

        var filterCount = 0;
        for (var f in facets) {
          var facet = facets[f];
          filterCount++;

          var filter = document.createElement('div');

          if (facet.values.length === 0) {
            continue;
          }

          // Construct label.
          var newLabel = document.createElement('label');
          newLabel.innerText = facet.label;
          newLabel.setAttribute('for', 'filter-' + filterCount);

          // Construct select.
          var newSelect = document.createElement('select');
          newSelect.className += ' chosen-enable';
          newSelect.id = 'filter-' + filterCount;

          // Add empty option.
          var newOption = document.createElement('option');
          newOption.value = f;
          newOption.text = Drupal.t('- Any -');
          newSelect.appendChild(newOption);

          // Add options.
          for (var o in facet.values) {
            var option = facet.values[o];
            var newOption = document.createElement('option');
            newOption.value = f + ':' + o;
            newOption.text = option;
            if (defaultFilters[f] && defaultFilters[f] == o) {
              newOption.selected = 'selected';
            }
            newSelect.appendChild(newOption);
          }

          // Hide filters.
          filter.className += ' processed block-views';
          filter.appendChild(newLabel);
          filter.appendChild(newSelect);
          filtersWrapper.appendChild(filter);

          if (Drupal.behaviors && Drupal.behaviors.chosen) {
            Drupal.behaviors.chosen.attach(newSelect, Drupal.settings);
            jQuery(newSelect).chosen().change(function(e) {
              handleSelect(e);
            });
          }
        }
        document.querySelector('#block-system-main').insertBefore(filtersWrapper, document.querySelector('#block-system-main').firstChild);

        var exportDiv = buildExportOptions();
        document.querySelector('#block-system-main').insertBefore(exportDiv, document.querySelector('#block-system-main').firstChild);

        eventFilters = $.extend(eventFilters, defaultFilters);
        // Trigger rerender.
        $calendar.fullCalendar('rerenderEvents');
      });

      var filters = document.querySelectorAll('.block-views');
      if (filters) {

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

        $.getJSON($settings.base_url + '/api/v0/timezones', function(timezones) {
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

      }
    }
  }
})(jQuery);
