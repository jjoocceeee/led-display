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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void transmit(char v){
	while( !SPI1->SR >> 1 ){ //Checking transmit buffer empty flag. If 0, then it is not empty
	}
	SPI1->DR = v;
}
void transmitWord(char* word){
	int counter = 0;
	char temp = word[counter];
	
	while( temp != '\0'){
		transmit(temp);
		temp = word[++counter];
	}
}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  HAL_Init();

  SystemClock_Config();
	
	RCC->AHBENR |= 1<<19 | 1<<18 | 1<<17;	 //Enable clock IOPC & IOPB & IOPA
	RCC->APB2ENR |= 1<<12; //Enable clock for SPI1
	
	//Setting up LEDs for testing + error checking.
	GPIOC->MODER |= 0x55000;
	GPIOC->OTYPER = 0x000;
	GPIOC->OSPEEDR = 0x0;
	GPIOC->PUPDR = 0x0;
	
	//Baud Rate Do we need to configure this? Max is 10MHz, which is the clockrate of the Discovery Board
	//SPI1->BR = HAL_RCC_GetHCLKFreq()/115200;
	SPI1->CR1 |= 0x7<<5; //Baud Rate f/256.
	
	//CRC length to 16 bits TODO
	SPI1->CR1 |= 1<<11; 
	
	//Set pins PB3-PB5 to alternative mode. Because we aren't doing an SPI receive, we aren't using the MISO
	GPIOB->MODER |= 0x980;
	GPIOA->MODER |= 2<<30; //NSS (Slave Selector) is PA15
	
	//TODO: do we need to do anything more to setup the altnerative functions?
	
	//select Simple Mode through BIDIMONE and BIDIOE
	SPI1->CR1 |= 1<<15;		//TODO I don't think we need this. BIIMOE (bidirectional data mode enabled)
	SPI1->CR1 |= 1<<14;		//Output Enabled a.k.a transmit-only mode
	SPI1->CR1 |= 1<<2;		//Master Selection
	//DMD expects 16-bit data
	SPI1->CR2 |= 0xf << 8;	//Bits [8:11] = 1111


	SPI1->CR1 |= 1<<6; //SPI Enable
	GPIOC->ODR |= (1<<9);
	while(1){
		GPIOC->ODR &= ~(1<<6);	//Sanity check. Turning LED
		GPIOC->ODR &= ~(1<<7);	//Sanity check. Turning LED
		GPIOC->ODR &= ~(1<<8);	//Sanity check. Turning LED
		HAL_Delay(1000);
		
		
		GPIOC->ODR ^= (1<<9);
		while (!SPI1->SR >> 7) //busy flag is on do nothing.
		{
			GPIOC->ODR |= (1<<6);	//Sanity check. Turning on a color
		}
		if(SPI1->SR >> 8){
			GPIOC->ODR |= (1<<8);		//Frame error.
		}
		if(SPI1->SR >> 3){
			GPIOC->ODR |= (1<<7);		//Underrun error.
		}
		
		
		char message[] = "Hi";
		//while there is still data to send, and the transfer buffer has room, send the next letter.
		transmitWord(message);
	}
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
