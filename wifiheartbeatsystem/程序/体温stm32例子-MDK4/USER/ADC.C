#include "stm32f10x.h"

void ADC1_Configuration(void)
{
//	ADC_InitTypeDef ADC_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;										    // 定义GPIO初始化结构体GPIO_InitStructure 
//	DMA_InitTypeDef DMA_InitStructure;											    // 定义DMA初始化结构体DMA_InitStructure

//	DMA_DeInit(DMA1_Channel1);													    // 重置DMA 1通道配置	
//	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;					    // 外设地址:ADC1_DR_Address	
//	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ECG_DATA_buf;			            // 内存地址:ECG_DATA
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							    // 外设作为数据来源
//	DMA_InitStructure.DMA_BufferSize = BufferSize;								    // DMA缓存大小:BufferSize
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			    // 外设地址寄存器不变 
//  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		 				    // 内存地址寄存器递增	
//  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;     // 外设数据宽度为16位
//  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			    // 内存数据宽度为16位
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								    // CAN工作在正常缓存模式
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							    // 设置DMA通道优先级为高
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								    // DMA通道不设置为内存道内存传输
//	DMA_Init(DMA1_Channel1, &DMA_InitStructure);								    // 初始化 
//	DMA_Cmd(DMA1_Channel1, ENABLE);

//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_5;		            // AD采集管脚配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						        // 独立ADC模式
//	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 						        // 扫描模式用于多通道采集）
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						        // 开启连续转换模式，即不停地进行ADC转换
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		        // 不使用外部触发转换(软件触发)
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 					        // 采集数据右对齐
//	ADC_InitStructure.ADC_NbrOfChannel = 2;	 								        // 要转换的通道数目
//	ADC_Init(ADC1, &ADC_InitStructure);
//	RCC_ADCCLKConfig(RCC_PCLK2_Div4);	        //  配置ADC时钟，为PCLK2的6分频，即14/8=1.75MHz   
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_239Cycles5 );   // PC2                         // 红外/红光 （采样时间是239.5个周期）转换时间=21us
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_239Cycles5 );   // PC5                         // 心电采集  （采样时间是239.5个周期）转换时间=21us	


//// AD的采集速度 采集速度=1.75/(239.5+12.5)=0.0069=6.9K/s 一路AD是6.9K   采集6.9个数是1ms


//	ADC_DMACmd(ADC1, ENABLE);							/* Enable ADC1 DMA */
//	ADC_Cmd(ADC1, ENABLE);								/* Enable ADC1 */   
//	ADC_ResetCalibration(ADC1);							/*复位校准寄存器 */
//	while(ADC_GetResetCalibrationStatus(ADC1));			/*等待校准寄存器复位完成 */
//	ADC_StartCalibration(ADC1);							/* ADC校准 */
//	while(ADC_GetCalibrationStatus(ADC1));				/* 等待校准完成*/ 
//	
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE);				/* 由于没有采用外部触发，所以使用软件触发ADC转换 */
   
}

void ADC2_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;	
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						    //独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 						    //扫描模式用于单通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;						    //单次装换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		    //不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 					    //采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 								    //要转换的通道数目
	ADC_Init(ADC2, &ADC_InitStructure);
    
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);										    //配置ADC时钟，为PCLK6的6分频，即14MHz 

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;		            // AD采集管脚配置 电池电压采集 通道13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;               // 模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_71Cycles5 );
    
    
    ADC_Cmd(ADC2, ENABLE);														/* Enable ADC2 */
	ADC_ResetCalibration(ADC2); 						//重置ADC2校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC2));			//等待ADC2校准重置完成
	ADC_StartCalibration(ADC2);							//开始ADC2校准
	while(ADC_GetCalibrationStatus(ADC2));				//等待ADC2校准完成
    
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);		    // 使能指定的ADC1的软件转换启动功能	 

}


u16 Get_ADC2(u8 ch,u16 Average,u8 m)          // // ch=ADC_Channel_14  采集的次数     采集的速率=20.8K m=2
{	
	unsigned int result=0;
	unsigned char i; 	
    ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_71Cycles5 );
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);		    // 使能指定的ADC1的软件转换启动功能	    
  	for(i=0;i<Average;i++)                              // 采集的次数
	{
        //		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		    // 使能指定的ADC1的软件转换启动功能	 
		while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC )); // 等待转换结束
        Animal_Heat_Vol[Animal_Heat_Number]=ADC_GetConversionValue(ADC2);
        
        Animal_Heat_Number++;
	}
	return result;	                                    // 返回平均值
}




