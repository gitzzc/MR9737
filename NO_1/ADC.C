#include "iostm8s003f3.h"
#include "delay.h"
#include "ADC.h"
unsigned int ReadAnalogChannel(unsigned char CH)
{
  unsigned int AD_Value;
  AD_Value=0;
  
  if(CH==0)
  {
  PD_DDR &=~0x40;     //PD6输入
  PD_CR1 &=~0x40;     //悬空
  PD_CR2 &=~0x40;     //禁止中断功能
  
  ADC_CR1 =0x00;      //f/2预分频
  ADC_CR2 =0x00;      //左对齐
  ADC_CSR =0x06;       //AIN6作为输入口
  ADC_CR1 |=0x01;
  }
  if(CH==1)
  {
  PC_DDR &=~0x10;     //PC4输入
  PC_CR1 &=~0x10;     //悬空
  PC_CR2 &=~0x10;     //禁止中断功能
  
  ADC_CR1 =0x00;      //f/2预分频
  ADC_CR2 =0x00;      //左右对齐
  ADC_CSR =0x02;       //AIN2作为输入口
  ADC_CR1 |=0x01;
  }
  delay_us();
  delay_us();
  delay_us();
  ADC_CR1 |=0x01;
  while(!(ADC_CSR&0x80)){}//等待转换结束
  AD_Value +=ADC_DRH;
  AD_Value=AD_Value<<2;
  AD_Value +=ADC_DRL;
  return AD_Value;
}

/***********************************数字滤波**********************************/
#define ADC_SUM_TIME	16          //每组平均数据由8个采样数据平均得出
unsigned int AverageADCData(unsigned char CHH)
{	
     unsigned char R7;
     unsigned char ch;
     unsigned int ADC_TotalData;							//8个采样数据的和
     ADC_TotalData = 0;
     ch=CHH;
     for(R7=0;R7<ADC_SUM_TIME;R7++)
      {		
       ADC_TotalData += ReadAnalogChannel(ch);	
      }
     ADC_TotalData = (ADC_TotalData / ADC_SUM_TIME);
     return ADC_TotalData; 							   //返回第二组数据，其他三组数据舍弃
}
