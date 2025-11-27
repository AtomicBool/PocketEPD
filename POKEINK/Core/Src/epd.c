/*
 * epd.c
 *
 *  Created on: Nov 25, 2025
 *      Author: atomicbool
 */


#include <epd.h>

uint8_t pBlack[GRAM_SIZE];
uint8_t pRed[GRAM_SIZE];

void EPD_clear(){
	memset(pBlack, 0xFF, GRAM_SIZE);
	memset(pRed, 0x00, GRAM_SIZE);
}

void EPD_update(){
	HAL_EPD_update(pBlack, pRed, GRAM_SIZE);
}

void EPD_drawPixel(int x, int y, COLOR color){
	if(y > (NUMBER_ROW - 1) || x > (NUMBER_COL - 1)) return;
	// GRAM Black: 0 -> black, 1 -> white
	// GRAM Red:   0 -> others, 1 -> red
	uint16_t byteOffset = y * BYTE_COL + (uint16_t) (x / 8);
	uint8_t bitOffset = 1 << (7 - (x % 8));
	switch(color){
		case WHITE:
			pBlack[byteOffset] |= bitOffset;
			pRed[byteOffset] &= ~bitOffset;
			break;
		case BLACK:
			pBlack[byteOffset] &= ~bitOffset;
			pRed[byteOffset] &= ~bitOffset;
			break;
		default:
			pRed[byteOffset] |= bitOffset;
	}
}

void EPD_drawByte(int x, int y, uint8_t mask, COLOR color){
	if(y > (NUMBER_ROW - 1) || (x + 7) > NUMBER_COL) return;

	uint16_t pageIndex = y * BYTE_COL + (uint16_t) (x / 8);
	uint8_t bitOffset = x % 8;

	bool crossPage = bitOffset ? true : false;
	uint8_t firstMask = (mask >> bitOffset);
	uint8_t secondMask = (mask << (8 - bitOffset)) & 0xFF;

	switch(color){
		case WHITE:
			pBlack[pageIndex] |= firstMask;
			pRed[pageIndex] &= ~firstMask;
			if(crossPage){
				pBlack[pageIndex + 1] |= secondMask;
				pRed[pageIndex + 1] &= ~secondMask;
			}
			break;
		case BLACK:
			pBlack[pageIndex] &= ~firstMask;
			pRed[pageIndex] &= ~firstMask;
			if(crossPage){
				pBlack[pageIndex + 1] &= ~secondMask;
				pRed[pageIndex + 1] &= ~secondMask;
			}
			break;
		default:
			pRed[pageIndex] |= firstMask;
			if(crossPage) pRed[pageIndex + 1] |= secondMask;
	}
}

void EPD_drawLine(int x0, int y0, int x1, int y1, COLOR color) {
    if (x0 == x1 && y0 == y1) {
    	EPD_drawPixel(x0, y0, color);
        return;
    }
    int dx = (x1 > x0 ? x1 - x0 : x0 - x1);
    int dy = (y1 > y0 ? y0 - y1 : y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (1) {
    	EPD_drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void EPD_drawPie(int center_x, int center_y, int r_out, int r_in, uint8_t percentage, COLOR color)
{
	int x0, y0, x1, y1, dx, dy;
	double angleRequired, angle, dist;

	// Clamping
	if (percentage > 100) percentage = 100;

	x0 = center_x - r_out;
	x1 = center_x + r_out;
	y0 = center_y - r_out;
	y1 = center_y + r_out;

	// angle in radians
	angleRequired = (percentage / 100.0) * 2.0 * PI;

	// iterate all points
	for (int y = y0; y <= y1; y++)
	{
		for (int x = x0; x <= x1; x++)
		{
			//计算该点距离圆心的距离
			dx = x - center_x;
			dy = -(y - center_y);
			dist = sqrt(dx * dx + dy * dy);

			if((dist >= r_in) && (dist <= r_out)){
				if(dist == 0){
					EPD_drawPixel(x, y, color);
				}else{
					angle = atan(fabs(dx) / fabs(dy)); // 1st Quadrant

					if ((dx >= 0) && (dy <= 0)) angle = PI - angle; // 2nd Quadrant
					else if ((dx <= 0) && (dy <= 0)) angle = angle + PI; // 3rd Quadrant
					else if ((dx <= 0) && (dy >= 0)) angle = 2 * PI - angle; // 4th Quadrant

					if (angle <= angleRequired) EPD_drawPixel(x, y, color);
				}
			}
		}
	}
}

void EPD_drawChar(int x, int y, char c, uint8_t* font, COLOR color){
	// 8x16 only
	if((y + 15) > NUMBER_ROW) return;
	uint16_t index = 16 * (c - 0x20);
	for(int i = 0; i < 16; i++){
		EPD_drawByte(x, y + i, font[index + i], color);
	}
}

void EPD_drawStr(int x, int y, char* str, uint8_t* font, COLOR color){
	int currentX = x;

	// Iterate through each character in the string
	while(*str != '\0') {
		// Draw current character
	    EPD_drawChar(currentX, y, *str, font, color);

	    // Move to the next character
	    currentX += 8;
	    str++;
	}
}


