#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include "core_cm3.h"
#include "FIFO.h"
#include "function.h"
#include "debug.h"

GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef UART_InitStruct;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
DMA_InitTypeDef g_DMA_InitStructure_st;



__IO uint32_t e_Systick_ui32 = 0;
fifo_t e_BUFFER_ui8;
uint32_t j, m;
uint8_t array[4];
uint32_t g_CurrentTick_ui32;
uint32_t g_Source_ui32[800];
uint32_t g_Destination_ui32[800];
uint8_t e_DMA_Flag_ui8 = 0;


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

/**
* Configure DMA
*/
			for(m = 0; m < 800; m++)
			{
				g_Source_ui32[m] = m;
			}

			/* Enable clock for DMA1 */
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
			/* Reset DMA1 channel */
			DMA_DeInit(DMA1_Channel1);
			g_DMA_InitStructure_st.DMA_PeripheralBaseAddr = (uint32_t)g_Source_ui32;
			g_DMA_InitStructure_st.DMA_MemoryBaseAddr = (uint32_t)g_Destination_ui32;
			g_DMA_InitStructure_st.DMA_DIR = DMA_DIR_PeripheralSRC;
			g_DMA_InitStructure_st.DMA_BufferSize = 800;
			g_DMA_InitStructure_st.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
			g_DMA_InitStructure_st.DMA_MemoryInc = DMA_MemoryInc_Enable;
			g_DMA_InitStructure_st.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
			g_DMA_InitStructure_st.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
			g_DMA_InitStructure_st.DMA_Mode = DMA_Mode_Normal;
			g_DMA_InitStructure_st.DMA_Priority = DMA_Priority_Medium;
			g_DMA_InitStructure_st.DMA_M2M = DMA_M2M_Disable;

			DMA1_Channel1->CNDTR
			DMA_Init(DMA1_Channel1, &g_DMA_InitStructure_st);
			DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

			/* Enable DMA1 channel IRQ Channel */
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			DMA_Cmd(DMA1_Channel1, ENABLE);


			while(e_DMA_Flag_ui8 == 0);
			printf("%d\n\r", g_Destination_ui32[100]);

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
				//EXTI->SWIER |= (uint32_t)EXTI_Line5;
        		/* Generate Software Interrupt on Line4 */
				//EXTI->SWIER |= (uint32_t)EXTI_Line4;

        		DMA_Cmd(DMA1_Channel1, DISABLE);
        		g_Source_ui32[100] = ++m;
        		DMA_Cmd(DMA1_Channel1, ENABLE);
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
        		//printf("%d\n\r", g_Destination_ui32[0]);
			}
}





