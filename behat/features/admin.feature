Feature: Content Management
  When I log into the website
  As an administrator
  I should see default content

  Background:
    Given I am logged in as a user with the "administrator" role
    And "ev_coordination_hub" terms:
      | name                         |
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
  Scenario: An administrative user should see category terms
    When I go to "admin/structure/taxonomy/ev_category"
    Then I should see "Conference"
    And I should see "Meeting"
    And I should see "Training"
    And I should see "Workshop"

  @api
  Scenario: An administrative user should see document type terms
    When I go to "admin/structure/taxonomy/ev_document_type"
    Then I should see "Agenda"
    And I should see "Meeting Minutes"
    And I should see "Presentation"

  @api
  Scenario: An administrative user should see coordination hub terms
    When I go to "admin/structure/taxonomy/ev_coordination_hub"
    Then I should see "Central Highland"
    And I should see "Central Region"
    And I should see "Eastern Region"

  @api
  Scenario: An administrative user should see disaster terms
    When I go to "admin/structure/taxonomy/ev_disaster"
    Then I should see "Earthquake Feb 1991"
    And I should see "Earthquake Apr 2016"
    And I should see "Avalanche"

  @api
  Scenario: An administrative user should see list terms
    When I go to "admin/structure/taxonomy/ev_list"
    Then I should see "Aviation"
    And I should see "Child Protection"
    And I should see "Nutrition"

  @api
  Scenario: An administrative user should see location terms
    When I go to "admin/structure/taxonomy/ev_location"
    Then I should see "Afghanistan"
    And I should see "Nejrab"
    And I should see "Azra"

  @api
  Scenario: An administrative user should see organisation terms
    When I go to "admin/structure/taxonomy/ev_organization"
    Then I should see "Act for Peace"
    And I should see "OCHA"
    And I should see "ACHIEVE"

  @api
  Scenario: An administrative user should see theme terms
    When I go to "admin/structure/taxonomy/ev_theme"
    Then I should see "Agriculture"
    And I should see "Education"
    And I should see "Health"

  @api
  Scenario: An administrative user create an event
    When I go to "node/add/ev-event"
    And for "title" I enter "xyzzy"
    And I scroll to the "#edit-submit" element
    And I press the "Save" button
    Then I should see "Event xyzzy has been created"

  @api
  Scenario: A user should see the event on the homepage
    When I go to "node/add/ev-event"
    And for "title" I enter "xyzzy"
    And I scroll to the "#edit-submit" element
    And I press the "Save" button
    And I go to "calendar"
    And I wait for the ajax response
    Then I should see "xyzzy"
