/*
 * task_song.c
 *
 *  Created on: May 2, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */
#include "task_song.h"

/******************************************************************************
* Task to play the sound effect of game
******************************************************************************/
void Task_Song(void *pvParameters)
{
    SOUND sound = NO_SOUND;

    while(1)
    {
        //receive the queue from the game task
        xQueueReceive(Queue_SONG, &sound, 0);
    }
}



