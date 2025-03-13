/*
 * bleCom_api.h
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */

#ifndef SRV_BLECOM_BLECOM_API_H_
#define SRV_BLECOM_BLECOM_API_H_

#include "bleCom_cfg.h"
#include "bleMessage_cfg.h"
#include "bleMsg_par.h"


void BleCom_Init(void);
void BleCom_Task(void);
void BleCom_MainFunction(void);

#endif /* SRV_BLECOM_BLECOM_API_H_ */
