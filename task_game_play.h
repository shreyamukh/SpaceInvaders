/*
 * game.h
 *
 *  Created on: Apr 30, 2023
 *      Author: shreyamukherjee
 */

#ifndef TASK_GAME_PLAY_H_
#define TASK_GAME_PLAY_H_

#include <main.h>


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



/******************************************************************************
 * Task to handle player's action, movement, shooting, and collision with npc
 ******************************************************************************/
void Task_Game_Play(void *pvParameters);

/**
 * Detect if this location is in boarder
 *
 * @param loc to detect
 * @return true if in, false otherwise
 */
LOCATION boarder_range_validate(LOCATION loc);

/**
 * Reset the location if it is invalid (exceed the range)
 *
 * @param loc to check
 * @return new loc that is valid
 */
bool is_on_screen(LOCATION loc);

/**
 * Detect if two location is collied/overlappped
 * @param loc1
 * @param loc2
 * @return true if collied, false otherwise
 */
bool is_collided(LOCATION loc1, LOCATION loc2);

/**
 * Generate a random int with in the range (inclusive)
 * @param lower
 * @param upper
 * @return random int
 */
int generate_random_in_range(int lower, int upper);

void remove_alien(LOCATION loc1);

#endif /* TASK_GAME_PLAY_H_ */
