/*
* Copyright (c) 2021，上海福宇龙汽车科技有限公司
* All rights reserved.
* @FilePath: ALPS_NT3_BTA_SRR_1229_BL051_V_1_0.c
* @abstract:
* @Current_Version: V1.0.0
* @Date: 2024-07-19 19:00:33
* @History_Version: V1.0.0，Ren Jinyu，文件创建
*/
#include <string.h>
#include "alps_nt3_bta_srr_1229_bl051_v_1_0.h"


BCU_11E_msg BCU_11E_t;
ZONE_13C_msg ZONE_13C_t;
BLE_SnifferParam_Dev1_msg BLE_SnifferParam_Dev1_t;
BLE_SnifferParam_Dev2_msg BLE_SnifferParam_Dev2_t;
BLE_SnifferParam_Dev3_msg BLE_SnifferParam_Dev3_t;
BLE_SnifferParam_Dev4_msg BLE_SnifferParam_Dev4_t;
BLE_SnifferParam_Dev5_msg BLE_SnifferParam_Dev5_t;
BTA4_RSSI_UWB_msg BTA4_RSSI_UWB_t;
UWB_Info_Ntf_msg UWB_Info_Ntf_t;
UWB_Info_Ntf_Sup_msg UWB_Info_Ntf_Sup_t;
IotHubSts_msg IotHubSts_t;
ZONE_296_msg ZONE_296_t;
BTA_SRR_297_msg BTA_SRR_297_t;
Device_IRK_Group_1_msg Device_IRK_Group_1_t;
Device_IRK_Group_2_msg Device_IRK_Group_2_t;
BTA4_FusnData_msg BTA4_FusnData_t;
ZONE_3AA_msg ZONE_3AA_t;
ZONE_3B2_msg ZONE_3B2_t;
BTA4_Log_Data_msg BTA4_Log_Data_t;
IotConnectSts_msg IotConnectSts_t;
IotDownLinkData_msg IotDownLinkData_t;
IotUpLinkFlowCtrl_msg IotUpLinkFlowCtrl_t;
IotUplinkData_msg IotUplinkData_t;
IotDownLinkFlowCtrl_msg IotDownLinkFlowCtrl_t;
IotScanConnectReq_msg IotScanConnectReq_t;
BTA_SRR_FactoryRsp_msg BTA_SRR_FactoryRsp_t;
BTA_SRR_FactoryReq_msg BTA_SRR_FactoryReq_t;
NM_ZONE_LEM_msg NM_ZONE_LEM_t;
NM_BTA1_msg NM_BTA1_t;
NM_BTA2_msg NM_BTA2_t;
NM_BTA3_msg NM_BTA3_t;
NM_BTA4_msg NM_BTA4_t;
NM_BTA5_msg NM_BTA5_t;
VC_Data_5E0_msg VC_Data_5E0_t;
DIAG_REQ_AllECU_msg DIAG_REQ_AllECU_t;
DIAG_REQ_BTA4_msg DIAG_REQ_BTA4_t;
DIAG_RESP_BTA4_msg DIAG_RESP_BTA4_t;


static void BCU_11E_RX_Handle(void)
{


}

static void ZONE_13C_RX_Handle(void)
{


}

static void BLE_SnifferParam_Dev1_RX_Handle(void)
{


}

static void BLE_SnifferParam_Dev2_RX_Handle(void)
{


}

static void BLE_SnifferParam_Dev3_RX_Handle(void)
{


}

static void BLE_SnifferParam_Dev4_RX_Handle(void)
{


}

static void BLE_SnifferParam_Dev5_RX_Handle(void)
{


}

static void BTA4_RSSI_UWB_RX_Handle(void)
{


}

static void UWB_Info_Ntf_RX_Handle(void)
{


}

static void UWB_Info_Ntf_Sup_RX_Handle(void)
{


}

static void IotHubSts_RX_Handle(void)
{


}

static void ZONE_296_RX_Handle(void)
{


}

static void BTA_SRR_297_RX_Handle(void)
{


}

static void Device_IRK_Group_1_RX_Handle(void)
{


}

static void Device_IRK_Group_2_RX_Handle(void)
{


}

static void BTA4_FusnData_RX_Handle(void)
{


}

static void ZONE_3AA_RX_Handle(void)
{


}

static void ZONE_3B2_RX_Handle(void)
{


}

