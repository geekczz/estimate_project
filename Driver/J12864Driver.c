#include "J12864Driver.h"

void Short_Delay(void)
{
	uint8_t Counter_I = 0;
	
	for(Counter_I = 0;Counter_I<100;Counter_I++)
	{
		__nop();
	}
}
//TYPE Ϊ0��������Ϊ����ָ�� Ϊ1��������Ϊ��ʾָ��
void SendByteLCD(uint8_t WLCDData)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if((WLCDData<<i)&0x80)J_SID_H;
		else J_SID_L;
		Short_Delay();
		J_CLK_L;
		Short_Delay();
		J_CLK_H;
	}
}
void SPIWR(uint8_t Wdata,uint8_t WRS)
{
	SendByteLCD(0xf8+(WRS<<1));
	SendByteLCD(Wdata&0xf0);
	SendByteLCD((Wdata<<4)&0xf0);
}
void WRCommand(uint8_t CMD)
{
	J_CS_L;
	Short_Delay();
	J_CS_H;
	SPIWR(CMD,0);
	bsp_delayms(1);//89S52 ��ģ�⴮��ͨ��,����,����89S52 ����ʱ,
}
void WRData(uint8_t Data)
{
	J_CS_L;
	Short_Delay();
	J_CS_H;
	SPIWR(Data,1);
}


void J_LCD_Init(void)
{
	J_LCD_RESET();
	
	WRCommand(0x30); //����ָ�,8 λ����
	WRCommand(0x06); //��ʼ���趨���������
	WRCommand(0x01); //�����ʾDDRAM
	WRCommand(0x0C); //��ʾ״̬���أ�������ʾ���������ʾ�أ������ʾ���׹�
	WRCommand(0x02); //��ַ����
}

void J_LCD_RESET(void)
{
	J_RST_L;
	bsp_delayms(10);
	J_RST_H;
	bsp_delayms(10);
}

//��ʾ�����ַ���(��ʾ�������16*8 ����)
void ShowEngChar(uint8_t addr,uint8_t *english,uint8_t count)
{
	uint8_t i;
	WRCommand(addr);//�趨DDRAM ��ַ
	for(i=0;i<count;)
	{
		WRData(english[i]);
		i++;
	}
}

//��ʾ�����ַ���(��ʾ�������16*8 ����)
void ShowQQChar(uint8_t addr,uint8_t *english,uint8_t count)
{
	uint8_t i;
	WRCommand(addr);//�趨DDRAM ��ַ
	for(i=0;i<count;)
	{
		WRData(english[i*2]);
		WRData(english[i*2+1]);
		i++;
	}
}
//��ʾ�����ִ�(����ַ�)
void ShowNUMChar(uint8_t addr,uint8_t i,uint8_t count)
{
	uint8_t j;
	for(j=0;j<count;)
	{
		WRCommand(addr);//�趨DDRAM ��ַ
		WRData(i+j);
		j++;
		WRData(i+j);
		addr++;
		j++;
	}
}
//�Զ����ַ�д��CGRAM
void WRCGRAM(uint8_t data1,uint8_t data2,uint8_t addr)
{
	uint8_t i;
	for(i=0;i<16;)
	{
		WRCommand(addr+i); //�趨CGRAM ��ַ
		WRData(data1);
		WRData(data1);
		i++;
		WRCommand(addr+i); //�趨CGRAM ��ַ
		WRData(data2);
		WRData(data2);
		i++;
	}
}

//��ʾ�Զ�����ַ�,��������ַ�����ȫ��16*16
void ShowCGChar(uint8_t addr,uint8_t i)
{
	uint8_t j;
	for(j=0;j<0x20;)
	{
		WRCommand(addr+j); //�趨DDRAM ��ַ
		WRData(0x00);
		WRData(i);
		j++;
	}
}

void CLEARGDRAM(void)
{
	uint8_t j;
	uint8_t i;
	WRCommand(0x34);
	WRCommand(0x36);
	for(j=0;j<32;j++)
	{
		WRCommand(0x80+j);
		WRCommand(0x80);//X ����
		for(i=0;i<32;i++)//
		{
			WRData(0x00);
		}
	}
}

//д��GDRAM ��ͼ,Y ��Y ��ͼ����,2 ���ֽ�һ��,CLONG ��ͼ�γ���,���ֽ�
//Ϊ��λ;HIGHT ��ͼ�θ߶�,TAB ��ͼ�����ݱ�.12864M ��ͼ����ʾ���൱��256*32 ����.
//������128*32 ����������ɣ�ͬһ�е�������ͷ��ַ����������δ��ַ��
//��ͼ�ڴ�������ʱ,����ڲ����µ�����Ҫ��һЩ
void WRGDRAM(uint8_t Y1,uint8_t clong,uint8_t hight,uint8_t *TAB1)
{
	uint16_t k;
	uint8_t j;
	uint8_t i;
	WRCommand(0x34);
	WRCommand(0x36);
	for(j=0;j<hight;j++)//32
	{ //���ϰ���
		WRCommand(Y1+j); //Y ������,���ڼ���
		WRCommand(0x80);//X ���꣬�������ڼ����ֽڿ�ʼд��
		for(i=0;i<clong;i++)//
		{
			WRData(TAB1[clong*j+i]);
		}
		//���°���
		for(k=0;k<clong;k++)//
		{
			WRData(TAB1[clong*(j+hight)+k]);
		}
	}
}
