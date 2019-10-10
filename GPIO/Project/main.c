#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "debug.h"

GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef UART_InitStruct;


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
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
			

			SysTick_Config(1000/1);

			while (1)
			{
				/* Toggle LED on PA0 */
				GPIOA->ODR = (uint32_t)0x00;
        		delay(150);
        		GPIOA->ODR = (uint32_t)(1<<0);
				delay(150);
			}
}

