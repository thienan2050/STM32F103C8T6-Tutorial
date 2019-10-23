/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t e_Systick_ui32;
uint32_t i;
extern uint8_t e_DMA_Flag_ui8;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
  /* Test on DMA1 Channel3 Transfer Complete interrupt */
  if(DMA_GetITStatus(DMA1_IT_HT3) == SET)
  {
	  /* Clear DMA1 Channel1 Half Transfer, Transfer Complete and Global interrupt pending bits */
	  DMA_ClearITPendingBit(DMA1_IT_HT3);
	  e_DMA_Flag_ui8 = 1;
	  printf("Half transmit\n\r");
	  /* Generate Software Interrupt on Line5 */
	  EXTI->SWIER |= (uint32_t)EXTI_Line5;
  }
	if(DMA_GetITStatus(DMA1_IT_TC3) == SET)
	{
	  /* Clear DMA1 Channel1 Half Transfer, Transfer Complete and Global interrupt pending bits */
	  DMA_ClearITPendingBit(DMA1_IT_TC3);
	  e_DMA_Flag_ui8 = 1;
	  printf("Transmit Completely\n\r");
	  /* Generate Software Interrupt on Line4 */
	  EXTI->SWIER |= (uint32_t)EXTI_Line4;
	}
}
void USART3_IRQHandler(void)
{
	uint8_t data;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		/* Clear the UASRT Interrupt pending bit */
		USART_ClearFlag(USART3, USART_IT_RXNE);
		/* Echo data */
		USART_SendData(USART3, (uint8_t)USART_ReceiveData(USART3));

	}
}


void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) == SET)
	{
		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line5);
		/* Inform */
		printf("EXTI_Line5 \n\r");
	}
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line4);
		/* Inform */
		printf("EXTI_Line4 \n\r");
	}
}



/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{
	e_Systick_ui32++;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
