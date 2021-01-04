#include "main.h"

void Peripheral_Init_And_SelfCheck(void);
void CueBaseMenu(void);
void CueSettingMenu1(void);
void CueSettingMenu2(void);
void CursorOn(void);
void CursorOff(void);
void UntilKeyRelease(KEY_TypeDef key);
void MeasureAction(void);
void CueCalibrationON(void);
void CueCalibrationOFF(void);

#define MAIN_SETTING_STORE_START_ADDRESS 0x0000
#define MEASURE_DATA_STORE_START_ADDRESS 0x0040
#define K_SETTING_STORE_START_ADDRESS 0x0030

typedef struct
{
	float ID;
	uint16_t relay_A_time;
	uint16_t relay_B_time;
	uint16_t stable_time;
	uint16_t record_counter;
}MAIN_SETTING_T;

MAIN_SETTING_T main_setting_store; 

typedef struct
{
	float ID;
	float K;
}MAIN_K_T;

MAIN_K_T main_k;

typedef enum
{
	BASE_MENU = 0,
	SETTING_MENU_1,
	SETTING_MENU_2,
	CALIBRA_MENU_ON,
}MENU_PAGE_T;

KEY_TypeDef main_get_key;
MENU_PAGE_T menu_page = BASE_MENU;
MENU_PAGE_T menu_page_last = SETTING_MENU_1;
uint8_t menu_force_update = 0;
MAIN_DATA maindata;

uint8_t ds18b20_state = 1;
uint8_t eeprom_state = 1;
uint8_t ads1220_state = 1;

uint8_t init_char_1[] = {"DS18B20 ready!"};
uint8_t init_char_2[] = {"DS18B20 not found"};
uint8_t init_char_3[] = {"EEPROM ready!"};
uint8_t init_char_4[] = {"EEPROM not found"};
uint8_t init_char_5[] = {"WaitforScale"};
uint8_t init_char_6[] = {"Scale ready!"};
uint8_t init_char_7[] = {"ADS1220 ready!"};
uint8_t init_char_8[] = {"ADS1220 error"};
uint8_t init_char_9[] = {"Enter System!"};

uint8_t setting_menu_char_1[] = {"入口继电器延时: "};
uint8_t setting_menu_char_2[] = {"出口继电器延时: "};
uint8_t setting_menu_char_3[] = {" (秒) "};
uint8_t setting_menu_relay_A_time[5] = {0};
uint8_t setting_menu_relay_B_time[5] = {0};
uint16_t relay_A_time = 50;
uint16_t relay_B_time = 50;
int8_t setting_menu_temp1[4][2] = {{0x90,5},{0x91,0},{0x98,5},{0x99,0}};
int8_t setting_menu_cursor_1 = 0;

uint8_t setting_menu_char_4[] = {"静置延时: "};
uint8_t setting_menu_char_5[] = {"测序编号: "};
uint8_t setting_menu_stable_time[5] = {0};
uint16_t stable_time = 500;
uint8_t setting_menu_counter[5] = {0};
uint16_t record_counter = 0;
int8_t setting_menu_temp2[6][2] = {{0x90,50},{0x91,0},{0x98,0},{0x99,0},{0x9A,0},{0x9B,0}};
int8_t setting_menu_cursor_2 = 0;

uint8_t start_menu_char_1[] = {"入口开启: "};
uint8_t start_menu_char_2[] = {"等待静置: "};
uint8_t start_menu_char_3[] = {"出口开启: "};
uint8_t start_menu_char_4[] = {"密度"};
uint8_t start_menu_char_5[] = {"秒"};
uint8_t start_menu_char_6[] = {"g/cm^3"};
uint16_t start_menu_delay[3] = {0};
uint8_t start_menu_delay_disp[3][5] = {0};
uint16_t measure_data = 0;

uint8_t calibration_menu_char_1[] = {"开始校准！"};
uint8_t calibration_menu_char_2[] = {"结束校准！"};
uint8_t calibration_menu_char_3[] = {". "};
float calibration_k = 0.0f;
float calibration_estimate = 0.0f;
uint8_t calibration_estimate_char[6] = {0};
int8_t calibration_temp[2][2] = {{0x9A,00},{0x9C,00}};
int8_t calibration_cursor = 0;

