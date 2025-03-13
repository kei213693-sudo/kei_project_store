/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv28_comcontrol.c
 * 摘要: UDS Application Layer service 0x28 CommunicationControl
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"
#include "udscan_confdefs.h"
#include "UdsItf_Srv.h"
#include "uds_api.h"

#ifdef UDS_ENABLE_COMMUNICATIONCONTROL

static void UDS_FN_TYPE		Uds_28ComCtrl_ExecuteCommControlAction( uint8_t i_CntrlType, uint8_t i_CommSubnetType );


/*
 * 函数介绍: 通信控制
 * 参数:
 * pAPduInd  : input parameters.
   pAPduResp : output parameters.
 * 返回值: {*}
 * 备注:
 * Sun-Function parameter for CommunicationControl(ISO15765-3 limited)
 * 	controlType
	0x00	ENABLE_RX_AND_TX
	0x01	ENABLE_RX_DISABLE_TX
	0x02	DISABLE_RX_ENABLE_TX
	0x03	DISABLE_RX_AND_TX

    NRC for CommunicationControl
 * 	0x12	UDS_NRC_SUBFUNCTIONNOTSUPPORTED
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 *	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *

 * communicationType for CommunicationControl(ISO15765-3 defined)
 *	bit-code value for each type
 *	01b - application: (all application-related communication, e.g. inter-application
						signal exchange between multiple in-vehicle servers.)
 *	10b - network management: (all network-management-related communication).
 *
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_ComControl(Uds_APduData_t* pAPduInd,
                                                Uds_APduData_t* pAPduResp )
{
	uint8_t lub_error;
	uint8_t lub_CommTypeByte;
	uint8_t lub_CommTypeTrafficBits;
	uint8_t lub_CommTypeSubnetBits;
	uint8_t lub_0x28CommCtrl_SubFunc;

	lub_error = UDS_POSITIVE_RESPONSE;

    if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NRC_GENERALREJECT;
    }
    else
    {
        /* code */
    }

    /* to handle the byte_1 control_type ,
		  except bit7 SuppressPositiveResponseMessgeIndicationBit
		*/
    lub_0x28CommCtrl_SubFunc = ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7);

    if ((pAPduInd->msgLength) != 0x03u)
	{
	   /* the valid request length for 0x28 svc is 0x03 bytes.
		  otherwise , to return NRC: 0x13 : IncorrectMessageLengthOrInvalidFormat
		*/
	   lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
	   return lub_error;
	}
    else if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() != UDS_EXTENDED_DIAGNOSTIC_SESSION)
    {
       /* 0x28 CommunicationControl will only be allowed in the non-default session
          otherwise , to return NRC: 0x7F, SevericeNotSupportedInCurrActiveSession
        */
       if ((UdsApi_GetAddrMode() == UDS0_CAN_RX_MSGID_FNC))
       {
            lub_error = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
       }
       else
       {
            lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
       }

       return lub_error;
    }
    else if ((UdsApi_GetAddrMode() == UDS0_CAN_RX_MSGID_FNC) &&
    (lub_0x28CommCtrl_SubFunc == UDS_C_COMMCONTROL_DISABLE_RX_ENABLE_TX))
    {
       lub_error = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
       return lub_error;
    }
    else
	{

       /* ep40 project 28 02 is not support */
       if (lub_0x28CommCtrl_SubFunc != UDS_C_COMMCONTROL_ENABLE_RX_ENABLE_TX &&
           lub_0x28CommCtrl_SubFunc != UDS_C_COMMCONTROL_ENABLE_RX_DISABLE_TX
#if 0
		   &&
           lub_0x28CommCtrl_SubFunc != UDS_C_COMMCONTROL_DISABLE_RX_DISABLE_TX
#endif
		   )
       {
           lub_error = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
           return lub_error;
       }

       switch(lub_0x28CommCtrl_SubFunc)
	   {
		   default:
				lub_error= UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
				break;
		         case UDS_C_COMMCONTROL_ENABLE_RX_ENABLE_TX   :
				 case UDS_C_COMMCONTROL_ENABLE_RX_DISABLE_TX  :
#if 0
				 case UDS_C_COMMCONTROL_DISABLE_RX_ENABLE_TX  :
				 case UDS_C_COMMCONTROL_DISABLE_RX_DISABLE_TX :
#endif
				lub_CommTypeByte        = (pAPduInd->pduData[0x02u]);
				lub_CommTypeTrafficBits = (lub_CommTypeByte & UDS_C_COMCTRL_COM_TYP_TRAFFIC_MASK);
				lub_CommTypeSubnetBits  =  ((lub_CommTypeByte  & UDS_C_COMCTRL_COM_TYP_SUBNET_MASK) >> 0x04u);

				if (lub_CommTypeTrafficBits != UDS_C_COMCTRL_COM_TYP_TRFC_NORM_COMM_MSG  &&
					lub_CommTypeTrafficBits != UDS_C_COMCTRL_COM_TYP_TRFC_NM_COMM_MSG    &&
					lub_CommTypeTrafficBits != UDS_C_COMCTRL_COM_TYP_TRFC_NORM_AND_NM_MSG )
				{
					lub_error = UDS_NRC_REQUESTOUTOFRANGE;
					return lub_error;
				}
				else if(lub_CommTypeSubnetBits != UDS_C_COMCTRL_SUBNET_RCV_NODE_AND_ALL_CONNECT_NET &&
						/*(lub_CommTypeSubnetBits < UDS_C_COMCTRL_SUBNET_SPEC_NET_BY_NET_NUM_MIN ||
						   lub_CommTypeSubnetBits > UDS_C_COMCTRL_SUBNET_SPEC_NET_BY_NET_NUM_MIN   )  &&
						 */
						lub_CommTypeSubnetBits != UDS_C_COMCTRL_SUBNET_NET_REQ_RCVD_ON                )

				{
					lub_error = UDS_NRC_REQUESTOUTOFRANGE;
					return lub_error;
				}
				else
				{
					UDS_APP_ITF_IMP_SET_COMM_CTRL_ACTION(lub_0x28CommCtrl_SubFunc,
															lub_CommTypeByte);
					/* the valid positive reponse will be 2 bytes totally
					 */
					(pAPduResp->msgLength) = 0x02u;
				}
				break;

	   }  /* end of switch(lub_0x28CommCtrl_SubFunc)  */
	} /* end of else if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_DEFAULT_SESSION) */

   return lub_error;
} /* end of Uds_28ComCtrl_ReqSpecialProcess() */

