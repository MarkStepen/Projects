#include "stm32f10x.h"

u16    Animal_Heat_Vol[30];                       // 采集温度的电压值 
u8     Animal_Heat_Number=0;                      // 采集电压数值的个数
u16    Last_RES=0;                                // 得到的电阻值
float    Animal_Heat_Compute[50];                   // 计算温度数组
u8     Animal_Heat_Compute_Number=0;              // 得到的温度的个数
u16   const Resistance[14]= {0x091A,0x09A6,0x0A3C,0X0AD2,0X0B68,0X0B90,0X0C26,0X0CBC,0X0CEE,0X0D84,0X0E1A,0X0EB0,0X0F46};     // 电阻对应的表
u16   const Voltage[14]=    {0X0673,0X06CA,0X0739,0X079D,0X080C,0X0825,0X0895,0X08F8,0X0911,0X0981,0X09F1,0X0A54,0X0AC4};     // ADC采样对应的表
u16   const Temperature[14]={0X10EA,0X1054,0X0FC8,0X0F3C,0X0EBA,0X0E92,0X0E1A,0X0DAC,0X0D84,0X0D16,0X0CB2,0X0C58,0X0BF4};     // 温度对应的表
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
    u8 i; u32 Temperature_Temp;

    if(Animal_Heat_Number<19)  { Get_ADC2(ADC_Channel_1,1,2); return;}
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
    printf("T:%.2f\n",Animal_Heat_Compute[Animal_Heat_Compute_Number]/100);
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






