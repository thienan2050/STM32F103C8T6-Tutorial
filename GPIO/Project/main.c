#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "debug.h"

GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef UART_InitStruct;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

int main (void)
{
/**
 * Configure USART2
 * GPIO_Pin_2 TX
 * GPIO_Pin_3 RX
 * BaudRate = 115200
 */
			/* Enable clock for USART2 */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			/* Enable clock for GPIOA */
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			/* Configure pin TX */
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
			/* Configure pin RX */
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
			/* Configure BaudRate = 115200 */
			UART_InitStruct.USART_BaudRate = 115200;
			UART_InitStruct.USART_WordLength = USART_WordLength_8b;
			UART_InitStruct.USART_StopBits = USART_StopBits_1;
			UART_InitStruct.USART_Parity = USART_Parity_No;
			UART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
			UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_Init(USART2, &UART_InitStruct);
			/* Enable USART2 Peripheral*/
			USART_Cmd(USART2, ENABLE);
/**
 * Configure GPIOC
 * Pin13 as output
 */
			/* Enable clock for GPIOC */
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			/* Configure P13 as push-pull output*/
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOC, &GPIO_InitStruct);

			DBG("GPIO_InitTypeDef GPIO_InitStruc is stored in SRAM %p \n\r",&GPIO_InitStruct);
			DBG("The address of GPIOC is %p \n\r",GPIOC);
/**
* Configure GPIOA
* Pin0 as input
* Pin1 as output
*/
			/* Configure P0 as push-pull output */
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
			/* Configure P1 as input */
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
			
/**
 * Configure Interrupt on PA0
 *
*/
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
			EXTI_ClearITPendingBit(EXTI_Line5);
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Line = EXTI_Line0;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);

			/*NVIC Configuration*/
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			while (1)
			{
				delay(200);
				if((GPIOA->IDR & (1<<4)) != 0)
				{
					printf("Button is pressed\n\r");
				}
			}
}

