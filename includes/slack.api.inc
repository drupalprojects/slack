<?php

/**
 * @file
 * Slack integration module API functions.
 */

/**
 * Send message to the Slack.
 *
 * @param string $message
 *   The message sent to the channel.
 * @param string $channel
 *   The channel in the Slack service to send messages.
 * @param string $username
 *   The bot name displayed in the channel.
 *
 * @return bool|object
 *   Slack response.
 */
function slack_send_message($webhook_url, $message, $channel = '', $username = '', $icon_options = array(), $attachment_options = array()) {
  if (!$webhook_url) {
    drupal_set_message('Please, enter the webhook value and try again', 'error');
    return FALSE;
  }
  if ($channel) {
    $message_options['channel'] = $channel;
  }
  else {
    $message_options['channel'] = slack_get_default_channel();
  }
  if ($username) {
    $message_options['username'] = $username;
  }
  else {
    $message_options['username'] = slack_get_default_username();
  }

  if (isset($icon_options['type'])) {
    if ($icon_options['type'] == 'emoji') {
      if ($icon_options['emoji']) {
        $message_options['icon_emoji'] = $icon_options['emoji'];
      }
      else {
        $message_options['icon_emoji'] = slack_get_default_icon_emoji();
      }
    }
    elseif ($icon_options['type'] == 'image') {
      if ($icon_options['icon']) {
        $message_options['icon_url'] = $icon_options['icon'];
      }
      else {
        $message_options['icon_url'] = slack_get_default_icon_url();
      }
    }
  }
  $result = _slack_send_message($webhook_url, $message, $message_options, $attachment_options);
  return $result;
}

/**
 * Send message to the Slack with more options.
 *
 * @return object
 *   Can contain:
 *                          success      fail          fail
 *     - data:                ok         No hooks      Invalid channel specified
 *     - status message:      OK         Not found     Server Error
 *     - code:                200        404           500
 *     - error:               -          Not found     Server Error
 */
function _slack_send_message($webhook_url, $message, $message_options = array(), $attachment_options = array()) {
  $headers = array(
    'Content-Type' => 'application/json',
  );

  $message = _slack_process_message($message);
  $message_options['text'] = $message;
  $attachment_options['fallback'] = $message;
  if (!empty($attachment_options['text'])) {
    $attachment_options['text'] = _slack_process_message($attachment_options['text']);
  }

  $message_options['attachments'] = array(
    $attachment_options,
  );
  $sending_data = '';
  if (!defined('JSON_UNESCAPED_SLASHES')) {
    $sending_data = str_replace('\/', '/', json_encode($message_options));
  }
  else {
    $sending_data = json_encode($message_options, JSON_UNESCAPED_SLASHES);
  }
  $options = array(
    'method' => 'POST',
    'data' => $sending_data,
    'headers' => $headers,
  );
  $sending_url = $webhook_url;
  $result = drupal_http_request($sending_url, $options);

  return $result;
}

/**
 * Replaces links with slack friendly tags. Strips all other html.
 *
 * @param string $message
 *   The message sent to the channel.
 *
 * @return string
 *   Message with replaced links.
 */
function _slack_process_message($message) {
  $regexp = "<a\s[^>]*href=(\"??)([^\" >]*?)\\1[^>]*>(.*)<\/a>";
  if (preg_match_all("/$regexp/siU", $message, $matches, PREG_SET_ORDER)) {
    $i = 1;
    foreach ($matches as $match) {
      $new_link = "<$match[2] | $match[3]>";
      $links['link-' . $i] = $new_link;
      $message = str_replace($match[0], 'link-' . $i, $message);
      $i++;
      $message = strip_tags($message);
      foreach ($links as $id => $link) {
        $message = str_replace($id, $link, $message);
      }
    }
  }
  return $message;
}

// Function above works only for first link.
// TODO: test function below
/* function _slack_process_message($message) { */
/*   $regexp = "<a\s[^>]*href=(\"??)([^\" >]*?)\\1[^>]*>(.*)<\/a>"; */
/*   if (preg_match_all("/$regexp/siU", $message, $matches, PREG_SET_ORDER)) { */
/*     $i = 1; */
/*     $links = array(); */
/*     foreach ($matches as $match) { */
/*       $new_link = "<$match[2] | $match[3]>"; */
/*       $links['link-' . $i] = $new_link; */
/*       $message = str_replace($match[0], 'link-' . $i, $message); */
/*       $i++; */
/*     } */
/*     $message = strip_tags($message); */
/*     foreach ($links as $id => $link) { */
/*       $message = str_replace($id, $link, $message); */
/*     } */
/*   } */
/*   return $message; */
/* } */

/**
 * Get default Webhook URL.
 *
 * @return string
 *   Get default Webhook URL.
 */
