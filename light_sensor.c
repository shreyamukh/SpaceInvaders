/*
 * light_sensor.c
 *
 *  Created on: Apr 24, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */

#include <light_sensor.h>

/******************************************************************************
 * Initialize the ambient light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void light_init(void)
{
    int i;

    // Reset the device using the CONFIG register
    i2c_write_16(I2C_LIGHT_ADDR, I2C_LIGHT_CONFIG, AM_LIGHT_RST);

    // delay
    for(i = 0; i < 50000; i++){};

    // Program the CONFIG register to POWER_UP
    i2c_write_16(I2C_LIGHT_ADDR, I2C_LIGHT_CONFIG, AM_LIGHT_POWERUP);
}

/******************************************************************************
 * Returns the lux value after conversion from ambient light sensor
 ******************************************************************************/
float light_get_lux(void)
{
    uint16_t raw;

    // Read the raw data from light sensor
    raw = i2c_read_16(I2C_LIGHT_ADDR, I2C_LIGHT_RESULT);

    uint16_t exp = raw & 0xF000; // exponent
    uint16_t frac = raw & 0x0FFF; // fraction

    float lux = 0.01 * pow(2, exp) * frac; // lux value

    return lux;
}


