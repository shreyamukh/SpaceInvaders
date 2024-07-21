/*
 * task_aliens.c
 *
 *  Created on: May 3, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */
#include "main.h"
#include "task_aliens.h"

// alien colors
// have to change
volatile uint16_t aliens_fgColor = 0xFFFF; // white for now
volatile uint16_t aliens_bgColor = 0x0000; // black for now
volatile int count = 0;

void Task_Aliens(void *pvParameters)
{
    srand(time(NULL));

    // create a new alien
    create_alien();

    count++;

    // move and render aliens
    move_aliens();
    //draw_aliens();

    // delay for 1 second
    vTaskDelay(pdMS_TO_TICKS(100));

}

// Function to create a new alien
void create_alien(void)
{
// Generate random location along the x-axis within the screen
    int alien_start_x = generate_random_in_range(
            ALIEN_WIDTH / 2, (SCREEN_WIDTH - (ALIEN_WIDTH / 2)) - 1);

// Generate random location within top 40 pixels of the screen
    int alien_start_y = 40;

    LOCATION location = { .x = alien_start_x, alien_start_y, ALIEN_WIDTH,
                          ALIEN_HEIGHT };

// Initialize alien location and movement variables
    ALIEN createAlien = { location, true };

    xQueueSendToBack(Queue_ALIENS, &createAlien, portMAX_DELAY);
}

// Function to move all alive aliens, and create new aliens when necessary
void move_aliens(void) {
	ALIEN moveAlien;
	xQueueReceive(Queue_ALIENS, &moveAlien, portMAX_DELAY);

	int new_y = moveAlien.loc.y - 20;
	int new_x = generate_random_in_range(2, 130);
	moveAlien.loc.x = new_x;
	moveAlien.loc.y = new_y;

	xQueueSendToBack(Queue_ALIENS, &moveAlien, portMAX_DELAY);
}

/**
 // Function to draw all alive aliens
 void draw_aliens(void)
 {
 ALIEN drawAlien;
 xQueueReceive(Queue_ALIENS, &drawAlien, portMAX_DELAY);

 if (drawAlien.alive)
 {
 // Draw alien at its current location
 xSemaphoreTake(Sem_RENDER, portMAX_DELAY);
 lcd_draw_image(drawAlien.loc.x, drawAlien.loc.y,
 ALIEN_WIDTH,
 ALIEN_HEIGHT, SIDownBitmaps, aliens_fgColor,
 aliens_bgColor);
 xSemaphoreGive(Sem_RENDER);
 }

 xQueueSendToBack(Queue_ALIENS, &drawAlien, portMAX_DELAY);
 }
 **/

/**
 * Generate a random int within the range (inclusive)
 * @param lower
 * @param upper
 * @return random int
 * CHECKING
 */
int generate_random_in_range(int lower, int upper)
{
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

/**
 // check if 0.5 seconds have elapsed
 if ((double) (clock() - last_switch_time) / CLOCKS_PER_SEC >= 0.5)
 {
 // switch the image
 if (is_alien_up)
 {
 // load and display alien_down.png
 is_alien_up = false;
 }
 else
 {
 // load and display alien_up.png
 is_alien_up = true;
 }

 last_switch_time = clock();
 }
 **/
