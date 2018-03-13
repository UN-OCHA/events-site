Feature: See basic page
  When on the website
  As a visitor
  I should be able to see pages

  Background:
    Given I am an anonymous user
  @api
  Scenario: Check basic page
    Given I am viewing a page:
      | title         | The title     |
      | body          | Some text     |
      | field_sidebar | Sidebar text  |
    Then I should see "The title"
    And I should see "Some text"
    And I should see "Sidebar text"
