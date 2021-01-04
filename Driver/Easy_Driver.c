#include "Easy_Driver.h"

void RelayControl(uint8_t which_relay, FunctionalState NewState)
{
	switch(which_relay)
	{
		case 1 :
			if(NewState != DISABLE) relay_1_out = 0;
			else relay_1_out = 1;
			break;
		case 2 :
			if(NewState != DISABLE) relay_2_out = 0;
			else relay_2_out = 1;
			break;
		default:
			break;
	}
}

void SetKeyOutput(uint8_t KeyInput)
{
	GPIO_WriteBit(columns_1,((KeyInput&0x01)?Bit_SET:Bit_RESET));
	GPIO_WriteBit(columns_2,((KeyInput&0x02)?Bit_SET:Bit_RESET));
	GPIO_WriteBit(columns_3,((KeyInput&0x04)?Bit_SET:Bit_RESET));
	GPIO_WriteBit(columns_4,((KeyInput&0x08)?Bit_SET:Bit_RESET));
}

uint8_t MY_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint8_t cachebitstatus = 0x00;
	uint8_t bitstatus = 0x00;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
	
	cachebitstatus = GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
	if(cachebitstatus == (uint8_t)Bit_SET)
	{
		bsp_delayms(1);
		cachebitstatus = GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
		if(cachebitstatus == (uint8_t)Bit_SET)
		{
			bitstatus = (uint8_t)Bit_SET;
		}
		else bitstatus = (uint8_t)Bit_RESET;
	}
	else bitstatus = (uint8_t)Bit_RESET;
	
	return bitstatus;
}

KEY_TypeDef GetKey(void)
{ 
	uint8_t a,b = 0;
	KEY_TypeDef getdata = KEY_NONE;
	uint8_t shiftData = 0x01;
	uint8_t i;
	for(i=0;i<4;i++)
	{
		SetKeyOutput(shiftData);
		a = MY_ReadInputDataBit(rows_1);
		b = MY_ReadInputDataBit(rows_2);
		if(a==1) getdata |= (0x01<<i);
		if(b==1) getdata |= (0x01<<(i+4));
		shiftData<<=1;
	}
	return getdata;
}

