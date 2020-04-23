/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"

void SystemClock_Config(void);
void Error_Handler(void);
void get_message(void);



void transmit(char v){
	GPIOC->ODR |= (1<<8);
	while(!((USART3->ISR >> 7) & 1)){ //Checking transmit bit which is 7th bit
		
	}
	USART3->TDR = v;
	GPIOC->ODR &= ~(1<<8);
}

void transmitString(char* word){
	int counter = 0;
	char temp = word[counter];
	
	while( temp != '\0'){
		transmit(temp);
		temp = word[++counter];
	}
}


/*
Sets up USART communication with the following ports enabled:
PC10: USART - TX
PC11: USART - RX
PC12: USART - CLK
BAUD RATE: 115200

1. Will send "Message?" across USART when turned on. 
2. Will wait for message to come through. A message is less than 10 characers long, and is followed by a "!"
3. When the Blue Button is pressed, steps 1 & 2 will occur again.
*/
volatile int new_message;
int main(void)
{
	
	HAL_Init();

  SystemClock_Config();

	RCC->AHBENR |= 1<<19;	//Enable clock
	RCC->APB1ENR |= 1<<18; //Enable clock for USART3
	RCC->AHBENR |= 1<<17;	//Enable peripheral clock for push buttons
	
	//Setting the Baud Rate to 115200
	USART3->BRR = HAL_RCC_GetHCLKFreq()/115200;
	

	//Setting up pins GPIO_C10 GPIO_C11 to alternative function mode
	GPIOC->MODER |= 0xa00000;
	
	//Setup Alternative Function
	GPIOC->AFR[1] |= (1<<8) | (1<<12);
	
	//Setting up LEDs //TODO: Kylee you will need to change this to enable them as analog inputs
	GPIOC->MODER |= 0x55000;
	GPIOC->OTYPER = 0x000;
	GPIOC->OSPEEDR = 0x0;
	GPIOC->PUPDR = 0x0;
	
	//Setting up Blue Button
	GPIOA->MODER &= ~3;
	GPIOA->OSPEEDR &= ~3;	//Low speed
	GPIOA->PUPDR |= 0x2; //Pull down resistor
	
	//Setting up Push Button Interrupt
	SYSCFG->EXTICR[1] |= 0x0;
	EXTI->IMR |= 0x1;
	EXTI->EMR |= 0x1;
	EXTI->RTSR |= 0x1;
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_SetPriority(EXTI0_1_IRQn, 3);
	
	
	USART3->CR1 |= 0xc;	//Enable Receiver + Transmitter
	//TODO: Trying to MUTE USART when not waiting for a message. This will keep data from being sent unwanted to USART
	//USART3->CR1 |= 5;	//Enable Receiver Interrupt
	//USART3->CR1 |= 1<<13; //Mute Mode Enable. Can switch between Mute and Wake modes.
	USART3->CR1 |= 0x1;	//Enable USART (Done in a second step because once this is enabled, many pins become read only)
	
	//Setting up interupt TODO: 
	// Optional - Interrupt which occurs when input is recieve. Will probably enable this at one point, but not right now.
	//NVIC_EnableIRQ(USART3_4_IRQn);
	//NVIC_SetPriority(USART3_4_IRQn, 3);
	
	
	GPIOC->ODR |= 1<<9;
	volatile char number;
	transmitString("Message?\n\r");
	new_message=1;
  while (1)
  {
		GPIOC->ODR &= ~(1<<6);
		GPIOC->ODR &= ~(1<<7);
		GPIOC->ODR &= ~(1<<8);
		GPIOC->ODR &= ~(1<<9);
		if(new_message){
			USART3->RQR |= 1<<3;			//Flush out Receive data
			get_message();
		}
	}
}

void get_message(void){
		USART3->RQR |= ~(1<<2);		//Reciever in active mode
		GPIOC->ODR |= (1<<9);
		while(!((USART3->ISR >> 5) & 1)){
			//hang until the number is recieved
		}
		GPIOC->ODR &= ~(1<<9);
		volatile char letter;
		letter = USART3->RDR; //The value received can be directly used as a char.
		char message[13] = "";
		int counter = 0;
		while( letter != '!'){
			GPIOC->ODR |= (1<<6);
			while(!((USART3->ISR >> 5) & 1)){
				//hang until the number is recieved
			}
			GPIOC->ODR &= ~(1<<6);
			if(counter >= 10){
				transmitString("Too long\n\r");
				return;
			}
				message[counter]=letter;	
				letter = USART3->RDR;
				counter++;
		}
		message[counter++] = '\n';
		message[counter++] = '\r';
		message[counter] = '\0';
		transmitString(message);
		new_message = 0;
		USART3->RQR |= 1<<2;		//Reciever in mute mode
}


void EXTI0_1_IRQHandler(void){
	transmitString("Message?\n\r");
	new_message = 1;
	EXTI->PR |= 0x1; //Mark the Interrupt as handled.
}






/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
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

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
