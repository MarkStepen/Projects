#include "stm32f10x.h"

u8 ESP8266_Receive_Number=0;
u8 Recv_Wifi_Buff[120];                                             // wifi��������

void Usart_TransData(USART_TypeDef* USARTx,u8 data)					//���ڷ��ͺ���,���ֽ�
{
	USARTx->DR=data;
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);  	//�����Ѿ���ת�Ƶ���λ�Ĵ���	
	USART_ClearFlag(USART1,USART_FLAG_TC);
}
void USART1_Configuration(void)
{
   USART_InitTypeDef  USART_InitStructure;							//����USART��ʼ���ṹ��USART_InitStructure
	GPIO_InitTypeDef GPIO_InitStructure;							//����GPIO��ʼ���ṹ��GPIO_InitStructure 

    
    NVIC_InitTypeDef NVIC_InitStructure;							//����NVIC��ʼ������NVIC_InitStructure  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 				//ѡ��NVIC���ȼ�����0 

	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				//���ڽ������ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   
    NVIC_Init(&NVIC_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;						//����USART1��Tx��(PA.9)Ϊ�ڶ������������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						//����USART1��Rx��(PA.10)Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate =115200;					//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ʹ�ܷ��ͺͽ���ģʽ	
	USART_Init(USART1, &USART_InitStructure);						//����USART_InitStruct��ָ���Ĳ�����ʼ������USART1�Ĵ���

	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);					//ʧ��USART1�ж� 					
    USART_Cmd(USART1, ENABLE);										//ʹ��USART1
	USART_ClearFlag(USART1,USART_FLAG_TC);
}
void USART2_Configuration(void)
{
   USART_InitTypeDef  USART_InitStructure;							//����USART��ʼ���ṹ��USART_InitStructure
	GPIO_InitTypeDef GPIO_InitStructure;							   //����GPIO��ʼ���ṹ��GPIO_InitStructure 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;						//����USART2��Tx��(PA.2)Ϊ�ڶ������������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;						//����USART2��Rx��(PA.3)Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;					                    // ����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		                // 8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			                // 1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				                // ��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                // ʹ�ܷ��ͺͽ���ģʽ	
	USART_Init(USART2, &USART_InitStructure);						                // ����USART_InitStruct��ָ���Ĳ�����ʼ������USART1�Ĵ���

	USART_ITConfig(USART2, USART_IT_RXNE,DISABLE);					                // ʧ��USART1�ж�					
    USART_Cmd(USART2, ENABLE);										                // ʹ��USART1
	USART_ClearFlag(USART2,USART_FLAG_TC);
}
void USART3_Configuration(void)
{
   USART_InitTypeDef  USART_InitStructure;							// ����USART��ʼ���ṹ��USART_InitStructure
	GPIO_InitTypeDef GPIO_InitStructure;							// ����GPIO��ʼ���ṹ��GPIO_InitStructure 
   
   NVIC_InitTypeDef NVIC_InitStructure;							    // ����NVIC��ʼ������NVIC_InitStructure  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 				// ѡ��NVIC���ȼ�����0 

	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;				// ���ڽ������ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   
    NVIC_Init(&NVIC_InitStructure);
//    
//    GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE); 
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						//����USART3��Tx��(PB.10)Ϊ�ڶ������������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;						                 // ����USART3��Rx��(PB.11)Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;					                     // ����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		                 // 8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			                 // 1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				                 // ��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	 // Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                 // ʹ�ܷ��ͺͽ���ģʽ	
	USART_Init(USART3, &USART_InitStructure);						                 // ����USART_InitStruct��ָ���Ĳ�����ʼ������USART1�Ĵ���

	USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);					                 // ʹ��USART1�ж�					
    USART_Cmd(USART3, ENABLE);										                 // ʹ��USART1
	USART_ClearFlag(USART3,USART_FLAG_TC);
}
void UART4_Configuration(void)
{
    USART_InitTypeDef  USART_InitStructure;							                 // ����USART��ʼ���ṹ��USART_InitStructure
	GPIO_InitTypeDef GPIO_InitStructure;							                 // ����GPIO��ʼ���ṹ��GPIO_InitStructure  
   
   NVIC_InitTypeDef NVIC_InitStructure;							                     // ����NVIC��ʼ������NVIC_InitStructure  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 				                 // ѡ��NVIC���ȼ�����0 

	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;				                 // ���ڽ������ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						                // ����USART1��Tx��(PA.9)Ϊ�ڶ������������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;						//����USART1��Rx��(PA.10)Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;					//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ʹ�ܷ��ͺͽ���ģʽ	
	USART_Init(UART4, &USART_InitStructure);						//����USART_InitStruct��ָ���Ĳ�����ʼ������USART1�Ĵ���

	USART_ITConfig(UART4, USART_IT_RXNE,ENABLE);					//ʹ��USART1�ж�					
   USART_Cmd(UART4, ENABLE);										//ʹ��USART1
	USART_ClearFlag(UART4,USART_FLAG_TC);
}




int fputc(int ch, FILE *f)
{
	/* ��Printf���ݷ������� */
	USART_SendData(USART1, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
	return ch;
}

void USARTWrite(USART_TypeDef* USARTx,char* data,u16 len)				//���ڷ��ͺ���,�ַ���
{
	u16 i;
	for (i=0; i<len; i++)
	{
		Usart_TransData(USARTx,data[i]);
	}
}

void USARTWriteData(USART_TypeDef* USARTx,u16 data)				//���ڷ��ͺ���,�ַ���
{
    if(data>999 && data<10000)
    {
        Usart_TransData(USARTx,data/1000+'0');
        Usart_TransData(USARTx,data%1000/100+'0');
        Usart_TransData(USARTx,data%1000%100/10+'0');
        Usart_TransData(USARTx,data%1000%100%10+'0');
        Usart_TransData(USARTx,0x0D);
        Usart_TransData(USARTx,0x0A);
    }else if(data>99 && data<1000)
    {
        Usart_TransData(USARTx,data/100+'0');
        Usart_TransData(USARTx,data%100/10+'0');
        Usart_TransData(USARTx,data%100%10+'0');
        Usart_TransData(USARTx,0x0D);
        Usart_TransData(USARTx,0x0A);
    }
}

u8 Compare_String(u8* Str1,u8* Str2,u16 Str1_Start,u16 Str2_Start,u16 StrLong)
{
	u16 CMP_Count,Same_Char_Count=0;
	
	for(CMP_Count=0;CMP_Count < StrLong;CMP_Count++)
	{
		if( Str1[CMP_Count+Str1_Start] == Str2[CMP_Count+Str2_Start] )
		{
			Same_Char_Count++;
		}
	}
	if(Same_Char_Count >= StrLong)
	{
		Same_Char_Count=0;
		return 1;
	}
	else
	{
		Same_Char_Count=0;
		return 0; 
	} 
}

