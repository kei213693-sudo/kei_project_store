/*
 * uwbTaskApp.h
 *
 *  Created on: 2024年7月19日
 *      Author: F704010638_
 */
#ifndef _UWBTASKAPP_H_
#define _UWBTASKAPP_H_
#include "phscaTypes.h"



#define MODE_RANGE         0
#define MODE_CPD           1
#define MODE_RANGE_CPD     2
#define MODE_CPD_RANGE     3

#define MAX_KEY_NUM       4

typedef struct
{
	uint8_t range_state;
	uint8_t cpd_state;
	uint16_t distance1[MAX_KEY_NUM];
	uint8_t  result; /*!< 本次检测结果 */
	float    motion;    /*!< 运动特征值*/
	float    frq;
	float 	amp_bfeature;/*!< 呼吸特征值*/
	float 	phs_bfeature;
}Range_Cpd_DataType;

extern void uwbTaskApp_Init(void);
extern void uwbTaskApp_PeriodicTask(uint32_t * const xLastWakeTime);
void uwbTaskApp_29d6WeakupCmd(void);
extern Range_Cpd_DataType* uwbTaskApp_GetRangeAndCpdResult(void);
void TestMode_Range();
void TestMode_Cpd();
void TestMode_Range_Cpd();
void TestMode_Cpd_Range();

#endif
