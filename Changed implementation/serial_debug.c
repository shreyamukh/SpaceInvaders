/*
 * serial_debug.c
 *
 *  Created on: Mar 7, 2023
 *      Author: shreyamukherjee
 */

#include "serial_debug.h"

// ADD CODE  - Copy this line after you #includes
Circular_Buffer *Tx_Buffer;

// create global string array to hold a message up to 80 characters
volatile char Rx_String[80];

// create global variable to hold message count
volatile uint16_t Rx_Char_Count = 0;

// create flag to tell when new message has arrived
volatile bool ALERT_STRING = false;

//****************************************************************************
// Helper function used to configure the eUSCI
//****************************************************************************
static void serial_debug_init_uart(void)
{
    // Configure the IO pins used for the UART
    // set 2-UART pin as secondary function
    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2|BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | EUSCI_B_CTLW0_SSEL__SMCLK; // Remain in reset  and configure clock source for SMCLK

    // Baud Rate calculation
    // 24000000/(16*115200) = 13.020833
    // Fractional portion = 0.020833
    // UCBRFx = int ( (13.020833-13)*16) = 0
    EUSCI_A0->BRW = 13; //24000000/16/115200

    // Set the fractional portion of the baud rate and turn
    // on oversampling
    EUSCI_A0->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;

    // Enable eUSCI in UART mode
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

    // Clear any outstanding interrupts
    EUSCI_A0->IFG &= ~(EUSCI_A_IFG_RXIFG | EUSCI_A_IFG_TXIFG);

    // Enable Rx Interrupt
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;

    // Enable EUSCIA0 Interrupt
    NVIC_EnableIRQ(EUSCIA0_IRQn);

    // Prime the pump -- ADD CODE
    EUSCI_A0->TXBUF = 0;
}

//****************************************************************************
// Initializes the EUSCI_A0 peripheral to be a UART with a baud rate of 115200
//
// NOTE: This function assumes that the SMCLK has been configured to run at
// 24MHz.  Please change __SYSTEM_CLOCK in system_msp432p401r.c to the
// correct value of 24000000
// ****************************************************************************/
void serial_debug_init(void)
{
    // Initialize the UART
    serial_debug_init_uart();

    // ADD CODE that initializes a circular buffer of size 80 and sets it
    // equal to Tx_Buffer

    Tx_Buffer = circular_buffer_init(80);

}

//****************************************************************************
// Prints a string to the serial debug UART
// ****************************************************************************/
void serial_debug_put_string(char * s)
{
    while(*s != 0)
    {
        // while UART is busy, wait
        while(EUSCI_A0->STATW & EUSCI_A_STATW_BUSY){};

        EUSCI_A0->TXBUF = *s;

        // advance to the next character
        ++s;
    }
}


//****************************************************************************
// This function is called from MicroLIB's stdio library.  By implementing
// this function, MicroLIB's putchar(), puts(), printf(), etc will now work.
// ****************************************************************************/
int fputc(int c, FILE* stream)
{
    // Busy wait while the circular buffer is full -- ADD CODE
    while(circular_buffer_full(Tx_Buffer)){
    }

    // globally disable interrupts
    __disable_irq();
    // add the character to the circular buffer  -- ADD CODE
    circular_buffer_add(Tx_Buffer, c);
    // globally disable interrupts
      __enable_irq();

      NVIC_EnableIRQ(EUSCIA0_IRQn);
    // Enable Tx Empty Interrupts  -- ADD CODE
      EUSCI_A0->IE |= EUSCI_A_IE_TXIE;

    return 0;
}
//****************************************************************************
// UART interrupt service routine
// ****************************************************************************/
void EUSCIA0_IRQHandler(void)
{
    char next;
    // Check for Rx Interrupts
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        if (EUSCI_A0->RXBUF != '\n' && EUSCI_A0->RXBUF != '\r') {
            Rx_String[Rx_Char_Count] = EUSCI_A0->RXBUF;
            Rx_Char_Count++;
        }
        else {
            Rx_String[Rx_Char_Count] = '\0';
            Rx_Char_Count++;
            ALERT_STRING = true;
        }


    }

    // Check for Tx Interrupts
      if (EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)
      {
          //Check to see if the Tx circular buffer is empty
          if(circular_buffer_empty(Tx_Buffer))
          {
              // Disable Tx Interrupts -- ADD CODE
              EUSCI_A0->IE &= ~EUSCI_A_IE_TXIE;

          }
          else
          {
              // Get the next char from the circular buffer -- ADD CODE
              next = circular_buffer_remove(Tx_Buffer);

              // Transmit the character -- ADD CODE
              EUSCI_A0->TXBUF = next;
          }
      }
}

