//#include "stm8s.h"
#include "iostm8s003f3.h"
#include "delay.h"
#include "TM1624.h"
#include "TIMER1.h"
#include "ADC.h"



//#define   LEFT   PB_IDR_IDR5==1       //左转
//#define   RIGHT  PB_IDR_IDR4==1       //右转
#define   LEFT   PB_IDR_IDR4==1       //左转
#define   RIGHT  PB_IDR_IDR5==1       //右转
#define   MORE_S PA_IDR_IDR3==0       //超速
#define   NEAR   PD_IDR_IDR4==1       //近光
#define   FAR    PD_IDR_IDR5==1       //远光

#define   V51_5  775
#define   V49    744
#define   V46_4  704
#define   V43_8  669
#define   V41    631
#define   V30    464
#define   DLT    20

unsigned char i;
unsigned char j;
unsigned int k;
unsigned int m;

unsigned char Speed;
unsigned char SHIWEI;
unsigned char GEWEI;
unsigned int  HALL_CNT;      //霍尔计数器
unsigned int  TEMP1;
unsigned char FLASH;
unsigned int  VOLT_B1;       //电瓶电压1
unsigned int  OLD_B1;        //电瓶电压1记忆电压，防止BMS突然断电
unsigned int  VOLT_B2;       //电瓶电压2
unsigned int  OLD_B2;        //电瓶电压2记忆电压，防止BMS突然断电
unsigned char B1_LIUSHUI;    //B1流水显示数
unsigned char B2_LIUSHUI;    //B1流水显示数
unsigned char B1_LED;        //B1显示LED数量数 0:不显示  1:闪烁T1  2:T1  3:T1/T2  4:T1/T2/T3  5:T1/T2/T3/T4  6:T1/T2/T3/T4/T5
unsigned char B1_OLD;        //B1前电量指示
unsigned char B2_LED;        //B2显示LED数量数
unsigned char B2_OLD;        //B2前电量指示
unsigned char CNT_LOST1;     //B1突然掉电计数
unsigned char CNT_LOST2;     //B2突然掉电计数
struct BitFlag                //位操作结构体
{
unsigned char LEFT_F  : 1;
unsigned char RIGHT_F : 1;
unsigned char MORE_F  : 1;
unsigned char FLASH   : 1;
unsigned char SERIAL1 : 1;     //复位后B1显示流水电量，显示完后不用
unsigned char SERIAL2 : 1;     //复位后B2显示流水电量，显示完后不用
unsigned char B1_F    : 1;     //B1亏点闪烁T1
unsigned char B2_F    : 1;     //B2亏点闪烁U1
}Flag;

///////////////////////////////////////////////////////////////
void GPIO_INIT(void);
void Init_Timer4(void);
void Init_EXTI2(void);
void Init_Time2(void);

