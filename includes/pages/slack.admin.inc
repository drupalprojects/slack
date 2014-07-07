<?php

/**
 * @file
 * Slack integration module admin functions.
 */

/**
 * Slack module admin form.
 */
function slack_configure_form($form, &$form_state) {
  $form['slack_team'] = array(
    '#type' => 'textfield',
    '#title' => t('Team name'),
    '#default_value' => slack_get_default_team_name(),
    '#required' => TRUE,
  );
  $form['slack_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Team token'),
    '#description' => t('Add the "Incoming WebHooks" integration in the Slack and take the token there.'),
    '#default_value' => slack_get_default_team_token(),
    '#required' => TRUE,
  );
  $form['slack_channel'] = array(
    '#type' => 'textfield',
    '#title' => t('Default channel'),
    '#description' => t('Enter your channel name with # symbol, for example #general (or @username for a private message or a private group name).'),
    '#default_value' => variable_get('slack_channel'),
  );
  $form['slack_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Default username'),
    '#description' => t('What would you like to name your Slack bot?'),
    '#default_value' => variable_get('slack_username'),
  );
  return system_settings_form($form);
}
