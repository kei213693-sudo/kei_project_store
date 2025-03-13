/*
 * app_config.h
 *
 *  Created on: 2024年7月5日
 *      Author: F704012276_
 */

#ifndef SRV_CONFIG_APP_CONFIG_H_
#define SRV_CONFIG_APP_CONFIG_H_

typedef struct{
	uint8_t adType;
}bleAdStructData0_t;

typedef struct{
	uint8_t uuid16[16];
}bleAdStructData1_t;

typedef struct{
	uint16_t CompanyID;
	uint8_t Reserved : 1;
	uint8_t deviceType : 2;
	uint8_t NIO_ADVersion : 5;
	uint8_t SubType;
	uint8_t PID[3];
	uint8_t BTA_Verion;
}bleAdStructData2_t;

#endif /* SRV_CONFIG_APP_CONFIG_H_ */
