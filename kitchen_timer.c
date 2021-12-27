/*
 * kitchen_timer.c
 *
 *  Created on: Jun 18, 2021
 *      Author: Anna Stephan
 */

#include "kitchen_timer.h"

uint8_t CLEAR = 0x00;
uint8_t M = 0x01;
uint8_t H = 0x02;
uint8_t M_H = 0x03;
uint8_t UP = 0x08;
uint8_t DOWN = 0x04;

uint8_t D0 = 0x00;
uint8_t D1 = 0x00;
uint8_t D2 = 0x00;
uint8_t D3 = 0x00;
uint8_t button_status = 0x00;
uint8_t location = 0x00;
bool blink_status = true;

/******************************************************************************
 * Make sure you examine the functions that are made available to you in
 * the provided_code directory.  There are already functions provided that
 * configure timers for 1 second and 2mS.  The ISRs for these timers are
 * also provided.
 *
 * You should also make use of the functions you completed in display.c and
 * cap_sense.c
 *
 * You can add as many helper functions are you like to this file.
 *
 * DO NOT try to code all of this at once.  Break this into smaller steps.
 *
 *      -> Write  a small amount of code to accomplish a simple requirement.
 *      -> Verify your new code works.
 *      -> Verify that you did not break any of your earlier code.
 *      -> Continue to the next requirement.
 *
 * Use the debugger to verify your code!
 *
 * Suggested Development Strategy
 *
 * 1. Detect when the user presses one of the Cap Sense Buttons
 * 2. Determine which button has been pressed
 * 3. Print out a number to a single seven segment digit
 * 4. Print out a 4-digit number to the display (see video on POV)
 * 5. Blink the 4-digit number on/off at a rate of 1 second
 * 6. Modify the 4-digit number displayed using the Cap Sense Buttons
 * 6. Count down to 00:00
 * 7. Toggle the eyes/buzzer
 * 8. Complete remaining requirements.
 *****************************************************************************/

/*****************************************************
 * Allows the user to set the duration of the timer
 *****************************************************/
void kitchen_timer_mode_init(void)
{
    button_status = 0x00;
    bool M_en = false;
    bool H_en = false;

    while(1){
        // display digits
        if(ALERT_1_SECOND){
            ALERT_1_SECOND = false;
            blink_status = !blink_status;
        }

        // turn on digits
        while(!ALERT_1_SECOND){
            if(ALERT_2_MILLISECOND && blink_status){
                ALERT_2_MILLISECOND = false;
                switch(location){
                case 0x00:
                    display_digit(location,D0);
                    break;
                case 0x01:
                    display_digit(location,D1);
                    break;
                case 0x02:
                    display_digit(location,D2);
                    break;
                case 0x03:
                    display_digit(location,D3);
                    break;
                }
                location = (location + 1) % 4;
            } else if(ALERT_2_MILLISECOND && ! blink_status){
                ALERT_2_MILLISECOND = false;
                // turn off digits
                display_all_dig_off();
            }

            // read button status if pressed
            if(ALERT_BUTTON_PRESSED){
                ALERT_BUTTON_PRESSED = false;
                button_status = cap_sense_get_buttons();
            }

            switch(button_status){
            case 0x03:  // M_H
                return;
            case 0x01:  // M
                button_status = 0x00;
                H_en = false;
                M_en = true;
                break;
            case 0x02:  // H
                button_status = 0x00;
                M_en = false;
                H_en = true;
                break;
            case 0x08:  // UP
                // increment min/hour
                button_status = 0x00;
                if(M_en){
                    if(D1 == 5 && D0 == 9){     // check edge case of end-range
                        D1 = 0x00;
                        D0 = 0x00;
                    } else if(D0 == 9){     // check edge case of mid-range
                        D1++;
                        D0 = 0x00;
                    } else {
                        D0++;
                    }
                } else if(H_en){
                    if(D3 == 9 && D2 == 9){     // check edge case of end-range
                        D3 = 0x00;
                        D2 = 0x00;
                    } else if(D2 == 9){     // check edge case of mid-range
                        D3++;
                        D2 = 0x00;
                    } else {
                        D2++;
                    }
                }
                break;
            case 0x04:  // DOWN
                // decrement min/hour
                button_status = 0x00;
                if(M_en){
                    if(D1 == 0 && D0 == 0){     // check edge case of end-range
                        D1 = 0x05;
                        D0 = 0x09;
                    } else if(D0 == 0){     // check edge case of mid-range
                        D1--;
                        D0 = 0x09;
                    } else {
                        D0--;
                    }
                } else if(H_en){
                    if(D3 == 0 && D2 == 0){     // check edge case of end-range
                        D3 = 0x09;
                        D2 = 0x09;
                    } else if(D2 == 0){     // check edge case of mid-range
                        D3--;
                        D2 = 0x09;
                    } else {
                        D2--;
                    }
                }
                break;
            }
        }
    }
}

