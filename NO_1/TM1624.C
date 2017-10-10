#include "TM1624.h"
#include "delay.h"
extern unsigned char  RAM[7];
extern unsigned char SHIWEI;
extern unsigned char GEWEI;
void TM1624(unsigned char REG)
{
  unsigned char i;
  for(i = 0; i <8; ++i)
	{
          
          
          if((0x01&REG)==0x01)
	    {
		DIO_1
	    }
	  else
	    {
		DIO_0
	    }
          asm("nop");
          CLK_L
          asm("nop");  
	  CLK_H
          delay_us();
	  REG >>= 1;
	}
}
  
void COMMAND(unsigned char COM)
{
  CS_H
  //delay_us();  
  CS_L
  //delay_us();  
  TM1624(COM);
  
}
void DISPLAY(void)
{
    unsigned char k;
    COMMAND(DISP_7X11);
    COMMAND(ALL_ADDRESS);
    COMMAND(STAB);
    for(k=0;k<7;k++)
     {
        TM1624(RAM[k]);
        TM1624(0x00);
      }
    COMMAND(DISP_ON_14);
    CS_H
    delay_us();
}
/***********************************************************************************/
void Num_Char(unsigned char Num)
{
  if(Num>99)	{Num=99;}	
  SHIWEI=Num/10;
  GEWEI=Num%10;
  switch(SHIWEI)
  {
  case 0:
              A1_ON;A2_ON;F1_ON;F2_ON;
              E1_ON;E2_ON;D1_ON;D2_ON;
              C1_ON;C2_ON;B1_ON;B2_ON;
              G1_OFF;G2_OFF;
               break;
  case 1:
              F1_ON;F2_ON;
              A1_OFF;A2_OFF;B1_OFF;B2_OFF;C1_OFF;C2_OFF;D1_OFF;D2_OFF;
              E1_ON;E2_ON;G1_OFF;G2_OFF;
              break;            
   case 2:
              A1_ON;A2_ON;F1_ON;F2_ON;G1_ON;G2_ON
              D1_ON;D2_ON;C1_ON;C2_ON;
              B1_OFF;B2_OFF;E1_OFF;E2_OFF;
              break;       
   case 3:
              A1_ON;A2_ON;F1_ON;F2_ON;G1_ON;G2_ON
              D1_ON;D2_ON;C1_OFF;C2_OFF;
              B1_OFF;B2_OFF;E1_ON;E2_ON;
              break;     
    case 4:
              A1_OFF;A2_OFF;F1_ON;F2_ON;G1_ON;G2_ON
              D1_OFF;D2_OFF;C1_OFF;C2_OFF;
              B1_ON;B2_ON;E1_ON;E2_ON;
              break;  
    case 5:
              A1_ON;A2_ON;F1_OFF;F2_OFF;G1_ON;G2_ON
              D1_ON;D2_ON;C1_OFF;C2_OFF;
              B1_ON;B2_ON;E1_ON;E2_ON;
              break;
     case 6:
              A1_ON;A2_ON;F1_OFF;F2_OFF;
              E1_ON;E2_ON;D1_ON;D2_ON;
              C1_ON;C2_ON;B1_ON;B2_ON;
              G1_ON;G2_ON;
               break;         
       case 7:
              F1_ON;F2_ON;
              A1_ON;A2_ON;B1_OFF;B2_OFF;C1_OFF;C2_OFF;D1_OFF;D2_OFF;
              E1_ON;E2_ON;G1_OFF;G2_OFF;
              break;          
       case 8:
              A1_ON;A2_ON;F1_ON;F2_ON;
              E1_ON;E2_ON;D1_ON;D2_ON;
              C1_ON;C2_ON;B1_ON;B2_ON;
              G1_ON;G2_ON;
               break;   
        case 9:
              A1_ON;A2_ON;F1_ON;F2_ON;
              E1_ON;E2_ON;D1_ON;D2_ON;
              C1_OFF;C2_OFF;B1_ON;B2_ON;
              G1_ON;G2_ON;
              break;    
  default:break;
  }
  
  
  switch(GEWEI)
  {
  case 0:
              A3_ON;A4_ON;F3_ON;F4_ON;
              E3_ON;E4_ON;D3_ON;D4_ON;
              C3_ON;C4_ON;B3_ON;B4_ON;
              G3_OFF;G4_OFF;
               break;
  case 1:
              F3_ON;F4_ON;
              A3_OFF;A4_OFF;B3_OFF;B4_OFF;C3_OFF;C4_OFF;D3_OFF;D4_OFF;
              E3_ON;E4_ON;G3_OFF;G4_OFF;
              break;            
   case 2:
              A3_ON;A4_ON;F3_ON;F4_ON;G3_ON;G4_ON
              D3_ON;D4_ON;C3_ON;C4_ON;
              B3_OFF;B4_OFF;E3_OFF;E4_OFF;
              break;       
   case 3:
              A3_ON;A4_ON;F3_ON;F4_ON;G3_ON;G4_ON
              D3_ON;D4_ON;C3_OFF;C4_OFF;
              B3_OFF;B4_OFF;E3_ON;E4_ON;
              break;     
    case 4:
              A3_OFF;A4_OFF;F3_ON;F4_ON;G3_ON;G4_ON
              D3_OFF;D4_OFF;C3_OFF;C4_OFF;
              B3_ON;B4_ON;E3_ON;E4_ON;
              break;  
    case 5:
              A3_ON;A4_ON;F3_OFF;F4_OFF;G3_ON;G4_ON
              D3_ON;D4_ON;C3_OFF;C4_OFF;
              B3_ON;B4_ON;E3_ON;E4_ON;
              break;
     case 6:
              A3_ON;A4_ON;F3_OFF;F4_OFF;
              E3_ON;E4_ON;D3_ON;D4_ON;
              C3_ON;C4_ON;B3_ON;B4_ON;
              G3_ON;G4_ON;
               break;         
       case 7:
              F3_ON;F4_ON;
              A3_ON;A4_ON;B3_OFF;B4_OFF;C3_OFF;C4_OFF;D3_OFF;D4_OFF;
              E3_ON;E4_ON;G3_OFF;G4_OFF;
              break;          
       case 8:
              A3_ON;A4_ON;F3_ON;F4_ON;
              E3_ON;E4_ON;D3_ON;D4_ON;
              C3_ON;C4_ON;B3_ON;B4_ON;
              G3_ON;G4_ON;
               break;   
        case 9:
              A3_ON;A4_ON;F3_ON;F4_ON;
              E3_ON;E4_ON;D3_ON;D4_ON;
              C3_OFF;C4_OFF;B3_ON;B4_ON;
              G3_ON;G4_ON;
              break;    
  default:break;
  }
}