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
      var $calendarId = settings.fullcalendar_api.calendarId;
      var $calendar = $('#' + $calendarId, context);
      if (!$calendar.length) {
        return;
      }

      // Redirect to selected option.
      var handleSelect = function (e) {
        if (e.target.value) {
          data = e.target.value;
          data = data.split("?").pop();
          data = data.replace('f[0]=', '');
          parts = data.split('%3A');

          var $cal = jQuery('#fullcalendar');
          $cal.fullCalendar('getEventSources')[0].data[parts[0]] = parts[1];
          $cal.fullCalendar('refetchEvents');
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
            newOption.text = newLabel.innerText;
            newSelect.appendChild(newOption);

            // Add options.
            for (var j = 0; j < options.length; j++) {
              var option = options[j];
              var newOption = document.createElement('option');
              newOption.value = option.href;
              newOption.text = option.innerText;
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

        // Listen for change events.
        var details = document.querySelector('.region-content').addEventListener("change", handleSelect);
      }
    }
  }
})(jQuery);
