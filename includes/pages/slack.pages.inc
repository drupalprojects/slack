<?php

/**
 * @file
 * Slack module page functions.
 */

/**
 * Slack test message form.
 */
function slack_send_test_message_form(&$form_state) {
  $form['slack_test_channel'] = array(
    '#type' => 'textfield',
    '#title' => t('Channel'),
    '#default_value' => variable_get('slack_channel', SLACK_DEFAULT_CHANNEL),
  );
  $form['slack_test_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('slack_username', SLACK_DEFAULT_USERNAME),
  );
  $form['slack_test_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send message'),
  );
  return $form;
}

/**
 * Submit handler for slack test message form.
 */
function slack_send_test_message_form_submit($form, &$form_state) {
  $channel = $form_state['values']['slack_test_channel'];
  $username = $form_state['values']['slack_test_username'];
  $message = $form_state['values']['slack_test_message'];
  $result = slack_send_message($message, $channel, $username);
  if (!$result) {
    drupal_set_message(t("Message wasn't sent. Please, check slack module configuration."));
  }
  elseif (!isset($result->error) && $result->code == SLACK_CODE_OK) {
    drupal_set_message(t('Message was successfully sent.'));
  }
  else {
    drupal_set_message(t("Message wasn't sent."), 'error');
    slack_watchdog_sending_error($result);
  }
}

/**
 * Create a log in watchdog containing Slack's response information.
 */
function slack_watchdog_sending_error($result) {
  $headers['Content-Type'] = $result->headers['Content-Type'];
  $headers['Content-Length'] = $result->headers['Content-Length'];
  $headers['Connection'] = $result->headers['Connection'];
  $headers['Access-Control-Allow-Origin'] = $result->headers['Access-Control-Allow-Origin'];
  $headers['Content-Security-Policy'] = $result->headers['Content-Security-Policy'];
  $headers['Date'] = $result->headers['Date'];
  $headers['Server'] = $result->headers['Server'];
  $headers['Strict-Transport-Security'] = $result->headers['Strict-Transport-Security'];
  $headers['X-Frame-Options'] = $result->headers['X-Frame-Options'];
  $headers['X-Cache'] = $result->headers['X-Cache'];
  $headers['Via'] = $result->headers['Via'];
  $headers['X-Amz-Cf-Id'] = $result->headers['X-Amz-Cf-Id'];

  $message = "<strong>Error message:</strong> $result->status_message<br>
    <strong>Code:</strong> $result->code<br>
    <strong>Request:</strong> $result->request<br><br>
    <strong>Headers:</strong><br><ul>";
  foreach ($headers as $key => $header) {
    $message .= "<li>$key: $header</li>";
  }
  $message .= '</ul>';

  watchdog('slack', $message, array(), WATCHDOG_ERROR);
}
