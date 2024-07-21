/*
 * task_mkII_s1.c
 *
 *  Created on: Oct 14, 2020
 *      Author: Shawn Zhu,
 */

#include <main.h>


/******************************************************************************
 * De-bounce switch S1.  If is has been pressed, change the tri-Color LED on
 * the MKII. Everytime S1 is pressed, the color should change in
 * the following order:
 *      RED->GREEN->BLUE->RED->GREEN....
 *****************************************************************************/
void Task_MKII_S1(void *pvParameters)
{
    // Declare a uint8_t variable that will be used to de-bounce S1
    uint8_t debounce_state = 0x00;

    while(1)
    {
        /******************************************************************
         * Shift the de-bounce variable to the left
         *
         * ADD CODE
         ******************************************************************/
        debounce_state <<= 1;

        /******************************************************************
         * If S1 is being pressed, set the LSBit of debounce_state to a 1;
         *
         * ADD CODE
         ******************************************************************/
        if(ece353_MKII_S1())
        {
            debounce_state |= 0x01;
        }

        /******************************************************************
         * If the de-bounce variable is equal to 0x7F, change the color
         * of the tri-color LED.
         ******************************************************************/
        if(debounce_state == 0x7F)
        {
            /******************************************************************
             * give the Sem_S1_to_GAME semaphore
             *
             * ADD CODE
             ******************************************************************/
            xSemaphoreGive(Sem_S1_to_GAME);
        }

        // Delay for 10mS using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}