/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_28ComCtrl_ExecuteCommControlAction(
      uint8_t i_CntrlType,
      uint8_t i_CommSubnetType )
{
    uint8_t lub_CommTypeTrafficBits;
    uint8_t lub_CommTypeSubnetBits;

    lub_CommTypeTrafficBits =   (i_CommSubnetType & UDS_C_COMCTRL_COM_TYP_TRAFFIC_MASK);
    lub_CommTypeSubnetBits  =  ((i_CommSubnetType  & UDS_C_COMCTRL_COM_TYP_SUBNET_MASK) >> 0x04u);

    if( lub_CommTypeSubnetBits == UDS_C_COMCTRL_SUBNET_RCV_NODE_AND_ALL_CONNECT_NET ||
        lub_CommTypeSubnetBits == UDS_C_COMCTRL_SUBNET_NET_REQ_RCVD_ON                )
    {
        switch(i_CntrlType)
        {

            case UDS_C_COMMCONTROL_ENABLE_RX_ENABLE_TX   :
                if (lub_CommTypeTrafficBits == UDS_C_COMCTRL_COM_TYP_TRFC_NORM_COMM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_TX_TRAFFIC();
                }
                else if (lub_CommTypeTrafficBits == UDS_C_COMCTRL_COM_TYP_TRFC_NM_COMM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_TX_TRAFFIC();
                }
                else if (lub_CommTypeTrafficBits == UDS_C_COMCTRL_COM_TYP_TRFC_NORM_AND_NM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_TX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_TX_TRAFFIC();
                }
                else
                {
                    /* code */
                }
                break;

			case UDS_C_COMMCONTROL_ENABLE_RX_DISABLE_TX  :
                if (lub_CommTypeTrafficBits  == UDS_C_COMCTRL_COM_TYP_TRFC_NORM_COMM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_TX_TRAFFIC();
                }
                else if (lub_CommTypeTrafficBits == UDS_C_COMCTRL_COM_TYP_TRFC_NM_COMM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_TX_TRAFFIC();
                }
                else if (lub_CommTypeTrafficBits  == UDS_C_COMCTRL_COM_TYP_TRFC_NORM_AND_NM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_TX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_TX_TRAFFIC();
                }
                else
                {
                    /* code */
                }
                break;

			case UDS_C_COMMCONTROL_DISABLE_RX_ENABLE_TX  :
                if( lub_CommTypeTrafficBits == UDS_C_COMCTRL_COM_TYP_TRFC_NORM_COMM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_TX_TRAFFIC();
                }
                else if( lub_CommTypeTrafficBits == UDS_C_COMCTRL_COM_TYP_TRFC_NM_COMM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_TX_TRAFFIC();
                }
                else if (lub_CommTypeTrafficBits == UDS_C_COMCTRL_COM_TYP_TRFC_NORM_AND_NM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_TX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_TX_TRAFFIC();
                }
                else
                {
                    /* code */
                }
			    break;

			case UDS_C_COMMCONTROL_DISABLE_RX_DISABLE_TX :
                if (lub_CommTypeTrafficBits == UDS_C_COMCTRL_COM_TYP_TRFC_NORM_COMM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_TX_TRAFFIC();
                }
                else if (lub_CommTypeTrafficBits == UDS_C_COMCTRL_COM_TYP_TRFC_NM_COMM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_TX_TRAFFIC();
                }
                else if (lub_CommTypeTrafficBits == UDS_C_COMCTRL_COM_TYP_TRFC_NORM_AND_NM_MSG)
                {
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_TX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_RX_TRAFFIC();
                    UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_TX_TRAFFIC();
                }
                else
                {
                }
                break;

        } /* end of switch(i_CntrlType)  */
    } /* end of if( lub_CommTypeSubnetBits == UDS_C_COMCTRL_SUBNET_RCV_NODE_AND_ALL_CONNECT_NET */
}
/*
 * 函数介绍: 通信控制初始化
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_28ComCtrl_Init(void)
{

    uint8_t CURR_ACTIVE_SESSION = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();
    if (UDS_EXTENDED_DIAGNOSTIC_SESSION == CURR_ACTIVE_SESSION)
    {
        return;
    }
    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_RX_TRAFFIC(); /* TODO: need the itf from IL   */
    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_TX_TRAFFIC(); /* TODO: need the itf from IL   */

    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_RX_TRAFFIC(); /* TODO: need the itf from NM */
    UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_TX_TRAFFIC(); /* TODO: need the itf from NM */
}

#endif /* UDS_ENABLE_COMMUNICATIONCONTROL */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
