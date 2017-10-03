#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "timer.h"
#include "stdio.h"
//#include "PWM.h"

//extern int IBI;          //���ڽ���ʱ��
//extern int BPM;          //����ֵ             
//extern int Signal;       //ԭʼ�ź�ֵ            
//extern unsigned char QS; //����������־

////��λ�����ͺ���
//void sendDataToProcessing(u8 symbol, int16_t dat )
//{
//    putchar(symbol);       // symbol prefix tells Processing what type of data is coming
//		printf("%d\r\n",dat);	 // the data to send culminating in a carriage return
//}

///**
//  * @brief  ������
//  * @param  ��  
//  * @retval ��
//  */
//int main(void)
//{
//	u16	rate_y,rate_y_buf,rate_y_old;//�ĵ�ͼ�����꣨10��160��
//	u16 rate_x;//�ĵ�ͼ�����꣨0-320��
//	
//	delay_init();
//	LED_Init();
//	NVIC_Configuration();
//	uart_init(115200);
//	LCD_Init();
//	Adc_Init();
//	TIM3_Int_Init(1999,71);//��ʱ����2ms
//	LED1=0;
//	
///**�������ߣ����ĵ�ͼ��������ʾ�ֿ�**/
//	POINT_COLOR=RED ;//��������Ϊ��ɫ
//	LCD_DrawLine(0, 9, 320, 9);
//	LCD_DrawLine(0, 161, 320, 161);
///*-------------------------------*/	
//  POINT_COLOR=BLUE;//��������Ϊ��ɫ
//	LCD_ShowString(60,170,200,16,16,"* Signal:");	      
//	LCD_ShowString(60,190,200,16,16,"* BPM: ");	
//	LCD_ShowString(60,210,200,16,16,"* IBI");	  
//	
//	/* TIM3 ���¿�ʱ�ӣ���ʼ��ʱ */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
//	
//	while (1)
//  {
//		sendDataToProcessing('S', Signal); // send Processing the raw Pulse Sensor data	
//    LCD_ShowxNum(156,170,Signal,4,16,0);
///**���ĵ�ͼ**/		
//		rate_y = 160-(u16)(Signal*0.14648);//�ĵ�ͼ��������ݱ�����ʽ���(150/1024=0.14648)��150Ϊ�������ܵĵ�����1024Ϊ�����������ֵ��
//		
//		if(rate_x<310) {
//			rate_x++;//�ĵ�ͼ�����꣬ÿѭ��һ�λ�һ���㣬��׼ȷ��������㷨
//			POINT_COLOR=WHITE;
//			LCD_DrawLine(rate_x,10,rate_x,160);//����һ�ε��ڸ��߶��µ�ͼ�������
//			POINT_COLOR=BLUE;
//		}
//		else rate_x=10;//��ͷ��ʼ��
//		
//		LCD_Fast_DrawPoint(rate_x,rate_y,RED);//���ĵ�ͼ��x���y���Ӧ�ĵ㻭����
//		
//		/**�öγ���Ϊ�����ĵ�ͼ���ӵĸ����ܣ�û����һ�λ������Եĵ���ɵ�ͼ***/
//		rate_y_buf=rate_y;
//		if(rate_y_old>rate_y_buf)
//			for(;rate_y_old>rate_y_buf;rate_y_old--) LCD_Fast_DrawPoint((rate_x-1),rate_y_old,RED);
//		else 
//			for(;rate_y_buf>rate_y_old;rate_y_buf--) LCD_Fast_DrawPoint((rate_x-1),rate_y_buf,RED);
///****���ĵ�ͼ����****/			
//			rate_y_old = rate_y;//��¼�˴ε������ź�
///**------------------------------------------------------------------------**/					
//		if (QS == true)
//		{
//			//sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefixͨ�����ڷ���Ϣ����λ��
//			//printf("****B %d \r\n",BPM);
//			LCD_ShowxNum(156,190,BPM,4,16,0);
//				
//			//sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
//			//printf("****Q %d \r\n",IBI);			
//			LCD_ShowxNum(156,210,IBI,4,16,0);
//			
//		  QS = false; // reset the Quantified Self flag for next time
//				
//			//��BPMֵ���з�Χ����
//			if(BPM>50&&BPM<100)
//				LCD_ShowString(240,210,200,16,16,"OK!   ");	
//			else	
//				LCD_ShowString(240,210,200,16,16,"FALSE!");	
//		}
//		delay_ms(20);	
//	}
//}
#include "sensor.h"

int main()
{
	delay_init();
	LED_Init();
	NVIC_Configuration();
	uart_init(115200);
	LCD_Init();
	//Adc_Init_time1();
	Adc_Init();
	Sensor();//���ʴ����������ʺ���
}


