This module adds context filtering capabilities to the Drupal core admin
translation interface.

Impacted pages
Configuration > Regional and Language > Translate interface > Translate
Allows to filter the translated strings by context.

Configuration > Regional and Language > Translate interface > Export
Allows to export strings for a specific context.


Use case
Using contexts when writing custom code is a good practice. It allows to clearly
separate your custom strings from the core and contrib modules strings, and to
be able to translate the same string differently if the conext of the word is
not the same.

Example of a translation string using context:
t('This is my custom string', array(), array('context' => 'custom_project');

Unfortunately, the Drupal core does not provide any functionalities in the admin
interface to make use of the strings translation contexts. This module adds
basic functionalities to manipulate contexts.

Developers remarks:
The Drupal core administration functions related to string translation are not
very extendable, so to implement new functionalities without patching the Drupal
core I had to copy-paste some of the core functions in this module to be able to
add new behaviors to them.
You can find these rewritten functions in
locale_translation_context.rewrite.inc.
