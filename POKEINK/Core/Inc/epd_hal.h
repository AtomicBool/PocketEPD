/*
 * epd.h
 *
 *  Created on: Nov 19, 2025
 *      Author: atomicbool
 */

#ifndef INC_EPD_HAL_H_
#define INC_EPD_HAL_H_

#include "spi.h"

void HAL_EPD_CMD(uint8_t cmd);
void HAL_EPD_DATA(uint8_t data);
void HAL_EPD_DATA_ARRAY(const uint8_t* data, size_t len);

void HAL_EPD_WAIT();
void HAL_EPD_Init(void);
void HAL_EPD_update(const uint8_t* GRAM_BLK, const uint8_t* GRAM_RED, size_t bufferSize);
void HAL_EPD_COUNTER_RST(void);

#endif /* INC_EPD_HAL_H_ */
