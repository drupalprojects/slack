<?php

/**
 * @file
 * Slack module page functions.
 */

/**
 * Slack test message form.
 */
function slack_send_test_message_form($form, &$form_state) {
  $form['slack_test_channel'] = array(
    '#type' => 'textfield',
    '#title' => t('Channel'),
    '#default_value' => variable_get('slack_channel'),
  );

  $form['slack_attachments'] = array(
    '#type' => 'fieldset',
    '#title' => t('Attachment options'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('slack_attachment_enabled'),
  );
  $form['slack_attachments']['slack_attachment_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable attachment'),
    '#default_value' => variable_get('slack_attachment_enabled'),
  );
  $form['slack_attachments']['slack_test_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Color'),
    '#default_value' => variable_get('slack_attachment_color'),
  );
  $form['slack_attachments']['slack_test_pretext'] = array(
    '#type' => 'textarea',
    '#title' => t('Message pretext'),
    '#default_value' => variable_get('slack_attachment_pretext'),
  );
  $form['slack_attachments']['slack_test_attachment_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Attachment text'),
    '#default_value' => variable_get('slack_attachment_text'),
  );
  $form['slack_attachments']['slack_test_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Message title'),
    '#default_value' => variable_get('slack_attachment_title'),
  );
  $form['slack_attachments']['slack_test_title_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Message title link'),
    '#default_value' => variable_get('slack_attachment_title_link'),
  );
  $form['slack_attachments']['slack_test_image_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Image link'),
    '#default_value' => variable_get('slack_attachment_image_url'),
  );
  $form['slack_attachments']['slack_test_author_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Attachment author name'),
    '#default_value' => variable_get('slack_attachment_author_name'),
  );
  $form['slack_attachments']['slack_test_author_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Attachment author link'),
    '#default_value' => variable_get('slack_attachment_author_link'),
  );
  $form['slack_attachments']['slack_test_author_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Attachment author icon'),
    '#default_value' => variable_get('slack_attachment_author_icon'),
  );
  $form['slack_attachments']['slack_test_footer'] = array(
    '#type' => 'textfield',
    '#title' => t('Attachment footer'),
    '#default_value' => variable_get('slack_attachment_footer'),
  );
  $form['slack_attachments']['slack_test_footer_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Attachment footer icon'),
    '#default_value' => variable_get('slack_attachment_footer_icon'),
  );
  $form['slack_attachments']['slack_test_ts'] = array(
    '#type' => 'textfield',
    '#title' => t('Attachment time stamp'),
    '#default_value' => variable_get('slack_attachment_ts'),
  );
  $form['slack_attachments']['slack_attachment_mrkdwn'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Send message with markdown'),
    '#description' => t('Allow to send messages, based on markdown syntax like (*) for bold and (_) for italics.'),
    '#default_value' => variable_get('slack_attachment_mrkdwn'),
    '#options' => array(
      'text' => t('Text'),
      'pretext' => t('Pretext'),
      'fields' => t('Fields'),
    ),
  );
  $form['slack_test_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#validate' => array('slack_validate_test_message'),
    '#value' => t('Send message'),
  );
  return $form;
}

/**
 * Validate if text or attachment for test message is not empty.
 */
function slack_validate_test_message($form, &$form_state) {
  $message_is_empty = empty($form_state['values']['slack_test_message']);
  $attachment_enabled = $form_state['values']['slack_attachment_enabled'];
  // At least one of these fields must be filled to send message.
  $fields_to_check_in_attachments = array(
    'slack_test_pretext',
    'slack_test_attachment_text',
    'slack_test_footer',
  );
  $attachment_is_empty = TRUE;
  foreach ($fields_to_check_in_attachments as $field_to_check) {
    if (!empty($form_state['values'][$field_to_check])) {
      $attachment_is_empty = FALSE;
      break;
    }
  }
  if ($message_is_empty && (!$attachment_enabled || $attachment_is_empty)) {
    form_set_error('empty_message', t('Empty message and attachment text'));
  }
}

/**
 * Submit handler for slack test message form.
 */
function slack_send_test_message_form_submit($form, &$form_state) {
  $webhook_url = slack_get_default_webhook_url();
  $channel = $form_state['values']['slack_test_channel'];
  $message = $form_state['values']['slack_test_message'];

  $attachment_options = array();
  $use_attachments = $form_state['values']['slack_attachment_enabled'];
  if ($use_attachments) {
    $attachment_options['pretext'] = $form_state['values']['slack_test_pretext'];
    $attachment_options['text'] = $form_state['values']['slack_test_attachment_text'];
    $attachment_options['title'] = $form_state['values']['slack_test_title'];
    $attachment_options['title_link'] = $form_state['values']['slack_test_title_link'];
    $attachment_options['color'] = $form_state['values']['slack_test_color'];
    $attachment_options['image_url'] = $form_state['values']['slack_test_image_url'];
    $attachment_options['author_name'] = $form_state['values']['slack_test_author_name'];
    $attachment_options['author_link'] = $form_state['values']['slack_test_author_link'];
    $attachment_options['author_icon'] = $form_state['values']['slack_test_author_icon'];
    $attachment_options['footer'] = $form_state['values']['slack_test_footer'];
    $attachment_options['footer_icon'] = $form_state['values']['slack_test_footer_icon'];
    $attachment_options['ts'] = $form_state['values']['slack_test_ts'];
    foreach ($form_state['values']['slack_attachment_mrkdwn'] as $option => $value) {
      if ($value) {
        $attachment_options['mrkdwn_in'][] = $option;
      }
    }
  }
  $icon_type = variable_get('slack_icon_type');
  $icon = variable_get('slack_icon_url');
  $emoji = variable_get('slack_icon_emoji');

  $icon_options = array(
    'icon' => $icon,
    'emoji' => $emoji,
    'type' => $icon_type,
  );

  $result = slack_send_message($webhook_url, $message, $channel, '', $icon_options, $attachment_options);
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
 * Function to sending error for watchdog.
 *
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
