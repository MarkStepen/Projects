#include "stm32f10x.h"

/**************************************************
* 函数名  GPIO_Configuration
* 参数    无
* 描述    对所用到的IO口进行基本配置
*
**************************************************/
void GPIO_Configuration(void) 
{ 
	GPIO_InitTypeDef GPIO_InitStructure;					//定义GPIO初始化结构体GPIO_InitStructure 


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;	              // PA0为按键管脚			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;             // GPIO_Mode_IPU管脚配置成上拉  GPIO_Mode_IPD为下拉输入              
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

/*************************液晶管脚定义*******************************************/    
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;   // PB12复位引脚 PB13时钟引脚  PB14 数据引脚   PB15数据/命令 控制引脚 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);						     // 复位引脚拉高

/*************************血样采集脚定义*****************************************/ 
    GPIO_InitStructure.GPIO_Pin   =GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_3| GPIO_Pin_6|GPIO_Pin_8 ;    // PC0 红光 PC1红外  PC3 绿灯
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                               //  PC6为低功耗，PC8 为快速回复
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;	  // PC4为按键管脚		  PC7为 导联检测			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;             // GPIO_Mode_IPU管脚配置成上拉  GPIO_Mode_IPD为下拉输入              
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;	              // PC4为按键管脚					
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     // 管脚配置成上拉              
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}










