/*
 * task_aliens.h
 *
 *  Created on: May 3, 2023
 *      Author: pokem
 */

#ifndef TASK_ALIENS_H_
#define TASK_ALIENS_H_

#include <main.h>
extern volatile uint16_t aliens_fgColor;
extern volatile uint16_t aliens_bgColor;

extern volatile int count;
void Task_Aliens(void *pvParameters);
int generate_random_in_range(int lower, int upper);
void create_alien(void);
void move_aliens(void);
void draw_aliens(void);


#endif /* TASK_ALIENS_H_ */
