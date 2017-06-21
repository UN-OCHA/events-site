<?php

use Drupal\DrupalExtension\Context\RawDrupalContext;
use Behat\Behat\Context\SnippetAcceptingContext;
use Behat\Gherkin\Node\PyStringNode;
use Behat\Gherkin\Node\TableNode;
use Behat\Behat\Tester\Exception\PendingException;
use Behat\Mink\Driver\Selenium2Driver;
use Behat\Mink\Exception\ElementNotFoundException;
use Behat\Behat\Hook\Scope\AfterStepScope;
use Behat\Behat\Hook\Scope\BeforeStepScope;

/**
 * Defines application features from the specific context.
 */
class FeatureContext extends RawDrupalContext implements SnippetAcceptingContext {

  public $screenshots;
  public $lastStep = 'none';

  /**
   * Initializes context.
   *
   * Every scenario gets its own context instance.
   * You can also pass arbitrary arguments to the
   * context constructor through behat.yml.
   */
  public function __construct($screenshots) {
    $this->screenshots = $screenshots ? $screenshots : '/tmp';
  }

  /**
   * @Then /^I wait for the ajax response$/
   */
  public function iWaitForTheAjaxResponse()
  {
      $this->getSession()->wait(5000, '(0 === jQuery.active)');
  }

  /**
   * @Given /^I select "([^"]*)" from chosen\.js "([^"]*)"$/
   */
  public function iSelectFromChosenJsSelectBox($option, $select) {
    $select = $this->fixStepArgument($select);
    $option = $this->fixStepArgument($option);

    $page = $this->getSession()->getPage();
    $field = $page->findField($select, true);

    if (null === $field) {
      throw new ElementNotFoundException($this->getDriver(), 'form field', 'id|name|label|value', $select);
    }

    $id = $field->getAttribute('id');
    $opt = $field->find('named', array('option', $option));
    $val = $opt->getValue();

    $javascript = "jQuery('#$id').val('$val');
                  jQuery('#$id').trigger('chosen:updated');
                  jQuery('#$id').trigger('change');";

    $this->getSession()->executeScript($javascript);
  }

  /**
   * Creates one or more terms on an existing vocabulary.
   *
   * Provide term data in the following format:
   *
   * | name  | parent | description | weight | taxonomy_field_image |
   * | Snook | Fish   | Marine fish | 10     | snook-123.jpg        |
   * | ...   | ...    | ...         | ...    | ...                  |
   *
   * Only the 'name' field is required.
   *
   * @Given :vocabulary with terms:
   */
  public function createTerms($vocabulary, TableNode $termsTable) {
    foreach ($termsTable->getHash() as $termsHash) {
      $term = (object) $termsHash;
      if (isset($term->parent)) {
        $term->parent = trim($term->parent);
      }
      $term->vocabulary_machine_name = $vocabulary;
      $this->_termCreateWithParent($term);
    }
  }

  /**
   * Create a term.
   *
   * @return object
   *   The created term.
   */
  public function _termCreateWithParent($term) {
    if (!empty($term->parent)) {
      if (!$parent = $this->_getExistingTerm($term->parent, $term->vocabulary_machine_name)) {
        $parent = $this->termCreate((object) array(
          'vocabulary_machine_name' => $term->vocabulary_machine_name,
          'name' => $term->parent,
        ));
      }
      $term->parent = $parent->tid;
    }
    $this->dispatchHooks('BeforeTermCreateScope', $term);
    $this->parseEntityFields('taxonomy_term', $term);
    $saved = $this->getDriver()->createTerm($term);
    $this->dispatchHooks('AfterTermCreateScope', $saved);
    $this->terms[] = $saved;
    return $saved;
  }

  /**
   * Returns a testing term.
   *
   * @param string $name
   *   The term name to check.
   * @param string $vid
   *   The term vocabulary ID.
   *
   * @return \stdClass|null
   *   The term object or NULL if no term has been created yet.
   */
  protected function _getExistingTerm($name, $vid) {
    foreach ($this->terms as $term) {
      if ($term->name === $name && $term->vocabulary_machine_name === $vid) {
        return $term;
      }
    }
    return NULL;
  }

  /**
   * Creates content of a given type provided in the form:
   *
   * @Given list of events:
   */
  public function createEvents(TableNode $nodesTable) {
    foreach ($nodesTable->getHash() as $nodeHash) {
      if (!isset($nodeHash['field_event_date:value'])) {
        $nodeHash['field_event_date:value'] = format_date(REQUEST_TIME, 'custom', 'Y-m-d\TH:i:s');
        $nodeHash['field_event_date:value2'] = format_date(REQUEST_TIME + rand(15 * 60, 3 * 60 * 60), 'custom', 'Y-m-d\TH:i:s');
      }
      $node = (object) $nodeHash;
      $node->type = 'ev_event';
      $this->nodeCreate($node);
    }
  }

  /**
   * @BeforeStep
   */
  public function trackLastStep(BeforeStepScope $scope) {
    $this->lastStep = $scope->getStep();
  }

  /**
   * @AfterStep
   */
  public function takeScreenShotAfterFailedStep(afterStepScope $scope) {
    if (99 === $scope->getTestResult()->getResultCode()) {
      $driver = $this->getSession()->getDriver();
      if (!($driver instanceof Selenium2Driver)) {
        return;
      }
      $filename = preg_replace('/[^a-zA-Z0-9]/', '-', $this->lastStep->getText());
      file_put_contents($this->screenshots . '/' . $filename . '.png', $this->getSession()->getDriver()->getScreenshot());
    }
  }

}
