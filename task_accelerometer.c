/*
 * task_accelerometer.c
 *
 *  Created on: May 1, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */
#include "task_accelerometer.h"

volatile ACCELEROMETER_DIR_t dir = ACCELEROMETER_DIR_CENTER; //the current direction

/******************************************************************************
 * Used to start an ADC14 Conversion
 ******************************************************************************/
void Task_Accelerometer_Timer(void *pvParameters) {
	while (1) {
		// Start ADC conversion.
		ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

		// Delay for 10mS using vTaskDelay
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

/******************************************************************************
 * Examines the ADC data from the accelerometer on the MKII
 ******************************************************************************/
void Task_Accelerometer_Bottom_Half(void *pvParameters) {

	uint32_t ACCELEROMETER_X_DIR = 0;

	/* ADD CODE
	 * Wait until we get a task notification from the ADC14 ISR
	 */
	while (1) {
		// Get/Wait for task notification with x-axis value
		if (xTaskNotifyWait(0, 0, &ACCELEROMETER_X_DIR, portMAX_DELAY) != pdPASS) {
			continue;
		}

		/*
		 * Set the dir variable to one of the following values based
		 * on the values of ACCELEROMETER_X_DIR and ACCELEROMETER_Y_DIR
		 */
		if (ACCELEROMETER_X_DIR < 1750) {
			dir = ACCELEROMETER_DIR_LEFT;
		}
		else if (ACCELEROMETER_X_DIR > 2150) {
			dir = ACCELEROMETER_DIR_RIGHT;
		}
		else {
			dir = ACCELEROMETER_DIR_CENTER;
		}
	}

}

/******************************************************************************
 * Top Half of ADC14 Handler.
 ******************************************************************************/
void ADC14_IRQHandler(void) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	uint32_t ACCELEROMETER_X_DIR = 0;
	ACCELEROMETER_X_DIR = ADC14->MEM[2]; // Read the value and clear the interrupt

	/*
	 * Send a task notification to Task_Accelerometer_Bottom_Half
	 */
	xTaskNotifyFromISR(Task_Accelerometer_Bottom_Half_Handle,
			ACCELEROMETER_X_DIR, eSetValueWithOverwrite,
			&xHigherPriorityTaskWoken);

}

