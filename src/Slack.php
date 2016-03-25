<?php

/**
 * @file
 * Contains \Drupal\slack\Slack.
 */

namespace Drupal\slack;
use Drupal\Core\Config\ConfigFactoryInterface;
use GuzzleHttp\ClientInterface;


/**
 * Send messages to Slack.
 */
class Slack {
  /**
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  private $config;

  /**
   * @var \GuzzleHttp\ClientInterface
   */
  private $httpClient;

  /**
   * Constructs a Slack object.
   *
   * @param ConfigFactoryInterface $config
   * @param \GuzzleHttp\ClientInterface $http_client
   */
  public function __construct(ConfigFactoryInterface $config, ClientInterface $http_client) {
    $this->config = $config;
    $this->httpClient = $http_client;
  }

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
  public function sendMessage($webhook_url, $message, $channel = '', $username = '') {
    if (empty($webhook_url)) {
      drupal_set_message(t('You need to enter a webhook!'), 'error');
      return false;
    }
    $config = $this->prepareMessage($webhook_url, $channel, $username);
    $result = $this->sendRequest(
      $config['webhook_url'], $message, $config['message_options']
    );
    return $result;
  }

  /**
   * Prepare message meta fields for Slack.
   *
   * @param $webhook_url
   * @param $channel
   * @param $username
   * @return array
   */
  private function prepareMessage($webhook_url, $channel, $username) {
    $config = $this->config->get('slack.settings');
    $message_options = array();
    if (!empty($channel)) {
      $message_options['channel'] = $channel;
    }
    elseif (!empty($config->get('slack_channel'))) {
      $message_options['channel'] = $config->get('slack_channel');
    }
    if (!empty($username)) {
      $message_options['username'] = $username;
    }
    elseif (!empty($config->get('slack_username'))) {
      $message_options['username'] = $config->get('slack_username');
    }
    $icon_type = $config->get('slack_icon_type');
    if ($icon_type == 'emoji') {
      $message_options['icon_emoji'] = $config->get('slack_icon_emoji');
    }
    elseif ($icon_type == 'image') {
      $message_options['icon_url'] = $config->get('slack_icon_url');
    }
    $message_options['as_user'] = true;
    return [
      'webhook_url' => $webhook_url,
      'message_options' => $message_options
    ];
  }

  /**
   * Send message to the Slack with more options.
   *
   * @param string $team_name
   *   Your team name in the Slack.
   * @param string $team_token
   *   The token from "Incoming WebHooks" integration in the Slack.
   * @param string $message
   *   The message sent to the channel.
   * @param array $message_options
   *   An associative array, it can contain:
   *     - channel: The channel in the Slack service to send messages
   *     - username: The bot name displayed in the channel
   *     - icon_emoji: The bot icon displayed in the channel
   *     - icon_url: The bot icon displayed in the channel
   *
   * @return object
   *   Can contain:
   *                          success      fail          fail
   *     - data:                ok         No hooks      Invalid channel specified
   *     - status message:      OK         Not found     Server Error
   *     - code:                200        404           500
   *     - error:               -          Not found     Server Error
   */
  private function sendRequest($webhook_url, $message, $message_options = array()) {
    $headers = array(
      'Content-Type' => 'application/x-www-form-urlencoded',
    );
    $message_options['text'] = $this->processMessage($message);
    $sending_data = 'payload=' . json_encode($message_options);

    try {
      $response = $this->httpClient->request('POST', $webhook_url, array('headers' => $headers, 'body' => $sending_data, 'timeout' => 2));
      \Drupal::logger('slack')->info('Message was successfully sent!');
      return $response;
    } catch (\GuzzleHttp\Exception\ServerException $e) {
      \Drupal::logger('slack')->error('Server error! It may appear if you try to use unexisting chatroom.');
      return FALSE;
    } catch (\GuzzleHttp\Exception\RequestException $e) {
      \Drupal::logger('slack')->error('Request error! It may appear if you entered the invalid Webhook value.');
      return FALSE;
    } catch (\GuzzleHttp\Exception\ConnectException $e) {
      \Drupal::logger('slack')->error('Connection error! Something wrong with your connection. Message was\'nt sent.');
      return FALSE;
    }
  }

  /**
   * Replaces links with slack friendly tags. Strips all other html.
   *
   * @param string $message
   *   The message sent to the channel.
   *
   * @return string
   *   Replaces links with slack friendly tags. Strips all other html.
   */
  private function processMessage($message) {
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
}