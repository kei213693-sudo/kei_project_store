/*
 * uciMessage.c
 *
 *  Created on: 2024年7月19日
 *      Author: F704010638_
 */
#include <uciMsg.h>
#include <uwbTaskApp.h>
#include "phscaUci.h"
#include "canbus_app.h"
#include "UWB_CAN.h"
#include "alps_nt3_bta_srr_1229_bl051_v_1_0.h"
#include "app.h"
uciMsg_UciMsgCMDPack_t uciMsg_Cmd = {0};
uciMsg_UciMsgRSPPack_t uciMsg_Rsp = {0};
uciMsg_UciMsgNTFPack_t uciMsg_Ntf = {0};
static uint8_t uciMsg_gidOngoing = 0;
static uint8_t uciMsg_oidOngoing = 0;
static uciMsg_WorkStatus_t uciMsgWorkState = UCIMSG_WORK_STATE_IDLE;
uint8_t last_scanCmd = 0;
uciMsg_WorkStatus_t uciMsg_GetUciMsgWorkState(void)
{
	return uciMsgWorkState;
}

void uciMsg_SetUciMsgWorkState(uciMsg_WorkStatus_t state_val)
{
	uciMsgWorkState = state_val;
}


void uciMsg_UciMsgCmdTx(void)
{
	uint16_t dataLength = 0;
	if(uciMsgWorkState == UCIMSG_WORK_STATE_IDLE)
	{
//		dbg_Printf("uciMsg_Cmd.sendHead:%d\n\r",uciMsg_Cmd.sendHead);
//		dbg_Printf("uciMsg_Cmd.sendEnd:%d\n\r",uciMsg_Cmd.sendEnd);
//		dbg_Printf("uciMsg_Cmd.sendHeadRoundCnt:%d\n\r",uciMsg_Cmd.sendHeadRoundCnt);
//		dbg_Printf("uciMsg_Cmd.sendEndRoundCnt:%d\n\r",uciMsg_Cmd.sendEndRoundCnt);
//		dbg_Printf("uciMsg_Cmd GID:%d\n\r",uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid);
//		dbg_Printf("uciMsg_Cmd OID:%d\n\r",uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid);
		uciMsg_Rsp.sendEnd = 0;
		uciMsg_Rsp.sendHead = 0;
		uciMsg_Rsp.sendEndRoundCnt = 0;
		uciMsg_Rsp.sendHeadRoundCnt = 0;
		dataLength = (uint16_t)(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength_H << 8 | uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
		uciMsg_SetUciMsgWorkState(UCIMSG_WORK_STATE_SEND_CMD);
		uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.pbf = 0;
		phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,dataLength);
		uciMsg_SetUciMsgWorkState(UCIMSG_WORK_STATE_WAIT_RSP);
		uciMsg_Cmd.waitRspTimeout_T = uciMsgTimerCreate(2000u);
	}
	else
	{
		;
	}
}

void uciMsg_UciMsgResp(void)
{
	uint8_t canFrameCount = 0;
	uint16_t dataLength = (uint16_t)(uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.payloadLength_H << 8 | uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.payloadLength);
	canFrameCount = dataLength / 60 +1;
	canbusTxMsgFast(UWB_CAN_UCI_RSP_SEND_CAN_ID,canFrameCount,5,0);
}

void uciMsg_UciMsgNotif(void)
{
	uint8_t canFrameCount = 0;
	uint16_t dataLength = (uint16_t)(uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciInfo.payloadLength_H << 8 | uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciInfo.payloadLength);
	canFrameCount = dataLength / 60 +1;
	canbusTxMsgFast(UWB_CAN_UCI_RSP_SEND_CAN_ID,canFrameCount,5,0);
}

