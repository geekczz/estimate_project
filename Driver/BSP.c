#include "BSP.h"

BSP_DATA bspdata;


void BSP_Init(void)
{
	BSP_GPIO_Init();
	BSP_SPI_Init();
	BSP_USART_Init();
	BSP_DMA_Init();
	BSP_TIM_Init();
}

void BSP_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_ITD;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/**********************DS18B20初始化***********************/
	//PA0 in&out
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_0;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_ITD);
	/**********************DS18B20初始化***********************/
	
	/**********************电子秤接口初始化***********************/
	//PA1 DOUT in
	//PA2 SCK out
	GPIO_ITD.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_1;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_ITD);
	
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_2;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_ITD);
	/**********************电子秤接口初始化***********************/
	
	/**********************矩阵键盘接口初始化***********************/
	//Row1 PA3 in
	//Row2 PA4 in
	//Column1 PA5 out
	//Column2 PA6 out
	//Column3 PA7 out
	//Column4 PB0 out
	GPIO_ITD.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_ITD);
	
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_ITD);
	
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_0;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_ITD);
	/**********************矩阵键盘接口初始化***********************/
	
	/**********************ADS1220接口初始化***********************/
	//CS PB10 out
	//CLK PB11 out
	//DRDY PA10 in
	//DOUT PA11 in
	//DIN PA12 out
	GPIO_ITD.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_ITD);
	
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_ITD);
	
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_12;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_ITD);
	/**********************ADS1220接口初始化***********************/
	
	/**********************EEPROM接口初始化***********************/
	//CS PB12 out
	//SCK PB13 af
	//SDO PB14 af
	//SDI PB15 af
	//WP PA8 out
	//HOLD PA9 out
	GPIOB->ODR |= 1<<12;
	GPIOA->ODR |= 3<<8;
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_12;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_ITD);
	
	GPIO_ITD.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_ITD);
	
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_ITD);
	/**********************EEPROM接口初始化***********************/
	
	/**********************显示屏接口初始化***********************/
	//RST PA15 out
	//CLK PB3 out
	//SID PB4 out
	//CS PB5 out
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_15;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_ITD);
	
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_ITD);
	/**********************显示屏接口初始化***********************/
	
	/**********************串口初始化***********************/
	//U1_RX PB7
	//U1_TX PB6
	GPIO_ITD.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_6;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_ITD);
	
	GPIO_ITD.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_7;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_ITD);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	/**********************串口初始化***********************/
	
	/**********************继电器初始化***********************/
	//relay1 PB9 out
	//relay2 PB8 out
	GPIOB->ODR |= 0x00000300;
	GPIO_ITD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_ITD.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_ITD.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_ITD);
	/**********************继电器初始化***********************/
}

void BSP_SPI_Init(void)
{
	SPI_InitTypeDef SPI_ITD;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	SPI_ITD.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_ITD.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_ITD.SPI_CPOL = SPI_CPOL_Low;
	SPI_ITD.SPI_CRCPolynomial = 7;
	SPI_ITD.SPI_DataSize = SPI_DataSize_8b;
	SPI_ITD.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_ITD.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_ITD.SPI_Mode = SPI_Mode_Master;
	SPI_ITD.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2,&SPI_ITD);
	
	SPI_Cmd(SPI2,ENABLE);
}

void BSP_USART_Init(void)
{
	USART_InitTypeDef USART_ITD;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	USART_ITD.USART_BaudRate = 115200;
	USART_ITD.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_ITD.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_ITD.USART_Parity = USART_Parity_No;
	USART_ITD.USART_StopBits = USART_StopBits_1;
	USART_ITD.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_ITD);
	
	USART_Cmd(USART1,ENABLE);
}

void BSP_DMA_Init(void)
{
	DMA_InitTypeDef DMA_ITD;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//USART1_RX Channel5
	DMA_ITD.DMA_BufferSize = 5;
	DMA_ITD.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_ITD.DMA_M2M = DMA_M2M_Disable;
	DMA_ITD.DMA_MemoryBaseAddr = (uint32_t)bspdata.U1_ReceiveData;
	DMA_ITD.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_ITD.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_ITD.DMA_Mode = DMA_Mode_Circular;
	DMA_ITD.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
	DMA_ITD.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_ITD.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_ITD.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel5, &DMA_ITD);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
}

void BSP_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TBITD;
	NVIC_InitTypeDef NVIC_ITD;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TBITD.TIM_ClockDivision = 2-1;
	TIM_TBITD.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TBITD.TIM_Period = 10000 - 1;
	TIM_TBITD.TIM_Prescaler = 720 - 1;
	TIM_TBITD.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TBITD);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	
	NVIC_ITD.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_ITD.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_ITD.NVIC_IRQChannelSubPriority = 0;
	NVIC_ITD.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_ITD);
	
	
	
	TIM_TBITD.TIM_ClockDivision = 2-1;
	TIM_TBITD.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TBITD.TIM_Period = 20 - 1;
	TIM_TBITD.TIM_Prescaler = 7200 - 1;
	TIM_TBITD.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TBITD);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,DISABLE);
	
	NVIC_ITD.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_ITD.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_ITD.NVIC_IRQChannelSubPriority = 1;
	NVIC_ITD.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_ITD);
	
	
	
	TIM_TBITD.TIM_ClockDivision = 2-1;
	TIM_TBITD.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TBITD.TIM_Period = 5000 - 1;
	TIM_TBITD.TIM_Prescaler = 7200 - 1;
	TIM_TBITD.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TBITD);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,DISABLE);
	
	NVIC_ITD.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_ITD.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_ITD.NVIC_IRQChannelSubPriority = 1;
	NVIC_ITD.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_ITD);
}

void bsp_delayms(u16 counter)
{
//	u32 temp = counter+bspdata.systime;
//	while(bspdata.systime<temp);
//	bspdata.systime = 0;
	delay_ms(counter);
}

void bsp_delay100ms(u16 counter)
{
	u8  repeat=counter / 500;									
	u16 remain=counter % 500;
	while(repeat)
	{
		bsp_delayms(50000);
		repeat--;
	}
	if(remain)bsp_delayms(remain*100);
}

void fun_usartSend(USART_TypeDef *com ,u8 data)
{
  while(USART_GetFlagStatus(com, USART_FLAG_TC) == RESET)
  {}
  USART_SendData(com, (u8) data);
}

/********************************************************************************************************
Function Name: 
Author       : xiaobo
Date         : 2013-03-14
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
u8 fun_usartRec(USART_TypeDef *com)
{
  while(USART_GetFlagStatus(com, USART_FLAG_RXNE) == RESET)
  {}
  return USART_ReceiveData(com);
}

/********************************************************************************************************
Function Name: 
Author       : xiaobo
Date         : 2014-03-14
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
u8 fun_spiSwap(SPI_TypeDef* spix,u8 data)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(spix, SPI_I2S_FLAG_TXE) == RESET)      //?????????
  {

  }			  
  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(spix, data);                                    //????SPI1??????

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(spix, SPI_I2S_FLAG_RXNE) == RESET)   //??????????
  {

  }	  						    
  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(spix);                  
}

int fun_printf(USART_TypeDef* com, char * plog,...)
{
    int nret = 0;
    va_list args;
    char data[30];
    char *p;
    if (plog == NULL) {
      return nret;
    }	
    va_start(args, plog);
    nret = vsprintf(data,plog,args);
    p = data;
    while(*p)
    {
      fun_usartSend(com, *p++);
    }
    va_end(args);
    return nret;
}
