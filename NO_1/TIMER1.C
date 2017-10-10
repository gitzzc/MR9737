#include "TIMER1.h"

extern unsigned char FLASH;
extern unsigned char RAM[];
extern unsigned char B1_LIUSHUI;    //B1��ˮ��ʾ��
extern unsigned char B2_LIUSHUI;    //B2��ˮ��ʾ��
unsigned char B1_j;                  //B1��ˮ����
unsigned char B2_j;                  //B2��ˮ����

extern struct BitFlag
{
unsigned char LEFT_F  : 1;
unsigned char RIGHT_F : 1;
unsigned char MORE_F  : 1;
unsigned char FLASH   : 1;
unsigned char SERIAL1 : 1;     //��λ��B1��ʾ��ˮ��������ʾ�����
unsigned char SERIAL2 : 1;     //��λ��B2��ʾ��ˮ��������ʾ�����
unsigned char B1_F    : 1;     //B1������˸T1
unsigned char B2_F    : 1;     //B2������˸U1
}Flag;


void Timer1Init(void)
{
 asm("sim");        // ��ȫ���ж�
 TIM1_IER=0x00;
 TIM1_PSCRH=0x00;
 TIM1_PSCRL=0x32;     //(fCK_CNT)����fCK_PSC/( PSCR[15:0]+1)��16M/(15999+1)=1000  1ms
 TIM1_ARRH=0x03;    // 0x247=583*1ms    70��/��
 TIM1_ARRL=0x59;
 TIM1_CNTRH=0x00;
 TIM1_CNTRL=0x00;   //�Զ�װ�ؼĴ�����ֵ
 TIM1_IER=0x01;     //���������ж�
 TIM1_RCR=20;       //�ظ������Ĵ���ֵ?
 TIM1_CR1=0x01;     //���Զ�װ�أ��򿪼�����
// TIM1_IER=0x01;
 //asm("rim");        // ��ȫ���ж�
}

#pragma vector=TIM1_OVR_UIF_vector
__interrupt void TIM1_OVF_IRQHandler(void)
{

if(TIM1_SR1&0x01)
{
  TIM1_SR1&=~0x01;//����жϱ�־
  TIM1_ARRH=0x03;    //500ms
  TIM1_ARRL=0x59;

  if(Flag.SERIAL1==1)
  {
    if(B1_j<B1_LIUSHUI)
    {
      switch(B1_j)
      {
      case 0:  RAM[5] |=0x01;break;
      case 1:  RAM[3] |=0x08;break;
      case 2:  RAM[3] |=0x04;break;
      case 3:  RAM[3] |=0x02;break;
      case 4:  RAM[3] |=0x01;break;
      default:break;
       }
     if(B1_j==(B1_LIUSHUI-1)) {Flag.SERIAL1=0;} 
    }
    B1_j++;
  }
  if(Flag.SERIAL2==1)
  {
    if(B2_j<B2_LIUSHUI)
    {
      switch(B2_j)
      {
      case 0:  RAM[5] |=0x02;break;
      case 1:  RAM[3] |=0x80;break;
      case 2:  RAM[3] |=0x40;break;
      case 3:  RAM[3] |=0x20;break;
      case 4:  RAM[3] |=0x10;break;
      default:break;
       }
      if(B2_j==(B2_LIUSHUI-1)) {Flag.SERIAL2=0;}
    }
    B2_j++;
  } 
  
  
  
  
  if(Flag.FLASH==0)
   {
    Flag.FLASH=1;
    if(Flag.B1_F==1)   {RAM[5] |=0x01; }
    if(Flag.B2_F==1)   {RAM[5] |=0x02; }
    if(Flag.LEFT_F==1) {RAM[6] |=0x08; }
    if(Flag.RIGHT_F==1){RAM[6] |=0x10; }
   // if(Flag.MORE_F==1) {RAM[6] |=0x20; }
    
    }
  else
    {
      RAM[6]&=~0x18;
      Flag.FLASH=0;
      if(Flag.B1_F==1)   {RAM[5] &=~0x01; }
      if(Flag.B2_F==1)   {RAM[5] &=~0x02; }
    }
  

}
}