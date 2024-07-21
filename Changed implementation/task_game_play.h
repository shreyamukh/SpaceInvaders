/*
 * task_game_play.h
 *
 *  Created on: May 1, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */
#ifndef TASK_GAME_PLAY_H_
#define TASK_GAME_PLAY_H_

#include "main.h"
//#include <task_song.h>
//#include <task_aliens.h>
#include <task_LCD.h>

// get random number from hardware noise
#include <task_accelerometer.h>

/**
 * used to store an object location on screen
 */
typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t height;
	uint16_t width;
} LOCATION;

/**
 * store bullet status/ create object
 */
typedef struct {
	LOCATION loc;
	bool in_use;
} BULLET;

/**
 * alien status
 */
typedef struct {
	LOCATION loc;
	bool alive;
} ALIEN;


#define SCREEN_WIDTH 132
#define SCREEN_HEIGHT 132

// position restriction
#define SHIP_x_MIN 2
#define SHIP_x_MAX 130
#define BULLET_Y_MAX 130
#define BULLET_Y_MIN SHIP_HEIGHT

// Task Game
void Task_Game(void *pvParameters);

bool is_collided(LOCATION loc1, LOCATION loc2);
#endif /* TASK_GAME_PLAY_H_ */
