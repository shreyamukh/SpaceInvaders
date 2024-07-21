/*
 * game.h
 *
 *  Created on: Apr 30, 2023
 *      Author: shreyamukherjee
 */

#ifndef GAME_H_
#define GAME_H_

#include <main.h>

/**
 * used to store an object location on screen
 */
typedef struct
{
    int x;
    int y;
    int height;
    int width;
} LOCATION;


/**
 * store bullet status/ create object
 */
typedef struct
{
    LOCATION loc;
    ADC_MOVE dir;
    bool in_use;
} BULLET;

/******************************************************************************
* Task to handle player's action, movement, shooting, and collision with npc
******************************************************************************/
void Task_Game_Host(void *pvParameters);

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
bool is_in_boarder(LOCATION loc);

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


#endif /* GAME_H_ */
