Feature: See events
  When on the website
  As a visitor
  I should be able to see events

  Background:
    Given I am an anonymous user
    And "ev_coordination_hub" terms:
      | name             |
      | Central Highland |
      | Central Region   |
      | Eastern Region   |
    And "ev_disaster" terms:
      | name                |
      | Earthquake Feb 1991 |
      | Earthquake Apr 2016 |
      | Avalanche Jan 2009  |
    And "ev_list" terms:
      | name             |
      | Aviation         |
      | Child Protection |
      | Nutrition        |
    And "ev_location" with terms:
      | name        | parent      |
      | Capital     | Afghanistan |
      | Kapisa      | Capital     |
      | Alasay      | Kapisa      |
      | Nejrab      | Kapisa      |
      | Logar       | Capital     |
      | Azra        | Logar       |
    And "ev_organization" terms:
      | name          |
      | Act for Peace |
      | OCHA          |
      | ACHIEVE       |
    And "ev_theme" terms:
      | name        |
      | Agriculture |
      | Education   |
      | Health      |
    And list of events:
      | title       | field_event_category | field_event_info | field_event_organization | field_event_theme   | field_event_disasters                    | field_event_cluster  | field_event_location | field_event_coordination_hub |
      | Event1      | Training             | Just testing     | OCHA                     | Agriculture, Health | Earthquake Feb 1991, Earthquake Apr 2016 | Aviation             | Nejrab               | Central Region               |
      | Event2      | Training             | Just testing     | OCHA                     | Agriculture, Health | Earthquake Apr 2016                      | Aviation             | Nejrab               | Central Region               |
      | Event3      | Training             | Just testing     | ACHIEVE                  | Health              | Earthquake Feb 1991, Earthquake Apr 2016 | Aviation             | Nejrab               | Central Region               |
    And I run drush "search-api-index"

  @api
  Scenario: Check homepage
    Given I am on the homepage
    Then I should not see the link "Add event"

  @api
  Scenario: See the event on the homepage
    When I go to "calendar"
    And I wait for AJAX to finish
    Then I should see "Event1"
    And I should see "Event2"
    And I should see "Event3"
