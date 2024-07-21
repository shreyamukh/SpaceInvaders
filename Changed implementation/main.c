/*
 * main.c
 *
 *  Created on: May 1, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */

#include "main.h"

/*
 *  ======== main ========
 */
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // initialize peripherals
    ece353_setup_all();

    // creating queues
    Queue_LCD = xQueueCreate(250,sizeof(IMAGE));

    xTaskCreate
    (   Task_Light,
        "Task light Intensity",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_Light_Handle
    );

    // Create task for LCD
    xTaskCreate
    (   Task_LCD,
        "Update LCD frame",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_LCD_Handle
    );

    xTaskCreate(Task_Game, "Start the game",
    	configMINIMAL_STACK_SIZE,
    	NULL, 4, &Task_Game_Handle);

    xTaskCreate
      (   Task_Accelerometer_Timer,
          "Task Accelerometer Timer",
          configMINIMAL_STACK_SIZE,
          NULL,
          4,
          &Task_Accelerometer_Timer_Handle
      );

      // Create Accelerometer Bottom Half Task.
      xTaskCreate
      (   Task_Accelerometer_Bottom_Half,
          "Task Accelerometer Read",
          configMINIMAL_STACK_SIZE,
          NULL,
          4,
          &Task_Accelerometer_Handle
      );

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    while (1)
    {
    };
    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while (1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while (1)
    {
    }
}