double estimate_sumup = 0.0;
double estimate_ave = 0.0;
uint8_t estimate_char[7] = {0};

float cal_k = 260.0;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(72);
	BSP_Init();
	
	Peripheral_Init_And_SelfCheck();
	
	EEPROM_Continute_Read(MAIN_SETTING_STORE_START_ADDRESS, (uint8_t*)&main_setting_store, sizeof(MAIN_SETTING_T));
	EEPROM_Continute_Read(K_SETTING_STORE_START_ADDRESS, (uint8_t*)&main_k, sizeof(MAIN_K_T));
	
	if(main_setting_store.ID >= 0.108f)
	{
		relay_A_time = main_setting_store.relay_A_time;
		relay_B_time = main_setting_store.relay_B_time;
		record_counter = main_setting_store.record_counter;
		stable_time = main_setting_store.stable_time;
		
		setting_menu_temp1[0][1] = relay_A_time/10;
		setting_menu_temp1[1][1] = relay_A_time%10;
		setting_menu_temp1[2][1] = relay_B_time/10;
		setting_menu_temp1[3][1] = relay_B_time%10;
		
		setting_menu_temp2[0][1] = stable_time/10;
		setting_menu_temp2[1][1] = stable_time%10;
		setting_menu_temp2[2][1] = record_counter / 1000;
		setting_menu_temp2[3][1] = record_counter % 1000 / 100;
		setting_menu_temp2[4][1] = record_counter % 100 / 10;
		setting_menu_temp2[5][1] = record_counter % 10;
	}
	
	if(main_k.ID >= 0.108f)
	{
//		cal_k = main_k.K;
	}
	
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	
	while(1)
	{
		if((menu_page != menu_page_last)||(menu_force_update != 0))
		{
			switch(menu_page)
			{
				case BASE_MENU:
					CueBaseMenu();
				break;
				
				case SETTING_MENU_1:
					CueSettingMenu1();
				break;
				
				case SETTING_MENU_2:
					CueSettingMenu2();
				break;
				
				case CALIBRA_MENU_ON:
					CueCalibrationON();
				break;
				
				default:
					break;
			}
			menu_page_last = menu_page;
			menu_force_update = 0;
		}
		
		main_get_key = GetKey();
		switch(main_get_key)
		{
			case KEY1:
				UntilKeyRelease(main_get_key);
				if(menu_page == SETTING_MENU_1)
				{
					setting_menu_cursor_1 -= 1;
					if(setting_menu_cursor_1 == -1)setting_menu_cursor_1 = 3;
				}
				if(menu_page == SETTING_MENU_2)
				{
					setting_menu_cursor_2 -= 1;
					if(setting_menu_cursor_2 == -1)setting_menu_cursor_2 = 5;
				}
				if(menu_page == CALIBRA_MENU_ON)
				{
					calibration_cursor -= 1;
					if(calibration_cursor == -1)calibration_cursor = 1;
				}
				menu_force_update = 1;
				break;
				
			case KEY2:
				UntilKeyRelease(main_get_key);
				if(menu_page == SETTING_MENU_1)
				{
					setting_menu_cursor_1 += 1;
					if(setting_menu_cursor_1 == 4)setting_menu_cursor_1 = 0;
				}
				if(menu_page == SETTING_MENU_2)
				{
					setting_menu_cursor_2 += 1;
					if(setting_menu_cursor_2 == 6)setting_menu_cursor_2 = 0;
				}
				if(menu_page == CALIBRA_MENU_ON)
				{
					calibration_cursor += 1;
					if(calibration_cursor == 2)calibration_cursor = 0;
				}
				menu_force_update = 1;
				break;
				
			case KEY3:
//				UntilKeyRelease(main_get_key);
				if(menu_page == SETTING_MENU_1)
				{
					switch(setting_menu_cursor_1)
					{
						case 0:
						case 2:
							setting_menu_temp1[setting_menu_cursor_1][1] += 1;
							if(setting_menu_temp1[setting_menu_cursor_1][1] >= 100)setting_menu_temp1[setting_menu_cursor_1][1] = 0;
							break;
						
						case 1:
						case 3:
							setting_menu_temp1[setting_menu_cursor_1][1] += 1;
							if(setting_menu_temp1[setting_menu_cursor_1][1] >= 10)setting_menu_temp1[setting_menu_cursor_1][1] = 0;
							break;
						
						default:
							break;
					}
				}
				
				else if(menu_page == SETTING_MENU_2)
				{
					switch(setting_menu_cursor_2)
					{
						case 0:
							setting_menu_temp2[setting_menu_cursor_2][1] += 1;
							if(setting_menu_temp2[setting_menu_cursor_2][1] >= 100)setting_menu_temp2[setting_menu_cursor_2][1] = 0;
							break;
						
						case 1:
							setting_menu_temp2[setting_menu_cursor_2][1] += 1;
							if(setting_menu_temp2[setting_menu_cursor_2][1] >= 10)setting_menu_temp2[setting_menu_cursor_2][1] = 0;
							break;
						
						case 2:
						case 3:
						case 4:
						case 5:
							setting_menu_temp2[setting_menu_cursor_2][1] += 1;
							if(setting_menu_temp2[setting_menu_cursor_2][1] >= 10)setting_menu_temp2[setting_menu_cursor_2][1] = 0;
							break;
						
						default:
							break;
					}
				}
				
				else if(menu_page == CALIBRA_MENU_ON)
				{
					switch(calibration_cursor)
					{
						case 0:
						case 1:
							calibration_temp[calibration_cursor][1] += 1;
							if(calibration_temp[calibration_cursor][1] >= 100)calibration_temp[calibration_cursor][1] = 0;
							break;
						default:
							break;
					}
				}
				
				relay_A_time = setting_menu_temp1[0][1] * 10 + setting_menu_temp1[1][1];
				relay_B_time = setting_menu_temp1[2][1] * 10 + setting_menu_temp1[3][1];
				stable_time = setting_menu_temp2[0][1] * 10 + setting_menu_temp2[1][1];
				record_counter = setting_menu_temp2[2][1] * 1000 + setting_menu_temp2[3][1] * 100 + setting_menu_temp2[4][1] * 10 + setting_menu_temp2[5][1];
				if(record_counter > 5000)record_counter = 5000;
				calibration_estimate = calibration_temp[0][1] + (float)calibration_temp[1][1] / 100;
				
				menu_force_update = 1;
				bsp_delayms(150);
				break;
				
			case KEY4:
//				UntilKeyRelease(main_get_key);
				if(menu_page == SETTING_MENU_1)
				{
					switch(setting_menu_cursor_1)
					{
						case 0:
						case 2:
							setting_menu_temp1[setting_menu_cursor_1][1] -= 1;
							if(setting_menu_temp1[setting_menu_cursor_1][1] <= -1)setting_menu_temp1[setting_menu_cursor_1][1] = 99;
							break;
						
						case 1:
						case 3:
							setting_menu_temp1[setting_menu_cursor_1][1] -= 1;
							if(setting_menu_temp1[setting_menu_cursor_1][1] <= -1)setting_menu_temp1[setting_menu_cursor_1][1] = 9;
							break;
						
						default:
							break;
					}
				}
				
				else if(menu_page == SETTING_MENU_2)
				{
					switch(setting_menu_cursor_2)
					{
						case 0:
							setting_menu_temp2[setting_menu_cursor_2][1] -= 1;
							if(setting_menu_temp2[setting_menu_cursor_2][1] <= -1)setting_menu_temp2[setting_menu_cursor_2][1] = 99;
							break;
						
						case 1:
							setting_menu_temp2[setting_menu_cursor_2][1] -= 1;
							if(setting_menu_temp2[setting_menu_cursor_2][1] <= -1)setting_menu_temp2[setting_menu_cursor_2][1] = 9;
							break;
						
						case 2:
						case 3:
						case 4:
						case 5:
							setting_menu_temp2[setting_menu_cursor_2][1] -= 1;
							if(setting_menu_temp2[setting_menu_cursor_2][1] <= -1)setting_menu_temp2[setting_menu_cursor_2][1] = 9;
							break;
						
						default:
							break;
					}
				}
				
				else if(menu_page == CALIBRA_MENU_ON)
				{
					switch(calibration_cursor)
					{
						case 0:
						case 1:
							calibration_temp[calibration_cursor][1] -= 1;
							if(calibration_temp[calibration_cursor][1] <= -1)calibration_temp[calibration_cursor][1] = 99;
							break;
						default:
							break;
					}
				}
				
				relay_A_time = setting_menu_temp1[0][1] * 10 + setting_menu_temp1[1][1];
				relay_B_time = setting_menu_temp1[2][1] * 10 + setting_menu_temp1[3][1];
				stable_time = setting_menu_temp2[0][1] * 10 + setting_menu_temp2[1][1];
				record_counter = setting_menu_temp2[2][1] * 1000 + setting_menu_temp2[3][1] * 100 + setting_menu_temp2[4][1] * 10 + setting_menu_temp2[5][1];
				if(record_counter > 5000)record_counter = 5000;
				calibration_estimate = calibration_temp[0][1] + (float)calibration_temp[1][1] / 100;
				menu_force_update = 1;
				bsp_delayms(150);
				break;
			
			case KEY5:
				UntilKeyRelease(main_get_key);
				if(menu_page == BASE_MENU) menu_page = SETTING_MENU_1;
				else if(menu_page == SETTING_MENU_1) menu_page = SETTING_MENU_2;
				else
				{
					menu_page = BASE_MENU;
					main_setting_store.ID = 0.108f;
					main_setting_store.record_counter = record_counter;
					main_setting_store.relay_A_time = relay_A_time;
					main_setting_store.relay_B_time = relay_B_time;
					main_setting_store.stable_time = stable_time;
					EEPROM_Continute_Write(MAIN_SETTING_STORE_START_ADDRESS, (uint8_t*)&main_setting_store, sizeof(MAIN_SETTING_T));					
				}
				break;
			
			case KEY6:
				UntilKeyRelease(main_get_key);
				if(menu_page == BASE_MENU)
				{
					MeasureAction();
					menu_force_update = 1;
					main_setting_store.record_counter = record_counter;
					EEPROM_Continute_Write(MAIN_SETTING_STORE_START_ADDRESS, (uint8_t*)&main_setting_store, sizeof(MAIN_SETTING_T));
					
					//打开以实现存储功能
//					EEPROM_Continute_Write( (MEASURE_DATA_STORE_START_ADDRESS + (record_counter * sizeof(double))), 
//						(uint8_t*)&estimate_ave, sizeof(double));
				}
				else if(menu_page == CALIBRA_MENU_ON)
				{
					CursorOff();
					MeasureAction();
					calibration_k = cal_k * ((float)calibration_estimate / estimate_ave);
					cal_k = calibration_k;
					main_k.K = calibration_k;
					main_k.ID = 0.108f;
					EEPROM_Continute_Write(K_SETTING_STORE_START_ADDRESS, (uint8_t*)&main_k, sizeof(MAIN_K_T));	
					
					CueCalibrationOFF();
					menu_page = BASE_MENU;
				}
				break;
			
			case KEY7:
				UntilKeyRelease(main_get_key);
				if(menu_page == BASE_MENU)
				{
					menu_page = CALIBRA_MENU_ON;
				}					
			
				break;
			case KEY8:
				break;
			default:
				break;
		}
	}
}

