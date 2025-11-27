/*
 * epd.c
 *
 *  Created on: Nov 19, 2025
 *      Author: atomicbool
 */

#include <epd_hal.h>

void HAL_EPD_WAIT(){
	HAL_GPIO_WritePin(PIN_LED_GPIO_Port, PIN_LED_Pin, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(BUSY_GPIO_Port, BUSY_Pin) != GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PIN_LED_GPIO_Port, PIN_LED_Pin, GPIO_PIN_RESET);
}

void HAL_EPD_Init(){
	HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_EPD_WAIT();
	HAL_EPD_CMD(0x12); // software reset
	HAL_EPD_WAIT();

	HAL_EPD_CMD(0x01); 	// Display Settings
	HAL_EPD_DATA(0x27); // MUX127
	HAL_EPD_DATA(0x01);
	HAL_EPD_DATA(0x00);	// GD,SM,TB = 0

	HAL_EPD_CMD(0x11); 	// Scan Mode
	HAL_EPD_DATA(0x03); // Z shape, up to down

	HAL_EPD_CMD(0x44); 	// range of X
	HAL_EPD_DATA(0x00);
	HAL_EPD_DATA(0x0F);

	HAL_EPD_CMD(0x45); 	// range of Y
	HAL_EPD_DATA(0x00);
	HAL_EPD_DATA(0x00);
	HAL_EPD_DATA(0x27); // 0x127
	HAL_EPD_DATA(0x01);

	HAL_EPD_CMD(0x3C); 	// border
	HAL_EPD_DATA(0x01);

	//EPD_CMD(0x21); 	// Display update control
	//EPD_DATA(0x00);
	//EPD_DATA(0x80);

	HAL_EPD_WAIT();
}

void HAL_EPD_update(const uint8_t* GRAM_BLK, const uint8_t* GRAM_RED, size_t bufferSize){
	HAL_EPD_COUNTER_RST();
	HAL_EPD_CMD(0x24);
	HAL_EPD_DATA_ARRAY(GRAM_BLK, bufferSize);

	HAL_EPD_COUNTER_RST();
	HAL_EPD_CMD(0x26);
	HAL_EPD_DATA_ARRAY(GRAM_RED, bufferSize);

	HAL_EPD_CMD(0x20);
	HAL_EPD_WAIT();
}

void HAL_EPD_COUNTER_RST(){
	HAL_EPD_CMD(0x4E); 	// X counter
	HAL_EPD_DATA(0x00);
	HAL_EPD_CMD(0x4F);	// Y counter
	HAL_EPD_DATA(0x00);
	HAL_EPD_DATA(0x00);
}

void HAL_EPD_CMD(uint8_t cmd){
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DCX_GPIO_Port, DCX_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &cmd, sizeof(cmd), HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void HAL_EPD_DATA(uint8_t data){
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DCX_GPIO_Port, DCX_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, &data, sizeof(data), HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void HAL_EPD_DATA_ARRAY(const uint8_t* data, size_t len){
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DCX_GPIO_Port, DCX_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)data, len, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}
