#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

void ADC1_Configuration(void);
void ADC2_Configuration(void);
u16 Get_ADC2(u8 ch,u16 Average,u8 m);
//u16  Get_Adc(u8 ch); 
//u16 Get_Adc_Average(u8 ch,u8 times); 
 
#endif 
