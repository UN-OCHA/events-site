/**
 * @file
 * Calendar.
 */

/**
 * Exports
 */
var evExports = function ($) {
  'use strict';

  function _buildOption (text, fn) {
    var listItem = $('<li />');
    var button = $('<button type="button">' + Drupal.t(text) + '</button>');
    button.on('click', fn);
    listItem.append(button);
    return listItem;
  }

  function _init () {
    var exportOptionsList = $('<ul class="dropdown-menu" aria-labelledby="export-dropdown"></ul>');
    var exportButton = $('<button type="button" id="export-dropdown" class="calendar-export__button calendar-actions__toggle">' + Drupal.t('Export') + '</button>');
    exportButton.attr('data-toggle', 'dropdown');
    exportButton.attr('aria-haspopup', 'true');
    exportButton.attr('aria-expanded', 'false');

    exportOptionsList.append(_buildOption('ICAL', exportICAL)).append(_buildOption('PDF', exportPDF));
    evCalendar.settings.exportContainer.append(exportButton).append(exportOptionsList);
  }

  function exportICAL () {
    var baseUrl = Drupal.settings.fullcalendar_api.calendarSettings.base_url;
    var url = baseUrl + '/ical?';
    url += $.param(evFilters.settings.eventFilters);
    window.location = url;
  }

  function exportPDF () {
    var calendar = $('.fc-view-container').clone();
    var table = _formatPdfEvents(calendar);
    var filters = evFilters.getCurrentFilters();
    var tableY = 90;
    var docMargin = 40;
    var doc = new jsPDF('l', 'pt');

    // Logo.
    var logoData = 'data:image/jpeg;base64,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';
    doc.addImage(logoData, 'JPEG', doc.internal.pageSize.width - 158, docMargin, 108, 30);

    // Heading.
    var headingText = $('.fc-toolbar h2').text();
    var calendarHeading = Drupal.t('CALENDAR') + ': ' + headingText.toUpperCase();
    doc.setFontSize(18);
    doc.setFontType('bold');
    doc.setTextColor(0,122,192);
    doc.text(calendarHeading, docMargin, 60);

    // Filters.
    if (filters) {
      doc.setFontSize(10);
      doc.setTextColor(0);
      doc.setFontType('bold');
      doc.text(Drupal.t('Filter Criteria') + ': ', docMargin, tableY);

      doc.setTextColor(0,122,192);
      doc.text(filters, docMargin + 70, tableY);
      tableY = tableY + 20;
    }

    // Events.
    doc.autoTable(table.cols, table.rows, {
      startY: tableY,
      styles: {overflow: 'linebreak', columnWidth: 'wrap'},
      columnStyles: {
        name: {columnWidth: 'auto'},
        location: {columnWidth: 'auto'}
      },
      addPageContent: function (data) {
        _getPdfFooter(data, doc);
      }
    });

    // Add page numbers.
    doc.setFontSize(8);
    doc.setFontType('normal');
    var totalPages = doc.internal.getNumberOfPages();
    for (var i = 0; i < totalPages; i++) {
      var realPageNumber = i + 1;
      doc.setPage(realPageNumber);
      doc.text(Drupal.t('Page') + ' ' + realPageNumber + '/' + totalPages, docMargin, doc.internal.pageSize.height - 25);
    }

    // Save.
    var fileName = 'events-';
    var fileNameHeading = headingText.replace(' – ', '-');
    fileNameHeading = fileNameHeading.replace(',', '');
    fileNameHeading = fileNameHeading.replace(/\s+/g, '-').toLowerCase();
    doc.save(fileName + fileNameHeading + '.pdf');
  }

  function _formatPdfEvents (calendar) {
    var events = [];
    var eventsLength = 0;
    var eventsType;
    var displayEvents = [];
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

    var timeSelector = eventsType === 'list' ? '.fc-list-item-time' : '.fc-time';
    var titleSelector = eventsType === 'list' ? '.fc-list-item-title a' : '.fc-title';

    for (var i=0; i < eventsLength; i++) {
      var event = events[i];
      var start = moment($(event).data('start'));
      var startDate = start.format('DD MMMM YYYY');
      var locationString = '';
      var timeString = $(event).find(timeSelector).text() ? $(event).find(timeSelector).text() : 'All day';
      if ($(event).find('.fc-location-details').length) {
        locationString += $(event).find('.fc-location-details').text();
        if ($(event).find('.fc-location').length) {
          locationString += ', ';
        }
      }
      if ($(event).find('.fc-location').length) {
        locationString += $(event).find('.fc-location').text();
      }

      displayEvents.push(
        {
          'date': startDate,
          'time': timeString,
          'name': $(event).find(titleSelector).text(),
          'location': locationString
        }
      );
    }

    var columns = [
      {
        title: 'Date',
        dataKey: 'date'
      },
      {
        title: 'Time',
        dataKey: 'time'
      },
      {
        title: 'Name of meeting',
        dataKey: 'name'
      },
      {
        title: 'Location',
        dataKey: 'location'
      }
    ];

    return {
      cols: columns,
      rows: displayEvents
    };
  }

  function _getPdfFooter (data, doc) {
    var createdAt = 'Created: ' + moment().format('DD MMM YYYY');
    var poweredBy = 'Powered by Humanitarian Events. https://events.rwlabs.org';
    doc.setFontSize(8);
    doc.setFontType('normal');
    doc.text(createdAt, doc.internal.pageSize.width - 118, doc.internal.pageSize.height - 35);
    doc.text(poweredBy, doc.internal.pageSize.width - 250, doc.internal.pageSize.height - 25);
  }

  return {
    init: _init
  };

}(jQuery);

