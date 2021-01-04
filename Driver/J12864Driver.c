#include "J12864Driver.h"

void Short_Delay(void)
{
	uint8_t Counter_I = 0;
	
	for(Counter_I = 0;Counter_I<100;Counter_I++)
	{
		__nop();
	}
}
//TYPE 为0代表命令为控制指令 为1代表命令为显示指令
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
	bsp_delayms(1);//89S52 来模拟串行通信,所以,加上89S52 的延时,
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
	
	WRCommand(0x30); //基本指令集,8 位并行
	WRCommand(0x06); //启始点设定：光标右移
	WRCommand(0x01); //清除显示DDRAM
	WRCommand(0x0C); //显示状态开关：整体显示开，光标显示关，光标显示反白关
	WRCommand(0x02); //地址归零
}

void J_LCD_RESET(void)
{
	J_RST_L;
	bsp_delayms(10);
	J_RST_H;
	bsp_delayms(10);
}

//显示数组字符串(显示半宽字型16*8 点阵)
void ShowEngChar(uint8_t addr,uint8_t *english,uint8_t count)
{
	uint8_t i;
	WRCommand(addr);//设定DDRAM 地址
	for(i=0;i<count;)
	{
		WRData(english[i]);
		i++;
	}
}

//显示数组字符串(显示半宽字型16*8 点阵)
void ShowQQChar(uint8_t addr,uint8_t *english,uint8_t count)
{
	uint8_t i;
	WRCommand(addr);//设定DDRAM 地址
	for(i=0;i<count;)
	{
		WRData(english[i*2]);
		WRData(english[i*2+1]);
		i++;
	}
}
//显示连续字串(半宽字符)
void ShowNUMChar(uint8_t addr,uint8_t i,uint8_t count)
{
	uint8_t j;
	for(j=0;j<count;)
	{
		WRCommand(addr);//设定DDRAM 地址
		WRData(i+j);
		j++;
		WRData(i+j);
		addr++;
		j++;
	}
}
//自定义字符写入CGRAM
void WRCGRAM(uint8_t data1,uint8_t data2,uint8_t addr)
{
	uint8_t i;
	for(i=0;i<16;)
	{
		WRCommand(addr+i); //设定CGRAM 地址
		WRData(data1);
		WRData(data1);
		i++;
		WRCommand(addr+i); //设定CGRAM 地址
		WRData(data2);
		WRData(data2);
		i++;
	}
}

//显示自定义的字符,并把这个字符填满全屏16*16
void ShowCGChar(uint8_t addr,uint8_t i)
{
	uint8_t j;
	for(j=0;j<0x20;)
	{
		WRCommand(addr+j); //设定DDRAM 地址
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
		WRCommand(0x80);//X 坐标
		for(i=0;i<32;i++)//
		{
			WRData(0x00);
		}
	}
}

//写入GDRAM 绘图,Y 是Y 绘图坐标,2 个字节一行,CLONG 是图形长度,以字节
//为单位;HIGHT 是图形高度,TAB 是图形数据表.12864M 的图形显示是相当于256*32 点阵.
//由两屏128*32 上下两屏组成，同一行的下屏的头地址紧接上屏的未地址。
//绘图在串口输入时,会比在并口下的输入要慢一些
void WRGDRAM(uint8_t Y1,uint8_t clong,uint8_t hight,uint8_t *TAB1)
{
	uint16_t k;
	uint8_t j;
	uint8_t i;
	WRCommand(0x34);
	WRCommand(0x36);
	for(j=0;j<hight;j++)//32
	{ //先上半屏
		WRCommand(Y1+j); //Y 总坐标,即第几行
		WRCommand(0x80);//X 坐标，即横数第几个字节开始写起
		for(i=0;i<clong;i++)//
		{
			WRData(TAB1[clong*j+i]);
		}
		//后下半屏
		for(k=0;k<clong;k++)//
		{
			WRData(TAB1[clong*(j+hight)+k]);
		}
	}
}
