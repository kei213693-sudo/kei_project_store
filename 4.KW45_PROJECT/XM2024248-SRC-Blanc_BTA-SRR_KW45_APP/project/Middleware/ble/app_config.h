/*
 * app_config.h
 *
 *  Created on: 2024年7月5日
 *      Author: F704012276_
 */

#ifndef SRV_CONFIG_APP_CONFIG_H_
#define SRV_CONFIG_APP_CONFIG_H_

typedef struct{//1bytes
	uint8_t adType;
}bleAdStructData0_t;

typedef struct{//26bytes
	uint16_t CompanyID;
	uint8_t LowBatWar : 1;
	uint8_t deviceType : 2;
	uint8_t NIO_ADVersion : 5;//bit7-3，取值0x0A/0x0B
	uint8_t SubType;//0x04
	uint8_t Key_UUID[16];
	uint8_t BTA_Verion[3];
	uint8_t res[3];
}bleAdStructData1_t;
#endif /* SRV_CONFIG_APP_CONFIG_H_ */
