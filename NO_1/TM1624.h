#include "iostm8s003f3.h"
#define CLK_H    PC_ODR  |=0x20;
#define CLK_L    PC_ODR  &=~0x20;
#define CS_H     PC_ODR  |=0x80;
#define CS_L     PC_ODR  &=~0x80;
#define DIO_1    PC_ODR  |=0x40;
#define DIO_0    PC_ODR  &=~0x40;

#define  DISP_7X11            0x03       //7*11  «}Œ´Ò«Œd
#define  DISP_ON_14           0x8F      //ôË«}Œ´ 14Ö{ƒH
#define  DISP_STAB_ADDRESS    0x44 //Þ‹–fÀÞÉÆ‡EÓçÈA³u
#define  ALL_ADDRESS                0x40
#define  STAB                                 0xC0       //SATB|ÈA³u    11000000|0000 0001~1101  ‡EÓçÈA³u

#define  A1_ON   RAM[0] |=  0x04;
#define  A1_OFF  RAM[0] &=~0x04;
#define  A2_ON   RAM[0] |=  0x08;
#define  A2_OFF  RAM[0] &=~0x08;
#define  A3_ON   RAM[1] |=  0x01;
#define  A3_OFF  RAM[1] &=~0x01;
#define  A4_ON   RAM[1] |=  0x02;
#define  A4_OFF  RAM[1] &=~0x02;

#define  B1_ON   RAM[0] |=  0x01;
#define  B1_OFF  RAM[0] &=~0x01;
#define  B2_ON   RAM[0] |=  0x02;
#define  B2_OFF  RAM[0] &=~0x02;
#define  B3_ON   RAM[0] |=  0x40;
#define  B3_OFF  RAM[0] &=~0x40;
#define  B4_ON   RAM[0] |=  0x80;
#define  B4_OFF  RAM[0] &=~0x80;

#define  C1_ON   RAM[1] |=  0x10;
#define  C1_OFF  RAM[1] &=~0x10;
#define  C2_ON   RAM[1] |=  0x20;
#define  C2_OFF  RAM[1] &=~0x20;
#define  C3_ON   RAM[2] |=  0x04;
#define  C3_OFF  RAM[2] &=~0x04;
#define  C4_ON   RAM[2] |=  0x08;
#define  C4_OFF  RAM[2] &=~0x08;

#define  D1_ON   RAM[1] |=  0x40;
#define  D1_OFF  RAM[1] &=~0x40;
#define  D2_ON   RAM[1] |=  0x80;
#define  D2_OFF  RAM[1] &=~0x80;
#define  D3_ON   RAM[2] |=  0x20;
#define  D3_OFF  RAM[2] &=~0x20;
#define  D4_ON   RAM[2] |=  0x10;
#define  D4_OFF  RAM[2] &=~0x10;

#define  E1_ON   RAM[2] |=  0x01;
#define  E1_OFF  RAM[2] &=~0x01;
#define  E2_ON   RAM[2] |=  0x02;
#define  E2_OFF  RAM[2] &=~0x02;
#define  E3_ON   RAM[2] |=  0x40;
#define  E3_OFF  RAM[2] &=~0x40;
#define  E4_ON   RAM[2] |=  0x80;
#define  E4_OFF  RAM[2] &=~0x80;

#define  F1_ON   RAM[0] |=  0x10;
#define  F1_OFF  RAM[0] &=~0x10;
#define  F2_ON   RAM[0] |=  0x20;
#define  F2_OFF  RAM[0] &=~0x20;
#define  F3_ON   RAM[1] |=  0x04;
#define  F3_OFF  RAM[1] &=~0x04;
#define  F4_ON   RAM[1] |=  0x08;
#define  F4_OFF  RAM[1] &=~0x08;

#define  G1_ON   RAM[5] |=  0x10;
#define  G1_OFF  RAM[5] &=~0x10;
#define  G2_ON   RAM[5] |=  0x20;
#define  G2_OFF  RAM[5] &=~0x20;
#define  G3_ON   RAM[5] |=  0x40;
#define  G3_OFF  RAM[5] &=~0x40;
#define  G4_ON   RAM[5] |=  0x80;
#define  G4_OFF  RAM[5] &=~0x80;

void TM1624(unsigned char REG);
void COMMAND(unsigned char COM);
void DISPLAY();
void Num_Char(unsigned char Num);