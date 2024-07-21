/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_freertos.c ========
 */
#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>
#include "main.h"

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>

/*
 *  ======== main ========
 */
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    //setup all peripherials
    ece353_setup_all();

    //create welcome message


    //initialize queue
    Queue_ACC_to_GAME = xQueueCreate(2,sizeof(ACCELEROMETER_DIR_t));
    Queue_SONG = xQueueCreate(2,sizeof(SOUND));
    Queue_ALIENS = xQueueCreate(10, sizeof(ALIEN));
    Queue_LCD = xQueueCreate(250, sizeof(IMAGE));

    //initialize semaphore
    Sem_GAME_PLAY = xSemaphoreCreateBinary();
    xSemaphoreGive(Sem_GAME_PLAY);
    Sem_RENDER = xSemaphoreCreateBinary();
    xSemaphoreGive(Sem_RENDER);
    Sem_PRINT = xSemaphoreCreateBinary();

    //create tasks
    xTaskCreate
    (   Task_Song,
        "Task_Song",
        configMINIMAL_STACK_SIZE,
        NULL,
        0,
        &Task_Song_Handle
    );

    xTaskCreate
     (   Task_Aliens,
         "Task_Aliens",
         configMINIMAL_STACK_SIZE,
         NULL,
         2,
         &Task_Aliens_Handle
     );

    xTaskCreate
    (   Task_Game_Play,             //task function name
        "Task_Game_Play",           //task name
        configMINIMAL_STACK_SIZE,
        NULL,
        5,                          //priority
        &Task_Game_Play_Handle     //task handle
    );

    xTaskCreate
    (   Task_LCD,
        "Task_LCD",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_LCD_Handle
    );

    xTaskCreate
    (   Task_Accelerometer_Timer,
        "Task_Accelerometer_Timer",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_Accelerometer_Timer_Handle
    );

    xTaskCreate
    (   Task_Accelerometer_Bottom_Half,
        "Task_Accelerometer_Bottom_Half",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_Accelerometer_Bottom_Half_Handle
    );

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    while(1){};
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
    while(1)
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
    while(1)
    {
    }
}
