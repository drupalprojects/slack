<?php

/**
 * @param string $message
 *   The message sent to the channel
 * @param string $channel
 *   The channel in the Slack service to send messages
 * @param string $username
 *   The bot name displayed in the channel
 * @return bool|object
 */
function slack_send_message($message, $channel = '', $username = '') {
  $team_name = slack_get_default_team_name();
  if (!$team_name) {
    return FALSE;
  }
  $team_token = slack_get_default_team_token();
  if (!$team_token) {
    return FALSE;
  }
  $message_options = array();
  if ($channel) {
    $message_options['channel'] = $channel;
  }
  if ($username) {
    $message_options['username'] = $username;
  }
  $result = _slack_send_message($team_name, $team_token, $message, $message_options);
  return $result;
}

/**
 * @param string $team_name
 *   Your team name in the Slack
 * @param string $team_token
 *   The token from "Incoming WebHooks" integration in the Slack
 * @param string $message
 *   The message sent to the channel
 * @param array $message_options
 *   An associative array, it can contain:
 *     - channel: The channel in the Slack service to send messages
 *     - username: The bot name displayed in the channel
 * @return object
 *   Can contain:
 *                          success      fail          fail
 *     - data:                ok         No hooks      Invalid channel specified
 *     - status message:      OK         Not found     Server Error
 *     - code:                200        404           500
 *     - error:               -          Not found     Server Error
 */
function _slack_send_message($team_name, $team_token, $message, $message_options = array()) {
  $headers = array(
    'Content-Type' => 'application/x-www-form-urlencoded'
  );
  $message_options['text'] = $message;
  $sending_data = 'payload=' . drupal_json_encode($message_options);
  $options = array(
    'method' => 'POST',
    'data' => $sending_data,
    'headers' => $headers,
  );
  $sending_url = 'https://' . $team_name . '.slack.com/services/hooks/incoming-webhook?token=' . $team_token;
  $result = drupal_http_request($sending_url, $options);
  return $result;
}

/**
 * @return string
 *   Get default team name
 */
function slack_get_default_team_name() {
  $team_name = variable_get('slack_team', '');
  return $team_name;
}

/**
 * @return string
 *   Get default team token
 */
function slack_get_default_team_token() {
  $team_token = variable_get('slack_token', '');
  return $team_token;
}