void Peripheral_Init_And_SelfCheck(void)
{
	//等待显示屏准备完毕
	bsp_delayms(1000);
	//显示屏初始化
	J_LCD_Init();
	
	WRCommand(0x01); //清除显示DDRAM
	//检查温度传感器状态，就位后为0
	ds18b20_state = DS18B20_Init();
	if(!ds18b20_state)ShowEngChar(0x80,init_char_1,sizeof(init_char_1)-1);
	else ShowEngChar(0x80,init_char_2,sizeof(init_char_2)-1);
	//检查EEPROM状态，就位后为0
	EEPROM_Get_ID();
	if(eeprom_param.eeprom_id == 0x29)eeprom_state = 0;
	if(!eeprom_state)ShowEngChar(0x90,init_char_3,sizeof(init_char_3)-1);
	else ShowEngChar(0x90,init_char_4,sizeof(init_char_4)-1);
	//称重ADC初始化
	ShowEngChar(0x88,init_char_5,sizeof(init_char_5)-1);
	ScaleInit();
	ShowEngChar(0x88,init_char_6,sizeof(init_char_6)-1);
	//ADS1220初始化
	ads1220_state = ADS1220_Init();
	if(!ads1220_state)ShowEngChar(0x98,init_char_7,sizeof(init_char_7)-1);
	else ShowEngChar(0x98,init_char_8,sizeof(init_char_8)-1);
	
	//清屏，显示进入系统
	bsp_delayms(1000);
	WRCommand(0x01);
	ShowEngChar(0x80,init_char_9,sizeof(init_char_9)-1);
	bsp_delayms(1000);
}

