#include <reg51.h>
#include <intrins.h> 
#define uchar unsigned char
#define   Data_0_time    4
#define uint unsigned int

typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  无符号8位整型变量  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  有符号8位整型变量  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  无符号16位整型变量 */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  有符号16位整型变量 */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  无符号32位整型变量 */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  有符号32位整型变量 */
typedef float          F32;      /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         F64;      /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */ 

//-----------------------------------------------------------------------------
// 全局变量
uchar LED[8];	//用于LED的8位显示缓存
U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;
U8  outdata[5];  //定义发送的字节数	   
U8  indata[5];
U8  count, count_r=0;
U16 U16temp1,U16temp2;

 //-----------------------------------------------------------------------------
// 函数原形定义

void main (void);					// 主函数
void LED4_Display (void);			// LED显示
void LED_OUT(uchar X);				// LED单字节串行移位函数

unsigned char code LED_0F[];		// LED字模表

sbit DIO = P0^2;				//串行数据输入																																																
sbit RCLK  = P0^1;				//时钟脉冲信号——上升沿有效
sbit SCLK = P0^0;				//打入信号————上升沿有效
sbit dht  = P0^3 ;	   //DHT-11	温度传感器
sbit fire = P3^7;	   //气体传感器
sbit bi =P3^6;		   //蜂鸣器	   
sbit CLK=P2^2;   //时钟信号
sbit ST=P2^1;   //启动信号
sbit EOC=P2^3;   //转换结束信号
sbit OE=P2^0;  //输出使能
sbit key=P3^3;
sbit rain=P2^7;
sbit light=P2^6;
sbit led=P2^5;	   
//****************************************//

void Delay(U16 j)
{      U8 i;
    for(;j>0;j--)
  { 	
	for(i=0;i<27;i++);

  }
}

void Delay_10us(void)
{
	U8 i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}
	
    void  COM(void)
  {
 
        U8 i;
      
 for(i=0;i<8;i++)	   
    {
	
   	    U8FLAG=2;	
   	while((!dht)&&U8FLAG++);
		Delay_10us();
	    Delay_10us();
		Delay_10us();
  		U8temp=0;
     if(dht)U8temp=1;
	    U8FLAG=2;
	 while((dht)&&U8FLAG++);
   	//超时则跳出for循环		  
   	 if(U8FLAG==1)break;
   	//判断数据位是0还是1	 
   	   
	// 如果高电平高过预定0高电平值则数据位为 1 
   	 
	   U8comdata<<=1;
   	   U8comdata|=U8temp;        //0
     }//rof
   
}

	//--------------------------------
	//-----湿度读取子程序 ------------
	//--------------------------------
	//----以下变量均为全局变量--------
	//----温度高8位== U8T_data_H------
	//----温度低8位== U8T_data_L------
	//----湿度高8位== U8RH_data_H-----
	//----湿度低8位== U8RH_data_L-----
	//----校验 8位 == U8checkdata-----
	//----调用相关子程序如下----------
	//---- Delay();, Delay_10us();,COM(); 
	//--------------------------------

void RH(void)
{
  //单片机拉低18ms 
   dht=0;
   Delay(180);
   dht=1;
 //总线由上拉电阻拉高 单片机延时20us
   Delay_10us();
   Delay_10us();
   Delay_10us();
   Delay_10us();
 //主机设为输入 判断从机响应信号 
   dht=1;
 //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
   if(!dht)		 //T !	  
   {
   U8FLAG=2;
 //判断从机是否发出 80us 的低电平响应信号是否结束	 
   while((!dht)&&U8FLAG++);
   U8FLAG=2;
 //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
   while((dht)&&U8FLAG++);
 //数据接收状态		 
   COM();
   U8RH_data_H_temp=U8comdata;
   COM();
   U8RH_data_L_temp=U8comdata;
   COM();
   U8T_data_H_temp=U8comdata;
   COM();
   U8T_data_L_temp=U8comdata;
   COM();
   U8checkdata_temp=U8comdata;
   dht=1;
 //数据校验 
 
   U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
   if(U8temp==U8checkdata_temp)
   {
   	  U8RH_data_H=U8RH_data_H_temp;
   	  U8RH_data_L=U8RH_data_L_temp;
	  U8T_data_H=U8T_data_H_temp;
   	  U8T_data_L=U8T_data_L_temp;
   	  U8checkdata=U8checkdata_temp;
   }//fi
   }//fi

}
//*****************************************************************************
// 主程序
//
void main (void) 
{
    int temp,flag=0;

	TMOD=0x02; //T1 工作模式 2
	TH0=0xFE;
	TL0=0xFE;
	IE=0x82;
	TR0=1;

	while(1)
	{

    	ST=0;ST=1;ST=0;    //启动 A/D 转换
        while(EOC==0);     //等待转换完成
        OE=1; 

	  temp=P1;
	  if(temp<15)					   //降雨量的量化
	  temp=10;
	  else if(15<temp&&temp<25)
	  temp=20;
	  else if(25<temp&&temp<35)
	  temp=30;
	  else if(35<temp)
	  temp=40;
	   
	    OE=0;
	 
	   if(key==0)
	 {     
	 		Delay(1000);		   //消除抖动
			if(key==0)
			  flag++;
	 }		  

	  if(flag%2==0)
	  {
		   RH();					//温湿度读取函数
		   LED[3]=U8RH_data_H/10;
		   LED[2]=U8RH_data_H%10;
		   LED[1]=U8T_data_H/10;
		   LED[0]=U8T_data_H%10;   //数码管显示湿度温度
		   LED4_Display ();
	   }
	   else
	   {
	     LED[2]=16;
	     LED[3]=light*5;	 //光强	（0是白天 5是黑夜），夜间自动开灯
	     LED[0]=16;
	     LED[1]=temp/10;     //雨量（4个等级）
	     LED4_Display ();
	   }
										
	   if(fire==0||rain==0)		 //发生火灾或降雨量过大
	     {bi=0;}				 //蜂鸣器报警
	   if(light==1)				 //如果在夜间
	    {led=0;}				 //LED点亮
	   else						//正常情况下都关闭
	     {bi=1;led=1;}
	} 
}
	 
void Timer0_INT() interrupt 1
{
	CLK=~CLK;						  //ADC0809需要500KHz的信号
}

void LED4_Display (void)
{
	unsigned char code *led_table;          // 查表指针
	uchar i;
	//显示第1位
	led_table = LED_0F + LED[0];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x01);		

	RCLK = 0;
	RCLK = 1;
	//显示第2位
	led_table = LED_0F + LED[1];
	i = *led_table;

	LED_OUT(i);		
	LED_OUT(0x02);		

	RCLK = 0;
	RCLK = 1;
	//显示第3位
	led_table = LED_0F + LED[2];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x04);	

	RCLK = 0;
	RCLK = 1;
	//显示第4位
	led_table = LED_0F + LED[3];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x08);		

	RCLK = 0;
	RCLK = 1;
}

void LED_OUT(uchar X)
{
	uchar i;
	for(i=8;i>=1;i--)
	{
		if (X&0x80) DIO=1; else DIO=0;
		X<<=1;
		SCLK = 0;
		SCLK = 1;
	}
}

unsigned char code LED_0F[] = 
{// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};



