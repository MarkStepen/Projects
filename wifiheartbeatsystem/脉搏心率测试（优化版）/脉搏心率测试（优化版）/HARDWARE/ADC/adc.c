#include "adc.h"
#include "delay.h"

/*12λAD �����������źŲɼ�ֻ��Ҫ10λ���ȣ�����AD����ֵ����2λ
 * ����������Χ0-1024
 */

//��ʼ��ADC
void Adc_Init(void)
{
	ADC_InitTypeDef   ADC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//PA1
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,& GPIO_InitStructure);
	
	ADC_DeInit(ADC1);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	      //ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  //ת��������������ⲿ��������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;//ѡ��TIM3��Ϊ�ⲿ����Դ
	ADC_InitStructure.ADC_DataAlign= ADC_DataAlign_Right;                	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                              //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	
	
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);//�����ⲿ����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);//adcת��ʱ��21us
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);									//��λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ�У׼������У׼����״̬ΪRESET
	ADC_StartCalibration(ADC1);									//ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));			//�ȴ�У׼����	
}

 

