void uciMsg_UciMsgErrorStateNotif(uint8_t State)
{
	uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_NOTIFICATION;
	uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_CORE;
	uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_CORE_GENERIC_ERROR_NTF;
	uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendEnd].uciInfo.payloadLength = 1;
	uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendEnd].uciInfo.payload[0] = State;
	uciMsg_Ntf.sendEnd++;
	if(uciMsg_Ntf.sendEnd >= UCIMSG_NTF_BUFF_LEN)
	{
		uciMsg_Ntf.sendEnd = 0;
	}
	canbusTxMsgFast(UWB_CAN_UCI_RSP_SEND_CAN_ID,1,0,1);
}

void uciMsg_cmdHandler(void)
{
	/*UWB 命令消息处理*/
//	if(UWB_Info_Ntf_t.On_line_flag == 1)
//	{
//		uciMsgRanging_RangeSessionParaCfgCMD();
//		UWB_Info_Ntf_t.On_line_flag = 0;
//	}
//
//	if(UWB_Info_Ntf_Sup_t.On_line_flag == 1)
//	{
//		uciMsgRanging_RangeURSKcfgCMD();
//		UWB_Info_Ntf_Sup_t.On_line_flag = 0;
//	}

//	if(canMsg296Rx.newMsgRecv_flg == 1)
//	{
//		if(canMsg296Rx.msg.sig.cpdEnable == 1)
//		{
//			uciMsgCPD_CPDStartCMD();
//		}
//		else if(canMsg296Rx.msg.sig.cpdEnable == 2)
//		{
//			uciMsgCPD_CPDStopCMD();
//		}
//		canMsg296Rx.newMsgRecv_flg = 0;
//	}

	if(canMsg1ccRx.newMsgRecv_flg == 1)
	{
		if(last_scanCmd != canMsg1ccRx.msg.sig.scanCMD)
		{
			if(canMsg1ccRx.msg.sig.scanCMD == 1)
			{
				uciMsgRanging_RangeBleMacAddcfgCMD();
				uciMsgRanging_RangeStartCMD();
			}
			else if(canMsg1ccRx.msg.sig.scanCMD == 2)
			{
				uciMsgRanging_RangeStopCMD();
			}
			last_scanCmd = canMsg1ccRx.msg.sig.scanCMD;
		}
		canMsg1ccRx.newMsgRecv_flg = 0;
	}

	/* If UCI communicate is allowed and new UCI command is available. */
	if (uciMsg_Cmd.sendEnd != uciMsg_Cmd.sendHead || uciMsg_Cmd.sendEndRoundCnt != uciMsg_Cmd.sendHeadRoundCnt)
	{
		if(EcuM_GetUwbSleepFlg() == 0 && (CLOCK_GetSysOscFreq() != 0))
		{
			uciMsg_UciMsgCmdTx();
		}
		else
		{
			uwbTaskApp_29d6WeakupCmd();
			EcuM_SetUwbSleepFlg(0);
		}
	}
	else
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendHead = 0;
		uciMsg_Cmd.sendEndRoundCnt = 0;
		uciMsg_Cmd.sendHeadRoundCnt = 0;
	}
}

