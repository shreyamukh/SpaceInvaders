/*
 * buzzer.h
 *
 *  Created on: Apr 30, 2023
 *      Author: shreyamukherjee
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "msp.h"
#include <stdint.h>
#include <stdbool.h>

#define BUZZER_PORT P2
#define BUZZER_PIN  BIT7

#define BUZZER_TIMER    TIMER_A0
#define BUZZER_CHANNEL  4

/*
 * Initialize Buzzer
 *
 * Parameters:
 *      None
 * Returns:
 *      None
 */
void ece353_MKII_Buzzer_Init(void);

/*
 * Turns buzzer on
 *
 * Parameters:
 *      None
 * Returns:
 *      None
 */
void buzzer_on(void);


/*
 * Turns buzzer oFF
 *
 * Parameters:
 *      None
 * Returns:
 *      None
 */
void buzzer_off(void);

/*
 * Set current buzzer tone
 *
 * Parameters:
 *      ticks_period tone to set
 * Returns:
 *      None
 */
void play_tone(int ticks_period);

/*
 * Checks buzzer run status
 *
 * Parameters:
 *      None
 * Returns:
 *      true    :   timer0A is on
 *      false   :   timer0A is off
 */
bool buzzer_run_status(void);



#endif /* BUZZER_H_ */
