/*
 * light_sensor.h
 *
 *  Created on: Apr 24, 2023
 *      Author: Shreya Mukherjee, Shawn Zhu
 */

#ifndef LIGHT_SENSOR_H_
#define LIGHT_SENSOR_H_

#include "msp.h"
#include "i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define I2C_LIGHT_ADDR   0x44

#define I2C_LIGHT_RESULT            0x00
#define I2C_LIGHT_CONFIG            0x01
#define I2C_LIGHT_DEV_ID            0x7F

/* CONFIGURATION REGISTER SETTINGS */
#define AM_LIGHT_RST                0xC810
#define AM_LIGHT_POWERUP            0x0400

/******************************************************************************
 * Initialize the ambient light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void light_init(void);

/******************************************************************************
 * Returns the lux value after conversion from ambient light sensor
 * Note: In Auto Mode, exp is 0, lux_value < 10 is dark, max 40.95
 ******************************************************************************/
float light_get_lux(void);


#endif /* LIGHT_SENSOR_H_ */