#define CAN_TRANSMIT_EN	0
extern uint8_t MAC_ADDRESS[6];
void uciMsg_rspNotifHandler(void)
{
	uint8_t uciMsg_gidRsp;
	uint8_t uciMsg_oidRsp;
	uint8_t uciMsgState = 0;
	/*UWB 响应消息处理*/
	if(UCIMSG_WORK_STATE_WAIT_RSP == uciMsgWorkState)
	{
		if(uciMsgTimerIsExpired(uciMsg_Cmd.waitRspTimeout_T))	//等待UCI响应
		{
			uciMsg_SetUciMsgWorkState(UCIMSG_WORK_STATE_RSP_TIMEOUT);
		}
		else
		{
			if(uciMsg_Rsp.sendEnd != uciMsg_Rsp.sendHead || uciMsg_Rsp.sendEndRoundCnt != uciMsg_Rsp.sendHeadRoundCnt)	//有新的UCI响应未处理
			{
#if !CAN_TRANSMIT_EN
				uciMsg_gidRsp = uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.gid;
				uciMsg_oidRsp = uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.oid;

				switch (uciMsg_gidRsp)
				{
					case (uint8_t)PHSCA_UCIMSG_GROUP_ID_RANGE:
					{
						uciMsgState = uciMsgRanging_IndicationRSP(uciMsg_oidRsp);
						break;
					}

					case (uint8_t)PHSCA_UCIMSG_GROUP_ID_CPD:
					{
						uciMsgState = uciMsgCPD_IndicationRSP(uciMsg_oidRsp);
						break;
					}

					case (uint8_t)PHSCA_UCIMSG_GROUP_ID_CUSTOM:
					{
						uciMsgState = uciMsgCustom_IndicationRSP(uciMsg_oidRsp);
						break;
					}

					default:
					{
#endif
						if(uciMsg_Rsp.rspCanSending == 0 && uciMsg_Ntf.notifCanSending == 0)
						{
							uciMsg_UciMsgResp();	//向CAN总线发送响应数据
							uciMsg_Rsp.rspCanSending = 1;
							uciMsg_Rsp.rspCanSendTimeout_T = uciMsgTimerCreate(500);
							uciMsg_Ntf.notifCanSendTimeout_T = uciMsgTimerCreate(500u);
							uciMsg_SetUciMsgWorkState(UCIMSG_WORK_STATE_IDLE);
							uciMsg_Cmd.sendHead++;
							if(uciMsg_Cmd.sendHead >= UCIMSG_CMD_BUFF_LEN)
							{
								uciMsg_Cmd.sendHead = 0;
								uciMsg_Cmd.sendHeadRoundCnt++;
							}
						}
						else
						{
							if(uciMsgTimerIsExpired(uciMsg_Rsp.rspCanSendTimeout_T))
							{
								uciMsg_Rsp.rspCanSending = 0;
								uciMsg_Ntf.notifCanSending = 0;
							}
						}
#if !CAN_TRANSMIT_EN
						break;
					}
#endif
				}
#if !CAN_TRANSMIT_EN
			}
			else
			{
				uciMsg_Rsp.sendEnd = 0;
				uciMsg_Rsp.sendHead = 0;
				uciMsg_Rsp.sendEndRoundCnt = 0;
				uciMsg_Rsp.sendHeadRoundCnt = 0;
			}
#endif
		}
	}

	/*响应超时处理*/
	if(UCIMSG_WORK_STATE_RSP_TIMEOUT == uciMsgWorkState)
	{
		/*TODO send MAC Address */
		uciMsg_SetUciMsgWorkState(UCIMSG_WORK_STATE_IDLE);
//		if(EcuM_GetUwbSleepFlg() != 1)
//		{
			uciMsg_Cmd.sendHead++;
			if(uciMsg_Cmd.sendHead >= UCIMSG_CMD_BUFF_LEN)
			{
				uciMsg_Cmd.sendHead = 0;
				uciMsg_Cmd.sendHeadRoundCnt++;
			}
//		}
//		else
//		{
//			shell_write("\r\n MAC send timeout! \r\n");
//			canMsg1ccRx.newMsgRecv_flg = 1;
//			//canMsg1ccRx.msg.sig.scanCMD = 1;
//			last_scanCmd = 0;
//			//memcpy(canMsg1ccRx.msg.sig.scanPara, MAC_ADDRESS, 6);
//		}
	}

	static uint16_t canMsgSendDelayCnt = 0;
	canMsgSendDelayCnt++;
	/*UWB 通知消息处理*/
	if(uciMsg_Ntf.sendEnd != uciMsg_Ntf.sendHead || uciMsg_Ntf.sendEndRoundCnt != uciMsg_Ntf.sendHeadRoundCnt)
	{
#if !CAN_TRANSMIT_EN
		uciMsg_gidRsp = uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciInfo.gid;
		uciMsg_oidRsp = uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciInfo.oid;
		switch (uciMsg_gidRsp)
		{
			case (uint8_t)PHSCA_UCIMSG_GROUP_ID_RANGE:
			case (uint8_t)PHSCA_UCIMSG_GROUP_ID_CPD:
			{
				Range_Cpd_DataType*	rangeAndCpdRet = uwbTaskApp_GetRangeAndCpdResult();
				if(uciMsg_oidRsp == PHSCA_UCIMSG_OID_RANGE_NOTIFY && rangeAndCpdRet->range_state == 1)
				{
					canMsg215Tx.msg.sig.disValue = rangeAndCpdRet->distance1[0];
					canMsg215Tx.msg.sig.rssiAndDisValid |= 0x02;
				}

				if(uciMsg_oidRsp == PHSCA_UCIMSG_OID_CPD_NOTIFY && rangeAndCpdRet->cpd_state == 1)
				{
					canMsg216Tx.msg.sig.cpdResult = rangeAndCpdRet->result;
					canMsg216Tx.msg.sig.cpdFreq = (uint16_t)(rangeAndCpdRet->frq*100);
					canMsg216Tx.msg.sig.cpdMotion = (uint16_t)rangeAndCpdRet->motion;
					canMsg216Tx.msg.sig.amp_bfeature = (uint16_t)(rangeAndCpdRet->amp_bfeature*100);
					canMsg216Tx.msg.sig.phs_bfeature = (uint16_t)(rangeAndCpdRet->phs_bfeature*100);
					canMsg216Tx.msgUpdata_flg = 1;
					if(canMsgSendDelayCnt >= 40)
					{
						canbusTxMsgFast(DV_TEST_SRF_216_CAN_ID,1,0,0);
						canMsgSendDelayCnt = 0;
					}
				}
				uciMsg_Ntf.sendHead++;
				if(uciMsg_Ntf.sendHead >= UCIMSG_NTF_BUFF_LEN)
				{
					uciMsg_Ntf.sendHead = 0;
					uciMsg_Ntf.sendHeadRoundCnt++;
				}
				break;
			}

//			case (uint8_t)0x0A:
//			{
//				if(uciMsg_oidRsp == 0x00)
//				{
//					uciMsg_Ntf.sendHead++;
//					if(uciMsg_Ntf.sendHead >= UCIMSG_NTF_BUFF_LEN)
//					{
//						uciMsg_Ntf.sendHead = 0;
//						uciMsg_Ntf.sendHeadRoundCnt++;
//					}
//					break;
//				}
//				//rangeRestartRequest = 1;
//			}

			default:
			{
#endif
				if(uciMsg_Rsp.rspCanSending == 0 && uciMsg_Ntf.notifCanSending == 0)
				{
					uciMsg_UciMsgNotif();	//向CAN总线发送响应数据
					uciMsg_Ntf.notifCanSending = 1;
					uciMsg_Ntf.notifCanSendTimeout_T = uciMsgTimerCreate(500u);
					uciMsg_Rsp.rspCanSendTimeout_T = uciMsgTimerCreate(500);
				}
				else
				{
					if(uciMsgTimerIsExpired(uciMsg_Ntf.notifCanSendTimeout_T))
					{
						uciMsg_Rsp.rspCanSending = 0;
						uciMsg_Ntf.notifCanSending = 0;
					}
				}
#if !CAN_TRANSMIT_EN
				break;
			}
		}
#endif
	}
	else
	{
		uciMsg_Ntf.sendEnd = 0;
		uciMsg_Ntf.sendHead = 0;
		uciMsg_Ntf.sendEndRoundCnt = 0;
		uciMsg_Ntf.sendHeadRoundCnt = 0;
	}
}

void uciMsg_Init(void)
{
	memset((uint8_t*)&uciMsg_Cmd,0,sizeof(uciMsg_Cmd));
	memset((uint8_t*)&uciMsg_Rsp,0,sizeof(uciMsg_Cmd));
	memset((uint8_t*)&uciMsg_Ntf,0,sizeof(uciMsg_Cmd));
}
