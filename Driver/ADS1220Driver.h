#ifndef __ADS1220DRIVER_H
#define __ADS1220DRIVER_H

#include "GLOBAL.h"

	//CS PB10 out
	//CLK PB11 out
	//DRDY PA10 in
	//DOUT PA11 in
	//DIN PA12 out

#define ADS1220_CS_PIN GPIOB,GPIO_Pin_10
#define ADS1220_CLK_PIN GPIOB,GPIO_Pin_11
#define ADS1220_DOUT_PIN GPIOA,GPIO_Pin_11
#define ADS1220_DIN_PIN GPIOA,GPIO_Pin_12
#define ADS1220_DRDY_PIN GPIOA,GPIO_Pin_10

#define ADS1220_CS_H GPIO_SetBits(ADS1220_CS_PIN)
#define ADS1220_CS_L GPIO_ResetBits(ADS1220_CS_PIN)

#define ADS1220_CLK_H GPIO_SetBits(ADS1220_CLK_PIN)
#define ADS1220_CLK_L GPIO_ResetBits(ADS1220_CLK_PIN)

#define ADS1220_DIN_H GPIO_SetBits(ADS1220_DIN_PIN)
#define ADS1220_DIN_L GPIO_ResetBits(ADS1220_DIN_PIN)

#define ADS1220_DOUT_GET GPIO_ReadInputDataBit(ADS1220_DOUT_PIN)

#define ADS1220_DADY_GET GPIO_ReadInputDataBit(ADS1220_DRDY_PIN)

typedef struct ADS_DATA
{
	uint8_t AdcDataTemp[3];
	uint32_t AdcData32;
	float    AdcVoltage;
	uint32_t AdcVoltageZero32;
}ADS_DATA;

extern ADS_DATA adsdata;

static void ShortDelay(void);
uint8_t ADS1220_Swap_Data(uint8_t DataIN);
void ADS1220_Write_Reg(uint8_t REG,uint8_t Data);
uint8_t ADS1220_Read_Reg(uint8_t REG);
void ADS1220_RESET(void);
void ADS1220_START(void);
void ADS1220_UPDATE_Data(void);
uint8_t ADS1220_Init(void);

#endif
