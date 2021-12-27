/*
 * opt3001.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Joe Krachey
 */

#ifndef CAP_SENSE_H_
#define CAP_SENSE_H_

#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "kitchen_timer.h"
#include "display.h"
#include "provided_code/i2c.h"
#include "provided_code/buzzer.h"
#include "provided_code/hw_timers.h"

#define AT42QT2120_SLAVE_ADDR                     0x1C

#define CAP_SEN_IRQ_PORT P5
#define CAP_SEN_IRQ_PIN  BIT5
#define CAP_SEN_IRQ_NUMBER PORT5_IRQn

extern volatile bool ALERT_BUTTON_PRESSED;

/******************************************************************************
 *
 *****************************************************************************/
void cap_sense_interrupt_init(void);

/******************************************************************************
 *
 *****************************************************************************/
uint8_t cap_sense_get_buttons(void);

#endif /* CAP_SENSE_H_ */
