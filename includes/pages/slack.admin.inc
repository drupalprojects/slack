<?php

/**
 * @file
 * Slack integration module admin functions.
 */

/**
 * Slack module admin form.
 */
function slack_configure_form($form, &$form_state) {
  $form['slack_webhook_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Webhook URL'),
    '#description' => t('Enter your Webhook URL from an Incoming WebHooks integration. It looks like https://hooks.slack.com/services/XXXXXXXXX/YYYYYYYYY/ZZZZZZZZZZZZZZZZZZZZZZZZ'),
    '#default_value' => slack_get_default_webhook_url(),
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
  $form['slack_icon_type'] = array(
    '#type' => 'radios',
    '#title' => t('Type of image'),
    '#options' => array (
      'emoji' => t('Emoji'),
      'image' => t('Image'),
      'none' => t('None (Use default integration settings)')),
    '#default_value' => slack_get_default_icon_type(),
  );
  $form['slack_icon_emoji'] = array(
    '#type' => 'textfield',
    '#title' => t('Emoji code'),
    '#default_value' => variable_get('slack_icon_emoji'),
    '#description' => t('What emoji would you use for your SlackBot?'),
    '#states' => array(
      'visible' => array(
        ':input[name="slack_icon_type"]' => array(
          'value' => 'emoji',
        ),
      ),
    ),
  );
  $form['slack_icon_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Image URL '),
    '#default_value' => variable_get('slack_icon_url'),
    '#description' => t('What icon would you use for your SlackBot?'),
    '#states' => array(
      'visible' => array(
        ':input[name="slack_icon_type"]' => array(
          'value' => 'image',
        ),
      ),
    ),
  );
  $form['slack_attachment_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send message with attachment styling'),
    '#description' => t('Attachments allow further styling of your messages.'),
    '#default_value' => variable_get('slack_attachment_enabled'),
  );

  $attachment_state_toggle = array(
    'visible' => array(
      ':input[name="slack_attachment_enabled"]' => array(
        'checked' => TRUE,
      )
    )
  );
  $form['slack_attachment_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Default message color'),
    '#description' => t('What color do you want to use for your slack messages by default?'),
    '#default_value' => variable_get('slack_attachment_color'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_pretext'] = array(
    '#type' => 'textfield',
    '#title' => t('Default pretext'),
    '#description' => t('Set a pretext of your message to provide meta information..'),
    '#default_value' => variable_get('slack_attachment_pretext'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default message title'),
    '#description' => t('What title do you want to use for your slack messages by default?'),
    '#default_value' => variable_get('slack_attachment_title'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_title_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Default link for title'),
    '#description' => t('Add a link to your message title.'),
    '#default_value' => variable_get('slack_attachment_title_link'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_image_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Default url for image attachment'),
    '#description' => t('Add the url for your image attachment.'),
    '#default_value' => variable_get('slack_attachment_image_url'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_author_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Default author name for your attachment'),
    '#description' => t('Add the author for your attachment.'),
    '#default_value' => variable_get('slack_attachment_author_name'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_author_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Default link for your attachments author'),
    '#description' => t('Add the url for your attachments author.'),
    '#default_value' => variable_get('slack_attachment_author_link'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_author_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Default author icon attachment'),
    '#description' => t('Add the icon for your attachments author.'),
    '#default_value' => variable_get('slack_attachment_author_icon'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_footer'] = array(
    '#type' => 'textfield',
    '#title' => t('Default footer for your attachment'),
    '#description' => t('Add a footer to your attachment.'),
    '#default_value' => variable_get('slack_attachment_footer'),
    '#states' => $attachment_state_toggle,
  );
  $form['slack_attachment_footer_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Default footer icon for your attachment'),
    '#description' => t('Add a footer icon to your attachment.'),
    '#default_value' => variable_get('slack_attachment_footer_icon'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_ts'] = array(
    '#type' => 'textfield',
    '#title' => t('Default time stamp for attachment'),
    '#description' => t('Add a time stamp to your attachment.'),
    '#default_value' => variable_get('slack_attachment_ts'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_mrkdwn'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Send message with markdown'),
    '#description' => t('Allow to send messages, based on markdown syntax like (*) for bold and (_) for italics.'),
    '#default_value' => variable_get('slack_attachment_mrkdwn'),
    '#options' => array (
      'text' => t('Text'),
      'pretext' => t('Pretext'),
      'fields' => t('Fields')
    ),
    '#states' => $attachment_state_toggle,
  );

  return system_settings_form($form);
}
