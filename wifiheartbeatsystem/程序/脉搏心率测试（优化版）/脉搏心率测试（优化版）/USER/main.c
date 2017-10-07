#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "timer.h"
#include "stdio.h"
#include "PWM.h"

extern int IBI;          //相邻节拍时间
extern int BPM;          //心率值             
extern int Signal;       //原始信号值            
extern unsigned char QS; //发现心跳标志

//上位机发送函数
void sendDataToProcessing(u8 symbol, int16_t dat )
{
    putchar(symbol);       // symbol prefix tells Processing what type of data is coming
		printf("%d\r\n",dat);	 // the data to send culminating in a carriage return
}

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	delay_init();
	LED_Init();
	NVIC_Configuration();
	uart_init(115200);
	Adc_Init();
	TIM3_Int_Init(1999,71);//定时周期2ms
	LED1=0;
	/* TIM3 重新开时钟，开始计时 */
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