/**
 * Filters
 */
var evFilters = function ($) {
  'use strict';

  var settings = {
  };

  function _addFilterLegend (select, chosen, categories) {
    var type = 'cat';
    if (select.find('option:first-child').val() !== type) { // Only add legends to category filter
      return;
    }
    var options = $(chosen.search_results).find('li');
    var optionsLength = options.length;
    var i = 0;
    for (i; i < optionsLength; i++) {
      var value = select.find('option').eq(i).val();
      var id = value.replace(type + ':', '');
      var color = _getLegendColor(id, categories);
      if (color) {
        var legend = '<span class="chosen-legend" style="background-color:' + color + '"></span>';
        $(options[i]).addClass('chosen-option-has-legend').prepend(legend);
      }
    }
  }

  function _buildAllFilters (facets) {
    var filterCount = 0;

    for (var f in facets) {
      var facet = facets[f];
      if (facet.values.length === 0) {
        continue;
      }

      if (typeof settings.eventFilters[f] === 'undefined') {
        continue;
      }

      filterCount++;

      var filter = $('<div class="calendar-filters--' + f + ' processed block-views"></div>');
      var newLabel = $('<label for="filter-' + filterCount + '">' + facet.label + '</label>');
      var newSelect = $('<select class="chosen-enable" data-type="' + f + '" id="filter-' + filterCount + '"></select>');
      var emptyOption = $('<option value="' + f + '">' + Drupal.t('- Any -') + '</option>');
      newSelect.append(emptyOption);

      // Flip key and value.
      var flipped = [];
      for (var key in facet.values) {
        flipped.push({
          'key': key,
          'label': facet.values[key]
        });
      }

      // Sort by label.
      flipped.sort(function(a, b) {
        return a.label.localeCompare(b.label);
      });

      // Add options.
      for (var o = 0; o < flipped.length; o++) {
        var option = flipped[o];
        var newOption = $('<option value="' + f + ':' + option.key + '">' + option.label + '</option>');
        if (settings.defaultFilters[f] && settings.defaultFilters[f] == o.key) {
          newOption.selected = 'selected';
        }
        newSelect.append(newOption);
      }

      filter.append(newLabel).append(newSelect);
      settings.filtersWrapperInner.append(filter);

      _registerChosenEvents(newSelect);
    }
  }

  function _buildHTML () {
    settings.filtersWrapperInner = $('<div class="calendar-filters__inner dropdown-menu clearfix"></div>');
    var currentFiltersWrapper = $('<div class="calendar-filters__current hidden"></div>');
    var filtersHeading = '<button type="button" data-toggle="dropdown" class="calendar-actions__toggle">' + Drupal.t('Filter events')+ '</button>';
    var clearBtn = '<button type="button" class="calendar-filters__clear btn-icon"><i class="icon-cancel"></i>Clear filters</button>';
    evCalendar.settings.filtersContainer.append(filtersHeading).append(settings.filtersWrapperInner).append(currentFiltersWrapper);
    currentFiltersWrapper.append('<p />').append(clearBtn);
  }

  function _changeFilter (e) {
    _updateCurrentFilters();

    if (e.target.value) {
      var data = e.target.value;
      var parts = data.split(':');

      settings.eventFilters[parts[0]] = parts[1];
      if (typeof parts[1] === 'undefined') {
        settings.eventFilters[parts[0]] = '';
      }

      evCalendar.updateState(true);
      evCalendar.settings.$calendar.fullCalendar('rerenderEvents');
    }
  }

  function _clearFilters () {
    for (var f in settings.defaultFilters) {
      settings.defaultFilters[f] = '';
    }
    $('select.chosen-enable').find('option:first-child').prop('selected', true).end().trigger('chosen:updated');
    _update(settings.defaultFilters);
    evCalendar.updateState(true);
    _updateCurrentFilters();
    evCalendar.settings.$calendar.fullCalendar('rerenderEvents');
  }

  function _getCurrentFilters () {
    var filters = $('.calendar-filters').find('.block-views');
    var filtersLength = filters.length;
    var selectedFilters = [];
    var selectedFiltersLength = 0;
    var defaultOptionText = '- Any -';
    var str = '';

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
      str += selectedFilters[j].value;
      if (j+1 < selectedFiltersLength) {
        str += ', ';
      }
    }

    return str;
  }

  function _getFacetsUrl () {
    var facetURL = '/api/v0/facets?';
    var forcedFilters = Drupal.settings.fullcalendar_api.calendarSettings.events.data;
    for (var f in forcedFilters) {
      if (forcedFilters.hasOwnProperty(f) && typeof forcedFilters[f] !== 'undefined' && forcedFilters[f] !== '') {
        facetURL += f + '=' + forcedFilters[f] + '&';
      }
    }
    return facetURL;
  }

  function _getLegendColor (id, categories) {
    var categoriesLength = categories.length;
    var i = 0;
    var color;
    for (i; i < categoriesLength; i++) {
      if (categories[i].tid === id) {
        color = categories[i].color;
      }
    }
    return color;
  }

  function _init () {
    settings.defaultFilters = Drupal.settings.fullcalendar_api.calendarSettings.defaultFilters;
    settings.eventFilters = Drupal.settings.fullcalendar_api.calendarSettings.availableFilters;
    _buildHTML();
    var baseUrl = Drupal.settings.fullcalendar_api.calendarSettings.base_url;
    $.getJSON(baseUrl + _getFacetsUrl(), function(facets) {
      _buildAllFilters(facets);
      _update($.extend(settings.eventFilters, settings.defaultFilters));
      _updateFilterSelects();
      evCalendar.settings.$calendar.fullCalendar('rerenderEvents');
    });

    $(document).on('click', '.calendar-filters__clear', function () {
      _clearFilters();
    });
  }

  function _registerChosenEvents (newSelect) {
    if (Drupal.behaviors && Drupal.behaviors.chosen) {
      Drupal.behaviors.chosen.attach(newSelect, Drupal.settings);
      newSelect.chosen().change(function(e) {
        _changeFilter(e);
      }).on('chosen:showing_dropdown', function (e, theChosen) {
        _addFilterLegend($(this), theChosen.chosen, Drupal.settings.fullcalendar_api.calendarSettings.categories);
      });
    }
  }

  function _update (filters) {
    settings.eventFilters = $.extend({}, filters);
  }

  function _updateCurrentFilters () {
    var currentFilters = _getCurrentFilters();
    var currentFiltersDiv = $('.calendar-filters__current');
    if (!currentFilters) {
      currentFiltersDiv.addClass('hidden');
      $('.calendar-actions').removeClass('calendar-actions--filtered');
      return;
    }
    var currentFiltersHtml = '<strong>Current filters:</strong> ' + currentFilters;
    currentFiltersDiv.find('p').html(currentFiltersHtml);
    currentFiltersDiv.removeClass('hidden');
    $('.calendar-actions').addClass('calendar-actions--filtered');
  }

  function _updateFilterSelects () {
    for (var f in settings.eventFilters) {
      if (settings.eventFilters[f]) {
        $('[data-type="' + f + '"]').val(f + ':' + settings.eventFilters[f]).trigger('chosen:updated');
      }
    }
    _updateCurrentFilters();
  }

  return {
    getCurrentFilters: _getCurrentFilters,
    settings: settings,
    init: _init,
    update: _update,
    updateFilterSelects: _updateFilterSelects
  };

}(jQuery);

