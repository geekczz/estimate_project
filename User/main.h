#ifndef __MAIN_H
#define __MAIN_H

#include "GLOBAL.h"

typedef struct MAIN_DATA
{
	float UltrasonicAcquireVoltage;
	float LiquidV;
	float EstimateDensity;
	
	int32_t WeightFilterOutput;
	int32_t voltage_diff_32_orign;
	int32_t voltage_diff_32_filter_output;
}MAIN_DATA;

extern MAIN_DATA maindata;

#endif 
