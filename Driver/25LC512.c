#include "25LC512.h"

EEPROM_25LC512_STRUCT eeprom_param;

uint8_t EEPROM_Swap_Data(uint8_t Data)
{
	uint8_t return_data;
	return_data = fun_spiSwap(SPI2, Data);
	return return_data;
}

void EEPROM_Write_Enable(void)
{
	EE_CS = 0;
	delay_ms(1);
	EEPROM_Swap_Data(0x06);
	delay_ms(1);
	EE_CS = 1;
}

void EEPROM_Write_Data(uint16_t DataAddress, uint8_t Data)
{
	uint8_t Address_H, Address_L = 0;
	
	Address_H = (DataAddress >> 8) & 0xFF;
	Address_L = DataAddress & 0xFF;
	
	EEPROM_Write_Enable();
	
	EE_CS = 0;
	delay_us(1);
	EEPROM_Swap_Data(0x02);
	EEPROM_Swap_Data(Address_H);
	EEPROM_Swap_Data(Address_L);
	EEPROM_Swap_Data(Data);
	delay_us(1);
	EE_CS = 1;
	delay_ms(5);
}

uint8_t EEPROM_Read_Data(uint16_t DataAddress)
{
	uint8_t Address_H, Address_L = 0;
	uint8_t DataTemp = 0;
	
	Address_H = (DataAddress >> 8) & 0xFF;
	Address_L = DataAddress & 0xFF;
	
	EE_CS = 0;
	delay_us(1);
	EEPROM_Swap_Data(0x03);
	EEPROM_Swap_Data(Address_H);
	EEPROM_Swap_Data(Address_L);
	DataTemp = EEPROM_Swap_Data(0x00);
	delay_us(1);
	EE_CS = 1;
	
	return DataTemp;
}

void EEPROM_Get_ID(void)
{
	EE_CS = 0;
	delay_ms(1);
	EEPROM_Swap_Data(0xAB);
	EEPROM_Swap_Data(0x00);
	EEPROM_Swap_Data(0x00);
	eeprom_param.eeprom_id = EEPROM_Swap_Data(0x00);
	delay_ms(1);
	EE_CS = 1;
}

void EEPROM_Read_Status_Register(void)
{
	EE_CS = 0;
	delay_us(1);
	EEPROM_Swap_Data(0x05);
	eeprom_param.eeprom_status_reg = EEPROM_Swap_Data(0x00);
	delay_us(1);
	EE_CS = 1;
}

void EEPROM_Write_Status_Regeister(void)
{
	EE_CS = 0;
	delay_us(1);
	EEPROM_Swap_Data(0x01);
	EEPROM_Swap_Data(0x0C);
	delay_us(1);
	EE_CS = 1;
}

void EEPROM_Continute_Write(uint16_t DataAddress, uint8_t* Payload, uint16_t num)
{
	uint16_t i;
	for(i=0;i<num;i++)
	{
		EEPROM_Write_Data(DataAddress, Payload[i]);
		DataAddress += 1;
	}
}

void EEPROM_Continute_Read(uint16_t DataAddress, uint8_t* Payload, uint16_t num)
{
	uint8_t Address_H, Address_L = 0;
	uint16_t Counter_I;
	
	Address_H = (DataAddress >> 8) & 0xFF;
	Address_L = DataAddress & 0xFF;
	
	EE_CS = 0;
	delay_us(1);
	EEPROM_Swap_Data(0x03);
	EEPROM_Swap_Data(Address_H);
	EEPROM_Swap_Data(Address_L);
	for(Counter_I=0; Counter_I<num; Counter_I++)
	{
		Payload[Counter_I] = EEPROM_Swap_Data(0x00);
	}
	delay_us(1);
	EE_CS = 1;
}