void CueBaseMenu(void)
{	
	start_menu_delay[0] = relay_A_time;
	start_menu_delay[1] = stable_time;
	start_menu_delay[2] = relay_B_time;
	
	if(menu_force_update == 0)
	{
		WRCommand(0x01);
		CursorOff();
		
		ShowQQChar(0x80,start_menu_char_1,(sizeof(start_menu_char_1)-1)/2);
		ShowQQChar(0x87,start_menu_char_5,(sizeof(start_menu_char_5)-1)/2);
		ShowQQChar(0x90,start_menu_char_2,(sizeof(start_menu_char_2)-1)/2);
		ShowQQChar(0x97,start_menu_char_5,(sizeof(start_menu_char_5)-1)/2);
		ShowQQChar(0x88,start_menu_char_3,(sizeof(start_menu_char_3)-1)/2);
		ShowQQChar(0x8F,start_menu_char_5,(sizeof(start_menu_char_5)-1)/2);
		ShowQQChar(0x98,start_menu_char_4,(sizeof(start_menu_char_4)-1)/2);
		ShowQQChar(0x9D,start_menu_char_6,(sizeof(start_menu_char_6)-1)/2);
	}
	
	sprintf(&start_menu_delay_disp[0][0],"%04.1f",(float)start_menu_delay[0]/10);
	sprintf(&start_menu_delay_disp[1][0],"%04.1f",(float)start_menu_delay[1]/10);
	sprintf(&start_menu_delay_disp[2][0],"%04.1f",(float)start_menu_delay[2]/10);
	
	ShowEngChar(0x85,start_menu_delay_disp[0],4);
	ShowEngChar(0x95,start_menu_delay_disp[1],4);
	ShowEngChar(0x8D,start_menu_delay_disp[2],4);
}

