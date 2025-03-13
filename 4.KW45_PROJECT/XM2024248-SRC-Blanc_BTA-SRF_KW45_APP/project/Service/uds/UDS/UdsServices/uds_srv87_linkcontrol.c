/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv87_linkcontrol.c
 * 摘要: UDS Application Layer service 0x87 LinkControl.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#ifdef UDS_ENABLE_LINKCONTROL
static uint8_t rub_0x87LinkCtrl_SubFunc;
static uint8_t rub_0x87LinkCtrlPrevBdRtType;
static uint8_t rub_Prepare2ChangedSpecBaudRateHighByte;
static uint8_t rub_Prepare2ChangedSpecBaudRateMiddleByte;
static uint8_t rub_Prepare2ChangedSpecBaudRateLowByte;
static uint8_t rub_Prepare2ChangedFixedBaudRateId;

/* Sun-Function parameter for LinkControl(ISO15765-3 limited)
 * 	linkControlType
	0x01	UDS_LINKCONTROL_VERIFYFiXED
	0x02	UDS_LINKCONTROL_VERIFYSPECIFIC
	0x03	UDS_LINKCONTROL_TRANSITION
 */

/* NRC for LinkControl
 * 	0x12	UDS_NRC_SUBFUNCTIONNOTSUPPORTED
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 *	0x24	UDS_NRC_REQUESTSEQUENCEERROR
 *	0x31	UDS_NRC_REQUESTOUTOFRANGE
 */

/* BaudrateIdentifier(Fixed Baudrate) for LinkControl
 *
	0x10	UDS_LINKCONTROL_FIXED_125K
	0x11	UDS_LINKCONTROL_FIXED_250K
	0x12	UDS_LINKCONTROL_FIXED_500K
	0x13	UDS_LINKCONTROL_FIXED_1M
 */

