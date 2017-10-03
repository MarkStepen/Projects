#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "timer.h"
#include "stdio.h"
#include "sensor.h"

#define true 1
#define false 0

// these variables are volatile because they are used during the interrupt service routine!
int BPM;                   			 // used to hold the pulse rate
int Signal;               			 // holds the incoming raw data
int IBI = 600;            			 // holds the time between beats, must be seeded! 
unsigned char Pulse = false;     // true when pulse wave is high, false when it's low
unsigned char QS = false;        // becomes true when Arduoino finds a beat.
int rate[10];                    // array to hold last ten IBI values
unsigned long sampleCounter = 0; // used to determine pulse timing
unsigned long lastBeatTime = 0;  // used to find IBI
int P =512;                      // used to find peak in pulse wave, seeded
int T = 512;                     // used to find trough in pulse wave, seeded
int thresh = 512;                // used to find instant moment of heart beat, seeded
int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
unsigned char firstBeat = true;  // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat = false;// used to seed rate array so we startup with reaso

//上位机发送函数
void sendDataToProcessing(u8 symbol, int16_t dat )
{
    putchar(symbol);       // symbol prefix tells Processing what type of data is coming
		printf("%d\r\n",dat);	 // the data to send culminating in a carriage return
}

/**心率检测*/
u8 Sensor(void)
{
	u16	rate_y,rate_y_buf,rate_y_old;//心电图纵坐标（10—160）
	u16 rate_x;//心电图横坐标（0-320）

	TIM3_Int_Init(1999,71);//定时周期2ms
	//TIM1_Int_Init(1999,71);//定时周期2ms
	LED1=0;
	
/**画两根线，将心电图和心率显示分开**/
	POINT_COLOR=RED ;//设置字体为红色
	LCD_DrawLine(0, 9, 320, 9);
	LCD_DrawLine(0, 161, 320, 161);
/*-------------------------------*/	
  POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(60,170,200,16,16,"* Signal:");	      
	LCD_ShowString(60,190,200,16,16,"* BPM: ");	
	LCD_ShowString(60,210,200,16,16,"* IBI");	  
	
	/* TIM3 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	/* TIM1 重新开时钟，开始计时 */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	
	while (1)
  {
		sendDataToProcessing('S', Signal); // send Processing the raw Pulse Sensor data	
    LCD_ShowxNum(156,170,Signal,4,16,0);
/**画心电图**/		
		rate_y = 160-(u16)(Signal*0.14648);//心电图纵坐标根据比例公式算的(150/1024=0.14648)。150为纵坐标总的点数，1024为采样到的最大值，
		
		if(rate_x<310) {
			rate_x++;//心电图横坐标，每循环一次画一个点，不准确，需加入算法
			POINT_COLOR=WHITE;
			LCD_DrawLine(rate_x,10,rate_x,160);//把上一次的在该线段下的图像清除掉
			POINT_COLOR=BLUE;
		}
		else rate_x=10;//从头开始画
		
		LCD_Fast_DrawPoint(rate_x,rate_y,RED);//把心电图的x轴和y轴对应的点画出来
		
		/**该段程序为了让心电图连接的更紧密，没有这一段会变成明显的点组成的图***/
		rate_y_buf=rate_y;
		if(rate_y_old>rate_y_buf)
			for(;rate_y_old>rate_y_buf;rate_y_old--)   {LCD_Fast_DrawPoint((rate_x-1),rate_y_old,RED);/*delay_us(100);*/}
		else 
			for(;rate_y_buf>rate_y_old;rate_y_buf--)   {LCD_Fast_DrawPoint((rate_x-1),rate_y_buf,RED);/*delay_us(100);*/}
/****画心电图结束****/			
			rate_y_old = rate_y;//记录此次的心率信号
/**------------------------------------------------------------------------**/					
		if (QS == true)
		{
			//sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix通过串口发信息给上位机
			//printf("****B %d \r\n",BPM);
			LCD_ShowxNum(156,190,BPM,4,16,0);
				
			//sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
			//printf("****Q %d \r\n",IBI);			
			LCD_ShowxNum(156,210,IBI,4,16,0);
			
		  QS = false; // reset the Quantified Self flag for next time
				
			//对BPM值进行范围划分
			if(BPM>50&&BPM<100)
				LCD_ShowString(240,210,200,16,16,"OK!   ");	
			else	
				LCD_ShowString(240,210,200,16,16,"FALSE!");	
		}
		delay_ms(20);	
		//LED0=!LED0;
	}
}


