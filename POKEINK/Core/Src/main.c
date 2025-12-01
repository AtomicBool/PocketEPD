/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "epd.h"
#include "font_8x16.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t usart_cmd;
uint8_t usart_data[20];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_EPD_Init();

  /*
		  EPD_clear();
		  EPD_drawStr(0, 0, "2025/11/27", font_8x16, RED);
		  EPD_drawStr(0, 16, "EPD Driver", font_8x16, BLACK);

		  EPD_drawPie(50, 100, 30, 20, 90, BLACK);
		  EPD_drawPie(50, 100, 30, 20, 40, RED);

		  EPD_update();
   */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  uint8_t ACK = 0x67;

  while (1)
  {
	  HAL_Delay(1);
	  HAL_UART_Receive(&huart1, &usart_cmd, 1, HAL_MAX_DELAY);
	  switch(usart_cmd){
	  	  int y0, y1, x0, x1;
	  	  case 0x00:
	  		  EPD_clear();
	  		  HAL_UART_Transmit(&huart1, &ACK, 1, HAL_MAX_DELAY);
	  		  break;
	  	  case 0x01:
	  		  EPD_update();
	  		  HAL_UART_Transmit(&huart1, &ACK, 1, HAL_MAX_DELAY);
	  		  break;
	  	  case 0x02:	// XX XX YY YY CLOR
	  		  HAL_UART_Receive(&huart1, usart_data, 5, HAL_MAX_DELAY);

	  		  x0 = usart_data[1] | (usart_data[0] << 8);
	  		  y0 = usart_data[3] | (usart_data[2] << 8);

	  		  EPD_drawPixel(
	  				  x0,
					  y0,
					  usart_data[4]		// color
	  		  );

	  		  HAL_UART_Transmit(&huart1, &ACK, 1, HAL_MAX_DELAY);
	  		  break;
	  	  case 0x03:	// XX XX YY YY MASK CLOR
	  		  HAL_UART_Receive(&huart1, usart_data, 6, HAL_MAX_DELAY);

	  		  x0 = usart_data[1] | (usart_data[0] << 8);
	  		  y0 = usart_data[3] | (usart_data[2] << 8);

	  		  EPD_drawByte(
	  				  x0,
					  y0,
					  usart_data[4],	// bit mask
					  usart_data[5]		// color
	  		  );

	  		  HAL_UART_Transmit(&huart1, &ACK, 1, HAL_MAX_DELAY);
	  		  break;
	  	  case 0x04:	// XX XX YY YY CHAR CLOR
	  		  HAL_UART_Receive(&huart1, usart_data, 6, HAL_MAX_DELAY);

	  		  x0 = usart_data[1] | (usart_data[0] << 8);
	  		  y0 = usart_data[3] | (usart_data[2] << 8);

	  		  EPD_drawChar(
	  				  x0,
					  y0,
					  usart_data[4],	// char
					  font_8x16,
					  usart_data[5]		// color
	  		  );

	  		  HAL_UART_Transmit(&huart1, &ACK, 1, HAL_MAX_DELAY);
	  		  break;
	  	  case 0x05:	// XX XX YY YY RO RI PT CLOR
	  		  HAL_UART_Receive(&huart1, usart_data, 8, HAL_MAX_DELAY);

	  		  x0 = usart_data[1] | (usart_data[0] << 8);
	  		  y0 = usart_data[3] | (usart_data[2] << 8);

	  		  EPD_drawPie(
	  				  x0,
					  y0,
					  usart_data[4],	// outer radius
					  usart_data[5],	// inner radius
					  usart_data[6],	// percentage
					  usart_data[7]		// color
	  		  );

	  		  HAL_UART_Transmit(&huart1, &ACK, 1, HAL_MAX_DELAY);
	  		  break;
	  	  case 0x06:	// XX XX YY YY XX XX YY YY CLOR
	  		  HAL_UART_Receive(&huart1, usart_data, 9, HAL_MAX_DELAY);

	  		  x0 = usart_data[1] | (usart_data[0] << 8);
	  		  y0 = usart_data[3] | (usart_data[2] << 8);
	  		  x1 = usart_data[5] | (usart_data[4] << 8);
	  		  y1 = usart_data[7] | (usart_data[6] << 8);

	  		  EPD_drawLine(
	  				  x0,
					  y0,
					  x1,
					  y1,
					  usart_data[8]		// color
	  		  );

	  		  HAL_UART_Transmit(&huart1, &ACK, 1, HAL_MAX_DELAY);
	  		  break;
	  	  case 0x07:	// XX XX YY YY XX XX YY YY CLOR
	  		  HAL_UART_Receive(&huart1, usart_data, 9, HAL_MAX_DELAY);

	  		  x0 = usart_data[1] | (usart_data[0] << 8);
	  		  y0 = usart_data[3] | (usart_data[2] << 8);
	  		  x1 = usart_data[5] | (usart_data[4] << 8);
	  		  y1 = usart_data[7] | (usart_data[6] << 8);

	  		  EPD_drawRectFilled(
	  				  x0,
					  y0,
					  x1,
					  y1,
					  usart_data[8]		// color
	  		  );

	  		  HAL_UART_Transmit(&huart1, &ACK, 1, HAL_MAX_DELAY);
	  		  break;
	  	  case 0x08: // XX XX YY YY CLOR CHAR*16
	  		  HAL_UART_Receive(&huart1, usart_data, 21, HAL_MAX_DELAY);

	  		  x0 = usart_data[1] | (usart_data[0] << 8);
	  		  y0 = usart_data[3] | (usart_data[2] << 8);

	  		  char str[17] = {0};
	  		  for(int i = 5; i < 21; i++){
	  			  str[i - 5] = usart_data[i];
	  			  if(usart_data[i] == 0x00) break;
	  		  }

	  		  EPD_drawStr(
	  				  x0,
					  y0,
					  str,
					  font_8x16,
					  usart_data[4]	// color
	  		  );

	  		  HAL_UART_Transmit(&huart1, &ACK, 1, HAL_MAX_DELAY);
	  		  break;
	  }
	  /* USER CODE END WHILE */

	  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
