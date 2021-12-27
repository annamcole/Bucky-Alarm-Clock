/*
 * display.h
 *
 *  Created on: Jun 22, 2021
 *      Author: Joe Krachey
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "kitchen_timer.h"
#include "cap_sense.h"
#include "provided_code/i2c.h"
#include "provided_code/buzzer.h"
#include "provided_code/hw_timers.h"

// Define the pins that each signal is connected to

#define SEG_A_PORT   P2
#define SEG_A_PIN    BIT7

#define SEG_B_PORT   P2
#define SEG_B_PIN    BIT6

#define SEG_C_PORT   P2
#define SEG_C_PIN    BIT5

#define SEG_D_PORT   P2
#define SEG_D_PIN    BIT4

#define SEG_E_PORT   P4
#define SEG_E_PIN    BIT3

#define SEG_F_PORT   P4
#define SEG_F_PIN    BIT2

#define SEG_G_PORT   P4
#define SEG_G_PIN    BIT1

#define SEG_DP_PORT  P4
#define SEG_DP_PIN   BIT0

#define DIG_3_PORT  P5
#define DIG_3_PIN   BIT7

#define DIG_2_PORT  P5
#define DIG_2_PIN   BIT2

#define DIG_1_PORT  P5
#define DIG_1_PIN   BIT1

#define DIG_0_PORT  P5
#define DIG_0_PIN   BIT0

#define LED_LEFT_PORT   P1
#define LED_LEFT_PIN   BIT6

#define LED_RIGHT_PORT   P1
#define LED_RIGHT_PIN   BIT7

//*****************************************************************************
// API Function Declarations
//*****************************************************************************
void display_init(void);
void display_all_dig_off(void);
void display_eye_left(bool on);
void display_eye_right(bool on);
void display_digit(uint8_t location, uint8_t number);

#endif /* DISPLAY_H_ */
