/*
*Copyright(c)2022,宁波福尔达智能科技股份有限公司
*Allrightsreserved.
*文件名称：autosar_nm_cfg.h
*摘要：定义autosar网络管理配置文件
*当前版本：V0.0.1，易源东，2022.04.14，首版
*历史版本：Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
*/
#include "autosar_nm_cfg.h"
#include "hal_test.h"
#include "ComSM.h"
#include "udscan_confdefs.h"
#include "canbus_trans.h"

#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
#include "BtaPos.h"

#define  NODE_ID  0x7C

void cannm_get_node_id(void)
{
	BtaPos_t pos;
	pos = Get_BTAPos();
	if (pos == External_Rear) {
		cannm_cfg_info[NM_CHANNEL_NUM-1].nodeid = 0x75;
	}else if (pos == External_Front)
	{
		cannm_cfg_info[NM_CHANNEL_NUM-1].nodeid = 0x7C;
	}
}
#else
#define  NODE_ID  0x73
#endif

const CANNM_CFG_TIMERType cannm_cfg_timer[NM_CHANNEL_NUM] = {
	/*  重复报文时间    网络管理超时    等待休眠    app报文发送时间 pdu快发时间 pdu正常时间 nm发送事件  唤醒时间 */
	{	3210, 			2000, 			2000, 			20, 		50, 		640, 	  10, 	   100}};

CANNM_CFG_INFOType cannm_cfg_info[NM_CHANNEL_NUM] = {
	/* 真实通道      基地址      节点地址   报文长度    数据偏移 数据长度 快发帧数	重复消息帧数	 CBV第一位掩码   CBV第二位掩码    CBV第三位掩码     CBV第四位PNI掩码 */
	{    0, 		0x500, 		NODE_ID, 	8, 			2, 		6, 		20, 		6,			0x01, 			0x08, 			0x10, 				0x40}};

const CANNM_CFG_FUNCALLType cannm_cfg_fun[NM_CHANNEL_NUM] = {
	{
		CANIf_NM_Transmit//发送报文api
		, Nm_BusSleepMode//通知休眠模式api
		, Nm_NetWorkMode//通知网络模式api
		, Nm_NetWorkStartIndication//通知网络模式开始api
		, NM_NetWorkReadySleepState//通知进入准备睡眠模式api
		, Nm_PrepareBusSleepMode//通知预睡眠模式api
		, CanSM_TxTimeoutException//通知发送超时api
		, Nm_CanPnc_effective//通知接受到的PNC有效api
	}
};

const CANNM_CFG_PDUType cannm_cfg_pdu[NM_CHANNEL_NUM] = {
	/* CBV字节  重复报文标志      主动激活标志   节点开关     节点字节 */
	{	1, 			0, 				4, 		   STD_OFF, 	0}};

CanNm_PnFilterMaskByteType cannm_FilterMask[NM_CHANNEL_NUM][6] = {
	/* PNC1-8掩码	PNC9-16掩码	PNC17-24掩码	PNC25-32掩码 	PNC33-40掩码	PNC41-48掩码*/
	{	0x04, 		  0x00, 	  0x00, 		  0x00,			 0x00,			0x00} /* 通道0的过滤掩码 */
	/* ZONE_LEM:PNC3*/
};
