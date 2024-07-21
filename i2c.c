/*
 * i2c.c
 *
 *  Created on: Apr 24, 2023
 *      Author: shreyamukherjee
 */

#include "i2c.h"



/**********************************************************************************************
 * Sets the Slave address
 **********************************************************************************************/
static __inline void i2c_set_subordinate_address(uint8_t subordinate_address)
{
    EUSCI_B1->I2CSA = subordinate_address;
}
/**********************************************************************************************
 * Writes a byte of data to the Tx Buffer
 **********************************************************************************************/
static __inline void i2c_tx_byte(uint8_t data)
{
    EUSCI_B1->TXBUF = data;
}

/**********************************************************************************************
 * Sets the eUSCI_B into Transmit Mode
 **********************************************************************************************/
static __inline void i2c_set_tx_mode(void)
{
    EUSCI_B1->CTLW0 |= BIT4;
}

/**********************************************************************************************
 * Sets the eUSCI_B into Receive mode
 **********************************************************************************************/
static __inline void i2c_set_rx_mode(void)
{
    EUSCI_B1->CTLW0 &= ~BIT4;
}

/**********************************************************************************************
 * Sends the Start Condition and Control Word
 **********************************************************************************************/
static __inline void i2c_send_start(void)
{
    EUSCI_B1->CTLW0 |= BIT1;

}

/**********************************************************************************************
 * Generates a Stop Bit
 **********************************************************************************************/
static __inline void i2c_send_stop(void)
{
    EUSCI_B1->CTLW0 |= BIT2;
}

/**********************************************************************************************
 * Busy wait until the Transmit interrupt flag is set
 **********************************************************************************************/
static __inline void i2c_wait_for_tx(void)
{
    while((EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0) == 0);
}

/**********************************************************************************************
 * Busy wait until the Stop Interrupt flag is set
 **********************************************************************************************/
static __inline void i2c_wait_for_stop(void)
{
    while ((EUSCI_B1->IFG & EUSCI_B_IFG_STPIFG) == 0);
}

/**********************************************************************************************
 * Busy wait until the receive interrupt flag is set.  After it has been set, return the byte
 * of data that was received.
 **********************************************************************************************/
static __inline uint8_t i2c_wait_for_rx(void)
{
    while((EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG0) == 0);
    return EUSCI_B1->RXBUF;
}

/**********************************************************************************************
 * Busy wait while the I2C bus is busy.  Note:  This flag is not in the IFG register
 **********************************************************************************************/
static __inline void i2c_wait_busy(void)
{
    while ((EUSCI_B1->STATW & BIT4) != 0);
}

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_clear_interrupts(void)
{
    EUSCI_B1->IFG = 0;
}

/**********************************************************************************************
 * Initialize I2C
 **********************************************************************************************/
void i2c_init(void)
{
    P6->SEL0 |= (BIT5 | BIT4);                // I2C pins
    P6->SEL1 &= ~(BIT5 | BIT4);                // I2C pins

    EUSCI_B1->CTLW0 |=  EUSCI_A_CTLW0_SWRST;            // Software reset enabled

    EUSCI_B1->CTLW0 =   EUSCI_A_CTLW0_SWRST |           // Remain eUSCI in reset mode
                        EUSCI_B_CTLW0_MODE_3 |          // I2C mode
                        EUSCI_B_CTLW0_MST |             // Monarch mode
                        EUSCI_B_CTLW0_SYNC |            // Sync mode
                        EUSCI_B_CTLW0_SSEL__SMCLK;      // SMCLK

    EUSCI_B1->BRW =     SystemCoreClock/100000;         // baudrate = SMCLK / ? = 100kHz

    EUSCI_B1->CTLW0 &=  ~EUSCI_A_CTLW0_SWRST;           // Release eUSCI from reset

}



/**********************************************************************************************
 * Write an I2C device
 **********************************************************************************************/
void i2c_write_16(uint8_t subordinate_address, uint8_t reg_address, uint16_t data)
{

    // Wait until the I2C bus is not being used
    i2c_wait_busy();

    // Put in Transmit mode
    i2c_set_tx_mode();

    // clear any outstanding interrupts
    i2c_clear_interrupts();

    // Set the Slave Address
    i2c_set_subordinate_address(subordinate_address);

    // Send the Start Bit and I2C Address
    i2c_send_start();

    // Wait for Tx buffer to become available
    i2c_wait_for_tx();

    // Send device address
    i2c_tx_byte(reg_address);

    // Wait for Tx buffer to become available.
    i2c_wait_for_tx();

    // Send upper byte of data
    i2c_tx_byte(data>>8);

    // Wait for Tx buffer to become available
    i2c_wait_for_tx();

    // send lower byte of data
    i2c_tx_byte(data);

    i2c_wait_for_tx();

    // Send the STOP Bit
    i2c_send_stop();

    // Wait until the last byte is transmitted
    i2c_wait_for_stop();

}

/**********************************************************************************************
 * Read an I2C device
 **********************************************************************************************/
uint16_t i2c_read_16(uint8_t subordinate_address, uint8_t reg_address)
{
    uint8_t upper_byte;
    uint8_t lower_byte;

    i2c_wait_busy();

    // Put in Transmit mode
    i2c_set_tx_mode();

    // clear interrupts
    i2c_clear_interrupts();

    // Set the Slave Address
    i2c_set_subordinate_address(subordinate_address);

    // Send the Start Bit and I2C Address
    i2c_send_start();

    // Wait for Tx buffer to become available
    i2c_wait_for_tx();

    // Send device address
    i2c_tx_byte(reg_address);

    i2c_wait_for_tx();

    // Send the STOP Bit
    i2c_send_stop();

    i2c_wait_for_stop();

    // Put in Receive mode
    i2c_set_rx_mode();

    // Send the Start Bit and I2C Address
    i2c_send_start();

    // Wait for the first byte
    upper_byte = i2c_wait_for_rx();

    // Send a NACK and STOP Bit
    i2c_send_stop();

    // Wait for the 2nd byte
    lower_byte = i2c_wait_for_rx();

    i2c_wait_for_stop();

    return ((uint16_t)(upper_byte) << 8) | lower_byte;

}



