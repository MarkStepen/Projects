#include "stm32f10x.h"

u8 ESP8266_Receive_Number=0;
u8 Recv_Wifi_Buff[120];                                             // wifi接收数组

void Usart_TransData(USART_TypeDef* USARTx,u8 data)					//串口发送函数,单字节
{
	USARTx->DR=data;
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);  	//数据已经被转移到移位寄存器	
	USART_ClearFlag(USART1,USART_FLAG_TC);
}
void USART1_Configuration(void)
{
   USART_InitTypeDef  USART_InitStructure;							//定义USART初始化结构体USART_InitStructure
	GPIO_InitTypeDef GPIO_InitStructure;							//定义GPIO初始化结构体GPIO_InitStructure 

    
    NVIC_InitTypeDef NVIC_InitStructure;							//定义NVIC初始化结体NVIC_InitStructure  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 				//选择NVIC优先级分组0 

	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				//串口接收终中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   
    NVIC_Init(&NVIC_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;						//设置USART1的Tx脚(PA.9)为第二功能推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						//设置USART1的Rx脚(PA.10)为悬空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate =115200;					//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//使能发送和接收模式	
	USART_Init(USART1, &USART_InitStructure);						//根据USART_InitStruct中指定的参数初始化外设USART1寄存器

	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);					//失能USART1中断 					
    USART_Cmd(USART1, ENABLE);										//使能USART1
	USART_ClearFlag(USART1,USART_FLAG_TC);
}
void USART2_Configuration(void)
{
   USART_InitTypeDef  USART_InitStructure;							//定义USART初始化结构体USART_InitStructure
	GPIO_InitTypeDef GPIO_InitStructure;							   //定义GPIO初始化结构体GPIO_InitStructure 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;						//设置USART2的Tx脚(PA.2)为第二功能推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;						//设置USART2的Rx脚(PA.3)为悬空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;					                    // 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		                // 8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			                // 1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;				                // 奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                // 使能发送和接收模式	
	USART_Init(USART2, &USART_InitStructure);						                // 根据USART_InitStruct中指定的参数初始化外设USART1寄存器

	USART_ITConfig(USART2, USART_IT_RXNE,DISABLE);					                // 失能USART1中断					
    USART_Cmd(USART2, ENABLE);										                // 使能USART1
	USART_ClearFlag(USART2,USART_FLAG_TC);
}
void USART3_Configuration(void)
{
   USART_InitTypeDef  USART_InitStructure;							// 定义USART初始化结构体USART_InitStructure
	GPIO_InitTypeDef GPIO_InitStructure;							// 定义GPIO初始化结构体GPIO_InitStructure 
   
   NVIC_InitTypeDef NVIC_InitStructure;							    // 定义NVIC初始化结体NVIC_InitStructure  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 				// 选择NVIC优先级分组0 

	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;				// 串口接收终中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   
    NVIC_Init(&NVIC_InitStructure);
//    
//    GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE); 
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						//设置USART3的Tx脚(PB.10)为第二功能推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;						                 // 设置USART3的Rx脚(PB.11)为悬空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;					                     // 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		                 // 8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			                 // 1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;				                 // 奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	 // 硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                 // 使能发送和接收模式	
	USART_Init(USART3, &USART_InitStructure);						                 // 根据USART_InitStruct中指定的参数初始化外设USART1寄存器

	USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);					                 // 使能USART1中断					
    USART_Cmd(USART3, ENABLE);										                 // 使能USART1
	USART_ClearFlag(USART3,USART_FLAG_TC);
}
void UART4_Configuration(void)
{
    USART_InitTypeDef  USART_InitStructure;							                 // 定义USART初始化结构体USART_InitStructure
	GPIO_InitTypeDef GPIO_InitStructure;							                 // 定义GPIO初始化结构体GPIO_InitStructure  
   
   NVIC_InitTypeDef NVIC_InitStructure;							                     // 定义NVIC初始化结体NVIC_InitStructure  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 				                 // 选择NVIC优先级分组0 

	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;				                 // 串口接收终中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						                // 设置USART1的Tx脚(PA.9)为第二功能推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;						//设置USART1的Rx脚(PA.10)为悬空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;					//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//使能发送和接收模式	
	USART_Init(UART4, &USART_InitStructure);						//根据USART_InitStruct中指定的参数初始化外设USART1寄存器

	USART_ITConfig(UART4, USART_IT_RXNE,ENABLE);					//使能USART1中断					
   USART_Cmd(UART4, ENABLE);										//使能USART1
	USART_ClearFlag(UART4,USART_FLAG_TC);
}




int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART1, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
	return ch;
}

void USARTWrite(USART_TypeDef* USARTx,char* data,u16 len)				//串口发送函数,字符串
{
	u16 i;
	for (i=0; i<len; i++)
	{
		Usart_TransData(USARTx,data[i]);
	}
}

void USARTWriteData(USART_TypeDef* USARTx,u16 data)				//串口发送函数,字符串
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

