/*
 * kitchen_timer.h
 *
 *  Created on: Jun 18, 2021
 *      Author: Joe Krachey
 */

#ifndef KITCHEN_TIMER_H_
#define KITCHEN_TIMER_H_

#include "stdint.h"
#include "stdbool.h"

#include "msp.h"
#include "cap_sense.h"
#include "display.h"
#include "provided_code/hw_timers.h"
#include "provided_code/buzzer.h"
#include "provided_code/hw_timers.h"

void kitchen_timer_mode_count_down(void);
void kitchen_timer_mode_init(void);

#endif /* KITCHEN_TIMER_H_ */
