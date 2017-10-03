#include "adc.h"
#include "delay.h"
#include "stm32f10x.h"

extern u16 Animal_Heat_Vol[30];                       // �ɼ��¶ȵĵ�ѹֵ 
extern u8 Animal_Heat_Number;                      // �ɼ���ѹ��ֵ�ĸ���

/*12λAD �����������źŲɼ�ֻ��Ҫ10λ���ȣ�����AD����ֵ����2λ
 * ����������Χ0-1024
 */

//��ʼ��ADC1
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
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	      //ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  //ת��������������ⲿ��������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;//ѡ��TIM3��Ϊ�ⲿ����Դ
	ADC_InitStructure.ADC_DataAlign= ADC_DataAlign_Right;                	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                              //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	
	
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);//�����ⲿ����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);//adcת��ʱ��21us
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);									//��λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ�У׼������У׼����״̬ΪRESET
	ADC_StartCalibration(ADC1);									//ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));			//�ȴ�У׼����	
}

void ADC2_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;	
    
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						    //����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 						    //ɨ��ģʽ���ڵ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;						    //����װ��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		    //��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 					    //�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 								    //Ҫת����ͨ����Ŀ
	ADC_Init(ADC2, &ADC_InitStructure);
    
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);										    //����ADCʱ�ӣ�ΪPCLK6��6��Ƶ����14MHz 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;		            // AD�ɼ��ܽ����� ��ص�ѹ�ɼ� ͨ��13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;               // ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_71Cycles5 );
    
    
  ADC_Cmd(ADC2, ENABLE);														/* Enable ADC2 */
	ADC_ResetCalibration(ADC2); 						//����ADC2У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC2));			//�ȴ�ADC2У׼�������
	ADC_StartCalibration(ADC2);							//��ʼADC2У׼
	while(ADC_GetCalibrationStatus(ADC2));				//�ȴ�ADC2У׼���
    
  ADC_SoftwareStartConvCmd(ADC2, ENABLE);		    // ʹ��ָ����ADC1�����ת����������	 

}


u16 Get_ADC2(u8 ch,u16 Average,u8 m)          // // ch=ADC_Channel_1  �ɼ��Ĵ���     �ɼ�������=20.8K m=2
{	
	unsigned int result=0;
	unsigned char i; 	
    ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_71Cycles5 );
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);		    // ʹ��ָ����ADC1�����ת����������	    
  	for(i=0;i<Average;i++)                              // �ɼ��Ĵ���
	{
      //		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		    // ʹ��ָ����ADC1�����ת����������	 
		while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC )); // �ȴ�ת������
        Animal_Heat_Vol[Animal_Heat_Number]=ADC_GetConversionValue(ADC2);
        
        Animal_Heat_Number++;
	}
	return result;	                                    // ����ƽ��ֵ
}





























