<?php

/**
 * @file
 * Slack integration module rules functions.
 */

/**
 * Implements hook_rules_action_info().
 */
function slack_rules_action_info() {
  $actions = array();
  $actions['slack_send_message'] = array(
    'base' => 'slack_rules_send_message_action',
    'label' => t('Slack send message'),
    'arguments' => array(
      'message' => array(
        'type' => 'string',
        'label' => t('Sending message'),
      ),
      'channel' => array(
        'type' => 'string',
        'label' => t('Channel'),
        'description' => t("Enter a channel name here. A channel name from the Slack module settings is '" . slack_get_default_channel() . "'."),
      ),
      'username' => array(
        'type' => 'string',
        'label' => t('Username'),
        'description' => t("Enter a username here. A username from the Slack module settings is '" . slack_get_default_username() . "'."),
      ),
    ),
    'module' => 'Slack',
  );
  return $actions;
}

/**
 * Rules action for sending a message to the Slack.
 */
function slack_rules_send_message_action($message, $channel, $username) {
  slack_send_message($message, $channel, $username);
}