///////////////////////////////////////////////////////////////
unsigned char  RAM[7];
int main()
{
  GPIO_INIT();
  B1_OLD=6;
  delay_ms(80);
  for(i=0;i<7;i++)
  {RAM[i]=0xFF;}
  DISPLAY();
  delay_ms(40);
  
  
  
  
  
  delay_ms(150);
    Num_Char(0);
    RAM[6] &=~0xFF;
    RAM[4] &=~0xFF;
    RAM[5] &=~0x0F;
    RAM[3] &=~0xFF;
    DISPLAY();
  
  VOLT_B1=AverageADCData(0);
 
  if(VOLT_B1<V30)
     {Flag.B1_F=0;Flag.SERIAL1=0;B1_OLD=0;RAM[5] &=~0x01;}       //B1低于30V,T1灭，也不要流水显示了
    if((VOLT_B1>=V30)&&(VOLT_B1<V41))
     {Flag.B1_F=1;Flag.SERIAL1=0;B1_OLD=1;}                      //B1低于41V高于30V,T1闪烁，也不要流水显示了
    if((VOLT_B1>=V41)&&(VOLT_B1<V43_8))
     {Flag.B1_F=0;Flag.SERIAL1=1;B1_LIUSHUI=1;B1_OLD=2;}       //B1低于43.8V高于41V,流水显示T1
    if((VOLT_B1>=V43_8)&&(VOLT_B1<V46_4))
     {Flag.B1_F=0;Flag.SERIAL1=1;B1_LIUSHUI=2;B1_OLD=3;}       //B1低于43.8V高于41V,流水显示T1\T2
    if((VOLT_B1>=V46_4)&&(VOLT_B1<V49))
     {Flag.B1_F=0;Flag.SERIAL1=1;B1_LIUSHUI=3;B1_OLD=4;}       //B1低于43.8V高于41V,流水显示T1\T2\T3
    if((VOLT_B1>=V49)&&(VOLT_B1<V51_5))
     {Flag.B1_F=0;Flag.SERIAL1=1;B1_LIUSHUI=4;B1_OLD=5;}       //B1低于43.8V高于41V,流水显示T1\T2\T3\T4
    if(VOLT_B1>=V51_5)
     {Flag.B1_F=0;Flag.SERIAL1=1;B1_LIUSHUI=5;B1_OLD=6;}       //B1V高于51.5V,流水显示T1\T2\T3\T4\T5
    
  VOLT_B2=AverageADCData(1);
  
    if(VOLT_B2<V30)
     {Flag.B2_F=0;Flag.SERIAL2=0;RAM[5] &=~0x02;}       //B2低于30V,T1灭，也不要流水显示了
    if((VOLT_B2>=V30)&&(VOLT_B2<V41))
     {Flag.B2_F=1;Flag.SERIAL2=0;}                      //B2低于41V高于30V,T1闪烁，也不要流水显示了
    if((VOLT_B2>=V41)&&(VOLT_B2<V43_8))
     {Flag.B2_F=0;Flag.SERIAL2=1;B2_LIUSHUI=1;}       //B2低于43.8V高于41V,流水显示T1
    if((VOLT_B2>=V43_8)&&(VOLT_B2<V46_4))
     {Flag.B2_F=0;Flag.SERIAL2=1;B2_LIUSHUI=2;}       //B2低于43.8V高于41V,流水显示T1\T2
    if((VOLT_B2>=V46_4)&&(VOLT_B2<V49))
     {Flag.B2_F=0;Flag.SERIAL2=1;B2_LIUSHUI=3;}       //B2低于43.8V高于41V,流水显示T1\T2\T3
    if((VOLT_B2>=V49)&&(VOLT_B2<V51_5))
     {Flag.B2_F=0;Flag.SERIAL2=1;B2_LIUSHUI=4;}       //B2低于43.8V高于41V,流水显示T1\T2\T3\T4
    if(VOLT_B2>=V51_5)
     {Flag.B2_F=0;Flag.SERIAL2=1;B2_LIUSHUI=5;}       //B2V高于51.5V,流水显示T1\T2\T3\T4\T5
  
  
  
  Speed=0;
  Timer1Init();
  Init_Time2();
  Init_EXTI2();
  Init_Timer4();
  asm("rim");//开全局中断
  while(1)
  { 
    
   
 /***********************B1电量显示**************************************/
   
    if(Flag.SERIAL1==0)
    {
    VOLT_B1=AverageADCData(0);
    if(OLD_B1>VOLT_B1){if((OLD_B1-VOLT_B1)>250){CNT_LOST1++;if(CNT_LOST1<5){VOLT_B1=OLD_B1;}else{CNT_LOST1=0;}}}
   OLD_B1=VOLT_B1;
    if(VOLT_B1<V30)
     {Flag.B1_F=0;B1_LED=0;RAM[5] &=~0x01;RAM[3] &=~0x0F;}       //B1低于30V,T1灭，也不要流水显示了
    if((VOLT_B1>=V30)&&(VOLT_B1<V41))
     {if(B1_OLD<1)
      {if((VOLT_B1-V30)>17){Flag.B1_F=1;B1_LED=1;RAM[3] &=~0x0F;}} 
      else
      {Flag.B1_F=1;B1_LED=1;RAM[3] &=~0x0F;}
     
     }                     //B1低于41V高于30V,T1闪烁，也不要流水显示了
  if((VOLT_B1>=V41)&&(VOLT_B1<V43_8))
     {if(B1_OLD<2)
      {if((VOLT_B1-V41)>DLT){Flag.B1_F=0;B1_LED=2;RAM[5] |=0x01;RAM[3] &=~0x0F; }} 
      else
      {Flag.B1_F=0;B1_LED=2;RAM[5] |=0x01;RAM[3] &=~0x0F;}
     
      }        //B1低于43.8V高于41V,流水显示T1
  if((VOLT_B1>=V43_8)&&(VOLT_B1<V46_4))
     {if(B1_OLD<3)
      {if((VOLT_B1-V43_8)>(DLT/2)){Flag.B1_F=0;B1_LED=3;RAM[5] |=0x01; RAM[3] |=0x08;RAM[3] &=~0x07;}} 
      else
      {Flag.B1_F=0;B1_LED=3;RAM[5] |=0x01;RAM[3] |=0x08;RAM[3] &=~0x07;}
      }        //B1低于43.8V高于41V,流水显示T1\T2
  if((VOLT_B1>=V46_4)&&(VOLT_B1<V49))
     {if(B1_OLD<4)
      {if((VOLT_B1-V46_4)>(12)){Flag.B1_F=0;B1_LED=4;RAM[5] |=0x01; RAM[3] |=0x0C;RAM[3] &=~0x03;}} 
      else
      {Flag.B1_F=0;B1_LED=4;RAM[5] |=0x01; RAM[3] |=0x0C;RAM[3] &=~0x03;}
      }       //B1低于43.8V高于41V,流水显示T1\T2\T3
    
  if((VOLT_B1>=V49)&&(VOLT_B1<V51_5))
     {if(B1_OLD<5)
      {if((VOLT_B1-V49)>(12)){Flag.B1_F=0;B1_LED=5;RAM[5] |=0x01; RAM[3] |=0x0E;RAM[3] &=~0x01;}} 
      else
      {Flag.B1_F=0;B1_LED=5;RAM[5] |=0x01; RAM[3] |=0x0E;RAM[3] &=~0x01;}
      }       //B1低于43.8V高于41V,流水显示T1\T2\T3\T4
  if(VOLT_B1>=V51_5)
     {if(B1_OLD<6)
      {if((VOLT_B1-V51_5)>(15)){Flag.B1_F=0;B1_LED=6;RAM[5] |=0x01; RAM[3] |=0x0F;}} 
      else
      {Flag.B1_F=0;B1_LED=6;RAM[5] |=0x01; RAM[3] |=0x0F;}
      }       //B1V高于51.5V,流水显示T1\T2\T3\T4\T5
  B1_OLD=B1_LED;
    }
    
 /***********************B2电量显示**************************************/   
    
    if(Flag.SERIAL2==0)
    {
    VOLT_B2=AverageADCData(1);
    if(OLD_B2>VOLT_B2){if((OLD_B2-VOLT_B2)>250){CNT_LOST2++;if(CNT_LOST2<5){VOLT_B2=OLD_B2;}else{CNT_LOST2=0;}}}
    OLD_B2=VOLT_B2;    
    if(VOLT_B2<V30)
     {Flag.B2_F=0;B2_LED=0;RAM[5] &=~0x02;RAM[3] &=~0xF0;}       //B1低于30V,T1灭，也不要流水显示了
    if((VOLT_B2>=V30)&&(VOLT_B2<V41))
     {if(B2_OLD<1)
      {if((VOLT_B2-V30)>20){Flag.B2_F=1;B2_LED=2;RAM[3] &=~0xF0;}} 
      else
      {Flag.B2_F=1;B2_LED=1;RAM[3] &=~0xF0;}
     
     }                     //B1低于41V高于30V,T1闪烁，也不要流水显示了
  if((VOLT_B2>=V41)&&(VOLT_B2<V43_8))
     {if(B2_OLD<2)
      {if((VOLT_B2-V41)>20){Flag.B2_F=0;B2_LED=2;RAM[5] |=0x02;RAM[3] &=~0xF0; }} 
      else
      {Flag.B2_F=0;B2_LED=2;RAM[5] |=0x02;RAM[3] &=~0xF0;}
     
      }        //B1低于43.8V高于41V,流水显示T1
  if((VOLT_B2>=V43_8)&&(VOLT_B2<V46_4))
     {if(B2_OLD<3)
      {if((VOLT_B2-V43_8)>10){Flag.B2_F=0;B2_LED=3;RAM[5] |=0x02; RAM[3] |=0x80;RAM[3] &=~0x70;}} 
      else
      {Flag.B2_F=0;B2_LED=3;RAM[5] |=0x02;RAM[3] |=0x80;RAM[3] &=~0x70;}
      }        //B1低于43.8V高于41V,流水显示T1\T2
  if((VOLT_B2>=V46_4)&&(VOLT_B2<V49))
     {if(B2_OLD<4)
      {if((VOLT_B2-V46_4)>10){Flag.B2_F=0;B2_LED=4;RAM[5] |=0x02; RAM[3] |=0xC0;RAM[3] &=~0x30;}} 
      else
      {Flag.B2_F=0;B2_LED=4;RAM[5] |=0x02; RAM[3] |=0xC0;RAM[3] &=~0x30;}
      }       //B1低于43.8V高于41V,流水显示T1\T2\T3
  if((VOLT_B2>=V49)&&(VOLT_B2<V51_5))
     {if(B2_OLD<5)
      {if((VOLT_B2-V49)>10){Flag.B2_F=0;B2_LED=5;RAM[5] |=0x02; RAM[3] |=0xE0;RAM[3] &=~0x10;}} 
      else
      {Flag.B2_F=0;B2_LED=5;RAM[5] |=0x02; RAM[3] |=0xE0;RAM[3] &=~0x10;}
      }       //B1低于43.8V高于41V,流水显示T1\T2\T3\T4
  if(VOLT_B2>=V51_5)
     {if(B2_OLD<6)
      {if((VOLT_B2-V51_5)>10){Flag.B2_F=0;B2_LED=6;RAM[5] |=0x02; RAM[3] |=0xF0;}} 
      else
      {Flag.B2_F=0;B2_LED=6;RAM[5] |=0x02; RAM[3] |=0xF0;}
      }       //B1V高于51.5V,流水显示T1\T2\T3\T4\T5
  B2_OLD=B2_LED;
    }
    
    
    //if(LEFT)  { Flag.LEFT_F=1; }   else{Flag.LEFT_F=0;}
    //if(RIGHT) { Flag.RIGHT_F=1;}   else{Flag.RIGHT_F=0;}
    if(MORE_S){ RAM[6] |=0x20; }   else{RAM[6] &=~0x20;}
    if(NEAR)  {RAM[6] |=0x04;}     else{RAM[6] &=~0x04;}
    if(FAR)   {RAM[6] |=0x01;}     else{RAM[6] &=~0x01;}
    RAM[6] &=~0x02;
    RAM[4] &=~0xFF;
    RAM[5] &=~0x0C;

    Num_Char(Speed);
    DISPLAY();
    //if(Speed>99){Speed=0;}	//20170926
   }
}