Std_UdsReturnType UDS_FN_TYPE UdsApp_LinkControl(Uds_APduData_t* pAPduInd,
                                              Uds_APduData_t* pAPduResp )
{
	uint8_t lub_error = UDS_POSITIVE_RESPONSE;

	uint8_t lub_LinkCtrlFixedBaudRateId;
	/* uint8_t lub_LinkCtrlTransitBaudRateRc; */

	if ((pAPduInd->msgLength) < 0x02u)
	{
		/* the valid request length for 0x87 svc is at least 0x02 bytes.
		  otherwise , to return NRC: 0x13 : IncorrectMessageLengthOrInvalidFormat
		*/
		lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
	}
	else if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_DEFAULT_SESSION)
	{
		/* 0x87 LinkControl will only be allowed in the non-default session
		  otherwise , to return NRC: 0x7F, SevericeNotSupportedInCurrActiveSession
		*/
		lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
	}
	else /* else if ((pAPduInd->msgLength) >= 0x02u) */
	{
		/* to handle the byte_1 control_type ,
		  except bit7 SuppressPositiveResponseMessgeIndicationBit
		*/

		rub_0x87LinkCtrl_SubFunc = ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7);

		switch(rub_0x87LinkCtrl_SubFunc)
		{
		    default:
				lub_error= UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
				break;

		    case UDS_C_LNKCTRL_VERIFY_FIXED_BAUDRATE:
				if ((pAPduInd->msgLength) != 0x03u)
				{
					lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
				}
				else /* if ((pAPduInd->msgLength) == 0x03u) */
				{
					lub_LinkCtrlFixedBaudRateId = (pAPduInd->pduData[0x02u]);

					if (lub_LinkCtrlFixedBaudRateId != UDS_C_LNKCTRL_FIXED_BAUDRATE_ID_125K &&
						lub_LinkCtrlFixedBaudRateId != UDS_C_LNKCTRL_FIXED_BAUDRATE_ID_250K &&
						lub_LinkCtrlFixedBaudRateId != UDS_C_LNKCTRL_FIXED_BAUDRATE_ID_500K &&
						lub_LinkCtrlFixedBaudRateId != UDS_C_LNKCTRL_FIXED_BAUDRATE_ID_1M      )
					{
						lub_error = UDS_NRC_REQUESTOUTOFRANGE;
					}
					else
					{
						if (UDS_APP_ITF_IMP_SET_CHK_FIX_BDRT_IS_ALLOWD(lub_LinkCtrlFixedBaudRateId))
						{
							rub_0x87LinkCtrlPrevBdRtType = UDS_C_LNKCTRL_VERIFY_FIXED_BAUDRATE;
							rub_Prepare2ChangedFixedBaudRateId = lub_LinkCtrlFixedBaudRateId;
						}
						else
						{
							lub_error = UDS_NRC_REQUESTOUTOFRANGE;
						}
					}
				}  /* end of  if ((pAPduInd->msgLength) == 0x03u) */
				break;

			case UDS_C_LNKCTRL_VERIFY_SPEC_BAUDRATE :
				if ((pAPduInd->msgLength) != 0x05u)
				{
					lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
				}
				else /* if ((pAPduInd->msgLength) == 0x05u) */
				{
					if (UDS_APP_ITF_IMP_SET_CHK_SPEC_BDRT_IS_ALLOWD(pAPduInd->pduData[0x02u],
																	pAPduInd->pduData[0x03u],
																	pAPduInd->pduData[0x04u] ))
					{
						rub_0x87LinkCtrlPrevBdRtType = UDS_C_LNKCTRL_VERIFY_SPEC_BAUDRATE;
						rub_Prepare2ChangedSpecBaudRateHighByte   = pAPduInd->pduData[0x02u];
						rub_Prepare2ChangedSpecBaudRateMiddleByte = pAPduInd->pduData[0x03u];
						rub_Prepare2ChangedSpecBaudRateLowByte    = pAPduInd->pduData[0x04u];
					}
					else
					{
						lub_error = UDS_NRC_REQUESTOUTOFRANGE;
					}
				}  /* end of  if ((pAPduInd->msgLength) == 0x05u) */
				break;

			case UDS_C_LNKCTRL_TRANSITION_BAUDRATE  :
				if ((pAPduInd->msgLength) != 0x02u)
				{
					lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
				}
				else if (rub_0x87LinkCtrlPrevBdRtType == UDS_C_LNKCTRL_VERIFY_FIXED_BAUDRATE)
				{
					if (UDS_APP_ITF_IMP_SET_TRANSIT_FIXED_BDRT(rub_Prepare2ChangedFixedBaudRateId))
					{

					}
					else
					{
						lub_error = UDS_NRC_CONDITIONSNOTCORRECT;
					}
				}
				else if (rub_0x87LinkCtrlPrevBdRtType == UDS_C_LNKCTRL_VERIFY_SPEC_BAUDRATE)
				{
					if (UDS_APP_ITF_IMP_SET_TRANSIT_SPEC_BDRT(rub_Prepare2ChangedSpecBaudRateHighByte,
															  rub_Prepare2ChangedSpecBaudRateMiddleByte,
															  rub_Prepare2ChangedSpecBaudRateLowByte))
					{

					}
					else
					{
					   lub_error = UDS_NRC_CONDITIONSNOTCORRECT;
					}
				}
				else
				{
				   lub_error = UDS_NRC_REQUESTSEQUENCEERROR;
				}
				break;
		}  /* end of switch(lub_0x87LinkCtrl_SubFunc)  */
	} /* end of else // else if ((pAPduInd->msgLength) >= 0x02u) */

	if (lub_error == UDS_POSITIVE_RESPONSE)
	{
	   (pAPduResp->msgLength) = 0x02u;
	}
	else
	{

	}

	return lub_error;
}  /* end of UdsApp_87LinkCtrl_ReqSpecialProcess() */

void UDS_FN_TYPE UdsApp_87LnkCtrl_Init(void)
{
	/* lub_0x87LinkCtrl_Sid = 0x00u; */
	rub_0x87LinkCtrl_SubFunc  = 0xFFu;
	rub_0x87LinkCtrlPrevBdRtType = 0xFFu;
	rub_Prepare2ChangedSpecBaudRateHighByte = 0xFFu;
	rub_Prepare2ChangedSpecBaudRateMiddleByte = 0xFFu;
	rub_Prepare2ChangedSpecBaudRateLowByte = 0xFFu;
	rub_Prepare2ChangedFixedBaudRateId = 0xFFu;

	/* lets link control to transit to the default baudrate :
	   current value is 500kbit/s
	 */
	UDS_APP_ITF_IMP_SET_TRANSIT_FIXED_BDRT(UDS_C_LNKCTRL_FIXED_BAUDRATE_ID_DEFAULT);
}

#endif /* UDS_ENABLE_LINKCONTROL */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
