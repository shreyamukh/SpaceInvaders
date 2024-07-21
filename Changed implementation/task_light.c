/*
 * task_light.c
 *
 *  Created on: May 1, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */
#include <task_light.h>

volatile light_t intensity_light = LIGHT_AVG;

// Initialize the opt3001 ambient light sensor on the MKII.
// This function assumes I2C interface has already been configured to 100KHz.
void _opt3001_init(void)
{
    // 1. Conversion time: 100ms
    // 2. Mode of conversion: Continuous conversion
    // Write to configure register
    i2c_write_16(OPT3001_SLAVE_ADDRESS, CONFIG_REG, 0xC610);
}

// Returns the current ambient light in Lux
float _opt3001_read_lux(void)
{
    // Read the Result register of OPT3001
    uint16_t read = i2c_read_16(OPT3001_SLAVE_ADDRESS, RESULT_REG);

    // calculate lux from read value
    return (double)0.01 * (double)pow(2, read>>12) * (double)(read & 0x0FFF);
}

// Get light intensity status
void update_light_status(void)
{
    float lux = _opt3001_read_lux();

    if (lux < LIGHT_THRESHOLD_LOW)
    {
        intensity_light = LIGHT_LOW;
    }
    else if (lux < LIGHT_THRESHOLD_HIGH)
    {
        intensity_light = LIGHT_AVG;
    }
    else
    {
        intensity_light = LIGHT_HIGH;
    }
}

/**
 * Task to handle actions and reactions to change in light intensity
 */
void Task_Light(void *pvParameters)
{
    // Detect light intensity
    while (1)
    {
    	update_light_status();
        // Delay for 10mS using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(50));
    }

}

/**
 * Initializing the liight sensor on the MKII
 */
void light_init()
{
    i2c_init();
    MKII_RGB_LED_init();
    _opt3001_init();

}