function slack_get_default_webhook_url() {
  $channel = variable_get('slack_webhook_url');
  return $channel;
}

/**
 * Get default team channel.
 *
 * @return string
 *   Get default team channel.
 */
function slack_get_default_channel() {
  $channel = variable_get('slack_channel');
  return $channel;
}

/**
 * Get default Slack bot username.
 *
 * @return string
 *   Get default Slack bot username.
 */
function slack_get_default_username() {
  $username = variable_get('slack_username');
  return $username;
}

/**
 * Get default Slack bot icon.
 *
 * @return string
 *   Get default Slack bot icon.
 */
function slack_get_default_icon_url() {
  $icon['url'] = variable_get('slack_icon_url');
  return $icon['url'];
}

/**
 * Get default Slack bot icon emoji.
 *
 * @return string
 *   Get default Slack bot icon emoji.
 */
function slack_get_default_icon_emoji() {
  $icon['emoji'] = variable_get('slack_icon_emoji');
  return $icon['emoji'];
}

/**
 * Get default Slack bot image type radios value.
 *
 * @return string
 *   Get default Slack bot image type radios value.
 */
function slack_get_default_icon_type() {
  $select_image = variable_get('slack_icon_type', 'none');
  return $select_image;
}

/**
 * Get default Slack attachment color value.
 *
 * @return string
 *   Get default Slack attachment color value.
 */
function slack_get_default_attachment_color() {
  $attachment_color = variable_get('slack_attachment_color');
  return $attachment_color;
}

/**
 * Get default Slack attachment pretext value.
 *
 * @return string
 *   Get default Slack attachment pretext value.
 */
function slack_get_default_attachment_pretext() {
  $attachment_pretext = variable_get('slack_attachment_pretext');
  return $attachment_pretext;
}

/**
 * Get default Slack attachment title value.
 *
 * @return string
 *   Get default Slack attachment title value.
 */
function slack_get_default_attachment_title() {
  $attachment_title = variable_get('slack_attachment_title');
  return $attachment_title;
}

/**
 * Get default Slack attachment link value.
 *
 * @return string
 *   Get default Slack attachment link value.
 */
function slack_get_default_attachment_link() {
  $attachment_link = variable_get('slack_attachment_title_link');
  return $attachment_link;
}

/**
 * Get default Slack attachment image url value.
 *
 * @return string
 *   Get default Slack attachment image url value.
 */
function slack_get_default_attachment_image_url() {
  $attachment_image = variable_get('slack_attachment_image_url');
  return $attachment_image;
}

/**
 * Get default Slack attachment author name value.
 *
 * @return string
 *   Get default Slack attachment author name value.
 */
function slack_get_default_attachment_author_name() {
  $attachment_author = variable_get('slack_attachment_author_name');
  return $attachment_author;
}

/**
 * Get default Slack attachment author link value.
 *
 * @return string
 *   Get default Slack attachment author link value.
 */
function slack_get_default_attachment_author_link() {
  $attachment_author_link = variable_get('slack_attachment_author_link');
  return $attachment_author_link;
}

/**
 * Get default Slack attachment author icon value.
 *
 * @return string
 *   Get default Slack attachment author icon value.
 */
function slack_get_default_attachment_author_icon() {
  $attachment_author_icon = variable_get('slack_attachment_author_icon');
  return $attachment_author_icon;
}

/**
 * Get default Slack attachment footer value.
 *
 * @return string
 *   Get default Slack attachment footer value.
 */
function slack_get_default_attachment_footer() {
  $attachment_footer = variable_get('slack_attachment_footer');
  return $attachment_footer;
}

/**
 * Get default Slack attachment footer icon value.
 *
 * @return string
 *   Get default Slack attachment footer icon value.
 */
function slack_get_default_attachment_footer_icon() {
  $attachment_footer_icon = variable_get('slack_attachment_footer_icon');
  return $attachment_footer_icon;
}

/**
 * Get default Slack attachment timestamp value.
 *
 * @return string
 *   Get default Slack attachment timestamp value.
 */
function slack_get_default_attachment_ts() {
  $attachment_ts = variable_get('slack_attachment_ts');
  return $attachment_ts;
}

/**
 * Get default Slack attachment mrkdwn value.
 *
 * @return array
 *   Get default Slack attachment mrkdwn value.
 */
function slack_get_default_attachment_mrkdwn() {
  $mrkdwn = variable_get('slack_attachment_mrkdwn');
  $attachment_mrkdwn = array();
  foreach ($mrkdwn as $option => $value) {
    if ($value != '0') {
      $attachment_mrkdwn[] = $option;
    }
  }
  return $attachment_mrkdwn;
}
