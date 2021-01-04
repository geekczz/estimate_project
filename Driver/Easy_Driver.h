#ifndef __EASY_DRIVER_H
#define __EASY_DRIVER_H
#include "GLOBAL.h"

#define relay_1_out PBout(9)
#define relay_2_out PBout(8)

#define columns_1 GPIOA,GPIO_Pin_5
#define columns_2 GPIOA,GPIO_Pin_6
#define columns_3	GPIOA,GPIO_Pin_7
#define columns_4	GPIOB,GPIO_Pin_0
#define rows_1    GPIOA,GPIO_Pin_3
#define rows_2    GPIOA,GPIO_Pin_4

typedef enum
{
	KEY_NONE =0x00,
  KEY1 = 0x01,
	KEY2 = 0x02,
	KEY3 = 0x04,
	KEY4 = 0x08,
	KEY5 = 0x10,
	KEY6 = 0x20,
	KEY7 = 0x40,
	KEY8 = 0x80
}KEY_TypeDef;
/*--------------------------------------  D E C L A R A T I O N  --------------------------------------*/
/* Internal Variable */

/* External Variable */

/* Internal Function */
static void SetKeyOutput(uint8_t KeyInput);
static uint8_t MY_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
/* External Function */
extern void RelayControl(uint8_t which_relay, FunctionalState NewState);
extern KEY_TypeDef GetKey(void);

#endif
