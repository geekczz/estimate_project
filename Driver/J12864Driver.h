#ifndef __J12864DRIVER_H
#define __J12864DRIVER_H

#include "GLOBAL.h"

	//RST PA15 out
	//CLK PB3 out
	//SID PB4 out
	//CS PB5 out

#define J_SID_OUT {GPIOB->CRL&=~(0xC<<4);GPIOB->CRL|=3<<4;}
#define J_SID_IN  {GPIOB->CRL&=~(0xB<<4);GPIOB->CRL|=4<<4;}

#define J_RST_PIN GPIOA,GPIO_Pin_15
#define J_CLK_PIN GPIOB,GPIO_Pin_3
#define J_SID_PIN GPIOB,GPIO_Pin_4
#define J_CS_PIN  GPIOB,GPIO_Pin_5

#define J_RST_H GPIO_SetBits(J_RST_PIN)
#define J_RST_L GPIO_ResetBits(J_RST_PIN)

#define J_CLK_H GPIO_SetBits(J_CLK_PIN)
#define J_CLK_L GPIO_ResetBits(J_CLK_PIN)

#define J_SID_H GPIO_SetBits(J_SID_PIN)
#define J_SID_L GPIO_ResetBits(J_SID_PIN)

#define J_SID_GET GPIO_ReadInputDataBit(J_SID_PIN)

#define J_CS_H GPIO_SetBits(J_CS_PIN)
#define J_CS_L GPIO_ResetBits(J_CS_PIN)

void Short_Delay(void);

void SendByteLCD(uint8_t WLCDData);
void SPIWR(uint8_t Wdata,uint8_t WRS);
void WRCommand(uint8_t CMD);
void WRData(uint8_t Data);

void J_LCD_Init(void);

void ShowEngChar(uint8_t addr,uint8_t *english,uint8_t count);
void ShowQQChar(uint8_t addr,uint8_t *english,uint8_t count);
void ShowNUMChar(uint8_t addr,uint8_t i,uint8_t count);
void WRCGRAM(uint8_t data1,uint8_t data2,uint8_t addr);
void ShowCGChar(uint8_t addr,uint8_t i);
void CLEARGDRAM(void);
void WRGDRAM(uint8_t Y1,uint8_t clong,uint8_t hight,uint8_t *TAB1);

void J_LCD_RESET(void);


#endif
