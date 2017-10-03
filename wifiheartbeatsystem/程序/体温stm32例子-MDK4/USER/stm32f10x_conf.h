/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_conf.h 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Library configuration file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Uncomment/Comment the line below to enable/disable peripheral header file inclusion */
#include "stm32f10x_adc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_can.h"
#include "stm32f10x_cec.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dbgmcu.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_wwdg.h"
#include "misc.h" /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */
#include "stdio.h"
#include "time.h"
#include "math.h"
#include "stdlib.h"

#define	DELTA(x,y)   ((x)>(y)?(x)-(y):(y)-(x))
#define soft_reset()		(((void (code *)(void))0x0000)())
#define ASUCHAR(x,ofs)  (*((u8  *)&x+ofs))					// xΪҪ�����,0fsΪ�ڼ�λ
#define ASU16(x,ofs)  	(*((u16  *)&x+ofs))	                // xΪҪ�����,0fsΪ�ڼ�λ



void SysInit_Configuration(void);
void System_selftest(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);     
void TIM1_Configuration(void);     
void TIM2_Configuration(void);        
void ADC1_Configuration(void);	
void ADC2_Configuration(void);
void DAC1_Configuration(void);
void USART1_Configuration(void);   
void USART2_Configuration(void);	
void USART3_Configuration(void);   
void UART4_Configuration(void);
void UART5_Configuration(void);  
void NVIC_Configuration(void);  
void SPI_Configuration(void);
void TIM3_Configuration(u16 arr,u16 psc);  // ��ʱ��3��PWM���

void USARTWrite(USART_TypeDef* USARTx,char* data,u16 len);
void USARTWriteData(USART_TypeDef* USARTx,u16 data)	;

u16 Get_ADC1(u8 ch,u16 Average,u8 m);
u16 Get_ADC2(u8 ch,u16 Average,u8 m);
void DAC1_Set_VOL(u16 vol) ;                  // DAC����         
u8 Compare_String(u8* Str1,u8* Str2,u16 Str1_Start,u16 Str2_Start,u16 StrLong);
u8 Key_Scan(void);                  // ��������

void SendCMD(u8 dat);
void SendDat(u8 dat);
void SendByte(u8 dat);
void Delay10us(u16 nus);
void delay_ms(u16 nms);
void RTC_Configuration(void);


void Key_Control(void);
float Input_data(float Parameters,u8 Y);


/*****************************����*****************************************/
u8 Key_Scan(void);

/*****************************�߲ʵƵĺ���*********************************/
void MX_QCD(u16 blue,u16 red,u16 green);

/*****************************WIFI****************************************/
void WIFI_Configuration(void);
u8 WIFI_send1_cmd(char* cmd,u16 len1,u16 receive_Number);

u16 GetData(u8 REG_Address) ;
/*************************************************************************************/


/******************************����*************************************/

u16 get_pluse(u16 *p,u16 size,u8 Site);             // Green_Light_DATA

/*************************************************************************/

/******************************����*************************************/

void Reduced_Temperature(void);                     // Green_Light_DATA

/*************************************************************************/




//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08  
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)       // ��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)       // ���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)       // ��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)       // ���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)       // ��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)       // ���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)       // ��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)       // ���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)       // ��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)       // ����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)       // ��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)       // ����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)       // ��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)       // ����



/***************************�����С***********************************/
#define BufferSize          28
#define ECG_BufferSize      3750
#define Pulse_BufferSize    750
#define WAITUSARTACK        0x5fd0000
#define ADC1_DR_Address     0x4001244c

#define RECV_BUFFER_SIZE  1100   //�Ӵ��� �շ�������

#define ASUCHAR(x,ofs)  (*((u8  *)&x+ofs))					//xΪҪ�����,0fsΪ�ڼ�λ

extern u32   Animal_Heat;                          // ���±���
extern u16    Animal_Heat_Vol[30];                  // �ɼ��¶ȵĵ�ѹֵ                                       
extern u8     Animal_Heat_Number;                   // �ɼ���ѹ��ֵ�ĸ���

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM32F10x_CONF_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
