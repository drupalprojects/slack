Description
-----------
This module provides slack integrations.


Configuration
-------------
Go to the configuration page: your-site-url/admin/config/services/slack/config
Admin -> Configuration -> Web services -> Slack -> Configuration.

Get Webhook URL

At first step you should create a webhook integration if you have no one.
You can do it here: https://your-team-domain.slack.com/apps/manage/custom-integrations .
Then you should go to your webhook edit page and copy the wehook URL.
List of your custom integrations you can find here: https://your-team-domain.slack.com/apps/manage/custom-integrations .

See for more information:
  - https://api.slack.com/custom-integrations
  - https://api.slack.com/incoming-webhooks


Messaging testing
-----------------------
Go to the send message page: your-site-url/admin/config/services/slack/test
Admin -> Configuration -> Web services -> Slack -> Send a message.


Rules integrations
------------------
Slack provides action that's called "Send slack message".
If you don't know how to work with the rules module,
you can read the documentation: https://www.drupal.org/node/1866108 .

Useful links (about Rules module):
  - https://www.drupal.org/project/rules
  - https://www.drupal.org/documentation/modules/rules
  - https://fago.gitbooks.io/rules-docs/content/

Developer information
---------------------
This module is developed and maintained by ADCI Solutions(http://www.adcisolutions.com/).
