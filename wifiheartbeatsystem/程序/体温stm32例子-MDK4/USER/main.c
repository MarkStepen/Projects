/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：用3.5.0版本建的工程模板。         
 * 实验平台：STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ： 济通电气
 * qq群    ： 单片机项目茶座
 * 淘宝    ：http://.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"


u32    Animal_Heat=0;                             // 体温变量




/* --------------------------------------------------------------------------------------------------
 * 函数名：main
 * 描述  : 主函数
 * 输入  ：无
 * 输出  : 无
 ---------------------------------------------------------------------------------------------------*/

int main(void)
{

    SysInit_Configuration();				          // 系统初始化函数


	while(1)
    {

        Reduced_Temperature();                          // 体温采集
 
   }
}

void SysInit_Configuration(void)
{	
	RCC_Configuration();      	    // 利用外部高速时钟HSE对系统相关时钟进行配置
	GPIO_Configuration();     	    // IO口进行基本配置
  USART1_Configuration();
	TIM2_Configuration();     	    // 定时器2配置
	//ADC1_Configuration();	  	    // 模数转换器配置
    ADC2_Configuration();	  	    // 模数转换器配置


    TIM_Cmd(TIM2, ENABLE);    	    // 定时器2启动

}
