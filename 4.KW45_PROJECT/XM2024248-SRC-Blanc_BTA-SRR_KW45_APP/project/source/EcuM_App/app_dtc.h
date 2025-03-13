/*
 * app_dtc.h
 *
 *  Created on: 2024年7月1日
 *      Author: F704012276_
 */

#ifndef APP_DTC_H_
#define APP_DTC_H_

#define PMS_NORMAL	0
#define PMS_UNDER	1
#define PMS_ABOVE	2

#define DTC_NORMAL_VOLTAGE	0
#define DTC_OVER_VOLTAGE	1
#define DTC_UNDER_VOLTAGE	2

void AppDtc_MainFunction(void);
uint8_t Get_PMState(void);

typedef union{
	uint8_t data;
	struct{
		uint8_t b_busoffUpdate:1;
		uint8_t b_underVoltageUpdate:1;
		uint8_t b_OverVoltageUpdate:1;
		uint8_t b_loseControlUpdate:1;
		uint8_t b_watchRstUpdate:1;
		uint8_t b_HVDRstUpdate:1;
		uint8_t reserved:2;
	}b;
} DtcState_t;

#endif /* APP_DTC_H_ */
