/*
 * buzzer.c
 *
 *  Created on: Apr 30, 2023
 *      Author: shreyamukherjee
 */

#include "buzzer.h"

/******************************************************************************
 * Initializes the buzzer													  *
 * 																			  *
 * Parameters:																  *
 * 		None																  *
 * Returns:																	  *
 * 		None																  *
 ******************************************************************************/
void ece353_MKII_Buzzer_Init(void)
{
    // Setting P2.7 to be GPIO Output
    P2->DIR |= BIT7;

    // Configuring TIMER_A on buzzer for PWM
    // P2.7 - TA0.4
    P2->SEL0 |= BIT7;
    P2->SEL1 &= ~BIT7;

    // Turning off TA0
    TIMER_A0->CTL = 0;

    // Configuring TIMER_A0.4
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7; // Setting it to RESET/SET mode
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK; // Selecting master clock

}

/******************************************************************************
 * Turns on the buzzer   													  *
 *																			  *
 * Parameters:																  *
 * 		None																  *
 * Returns:																	  *
 * 		None																  *
 ******************************************************************************/
void buzzer_on(void){
    // clear the current mode control bits
	TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

    // set mode control to UP and clear the current count
	TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}

/******************************************************************************
 * Turns off the buzzer   													  *
 *																			  *
 * Parameters:																  *
 * 		None																  *
 * Returns:																	  *
 * 		None																  *
 ******************************************************************************/
void buzzer_off(void){
    // turn off timer
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}

void play_tone(int ticks_period) {
    TIMER_A0->CCR[0] = ticks_period - 1;
    TIMER_A0->CCR[BUZZER_CHANNEL] = ((ticks_period / 2) - 1);
}

/*
 * Checks buzzer run status
 *
 * Parameters:
 *      None
 * Returns:
 *      true    :   timer0A is on
 *      false   :   timer0A is off
 */

/******************************************************************************
 * Checks buzzer's run status												  *
 *																			  *
 * Parameters:																  *
 * 		None																  *
 * Returns:																	  *
 * 		true    :   timer0A is on											  *
 *      false   :   timer0A is off								     		  *
 ******************************************************************************/
bool ece353_MKII_Buzzer_Run_Status(void){

    if((TIMER_A0->CTL & TIMER_A_CTL_MC_MASK) == TIMER_A_CTL_MC__STOP) {
    	return false;
    }

    return true;
}



