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
    'group' => t('Slack'),
    'parameter' => array(
      'webhook_url' => array(
        'type' => 'text',
        'label' => t('Webhook URL'),
        'description' => t("It will be using a webhook_url from slack module settings, if you don't enter it here."),
        'optional' => TRUE,
      ),
      'channel' => array(
        'type' => 'text',
        'label' => t('Channel'),
        'description' => t("It will be using a channel from slack module settings, if you don't enter channel here."),
        'optional' => TRUE,
      ),
      'username' => array(
        'type' => 'text',
        'label' => t('Username'),
        'description' => t("It will be using a username from slack module settings, if you don't enter username here."),
        'optional' => TRUE,
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Sending message'),
      ),
      'type_token' => array(
        'type' => 'text',
        'label' => t('Icon type'),
        'description' => t("Select needed token for define slackbot icon type, or select nothing to use default"),
        'optional' => TRUE,
      ),
      'emoji' => array(
        'type' => 'text',
        'label' => t('Emoji code'),
        'description' => t("It will be using a emoji code from slack module settings, if you don't enter username here."),
        'optional' => TRUE,
      ),
      'icon' => array(
        'type' => 'text',
        'label' => t('Icon url'),
        'description' => t("It will be using a icon url from slack module settings, if you don't enter username here."),
        'optional' => TRUE,
      ),
      'color' => array(
        'type' => 'text',
        'label' => t('Message color'),
        'optional' => TRUE,
      ),
      'pretext' => array(
        'type' => 'text',
        'label' => t('Message pretext'),
        'optional' => TRUE,
      ),
      'title' => array(
        'type' => 'text',
        'label' => t('Message title'),
        'optional' => TRUE,
      ),
      'title_link ' => array(
        'type' => 'text',
        'label' => t('Link for message title'),
        'optional' => TRUE,
      ),
      'image_url' => array(
        'type' => 'text',
        'label' => t('Image url for attachment'),
        'optional' => TRUE,
      ),
    ),
  );
  return $actions;
}

/**
 * Rules action for sending a message to the Slack.
 */
function slack_rules_send_message_action($webhook_url, $channel, $username, $message, $type_token = '', $emoji = '', $icon = '', $color = '', $pretext = '', $title = '', $title_link = '', $image_url = '') {
  if (!$webhook_url) {
    $webhook_url = slack_get_default_webhook_url();
  }
  if (!$channel) {
    $channel = slack_get_default_channel();
  }
  if (!$username) {
    $username = slack_get_default_username();
  }
  if (!$emoji) {
    $emoji = slack_get_default_icon_emoji();
  }
  if (!$icon) {
    $icon = slack_get_default_icon_url();
  }

  $attachment_options = array();
  $attachment = variable_get('slack_attachment_enabled');
  if ($attachment) {
    if (!$color) {
      $color = slack_get_default_attachment_color();
    }
    if (!$pretext) {
      $pretext = slack_get_default_attachment_pretext();
    }
    if (!$title) {
      $title = slack_get_default_attachment_title();
    }
    if (!$title_link) {
      $title_link = slack_get_default_attachment_link();
    }
    if (!$image_url) {
      $image_url = slack_get_default_attachment_image_url();
    }
    $attachment_options['color'] = $color;
    $attachment_options['pretext'] = $pretext;
    $attachment_options['title'] = $title;
    $attachment_options['title_link'] = $title_link;
    $attachment_options['image_url'] = $image_url;
    $attachment_options['mrkdwn_in'] = slack_get_default_attachment_mrkdwn();
  }

  $icon_options = array('icon' => $icon, 'emoji' => $emoji, 'type' => $type_token);
  slack_send_message($webhook_url, $message, $channel, $username, $icon_options, $attachment_options);
}