/*****************************************************
 * Starts the timer
 *****************************************************/
void kitchen_timer_mode_count_down(void)
{
    // clear/update variables
    uint8_t minute_cnt = 1;
    button_status = 0x00;
    uint8_t D0_cnt = D0;
    uint8_t D1_cnt = D1;
    uint8_t D2_cnt = D2;
    uint8_t D3_cnt = D3;
    uint8_t sec = minute_cnt;
    bool ALERT_CHANGE_MINUTE = false;

    while(1){
        // display digits
        if(ALERT_2_MILLISECOND){
            ALERT_2_MILLISECOND = false;
            switch(location){
            case 0x00:
                display_digit(location,D0_cnt);
                break;
            case 0x01:
                display_digit(location,D1_cnt);
                break;
            case 0x02:
                display_digit(location,D2_cnt);
                break;
            case 0x03:
                display_digit(location,D3_cnt);
                break;
            }
            location = (location + 1) % 4;
        }

        // check for exit input
        if(ALERT_BUTTON_PRESSED){
            ALERT_BUTTON_PRESSED = false;
            button_status = cap_sense_get_buttons();
            if(button_status == M_H){
                return;
            }
        }

        // decrement seconds count
        if(ALERT_1_SECOND){
            ALERT_1_SECOND = false;
            sec--;
            if(sec == 0){
                ALERT_CHANGE_MINUTE = true;
                sec = minute_cnt;
            }
        }

        // update time
        if(ALERT_CHANGE_MINUTE){
            ALERT_CHANGE_MINUTE = false;

            if (D0_cnt == 0){
                if(D1_cnt != 0){
                    D1_cnt--;
                    D0_cnt = 9;
                } else {
                    if(D2_cnt != 0){
                        D2_cnt--;
                        D1_cnt = 5;
                        D0_cnt = 9;
                    } else {
                        if(D3_cnt != 0){
                            D3_cnt--;
                            D2_cnt = 9;
                            D1_cnt = 5;
                            D0_cnt = 9;
                        }
                    }
                }
            } else {
                D0_cnt--;
            }
        }

        // check if count finished
        if(D3_cnt == 0 && D2_cnt == 0 && D1_cnt == 0 && D0_cnt == 0){
            while(1){
                // sound alarm!!!
                if(ALERT_1_SECOND){
                    ALERT_1_SECOND = false;
                    blink_status = !blink_status;

                    if(blink_status){
                        // cycle alarm on
                        buzzer_on();
                        display_eye_left(true);
                        display_eye_right(true);
                    } else {
                        // cycle alarm off
                        buzzer_off();
                        display_eye_left(false);
                        display_eye_right(false);
                    }
                }

                // ALARM!!!!!!!!
                while(!ALERT_1_SECOND){
                    if(ALERT_2_MILLISECOND){
                        ALERT_2_MILLISECOND = false;
                        switch(location){
                        // display digits
                        case 0x00:
                            display_digit(location,D0_cnt);
                            break;
                        case 0x01:
                            display_digit(location,D1_cnt);
                            break;
                        case 0x02:
                            display_digit(location,D2_cnt);
                            break;
                        case 0x03:
                            display_digit(location,D3_cnt);
                            break;
                        }
                        location = (location + 1) % 4;
                    }



                    // check for exit input
                    if(ALERT_BUTTON_PRESSED){
                        ALERT_BUTTON_PRESSED = false;
                        button_status = cap_sense_get_buttons();
                        if(button_status == M_H){
                            buzzer_off();
                            display_eye_left(false);
                            display_eye_right(false);
                            return;
                        }
                    }
                }
            }
        }
    }

}


