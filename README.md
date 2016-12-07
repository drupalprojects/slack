# Slack module
## Description
This module allows you to send messages from Drupal website to Slack.
It has Rules module integration.
Also you can use our module API in your modules.
 
## Installation instructions:
 1. Install the module as usual.
 2. Go to admin/config/services/slack/config and enter your Slack Webhook URL.  It looks like  https://hooks.slack.com/services/XXXXXXXXX/YYYYYYYYY/ZZZZZZZZZZZZZZZZZZZZZZZZ
 3. If you want to enable the Rules-based Slack integration, you will need to download the Rules module.

## Configurations
Go to the configuration page: your-site-url/admin/config/services/slack/config  
 ```Admin -> Configuration -> Web services -> Slack -> Configuration.```

### Get Webhook URL
At first step you should create a webhook integration if you have no one.
You can do it here: https://your-team-domain.slack.com/apps/manage/custom-integrations .
Then you should go to your webhook edit page and copy the wehook URL.
List of your custom integrations you can find here: https://your-team-domain.slack.com/apps/manage/custom-integrations .
See for more information:
  - https://api.slack.com/custom-integrations
  - https://api.slack.com/incoming-webhooks

### Messaging testing
Go to the send message page: your-site-url/admin/config/services/slack/test  
 ```Admin -> Configuration -> Web services -> Slack -> Send a message.```

## Rules integrations
Slack provides action called "Send slack message".

Useful links (about Rules module):
  - https://www.drupal.org/project/rules
  - https://www.drupal.org/documentation/modules/rules
  - http://d8rules.org/
  - https://fago.gitbooks.io/rules-docs/content/

## Developer information
This module is developed and maintained by [ADCI Solutions](http://www.adcisolutions.com/).

