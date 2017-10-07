#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "timer.h"
#include "stdio.h"
//#include "PWM.h"

//extern int IBI;          //相邻节拍时间
//extern int BPM;          //心率值             
//extern int Signal;       //原始信号值            
//extern unsigned char QS; //发现心跳标志

////上位机发送函数
//void sendDataToProcessing(u8 symbol, int16_t dat )
//{
//    putchar(symbol);       // symbol prefix tells Processing what type of data is coming
//		printf("%d\r\n",dat);	 // the data to send culminating in a carriage return
//}

///**
//  * @brief  主函数
//  * @param  无  
//  * @retval 无
//  */
//int main(void)
//{
//	u16	rate_y,rate_y_buf,rate_y_old;//心电图纵坐标（10—160）
//	u16 rate_x;//心电图横坐标（0-320）
//	
//	delay_init();
//	LED_Init();
//	NVIC_Configuration();
//	uart_init(115200);
//	LCD_Init();
//	Adc_Init();
//	TIM3_Int_Init(1999,71);//定时周期2ms
//	LED1=0;
//	
///**画两根线，将心电图和心率显示分开**/
//	POINT_COLOR=RED ;//设置字体为红色
//	LCD_DrawLine(0, 9, 320, 9);
//	LCD_DrawLine(0, 161, 320, 161);
///*-------------------------------*/	
//  POINT_COLOR=BLUE;//设置字体为蓝色
//	LCD_ShowString(60,170,200,16,16,"* Signal:");	      
//	LCD_ShowString(60,190,200,16,16,"* BPM: ");	
//	LCD_ShowString(60,210,200,16,16,"* IBI");	  
//	
//	/* TIM3 重新开时钟，开始计时 */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
//	
//	while (1)
//  {
//		sendDataToProcessing('S', Signal); // send Processing the raw Pulse Sensor data	
//    LCD_ShowxNum(156,170,Signal,4,16,0);
///**画心电图**/		
//		rate_y = 160-(u16)(Signal*0.14648);//心电图纵坐标根据比例公式算的(150/1024=0.14648)。150为纵坐标总的点数，1024为采样到的最大值，
//		
//		if(rate_x<310) {
//			rate_x++;//心电图横坐标，每循环一次画一个点，不准确，需加入算法
//			POINT_COLOR=WHITE;
//			LCD_DrawLine(rate_x,10,rate_x,160);//把上一次的在该线段下的图像清除掉
//			POINT_COLOR=BLUE;
//		}
//		else rate_x=10;//从头开始画
//		
//		LCD_Fast_DrawPoint(rate_x,rate_y,RED);//把心电图的x轴和y轴对应的点画出来
//		
//		/**该段程序为了让心电图连接的更紧密，没有这一段会变成明显的点组成的图***/
//		rate_y_buf=rate_y;
//		if(rate_y_old>rate_y_buf)
//			for(;rate_y_old>rate_y_buf;rate_y_old--) LCD_Fast_DrawPoint((rate_x-1),rate_y_old,RED);
//		else 
//			for(;rate_y_buf>rate_y_old;rate_y_buf--) LCD_Fast_DrawPoint((rate_x-1),rate_y_buf,RED);
///****画心电图结束****/			
//			rate_y_old = rate_y;//记录此次的心率信号
///**------------------------------------------------------------------------**/					
//		if (QS == true)
//		{
//			//sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix通过串口发信息给上位机
//			//printf("****B %d \r\n",BPM);
//			LCD_ShowxNum(156,190,BPM,4,16,0);
//				
//			//sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
//			//printf("****Q %d \r\n",IBI);			
//			LCD_ShowxNum(156,210,IBI,4,16,0);
//			
//		  QS = false; // reset the Quantified Self flag for next time
//				
//			//对BPM值进行范围划分
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
	Sensor();//心率传感器测心率函数
}


