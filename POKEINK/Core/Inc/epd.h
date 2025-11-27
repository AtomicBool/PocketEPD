/*
 * epd.h
 *
 *  Created on: Nov 25, 2025
 *      Author: atomicbool
 */

#ifndef INC_EPD_H_
#define INC_EPD_H_

#include "main.h"
#include "epd_hal.h"

#define NUMBER_ROW 296
#define NUMBER_COL 128

#define BYTE_COL (NUMBER_COL / 8)
#define GRAM_SIZE (NUMBER_ROW * BYTE_COL)

#define PI 3.1415926535

typedef enum
{
      WHITE = 0,// white
	  BLACK,   	// black
	  RED		// red
} COLOR;

void EPD_clear(void);
void EPD_update(void);

void EPD_drawPixel(int x, int y, COLOR clr);
void EPD_drawByte(int x, int y, uint8_t mask, COLOR color);

void EPD_drawLine(int x0, int y0, int x1, int y1, COLOR color);
void EPD_drawPie(int center_x, int center_y, int r_out, int r_in, uint8_t percentage, COLOR color);

void EPD_drawChar(int x, int y, char c, uint8_t* font, COLOR color);
void EPD_drawStr(int x, int y, char* str, uint8_t* font, COLOR color);

#endif /* INC_EPD_H_ */
