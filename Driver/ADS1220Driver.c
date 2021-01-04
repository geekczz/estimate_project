#include "ADS1220Driver.h"

ADS_DATA adsdata;

void ShortDelay(void)
{
	uint8_t i = 0;
	for(i=0;i<2;i++)
	{
		__nop();
	}
}

uint8_t ADS1220_Swap_Data(uint8_t DataIN)
{
	uint8_t DataTemp = 0;
	uint8_t CounterI = 0;
	uint8_t ResultTemp = 0;
	for(CounterI = 0 ; CounterI < 8 ; CounterI++)
	{
		if(DataIN & 0x80)ADS1220_DIN_H;
		else ADS1220_DIN_L;
		DataIN = DataIN << 0x01;
		
		ADS1220_CLK_H;
		ShortDelay();
		ADS1220_CLK_L;
		
		ResultTemp = ResultTemp << 0x01;
		DataTemp = ADS1220_DOUT_GET;
		ResultTemp |=  DataTemp;
		
		ShortDelay();
	}
	return ResultTemp;
}

void ADS1220_Write_Reg(uint8_t REG,uint8_t Data)
{
	ADS1220_CS_L;
	ShortDelay();
	
	ADS1220_Swap_Data(REG);
	ADS1220_Swap_Data(Data);
	
	ShortDelay();
	ADS1220_CS_H;
}

uint8_t ADS1220_Read_Reg(uint8_t REG)
{
	uint8_t ResultTemp = 0;
	
	ADS1220_CS_L;
	ShortDelay();
	
	ADS1220_Swap_Data(REG);
	ResultTemp = ADS1220_Swap_Data(0x00);
	
	ShortDelay();
	ADS1220_CS_H;
	
	return ResultTemp;
}

void ADS1220_RESET(void)
{
	ADS1220_CS_L;
	ShortDelay();
	
	ADS1220_Swap_Data(0x06);
	
	ShortDelay();
	ADS1220_CS_H;
}

void ADS1220_START(void)
{
	ADS1220_CS_L;
	ShortDelay();
	
	ADS1220_Swap_Data(0x08);
	
	ShortDelay();
	ADS1220_CS_H;
}

void ADS1220_UPDATE_Data(void)
{
	ADS1220_CS_L;
	ShortDelay();
	
	while(ADS1220_DADY_GET == 1);
	
	adsdata.AdcDataTemp[0] = ADS1220_Swap_Data(0x00);
	adsdata.AdcDataTemp[1] = ADS1220_Swap_Data(0x00);
	adsdata.AdcDataTemp[2] = ADS1220_Swap_Data(0x00);
	
	ShortDelay();
	ADS1220_CS_H;
	
	adsdata.AdcData32 = adsdata.AdcDataTemp[0]<<16 | adsdata.AdcDataTemp[1]<<8 | adsdata.AdcDataTemp[2];
	
	adsdata.AdcVoltage = adsdata.AdcData32 * 2.048 / 8388608 * 5.039;
}

uint8_t ADS1220_Init(void)
{
	uint8_t DataTemp = 0;
	uint8_t i = 0;
	uint32_t VoltageSum = 0.0;
	uint32_t VoltageAve = 0.0;
	
	ADS1220_RESET();
	
	bsp_delayms(10);
	
	ADS1220_Write_Reg(0x40,0x91);
	DataTemp = ADS1220_Read_Reg(0x20);
	if(DataTemp != 0x91)
	{
		return 1;
	}
	
	ADS1220_Write_Reg(0x44,0xC4);
	DataTemp = ADS1220_Read_Reg(0x24);
	
	ADS1220_Write_Reg(0x48,0x00);
	DataTemp = ADS1220_Read_Reg(0x28);
	
	ADS1220_Write_Reg(0x4C,0x00);
	DataTemp = ADS1220_Read_Reg(0x2C);
	
	ADS1220_START();
	bsp_delayms(100);
	
	
	for(i = 0;i<20;i++)
	{
		ADS1220_UPDATE_Data();
		
		VoltageSum += adsdata.AdcData32;
	}
	VoltageAve = VoltageSum / 20;
	
	adsdata.AdcVoltageZero32 = VoltageAve;
	
	return 0;
}
