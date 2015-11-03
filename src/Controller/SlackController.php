<?php

/*
 * @file
 * Contains \Drupal\Slack\Controller\SlackController.
 */

namespace Drupal\slack\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\slack\SlackAPI;

class SlackController extends ControllerBase {

  public static function GetSlackApiItem() {
    $SlackApi = new SlackAPI();
    return $SlackApi;
  }

}