static void BTA4_Log_Data_RX_Handle(void)
{


}

static void IotConnectSts_RX_Handle(void)
{


}

static void IotDownLinkData_RX_Handle(void)
{


}

static void IotUpLinkFlowCtrl_RX_Handle(void)
{


}

static void IotUplinkData_RX_Handle(void)
{


}

static void IotDownLinkFlowCtrl_RX_Handle(void)
{


}

static void IotScanConnectReq_RX_Handle(void)
{


}

static void BTA_SRR_FactoryRsp_RX_Handle(void)
{


}

static void BTA_SRR_FactoryReq_RX_Handle(void)
{


}

static void NM_ZONE_LEM_RX_Handle(void)
{


}

static void NM_BTA1_RX_Handle(void)
{


}

static void NM_BTA2_RX_Handle(void)
{


}

static void NM_BTA3_RX_Handle(void)
{


}

static void NM_BTA4_RX_Handle(void)
{


}

static void NM_BTA5_RX_Handle(void)
{


}

static void VC_Data_5E0_RX_Handle(void)
{


}

static void DIAG_REQ_AllECU_RX_Handle(void)
{


}

static void DIAG_REQ_BTA4_RX_Handle(void)
{


}

static void DIAG_RESP_BTA4_RX_Handle(void)
{


}


