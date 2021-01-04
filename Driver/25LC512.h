#ifndef __25LC512_H
#define __25LC512_H 
#include "GLOBAL.h"  

#define EE_CS PBout(12)
#define EE_HOLD PAout(9)
#define EE_WP PAout(8)

typedef struct
{
	uint8_t eeprom_status_reg;
	uint8_t eeprom_id;
}EEPROM_25LC512_STRUCT;

extern EEPROM_25LC512_STRUCT eeprom_param;

static uint8_t EEPROM_Swap_Data(uint8_t Data);
extern void EEPROM_Write_Enable(void);
extern void EEPROM_Write_Data(uint16_t DataAddress, uint8_t Data);
extern uint8_t EEPROM_Read_Data(uint16_t DataAddress);
extern void EEPROM_Read_Status_Register(void);
extern void EEPROM_Write_Status_Regeister(void);
extern void EEPROM_Get_ID(void);

extern void EEPROM_Continute_Write(uint16_t DataAddress, uint8_t* Payload, uint16_t num);
extern void EEPROM_Continute_Read(uint16_t DataAddress, uint8_t* Payload, uint16_t num);

#endif
