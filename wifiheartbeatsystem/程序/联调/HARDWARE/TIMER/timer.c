#include "timer.h"
#include "led.h"
#include "usart.h"
#include "stm32f10x.h"

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

/*
 * ��������TIM3_Int_Init
 * ����  ������TIM3
 * ����  ��arr, psc
 * ���  ����
 * ����  ���ⲿ����
 */

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); 
	 
  TIM_TimeBaseStructure.TIM_Period        = arr;      
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;	    
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);	/*ѡ��update event��ΪTRGO,����TIM3����ADCͨ�� */
	//ÿ����ʱ���ڽ����󴥷�һ��
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM3, ENABLE);                 
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE); 
	/*�ȹرյȴ�ʹ��*/ 
}

//��ʱ��3�жϷ�����
 /*��Դ���ɿ�ԴӲ���ṩ*/
void TIM3_IRQHandler(void)
{
	uint16_t runningTotal=0;
	uint8_t i;
	uint16_t Num;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		//��ȡ����ֵ����2λ��12λ-->10λ
		Signal = ADC_GetConversionValue(ADC1)/4;     // read the Pulse Senso
		//Signal=Get_Adc_Average(ADC_Channel_1,1)>>2; 
		sampleCounter += 2;                          // keep track of the time in mS with this variable
		Num = sampleCounter - lastBeatTime;          // monitor the time since the last beat to avoid noise	
		
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
        for(i=0; i<=9; i++){                 // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                     //rate[0]-rate[9]��ֵ��һ���𣿣�����
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        return;                              // IBI value is unreliable so discard it
      }   

      // keep a running total of the last 10 IBI values
     // runningTotal = 0;                  		// clear the runningTotal variable    

      for(i=0; i<=8; i++){                    // shift data in the rate array
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

void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;			    // ����TIM_TimeBase��ʼ���ṹ��TIM_TimeBaseStructure
	TIM_OCInitTypeDef			TIM_OCInitStructure;			    // ����TIM_OCInit��ʼ���ṹ��TIM_OCInitStructure
	NVIC_InitTypeDef NVIC_InitStructure;						    // ����NVIC��ʼ������NVIC_InitStructure     
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 			    // ѡ��NVIC���ȼ�����0 

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				    // ʹ��TIM2ȫ���ж�,0����ռ���ȼ�,3����ռ���Ȼ�	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       // �趨��ԱNVIC_IRQChannel�е���ռ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;              // �趨��ԱNVIC_IRQChannel�еĴ����ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 // ʹ�ܳ�ԱNVIC_IRQChannel
	NVIC_Init(&NVIC_InitStructure);	                                // ����NCIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_TimeBaseStructure.TIM_Period=65535;		 				    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ)������������һ�������¼�װ�����Զ���װ�ڽ��������ڵ�ֵ�� */
    TIM_TimeBaseStructure.TIM_Prescaler=0;						    /* ʱ��Ԥ��Ƶ����������������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ��(��PSC�Ĵ�������)*/
    TIM_TimeBaseStructure.TIM_ClockDivision=0; 					    /* ʱ�ӷָ� */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	    /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

	TIM_PrescalerConfig(TIM2,5599,TIM_PSCReloadMode_Immediate);     // ����TIM2Ԥ��Ƶ(��仰����˼�ǰ�Ԥ��Ƶֵ��ʱ��װ��Ԥ��Ƶ����)�������ʱ��ʱ�ӷ���72MHz/(7199+1)=10KHz(��ʱ��ÿ�μ���ʱ����Ϊ1/10ms)��7199ΪԤ��Ƶֵ((��PSC�Ĵ�������))��(TIM_PSCReloadMode_ImmediateλԤ��Ƶֵ��ʱװ��ģʽ)
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_Timing;			    // ѡ��ʱ��ģʽΪTIM����Ƚ�ʱ��ģʽ(ѡ��ʱ��ģʽ)
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	    // ʹ������Ƚ�״̬
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;		    // TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse=100;							    // �����˴�װ�벶��ȽϼĴ���������ֵ
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);						    // ����TIM_OCInitStruct��ָ���Ĳ�����ʼ��TIM2ͨ��1
	TIM_OCInitStructure.TIM_Pulse=10000;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse=40;
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Disable);			     // ʧ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Disable);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Disable);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);						     // ��������¼���־λ(��ֹ�������ж�)
	TIM_ITConfig(TIM2,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3,ENABLE);	     // ʹ��TIM2�жϣ�ʹ�ܲ���/�Ƚ�1,2,3�ж�Դ�� 						  		
}

void TIM2_IRQHandler(void) 
{ 
	vu16 capture=0;	    // ��ǰ�������ֵ�ֲ����� 

	if(TIM_GetITStatus(TIM2,TIM_IT_CC1)!=RESET)			            // ���TIM2��TIM����/�Ƚ�1�ж�Դ����10ms
	{

        capture=TIM_GetCapture1(TIM2);				                 // ��ȡ��ǰ����ֵ
        TIM_SetCompare1(TIM2,capture+100);			                 // ���ݵ�ǰ����ֵ�����������Ĵ�������Ϊ�Ƕ�ʱ���ļ����������ϼ���(ÿ��һ������ʱ����1/10ms)��
        TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);                      // 10ms
	}else if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)
	{  
        capture=TIM_GetCapture2(TIM2);                               // ���TIMx���벶��2��ֵ
        TIM_SetCompare2(TIM2,capture+10000);                         // ����TIMx����Ƚ�2�Ĵ���ֵ
        TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);	                     // 1s	
	}
	else if(TIM_GetITStatus(TIM2,TIM_IT_CC3)!=RESET)                  // 4ms   
	{       
   
        capture=TIM_GetCapture3(TIM2);
        TIM_SetCompare3(TIM2,capture+40);                             // 4ms�Ķ�ʱ(����TIMx����Ƚ�3�Ĵ���ֵ)
        TIM_ClearITPendingBit(TIM2,TIM_IT_CC3);	
	}
}
