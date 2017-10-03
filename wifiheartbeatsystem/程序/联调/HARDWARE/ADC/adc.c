#include "adc.h"
#include "delay.h"
#include "stm32f10x.h"

extern u16 Animal_Heat_Vol[30];                       // 采集温度的电压值 
extern u8 Animal_Heat_Number;                      // 采集电压数值的个数

/*12位AD ，由于心率信号采集只需要10位精度，所以AD采样值右移2位
 * 数字量化范围0-1024
 */

//初始化ADC1
void ADC1_Configuration(void)
{
	ADC_InitTypeDef   ADC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//PA1
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,& GPIO_InitStructure);
	
	ADC_DeInit(ADC1);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	      //模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  //转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;//选择TIM3作为外部触发源
	ADC_InitStructure.ADC_DataAlign= ADC_DataAlign_Right;                	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                              //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	
	
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);//采用外部触发
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);//adc转换时间21us
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);									//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待校准结束，校准结束状态为RESET
	ADC_StartCalibration(ADC1);									//AD校准
	while(ADC_GetCalibrationStatus(ADC1));			//等待校准结束	
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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
	
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


u16 Get_ADC2(u8 ch,u16 Average,u8 m)          // // ch=ADC_Channel_1  采集的次数     采集的速率=20.8K m=2
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





























