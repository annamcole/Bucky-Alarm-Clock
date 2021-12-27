/*
 * opt3001.c
 *
 *  Created on: Jun 18, 2021
 *      Author: Anna Stephan
 */

#include <cap_sense.h>

volatile bool ALERT_BUTTON_PRESSED = false;

/******************************************************************************
 * CHANGE Signal interrupt handler
 *****************************************************************************/
void PORT5_IRQHandler()
{
    uint32_t reg_val;
    ALERT_BUTTON_PRESSED = true;

    // Clear the interrupt
    reg_val = CAP_SEN_IRQ_PORT->IV;
}


/******************************************************************************
 * Functions for reading/writing registers
 *****************************************************************************/

/** Write a register on the AT42QT2120
 *
 * @param reg The reg address to read
 * @param value The value to be written
 *
 */
void AT42QT2120_write_reg(uint8_t reg, uint8_t value)
{
    i2c_write_8(AT42QT2120_SLAVE_ADDR, reg, value);
}

/** Read a register on the AT42QT2120
 *
 * @param reg The reg address to read
 *
 */
uint8_t AT42QT2120_read_reg(uint8_t reg)
{


    return i2c_read_8(AT42QT2120_SLAVE_ADDR, reg);
}


/******************************************************************************
 * Complete the following Functions
 *****************************************************************************/

/** Read Chip Status Low Register
 *
 *
 *
 */
uint8_t AT42QT2120_read_key_status_lo(void)
{
    return i2c_read_8(AT42QT2120_SLAVE_ADDR, 0x03);
}

/** Read Chip Status Hi Register
 *
 *
 *
 */
uint8_t AT42QT2120_read_key_status_hi(void)
{
    return i2c_read_8(AT42QT2120_SLAVE_ADDR, 0x04);
}

/**
 *
 *
 *
 */
uint8_t AT42QT2120_read_detection_status(void)
{
    return i2c_read_8(AT42QT2120_SLAVE_ADDR, 0x02);
}

/**
 *
 *
 *
 */
uint8_t AT42QT2120_read_slider_status(void)
{
    return i2c_read_8(AT42QT2120_SLAVE_ADDR, 0x05);
}



/******************************************************************************
 * Pulic API
 *****************************************************************************/

/******************************************************************************
 *
 *****************************************************************************/
void cap_sense_interrupt_init(void)
{
    // Input
    CAP_SEN_IRQ_PORT->DIR &= ~CAP_SEN_IRQ_PIN;

    // Low to High
    CAP_SEN_IRQ_PORT->IES |= CAP_SEN_IRQ_PIN;

    // Enable
    CAP_SEN_IRQ_PORT->IE |= CAP_SEN_IRQ_PIN;

    NVIC_EnableIRQ(CAP_SEN_IRQ_NUMBER);
    NVIC_SetPriority(CAP_SEN_IRQ_NUMBER, 0);
}

/******************************************************************************
 *
 *****************************************************************************/
uint8_t cap_sense_get_buttons(void)
{
    uint8_t button_status;

    // get the button status.  Need to read 4 status registers
    // even though we only use the status_low bits
     button_status = AT42QT2120_read_key_status_hi();
     button_status = AT42QT2120_read_detection_status();
     button_status = AT42QT2120_read_slider_status();

     button_status = AT42QT2120_read_key_status_lo();

     return button_status;
}




