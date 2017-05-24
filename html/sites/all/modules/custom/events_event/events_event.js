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
      var state = {
        'view': 'month',
        'date': '',
      };

      var updateState = function () {
        $.extend(state, eventFilters);
        var path = '?';
        for (f in state) {
          if (state.hasOwnProperty(f) && typeof state[f] != 'undefined' && state[f] != '') {
            path += f + '=' + state[f] + '&';
          }
        }
        history.replaceState(state, '', path);
      };

      var updateEventFilters = function (filters) {
        eventFilters = filters;
        updateState();
      };

      var addFilterLegend = function (select, chosen) {
        // If new categories are added, add them to the sass map in variables/_colours.scss
        // This is pretty fragile as relies on the category names, would be better to
        // return the colours from drupal somehow.
        if (select.find('option:first-child').val() !== 'cat') { // Only add legends to category filter
          return;
        }
        var options = $(chosen.search_results).find('li');
        var optionsLength = options.length;
        var i = 0;
        for (i; i < optionsLength; i++) {
          var optionLabel = $(options[i]).text();
          optionLabel = optionLabel.split(' ').join('-');
          optionLabel = optionLabel.toLowerCase();
          $(options[i]).addClass('chosen-option-has-legend').prepend('<span class="chosen-legend ' + optionLabel + '"></span>');
        }
      };

      var $settings = settings.fullcalendar_api.calendarSettings;

      // Needed to fix navigation problem on past events.
      var alreadyTrigger = false;

      $.extend($settings, {
        eventLimit: false,
        eventRender: function(event, element, view) {
          element.attr('data-start', event.start._i);

          for (f in eventFilters) {
            if (eventFilters.hasOwnProperty(f) && event.hasOwnProperty(f) && typeof eventFilters[f] != 'undefined' && eventFilters[f] != '' && event[f].indexOf(eventFilters[f]) === -1) {
              return false;
            }
          }

          // Add location.
          if (event.location) {
            if (view.name === 'listYear' || view.name === 'upcoming' || view.name === 'past') {
              if (event.locationDetails) {
                element.find('.fc-list-item-title').html(element.find('.fc-list-item-title').html() + '<div class="fc-location-details">' + event.locationDetails + '</div>');
              }
              element.find('.fc-list-item-title').html(element.find('.fc-list-item-title').html() + '<div class="fc-location">' + event.location + '</div>');
            }
            else {
              if (event.locationDetails) {
                element.find('.fc-content').append('<span class="fc-location-details">' + event.locationDetails + '</span>');
              }
              element.find('.fc-content').append('<span class="fc-location">' + event.location + '</span>');
            }
          }

          return true;
        },
        height: 'auto',
        viewRender: function(view) {
          // Store view.name, view.start and view.end
          state.view = view.name;
          state.date = $calendar.fullCalendar('getDate').toISOString();
          updateState();

          if (view.name === 'upcoming') {
            if ($calendar.fullCalendar('getDate').unix() < moment().add(-1, 'days').unix()) {
              $calendar.fullCalendar('gotoDate', moment());
            }
          }
          else if (view.name === 'past') {
            if (!alreadyTrigger && $calendar.fullCalendar('getDate').toISOString() >= moment().format('Y-MM-DD')) {
              $calendar.fullCalendar('gotoDate', moment().add(0, 'days'));
              alreadyTrigger = true;
              window.setTimeout(function () {
                $calendar.fullCalendar('prev');
                alreadyTrigger = false;
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
          'validRange': function(currentDate) {
            return {
              end: currentDate.clone()
            };
          },
          'visibleRange': function(currentDate) {
            return {
              start: currentDate.clone().add(-90, 'days'),
              end: currentDate.clone()
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
          if (typeof parts[1] == 'undefined') {
            eventFilters[parts[0]] = '';
          }
          updateEventFilters(eventFilters);

          // Trigger rerender.
          $calendar.fullCalendar('rerenderEvents');
        }
      };

      var buildIcalButton = function () {
        var button = document.createElement('button');
        button.innerHTML = Drupal.t('ICAL');
        button.addEventListener('click', handleICal);
        return button;
      };

      var renderPdf = function (pdfContainer) {
        pdfContainer.removeClass('hidden');
        html2canvas(pdfContainer, {
          onrendered: function(canvas) {
            canvasImage = new Image();
            canvasImage.src= canvas.toDataURL("image/png");
            canvasImage.onload = splitImage;
          }
        });
      };

      var splitImage = function () {
        var a4 = [990.89, 595.28];
        var winHeight = a4[1];
        var contentHeight = $('.pdf-container').height();
        var contentWidth = $('.pdf-container').width();
        var imagePieces = [];
        var totalImgs = Math.round(contentHeight/winHeight) + 1; // Add a page so content doesn't get cut off. Could be cleverer.

        for(var i = 0; i < totalImgs; i++) {
          var canvas = document.createElement('canvas');
          var ctx = canvas.getContext('2d');
          canvas.width = contentWidth;
          canvas.height = winHeight;
          ctx.drawImage(canvasImage, 0, i * winHeight, contentWidth, winHeight, 0, 0, canvas.width, canvas.height);
          imagePieces.push(canvas.toDataURL("image/png"));
        }

        var totalPieces = imagePieces.length - 1;
        var doc = new jsPDF('landscape', 'px', 'A4', true);
        imagePieces.forEach(function(img){
            doc.addImage(img, 'JPEG', 20, 20);
            if(totalPieces) {
              doc.addPage();
            }
            totalPieces--;
        });
        doc.save('export.pdf');
        $('.pdf-container').addClass('hidden');
      };

      var formatPdfHeading = function ($calendar) {
        var headingText = $calendar.find('.fc-toolbar h2').text();
        var heading = $('<h2 />');
        heading.text('Calendar: ' + headingText);
        return heading;
      };

      var formatPdfEvent = function (event, type, monthView) {
        var tableRow = $('<tr />');
        var date = $('<td />');
        var time = $('<td />');
        var title = $('<td />');
        var location = $('<td />');
        var timeSelector = type === 'list' ? '.fc-list-item-time' : '.fc-time';
        var titleSelector = type === 'list' ? '.fc-list-item-title a' : '.fc-title';
        var start = moment($(event).data('start'))
        var startDate = start.format('DD MMMM YYYY');
        var locationString = '';

        // On month view only include events in that month
        if (monthView) {
          var monthDate = moment($('.fc-toolbar h2').text(), 'MMMM YYYY');

          if (monthDate.month() !== start.month()) {
            return false;
          }
        }

        if ($(event).find('.fc-location-details').length) {
          locationString += $(event).find('.fc-location-details').text();
          if ($(event).find('.fc-location').length) {
            locationString += ', ';
          }
        }
        if ($(event).find('.fc-location').length) {
          locationString += $(event).find('.fc-location').text();
        }

        var timeString = $(event).find(timeSelector).text() ? $(event).find(timeSelector).text() : 'All day';
        date.html(startDate);
        time.html(timeString);
        title.html($(event).find(titleSelector).text());
        location.html(locationString);
        tableRow.append(date).append(time).append(title).append(location);
        return tableRow;
      };

      var formatPdfEvents = function (calendar, monthView) {
        var events = [];
        var eventsLength = 0;
        var eventsType;
        var displayEvents = [];
        var displayEventsLength = 0;
        var table = $('<table />');
        var tableHeader = $('<tr />');
        tableHeader.append($('<th>Date</th><th>Time</th><th>Name of meeting</th><th>Location</th>'));
        table.append(tableHeader);

        var eventsList = calendar.find('.fc-list-table'); //list view (upcoming/past events)
        events = eventsList.length ? eventsList.find('.fc-list-item') : calendar.find('.fc-event-container').not('.fc-helper-container').find('.fc-event');
        eventsLength = events.length;
        eventsType = eventsList.length ? 'list' : 'calendar';

        //sort calendar events by date (because the dom for the calendar isnt in order)
        if (!eventsList.length) {
          events.sort(function (a,b) {
            return moment($(a).data('start')) - moment($(b).data('start'));
          });
        }

        for (var i=0; i < eventsLength; i++) {
          displayEvents.push(formatPdfEvent(events[i], eventsType, monthView));
        }
        displayEventsLength = displayEvents.length;
        for (var j = 0; j < displayEventsLength; j++) {
          table.append($(displayEvents[j]));
        }
        return table;
      };

      var formatPdfFilters = function () {
        var filters = $('.calendar-filters').find('.block-views');
        var filtersLength = filters.length;
        var selectedFilters = [];
        var selectedFiltersLength = 0;
        var defaultOptionText = '- Any -';
        var filtersDiv = $('<div />');
        var filtersHeading = $('<h3 />');
        var displayFilters =  $('<ul />');

        for (var i = 0; i < filtersLength; i++) {
          var label = $(filters[i]).find('label').text();
          var option = $(filters[i]).find('select').find('option:selected').text();

          if (option !== defaultOptionText) {
            selectedFilters.push({label: label, value: option});
          }
        }
        selectedFiltersLength = selectedFilters.length;
        if (!selectedFiltersLength) {
          return;
        }
        for (var j = 0; j < selectedFiltersLength; j++) {
          var filterItem = '<li><strong>' + selectedFilters[j].label + ':</strong> ' + selectedFilters[j].value +  '</pli>';
          displayFilters.append($(filterItem));
        }

        filtersDiv.addClass('pdf-filters');
        filtersHeading.text(Drupal.t('Filtered by:'));
        filtersDiv.append(filtersHeading).append(displayFilters);
        return filtersDiv;
      };

      var buildPdfButton = function () {
        $('body').append('<div class="pdf-container hidden" />');
        var pdfContainer = $('.pdf-container');

        var button = document.createElement('button');
        button.innerHTML = Drupal.t('PDF');
        button.addEventListener('click', function (e) {
          e.preventDefault();
          pdfContainer.html('');
          var elements = [];
          var logo = $('.site-header__logo').clone();
          var calendar = $calendar.find('.fc-view-container').clone();
          var monthView = calendar.find('.fc-month-view').length;
          var heading = formatPdfHeading($calendar);
          var table = formatPdfEvents(calendar, monthView);
          var filters = formatPdfFilters();

          pdfContainer.append(logo).append(heading).append(filters).append(table);
          elements.push(pdfContainer);
          renderPdf(pdfContainer);
        });
        return button;
      };

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

      // Build URL parameters for facets.
      var facetURL = '/api/v0/facets?';
      var forcedFilters = Drupal.settings.fullcalendar_api.calendarSettings.events.data;
      for (f in forcedFilters) {
        if (forcedFilters.hasOwnProperty(f) && typeof forcedFilters[f] != 'undefined' && forcedFilters[f] != '') {
          facetURL += f + '=' + forcedFilters[f] + '&';
        }
      }

      $.getJSON($settings.base_url + facetURL, function(facets) {
        var filtersWrapper = document.createElement('div');
        filtersWrapper.className = 'calendar-filters clearfix';

        var filterCount = 0;
        for (var f in facets) {
          var facet = facets[f];
          if (facet.values.length === 0) {
            continue;
          }

          if (typeof eventFilters[f] == 'undefined') {
            continue;
          }

          filterCount++;
          var filter = document.createElement('div');

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
            }).on('chosen:showing_dropdown', function (e, theChosen) {
              addFilterLegend($(this), theChosen.chosen);
            });
          }
        }
        $('.calendar-top').append(filtersWrapper);
        var exportDiv = buildExportOptions();
        $('.calendar-top').append(exportDiv);
        eventFilters = $.extend(eventFilters, defaultFilters);
        // Trigger rerender.
        $calendar.fullCalendar('rerenderEvents');
      });

      var filters = document.querySelectorAll('.block-views');
      if (filters) {

        $('#block-system-main').prepend('<div class="calendar-top"></div>')

        // Add timezone selector.
        var tzDiv = document.createElement('div');
        tzDiv.className += 'calendar-settings';

        var tzToggle = document.createElement('button');
        tzToggle.id = 'timezone-dropdown';
        tzToggle.className += 'calendar-settings__tz-button';
        tzToggle.setAttribute('type', 'button');
        tzToggle.setAttribute('data-toggle', 'dropdown');
        tzToggle.setAttribute('aria-haspopup', 'true');
        tzToggle.setAttribute('aria-expanded', 'false');
        tzToggle.innerHTML = Drupal.t('Time zone: ');
        tzDiv.appendChild(tzToggle);

        var tzDropdown = document.createElement('div');
        tzDropdown.className = 'dropdown-menu';
        tzDropdown.setAttribute('aria-labelledby', 'timezone-dropdown');
        tzDiv.appendChild(tzDropdown);

        var tzLabel = document.createElement('label');
        tzLabel.setAttribute('for', 'timezone-selector');
        tzLabel.innerHTML = Drupal.t('Display times from the following time zone');
        tzDropdown.appendChild(tzLabel);

        var tzSelect = document.createElement('select');
        tzSelect.id = 'timezone-selector';

        tzDropdown.appendChild(tzSelect);

        $(document).on('click', '.calendar-settings .dropdown-menu', function (e) {
          e.stopPropagation();
        });

        $('.calendar-top').append(tzDiv);

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

          tzToggle.innerHTML += currentTz;

          if (Drupal.behaviors && Drupal.behaviors.chosen) {
            $tz.chosen('destroy');
            $tz.addClass('chosen-enable');
            Drupal.behaviors.chosen.attach($tz, Drupal.settings);
            $tz.chosen().change(function(e) {
              handleTimezone(e);
              tzToggle.innerHTML = 'Time zone: ' + $(e.target).find('option:selected').text();
            });
          }
        });

      }
    }
  }
})(jQuery);