void can_msg_init(void)
{
    memset(&BCU_11E_t, 0u, sizeof(BCU_11E_msg));
    BCU_11E_t.ID = 0x11eU;
    BCU_11E_t.cycle = 20U;
    BCU_11E_t.DLC = 8U;


    memset(&ZONE_13C_t, 0u, sizeof(ZONE_13C_msg));
    ZONE_13C_t.ID = 0x13cU;
    ZONE_13C_t.cycle = 20U;
    ZONE_13C_t.DLC = 8U;


    memset(&BLE_SnifferParam_Dev1_t, 0u, sizeof(BLE_SnifferParam_Dev1_msg));
    BLE_SnifferParam_Dev1_t.ID = 0x1caU;
    BLE_SnifferParam_Dev1_t.cycle = 100U;
    BLE_SnifferParam_Dev1_t.DLC = 24U;


    memset(&BLE_SnifferParam_Dev2_t, 0u, sizeof(BLE_SnifferParam_Dev2_msg));
    BLE_SnifferParam_Dev2_t.ID = 0x1cbU;
    BLE_SnifferParam_Dev2_t.cycle = 100U;
    BLE_SnifferParam_Dev2_t.DLC = 24U;


    memset(&BLE_SnifferParam_Dev3_t, 0u, sizeof(BLE_SnifferParam_Dev3_msg));
    BLE_SnifferParam_Dev3_t.ID = 0x1ccU;
    BLE_SnifferParam_Dev3_t.cycle = 100U;
    BLE_SnifferParam_Dev3_t.DLC = 24U;


    memset(&BLE_SnifferParam_Dev4_t, 0u, sizeof(BLE_SnifferParam_Dev4_msg));
    BLE_SnifferParam_Dev4_t.ID = 0x1cdU;
    BLE_SnifferParam_Dev4_t.cycle = 100U;
    BLE_SnifferParam_Dev4_t.DLC = 24U;


    memset(&BLE_SnifferParam_Dev5_t, 0u, sizeof(BLE_SnifferParam_Dev5_msg));
    BLE_SnifferParam_Dev5_t.ID = 0x1ceU;
    BLE_SnifferParam_Dev5_t.cycle = 100U;
    BLE_SnifferParam_Dev5_t.DLC = 24U;


    memset(&BTA4_RSSI_UWB_t, 0u, sizeof(BTA4_RSSI_UWB_msg));
    BTA4_RSSI_UWB_t.ID = 0x20dU;
    BTA4_RSSI_UWB_t.cycle = 50U;
    BTA4_RSSI_UWB_t.DLC = 32U;


    memset(&UWB_Info_Ntf_t, 0u, sizeof(UWB_Info_Ntf_msg));
    UWB_Info_Ntf_t.ID = 0x222U;
    UWB_Info_Ntf_t.cycle = 0U;
    UWB_Info_Ntf_t.DLC = 48U;


    memset(&UWB_Info_Ntf_Sup_t, 0u, sizeof(UWB_Info_Ntf_Sup_msg));
    UWB_Info_Ntf_Sup_t.ID = 0x223U;
    UWB_Info_Ntf_Sup_t.cycle = 0U;
    UWB_Info_Ntf_Sup_t.DLC = 48U;


    memset(&IotHubSts_t, 0u, sizeof(IotHubSts_msg));
    IotHubSts_t.ID = 0x24dU;
    IotHubSts_t.cycle = 100U;
    IotHubSts_t.DLC = 12U;


    memset(&ZONE_296_t, 0u, sizeof(ZONE_296_msg));
    ZONE_296_t.ID = 0x296U;
    ZONE_296_t.cycle = 100U;
    ZONE_296_t.DLC = 8U;

    ZONE_296_t.msg.sig.UWBLDTEnReq_Sr = 2U;

    memset(&BTA_SRR_297_t, 0u, sizeof(BTA_SRR_297_msg));
    BTA_SRR_297_t.ID = 0x297U;
    BTA_SRR_297_t.cycle = 100U;
    BTA_SRR_297_t.DLC = 20U;

    BTA_SRR_297_t.msg.sig.AdltBrthRt_Sr = 125U;
    BTA_SRR_297_t.msg.sig.ChildBrthRt_Sr = 125U;
    BTA_SRR_297_t.msg.sig.ChildMotnSts_Sr = 3U;
    BTA_SRR_297_t.msg.sig.ChildPysclSts_Sr = 2U;
    BTA_SRR_297_t.msg.sig.UWBLDTEnRsp_Sr = 2U;

    memset(&Device_IRK_Group_1_t, 0u, sizeof(Device_IRK_Group_1_msg));
    Device_IRK_Group_1_t.ID = 0x30bU;
    Device_IRK_Group_1_t.cycle = 100U;
    Device_IRK_Group_1_t.DLC = 64U;


    memset(&Device_IRK_Group_2_t, 0u, sizeof(Device_IRK_Group_2_msg));
    Device_IRK_Group_2_t.ID = 0x30cU;
    Device_IRK_Group_2_t.cycle = 100U;
    Device_IRK_Group_2_t.DLC = 64U;


    memset(&BTA4_FusnData_t, 0u, sizeof(BTA4_FusnData_msg));
    BTA4_FusnData_t.ID = 0x30eU;
    BTA4_FusnData_t.cycle = 100U;
    BTA4_FusnData_t.DLC = 64U;


    memset(&ZONE_3AA_t, 0u, sizeof(ZONE_3AA_msg));
    ZONE_3AA_t.ID = 0x3aaU;
    ZONE_3AA_t.cycle = 1000U;
    ZONE_3AA_t.DLC = 8U;


    memset(&ZONE_3B2_t, 0u, sizeof(ZONE_3B2_msg));
    ZONE_3B2_t.ID = 0x3b2U;
    ZONE_3B2_t.cycle = 1000U;
    ZONE_3B2_t.DLC = 8U;

    ZONE_3B2_t.msg.sig.Mth = 1U;
    ZONE_3B2_t.msg.sig.Day = 1U;

    memset(&BTA4_Log_Data_t, 0u, sizeof(BTA4_Log_Data_msg));
    BTA4_Log_Data_t.ID = 0x495U;
    BTA4_Log_Data_t.cycle = 0U;
    BTA4_Log_Data_t.DLC = 64U;


    memset(&IotConnectSts_t, 0u, sizeof(IotConnectSts_msg));
    IotConnectSts_t.ID = 0x49bU;
    IotConnectSts_t.cycle = 0U;
    IotConnectSts_t.DLC = 64U;


    memset(&IotDownLinkData_t, 0u, sizeof(IotDownLinkData_msg));
    IotDownLinkData_t.ID = 0x49cU;
    IotDownLinkData_t.cycle = 0U;
    IotDownLinkData_t.DLC = 64U;


    memset(&IotUpLinkFlowCtrl_t, 0u, sizeof(IotUpLinkFlowCtrl_msg));
    IotUpLinkFlowCtrl_t.ID = 0x49dU;
    IotUpLinkFlowCtrl_t.cycle = 0U;
    IotUpLinkFlowCtrl_t.DLC = 64U;


    memset(&IotUplinkData_t, 0u, sizeof(IotUplinkData_msg));
    IotUplinkData_t.ID = 0x49eU;
    IotUplinkData_t.cycle = 0U;
    IotUplinkData_t.DLC = 64U;


    memset(&IotDownLinkFlowCtrl_t, 0u, sizeof(IotDownLinkFlowCtrl_msg));
    IotDownLinkFlowCtrl_t.ID = 0x49fU;
    IotDownLinkFlowCtrl_t.cycle = 0U;
    IotDownLinkFlowCtrl_t.DLC = 64U;


    memset(&IotScanConnectReq_t, 0u, sizeof(IotScanConnectReq_msg));
    IotScanConnectReq_t.ID = 0x4b2U;
    IotScanConnectReq_t.cycle = 0U;
    IotScanConnectReq_t.DLC = 8U;


    memset(&BTA_SRR_FactoryRsp_t, 0u, sizeof(BTA_SRR_FactoryRsp_msg));
    BTA_SRR_FactoryRsp_t.ID = 0x4faU;
    BTA_SRR_FactoryRsp_t.cycle = 0U;
    BTA_SRR_FactoryRsp_t.DLC = 64U;


    memset(&BTA_SRR_FactoryReq_t, 0u, sizeof(BTA_SRR_FactoryReq_msg));
    BTA_SRR_FactoryReq_t.ID = 0x4fbU;
    BTA_SRR_FactoryReq_t.cycle = 0U;
    BTA_SRR_FactoryReq_t.DLC = 64U;


    memset(&NM_ZONE_LEM_t, 0u, sizeof(NM_ZONE_LEM_msg));
    NM_ZONE_LEM_t.ID = 0x505U;
    NM_ZONE_LEM_t.cycle = 0U;
    NM_ZONE_LEM_t.DLC = 8U;


    memset(&NM_BTA1_t, 0u, sizeof(NM_BTA1_msg));
    NM_BTA1_t.ID = 0x570U;
    NM_BTA1_t.cycle = 0U;
    NM_BTA1_t.DLC = 8U;


    memset(&NM_BTA2_t, 0u, sizeof(NM_BTA2_msg));
    NM_BTA2_t.ID = 0x571U;
    NM_BTA2_t.cycle = 0U;
    NM_BTA2_t.DLC = 8U;


    memset(&NM_BTA3_t, 0u, sizeof(NM_BTA3_msg));
    NM_BTA3_t.ID = 0x572U;
    NM_BTA3_t.cycle = 0U;
    NM_BTA3_t.DLC = 8U;


    memset(&NM_BTA4_t, 0u, sizeof(NM_BTA4_msg));
    NM_BTA4_t.ID = 0x573U;
    NM_BTA4_t.cycle = 0U;
    NM_BTA4_t.DLC = 8U;


    memset(&NM_BTA5_t, 0u, sizeof(NM_BTA5_msg));
    NM_BTA5_t.ID = 0x574U;
    NM_BTA5_t.cycle = 0U;
    NM_BTA5_t.DLC = 8U;


    memset(&VC_Data_5E0_t, 0u, sizeof(VC_Data_5E0_msg));
    VC_Data_5E0_t.ID = 0x5e0U;
    VC_Data_5E0_t.cycle = 50U;
    VC_Data_5E0_t.DLC = 8U;


    memset(&DIAG_REQ_AllECU_t, 0u, sizeof(DIAG_REQ_AllECU_msg));
    DIAG_REQ_AllECU_t.ID = 0x601U;
    DIAG_REQ_AllECU_t.cycle = 0U;
    DIAG_REQ_AllECU_t.DLC = 8U;


    memset(&DIAG_REQ_BTA4_t, 0u, sizeof(DIAG_REQ_BTA4_msg));
    DIAG_REQ_BTA4_t.ID = 0x673U;
    DIAG_REQ_BTA4_t.cycle = 0U;
    DIAG_REQ_BTA4_t.DLC = 64U;


    memset(&DIAG_RESP_BTA4_t, 0u, sizeof(DIAG_RESP_BTA4_msg));
    DIAG_RESP_BTA4_t.ID = 0x6f3U;
    DIAG_RESP_BTA4_t.cycle = 0U;
    DIAG_RESP_BTA4_t.DLC = 64U;


}

