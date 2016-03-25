<?php

/**
 * @file
 * Contains \Drupal\rules\Plugin\RulesAction\SlackSendMessage.
 */

namespace Drupal\slack\Plugin\RulesAction;

use Drupal\slack;
use Drupal\rules\Core\RulesActionBase;

/**
 * Provides a 'Slack send message' action.
 *
 * @RulesAction(
 *   id = "rules_slack_send_message",
 *   label = @Translation("Send message to Slack"),
 *   category = @Translation("Slack"),
 *   context = {
 *     "message" = @ContextDefinition("string",
 *       label = @Translation("Message"),
 *       description = @Translation("Specify the message, which should be sent to Slack.")
 *     ),
 *     "channel" = @ContextDefinition("string",
 *       label = @Translation("Channel"),
 *       description = @Translation("Specify the channel.")
 *     )
 *   }
 * )
 */
class SlackSendMessage extends RulesActionBase {

/**
 * Send message to slack.
 *
 * @param string $message
 *    The message to be sended.
 * @param string $channel
 *    The slack channel.
 */
  protected function doExecute($message, $channel) {
    $config = \Drupal::configFactory()->getEditable('slack.settings');

    $username = $config->get('slack_username');
    $webhook_url = $config->get('slack_webhook_url');

    \Drupal::service('slack')->sendMessage($webhook_url, $message, $channel, $username);
  }
}
