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
 * Create a log in watchdog containing Slack's response information when message
 * sending was failed.
 */
function slack_watchdog_sending_error($result) {
  $message = '';
  $status_message = $result->status_message;
  if (isset($status_message)) {
    $message .= "<strong>Error message:</strong> $status_message";
  }

  $code = $result->code;
  if (isset($code)) {
    $message .= "<br /><strong>Code:</strong> $code";
  }

  $request = $result->request;
  if (isset($request)) {
    $message .= "<br /><strong>Request:</strong> $request";
  }

  $headers = $result->headers;
  if (isset($headers)) {
    $message .= "<br /><br /><strong>Headers:</strong><br><ul>";
    foreach ($headers as $key => $header) {
      $message .= "<li>$key: $header</li>";
    }
    $message .= '</ul>';
  }

  watchdog('slack', $message, array(), WATCHDOG_ERROR);
}
