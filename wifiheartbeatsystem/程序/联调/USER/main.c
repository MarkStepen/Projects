//#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "timer.h"
#include "stdio.h"
#include "PWM.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_iwdg.h"

u32   Animal_Heat=0;     
u16    Animal_Heat_Vol[30];                       // 采集温度的电压值 
u8     Animal_Heat_Number=0;                      // 采集电压数值的个数
u16    Last_RES=0;                                // 得到的电阻值
u16    Animal_Heat_Compute[50];                   // 计算温度数组
u8     Animal_Heat_Compute_Number=0;              // 得到的温度的个数
u16   const Resistance[14]= {0x091A,0x09A6,0x0A3C,0X0AD2,0X0B68,0X0B90,0X0C26,0X0CBC,0X0CEE,0X0D84,0X0E1A,0X0EB0,0X0F46};     // 电阻对应的表
u16   const Voltage[14]=    {0X0673,0X06CA,0X0739,0X079D,0X080C,0X0825,0X0895,0X08F8,0X0911,0X0981,0X09F1,0X0A54,0X0AC4};     // ADC采样对应的表
u16   const Temperature[14]={0X10EA,0X1054,0X0FC8,0X0F3C,0X0EBA,0X0E92,0X0E1A,0X0DAC,0X0D84,0X0D16,0X0CB2,0X0C58,0X0BF4};     // 温度对应的表
float tempvalue = 0;     //温度值
extern int IBI;          //相邻节拍时间
extern int BPM;          //心率值             
extern int Signal;       //原始信号值            
extern unsigned char QS; //发现心跳标志

//心率参数发送函数
void sendDataToProcessing(u8 symbol, int16_t dat )
{
    putchar(symbol);       // symbol prefix tells Processing what type of data is coming
		printf("%d\r\n",dat);	 // the data to send culminating in a carriage return
}

//体温发送函数
void sendDatatemp(u8 symbol, float dat )
{
    putchar(symbol);       // symbol prefix tells Processing what type of data is coming
		printf("%.1f\r\n",dat);	 // the data to send culminating in a carriage return
}

void Get_Resistance_Value()
{
   u8 i,j,Sum_Number=0; u16 ADC_Value;
   u16 Temperature_Vol_sum=0,Temperature_Vol_sum1=0;
   u16 Average_Value=0,Average_Value1=0,Average_Value2=0; 
   if(Animal_Heat_Number>=19)
   {          
        for(i=0;i<20;i++)                                              // 对采集到的数组进行从小到大的顺序排列                   
        {
            for(j=i+1;j<20;j++)
            {
                if(Animal_Heat_Vol[i]>Animal_Heat_Vol[j])                                    
                {
                    ADC_Value=Animal_Heat_Vol[i];
                    Animal_Heat_Vol[i]=Animal_Heat_Vol[j];
                    Animal_Heat_Vol[j]=ADC_Value;
//                    break;
                }	
            }
        }
        Temperature_Vol_sum=Sum_Number=0;
        for(i=5;i<15;i++)  Temperature_Vol_sum+=Animal_Heat_Vol[i];    // 计算出中间10个数的总和
        Average_Value=Temperature_Vol_sum/10;                          // 求10个数的平均数
        Temperature_Vol_sum1=0;
        for(i=5;i<15;i++)
        {     
            if(abs(Animal_Heat_Vol[i]-Average_Value)<0x02)
            {
                Temperature_Vol_sum1+=Animal_Heat_Vol[i];    // 计算出中间10个数的总和
                Sum_Number++;
            }                
        }
        if(Sum_Number>0) Average_Value1=Temperature_Vol_sum1/Sum_Number;
        else             Average_Value1=Animal_Heat_Vol[5];
        Average_Value2=(Average_Value+Average_Value1)/2;
        
//        printf("\r\n 平均电压值=%d \r\n ",Average_Value2);
        
        if((Average_Value2>=0X0673)&&(Average_Value2<=0x0AC4))
        {
            for(i=0;i<12;i++)
            {
                if((Voltage[i]<=Average_Value2)&&(Average_Value2<=Voltage[i+1]))
                {
                    Last_RES=(((Average_Value2-Voltage[i])*100/(Voltage[i+1]-Voltage[i]))*(Resistance[i+1]-Resistance[i]))/100+ Resistance[i];  //将ADC的值转换成电阻值  
                }
            }
        }
        Average_Value2=0;       Animal_Heat_Number=0;   
   }
}

