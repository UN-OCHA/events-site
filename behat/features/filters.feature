Feature: Check filters

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
    | Test 1      | Training             | Just testing     | OCHA                     | Agriculture, Health | Earthquake Feb 1991, Earthquake Apr 2016 | Aviation             | Nejrab               | Central Region               |
    | Test 2      | Training             | Just testing     | OCHA                     | Agriculture, Health | Earthquake Apr 2016                      | Aviation             | Nejrab               | Central Region               |
    | Test 3      | Training             | Just testing     | ACHIEVE                  | Health              | Earthquake Feb 1991, Earthquake Apr 2016 | Aviation             | Nejrab               | Central Region               |

@api
Scenario: Check 'Themes' filter
  Given I am on the homepage
  And I wait for AJAX to finish
  Then I should see "Themes"

@api
Scenario: Check 'Location of Event' filter
  Given I am on the homepage
  And I wait for AJAX to finish
  Then I should see "Location of Event"

@api
Scenario: Check 'Disasters' filter
  Given I am on the homepage
  And I wait for AJAX to finish
  Then I should see "Disasters"

@api
Scenario: Check 'Organizations' filter
  Given I am on the homepage
  And I wait for AJAX to finish
  Then I should see "Organizations"

@api
Scenario: Check 'Clusters' filter
  Given I am on the homepage
  And I wait for AJAX to finish
  Then I should see "Clusters"

@api
Scenario: Check 'Categories' filter
  Given I am on the homepage
  And I wait for AJAX to finish
  Then I should see "Categories"
