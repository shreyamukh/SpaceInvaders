/*
 * task_game_play.c
 *
 *  Created on: May 1, 2023
 *      Author: Shawn Zhu, Shreya Mukherjee
 */
#include "main.h"

// Task Game
// calculate next frame and send LCD parameters to Queue_LCD for Task_LCD to draw
void Task_Game(void *pvParameters) {

	// Define IMAGGE variables to hold the current and next frames for ship, alien and bullet
	IMAGE currentShipFrame;
	IMAGE currentBulletFrame;
	IMAGE currentAlienFrame;

	IMAGE nextShipFrame;
	IMAGE nextBulletFrame;
	IMAGE nextAlienFrame;

	IMAGE startFrame;

	// LOCATION VARIABLES

	//Start frame (the start up screen)
	// Initialize the current frame with the ship's initial position
	startFrame.width_pixels = SCREEN_WIDTH;
	startFrame.height_pixels = SCREEN_HEIGHT;
	startFrame.image = logoBitmaps;
	startFrame.bColor = LCD_COLOR_GREEN2;
	startFrame.fColor = LCD_COLOR_BLACK;
	startFrame.x = SCREEN_WIDTH/2;
	startFrame.y = SCREEN_HEIGHT/2;

	//SPACESHIP VARIABLES
	//Ship starting location
	LOCATION ship_location = { .x = SCREEN_WIDTH / 2, .y =
	SCREEN_HEIGHT - SHIP_HEIGHT, .height = SHIP_HEIGHT, .width =
	SHIP_WIDTH };
	//ship speed
	int SHIP_SPEED = 1;

	// Initialize the current frame with the ship's initial position
	currentShipFrame.width_pixels = ship_location.width;
	currentShipFrame.height_pixels = ship_location.height;
	currentShipFrame.image = shipBitmaps;
	currentShipFrame.bColor = LCD_COLOR_BLACK;
	currentShipFrame.x = ship_location.x;
	currentShipFrame.y = ship_location.y;

	//ALIEN VARIABLES
	//Alien starting location
	LOCATION alien_location = { .x = (rand()
			% (SCREEN_WIDTH - (ALIEN_WIDTH + 1))) + ALIEN_WIDTH / 2, .y =
	ALIEN_WIDTH / 2, .height = ALIEN_HEIGHT, .width = ALIEN_WIDTH };

	//Alien Object
	ALIEN alien = { .loc = alien_location, .alive = true };
	//ship speed
	int ALIEN_SPEED = 1;

	//BULLET VARIABLES
	//Bullet starting location
	LOCATION bullet_location = { .x = SCREEN_WIDTH / 2 - SHIP_WIDTH / 2, .y =
	SCREEN_HEIGHT - SHIP_HEIGHT, .height = BULLET_HEIGHT, .width =
	BULLET_WIDTH };

	//bullet object
	BULLET bullet = { .loc = bullet_location, .in_use = false };

	// Initialize the current frame with the ship's initial position
	currentBulletFrame.width_pixels = bullet_location.width;
	currentBulletFrame.height_pixels = bullet_location.height;
	currentBulletFrame.image = bulletBitmaps;
	currentBulletFrame.bColor = LCD_COLOR_BLACK;
	currentBulletFrame.x = bullet_location.x;
	currentBulletFrame.y = bullet_location.y;

	//bullet speed
	int BULLET_SPEED = 1;

	// Define a double buffer to hold the current and next frames

	// variables for background color, ship color, and alien color
	uint16_t background_color = LCD_COLOR_BLACK;
	uint16_t ship_color = LCD_COLOR_GREEN2; //ship's color remains the same regardless of the background color
	uint16_t alien_color = LCD_COLOR_WHITE;
	uint16_t bullet_color = LCD_COLOR_GREEN2; //the bullet color will also always be the same

	// variable to see if the game starts
	bool game = false;
	xQueueSendToBack(Queue_LCD, &startFrame, portMAX_DELAY);

	while (1) {

		while (!game){

			if(ece353_MKII_S2())
			{
				game = true;
				startFrame.bColor = background_color;
				startFrame.fColor = background_color;
				xQueueSendToBack(Queue_LCD, &startFrame, portMAX_DELAY);
			}
		}
		//
		ece353_MKII_RGB_LED(false, false, false);

//		//screen.fcolor = background_color;
//		xQueueSendToBack(Queue_LCD, &screen, portMAX_DELAY);
		// Draw the ship on the next frame buffer
		nextShipFrame.width_pixels = ship_location.width;
		nextShipFrame.height_pixels = ship_location.height;
		nextShipFrame.image = shipBitmaps;
		nextShipFrame.fColor = ship_color;
		nextShipFrame.bColor = background_color;
		nextShipFrame.x = ship_location.x;
		nextShipFrame.y = ship_location.y;

		// Send the next frame to the LCD queue
		xQueueSendToBack(Queue_LCD, &nextShipFrame, portMAX_DELAY);

		// Swap the current and next frame buffers
		IMAGE temp = currentShipFrame;
		currentShipFrame = nextShipFrame;
		nextShipFrame = temp;

		// Calculate the new position of the ship based on accelerometer data
		// Check if the ship has moved more than the threshold value
		if (ACCELEROMETER_DATA == ACCELEROMETER_DIR_RIGHT) {
			// Update the ship's position
			if (ship_location.x < 120) { // 120 so that the ship does not get cut off at the edge of the screen
				ship_location.x += 1;
			}

			// Draw the ship on the next frame
			nextShipFrame.width_pixels = ship_location.width;
			nextShipFrame.height_pixels = ship_location.height;
			nextShipFrame.image = shipBitmaps;
			nextShipFrame.bColor = background_color;
			nextShipFrame.x = ship_location.x;
			nextShipFrame.y = ship_location.y;

			// Send the next frame to the LCD queue
			xQueueSendToBack(Queue_LCD, &nextShipFrame, portMAX_DELAY);

			// Swap the current and next frame
			IMAGE temp = currentShipFrame;
			currentShipFrame = nextShipFrame;
			nextShipFrame = temp;
		}

		if (ACCELEROMETER_DATA == ACCELEROMETER_DIR_LEFT) {
			// Update the ship's position
			if (ship_location.x > 15) { // 15 so that the ship does not get cut off at the edge of the screen
				ship_location.x -= 1;
			}

			// Draw the ship on the next frame
			nextShipFrame.width_pixels = ship_location.width;
			nextShipFrame.height_pixels = ship_location.height;
			nextShipFrame.image = shipBitmaps;
			nextShipFrame.bColor = background_color;
			nextShipFrame.x = ship_location.x;
			nextShipFrame.y = ship_location.y;

			// Send the next frame to the LCD queue
			xQueueSendToBack(Queue_LCD, &nextShipFrame, portMAX_DELAY);

			// Swap the current and next frame
			temp = currentShipFrame;
			currentShipFrame = nextShipFrame;
			nextShipFrame = temp;
		}

//			// losing
//			//check for collision between Alien and Spaceship/Ground
//			if (alien.alive
//					&& (is_collided(ship_location, alien_location)
//							|| (alien_location.y <= ship_location.y))) {
//
//			}

		//make bullet
		//shoot using button S1
		// button pressed, jump
		if (ece353_MKII_S2()) {
			bullet.in_use = true;
		}

		temp = currentBulletFrame;
		currentBulletFrame = nextBulletFrame;
		nextBulletFrame = temp;


		if (bullet.in_use) {
			bullet_location.y -= BULLET_SPEED;
			// draw bullet if in the screen
			if (bullet_location.y <= SCREEN_HEIGHT - 2) {
				nextBulletFrame.width_pixels = bullet_location.width;
				nextBulletFrame.height_pixels = bullet_location.height;
				nextBulletFrame.image = bulletBitmaps;
				nextBulletFrame.fColor = bullet_color;
				nextBulletFrame.bColor = background_color;
				nextBulletFrame.x = (uint16_t) bullet_location.x;
				nextBulletFrame.y = bullet_location.y;

				// send next frame to Queue_LCD
				xQueueSendToBack(Queue_LCD, &nextBulletFrame, portMAX_DELAY);

				// Swap the current and next frame buffers
				IMAGE temp = currentBulletFrame;
				currentBulletFrame = nextBulletFrame;
				nextBulletFrame = temp;
			}
		} else {
			bullet_location.x = ship_location.x;
		}

		// ALIEN
		temp = currentAlienFrame;
		currentAlienFrame = nextAlienFrame;
		nextAlienFrame = temp;

		// Make the alien appear at the top of the screen and start falling again when it goes off the bottom
		if (!alien.alive) {
			// Reset the alien's location
			alien_location.x = (rand() % (SCREEN_WIDTH - (ALIEN_WIDTH + 1)))
					+ ALIEN_WIDTH / 2;
			alien_location.y = ALIEN_WIDTH / 2;
			alien.alive = true;
		}

		// Move the alien down the screen
		alien_location.y += ALIEN_SPEED;
		// Draw the alien on the next frame buffer
		nextAlienFrame.width_pixels = alien_location.width;
		nextAlienFrame.height_pixels = alien_location.height;
		nextAlienFrame.image = SIDownBitmaps;
		nextAlienFrame.bColor = background_color;
		nextAlienFrame.x = alien_location.x;
		nextAlienFrame.y = alien_location.y;
		nextAlienFrame.fColor = alien_color;

		// Check if the alien has gone off the bottom of the screen
		if (alien_location.y > SCREEN_HEIGHT - ALIEN_HEIGHT / 2 - 1) {
			// Make the alien appear at the top of the screen and start falling again
			nextAlienFrame.fColor = background_color;
			xQueueSendToBack(Queue_LCD, &nextAlienFrame, portMAX_DELAY);
			alien_location.x = (rand() % (SCREEN_WIDTH - (ALIEN_WIDTH + 1)))
					+ ALIEN_WIDTH / 2;
			alien_location.y = ALIEN_WIDTH / 2;
		}

		// Detecting if the alien has hit the ship
		if (bullet.in_use && alien.alive
				&& is_collided(bullet_location, alien_location)) {
			ece353_MKII_RGB_LED(true, false, false);
		}

		// Send the next frame to the LCD queue
		xQueueSendToBack(Queue_LCD, &nextAlienFrame, portMAX_DELAY);

		// Swap the current and next frame buffers
		temp = currentAlienFrame;
		currentAlienFrame = nextAlienFrame;
		nextAlienFrame = temp;

		//Check for collision between Bullet and Alien
		if (bullet.in_use && alien.alive
				&& is_collided(alien.loc, bullet.loc)) {
			bullet.in_use = false;
			bullet_location.x = ship_location.x;
			bullet_location.y = ship_location.y;

			alien.alive = false;
			nextAlienFrame.fColor = background_color;
			xQueueSendToBack(Queue_LCD, &nextAlienFrame, portMAX_DELAY);
			alien_location.x = (rand() % (SCREEN_WIDTH - (ALIEN_WIDTH + 1)))
					+ ALIEN_WIDTH / 2;
			alien_location.y = ALIEN_WIDTH / 2;
		}
		if (bullet.loc.y < BULLET_HEIGHT / 2) {

		}

		// Simple collision detection
		if (is_collided(ship_location, alien_location)) {
			ece353_MKII_RGB_LED(true, false, false);
			music_play_hit();
		}

		if (intensity_light == LIGHT_LOW) {
			alien_color = LCD_COLOR_CYAN;
			bullet_color = alien_color;
			ship_color = LCD_COLOR_BLUE;
		} else if (intensity_light == LIGHT_HIGH) {
			alien_color = LCD_COLOR_WHITE;
			bullet_color = alien_color;
			ship_color = LCD_COLOR_GREEN2;
		} else {
			alien_color = LCD_COLOR_ORANGE;
			bullet_color = alien_color;
			ship_color = LCD_COLOR_MAGENTA;
		}

		// delay for 10mS using vTaskDelay
		vTaskDelay(pdMS_TO_TICKS(20));
	}

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

///**
// * function to change the location of the alien to start back at the top and fall again
// */
//void alien_change_location() {
//	alien_location.x = (rand() % (SCREEN_WIDTH - (ALIEN_WIDTH + 1)))
//			+ ALIEN_WIDTH / 2;
//	alien_location.y = ALIEN_WIDTH / 2;
//}