void Reduced_Temperature(void)
{
    u8 i; u32 Temperature_Temp;

    if(Animal_Heat_Number<19)  { Get_ADC2(ADC_Channel_1,1,2); return;}
//    printf("测试到的电压=%d          ",Animal_Heat_Vol[10]);
    Get_Resistance_Value();                                             // 得到电阻值
//    printf("电阻值=%d          ",Last_RES);
    if((0x091A<=Last_RES)&&(Last_RES<=0X0F46))
    {
       for(i=0;i<12;i++)
       {
           if((Last_RES>=Resistance[i])&&(Last_RES<=Resistance[i+1]))
           {
               Animal_Heat_Compute[Animal_Heat_Compute_Number]=Temperature[i]-(((Last_RES-Resistance[i])*100/(Resistance[i+1]-Resistance[i]))*(Temperature[i+1]-Temperature[i]))/100; // 得到体温
//               Animal_Heat=Animal_Heat_Compute[Animal_Heat_Compute_Number];
               Last_RES=0;  
               break; 
           }
       }
    }
//    printf("温度值1=%d          ",Animal_Heat_Compute[Animal_Heat_Compute_Number]);
		tempvalue = Animal_Heat_Compute[Animal_Heat_Compute_Number];
    Temperature_Temp=0;
    for(i=0;i<50;i++)
    {
       Temperature_Temp+=Animal_Heat_Compute[i];
    }
    Animal_Heat= Temperature_Temp/500;
  //  printf("温度值2=%d \r\n ",Animal_Heat);
    Animal_Heat_Compute_Number++;
    if(Animal_Heat_Compute_Number>49) Animal_Heat_Compute_Number=0;    
}

void RCC_Configuration(void) 
{ 
	ErrorStatus     HSEStartUpStatus;							//定义枚举类型变量HSEStartUpStatus
	RCC_DeInit(); 	 											//复位系统时钟 
	RCC_HSEConfig(RCC_HSE_ON); 	 								//开启HSE 
	HSEStartUpStatus=RCC_WaitForHSEStartUp(); 					//等待HSE稳定起振
	
	if(HSEStartUpStatus==SUCCESS)								//如果HSE稳定起振								 
	{ 		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);  	                  	//选择HCLK(AHB)时钟源为SYSCLK 1分频 56MHz	
		RCC_PCLK2Config(RCC_HCLK_Div4); 	                  	//选择PCLK2时钟源为HCLK(AHB) 1分频 56MHz/4=14MHz 		
		RCC_PCLK1Config(RCC_HCLK_Div2); 	                  	//选择PLCK1时钟源为HCLK(AHB) 2分频 28MHz  （最大是36MHz）	
		FLASH_SetLatency(FLASH_Latency_2); 	                  	//设置FLASH延时周期数为2 		 
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); 	//使能FLASH预取缓存		
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_7);   	//选择PLL时钟源为HSE 1分频,倍频数为9,则PLL=8MHz*7=56MHz 	
		RCC_PLLCmd(ENABLE); 							 	 	//使能PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);  		//等待PLL输出稳定 	
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 			  	//以PLL作为系统时钟  		 
		while(RCC_GetSYSCLKSource()!=0x08); 	         	  	//等待PLL成为有效系统时钟源
	} 	 
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1|RCC_AHBPeriph_DMA2,ENABLE);		  	                                  // 使能或者失能AHB外设时钟   RCC_APB2Periph_ADC1|RCC_APB2Periph_TIM1 |
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|       // 使能或者失能APB2外设时钟
	                       RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1|RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2 ,ENABLE); 			
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |RCC_APB1Periph_TIM3 |RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4|     // 使能或者失能APB1的外设时钟
                           RCC_APB1Periph_UART5|RCC_APB1Periph_BKP|RCC_APB1Periph_PWR|RCC_APB1Periph_DAC, ENABLE);
}

void upload_data(void)
{
	UART1_Send_DATA('B');
	UART1_Send_DATA(BPM);
	USART_SendString("\r\n");
	UART1_Send_DATA('T');
	UART1_Send_DATA(tempvalue/100);
	USART_SendString("\r\n");
}

void IWDG_Init()
{

//Enable write access to IWDG_PR and IWDG_RLR registers

IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

//Configure the IWDG prescaler

IWDG_SetPrescaler(IWDG_Prescaler_16); //10k

//Configure the IWDG counter value

IWDG_SetReload(2500); // Bits11:0 RL[11:0]: Watchdog counter reload value ~ Only 12bit ~max value = 4096

IWDG_ReloadCounter();

IWDG_Enable();

}

void sysconfig(void)
{
	delay_init();
// LED_Init();
	IWDG_Init();								 // 看门狗初始化
	NVIC_Configuration();        // 系统中断向量配置
	RCC_Configuration();         // 系统时钟配置
	ADC1_Configuration();        // ADC1配置-心率传感器
	ADC2_Configuration();	  	   // ADC2配置-体温传感器
	uart_init(115200);           // 串口1
//  uart3_init(9600);            // 串口3
	TIM3_Int_Init(1999,55);      // 定时器3配置 周期2ms 系统56M (56MHz/(55+1))*(1999+1)=2ms
	TIM2_Configuration();        // 定时器2配置 10ms 56/56*1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);	//TIM3 重新开时钟，开始计时 
	TIM_Cmd(TIM2, ENABLE);    	 // 定时器2启动
}

int main(void)
{
  sysconfig();
	
	while (1)
  {
      sendDataToProcessing('S', Signal); // send Processing the raw Pulse Sensor data	
		if (QS == true)
		{
	  	sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
			sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix			
	    sendDatatemp('T',tempvalue/100); //发送温度值
			IWDG_ReloadCounter();            //喂狗狗
		  QS = false;                      // reset the Quantified Self flag for next time	
		}		
			delay_ms(20);	
			Reduced_Temperature();           // 体温采集
	}
}





