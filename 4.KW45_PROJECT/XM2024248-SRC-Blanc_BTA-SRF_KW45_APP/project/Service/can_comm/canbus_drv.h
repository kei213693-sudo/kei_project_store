/*
***************************************************************************************
* FileName:  canbus_drv.h
* Description:
* Date:
***************************************************************************************
*/
#ifndef CANBUS_DRV_H__
#define CANBUS_DRV_H__

#include "hal_can_typedef.h"
void canbusDrvInit(void);

uint8_t canbusDrvTransmit(uint32_t id, uint8_t dlc, uint8_t *data);

uint8_t canbusDrvIsTransmitting(void);

void CANIf_Indication(const hal_can_pdu_st * pdata);
uint8_t canbusDrvSendIsConfirm(uint32_t id);

void canbusDrvSendClrConfirmId(void);

void canbusDrvSendWaitConfirmId(uint32_t id);

#endif







