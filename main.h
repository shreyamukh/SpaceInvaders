/*
 * main.h
 *
 *  Created on: May 1, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */

#ifndef PROJECT_MAIN_H_
#define PROJECT_MAIN_H_


#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* ECE353 Includes */
#include <ece353.h>
#include <images.h>
#include <buzzer.h>
#include <task_game_play.h>
#include <task_accelerometer.h>
#include <task_song.h>
#include <task_aliens.h>
#include <task_LCD.h>


// define semaphore
//SemaphoreHandle_t Sem_SEMAPHORE_NAME;
SemaphoreHandle_t Sem_GAME_PLAY;
SemaphoreHandle_t Sem_RENDER;
SemaphoreHandle_t Sem_PRINT;

// define handle
//TaskHandle_t Task_TASK_NAME;
TaskHandle_t Task_Accelerometer_Bottom_Half_Handle;
TaskHandle_t Task_Accelerometer_Timer_Handle;
TaskHandle_t Task_Aliens_Handle;
TaskHandle_t Task_Game_Play_Handle;
TaskHandle_t Task_Song_Handle;
TaskHandle_t Task_LCD_Handle;

// define queue
//QueueHandle_t Queue_QUEUE_NAME;
QueueHandle_t Queue_ACC_to_GAME;
QueueHandle_t Queue_SONG;
QueueHandle_t Queue_ALIENS;
QueueHandle_t Queue_LCD;



#endif /* PROJECT_MAIN_H_ */
