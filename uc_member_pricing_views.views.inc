<?php
// $Id$

/**
 * @file
 */

/**
 * Implementation of hook_views_data().
 */
function uc_member_pricing_views_views_data() {
  //drupal_set_message('View hook called.');
  $data['uc_member_prices'] = array(
    'table' => array(
      'group' => t('Ubercart Member Pricing'),
      'title' => 'uc_member_prices_views',
      'join'  => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
	),
        'uc_products' => array(
          'left_field' => 'nid',
          'field' => 'nid',
	),
      ),
    ),
    'price' => array(
      'title' => t('Role Price'),
      'help' => t('A product price per roles'),
      'field' => array(
        'handler' => 'uc_member_pricing_views_views_handler_field_price_role',
	'click sortable' => TRUE,
	'float' => TRUE,
	'additional fields' => array(
          'nid' => array(
            'table' => 'node',
	    'field' => 'nid',
          ),
        ),
      ),
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function uc_member_pricing_views_views_handlers(){
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'uc_member_pricing_views'),
    ),
    'handlers' => array(
      'uc_member_pricing_views_views_handler_field_price_role' => array(
        'parent' => 'views_handler_field_numeric',
      ),
    ),
  );
}
