<?php
// $Id: uc_product_handler_field_price.inc,v 1.1.2.8 2010/07/30 15:07:22 islandusurper Exp $

/**
 * @file
 * Views handler: Product price field by role.
 */

/**
 * Return a formatted price value to display in the View.
 */
class uc_member_pricing_views_views_handler_field_price_role extends views_handler_field_numeric {
  function option_definition() {
    $options = parent::option_definition();

    $options['format'] = array('default' => 'uc_price');
    $options['revision'] = array('default' => 'themed');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->options;

    $form['format'] =  array(
      '#title' => t('Format'),
      '#type' => 'radios',
      '#options' => array(
        'uc_price' => t('Ubercart price'),
//        'numeric' => t('Numeric'),
      ),
      '#default_value' => 'uc_price', //$options['format'],
      '#weight' => 1,
    );

    $form['revision'] = array(
      '#title' => t('Revision'),
      '#type' => 'select',
      '#options' => array(
        'original' => t('Original'),
        'altered' => t('Altered'),
        'formatted-original' => t('Formatted original'),
        'formatted' => t('Formatted'),
        'themed-original' => t('Themed original'),
        'themed' => t('Themed'),
      ),
      '#default_value' => $options['revision'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[format]' => array('uc_price')),
      '#weight' => 1,
    );

    // Change weight and dependency of the previous field on the parent numeric ones
    $weight = 2;
    foreach (array('set_precision', 'precision', 'decimal', 'separator', 'prefix', 'suffix') as $field) {
      $form[$field]['#process'] = array('views_process_dependency');

      if (is_array($form[$field]['#dependency'])) {
        $form[$field]['#dependency'] += array('radio:options[format]' => array('numeric'));
        $form[$field]['#dependency_count'] = count($form[$field]['#dependency']);
      }
      else {
        $form[$field]['#dependency'] = array('radio:options[format]' => array('numeric'));
      }

      $form[$field]['#weight'] = ++$weight;
    }

    // list of roles
    $form['role'] = array(
      '#title' => t('Select role'),
      '#type' => 'select',
      '#options' => user_roles(),
      '#default_value' => $options['role'],
      '#weight' => 1.
    );
  }

  function render($values) {
/*    // load module uc_member_pricing
    module_load_include('inc', 'uc_member_pricing', 'uc_member_pricing.api');
    // load product price by role
    $price_role = uc_member_pricing_select_rules(
      array(
        'nid' => $values->nid,
        'xid' => $this->options['role'],
      )
    );
     
    if ($this->options['format'] == 'numeric') {
//	    print_r($values);die();
      $values->uc_products_sell_price = $price_role['price'];
      return parent::render($values);//. (preg_match('/^(\+\-)/'));
    }
 */
    // format as price
    if ($this->options['format'] == 'uc_price') {
      $product = node_load($values->{$this->aliases['nid']});
      $context = array(
        'revision' => $this->options['revision'],
        'type' => 'product',
        'class' => array(
          'product',
          $this->field,
        ),
        'field' => $this->real_field,
        'subject' => array(
          'node' => $product,
        ),
      );
      $options = array('label' => FALSE);
      
      // load module uc_member_pricing
      module_load_include('inc', 'uc_member_pricing', 'uc_member_pricing.api');
      
      // load product price by role
      $price_role = uc_member_pricing_select_rules(
        array(
	  'nid' => $values->nid,
	  'xid' => $this->options['role'],
        )
      );
      return (
        $price_role ?
	  uc_price(
	    uc_member_pricing_calculate_price($values->uc_products_sell_price, $price_role['price']),
	    $context,
	    $options
          ) :
	''
      );
    }
  }
}
