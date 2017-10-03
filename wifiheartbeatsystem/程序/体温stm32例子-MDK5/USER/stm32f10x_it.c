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
#include "stm32f10x_conf.h"
u16 ECG_DATA_buf[BufferSize];				//�ĵ����ݻ�����

u16  ECG_Receive_Curr=0;         // ����ECG ��ǰָ�� 
u16  Pulse_Receive_Curr=0;       // ���պ����� ��ǰָ�� 
u16  ECG_Receive_Last=0;         // ����ECG ��ǰ����ָ��  Pulse_DATA
u8   ECG_sign=0;  
u8   Pulse_sign=0; 
u16  i=0;

u8   Count_Red=0;                 // PWM����
u8   Count_GREEN=0;               // PWM���� 
u8   Count_BLUE=0;

u16  Count_Pule_Ir=0;           //  ������ʱ��������
u16  Count_Pule_Red=0;          //  ���ɼ���ʱ��������

u8   Blood_Oxygen_Time=0;
u32  Blood_Oxygen_Sum=0;

u8   Key_Valuatione;          // �õ�����ֵ
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

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
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void DMA1_Channel4_IRQHandler(void)
{		
		
}
void DMA1_Channel5_IRQHandler(void)
{		
		
}
void DMA1_Channel6_IRQHandler(void)
{		
		
}
void DMA1_Channel7_IRQHandler(void)
{		
		
}
void DMA2_Channel5_IRQHandler(void)
{		
		
}
void DMA2_Channel3_IRQHandler(void)
{		
		
}
void DMA2_Channel4_5_IRQHandler(void)
{	
	
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{


        
	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{

        
	}
}

void TIM2_IRQHandler(void) 
{ 
	vu16 capture=0;	    // ��ǰ�������ֵ�ֲ����� 

	if(TIM_GetITStatus(TIM2,TIM_IT_CC1)!=RESET)			            // ���TIM2��TIM����/�Ƚ�1�ж�Դ����10ms
	{

        capture=TIM_GetCapture1(TIM2);				                 // ��ȡ��ǰ����ֵ
        TIM_SetCompare1(TIM2,capture+100);			                 // ���ݵ�ǰ����ֵ�����������Ĵ�������Ϊ�Ƕ�ʱ���ļ����������ϼ���(ÿ��һ������ʱ����1/10ms)��
        TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);                      // 10ms
	}else if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)
	{  
        capture=TIM_GetCapture2(TIM2);                               // ���TIMx���벶��2��ֵ
        TIM_SetCompare2(TIM2,capture+10000);                         // ����TIMx����Ƚ�2�Ĵ���ֵ
        TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);	                     // 1s	
	}
	else if(TIM_GetITStatus(TIM2,TIM_IT_CC3)!=RESET)                  // 4ms   
	{       
   
        capture=TIM_GetCapture3(TIM2);
        TIM_SetCompare3(TIM2,capture+40);                             // 4ms�Ķ�ʱ(����TIMx����Ƚ�3�Ĵ���ֵ)
        TIM_ClearITPendingBit(TIM2,TIM_IT_CC3);	
	}
}

void TIM3_IRQHandler(void) 
{

}

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