void GPIO_INIT(void)
{
    PA_DDR   &=~0x08;      //PA3超速输入
    PA_CR1   |=0x08;       //PA3上拉                             //PC3567上拉
    PA_CR2   =0x00;        //无中断 
  
  
  
    PB_DDR   =0x00;        //PB4-5左右转向输入
    PB_CR1   |=0x30;       //PB4-5上拉                             //PC3567上拉
    PB_CR2   =0x00;        //无中断 
  
    // PC_DDR  |=0xE0;
    // PC_CR1  |=0xE8;                                     //PC3567上拉
    // PC_CR2  |=0x08;        //使能PCA3外部中断  霍尔脉冲  
    PC_DDR  |=0xE0;			//20170926
    PC_CR1  |=0xE0;        //PC567上拉,PC3悬浮输入
    PC_CR2  |=0x08;        //使能PC3外部中断  霍尔脉冲  
    
    PD_DDR |=0x0F;
    PD_CR1  |=0x3C;                                     //PD2345上拉   23远近光
    PD_CR2  =0x00;       
    
    
    
}
/*************************************************************************/
void Init_Timer4(void)
{
    TIM4_CR1=0x01;//使能计数器
    TIM4_IER=0x01;//更新中断使能
    TIM4_EGR=0x01;
    TIM4_CNTR=255;//计数器值
    TIM4_ARR=255;//自动重装的值
    TIM4_PSCR=0x07;//分频值

}
#pragma vector=TIM4_OVR_UIF_vector//0x19
__interrupt void TIM4_OVR_UIF_IRQHandler(void)//对应IAP的中断地址：0x8060
{
    TIM4_SR=0x00;
   
        
    j++;
    if(j>=30)
    {j=0;
     TEMP1=HALL_CNT*76/125;          //原来是*99/700，要求改为1400，每秒28个脉冲
     Speed=TEMP1;
	 if ( Speed > 25 )
		 Speed = (unsigned short)Speed * 122 / 100;	//20170929
     HALL_CNT=0;
    
    }
}
////////////////////////////////////////////////////
void Init_EXTI2(void)
{
    EXTI_CR1 |= 0x10;       //PC口外部中断上升沿促发
}

