#include "PWM.h"
#include "led.h"
#include "usart.h"

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
//uint16_t Num;
unsigned char firstBeat = true;  // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat = false;// used to seed rate array so we startup with reaso

/*
 * ��������TIM3_Mode_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */

static void TIM3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* PWM�źŵ�ƽ����ֵ */
	//u16 CCR1_Val = 1000;     
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); 
	
  /* Time base configuration *///	2ms	 
  TIM_TimeBaseStructure.TIM_Period = 1999;       //����ʱ����0������449����Ϊ450�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler =71;	    //����Ԥ��Ƶ(x+1)��2��Ƶ����Ϊ36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;  //����������ģʽ1	(����*2)
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update); //ѡ��TRGO����ԴΪ��ʱ������ʱ��	
	//����TIM3����ADCͨ��
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

//   /* PWM1 Mode configuration: Channel1 */
//   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //����ͨ����Ч  PB1 	
//   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
//   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;   //�������
//   TIM_OC4Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��4
//   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
}

void TIM3_PWM_Init(void)
{
	TIM3_Mode_Config();	
}


//��ʱ��3�жϷ�����
//2ms
void TIM3_IRQHandler(void)
{
	uint16_t runningTotal=0;
	uint8_t i;
	uint16_t Num;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		//TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
		//��ȡ����ֵ����2λ��12λ-->10λ
		Signal = ADC_GetConversionValue(ADC1)/4; 
		//Signal=Get_Adc_Average(ADC_Channel_1,1)>>2; // read the Pulse Senso
		//Signal = ADC_GetConversionValue(ADC1)/4;
		sampleCounter += 2;                          // keep track of the time in mS with this variable
		Num = sampleCounter - lastBeatTime;          // monitor the time since the last beat to avoid noise
		//	HAL_ADC_Start(&hadc1);									 //restart ADC conversion
		
		//printf("%d \r\n",Signal);	
		
		//�������岨�Ĳ���Ͳ���
		//  find the peak and trough of the pulse wave
		if(Signal < thresh && Num > (IBI/5)*3){   // avoid dichrotic noise by waiting 3/5 of last IBI
			if (Signal < T){                        // T is the trough
				T = Signal;                           // keep track of lowest point in pulse wave 
			}
		}

		if(Signal > thresh && Signal > P){        // thresh condition helps avoid noise
			P = Signal;                             // P is the peak
		}                                         // keep track of highest point in pulse wave

  //��ʼѰ������
	//���������ٵ�ʱ��signal��ֵ������
	//  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (Num > 250){                                 // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (Num > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
      LED0=0; 
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;                  // clear secondBeat flag
        for(i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                     //rate[0]-rate[9]��ֵ��һ���𣿣�����
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        return;                              // IBI value is unreliable so discard it
      }   

      // keep a running total of the last 10 IBI values
     // runningTotal = 0;                  			// clear the runningTotal variable    

      for(i=0; i<=8; i++){                // shift data in the rate array
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

	//���忪ʼ�½�
  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
    LED0=1; 															 //����
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  //û�м�⵽���壬����Ĭ��ֵ
	if (Num > 2500){                         // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
  }

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}