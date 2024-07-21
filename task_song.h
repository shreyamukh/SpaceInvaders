/*
 * task_song.h
 *
 *  Created on: May 2, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */

#ifndef TASK_SONG_H_
#define TASK_SONG_H_

#include "main.h"

/******************************************************************************
* Task to play the sound effect of game
******************************************************************************/
void Task_Song(void *pvParameters);

/**
 * Sound effects
 */
typedef enum
{
    NO_SOUND,
    INTRO,
    SHOOTING,
    KILLED,
    LOSE,
    WIN
} SOUND;



#endif /* TASK_SONG_H_ */