#pragma vector=EXTI2_vector //0x07
__interrupt void EXTI2_Hand_Fun(void)
{
    HALL_CNT++;
}

void Init_Time2(void)
{
	TIM2_PSCR = 0x02;
	TIM2_ARRH = 0x03;
	TIM2_ARRL = 0xE8;
	TIM2_IER  = 0x01;
	TIM2_CR1  = 0x01;
}

#pragma vector=TIM2_OVR_UIF_vector
__interrupt void TIM2_Hand_Fun(void)
{
	static unsigned int left_count=0,right_count=0;

	TIM2_SR1 = 0;

	if ( LEFT ){
		if ( left_count < 200 ){
			left_count += 2;
		} else if ( left_count < 1000 ){
			left_count += 2;
			Flag.LEFT_F = 1;
		} else {
			left_count = 1000;
		}
	} else {
		if ( left_count )
			left_count --;
		if ( left_count == 0 )
			Flag.LEFT_F = 0; 
	}
	
	if ( RIGHT ) {
		if ( right_count < 200 ){
			right_count += 2;
		} else if ( right_count < 1000 ){
			right_count += 2;
			Flag.RIGHT_F = 1;
		} else {
			right_count = 1000;
		}
	} else {
		if ( right_count )
			right_count --;
		if ( right_count == 0 )
			Flag.RIGHT_F = 0; 
	}	
}