void CueSettingMenu1(void)
{
	if(menu_force_update == 0)WRCommand(0x01);
	sprintf(setting_menu_relay_A_time,"%04.1f",(float)relay_A_time/10);
	sprintf(setting_menu_relay_B_time,"%04.1f",(float)relay_B_time/10);
	if(menu_force_update == 0)ShowQQChar (0x80,setting_menu_char_1,(sizeof(setting_menu_char_1)-1)/2);
	if(menu_force_update == 0)ShowQQChar (0x92,setting_menu_char_3,(sizeof(setting_menu_char_3)-1)/2);
	ShowEngChar(0x90,setting_menu_relay_A_time,sizeof(setting_menu_relay_A_time)-1);
	if(menu_force_update == 0)ShowQQChar (0x88,setting_menu_char_2,(sizeof(setting_menu_char_2)-1)/2);
	if(menu_force_update == 0)ShowQQChar (0x9A,setting_menu_char_3,(sizeof(setting_menu_char_3)-1)/2);
	ShowEngChar(0x98,setting_menu_relay_B_time,sizeof(setting_menu_relay_B_time)-1);
	CursorOn();
	WRCommand(setting_menu_temp1[setting_menu_cursor_1][0]);
}

void CueSettingMenu2(void)
{
	if(menu_force_update == 0)WRCommand(0x01);
	sprintf(setting_menu_stable_time,"%04.1f",(float)stable_time/10);
	sprintf(setting_menu_counter,"%04d",record_counter);
	if(menu_force_update == 0)ShowQQChar (0x80,setting_menu_char_4,(sizeof(setting_menu_char_4)-1)/2);
	if(menu_force_update == 0)ShowQQChar (0x92,setting_menu_char_3,(sizeof(setting_menu_char_3)-1)/2);
	if(menu_force_update == 0)ShowQQChar (0x88,setting_menu_char_5,(sizeof(setting_menu_char_5)-1)/2);
	ShowEngChar(0x90,setting_menu_stable_time,sizeof(setting_menu_stable_time)-1);
	
	ShowEngChar(0x98,&setting_menu_counter[0],1);
	ShowEngChar(0x99,&setting_menu_counter[1],1);
	ShowEngChar(0x9A,&setting_menu_counter[2],1);
	ShowEngChar(0x9B,&setting_menu_counter[3],1);
	CursorOn();
	WRCommand(setting_menu_temp2[setting_menu_cursor_2][0]);
}

