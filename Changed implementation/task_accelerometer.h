/*
 * task_accelerometer.h
 *
 *  Created on: May 1, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */
#ifndef TASK_ACCELEROMETER_H_
#define TASK_ACCELEROMETER_H_

#include "main.h"

// Possible states of the joystick.
typedef enum Accelerometer_States
{
    ACCELEROMETER_DIR_LEFT,
    FLAT,
    ACCELEROMETER_DIR_RIGHT,
} ACCELEROMETER_DIR_t;

extern volatile ACCELEROMETER_DIR_t ACCELEROMETER_DATA;

// Private functions (only called in task_joystick.c)
void accelerometer_init();

// Task Joystick
void Task_Accelerometer_Timer(void *pvParameters);
void Task_Accelerometer_Bottom_Half(void *pvParameters);

void ADC14_IRQHandler(void);

// Called in main.c
void Accel_init();

#endif /* TASK_BUTTON_H_ */
