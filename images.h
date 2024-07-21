/*
 * images.h
 *
 *  Created on: Apr 30, 2023
 *      Authors: Shawn Zhu, Shreya Mukherjee
 */

#include "msp.h"

#ifndef IMAGES_H_
#define IMAGES_H_

#define ALIEN_WIDTH       24        //same height and width for both
#define ALIEN_HEIGHT      16
extern const uint8_t SIDownBitmaps[];
extern const uint8_t SIUpBitmaps[];

#define SHIP_WIDTH          24
#define SHIP_HEIGHT         14
extern const uint8_t shipBitmaps[];

#define BULLET_WIDTH        7
#define BULLET_HEIGHT       7
extern const uint8_t bulletBitmaps[];

#endif /* IMAGES_H_ */
