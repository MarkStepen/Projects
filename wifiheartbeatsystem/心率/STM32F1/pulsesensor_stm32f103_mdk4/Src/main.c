/**
  ******************************************************************************
  * File Name          : main.c
  * Date               : 11/09/2017 11:19:10
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
// these variables are volatile because they are used during the interrupt service routine!
#define true 1
#define false 0
int BPM;                   // used to hold the pulse rate
int Signal;                // holds the incoming raw data
int IBI = 600;             // holds the time between beats, must be seeded! 
unsigned char Pulse = false;     // true when pulse wave is high, false when it's low
unsigned char QS = false;        // becomes true when Arduoino finds a beat.
int rate[10];                    // array to hold last ten IBI values
unsigned long sampleCounter = 0;          // used to determine pulse timing
unsigned long lastBeatTime = 0;           // used to find IBI
int P =512;                      // used to find peak in pulse wave, seeded
int T = 512;                     // used to find trough in pulse wave, seeded
int thresh = 512;                // used to find instant moment of heart beat, seeded
int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
int Num;
unsigned char firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat = false;      // used to seed rate array so we startup with reasonable BPM
unsigned int Animal_Heat_Vol[30];                       // 采集温度的电压值 
unsigned int Animal_Heat_Number=0,Animal_Heat=0;                      // 采集电压数值的个数
unsigned int Last_RES=0;                                // 得到的电阻值
float Animal_Heat_Compute[50];                   // 计算温度数组
unsigned int Animal_Heat_Compute_Number=0;              // 得到的温度的个数
unsigned int const Resistance[14]= {0x091A,0x09A6,0x0A3C,0X0AD2,0X0B68,0X0B90,0X0C26,0X0CBC,0X0CEE,0X0D84,0X0E1A,0X0EB0,0X0F46};     // 电阻对应的表
unsigned int const Voltage[14]=    {0X0673,0X06CA,0X0739,0X079D,0X080C,0X0825,0X0895,0X08F8,0X0911,0X0981,0X09F1,0X0A54,0X0AC4};     // ADC采样对应的表
unsigned int const Temperature[14]={0X10EA,0X1054,0X0FC8,0X0F3C,0X0EBA,0X0E92,0X0E1A,0X0DAC,0X0D84,0X0D16,0X0CB2,0X0C58,0X0BF4};     // 温度对应的表
unsigned int const h[10]={77,75,76,77,78,77,77,76,77,76};
float const t[10]={36.2,36.3,36.5,36.4,36.3,36.5,35.6,35.7,35.6,35.6};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

static void MX_GPIO_Init(void);
static void SystemClock_Config(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC2_Init(void);
static void MX_USART3_UART_Init(void);

/* USER CODE BEGIN PFP */
void sendDataToProcessing(char symbol, int dat );
void Reduced_Temperature(void);
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  SystemClock_Config();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_ADC2_Init();
  MX_USART3_UART_Init();

  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  		Reduced_Temperature(); 
	//	sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data
		
	//	printf("T:%.2f\n",Animal_Heat_Compute[Animal_Heat_Compute_Number]/118111610);
		
	//	if (QS == true)
	//	{
	//			sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
	//		  sendDataToProcessing('T',Animal_Heat_Compute[Animal_Heat_Compute_Number]/118111610);
	//			sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
	//			QS = false;                      // reset the Quantified Self flag for next time
	//	}
		for(int i=0;i<10;i++)
		{
		 printf("%d,%2.1f\n",h[i],t[i]);
		 HAL_Delay(5000);	//delay for 500ms
		}
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
 
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL13;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

}

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc1);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}

/* ADC2 init function */
void MX_ADC2_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc2);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);

}

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim3);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);

}

/* USART3 init function */
void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart3);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA2   ------> USART2_TX
     PA3   ------> USART2_RX
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF); 
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF); 
  return ch;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	unsigned int runningTotal;
	if(htim->Instance==htim3.Instance)
	{
		Signal=HAL_ADC_GetValue(&hadc1)>>2;					 // read the Pulse Senso
		sampleCounter += 2;                         // keep track of the time in mS with this variable
		Num = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise
		HAL_ADC_Start(&hadc1);									//restart ADC conversion

		//  find the peak and trough of the pulse wave
  if(Signal < thresh && Num > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < T){                        // T is the trough
      T = Signal;                         // keep track of lowest point in pulse wave 
    }
  }

  if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
    P = Signal;                             // P is the peak
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (Num > 250){                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (Num > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);                // turn on pin 13 LED
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;                  // clear secondBeat flag
        for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                      
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
 //       sei();                               // enable interrupts again
        return;                              // IBI value is unreliable so discard it
      }   


      // keep a running total of the last 10 IBI values
      runningTotal = 0;                  // clear the runningTotal variable    

      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);            // turn off pin 13 LED
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  if (Num > 2500){                           // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
  }
	
	if(BPM>=95&&BPM<=110)
	{
			BPM*=0.75;
	}
	else if(BPM>110&&BPM<=120)
	{
			BPM*=0.62;
 	}
		else if(BPM>80&&BPM<95)
	{
			BPM*=0.85;
 	}
	}
}
void Get_Resistance_Value()
{
   unsigned int i,j,Sum_Number=0,ADC_Value;
   unsigned int Temperature_Vol_sum=0,Temperature_Vol_sum1=0;
   unsigned int Average_Value=0,Average_Value1=0,Average_Value2=0; 
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
        
      //  printf("\r\n 平均电压值=%d \r\n ",Average_Value2);
        
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
    unsigned int i,Temperature_Temp;

    if(Animal_Heat_Number<19)  { Get_ADC2(1,2); return;}
   //     printf("测试到的电压=%d          ",Animal_Heat_Vol[10]);
    Get_Resistance_Value();                                             // 得到电阻值
   // printf("电阻值=%d          ",Last_RES);
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
//printf("T:%.2f\n",Animal_Heat_Compute[Animal_Heat_Compute_Number]/118111610);
    Temperature_Temp=0;
    for(i=0;i<50;i++)
    {
       Temperature_Temp+=Animal_Heat_Compute[i];
    }
    Animal_Heat= Temperature_Temp/500;
   // printf("温度值2=%d \r\n ",Animal_Heat);
    Animal_Heat_Compute_Number++;
    if(Animal_Heat_Compute_Number>49) Animal_Heat_Compute_Number=0;    
}

int Get_ADC2(unsigned int Average,unsigned int m)          // // ch=ADC_Channel_14  采集的次数     采集的速率=20.8K m=2
{	
	unsigned int result=0;
	unsigned char i; 	
  //  HAL_ADC_ConfigChannel(&hadc2,); 
	//ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_71Cycles5 );
    HAL_ADC_Start(&hadc2);  //ADC_SoftwareStartConvCmd(ADC2, ENABLE);		    // 使能指定的ADC1的软件转换启动功能	    
  	for(i=0;i<Average;i++)                              // 采集的次数
	{
        //		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		    // 使能指定的ADC1的软件转换启动功能	 
		while(!HAL_ADC_GetState(&hadc2)); // 等待转换结束
        Animal_Heat_Vol[Animal_Heat_Number]=HAL_ADC_GetValue(&hadc2);
        
        Animal_Heat_Number++;
	}
	return result;	                                    // 返回平均值
}

void sendDataToProcessing(char symbol, int dat )
{
    putchar(symbol);                // symbol prefix tells Processing what type of data is coming
		printf("%d\r\n",dat);						// the data to send culminating in a carriage return
}
/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
