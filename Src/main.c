/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


void SystemClock_Config(void);
static void MX_GPIO_Init(void);


uint8_t disp1ay[38][8]={
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0
{0x10,0x30,0x50,0x10,0x10,0x10,0x10,0x7c},//1
{0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E},//2
{0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0},//3
{0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8},//4
{0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0},//5
{0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0},//6
{0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8},//7
{0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8
{0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E},//9
{0x18,0x24,0x42,0x42,0x7E,0x42,0x42,0x42},//A
{0x3C,0x22,0x22,0x3c,0x22,0x22,0x3C,0x0},//B
{0x3C,0x40,0x40,0x40,0x40,0x40,0x40,0x3C},//C
{0x7C,0x22,0x22,0x22,0x22,0x22,0x22,0x7C},//D
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F
{0x3C,0x40,0x40,0x40,0x4c,0x44,0x44,0x3C},//G
{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H
{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I
{0x3C,0x8,0x8,0x8,0x8,0x8,0x48,0x30},//J
{0x0,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K
{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L
{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M
{0x0,0x42,0x62,0x52,0x4A,0x46,0x42,0x0},//N
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O
{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P
{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q
{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R
{0x0,0x1E,0x20,0x20,0x3E,0x2,0x2,0x3C},//S
{0x0,0x3E,0x8,0x8,0x8,0x8,0x8,0x8},//T
{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U
{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V
{0x0,0x49,0x49,0x49,0x49,0x2A,0x1C,0x0},//W
{0x0,0x41,0x22,0x14,0x8,0x14,0x22,0x41},//X
{0x41,0x22,0x14,0x8,0x8,0x8,0x8,0x8},//Y
{0x0,0x7F,0x2,0x4,0x8,0x10,0x20,0x7F},//Z
};

void write_byte (uint8_t byte)
{
	for (int i =0; i<8; i++)
	{
		HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, 0);  // pull the clock pin low
		HAL_GPIO_WritePin (GPIOA, GPIO_PIN_7, byte&0x80);  // write the MSB bit to the data pin
		byte = byte<<1;  // shift left
		HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, 1);  // pull the clock pin HIGH
	}
}

void write_max (uint8_t address, uint8_t data)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_6, 0);  // pull the CS pin LOW
	write_byte (address);
	write_byte (data); 
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_6, 1);  // pull the CS pin HIGH
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
}



// function for init

void max_init(void)
{
 write_max(0x09, 0x00);       //  no decoding
 write_max(0x0a, 0x03);       //  brightness intensity
 write_max(0x0b, 0x07);       //  scan limit = 8 LEDs
 write_max(0x0c, 0x01);       //  power down =0,normal mode = 1
 write_max(0x0f, 0x00);       //  no test display
}

void write_string (char *str)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	while (*str)
	{
		for(int i=1;i<9;i++)
			   {
			       write_max (i,disp1ay[(*str - 55)][i-1]);
			   }
		*str++;
		HAL_Delay (500);
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

 HAL_Init();

  SystemClock_Config();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	MX_GPIO_Init();
	max_init ();
	GPIO_InitTypeDef initStr = {GPIO_PIN_9,GPIO_MODE_OUTPUT_PP,GPIO_SPEED_FREQ_LOW, GPIO_NOPULL};
	HAL_GPIO_Init(GPIOC, &initStr);
	GPIO_InitTypeDef initStr3 = {GPIO_PIN_6| GPIO_PIN_7,GPIO_MODE_OUTPUT_PP,GPIO_SPEED_FREQ_LOW, GPIO_NOPULL};
	HAL_GPIO_Init(GPIOC, &initStr3);
	
	//Setting up LEDs for testing + error checking.
	GPIO_InitTypeDef initStr2 = {GPIO_PIN_8, GPIO_MODE_INPUT,GPIO_SPEED_FREQ_LOW, GPIO_NOPULL};
	HAL_GPIO_Init(GPIOC, &initStr2);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
	
	
	while(1){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
		write_string ("HI");
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_Delay (1000);
	}
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
	static void MX_GPIO_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitTypeDef GPIO_InitStruct2 = {0};

		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOD_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(GPIOA, clock_Pin|cs_Pin|data_Pin, GPIO_PIN_RESET);

		/*Configure GPIO pins : clock_Pin cs_Pin data_Pin */
		GPIO_InitStruct2.Pin = data_Pin;
		GPIO_InitStruct2.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct2.Pull = GPIO_PULLUP;
		GPIO_InitStruct2.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct2);
		
		GPIO_InitStruct.Pin = clock_Pin|cs_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIOB->AFR[0] = 0;

	}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

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

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
