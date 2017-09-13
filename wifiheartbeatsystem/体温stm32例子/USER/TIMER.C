#include "stm32f10x.h"

void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;			    // 定义TIM_TimeBase初始化结构体TIM_TimeBaseStructure
	TIM_OCInitTypeDef			TIM_OCInitStructure;			    // 定义TIM_OCInit初始化结构体TIM_OCInitStructure
	NVIC_InitTypeDef NVIC_InitStructure;						    // 定义NVIC初始化结体NVIC_InitStructure     
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 			    // 选择NVIC优先级分组0 

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				    // 使能TIM2全局中断,0级先占优先级,3级次占有先机	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       // 设定成员NVIC_IRQChannel中的先占优先级 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;              // 设定成员NVIC_IRQChannel中的从优先级 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 // 使能成员NVIC_IRQChannel
	NVIC_Init(&NVIC_InitStructure);	                                // 根据NCIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_TimeBaseStructure.TIM_Period=65535;		 				    /* 自动重装载寄存器周期的值(计数值)（设置了在下一个更新事件装入活动的自动重装在进村器周期的值） */
    TIM_TimeBaseStructure.TIM_Prescaler=0;						    /* 时钟预分频数（设置了用来作为TIMx时钟频率除数的预分频值）(对PSC寄存器操作)*/
    TIM_TimeBaseStructure.TIM_ClockDivision=0; 					    /* 时钟分割 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	    /* 向上计数模式 */
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

	TIM_PrescalerConfig(TIM2,5599,TIM_PSCReloadMode_Immediate);     // 设置TIM2预分频(这句话的意思是把预分频值计时的装入预分频器中)计算出定时器时钟方法72MHz/(7199+1)=10KHz(定时器每次计数时间间隔为1/10ms)（7199为预分频值((对PSC寄存器操作))）(TIM_PSCReloadMode_Immediate位预分频值即时装入模式)
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_Timing;			    // 选择定时器模式为TIM输出比较时间模式(选择定时器模式)
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	    // 使能输出比较状态
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		    // TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse=100;							    // 设置了待装入捕获比较寄存器的脉冲值
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);						    // 根据TIM_OCInitStruct中指定的参数初始化TIM2通道1
	TIM_OCInitStructure.TIM_Pulse=10000;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse=40;
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Disable);			     // 失能TIM2在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Disable);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Disable);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);						     // 清除更新事件标志位(防止启动便中断)
	TIM_ITConfig(TIM2,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3,ENABLE);	     // 使能TIM2中断（使能捕获/比较1,2,3中断源） 						  		
}






















































