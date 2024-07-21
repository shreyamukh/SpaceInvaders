/*
 * game.c
 *
 *  Created on: Apr 30, 2023
 *      Author: Shreya Mukherjee, Shawn Zhu
 */
#include "main.h"
#include "task_game_play.h"

/******************************************************************************
 * Task to run game and handle actions of the plater in regards to movement,  *
 * shooting and hitting the computer controlled alien ships.           		  *
 *    																		  *
 *    																		  *
 ******************************************************************************/
void Task_Game_Play(void *pvParameters) {
	//This is the image that we have to send to the queue to draw
	IMAGE updateImage;

//    // initialize random seed
//    srand(time(NULL));

//    // debounce state for S1
//    uint8_t debounce_state = 0x00;

// variables for background color, ship color, and alien color
	uint16_t background_color = LCD_COLOR_BLACK;
	uint16_t ship_color = LCD_COLOR_GREEN; //ship's color remains the same regardless of the background color;
	uint16_t alien_color = LCD_COLOR_WHITE;
	uint16_t bullet_color = LCD_COLOR_GREEN2; //the bullet will also always be the same
	uint16_t lux = light_get_lux();

	lcd_draw_rectangle(64, 64, SCREEN_WIDTH, SCREEN_HEIGHT, background_color);

	//Ship starting location
	LOCATION ship_location = { .x = SCREEN_WIDTH / 2, .y =
	SCREEN_HEIGHT - SHIP_HEIGHT, .height = SHIP_HEIGHT, .width =
	SHIP_WIDTH };

	//ship speed
	int SHIP_SPEED = 5;

	//Alien starting location
	LOCATION alien_location = { .x = (rand() % (SCREEN_WIDTH - ALIEN_WIDTH + 1))
			+ ALIEN_WIDTH / 2, .y = ALIEN_WIDTH / 2, .height = ALIEN_HEIGHT,
			.width = ALIEN_WIDTH };

	//Alien Object
	ALIEN alien = { .loc = alien_location, .alive = true };

	//ship speed
	int ALIEN_SPEED = 1;


	//Bullet starting location
	LOCATION bullet_location = { .x = SCREEN_WIDTH / 2 - SHIP_WIDTH / 2, .y =
	SCREEN_HEIGHT - SHIP_HEIGHT, .height = BULLET_HEIGHT, .width =
	BULLET_WIDTH };
	//bullet object
	BULLET bullet = { .loc = bullet_location, .in_use = false };

	//bullet speed
	int BULLET_SPEED = 3;

	//whether or not you lose the game
	bool win = true;

	// speed of cactus
	float speed_alien = 1.0;
	uint16_t speed_ship = 1;

	//main gameplay loop
	while (1) {
//        while (win)
//        {
		//make spaceship
		//set starting location (done outside the loop)

		//spaceship movement
		//receive the acc movement
		if (dir == ACCELEROMETER_DIR_LEFT) {
			if (ship_location.x > SHIP_WIDTH / 2) {
				ship_location.x = ship_location.x - SHIP_SPEED;
			} else {
				ship_location.x = SHIP_WIDTH / 2; //ternary operator. If ship's x is less then the width/2, then it can't move any further left
			}
		} else if (dir == ACCELEROMETER_DIR_RIGHT) {
			if (ship_location.x < SCREEN_WIDTH - (SHIP_WIDTH / 2)) {
				ship_location.x = ship_location.x + SHIP_SPEED; //ternary operator. If ship's x is greater than the screen - width/2, then it can't move any further right
			} else {
				ship_location.x = SCREEN_WIDTH - SHIP_WIDTH / 2;
			}
		}

		//now we have all the values. Set the update image and draw
		updateImage.x = ship_location.x;
		updateImage.y = ship_location.y;
		updateImage.height = ship_location.height;
		updateImage.width = ship_location.width;
		updateImage.image = shipBitmaps;
		updateImage.fColor = ship_color;
		updateImage.bColor = background_color;
		xQueueSendToBack(Queue_LCD, &updateImage, portMAX_DELAY);

//            //make aliens
//            //randomize the starting location. Always starts at the top. Randomize the x position (done outside the loop)
//
//            //alien movement
//            //every second, it moves down and a new alien is created at the top of the screen
//            if(alien_location.y > ship_location.y)
//            {
//                alien_location.y -= ALIEN_SPEED;
//            }
//
            //make bullet
            //shoot using button S1
            if (ece353_MKII_S1())
            {
                bullet.in_use = true;
            }
            else
            {
                bullet.in_use = false;
            }

            //bullet object has a condition where only if it is valid (after pressing S1), it will print and start moving
            //when bullet is not in use, move side to side with the ship. If in use, then stop moving with ship and move up
            if(bullet.in_use)
            {
                bullet_location.y += BULLET_SPEED;
            }
            else
            {
                bullet_location.x = ship_location.x;
            }

//            //Check for collision between Bullet and Alien
//            if(bullet.in_use && alien.alive && is_collided(bullet_location, alien_location))
//            {
//                bullet.in_use = false;
//                alien.alive = false;
//            }

//
//            //check for collision between Alien and Spaceship/Ground
//            if (alien.alive && (is_collided(ship_location, alien_location)||(alien_location.y <= ship_location.y)))
//            {
//                win = false;
//            }
//            //Print everything & Change colors based on lux
//            if(lux > 100)
//            {
//                background_color = LCD_COLOR_WHITE;
//                alien_color = LCD_COLOR_BLACK;
//            }
//            else
//            {
//                background_color = LCD_COLOR_BLACK;
//                alien_color = LCD_COLOR_WHITE;
//            }

		//loop around (increments the bullet up, the aliens down, and the spaceship according to the motion.

//        }

		// delay task
		vTaskDelay(pdMS_TO_TICKS(20));
	}

}

