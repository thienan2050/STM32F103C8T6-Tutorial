#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_spi.h"
#include "misc.h"
#include "core_cm3.h"
#include "FIFO.h"
#include "function.h"
#include "debug.h"

#define BufferSize 10

GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef UART_InitStruct;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
DMA_InitTypeDef g_DMA_InitStructure_st;
SPI_InitTypeDef g_SPI_InitStructure_st;


__IO uint32_t e_Systick_ui32 = 0;
fifo_t e_BUFFER_ui8;
uint32_t j, m = 1;
uint8_t array[4];
uint32_t g_CurrentTick_ui32;
uint32_t g_Source_ui32[2];
uint32_t g_Destination_ui32[2];
uint8_t e_DMA_Flag_ui8 = 0;

uint8_t SPI_MASTER_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                                            0x07, 0x08, 0x09, 0x0A};
uint8_t SPI_MASTER_Buffer_Rx[BufferSize];

uint8_t SPI_SLAVE_Buffer_Tx[BufferSize];
uint8_t SPI_SLAVE_Buffer_Rx[BufferSize];
uint8_t M_Tx = 0, M_Rx = 0;
uint8_t S_Tx = 0, S_Rx = 0;
uint8_t Flag_end=0;

/* Private function */
void Configure_USARTprintf(void);
void Configure_USART3(void);
void Configure_DMA1_Channel1(void);
void Configure_LED_C13(void);
void SPI2_Slave_Con(void);
void SPI1_Master_Con(void);
void NVIC_Configuration(void);




int main (void)
{
	/* Configure USART2 */
	Configure_USARTprintf();
	/* Configure USART3 */
	Configure_USART3();
	/* Configure DMA1 Channel1 */
	Configure_DMA1_Channel1();
	/* Configure Pin C13 */
	Configure_LED_C13();
	/* SPI master */
	SPI1_Master_Con();
	/* SPI slave */
	SPI2_Slave_Con();
	/* Priority */
	NVIC_Configuration();
	/* Configure Systick timer */
	SysTick_Config(SystemCoreClock / 1000);
	while (1)
	{
		/* Enable SPI RXNE interrupt */
		printf("Before \n\r");
		for(M_Tx = 0; M_Tx < 10; M_Tx++)
		{
			printf("%x ", SPI_SLAVE_Buffer_Rx[M_Tx]);
		}
		printf("\n\r");
		Delay_ms(1000);
		//SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
		SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
		//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		//SPI_I2S_SendData(SPI1, 0x01);
		//Delay_ms(10);
		//GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction) (0));
		for(M_Tx = 0; M_Tx < 33; M_Tx++)
		{
			/* Wait until SPI TX  */
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
			SPI_I2S_SendData(SPI1, SPI_MASTER_Buffer_Tx[M_Tx]);
			//Delay_ms(10);
		}
		SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, DISABLE);
		printf("After \n\r");
		for(M_Tx = 0; M_Tx < 10; M_Tx++)
		{
			printf("%x ", SPI_SLAVE_Buffer_Rx[M_Tx]);
		}
		printf("\n\r");
		while(1);
/*		if(Flag_end == 1)
		{
			Flag_end = 0;
			SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);
			SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, DISABLE);
			GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction) (1));
		}*/
	}
}


void Configure_USARTprintf(void)
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
}

void Configure_USART3(void)
{
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
	UART_InitStruct.USART_BaudRate = 256000;
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
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configure USART Interrupt */
	USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);
}


void Configure_DMA1_Channel1(void)
{

	/* Enable clock for DMA1 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* Reset DMA1 channel1 */
	DMA_DeInit(DMA1_Channel1);
	g_DMA_InitStructure_st.DMA_PeripheralBaseAddr = (uint32_t)g_Source_ui32;
	g_DMA_InitStructure_st.DMA_MemoryBaseAddr = (uint32_t)g_Destination_ui32;
	g_DMA_InitStructure_st.DMA_DIR = DMA_DIR_PeripheralSRC;
	g_DMA_InitStructure_st.DMA_BufferSize = 2;
	g_DMA_InitStructure_st.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	g_DMA_InitStructure_st.DMA_MemoryInc = DMA_MemoryInc_Enable;
	g_DMA_InitStructure_st.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	g_DMA_InitStructure_st.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	g_DMA_InitStructure_st.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Circular
	g_DMA_InitStructure_st.DMA_Priority = DMA_Priority_Medium;
	g_DMA_InitStructure_st.DMA_M2M = DMA_M2M_Enable;


	DMA_Init(DMA1_Channel1, &g_DMA_InitStructure_st);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

	/* Enable DMA1 channel IRQ Channel1 */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Configure_LED_C13(void)
{
	/* Enable clock for GPIOC */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	/* Configure P13 as push-pull output*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void SPI1_Master_Con(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	g_SPI_InitStructure_st.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	g_SPI_InitStructure_st.SPI_Mode = SPI_Mode_Master;
	g_SPI_InitStructure_st.SPI_DataSize = SPI_DataSize_8b;
	g_SPI_InitStructure_st.SPI_CPOL = SPI_CPOL_Low;
	g_SPI_InitStructure_st.SPI_CPHA = SPI_CPHA_1Edge;
	g_SPI_InitStructure_st.SPI_NSS = SPI_NSS_Soft;
	g_SPI_InitStructure_st.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	g_SPI_InitStructure_st.SPI_FirstBit = SPI_FirstBit_LSB;
	//SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &g_SPI_InitStructure_st);
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);
	SPI_Cmd(SPI1, ENABLE);
}

void SPI2_Slave_Con(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);


	g_SPI_InitStructure_st.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	g_SPI_InitStructure_st.SPI_Mode = SPI_Mode_Slave;
	g_SPI_InitStructure_st.SPI_DataSize = SPI_DataSize_8b;
	g_SPI_InitStructure_st.SPI_CPOL = SPI_CPOL_Low;
	g_SPI_InitStructure_st.SPI_CPHA = SPI_CPHA_1Edge;
	g_SPI_InitStructure_st.SPI_NSS = SPI_NSS_Soft;
	g_SPI_InitStructure_st.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	g_SPI_InitStructure_st.SPI_FirstBit = SPI_FirstBit_LSB;
	//SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &g_SPI_InitStructure_st);

	SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, DISABLE);
	SPI_Cmd(SPI2, ENABLE);
}

void NVIC_Configuration(void)
{

	/* 1 bit for preemption priority, 3 bits for sub-priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* Configure and enable SPI_MASTER interrupt -------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configure and enable SPI_SLAVE interrupt --------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}