void can_rx_handle(uint8_t *data, uint32_t can_id, uint8_t len)
{
    if (NULL != data)
    {
        switch (can_id)
        {
            case BCU_11E_CANID:
            {
                memcpy(BCU_11E_t.msg.data, data, BCU_11E_DLC);
                BCU_11E_RX_Handle();
                break;
            }
            case ZONE_13C_CANID:
            {
                memcpy(ZONE_13C_t.msg.data, data, ZONE_13C_DLC);
                ZONE_13C_RX_Handle();
                break;
            }
            case BLE_SnifferParam_Dev1_CANID:
            {
                memcpy(BLE_SnifferParam_Dev1_t.msg.data, data, BLE_SnifferParam_Dev1_DLC);
                BLE_SnifferParam_Dev1_RX_Handle();
                break;
            }
            case BLE_SnifferParam_Dev2_CANID:
            {
                memcpy(BLE_SnifferParam_Dev2_t.msg.data, data, BLE_SnifferParam_Dev2_DLC);
                BLE_SnifferParam_Dev2_RX_Handle();
                break;
            }
            case BLE_SnifferParam_Dev3_CANID:
            {
                memcpy(BLE_SnifferParam_Dev3_t.msg.data, data, BLE_SnifferParam_Dev3_DLC);
                BLE_SnifferParam_Dev3_RX_Handle();
                break;
            }
            case BLE_SnifferParam_Dev4_CANID:
            {
                memcpy(BLE_SnifferParam_Dev4_t.msg.data, data, BLE_SnifferParam_Dev4_DLC);
                BLE_SnifferParam_Dev4_RX_Handle();
                break;
            }
            case BLE_SnifferParam_Dev5_CANID:
            {
                memcpy(BLE_SnifferParam_Dev5_t.msg.data, data, BLE_SnifferParam_Dev5_DLC);
                BLE_SnifferParam_Dev5_RX_Handle();
                break;
            }
            case BTA4_RSSI_UWB_CANID:
            {
                memcpy(BTA4_RSSI_UWB_t.msg.data, data, BTA4_RSSI_UWB_DLC);
                BTA4_RSSI_UWB_RX_Handle();
                break;
            }
            case UWB_Info_Ntf_CANID:
            {
                memcpy(UWB_Info_Ntf_t.msg.data, data, UWB_Info_Ntf_DLC);
                UWB_Info_Ntf_RX_Handle();
                break;
            }
            case UWB_Info_Ntf_Sup_CANID:
            {
                memcpy(UWB_Info_Ntf_Sup_t.msg.data, data, UWB_Info_Ntf_Sup_DLC);
                UWB_Info_Ntf_Sup_RX_Handle();
                break;
            }
            case IotHubSts_CANID:
            {
                memcpy(IotHubSts_t.msg.data, data, IotHubSts_DLC);
                IotHubSts_RX_Handle();
                break;
            }
            case ZONE_296_CANID:
            {
                memcpy(ZONE_296_t.msg.data, data, ZONE_296_DLC);
                ZONE_296_RX_Handle();
                break;
            }
            case BTA_SRR_297_CANID:
            {
                memcpy(BTA_SRR_297_t.msg.data, data, BTA_SRR_297_DLC);
                BTA_SRR_297_RX_Handle();
                break;
            }
            case Device_IRK_Group_1_CANID:
            {
                memcpy(Device_IRK_Group_1_t.msg.data, data, Device_IRK_Group_1_DLC);
                Device_IRK_Group_1_RX_Handle();
                break;
            }
            case Device_IRK_Group_2_CANID:
            {
                memcpy(Device_IRK_Group_2_t.msg.data, data, Device_IRK_Group_2_DLC);
                Device_IRK_Group_2_RX_Handle();
                break;
            }
            case BTA4_FusnData_CANID:
            {
                memcpy(BTA4_FusnData_t.msg.data, data, BTA4_FusnData_DLC);
                BTA4_FusnData_RX_Handle();
                break;
            }
            case ZONE_3AA_CANID:
            {
                memcpy(ZONE_3AA_t.msg.data, data, ZONE_3AA_DLC);
                ZONE_3AA_RX_Handle();
                break;
            }
            case ZONE_3B2_CANID:
            {
                memcpy(ZONE_3B2_t.msg.data, data, ZONE_3B2_DLC);
                ZONE_3B2_RX_Handle();
                break;
            }
            case BTA4_Log_Data_CANID:
            {
                memcpy(BTA4_Log_Data_t.msg.data, data, BTA4_Log_Data_DLC);
                BTA4_Log_Data_RX_Handle();
                break;
            }
            case IotConnectSts_CANID:
            {
                memcpy(IotConnectSts_t.msg.data, data, IotConnectSts_DLC);
                IotConnectSts_RX_Handle();
                break;
            }
            case IotDownLinkData_CANID:
            {
                memcpy(IotDownLinkData_t.msg.data, data, IotDownLinkData_DLC);
                IotDownLinkData_RX_Handle();
                break;
            }
            case IotUpLinkFlowCtrl_CANID:
            {
                memcpy(IotUpLinkFlowCtrl_t.msg.data, data, IotUpLinkFlowCtrl_DLC);
                IotUpLinkFlowCtrl_RX_Handle();
                break;
            }
            case IotUplinkData_CANID:
            {
                memcpy(IotUplinkData_t.msg.data, data, IotUplinkData_DLC);
                IotUplinkData_RX_Handle();
                break;
            }
            case IotDownLinkFlowCtrl_CANID:
            {
                memcpy(IotDownLinkFlowCtrl_t.msg.data, data, IotDownLinkFlowCtrl_DLC);
                IotDownLinkFlowCtrl_RX_Handle();
                break;
            }
            case IotScanConnectReq_CANID:
            {
                memcpy(IotScanConnectReq_t.msg.data, data, IotScanConnectReq_DLC);
                IotScanConnectReq_RX_Handle();
                break;
            }
            case BTA_SRR_FactoryRsp_CANID:
            {
                memcpy(BTA_SRR_FactoryRsp_t.msg.data, data, BTA_SRR_FactoryRsp_DLC);
                BTA_SRR_FactoryRsp_RX_Handle();
                break;
            }
            case BTA_SRR_FactoryReq_CANID:
            {
                memcpy(BTA_SRR_FactoryReq_t.msg.data, data, BTA_SRR_FactoryReq_DLC);
                BTA_SRR_FactoryReq_RX_Handle();
                break;
            }
            case NM_ZONE_LEM_CANID:
            {
                memcpy(NM_ZONE_LEM_t.msg.data, data, NM_ZONE_LEM_DLC);
                NM_ZONE_LEM_RX_Handle();
                break;
            }
            case NM_BTA1_CANID:
            {
                memcpy(NM_BTA1_t.msg.data, data, NM_BTA1_DLC);
                NM_BTA1_RX_Handle();
                break;
            }
            case NM_BTA2_CANID:
            {
                memcpy(NM_BTA2_t.msg.data, data, NM_BTA2_DLC);
                NM_BTA2_RX_Handle();
                break;
            }
            case NM_BTA3_CANID:
            {
                memcpy(NM_BTA3_t.msg.data, data, NM_BTA3_DLC);
                NM_BTA3_RX_Handle();
                break;
            }
            case NM_BTA4_CANID:
            {
                memcpy(NM_BTA4_t.msg.data, data, NM_BTA4_DLC);
                NM_BTA4_RX_Handle();
                break;
            }
            case NM_BTA5_CANID:
            {
                memcpy(NM_BTA5_t.msg.data, data, NM_BTA5_DLC);
                NM_BTA5_RX_Handle();
                break;
            }
            case VC_Data_5E0_CANID:
            {
                memcpy(VC_Data_5E0_t.msg.data, data, VC_Data_5E0_DLC);
                VC_Data_5E0_RX_Handle();
                break;
            }
            case DIAG_REQ_AllECU_CANID:
            {
                memcpy(DIAG_REQ_AllECU_t.msg.data, data, DIAG_REQ_AllECU_DLC);
                DIAG_REQ_AllECU_RX_Handle();
                break;
            }
            case DIAG_REQ_BTA4_CANID:
            {
                memcpy(DIAG_REQ_BTA4_t.msg.data, data, DIAG_REQ_BTA4_DLC);
                DIAG_REQ_BTA4_RX_Handle();
                break;
            }
            case DIAG_RESP_BTA4_CANID:
            {
                memcpy(DIAG_RESP_BTA4_t.msg.data, data, DIAG_RESP_BTA4_DLC);
                DIAG_RESP_BTA4_RX_Handle();
                break;
            }
            default:
            {
                // Handle unknown CAN ID
                break;
            }
        }
    }
    else
    {
        // Handle NULL data pointer
    }
}


