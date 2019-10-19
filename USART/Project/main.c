#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "core_cm3.h"
#include "FIFO.h"
#include "debug.h"

GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef UART_InitStruct;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
__IO uint32_t e_Systick_ui32 = 0;
fifo_t e_BUFFER_ui8;
uint32_t j;

void *g_ui32Test;


uint8_t get_buffer(uint8_t arr[]);
void Delay_ms(uint32_t ms);
uint8_t array[4];
int main (void)
{
/**
 * Configure USART2 for debugging
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

			//DBG("GPIO_InitTypeDef GPIO_InitStruc is stored in SRAM %p \n\r",&GPIO_InitStruct);
			//DBG("The address of GPIOC is %p \n\r",GPIOC);
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


/**
 * Configure Interrupt on PA5
 *
*/
			/* Configure P1 as input */
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
			/* Mapping */
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);


			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Line = EXTI_Line5;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);

			/*NVIC Configuration*/
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

/**
* Configure Interrupt on PA4
*
*/
			/* Configure P4 as input */
			//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
			//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
			//GPIO_Init(GPIOA, &GPIO_InitStruct);
			/* Mapping */
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);


			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Line = EXTI_Line4;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);

			/*NVIC Configuration*/
			NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			SysTick_Config(SystemCoreClock / 1000);

/**
* Configure USART3 for debugging
* GPIO_Pin_11 TX
* GPIO_Pin_10 RX
* BaudRate = 115200
*/
			/* Enable clock for USART3 */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
			/* Enable clock for GPIOA */
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			/* Configure pin TX */
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOB, &GPIO_InitStruct);
			/* Configure pin RX */
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOB, &GPIO_InitStruct);
			/* Configure BaudRate = 115200 */
			UART_InitStruct.USART_BaudRate = 115200;
			UART_InitStruct.USART_WordLength = USART_WordLength_8b;
			UART_InitStruct.USART_StopBits = USART_StopBits_1;
			UART_InitStruct.USART_Parity = USART_Parity_No;
			UART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
			UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_Init(USART3, &UART_InitStruct);
			/* Enable USART2 Peripheral*/
			USART_Cmd(USART3, ENABLE);
			/*NVIC Configuration*/
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			/* Configure USART Interrupt */
			USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);
			/* Create e_BUFFER_ui8 */
			e_BUFFER_ui8 = fifo_create(400, sizeof(uint8_t));
			if(e_BUFFER_ui8 == NULL)
			{
				printf("Cannot create e_BUFFER_ui8 \n\r");
			}

#define COUNTER (*((volatile unsigned long *)0xE000E018))
			printf("%d \n\r", COUNTER);
			/* Frame data ID|H_DATA|L_DATA|\n */
			while (1)
			{
				/* Toggle LED on PA0 */
				GPIOA->ODR = (uint32_t)0x00;
				Delay_ms(1000);
				//delay(10000);
        		GPIOA->ODR = (uint32_t)(1<<0);
        		Delay_ms(1000);
        		/* Generate Software Interrupt on Line5 */
				EXTI->SWIER |= (uint32_t)EXTI_Line5;
        		/* Generate Software Interrupt on Line4 */
				EXTI->SWIER |= (uint32_t)EXTI_Line4;

        		if(get_buffer(array) == 1)
        		{
        			for(j = 0; j < 4; j++)
        			{
        				if(j != 3)
        					printf("%x ", array[j]);
        				else
        					printf("%x\n\r", array[j]);
        			}
        		}
			}
}



void Delay_ms(uint32_t ms)
{
	uint32_t currenttick = e_Systick_ui32;
	while((e_Systick_ui32 - currenttick) <= ms);
}

uint8_t get_buffer(uint8_t arr[])
{
	uint8_t k, ui8l_temp;
	if(fifo_is_empty(e_BUFFER_ui8) == true)
            return 0;
    else
    {
    	uint8_t ui8l_temp,i;
    	for(k = 0; k < 4; k++)
        {
    		fifo_get(e_BUFFER_ui8, &ui8l_temp);
    		arr[k] = ui8l_temp;
        }
        return 1;
     }
}
