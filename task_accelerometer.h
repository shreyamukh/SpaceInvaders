/*
 * task_accelerometer.h
 *
 *  Created on: May 1, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */

#include <main.h>

#ifndef TASK_ACCELEROMETER_H_
#define TASK_ACCELEROMETER_H_

#define VOLT_0P85  1056      // 0.85 /(3.3/4096)
#define VOLT_2P50  3103      // 2.50 /(3.3/4096)

extern TaskHandle_t Task_Accelerometer_Handle;
extern TaskHandle_t Task_Accelerometer_Timer_Handle;

extern volatile uint32_t ACCELEROMETER_X_DIR;
extern volatile uint32_t ACCELEROMETER_Y_DIR;
extern volatile uint32_t ACCELEROMETER_Z_DIR;
typedef enum {
    ACCELEROMETER_DIR_CENTER,
    ACCELEROMETER_DIR_LEFT,
    ACCELEROMETER_DIR_RIGHT,
    ACCELEROMETER_DIR_UP,
    ACCELEROMETER_DIR_DOWN,
} ACCELEROMETER_DIR_t;

extern volatile ACCELEROMETER_DIR_t dir;




/******************************************************************************
* Used to start an ADC14 Conversion
******************************************************************************/
void Task_Accelerometer_Timer(void *pvParameters);


/******************************************************************************
* Examines the ADC data from the accelerometer on the MKII
******************************************************************************/
void Task_Accelerometer_Bottom_Half(void *pvParameters);

#endif /* TASK_ACCELEROMETER_H_ */
