#ifndef _COMSM_H
#define _COMSM_H

void Nm_BusSleepMode(void);
void Nm_NetWorkMode();
void Nm_NetWorkStartIndication();
void NM_NetWorkReadySleepState(void);
void Nm_PrepareBusSleepMode();
void CanSM_TxTimeoutException();
void Nm_CanPnc_effective();

void NM_NetWorkRepeatMsgState(void);
uint8_t CanSM_GetAppTxMsgIsEnable(void);

#endif