/**
 * Calendar
 */
var evCalendar = function ($) {
  'use strict';

  var settings = {};

  function _addEventDetails (event, element, view) {
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

      // Add more details to past events.
      if (view.name === 'past') {
        if (event.description) {
          element.find('.fc-list-item-title').html(element.find('.fc-list-item-title').html() + '<span class="fc-description">' + event.description + '</span>');
        }
        if (event.files && event.files.length > 0) {
          var ul = $('<ul class="ev-files"></ul');
          for (var i = 0; i < event.files.length; i++) {
            ul.append('<li class="ev-doc-' + event.files[i].type_human.toLowerCase().replace(/[^0-9a-z]/gi,'-') + '"><a href="' + event.files[i].uri + '" target="_blank">' + event.files[i].name + '</a></li>');
          }
          element.find('.fc-list-item-title').append(ul);
        }
      }
    }
  }

  function _formatControls () {
    settings.calendarControls = $('.fc-toolbar .fc-left .fc-button-group');
    settings.calendarControls.addClass('fc-toolbar-controls');
    settings.calendarControls.find('.fc-prev-button span').removeClass('fc-icon fc-icon-left-single-arrow')
      .addClass('sr-only')
      .text('Previous')
      .after('<i class="icon-arrow-left"></i>');
    settings.calendarControls.find('.fc-next-button span').removeClass('fc-icon fc-icon-right-single-arrow')
      .addClass('sr-only')
      .text('Next')
      .after('<i class="icon-arrow-right"></i>');

    $('.fc-center').prepend(settings.calendarControls);

    $('.fc-toolbar .fc-today-button').html('<span>Go to </span>' + $('.fc-today-button').text());
    _formatViewSettings();

  }

  function _buildHTML () {
    settings.actionsContainer = $('<div class="calendar-actions"></div>');
    settings.filtersContainer = $('<div class="calendar-filters"></div>');
    settings.exportContainer = $('<div class="calendar-export"></div>');
    settings.timeZoneContainer = $('<div class="calendar-settings"></div>');
    settings.actionsContainer.append(settings.filtersContainer).append(settings.timeZoneContainer).append(settings.exportContainer);
    $('#block-system-main').prepend(settings.actionsContainer);
    $('.calendar-export, .calendar-settings').wrapAll('<div />');

    var sidebarBtn = '<button type="button" class="calendar-sidebar-btn btn-icon "><i class="icon-settings"></i><span>' + Drupal.t('Options') + '</span></button>';
    var closeBtn = '<button type="button" class="calendar-actions__close btn-icon "><i class="icon-cancel"></i><span>' + Drupal.t('Close') + '</span></button>';
    $('.calendar-actions').append(closeBtn).before(sidebarBtn);
    $('body').append('<div class="sidebar-underlay hidden"></div>');
    $(document).on('click', '.calendar-sidebar-btn, .calendar-actions__close', _toggleSidebar);
    $('.sidebar-underlay').on('click', _toggleSidebar); // separate click event for underlay to make it work in mobile safari.
    settings.sidebarOpen = false;
    settings.viewToggleContainer = $('<div class="calendar-view-selector"></div>');
    settings.viewToggle = $('<button type="button" id="viewSelector" class="calendar-actions__toggle" data-toggle="dropdown"/>');
    settings.viewToggle.html(Drupal.t('Showing: ') + '<span></span>');
    settings.viewToggle.attr('aria-haspopup', 'true');
    settings.viewToggle.attr('aria-expanded', 'false');
  }

  function _formatViewSettings () {
    var content = $('.fc-toolbar .fc-right .fc-button-group');
    content.removeClass('fc-button-group').addClass('dropdown-menu').attr('aria-labelledby', 'viewSelector');
    content.find('button').removeClass('fc-state-default');
    settings.viewToggleContainer.append(settings.viewToggle).append(content);
    settings.actionsContainer.append(settings.viewToggleContainer);
  }

  function _init () {
    var $calendarId = Drupal.settings.fullcalendar_api.calendarId;
    var $calendar = $('#' + $calendarId);
    if (!$calendar.length) {
      return;
    }
    settings.$calendar = $calendar;
    settings.state = {
      'view': 'month',
      'date': '',
    };

    settings.$settings = Drupal.settings.fullcalendar_api.calendarSettings;

    _buildHTML();

    // Needed to fix navigation problem on past events.
    var alreadyTrigger = false;

    $.extend(settings.$settings.events, {
      timeout: 5000,
      success: function() {
        $('.fc-view').removeClass('fc-view--error');
        $('.fc-loading-message, .fc-loading-message--error').remove();
      },
      error: function() {
        var errorMessage = '<div class="fc-loading-message fc-loading-message--error">' + Drupal.t('There was an error fetching events, please try again') + '</div>';
        $('.fc-view').addClass('fc-view--error').before(errorMessage);
      }
    });

    $.extend(settings.$settings, {
      loading: function(isLoading, view) {
        if (isLoading) {
          var loadingMessage = '<div class="fc-loading-message">' + Drupal.t('Please wait while we fetch events') + '</div>';
          view.el.addClass('fc-view--loading').before(loadingMessage);
          return;
        }
        if (!view.el.hasClass('fc-view--error')) {
          view.el.removeClass('fc-view--loading');
          $('.fc-loading-message').fadeOut();
        }
      },
      eventLimit: false,
      eventRender: function(event, element, view) {
        element.attr('data-start', event.start._i);

        for (var f in evFilters.settings.eventFilters) {
          if (evFilters.settings.eventFilters.hasOwnProperty(f) && event.hasOwnProperty(f) && typeof evFilters.settings.eventFilters[f] !== 'undefined' && evFilters.settings.eventFilters[f] !== '' && event[f].indexOf(evFilters.settings.eventFilters[f]) === -1) {
            return false;
          }
        }
        _addEventDetails(event, element, view);
        return true;
      },
      height: 'auto',
      titleFormat: 'DD MMM YYYY',
      listDayFormat: 'DD MMMM YYYY',
      views: {
        week: {
          columnFormat: 'ddd DD/MM'
        }
      },
      viewRender: function(view) {
        // Store view.name, view.start and view.end
        settings.state.view = view.name;
        settings.state.date = settings.$calendar.fullCalendar('getDate').toISOString();
        _updateState();
        _updateViewSettings();

        if (view.name === 'upcoming') {
          if (settings.$calendar.fullCalendar('getDate').unix() < moment().add(-1, 'days').unix()) {
            settings.$calendar.fullCalendar('gotoDate', moment());
          }
        }
        else if (view.name === 'past') {
          if (!alreadyTrigger && settings.$calendar.fullCalendar('getDate').toISOString() >= moment().format('Y-MM-DD')) {
            settings.$calendar.fullCalendar('gotoDate', moment().add(0, 'days'));
            alreadyTrigger = true;
            window.setTimeout(function () {
              settings.$calendar.fullCalendar('prev');
              alreadyTrigger = false;
            }, 250);
          }
        }
      }
    });

    $.extend(settings.$settings.views, {
      'upcoming': {
        'type': 'list',
        'buttonText': 'Upcoming',
        'duration': {
          'days': 7
        },
        'visibleRange': function(currentDate) {
          return {
            start: currentDate.clone(),
            end: currentDate.clone().add(7, 'days')
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

    settings.$calendar.fullCalendar(settings.$settings);

    _formatControls();
    _updateViewSettings();
    _sidebarSwipe();
  }

  function _parseQuery(qstr) {
    var query = {};
    var a = (qstr[0] === '?' ? qstr.substr(1) : qstr).split('&');
    for (var i = 0; i < a.length; i++) {
        var b = a[i].split('=');
        query[decodeURIComponent(b[0])] = decodeURIComponent(b[1] || '');
    }
    return query;
  }

  function _sidebarSwipe () {
    var startX = 0;
    var endX = 0;
    document.querySelector('body').addEventListener("touchstart", function (e) {
      startX = e.targetTouches[0].pageX;
    }, false);
    document.querySelector('body').addEventListener("touchend", function (e) {
      endX = e.changedTouches[0].pageX;
      if (settings.sidebarOpen && endX < startX - 120) { // arbitrary distance to swipe to trigger close
        _toggleSidebar();
      }
    }, false);
  }

  function _toggleSidebar () {
    if (!settings.actionsContainer.hasClass('active')) {
      settings.actionsContainer.find('button').first().focus();
    }
    settings.actionsContainer.toggleClass('active');
    $('.sidebar-underlay').toggleClass('hidden');
    $('body').toggleClass('no-scroll');
    settings.sidebarOpen = !settings.sidebarOpen;
  }

  function _updateState (isFiltering) {
    var currentFilters = evFilters.settings.eventFilters ? evFilters.settings.eventFilters : {};

    if (!isFiltering) {
      var qsObj = _parseQuery(window.location.search);
      for (var p in Drupal.settings.fullcalendar_api.calendarSettings.defaultFilters) {
        if (qsObj.hasOwnProperty(p)) {
          currentFilters[p] = qsObj[p];
        }
      }
      evFilters.update(currentFilters);
    }
    $.extend(settings.state, currentFilters);
    var path = '?';
    for (var f in settings.state) {
      if (settings.state.hasOwnProperty(f) && typeof settings.state[f] !== 'undefined' && settings.state[f] !== '') {
        path += f + '=' + settings.state[f] + '&';
      }
    }
    if (history.replaceState) {
      history.replaceState(settings.state, '', path);
    }
  }

  function _updateViewSettings () {
    $('.calendar-view-selector .fc-button').removeClass('fc-state-active');
    var selected = $('.calendar-view-selector .fc-' + settings.state.view + '-button');
    selected.addClass('fc-state-active');
    var label = selected.text() ? selected.text() : settings.state.view;
    settings.viewToggle.find('span').text(label);
  }

  return {
    init: _init,
    settings: settings,
    updateState: _updateState
  };

}(jQuery);

/**
 * Timezone
 */
var evTimeZone = function ($) {
  'use strict';

  var settings = {};

  function _buildHTML () {
    var toggle = $('<button type="button id="timezone-dropdown" class="calendar-settings__tz-button calendar-actions__toggle">' + Drupal.t('Time zone: ') +'</button>');
    var dropdown = $('<div class="calendar-settings__tz-dropdown dropdown-menu" aria-labelledby="timezone-dropdown"></div>');
    var label = $('<label for="timezone-selector">' + Drupal.t('Display times from the following time zone') + '</label>');
    var select = $('<select id="timezone-selector"></select>');
    toggle.attr('data-toggle', 'dropdown');
    toggle.attr('aria-haspopup', 'true');
    toggle.attr('aria-expanded', 'false');
    settings.toggle = toggle;
    evCalendar.settings.timeZoneContainer.append(toggle).append(dropdown);
    dropdown.append(label).append(select);
  }

  function _changeTimezone (e) {
    if (e.target.value) {
      var data = e.target.value;
      var filters = $.extend({}, evFilters.settings.eventFilters);
      filters.timezone = data;
      evFilters.update(filters);
      settings.toggle.text('Time zone: ' + $(e.target).find('option:selected').text());
      evCalendar.settings.$calendar.fullCalendar('option', 'timezone', data);
      $('.calendar-settings__tz-button').click();
    }
  }

  function _getTimeZones () {
    var baseUrl = Drupal.settings.fullcalendar_api.calendarSettings.base_url;

    $.getJSON(baseUrl + '/api/v0/timezones', function(timezones) {
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
      settings.toggle.text(settings.toggle.text() + ' ' + currentTz);

      if (Drupal.behaviors && Drupal.behaviors.chosen) {
        $tz.chosen('destroy');
        $tz.addClass('chosen-enable');
        Drupal.behaviors.chosen.attach($tz, Drupal.settings);
        $tz.chosen().change(function(e) {
          _changeTimezone(e);
        });
      }
    });

  }

  function _init () {
    _buildHTML();
    _getTimeZones();
  }

  return {
    init: _init
  };

}(jQuery);

(function($) {
  Drupal.behaviors.eventsEvent = {
    attach: function(context, settings) {
      'use strict';

      if (!settings.fullcalendar_api.calendarSettings) {
        return;
      }

      evCalendar.init();
      evFilters.init();
      evTimeZone.init();
      evExports.init();

      // Prevent the filters etc dropdowns closing when click on their contents
      $(document).on('click', '.calendar-filters .dropdown-menu, .calendar-settings .dropdown-menu', function (e) {
        e.stopPropagation();
      });
    }


  };
})(jQuery);
