#ifndef _COMSM_H
#define _COMSM_H
void Nm_NetWorkMode();
void Nm_BusSleepMode(void);
void NM_NetWorkReadySleepState(void);
void NM_NetWorkRepeatMsgState(void);
void Nm_NetWorkStartIndication();
void Nm_PrepareBusSleepMode();
void CanSM_TxTimeoutException();
void Nm_CanPnc_effective();
void Nm_LocalActive(void);
uint8_t CanSM_GetAppTxMsgIsEnable(void);
uint8_t CanSM_GetAppRxMsgIsEnable(void);
uint8_t CanSM_GetIsBusSleep(void);
uint8_t CanSM_GetIsNetwork(void);
#endif
