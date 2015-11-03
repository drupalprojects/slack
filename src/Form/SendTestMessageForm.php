<?php

/**
 * @file
 * Contains Drupal\slack\Form\SendTestMessageForm.
 */

namespace Drupal\slack\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
//use Drupal\Core\Url; /** del?* */
use Drupal\slack\SlackAPI;
use Drupal\slack\Controller\SlackController;
use Symfony\Component\HttpFoundation\RedirectResponse;

/**
 * Class SendTestMessageForm.
 *
 * @package Drupal\slack\Form
 */
class SendTestMessageForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'slack_send_test_message';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('slack.settings');
    $form['slack_test_channel'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Channel or username'),
      '#default_value' => $config->get('slack_channel'),
    );
    $form['slack_test_message'] = array(
      '#type' => 'textarea',
      '#title' => $this->t('Message'),
      '#required' => TRUE,
    );
    $form['actions']['#type'] = 'actions';
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => $this->t('Send message'),
      '#button_type' => 'primary',
    );
    if (empty($config->get('slack_webhook_url'))) {
      $url = new RedirectResponse('../slack/config');
      $url->send();
      return false;
    }
    else {
      return $form;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = \Drupal::config('slack.settings');
    
    $channel = $form_state->getValue('slack_test_channel');
    $message = $form_state->getValue('slack_test_message');
    
    $username = $config->get('slack_username');
    $webhook_url = $config->get('slack_webhook_url');
    
    $SlackAPI = SlackController::GetSlackApiItem();
    $SlackAPI->_send($webhook_url, $message, $channel, $username);
  }

}
