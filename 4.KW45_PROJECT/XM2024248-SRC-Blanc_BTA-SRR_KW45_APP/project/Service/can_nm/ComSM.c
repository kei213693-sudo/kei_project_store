#include <stdint.h>
#include "ComSM.h"
#include "autosar_nm.h"
/*network management tx flag */
uint8_t ComSm_bTxFlag;
static uint8_t NmRxFrame_Status = 0;  /*nm frame ,0:not receive,1:receive*/
static uint8_t NmTxFrame_status = 0;  /*nm frame ,0:not transmit,1:transmit*/
static uint8_t AppRxFrame_status = 0; /*nm frame ,0:not receive,1:receive*/
static uint8_t AppTxFrame_status = 0; /*nm frame ,0:not transmit,1:transmit*/

void Nm_BusSleepMode()
{
     NmRxFrame_Status = 1; /*bus sleep mode nm frame rx is open*/
     NmTxFrame_status = 0;
     AppRxFrame_status = 0;
     AppTxFrame_status = 0;
}

void NM_NetWorkReadySleepState(void)
{
     NmRxFrame_Status = 1; /*bus prepare sleep mode nm frame rx is open*/
     NmTxFrame_status = 0;
     AppRxFrame_status = 1;
     AppTxFrame_status = 1;
}

void NM_NetWorkRepeatMsgState(void)
{
     NmRxFrame_Status = 1; /*bus prepare sleep mode nm frame rx is open*/
     NmTxFrame_status = 1;
     AppRxFrame_status = 1;
     AppTxFrame_status = 1;
}

void Nm_NetWorkMode()
{
     #if 0
     NmRxFrame_Status = 1; /*net work mode nm frame rx is open*/
     NmTxFrame_status = 1;
     AppRxFrame_status = 1;
     AppTxFrame_status = 1;
     #endif
}
void Nm_NetWorkStartIndication()
{
}

void Nm_CanPnc_effective()
{
	CanNm_PassiveStartUp(0);
}
void ComSM_setNMTxFlag(uint8_t flag)
{
     ComSm_bTxFlag = flag;
}
void Nm_PrepareBusSleepMode()
{
     NmRxFrame_Status = 1; /*bus prepare sleep mode nm frame rx is open*/
     NmTxFrame_status = 0;
     AppRxFrame_status = 0;
     AppTxFrame_status = 0;
     ComSM_setNMTxFlag(0);
}

void CanSM_TxTimeoutException()
{
}

uint8_t CanSM_GetAppTxMsgIsEnable(void)
{
     Nm_StateType nmStatePtr;
     Nm_ModeType nmModePtr;

     CanNm_GetState(0, &nmStatePtr, &nmModePtr);

     return ((NM_NETWORK == nmModePtr) && (NM_FF_YES == CanNm_Get_first_frame(0)));
	//return app_msg_tx_enable;
}
