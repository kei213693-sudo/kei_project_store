/*
 * uciMsgCPD.c
 *
 *  Created on: 2024年8月15日
 *      Author: F704010638_
 */

#include <uciMsg.h>
#include "phscaUci.h"

uint8_t cpdStartingFlg = 0;
uint8_t cpdResultUpdataFlag = 0;
ucimsg_CpdFormat_t cpdResult = {0};
uint32_t cpdResultNotifTimeout_T = 0;

void uciMsgCPD_CPDStartCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_CPD;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_CPD_START;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 0;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}

void uciMsgCPD_CPDStopCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_CPD;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_CPD_STOP;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 0;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}

/*********************************************************
 * Public functions
 *********************************************************/
//活体年龄范围
uint8_t cpdGetAgeRange(float freq)
{
		float breathFreq = freq * 60.0;
		if (breathFreq >= 25.0)
		{
			BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf = 0x7D;
			BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = (uint8_t)breathFreq;
			return 0x1;		// Range1
		}
		else if ((breathFreq < 25.0) && (breathFreq >= 20.0))
		{
			BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf = 0x7D;
			BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = (uint8_t)breathFreq;
			return 0x2;		// Range2
		}
		else if ((breathFreq < 20.0) && (breathFreq >= 18.0))
		{
			BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf = (uint8_t)breathFreq;
			BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = 0x7D;
			return 0x3;		// Range3
		}
		else if (breathFreq < 18.0)
		{
			BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf = (uint8_t)breathFreq;
			BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = 0x7D;
			return 0x4;		// Range4
		}
}
//活体存在结果检测
//TODO:活体检测结果：当CPD功能开启后，检测进行时发送：BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf = ChildLDTRslt_Sf_Detecting_486；
//TODO:检测到活体时：BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf = ChildLDTRslt_Sf_Adult_and_child_488；

//活体运动状态检测结果
//TODO:当检测到活体处于静止时：BTA_SRF_295_t.msg.sig.ChildMotnSts_Sf = ChildMotnSts_Sf_lying_476;
//TODO:当检测到活体处于运动时：BTA_SRF_295_t.msg.sig.ChildMotnSts_Sf = ChildMotnSts_Sf_Moving_478;

//活体呼吸频率
//TODO:1、活体检测进行中。2、检测到活体但活体处于运动状态无法获取有效的活体呼吸频率：发送：BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf = AdltBrthRt_Sf_Detecting_467 ；
//BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = ChildBrthRt_Sf_Detecting_469；

