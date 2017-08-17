#include <reg51.h>
#include <intrins.h> 
#define uchar unsigned char
#define   Data_0_time    4
#define uint unsigned int

typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  �޷���8λ���ͱ���  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  �з���8λ���ͱ���  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  �޷���16λ���ͱ��� */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  �з���16λ���ͱ��� */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  �޷���32λ���ͱ��� */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  �з���32λ���ͱ��� */
typedef float          F32;      /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         F64;      /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */ 

//-----------------------------------------------------------------------------
// ȫ�ֱ���
uchar LED[8];	//����LED��8λ��ʾ����
U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;
U8  outdata[5];  //���巢�͵��ֽ���	   
U8  indata[5];
U8  count, count_r=0;
U16 U16temp1,U16temp2;

 //-----------------------------------------------------------------------------
// ����ԭ�ζ���

void main (void);					// ������
void LED4_Display (void);			// LED��ʾ
void LED_OUT(uchar X);				// LED���ֽڴ�����λ����

unsigned char code LED_0F[];		// LED��ģ��

sbit DIO = P0^2;				//������������																																																
sbit RCLK  = P0^1;				//ʱ�������źš�����������Ч
sbit SCLK = P0^0;				//�����źš���������������Ч
sbit dht  = P0^3 ;	   //DHT-11	�¶ȴ�����
sbit fire = P3^7;	   //���崫����
sbit bi =P3^6;		   //������	   
sbit CLK=P2^2;   //ʱ���ź�
sbit ST=P2^1;   //�����ź�
sbit EOC=P2^3;   //ת�������ź�
sbit OE=P2^0;  //���ʹ��
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
   	//��ʱ������forѭ��		  
   	 if(U8FLAG==1)break;
   	//�ж�����λ��0����1	 
   	   
	// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
   	 
	   U8comdata<<=1;
   	   U8comdata|=U8temp;        //0
     }//rof
   
}

	//--------------------------------
	//-----ʪ�ȶ�ȡ�ӳ��� ------------
	//--------------------------------
	//----���±�����Ϊȫ�ֱ���--------
	//----�¶ȸ�8λ== U8T_data_H------
	//----�¶ȵ�8λ== U8T_data_L------
	//----ʪ�ȸ�8λ== U8RH_data_H-----
	//----ʪ�ȵ�8λ== U8RH_data_L-----
	//----У�� 8λ == U8checkdata-----
	//----��������ӳ�������----------
	//---- Delay();, Delay_10us();,COM(); 
	//--------------------------------

void RH(void)
{
  //��Ƭ������18ms 
   dht=0;
   Delay(180);
   dht=1;
 //������������������ ��Ƭ����ʱ20us
   Delay_10us();
   Delay_10us();
   Delay_10us();
   Delay_10us();
 //������Ϊ���� �жϴӻ���Ӧ�ź� 
   dht=1;
 //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
   if(!dht)		 //T !	  
   {
   U8FLAG=2;
 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
   while((!dht)&&U8FLAG++);
   U8FLAG=2;
 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
   while((dht)&&U8FLAG++);
 //���ݽ���״̬		 
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
 //����У�� 
 
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
// ������
//
void main (void) 
{
    int temp,flag=0;

	TMOD=0x02; //T1 ����ģʽ 2
	TH0=0xFE;
	TL0=0xFE;
	IE=0x82;
	TR0=1;

	while(1)
	{

    	ST=0;ST=1;ST=0;    //���� A/D ת��
        while(EOC==0);     //�ȴ�ת�����
        OE=1; 

	  temp=P1;
	  if(temp<15)					   //������������
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
	 		Delay(1000);		   //��������
			if(key==0)
			  flag++;
	 }		  

	  if(flag%2==0)
	  {
		   RH();					//��ʪ�ȶ�ȡ����
		   LED[3]=U8RH_data_H/10;
		   LED[2]=U8RH_data_H%10;
		   LED[1]=U8T_data_H/10;
		   LED[0]=U8T_data_H%10;   //�������ʾʪ���¶�
		   LED4_Display ();
	   }
	   else
	   {
	     LED[2]=16;
	     LED[3]=light*5;	 //��ǿ	��0�ǰ��� 5�Ǻ�ҹ����ҹ���Զ�����
	     LED[0]=16;
	     LED[1]=temp/10;     //������4���ȼ���
	     LED4_Display ();
	   }
										
	   if(fire==0||rain==0)		 //�������ֻ���������
	     {bi=0;}				 //����������
	   if(light==1)				 //�����ҹ��
	    {led=0;}				 //LED����
	   else						//��������¶��ر�
	     {bi=1;led=1;}
	} 
}
	 
void Timer0_INT() interrupt 1
{
	CLK=~CLK;						  //ADC0809��Ҫ500KHz���ź�
}

void LED4_Display (void)
{
	unsigned char code *led_table;          // ���ָ��
	uchar i;
	//��ʾ��1λ
	led_table = LED_0F + LED[0];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x01);		

	RCLK = 0;
	RCLK = 1;
	//��ʾ��2λ
	led_table = LED_0F + LED[1];
	i = *led_table;

	LED_OUT(i);		
	LED_OUT(0x02);		

	RCLK = 0;
	RCLK = 1;
	//��ʾ��3λ
	led_table = LED_0F + LED[2];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x04);	

	RCLK = 0;
	RCLK = 1;
	//��ʾ��4λ
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



