#include "iostm8s003f3.h"
#include "delay.h"
#include "ADC.h"
unsigned int ReadAnalogChannel(unsigned char CH)
{
  unsigned int AD_Value;
  AD_Value=0;
  
  if(CH==0)
  {
  PD_DDR &=~0x40;     //PD6����
  PD_CR1 &=~0x40;     //����
  PD_CR2 &=~0x40;     //��ֹ�жϹ���
  
  ADC_CR1 =0x00;      //f/2Ԥ��Ƶ
  ADC_CR2 =0x00;      //�����
  ADC_CSR =0x06;       //AIN6��Ϊ�����
  ADC_CR1 |=0x01;
  }
  if(CH==1)
  {
  PC_DDR &=~0x10;     //PC4����
  PC_CR1 &=~0x10;     //����
  PC_CR2 &=~0x10;     //��ֹ�жϹ���
  
  ADC_CR1 =0x00;      //f/2Ԥ��Ƶ
  ADC_CR2 =0x00;      //���Ҷ���
  ADC_CSR =0x02;       //AIN2��Ϊ�����
  ADC_CR1 |=0x01;
  }
  delay_us();
  delay_us();
  delay_us();
  ADC_CR1 |=0x01;
  while(!(ADC_CSR&0x80)){}//�ȴ�ת������
  AD_Value +=ADC_DRH;
  AD_Value=AD_Value<<2;
  AD_Value +=ADC_DRL;
  return AD_Value;
}

/***********************************�����˲�**********************************/
#define ADC_SUM_TIME	16          //ÿ��ƽ��������8����������ƽ���ó�
unsigned int AverageADCData(unsigned char CHH)
{	
     unsigned char R7;
     unsigned char ch;
     unsigned int ADC_TotalData;							//8���������ݵĺ�
     ADC_TotalData = 0;
     ch=CHH;
     for(R7=0;R7<ADC_SUM_TIME;R7++)
      {		
       ADC_TotalData += ReadAnalogChannel(ch);	
      }
     ADC_TotalData = (ADC_TotalData / ADC_SUM_TIME);
     return ADC_TotalData; 							   //���صڶ������ݣ�����������������
}