//CPD功能控制
void uciMsgCPD_IndicationHandle(void)
{
	switch(ZONE_296_t.msg.sig.UWBPrestart_Sf)
	{
		case UWBPRESTART_REQ_NO_REQUEST:

			break;

		case UWBPRESTART_REQ_PRESTART:
			// 开启CPD，开启成功同时设置CAN TX 295的UWBPrestartRsp_Sf 为Prestart Success
			// 注意：两个信号都能开启CPD，但只要一方开启过；可以置位CPD开启状态，避免二次开启出现问题
			if (true != cpdStartingFlg)
			{
				uciMsgCPD_CPDStartCMD();
				cpdStartingFlg = 1;
				BTA_SRF_295_t.msg.sig.ChildLDTAgeRange_Sf = Range0_571;
				BTA_SRF_295_t.msg.sig.ChildMotnSts_Sf = ChildMotnSts_Sf_Detecting_479;
				BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf = ChildLDTRslt_Sf_Detecting_486;
				BTA_SRF_295_t.msg.sig.UWBPrestartRsp_Sf = Prestart_Success_464;
				BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf	= 0x7E;
				BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = 0x7E;
			}
			else
			{
				BTA_SRF_295_t.msg.sig.UWBPrestartRsp_Sf = Prestart_Success_464;
			}
			//TODO:若发送成功则发送成功报文：BTA_SRF_295_t.msg.sig.UWBPrestartRsp_Sf = Prestart_Success_464;
			//TODO:若发送失败则发送错误：;BTA_SRF_295_t.msg.sig.UWBPrestartRsp_Sf = Prestart_Unsuccess_465;
			//TODO:当CPD已开启，收到此信号则直接发送：BTA_SRF_295_t.msg.sig.UWBPrestartRsp_Sf = Prestart_Success_464;
			break;

		case UWBPRESTART_REQ_INVALID:
			// TBD
			break;

		default :
			break;
	}
	//CPD功能控制
	switch(ZONE_296_t.msg.sig.UWBLDTEnReq_Sf)
	{
		case UWBLDTEN_REQ_ENABLE_START_AFTERRUN:
			// TBD

			break;

		case UWBLDTEN_REQ_ENABLE_NORMAL:
			// 开启CPD，开启成功同时设置CAN TX 295的UWBLDTEnRsp_Sf 为Enable+Normal
			// 注意：两个信号都能开启CPD，但只要一方开启过；可以置位CPD开启状态，避免二次开启出现问题
			if (true != cpdStartingFlg)
			{
					uciMsgCPD_CPDStartCMD();
					cpdStartingFlg = 1;
					BTA_SRF_295_t.msg.sig.ChildLDTAgeRange_Sf = Range0_571;
					BTA_SRF_295_t.msg.sig.ChildMotnSts_Sf = ChildMotnSts_Sf_Detecting_479;
					BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf = ChildLDTRslt_Sf_Detecting_486;
					BTA_SRF_295_t.msg.sig.UWBLDTEnRsp_Sf = UWBLDTEnRsp_Sf_EnableNormal_559;
					BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf	= 0x7E;
					BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = 0x7E;
			}
			else
			{
				BTA_SRF_295_t.msg.sig.UWBLDTEnRsp_Sf = UWBLDTEnRsp_Sf_EnableNormal_559;
			}
			//TODO:若成功开启CPD，则需要将状态更新发送：BTA_SRF_295_t.msg.sig.UWBLDTEnRsp_Sf = UWBLDTEnRsp_Sf_EnableNormal_559;
			//TODO:若CPD开启失败，则需要置位：BTA_SRF_295_t.msg.sig.UWBLDTEnRsp_Sf = UWBLDTEnRsp_Sf_Radar_Error_564;
			//TODO:若已开启CPD，再次收到开启CPD指令则发送：BTA_SRF_295_t.msg.sig.UWBLDTEnRsp_Sf = UWBLDTEnRsp_Sf_EnableNormal_559;

			break;

		case UWBLDTEN_REQ_DIABLE:
			// 关闭CPD，同时设置CAN TX 295的UWBLDTEnRsp_Sf 为Disable
			if (true == cpdStartingFlg)
			{
					uciMsgCPD_CPDStopCMD();
					cpdStartingFlg = 0;
			}
			BTA_SRF_295_t.msg.sig.UWBLDTEnRsp_Sf = UWBLDTEnRsp_Sf_Disable_560;
			BTA_SRF_295_t.msg.sig.UWBPrestartRsp_Sf = No_Request_823;
			BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf = ChildLDTRslt_Sf_No_occupant_485;
			BTA_SRF_295_t.msg.sig.ChildLDTAgeRange_Sf = Range0_571;
			BTA_SRF_295_t.msg.sig.ChildMotnSts_Sf = ChildMotnSts_Sf_Detecting_479;
			BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = 0x7D;
			BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf = 0x7D;
			BTA_SRF_295_t.msg.sig.reserved0 = 0;
			break;

		case UWBLDTEN_REQ_NO_REQUEST:
			// TBD
			break;

		case UWBLDTEN_REQ_PREPARE:
			// TBD
			break;

		case RESERVED_1:
			//预留
			break;

		case RESERVED_2:
			//预留
			break;

		case RESERVED_3:
			//预留
			break;

		default :
			break;
	}

	if(cpdResultUpdataFlag == 1 && cpdStartingFlg == 1)
	{
		switch (cpdResult.result)//TODO:output->result(d6上传当前CPD检测结果（运动、呼吸、无活体）)
		{
			case CPD_RET_MOTION:
				BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf = ChildLDTRslt_Sf_Adult_and_child_488;
				BTA_SRF_295_t.msg.sig.ChildMotnSts_Sf = ChildMotnSts_Sf_Moving_478;
				if(BTA_SRF_295_t.msg.sig.ChildLDTAgeRange_Sf == 3 || BTA_SRF_295_t.msg.sig.ChildLDTAgeRange_Sf == 4)
				{
					BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf = AdltBrthRt_Sf_Detecting_467;
				}
				else
				{
					BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = ChildBrthRt_Sf_Detecting_469;
				}
				BTA_SRF_295_t.msg.sig.ChildLDTAgeRange_Sf =	0x0;
				BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf	= 0x7E;
				BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = 0x7E;
				break;
			case CPD_RET_BREATH:
				BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf = ChildLDTRslt_Sf_Adult_and_child_488;
				BTA_SRF_295_t.msg.sig.ChildMotnSts_Sf = ChildMotnSts_Sf_lying_476;
				BTA_SRF_295_t.msg.sig.ChildLDTAgeRange_Sf = cpdGetAgeRange(cpdResult.frq);//TODO:此值需要依据报过来的结果填写。

				break;
			case CPD_RET_NONE:
				BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf = ChildLDTRslt_Sf_No_occupant_485;
				BTA_SRF_295_t.msg.sig.ChildMotnSts_Sf = ChildMotnSts_Sf_Detecting_479;
				BTA_SRF_295_t.msg.sig.ChildLDTAgeRange_Sf =	0x0;
				BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf	= 0x7D;
				BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = 0x7D;
				break;

			default:
				break;
		}
		SetKW45_UWBFaultState(1);
		cpdResultNotifTimeout_T = uciMsgTimerCreate(1000);
	}

	if((cpdStartingFlg == 1) && (uciMsgTimerIsExpired(cpdResultNotifTimeout_T) == 1))
	{
		SetKW45_UWBFaultState(0);
		cpdResultNotifTimeout_T = uciMsgTimerCreate(1000);
	}
}


