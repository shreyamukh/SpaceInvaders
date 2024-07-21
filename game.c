/*
 * game.c
 *
 *  Created on: Apr 30, 2023
 *      Author: Shreya Mukherjee, Shawn Zhu
 */
#include "main.h"

/******************************************************************************
 * Task to run game and handle actions of the plater in regards to movement,   *
 * shooting and hitting the computer controlled alien ships.           		  *
 *    																		  *
 *    																		  *
 ******************************************************************************/
void Task_Game_Host(void *pvParameters) {
	// busy wait for controller
	xSemaphoreTake(Sem_GAME_HOST, portMAX_DELAY);

	// initialize random seed
	srand(time(NULL));

	// generating random location along the x-axis within the screen
	int ALIEN_START_X = generate_random_in_range(0, SCREEN_WIDTH - ALIEN_WIDTH);

	// generating random location within top 40 pixels of the screen
	int ALIEN_START_Y = generate_random_in_range(0, 40);

	// Initialize first alien location and movement variables
	LOCATION alien_location = { ALIEN_START_X, ALIEN_START_Y, ALIEN_WIDTH,
			ALIEN_HEIGHT };
	int alien_direction = 1; // 1 for down, -1 for up
	int alien_speed = ALIEN_SPEED;
	int alien_image_index = 0;
	bool alien_image_up = true;
	TickType_t last_alien_image_change_time = xTaskGetTickCount();

	// Initialize list of bullets
	int MAX_BULLETS = 50;
	BULLET bullet_list[MAX_BULLETS];
	int num_bullets = 0;

	// initialize list of aliens
	struct ALIEN aliens[10];
	int num_aliens = 1;
	int hit_aliens = 0;

	// Initialize ship location and movement variables
	LOCATION ship_location = { .x = LCD_WIDTH / 2 - jet_fighterWidthPixels / 2,
			.y = LCD_HEIGHT - SHIP_HEIGHT, .height = SHIP_HEIGHT, .width =
					SHIP_WIDTH };

	// Wait for accelerometer queue to receive data
	ACCELEROMETER_DATA_t current;
	while (xQueueReceive(Queue_ACC_to_GAME, &current, portMAX_DELAY) != pdTRUE)
		;

	// Update ship location based on accelerometer input
	if (current.acc.left && ship_location.x > ship_x_min) {
		ship_location.x -= SHIP_SPEED;
	} else if (current.acc.right && ship_location.x < ship_x_max) {
		ship_location.x += SHIP_SPEED;
	}

	ship_location = boarder_range_validate(ship_location);

	// alien colors
	uint16_t aliens_fgColor = COLOR_CODE[WHITE];
	uint16_t aliens_bgColor = COLOR_CODE[BLACK];

	// detect previous collision
	bool pre_is_collide = false;

	// light sensor boolean
	bool is_dark = false;

	// creating a list for the bullets used
	BULLET *bullet_list = NULL;

	// bullet control
	// creating first bullet
	BULLET new_bullet = { .loc = { .x = ship_location.x, .y = ship_location.y,
			.height = BULLET_HEIGHT, .width = BULLET_WIDTH }, .dir =
			new_bullet_dir, .in_use = true };

	// allocate memory for bullet list and add first bullet
	bullet_list = malloc(sizeof(BULLET));
	*bullet_list = new_bullet;

	if (bullet_list == NULL) {
		bullet_list = malloc(sizeof(BULLET));
		*bullet_list = new_bullet;
	}

	else {
		int num_bullets = sizeof(bullet_list) / sizeof(BULLET);
		bullet_list = realloc(bullet_list, (num_bullets + 1) * sizeof(BULLET));
		bullet_list[num_bullets] = new_bullet;
	}

	for (int i = 0; i < sizeof(bullet_list) / sizeof(BULLET); i++) {
		if (bullet_list[i].in_use) {
			lcd_draw_image(bullet_list[i].loc.x, bullet_list[i].loc.y,
					bullet_list[i].loc.width, bullet_list[i].loc.height,
					bullet_bitmap, fgColor, bgColor);
		}

		// check if bullet is off screen
		if (bullet_list[i].loc.y < 0 || bullet_list[i].loc.y > SCREEN_HEIGHT) {
			bullet_list[i].in_use = false;
		}
	}

	// debounce state for S1
	uint8_t debounce_state = 0x00;

	// EDITED
	// changing ship location based on accelerometer
	while (1) {
		// change
		// variables that keep the game going
		bool game_over = false;
		int score = 0;  // initial score

		// check if 0.5 seconds have elapsed
		if ((double) (clock() - last_switch_time) / CLOCKS_PER_SEC >= 0.5) {
			// switch the image
			if (is_alien_up) {
				// load and display alien_down.png
				is_alien_up = false;
			} else {
				// load and display alien_up.png
				is_alien_up = true;
			}

			last_switch_time = clock();
		}

		xQueueReceive(Queue_ACC_to_GAME, &current, 0);

		// Control ship movement using accelerometer
		if (current.acc.left) {
			ship_location.x -= SHIP_SPEED;
		} else if (current.acc.right) {
			ship_location.x += SHIP_SPEED;
		}

		ship_location = boarder_range_validate(ship_location);

		// creating alien at top of the screen
		create_alien();

		// move the alien down by alien_speed pixels
		alien_location.y += alien_speed * alien_direction;

		// check if the alien has reached the next 40 pixels of the screen
		if (alien_location.y >= current_alien_y_pos) {
			// generate a new alien at the top 40 pixels of the screen
			create_new_alien();
		}

		// Collision detection or alien reaching bottom of screen and game over check
		xQueueReceive(Queue_Game_NPC_to_Host, &alien_location, 0);
		bool current_collide = is_collided(ship_location, alien_location);

		if (current_collide && !pre_is_collide) {
			bool win = false;
			xSemaphoreTake(Sem_PRINT, portMAX_DELAY);
			printf("YOU LOST!\n\r");
			xSemaphoreGive(Sem_PRINT);
			xQueueSendToBack(Queue_Game_Host_NPC_to_Controller, &win, 0);
		}

		// check if the alien has reached the bottom of the screen
		if (alien_location.y + ALIEN_HEIGHT >= LCD_HEIGHT) {
			// Alien ship is off the bottom of the screen, so game is lost
			bool win = false;
			xSemaphoreTake(Sem_PRINT, portMAX_DELAY);
			printf("YOU LOST!\n\r");
			xSemaphoreGive(Sem_PRINT);
			xQueueSendToBack(Queue_Game_Host_NPC_to_Controller, &win, 0);
			vTaskDelete(NULL); // End the task
		}

		pre_is_collide = current_collide;

		// render image
		xSemaphoreTake(Sem_RENDER, portMAX_DELAY);
		lcd_draw_image(ship_location.x, ship_location.y, ship_location.width,
				ship_location.height, jet_fighterBitmaps, fgColor, bgColor);
		xSemaphoreGive(Sem_RENDER);

		// adding new bullet
		// add new bullet
		BULLET new_bullet = { .loc =
				{ .x = ship_location.x, .y = ship_location.y, .height =
						BULLET_HEIGHT, .width = BULLET_WIDTH }, .dir =
				new_bullet_dir, .in_use = true };

		num_bullets++;
		bullet_list = realloc(bullet_list, num_bullets * sizeof(BULLET));
		bullet_list[num_bullets - 1] = new_bullet;

		for (int i = 0; i < num_bullets; i++) {
			if (bullet_list[i].in_use) {
				lcd_draw_image(bullet_list[i].loc.x, bullet_list[i].loc.y,
						bullet_list[i].loc.width, bullet_list[i].loc.height,
						bullet_bitmap, fgColor, bgColor);
			}

			// handling actions and reactions of bullets used
			for (int q = 0; q < MAX_BULLETS; q++) {
				if (bullet_list[q].in_use) {
					bool hit_alien = false;
					// check for collision with alien ships
					if (is_collided(alien_down_location, bullet_list[q].loc)
							|| is_collided(alien_up_location,
									bullet_list[q].loc)) {
						// play sound for hitting alien ship
						SOUND sound = NPC_KILLED;
						xQueueSendToBack(Queue_Song, &sound, 0);
						// send message to indicate a hit to the alien ship
						bool flash = 1;
						xQueueSendToBack(Queue_Game_Host_to_NPC, &flash, 0);
						hit_alien = true;
					}

					if (bullet_list[q].loc.y <= 0 || hit_alien) {
						// if the bullet has gone out of screen or hit an alien ship, remove it
						xSemaphoreTake(Sem_RENDER, portMAX_DELAY);
						lcd_draw_image(bullet_list[q].loc.x,
								bullet_list[q].loc.y, bullet_list[q].loc.width,
								bullet_list[q].loc.height, bulletBitmaps,
								COLOR_CODE[BLACK], COLOR_CODE[BLACK]);
						xSemaphoreGive(Sem_RENDER);
						bullet_list[q].in_use = false;
					} else {
						// render bullet
						xSemaphoreTake(Sem_RENDER, portMAX_DELAY);
						lcd_draw_image(bullet_list[q].loc.x,
								bullet_list[q].loc.y, bullet_list[q].loc.width,
								bullet_list[q].loc.height, bulletBitmaps,
								COLOR_CODE[WHITE], COLOR_CODE[BLACK]);
						xSemaphoreGive(Sem_RENDER);

						// move the bullet upwards
						bullet_list[q].loc.y--;
					}
				}
			}

			// Initialize a bullet
			debounce_state = debounce_state << 1;
			if (ece353_s1_pressed()) {
				debounce_state = debounce_state | BIT0;
			}
			if (debounce_state == 0x7F) {
				// ready to shoot
				// create a new bullet
				BULLET new_bullet;
				new_bullet.in_use = true;
				new_bullet.dir = current.joy;
				new_bullet.loc.x = ship_location.x;
				new_bullet.loc.y = ship_location.y;

				// send shooting sound effect
				SOUND sound = PLAYER_SHOOTING;
				xQueueSendToBack(Queue_Song, &sound, 0);

				// add the new bullet to the bullet list
				for (int q = 0; q < MAX_BULLETS; q++) {
					if (!bullet_list[q].in_use) {
						bullet_list[q] = new_bullet;
						break;
					}
				}
			}

			// remove bullets that have gone off screen
			for (int q = 0; q < MAX_BULLETS; q++) {
				if (bullet_list[q].in_use
						&& (bullet_list[q].loc.y < 0
								|| bullet_list[q].loc.y > SCREEN_HEIGHT)) {
					// remove bullet and update its location
					xSemaphoreTake(Sem_RENDER, portMAX_DELAY);
					lcd_draw_image(bullet_list[q].loc.x, bullet_list[q].loc.y,
							bullet_list[q].loc.width, bullet_list[q].loc.height,
							bulletBitmaps, COLOR_CODE[BLACK],
							COLOR_CODE[BLACK]);
					xSemaphoreGive(Sem_RENDER);
					bullet_list[q].in_use = false;
				}
			}

			// checking light sensor and changing color if needed
			    if (is_dark && (light_get_lux() > 10)) {
			        is_dark = false;
			        aliens_fgColor = COLOR_CODE[WHITE];
			        aliens_bgColor = COLOR_CODE[BLACK];
			    }

			    if (!is_dark && (light_get_lux() < 10)) {
			        is_dark = true;
			        aliens_fgColor = COLOR_CODE[BLACK];
			        aliens_bgColor = COLOR_CODE[WHITE];
			    }

			// delay task
			vTaskDelay(pdMS_TO_TICKS(5));
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
		 * Detect if pixels at two locations overlap
		 * @param loc1
		 * @param loc2
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
			if ((((xA_0 < xB_0) && (xB_0 < xA_1))
					|| ((xA_0 < xB_1) && (xB_1 < xA_1)))
					&& (((yA_0 < yB_0) && (yB_0 < yA_1))
							|| ((yA_0 < yB_1) && (yB_1 < yA_1)))) {
				return true;
			}
			return false;
		}

// Function to check if the alien ship is still on the screen
		bool is_alien_on_screen(int alien_x) {
			// Check if any part of the alien ship is off the screen
			if (alien_x < 0 || alien_x + ALIEN_WIDTH >= SCREEN_WIDTH) {
				return false; // Alien ship is off the screen
			}
			return true; // Alien ship is still on the screen
		}

		/**
		 * Generate a random int within the range (inclusive)
		 * @param lower
		 * @param upper
		 * @return random int
		 * CHECKING
		 */
		int generate_random_in_range(int lower, int upper) {
			int num = (rand() % (upper - lower + 1)) + lower;
			return num;
		}

// Function to create a new alien
	void create_alien(void) {
		// Generate random location along the x-axis within the screen
		int alien_start_x = generate_random_in_range(0,
				SCREEN_WIDTH - ALIEN_WIDTH);

		// Generate random location within top 40 pixels of the screen
		int alien_start_y = generate_random_in_range(0, 40);

		// Initialize alien location and movement variables
		Alien alien = { alien_start_x, alien_start_y, ALIEN_WIDTH, ALIEN_HEIGHT,
				1 };

		// Add alien to list of aliens
		int i;
		for (i = 0; i < 10; i++) {
			if (!aliens[i].is_alive) {
				aliens[i] = alien;
				break;
			}
		}
	}

//		// Function to create a new alien
//		void create_alien(void) {
//			// Generate random location along the x-axis within the screen
//			int alien_start_x = generate_random_in_range(0,
//					SCREEN_WIDTH - ALIEN_WIDTH);
//
//			// Generate random location within top 40 pixels of the screen
//			int alien_start_y = generate_random_in_range(0, 40);
//
//			// Initialize alien location and movement variables
//			LOCATION alien_location = { alien_start_x, alien_start_y,
//					ALIEN_WIDTH, ALIEN_HEIGHT };
//
//			// Add alien to list of aliens
//			int i;
//			for (i = 0; i < 10; i++) {
//				if (!aliens[i].is_alive) {
//					aliens[i].is_alive = true;
//					aliens[i].loc = alien_location;
//					aliens[i].direction = 1;
//					aliens[i].speed = ALIEN_SPEED;
//					aliens[i].image_index = 0;
//					aliens[i].image_up = true;
//					aliens[i].last_image_change_time = xTaskGetTickCount();
//					break;
//				}
//			}
//		}

	void move_alien(void) {
		// iterate over all alive aliens
		for (int i = 0; i < 10; i++) {
			if (aliens[i].alive) {
				// get a new random location within the next lower 40 pixels than previous location
				int new_y = aliens[i].loc.y - 40;
				int new_x = generate_random_in_range(0, 132);

				// move the alien to the new location
				aliens[i].loc.x = new_x;
				aliens[i].loc.y = new_y;

				// render the alien in its new location
				xSemaphoreTake(Sem_RENDER, portMAX_DELAY);
				lcd_draw_image(aliens[i].loc.x, aliens[i].loc.y, ALIEN_WIDTH,
						ALIEN_HEIGHT, alienBitmaps, fgColor, bgColor);
				xSemaphoreGive(Sem_RENDER);
			}
		}
	}

		// Function to move all alive aliens, and create new aliens when necessary
		void move_alien(void) {
			// iterate over all alive aliens
			for (int i = 0; i < 10; i++) {
				if (aliens[i].is_alive) {
					// get a new random location within the next lower 40 pixels than previous location
					int new_y = aliens[i].loc.y - 40;
					int new_x = generate_random_in_range(0, 132);

					// move the alien to the new location
					aliens[i].loc.x = new_x;
					aliens[i].loc.y = new_y;

					// check if the alien is off-screen
					if (!is_on_screen(aliens[i].loc)) {
						aliens[i].is_alive = false; // alien is dead

						// create a new alien
						create_alien();
					}

					// render the alien in its new location
					xSemaphoreTake(Sem_RENDER, portMAX_DELAY);
					lcd_draw_image(aliens[i].loc.x, aliens[i].loc.y,
							ALIEN_WIDTH, ALIEN_HEIGHT, alien_downBitmaps,
							fgColor, bgColor);
					xSemaphoreGive(Sem_RENDER);
				}
			}
		}

//// Function to draw all alive aliens
//	void draw_aliens(void) {
//		int i;
//		for (i = 0; i < 10; i++) {
//			if (aliens[i].is_alive) {
//				printf("Drawing alien at (%d, %d)\n", aliens[i].x, aliens[i].y);
//				// Draw alien at its current location
//				// render image
//				xSemaphoreTake(Sem_RENDER, portMAX_DELAY);
//				// CHANGE
//				lcd_draw_image(ship_location.x, ship_location.y,
//						ship_location.width, ship_location.height,
//						jet_fighterBitmaps, fgColor, bgColor);
//				xSemaphoreGive(Sem_RENDER);
//			}
//		}
//	}

		// Function to draw all alive aliens
		void draw_aliens(void) {
			int i;
			for (i = 0; i < 10; i++) {
				if (aliens[i].is_alive) {
					// Update alien image
					update_alien_image(&aliens[i]);

					// Draw alien at its current location
					xSemaphoreTake(Sem_RENDER, portMAX_DELAY);
					if (aliens[i].image_up) {
						lcd_draw_image(aliens[i].loc.x, aliens[i].loc.y,
								ALIEN_WIDTH, ALIEN_HEIGHT, alien_up, fgColor,
								bgColor);
					} else {
						lcd_draw_image(aliens[i].loc.x, aliens[i].loc.y,
								ALIEN_WIDTH, ALIEN_HEIGHT, alien_down, fgColor,
								bgColor);
					}
					xSemaphoreGive(Sem_RENDER);
				}
			}
		}

		// Function to update the alien image
		void update_alien_image(ALIEN *alien) {
			TickType_t current_time = xTaskGetTickCount();

			if ((current_time - alien->last_image_change_time) >= 500) { // 0.5 seconds
				alien->last_image_change_time = current_time;
				alien->image_up = !alien->image_up; // toggle image
			}
		}




// bullet handling

		// outside while loop
	    // create bullet list for tracking
	    int i;
	    BULLET bullet_list[25];
	    for (i = 0; i < 25; i++) {
	        LOCATION new_bullet_loc = { .x = 0, .y = 0, .height = BULLET_WIDTH,
	                                    .width = BULLET_HEIGHT };
	        ADC_MOVE new_bullet_dir = { .center = false, .left = false, .right = false,
	                                    .up = false, .down = false };
	        bullet_list[i].loc = new_bullet_loc;
	        bullet_list[i].dir = new_bullet_dir;
	        bullet_list[i].in_use = false;
	    }


// inside while loop
	// handing actions and reactions of firing bullet
	for (q = 0; q < 25; q++) {
	    if (bullet_list[q].in_use) {
	        // move bullet upwards
		// below value controls speed of bullet
	        bullet_list[q].loc.y -= 5;

	        if (!is_on_screen(bullet_list[q].loc)) {
	            // bullet is off the screen, so mark it as not in use
	            bullet_list[q].in_use = false;
	        }
	else {
	        bool hit_alien = false;
	            // check if the bullet has hit any aliens
	            for (int a = 0; a < NUM_ALIENS; a++) {
	                if (aliens[a].is_alive && is_collided(aliens[a].loc, bullet_list[q].loc)) {
	                    // bullet has hit an alien, so mark it as not in use, update the alien's status, and send a message to the NPC task
	                    bullet_list[q].in_use = false;
	                    aliens[a].is_alive = false;
	                    hit_alien = true;
	                    bool flash = true;
	                    xQueueSendToBack(Queue_Game_Host_to_NPC, &flash, 0);
	                    break; // exit the for-loop early since we only want to update one alien per bullet
	                }
	            }
	        else {
	            // render the bullet
	            xSemaphoreTake(Sem_RENDER, portMAX_DELAY);
	            lcd_draw_image(
	                bullet_list[q].loc.x,
	                bullet_list[q].loc.y,
	                bullet_list[q].loc.width,
	                bullet_list[q].loc.height,
	                bulletBitmaps,
	                COLOR_CODE[WHITE],
	                COLOR_CODE[BLACK]
	            );
	            xSemaphoreGive(Sem_RENDER);
	        }
	    }
	}



	    // task code for aliens
	    // Create the first alien
	        create_alien();

	        while (1) {
	            // Move all aliens down by 50 pixels every 1 second
	            vTaskDelay(pdMS_TO_TICKS(1000));
	            move_alien();

	            // Draw all alive aliens
	            draw_aliens();
	        }

// alien creating and moving task
	        void Task_Aliens(void *pvParameters) {
	            // initialize list of aliens
	            struct ALIEN aliens[10];
	            // int num_aliens = 1;
	            // int hit_aliens = 0;

	            // alien colors
	            uint16_t aliens_fgColor = COLOR_CODE[WHITE];
	            uint16_t aliens_bgColor = COLOR_CODE[BLACK];

	            // create a new alien
	            createAlien();

//	            // initialize random seed
//	            srand(xTaskGetTickCount());

	            while (1) {
	                // move and render aliens
	                move_alien();
	                draw_aliens();

	                // delay for 1 second
	                vTaskDelay(pdMS_TO_TICKS(1000));
	            }
	        }
		
		
		
		
		/**
		while (true) {
        // create a new alien
        create_alien();

        // move and render aliens
        move_aliens();
        draw_aliens();

        // delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    */



