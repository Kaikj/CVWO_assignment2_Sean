<?php

namespace CVWO\HelloWorld {

    require_once drupal_get_path("module", MODULE_NAME)."/helloworld_d7_api.inc";

    /**
     * Displays the form for mass importing details of the user record
     */
    function helloworld_import_form($form, &$form_state, $page_mode = 'import') {
        $form = array();

        if ($page_mode != 'import') {
            // Something is wrong, redirect back to main page
            drupal_goto(MODULE_NAME);
        }

        if (isset($form_state['values'])) {
            $values = $form_state['values'];
        } else {
            $values = array(
                'info' => '',
                'comment' => '',
            );
        }

        if ($page_mode == 'view') {
            $disabled = TRUE;
        } else {
            $disabled = FALSE;
        }

        $form['instruction'] = array(
            '#markup' => t('<p>Please input data in the format Name, Email.<br>
                           input every new user in a new line.</p>
                           <p>eg. Jane Doe, janedoe@example.com<br>
                                  John Doe, johndoe@example.com</p>'),
        );

        $form['info'] = array(
            '#type' => 'textarea',
            '#title' => t('User Data'),
            '#default_value' => $values['info'],
            '#disabled' => $disabled,
            '#rows' => 5,
            '#required' => TRUE,
        );

        $form['comment'] = array(
            '#type' => 'textarea',
            '#title' => t('Comments'),
            '#disabled' => $disabled,
            '#default_value' => $values['comment'],
            '#rows' => 3,
            '#required' => FALSE,
        );

        $form['import'] = array(
            '#type' => 'submit',
            '#value' => t('Import'),
        );

        return $form;
    }

    /**
     * Implements form_validate
     */
    function helloworld_import_form_validate($form, &$form_state){
        // Split the information into a multidimensional array to work with
        $form_state['values']['info'] = preg_split("/\r\n|\n|\r/", $form_state['values']['info']);
        foreach ($form_state['values']['info'] as $key => $value) {
            $form_state['values']['info'][$key] = array_map('trim', explode(',', $value));
        }

        // valid_email_address is from drupal api
        // validation function checks whether the email is valid before inserting
        foreach ($form_state['values']['info'] as $value) {
            if (!valid_email_address($value[1])){
                form_set_error('email', t('Please enter a valid email address.'));
            }
        }
    }

    /**
     * Implements form_submit
     */
    function helloworld_import_form_submit($form, &$form_state){
        $error = FALSE;

        if (import_user($form_state['values'])) {
            drupal_set_message(t('Mass import successful'));
        } else {
            drupal_set_message(t('An error was encountered while mass importing'));
            $error = TRUE;
        }

        // if we encountered an error, we keep the form as it is
        // else we redirect back to home page
        if ($error) {
            $form_state['rebuild'] = TRUE;
        } else {
            $form_state['redirect'] = MODULE_NAME;
        }
    }
}
