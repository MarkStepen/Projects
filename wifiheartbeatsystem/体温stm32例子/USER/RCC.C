#include "stm32f10x.h"

void RCC_Configuration(void) 
{ 
	ErrorStatus     HSEStartUpStatus;							//定义枚举类型变量HSEStartUpStatus
	RCC_DeInit(); 	 											//复位系统时钟 
	RCC_HSEConfig(RCC_HSE_ON); 	 								//开启HSE 
	HSEStartUpStatus=RCC_WaitForHSEStartUp(); 					//等待HSE稳定起振
	
	if(HSEStartUpStatus==SUCCESS)								//如果HSE稳定起振								 
	{ 		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);  	                  	//选择HCLK(AHB)时钟源为SYSCLK 1分频 56MHz	
		RCC_PCLK2Config(RCC_HCLK_Div4); 	                  	//选择PCLK2时钟源为HCLK(AHB) 1分频 56MHz/4=14MHz 		
		RCC_PCLK1Config(RCC_HCLK_Div2); 	                  	//选择PLCK1时钟源为HCLK(AHB) 2分频 28MHz  （最大是36MHz）	
		FLASH_SetLatency(FLASH_Latency_2); 	                  	//设置FLASH延时周期数为2 		 
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); 	//使能FLASH预取缓存		
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_7);   	//选择PLL时钟源为HSE 1分频,倍频数为9,则PLL=8MHz*7=56MHz 	
		RCC_PLLCmd(ENABLE); 							 	 	//使能PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);  		//等待PLL输出稳定 	
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 			  	//以PLL作为系统时钟  		 
		while(RCC_GetSYSCLKSource()!=0x08); 	         	  	//等待PLL成为有效系统时钟源
	} 	 
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1|RCC_AHBPeriph_DMA2,ENABLE);		  	                                  // 使能或者失能AHB外设时钟   RCC_APB2Periph_ADC1|RCC_APB2Periph_TIM1 |
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|       // 使能或者失能APB2外设时钟
	                       RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1|RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2 ,ENABLE); 			
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |RCC_APB1Periph_TIM3 |RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4|     // 使能或者失能APB1的外设时钟
                           RCC_APB1Periph_UART5|RCC_APB1Periph_BKP|RCC_APB1Periph_PWR|RCC_APB1Periph_DAC, ENABLE);
}

void Delay10us(u16 nus)
{
	u32 i;
	i = 100*nus;
	while (--i);
}
void delay_ms(u16 nms)
{
	u32 i;
	i = 8000*nms;
	while (--i);
}

