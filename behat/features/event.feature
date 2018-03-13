Feature: Create event
  When I log into the website
  As a HID verified user
  I should be able to create events

  Background:
    Given I am logged in as a user with the "HID verified" role
    And "ev_coordination_hub" terms:
      | name              |
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

  @api
  Scenario: Create an event
    Given I am on the homepage
    Then I should see the link "Add event"

  @api
  Scenario: Create an event
    When I go to "node/add/ev-event"
    And for "title" I enter "xyzzy"
    And I scroll to the "#edit-submit" element
    And I press the "Save" button
    Then I should see "Event xyzzy has been created"
    And I should see the link "Edit"

  @api
  Scenario: See the event on the calendar
    When I go to "node/add/ev-event"
    And for "title" I enter "xyzzy"
    And I scroll to the "#edit-submit" element
    And I press the "Save" button
    And I go to "calendar"
    And I wait for the ajax response
    Then I should see "xyzzy"