void CueCalibrationON(void)
{
	if(menu_force_update == 0)
	{
		WRCommand(0x01);
		ShowQQChar(0x80,calibration_menu_char_1,(sizeof(calibration_menu_char_1)-1)/2);
		bsp_delay100ms(20);
		CueBaseMenu();
		ShowQQChar(0x9B,calibration_menu_char_3,(sizeof(calibration_menu_char_3)-1)/2);
		CursorOn();
	}
	sprintf(calibration_estimate_char,"%05.2f",(float)calibration_estimate);
	ShowEngChar(0x9A,&calibration_estimate_char[0],2);
	ShowEngChar(0x9C,&calibration_estimate_char[3],2);
	
	WRCommand(calibration_temp[calibration_cursor][0]);
}

void CueCalibrationOFF(void)
{
	WRCommand(0x01);
	ShowQQChar(0x80,calibration_menu_char_2,(sizeof(calibration_menu_char_2)-1)/2);
	bsp_delay100ms(10);
}

KEY_TypeDef measure_start_get_key = KEY_NONE;

uint16_t counter_f = 0;

void MeasureAction(void)
{
	uint32_t time_end = 0;
	uint32_t time_left = 0;
	
	time_end = bspdata.systime + start_menu_delay[0];
	RelayControl(1,ENABLE);
	while(bspdata.systime <= time_end)
	{
		measure_start_get_key = GetKey();
		if(measure_start_get_key == KEY6)
		{
			UntilKeyRelease(measure_start_get_key);
			goto MEASUREEND;
		}
		time_left = time_end - bspdata.systime;
		sprintf(&start_menu_delay_disp[0][0],"%04.1f",(float)time_left/10);
		ShowEngChar(0x85,start_menu_delay_disp[0],4);
	}
	RelayControl(1,DISABLE);
	
	time_end = bspdata.systime + start_menu_delay[1];
	while(bspdata.systime <= time_end)
	{
		measure_start_get_key = GetKey();
		if(measure_start_get_key == KEY6)
		{
			UntilKeyRelease(measure_start_get_key);
			goto MEASUREEND;
		}
		time_left = time_end - bspdata.systime;
		sprintf(&start_menu_delay_disp[1][0],"%04.1f",(float)time_left/10);
		ShowEngChar(0x95,start_menu_delay_disp[1],4);
	}
	
	time_end = bspdata.systime + 30;
	estimate_sumup = 0.0;
	counter_f = 0;
	while(bspdata.systime <= time_end)
	{
		maindata.EstimateDensity = (Scale_Data.ScaleData / (maindata.voltage_diff_32_filter_output/cal_k));
		estimate_sumup += maindata.EstimateDensity;
		counter_f++;
		bsp_delayms(10);
	}
	estimate_ave = estimate_sumup / counter_f;
	bsp_delay100ms(10);
	sprintf(estimate_char,"%05.2f",estimate_ave);
	if(menu_page == BASE_MENU)
	{
		ShowEngChar(0x9A,estimate_char,5);
	}
	
	time_end = bspdata.systime + start_menu_delay[2];
	RelayControl(2,ENABLE);
	while(bspdata.systime <= time_end)
	{
		measure_start_get_key = GetKey();
		if(measure_start_get_key == KEY6)
		{
			UntilKeyRelease(measure_start_get_key);
			goto MEASUREEND;
		}
		time_left = time_end - bspdata.systime;
		sprintf(&start_menu_delay_disp[2][0],"%04.1f",(float)time_left/10);
		ShowEngChar(0x8D,start_menu_delay_disp[2],4);
	}
	RelayControl(2,DISABLE);
	record_counter += 1;
	
	
MEASUREEND:
	RelayControl(1,DISABLE);
	RelayControl(2,DISABLE);
}

void CursorOn(void)
{
	WRCommand(0x0D);
}

void CursorOff(void)
{
	WRCommand(0x0C);
}

void UntilKeyRelease(KEY_TypeDef key)
{
	while(key != KEY_NONE)
	{
		key = GetKey();
	}
}

