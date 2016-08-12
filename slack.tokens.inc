<?php

/**
 * @file
 * Token integration.
 */

/**
 * Implements hook_info().
 */
function slack_token_info() {

  $types = array(
    'name' => t('Slack'),
    'description' => 'Slack tokens',
  );

  $tokens['slack-channel'] = array(
    'name' => t('Channel'),
    'description' => t('Name of channel.'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );
  $tokens['slack-default-webhook-url'] = array(
    'name' => t('Webhook URL'),
    'description' => t('Webhook URL which defined on settings page.'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );
  $tokens['slack-icon-url'] = array(
    'name' => t('Icon URL'),
    'description' => t('Absolute Url to image you would like to use for your Slackbot.'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );
  $tokens['slack-icon-emoji'] = array(
    'name' => t('Emoji code'),
    'description' => t('Emoji code you would like to use for your Slackbot.'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );
  $tokens['slack-username'] = array(
    'name' => t('Username'),
    'description' => t('Default username for your Slackbot.'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );

  return array(
    'types' => array('slack' => $types),
    'tokens' => array('slack' => $tokens),
  );
}

/**
 * Implements hook_tokens().
 */
function slack_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ($type == 'slack') {
    $sanitize = !empty($options['sanitize']);
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'slack-channel':
          $replacements[$original] = $sanitize ? check_plain(slack_get_default_channel()) : slack_get_default_channel();
          break;

        case 'slack-default-webhook-url':
          $replacements[$original] = $sanitize ? check_plain(slack_get_default_webhook_url()) : slack_get_default_webhook_url();
          break;

        case 'slack-icon-url':
          $replacements[$original] = $sanitize ? check_plain(slack_get_default_icon_url()) : slack_get_default_icon_url();
          break;

        case 'slack-icon-emoji':
          $replacements[$original] = $sanitize ? check_plain(slack_get_default_icon_emoji()) : slack_get_default_icon_emoji();
          break;

        case 'slack-username':
          $replacements[$original] = $sanitize ? check_plain(slack_get_default_username()) : slack_get_default_username();
          break;
      }
    }
  }

  return $replacements;
}