/**
 * Function to check if the pixel is on the screen
 *
 * @param loc to detect
 * @return true if in, false otherwise
 */
bool is_on_screen(LOCATION loc) {

	int x0, x1, y0, y1;

	x0 = loc.x - (loc.width / 2);
	x1 = loc.x + (loc.width / 2);

	y0 = loc.y - (loc.height / 2);
	y1 = loc.y + (loc.height / 2);

	if (x0 <= 0 || y0 <= 0 || x1 >= 128 || y1 >= 128) {
		return true;
	}
	return false;
}

/**
 * Detect if location sent overlaps with pixels of any aliens in queue
 * @param loc1
 * @return true if collied, false otherwise
 */
bool is_collided(LOCATION loc1, LOCATION loc2) {

	int xA_0, xA_1, yA_0, yA_1;

	xA_0 = loc1.x - (loc1.width / 2) + 2;
	xA_1 = loc1.x + (loc1.width / 2) - 2;

	yA_0 = loc1.y - (loc1.height / 2) + 2;
	yA_1 = loc1.y + (loc1.height / 2) - 2;

	int xB_0, xB_1, yB_0, yB_1;

	xB_0 = loc2.x - (loc2.width / 2) + 2;
	xB_1 = loc2.x + (loc2.width / 2) - 2;

	yB_0 = loc2.y - (loc2.height / 2) + 2;
	yB_1 = loc2.y + (loc2.height / 2) - 2;

	// checking for overlap
	if ((((xA_0 < xB_0) && (xB_0 < xA_1)) || ((xA_0 < xB_1) && (xB_1 < xA_1)))
			&& (((yA_0 < yB_0) && (yB_0 < yA_1))
					|| ((yA_0 < yB_1) && (yB_1 < yA_1)))) {
		return true;
	}

	return false;
}

// Function to check if the alien ship is still on the screen
bool is_alien_on_screen() {
	int x;
	for (x = 0; x < count; x++) {
		ALIEN checkAlien;
		xQueueReceive(Queue_ALIENS, &checkAlien, portMAX_DELAY);

		if (checkAlien.alive) {
			// Check if any part of the alien ship is off the screen
			if (checkAlien.loc.x == SCREEN_HEIGHT - SHIP_HEIGHT) {
				return true;
			}
		}
		xQueueSendToBack(Queue_ALIENS, &checkAlien, portMAX_DELAY);
	}
	return false;
}

