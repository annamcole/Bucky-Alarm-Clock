#include "msp.h"
#include "kitchen_timer.h"
#include "cap_sense.h"
#include "display.h"
#include "provided_code/i2c.h"
#include "provided_code/buzzer.h"
#include "provided_code/hw_timers.h"

/**
 * main.c
 */
void main(void)
{
    uint8_t buttons;
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	display_init();

	buzzer_init(8000);
	buzzer_off();

	i2c_init();
	cap_sense_interrupt_init();
	buttons = cap_sense_get_buttons();

	hw_timer_init_1S_irq();
	hw_timer_init_2ms_irq();

	display_eye_left(false);
	display_eye_right(false);

    __enable_irq();

    while(1){
        kitchen_timer_mode_init();
        kitchen_timer_mode_count_down();
    };

}