uint8_t uciMsgCPD_IndicationRSP(uint8_t const opcodeId)
{
	uint8_t uciMsgState = 1;
	/* If device is in error state or not supported state, UCI commands are
	 * rejected */
	switch (opcodeId)
	{
		case PHSCA_UCIMSG_OID_CPD_START:
		{
			cpdStartingFlg = 1;
			cpdResultNotifTimeout_T = uciMsgTimerCreate(1000);
			if(ZONE_296_t.msg.sig.UWBPrestart_Sf == UWBPRESTART_REQ_PRESTART)
			{
				BTA_SRF_295_t.msg.sig.UWBPrestartRsp_Sf = Prestart_Success_464;
			}

			if(ZONE_296_t.msg.sig.UWBLDTEnReq_Sf == UWBLDTEN_REQ_ENABLE_NORMAL)
			{
				BTA_SRF_295_t.msg.sig.UWBLDTEnRsp_Sf = UWBLDTEnRsp_Sf_EnableNormal_559;
			}
			BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf = ChildLDTRslt_Sf_Detecting_486;
			break;
		}

		case PHSCA_UCIMSG_OID_CPD_STOP:
		{
			cpdStartingFlg = 0;
			break;
		}

		default:
		{
			uciMsgState = 0;
			break;
		}
	}
	uciMsg_Rsp.sendHead++;
	if(uciMsg_Rsp.sendHead >= UCIMSG_RSP_BUFF_LEN)
	{
		uciMsg_Rsp.sendHead = 0;
		uciMsg_Rsp.sendHeadRoundCnt++;
	}
	uciMsg_SetUciMsgWorkState(UCIMSG_WORK_STATE_IDLE);
	uciMsg_Cmd.sendHead++;
	if(uciMsg_Cmd.sendHead >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendHead = 0;
		uciMsg_Cmd.sendHeadRoundCnt++;
	}
	return uciMsgState;
}

uint8_t uciMsgCPD_rspTimeoutHandle(uint8_t const opcodeId)
{
	uint8_t uciMsgState = 1;
	switch (opcodeId)
	{
		case PHSCA_UCIMSG_OID_CPD_START:
		{
			if(ZONE_296_t.msg.sig.UWBPrestart_Sf == UWBPRESTART_REQ_PRESTART)
			{
				BTA_SRF_295_t.msg.sig.UWBPrestartRsp_Sf = Prestart_Unsuccess_465;
			}

			if(ZONE_296_t.msg.sig.UWBLDTEnReq_Sf == UWBLDTEN_REQ_ENABLE_NORMAL)
			{
				BTA_SRF_295_t.msg.sig.UWBLDTEnRsp_Sf = UWBLDTEnRsp_Sf_Radar_Error_564;
			}
			break;
		}

		case PHSCA_UCIMSG_OID_CPD_STOP:
		{
			break;
		}

		default:
		{
			uciMsgState = 0;
			break;
		}
	}
}
