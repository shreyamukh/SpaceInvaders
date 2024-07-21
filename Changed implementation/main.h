/*
 * main.h
 *
 *  Created on: May 1, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */
#ifndef MAIN_H_
#define MAIN_H_

// C library
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "msp.h"
#include "msp432p401r.h"
#include <stdio.h>
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
#include <sound.h>
#include <task_light.h>
#include <task_lcd.h>
#include <task_accelerometer.h>
#include <task_game_play.h>
#include <task_LCD.h>

// Queue declaration
QueueHandle_t Queue_LCD;

// intializing task handles
TaskHandle_t Task_Accelerometer_Handle;
TaskHandle_t Task_Accelerometer_Timer_Handle;
TaskHandle_t Task_LCD_Handle;
TaskHandle_t Task_Game_Handle;
TaskHandle_t Task_Light_Handle;


#endif /* MAIN_H_ */
