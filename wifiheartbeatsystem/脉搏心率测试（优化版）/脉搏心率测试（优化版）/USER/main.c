#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "timer.h"
#include "stdio.h"
#include "PWM.h"

extern int IBI;          //���ڽ���ʱ��
extern int BPM;          //����ֵ             
extern int Signal;       //ԭʼ�ź�ֵ            
extern unsigned char QS; //����������־

//��λ�����ͺ���
void sendDataToProcessing(u8 symbol, int16_t dat )
{
    putchar(symbol);       // symbol prefix tells Processing what type of data is coming
		printf("%d\r\n",dat);	 // the data to send culminating in a carriage return
}

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
	delay_init();
	LED_Init();
	NVIC_Configuration();
	uart_init(115200);
	Adc_Init();
	TIM3_Int_Init(1999,71);//��ʱ����2ms
	LED1=0;
	/* TIM3 ���¿�ʱ�ӣ���ʼ��ʱ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	
	while (1)
  {
    sendDataToProcessing('S', Signal); // send Processing the raw Pulse Sensor data	
		if (QS == true)
		{
			sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
	  	//printf("****B %d \r\n",BPM);	
			sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
			//printf("****Q %d \r\n",IBI);					
		  QS = false; // reset the Quantified Self flag for next time	
		}		
		delay_ms(20);	
	}
}


