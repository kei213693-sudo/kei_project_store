/*
* Copyright (c) 2021，上海福宇龙汽车科技有限公司
* All rights reserved.
* @FilePath: ALPS_NT3_BTA_SRR_1229_BL051_V_1_0.h
* @abstract:
* @Current_Version: V1.0.0
* @Date: 2024-07-19 19:00:33
* @History_Version: V1.0.0，Ren Jinyu，文件创建
*/
#ifndef DBC_CAN_MSG_H
#define DBC_CAN_MSG_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>


// def @BCU_11E CAN Message (286  0x11e)
#define BCU_11E_IDE (0U)
#define BCU_11E_DLC (8U)
#define BCU_11E_CANID (0x11eU)
#define BCU_11E_CYC (20U)
// signal: @VehSpd_Rdnt1_phy_0
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_VehSpd_Rdnt1_phy_0_CovFactor (0)
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_VehSpd_Rdnt1_phy_0_toS(x) ( (uint8_t) ((x) / (0)) )
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_VehSpd_Rdnt1_phy_0_fromS(x) ( ((x) * (0)) )
// signal: @VehSpd_Rdnt1_phy_1
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_VehSpd_Rdnt1_phy_1_CovFactor (0)
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_VehSpd_Rdnt1_phy_1_toS(x) ( (uint8_t) ((x) / (0)) )
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_VehSpd_Rdnt1_phy_1_fromS(x) ( ((x) * (0)) )

typedef enum
{
    VehSpdSts_Rdnt1_Inalid_0 = 0U,
    VehSpdSts_Rdnt1_Valid_1 = 1U
}VehSpdSts_Rdnt1_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint8_t VehSpd_Rdnt1_phy_0 : 5U;// Factor= 0               Unit:'km/h'
    // Vehicle speed validity and initialization status
    //  0 : "Inalid"
    //  1 : "Valid"
    VehSpdSts_Rdnt1_t VehSpdSts_Rdnt1 : 1U;
    uint8_t VehSpd_Rdnt1_phy_1 : 8U;// Factor= 0               Unit:'km/h'
    uint32_t reserved3 : 3U;

    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    uint32_t reserved6 : 8U;
  }sig;
} BCU_11E_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BCU_11E_un msg;
}BCU_11E_msg;

// def @ZONE_13C CAN Message (316  0x13c)
#define ZONE_13C_IDE (0U)
#define ZONE_13C_DLC (8U)
#define ZONE_13C_CANID (0x13cU)
#define ZONE_13C_CYC (20U)

typedef enum
{
    DrvState_DRVSTNONE_2 = 0U,
    DrvState_DRVSTMD_3 = 1U,
    DrvState_DRVSTAD_4 = 2U,
    DrvState_DRVSTINVALID_5 = 7U
}DrvState_t;

typedef enum
{
    OperatorState_OPERATORSTNONE6 = 0U,
    OperatorState_OPERATORSTOUTSIDE_7 = 1U,
    OperatorState_OPERATORSTAVAILABLE_8 = 2U,
    OperatorState_OPERATORSTINSIDE_9 = 3U,
    OperatorState_OPERATORSTINVALID_10 = 7U
}OperatorState_t;

typedef enum
{
    VEHSTPARKED_11 = 0U,
    VEHSTDRIVRDY_12 = 1U,
    VEHSTDRIVING_13 = 2U,
    VEHSTSWUPDATE_14 = 3U,
    VEHSTCHARGING_15 = 4U,
    VEHSTPWRSWAP_16 = 5U,
    VEHSTINVALID_17 = 15U
}VehStateDetailed_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    // AUTOSAR E2E profile1, CRC
    uint8_t ZONE_13C_CRC : 8U;
    // AUTOSAR E2E profile1, Message Counter
    uint8_t ZONE_13C_MsgCntr : 4U;
    uint32_t reserved0 : 4U;
    // Driving States
    //  0 : "DRV_ST_NONE"
    //  1 : "DRV_ST_MD"
    //  2 : "DRV_ST_AD"
    //  7 : "DRV_ST_INVALID"
    DrvState_t DrvState : 3U;
    // Operator State
    //  0 : "OPERATOR_ST_NONE      "
    //  1 : "OPERATOR_ST_OUTSIDE"
    //  2 : "OPERATOR_ST_AVAILABLE"
    //  3 : "OPERATOR_ST_INSIDE"
    //  7 : "OPERATOR_ST_INVALID"
    OperatorState_t OperatorState : 3U;
    uint32_t reserved1 : 2U;
    // Vehicle State
    //  0 : "VEH_ST_PARKED"
    //  1 : "VEH_ST_DRIVRDY"
    //  2 : "VEH_ST_DRIVING"
    //  3 : "VEH_ST_SWUPDATE"
    //  4 : "VEH_ST_CHARGING"
    //  5 : "VEH_ST_PWRSWAP"
    //  15 : "VEH_ST_INVALID"
    VehStateDetailed_t VehStateDetailed : 4U;
    uint32_t reserved2 : 4U;

    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    uint32_t reserved6 : 8U;
  }sig;
} ZONE_13C_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  ZONE_13C_un msg;
}ZONE_13C_msg;

// def @BLE_SnifferParam_Dev1 CAN Message (458  0x1ca)
#define BLE_SnifferParam_Dev1_IDE (0U)
#define BLE_SnifferParam_Dev1_DLC (24U)
#define BLE_SnifferParam_Dev1_CANID (0x1caU)
#define BLE_SnifferParam_Dev1_CYC (100U)

typedef enum
{
    No_request_default_value_18 = 0U,
    Request_to_start_sniff_19 = 1U,
    Request_to_stop_sniff_20 = 2U,
    Reserved_21 = 3U
}SniffReqest_Dev1_t;

typedef union
{
  uint8_t data[24u];
  struct
  {
    // connect hop increment
    uint8_t ConnHopIncrement_Dev1 : 8U;
    // transmit window side
    uint8_t TransmitWindowSize_Dev1 : 8U;
    uint8_t TransmitWindowOffset_Dev1_0 : 8U;
    uint8_t TransmitWindowOffset_Dev1_1 : 8U;
    uint8_t ConnInterval_Dev1_0 : 8U;
    uint8_t ConnInterval_Dev1_1 : 8U;
    uint8_t ConnChannelMap_Dev1_0 : 8U;
    uint8_t ConnChannelMap_Dev1_1 : 8U;
    uint8_t ConnChannelMap_Dev1_2 : 8U;
    uint8_t ConnChannelMap_Dev1_3 : 8U;
    uint8_t ConnChannelMap_Dev1_4 : 8U;
    uint8_t CRCInitializatonValue_Dev1_0 : 8U;
    uint8_t CRCInitializatonValue_Dev1_1 : 8U;
    uint8_t CRCInitializatonValue_Dev1_2 : 8U;
    uint8_t AccessAddress_Dev1_0 : 8U;
    uint8_t AccessAddress_Dev1_1 : 8U;
    uint8_t AccessAddress_Dev1_2 : 8U;
    uint8_t AccessAddress_Dev1_3 : 8U;
    // current data channel
    uint8_t CurrentDatachannel_Dev1 : 6U;
    // sinff request
    //  0 : "No request (default value)"
    //  1 : "Request to start sniff"
    //  2 : "Request to stop sniff"
    //  3 : "Reserved"
    SniffReqest_Dev1_t SniffReqest_Dev1 : 2U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
  }sig;
} BLE_SnifferParam_Dev1_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_SnifferParam_Dev1_un msg;
}BLE_SnifferParam_Dev1_msg;

// def @BLE_SnifferParam_Dev2 CAN Message (459  0x1cb)
#define BLE_SnifferParam_Dev2_IDE (0U)
#define BLE_SnifferParam_Dev2_DLC (24U)
#define BLE_SnifferParam_Dev2_CANID (0x1cbU)
#define BLE_SnifferParam_Dev2_CYC (100U)

typedef enum
{
    No_request_default_value_22 = 0U,
    Request_to_start_sniff_23 = 1U,
    Request_to_stop_sniff_24 = 2U,
    Reserved_25 = 3U
}SniffReqest_Dev2_t;

typedef union
{
  uint8_t data[24u];
  struct
  {
    // connect hop increment
    uint8_t ConnHopIncrement_Dev2 : 8U;
    // transmit window side
    uint8_t TransmitWindowSize_Dev2 : 8U;
    uint8_t TransmitWindowOffset_Dev2_0 : 8U;
    uint8_t TransmitWindowOffset_Dev2_1 : 8U;
    uint8_t ConnInterval_Dev2_0 : 8U;
    uint8_t ConnInterval_Dev2_1 : 8U;
    uint8_t ConnChannelMap_Dev2_0 : 8U;
    uint8_t ConnChannelMap_Dev2_1 : 8U;
    uint8_t ConnChannelMap_Dev2_2 : 8U;
    uint8_t ConnChannelMap_Dev2_3 : 8U;
    uint8_t ConnChannelMap_Dev2_4 : 8U;
    uint8_t CRCInitializatonValue_Dev2_0 : 8U;
    uint8_t CRCInitializatonValue_Dev2_1 : 8U;
    uint8_t CRCInitializatonValue_Dev2_2 : 8U;
    uint8_t AccessAddress_Dev2_0 : 8U;
    uint8_t AccessAddress_Dev2_1 : 8U;
    uint8_t AccessAddress_Dev2_2 : 8U;
    uint8_t AccessAddress_Dev2_3 : 8U;
    // current data channel
    uint8_t CurrentDatachannel_Dev2 : 6U;
    // sinff request
    //  0 : "No request (default value)"
    //  1 : "Request to start sniff"
    //  2 : "Request to stop sniff"
    //  3 : "Reserved"
    SniffReqest_Dev2_t SniffReqest_Dev2 : 2U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
  }sig;
} BLE_SnifferParam_Dev2_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_SnifferParam_Dev2_un msg;
}BLE_SnifferParam_Dev2_msg;

// def @BLE_SnifferParam_Dev3 CAN Message (460  0x1cc)
#define BLE_SnifferParam_Dev3_IDE (0U)
#define BLE_SnifferParam_Dev3_DLC (24U)
#define BLE_SnifferParam_Dev3_CANID (0x1ccU)
#define BLE_SnifferParam_Dev3_CYC (100U)

typedef enum
{
    No_request_default_value_26 = 0U,
    Request_to_start_sniff_27 = 1U,
    Request_to_stop_sniff_28 = 2U,
    Reserved_29 = 3U
}SniffReqest_Dev3_t;

typedef union
{
  uint8_t data[24u];
  struct
  {
    // connect hop increment
    uint8_t ConnHopIncrement_Dev3 : 8U;
    // transmit window side
    uint8_t TransmitWindowSize_Dev3 : 8U;
    uint8_t TransmitWindowOffset_Dev3_0 : 8U;
    uint8_t TransmitWindowOffset_Dev3_1 : 8U;
    uint8_t ConnInterval_Dev3_0 : 8U;
    uint8_t ConnInterval_Dev3_1 : 8U;
    uint8_t ConnChannelMap_Dev3_0 : 8U;
    uint8_t ConnChannelMap_Dev3_1 : 8U;
    uint8_t ConnChannelMap_Dev3_2 : 8U;
    uint8_t ConnChannelMap_Dev3_3 : 8U;
    uint8_t ConnChannelMap_Dev3_4 : 8U;
    uint8_t CRCInitializatonValue_Dev3_0 : 8U;
    uint8_t CRCInitializatonValue_Dev3_1 : 8U;
    uint8_t CRCInitializatonValue_Dev3_2 : 8U;
    uint8_t AccessAddress_Dev3_0 : 8U;
    uint8_t AccessAddress_Dev3_1 : 8U;
    uint8_t AccessAddress_Dev3_2 : 8U;
    uint8_t AccessAddress_Dev3_3 : 8U;
    // current data channel
    uint8_t CurrentDatachannel_Dev3 : 6U;
    // sinff request
    //  0 : "No request (default value)"
    //  1 : "Request to start sniff"
    //  2 : "Request to stop sniff"
    //  3 : "Reserved"
    SniffReqest_Dev3_t SniffReqest_Dev3 : 2U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
  }sig;
} BLE_SnifferParam_Dev3_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_SnifferParam_Dev3_un msg;
}BLE_SnifferParam_Dev3_msg;

// def @BLE_SnifferParam_Dev4 CAN Message (461  0x1cd)
#define BLE_SnifferParam_Dev4_IDE (0U)
#define BLE_SnifferParam_Dev4_DLC (24U)
#define BLE_SnifferParam_Dev4_CANID (0x1cdU)
#define BLE_SnifferParam_Dev4_CYC (100U)

typedef enum
{
    No_request_default_value_30 = 0U,
    Request_to_start_sniff_31 = 1U,
    Request_to_stop_sniff_32 = 2U,
    Reserved_33 = 3U
}SniffReqest_Dev4_t;

typedef union
{
  uint8_t data[24u];
  struct
  {
    // connect hop increment
    uint8_t ConnHopIncrement_Dev4 : 8U;
    // transmit window side
    uint8_t TransmitWindowSize_Dev4 : 8U;
    uint8_t TransmitWindowOffset_Dev4_0 : 8U;
    uint8_t TransmitWindowOffset_Dev4_1 : 8U;
    uint8_t ConnInterval_Dev4_0 : 8U;
    uint8_t ConnInterval_Dev4_1 : 8U;
    uint8_t ConnChannelMap_Dev4_0 : 8U;
    uint8_t ConnChannelMap_Dev4_1 : 8U;
    uint8_t ConnChannelMap_Dev4_2 : 8U;
    uint8_t ConnChannelMap_Dev4_3 : 8U;
    uint8_t ConnChannelMap_Dev4_4 : 8U;
    uint8_t CRCInitializatonValue_Dev4_0 : 8U;
    uint8_t CRCInitializatonValue_Dev4_1 : 8U;
    uint8_t CRCInitializatonValue_Dev4_2 : 8U;
    uint8_t AccessAddress_Dev4_0 : 8U;
    uint8_t AccessAddress_Dev4_1 : 8U;
    uint8_t AccessAddress_Dev4_2 : 8U;
    uint8_t AccessAddress_Dev4_3 : 8U;
    // current data channel
    uint8_t CurrentDatachannel_Dev4 : 6U;
    // sinff request
    //  0 : "No request (default value)"
    //  1 : "Request to start sniff"
    //  2 : "Request to stop sniff"
    //  3 : "Reserved"
    SniffReqest_Dev4_t SniffReqest_Dev4 : 2U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
  }sig;
} BLE_SnifferParam_Dev4_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_SnifferParam_Dev4_un msg;
}BLE_SnifferParam_Dev4_msg;

// def @BLE_SnifferParam_Dev5 CAN Message (462  0x1ce)
#define BLE_SnifferParam_Dev5_IDE (0U)
#define BLE_SnifferParam_Dev5_DLC (24U)
#define BLE_SnifferParam_Dev5_CANID (0x1ceU)
#define BLE_SnifferParam_Dev5_CYC (100U)

typedef enum
{
    No_request_default_value_34 = 0U,
    Request_to_start_sniff_35 = 1U,
    Request_to_stop_sniff_36 = 2U,
    Reserved_37 = 3U
}SniffReqest_Dev5_t;

typedef union
{
  uint8_t data[24u];
  struct
  {
    // connect hop increment
    uint8_t ConnHopIncrement_Dev5 : 8U;
    // transmit window side
    uint8_t TransmitWindowSize_Dev5 : 8U;
    uint8_t TransmitWindowOffset_Dev5_0 : 8U;
    uint8_t TransmitWindowOffset_Dev5_1 : 8U;
    uint8_t ConnInterval_Dev5_0 : 8U;
    uint8_t ConnInterval_Dev5_1 : 8U;
    uint8_t ConnChannelMap_Dev5_0 : 8U;
    uint8_t ConnChannelMap_Dev5_1 : 8U;
    uint8_t ConnChannelMap_Dev5_2 : 8U;
    uint8_t ConnChannelMap_Dev5_3 : 8U;
    uint8_t ConnChannelMap_Dev5_4 : 8U;
    uint8_t CRCInitializatonValue_Dev5_0 : 8U;
    uint8_t CRCInitializatonValue_Dev5_1 : 8U;
    uint8_t CRCInitializatonValue_Dev5_2 : 8U;
    uint8_t AccessAddress_Dev5_0 : 8U;
    uint8_t AccessAddress_Dev5_1 : 8U;
    uint8_t AccessAddress_Dev5_2 : 8U;
    uint8_t AccessAddress_Dev5_3 : 8U;
    // current data channel
    uint8_t CurrentDatachannel_Dev5 : 6U;
    // sinff request
    //  0 : "No request (default value)"
    //  1 : "Request to start sniff"
    //  2 : "Request to stop sniff"
    //  3 : "Reserved"
    SniffReqest_Dev5_t SniffReqest_Dev5 : 2U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
  }sig;
} BLE_SnifferParam_Dev5_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_SnifferParam_Dev5_un msg;
}BLE_SnifferParam_Dev5_msg;

// def @BTA4_RSSI_UWB CAN Message (525  0x20d)
#define BTA4_RSSI_UWB_IDE (0U)
#define BTA4_RSSI_UWB_DLC (32U)
#define BTA4_RSSI_UWB_CANID (0x20dU)
#define BTA4_RSSI_UWB_CYC (50U)
// signal: @BTA4_RSSI_Dev1_phy
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev1_phy_CovFactor (1)
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev1_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev1_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA4_RSSI_Dev2_phy
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev2_phy_CovFactor (1)
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev2_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev2_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA4_RSSI_Dev3_phy
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev3_phy_CovFactor (1)
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev3_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev3_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA4_RSSI_Dev4_phy
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev4_phy_CovFactor (1)
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev4_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev4_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA4_RSSI_Dev5_phy
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev5_phy_CovFactor (1)
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev5_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_BTA4_RSSI_Dev5_phy_fromS(x) ( ((x) + (-255)) )

typedef enum
{
    idle_38 = 0U,
    processing_39 = 1U,
    success_40 = 2U,
    fail_invalid_payload_length_41 = 3U,
    fail_invaild_checksum_42 = 4U
}UWB_Info_Sync_Sts_dev1_t;

typedef enum
{
    idle_43 = 0U,
    processing_44 = 1U,
    success_45 = 2U,
    fail_invalid_payload_length_46 = 3U,
    fail_invaild_checksum_47 = 4U
}UWB_Info_Sync_Sts_dev2_t;

typedef enum
{
    idle_48 = 0U,
    processing_49 = 1U,
    success_50 = 2U,
    fail_invalid_payload_length_51 = 3U,
    fail_invaild_checksum_52 = 4U
}UWB_Info_Sync_Sts_dev3_t;

typedef enum
{
    idle_53 = 0U,
    processing_54 = 1U,
    success_55 = 2U,
    fail_invalid_payload_length_56 = 3U,
    fail_invaild_checksum_57 = 4U
}UWB_Info_Sync_Sts_dev4_t;

typedef enum
{
    idle_58 = 0U,
    processing_59 = 1U,
    success_60 = 2U,
    fail_invalid_payload_length_61 = 3U,
    fail_invaild_checksum_62 = 4U
}UWB_Info_Sync_Sts_dev5_t;

typedef enum
{
    none_63 = 0U,
    start_64 = 1U,
    stop_65 = 2U,
    suspend_66 = 3U,
    recovery_67 = 4U,
    restart_68 = 5U
}UWB_Ranging_Sts_dev1_t;

typedef enum
{
    none_69 = 0U,
    start_70 = 1U,
    stop_71 = 2U,
    suspend_72 = 3U,
    recovery_73 = 4U,
    restart_74 = 5U
}UWB_Ranging_Sts_dev2_t;

typedef enum
{
    none_75 = 0U,
    start_76 = 1U,
    stop_77 = 2U,
    suspend_78 = 3U,
    recovery_79 = 4U,
    restart_80 = 5U
}UWB_Ranging_Sts_dev3_t;

typedef enum
{
    none_81 = 0U,
    start_82 = 1U,
    stop_83 = 2U,
    suspend_84 = 3U,
    recovery_85 = 4U,
    restart_86 = 5U
}UWB_Ranging_Sts_dev4_t;

typedef enum
{
    none_87 = 0U,
    start_88 = 1U,
    stop_89 = 2U,
    suspend_90 = 3U,
    recovery_91 = 4U,
    restart_92 = 5U
}UWB_Ranging_Sts_dev5_t;

typedef union
{
  uint8_t data[32u];
  struct
  {
    uint8_t BTA4_RSSI_Dev1_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA4_RSSI_Dev2_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA4_RSSI_Dev3_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA4_RSSI_Dev4_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA4_RSSI_Dev5_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA4_UWB_Dist_Dev1_0 : 8U; // Unit:'cm'
    uint8_t BTA4_UWB_Dist_Dev1_1 : 8U; // Unit:'cm'
    uint8_t BTA4_UWB_Dist_Dev2_0 : 8U; // Unit:'cm'
    uint8_t BTA4_UWB_Dist_Dev2_1 : 8U; // Unit:'cm'
    uint8_t BTA4_UWB_Dist_Dev3_0 : 8U; // Unit:'cm'
    uint8_t BTA4_UWB_Dist_Dev3_1 : 8U; // Unit:'cm'
    uint8_t BTA4_UWB_Dist_Dev4_0 : 8U; // Unit:'cm'
    uint8_t BTA4_UWB_Dist_Dev4_1 : 8U; // Unit:'cm'
    uint8_t BTA4_UWB_Dist_Dev5_0 : 8U; // Unit:'cm'
    uint8_t BTA4_UWB_Dist_Dev5_1 : 8U; // Unit:'cm'
    // UWB Information Synchronous Status Device1
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWB_Info_Sync_Sts_dev1_t UWB_Info_Sync_Sts_dev1 : 8U;
    // UWB Information Synchronous Status Device2
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWB_Info_Sync_Sts_dev2_t UWB_Info_Sync_Sts_dev2 : 8U;
    // UWB Information Synchronous Status Device3
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWB_Info_Sync_Sts_dev3_t UWB_Info_Sync_Sts_dev3 : 8U;
    // UWB Information Synchronous Status Device4
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWB_Info_Sync_Sts_dev4_t UWB_Info_Sync_Sts_dev4 : 8U;
    // UWB Information Synchronous Status Device5
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWB_Info_Sync_Sts_dev5_t UWB_Info_Sync_Sts_dev5 : 8U;
    // UWB Ranging Status Device1
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWB_Ranging_Sts_dev1_t UWB_Ranging_Sts_dev1 : 8U;
    // UWB Ranging Status Device2
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWB_Ranging_Sts_dev2_t UWB_Ranging_Sts_dev2 : 8U;
    // UWB Ranging Status Device3
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWB_Ranging_Sts_dev3_t UWB_Ranging_Sts_dev3 : 8U;
    // UWB Ranging Status Device4
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWB_Ranging_Sts_dev4_t UWB_Ranging_Sts_dev4 : 8U;
    // UWB Ranging Status Device5
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWB_Ranging_Sts_dev5_t UWB_Ranging_Sts_dev5 : 8U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    uint32_t reserved6 : 8U;
  }sig;
} BTA4_RSSI_UWB_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA4_RSSI_UWB_un msg;
}BTA4_RSSI_UWB_msg;

// def @UWB_Info_Ntf CAN Message (546  0x222)
#define UWB_Info_Ntf_IDE (0U)
#define UWB_Info_Ntf_DLC (48U)
#define UWB_Info_Ntf_CANID (0x222U)

typedef enum
{
    Device_Type_invalid_93 = 0U,
    Device_Type_nio_phone_94 = 1U,
    Device_Type_ccc_phone_95 = 2U
}Device_Type_t;

typedef enum
{
    UWB_Session_Req_No_Request_96 = 0U,
    UWB_Session_Req_Ranging_Session_Initiate_97 = 1U,
    UWB_Session_Req_Ranging_Session_Suspend_98 = 2U,
    UWB_Session_Req_Ranging_Session_Recovery_99 = 3U,
    UWB_Session_Req_Ranging_Session_Teminate_100 = 4U,
    UWB_Session_Req_Ranging_Session_Restart_101 = 5U
}UWB_Session_Req_t;

typedef enum
{
    Device_Index_all_102 = 0U,
    Device_Index_device1_103 = 1U,
    Device_Index_device2_104 = 2U,
    Device_Index_device3_105 = 3U,
    Device_Index_device4_106 = 4U,
    Device_Index_device5_107 = 5U
}Device_Index_t;

typedef union
{
  uint8_t data[48u];
  struct
  {
    // Checksum
    uint8_t Checksum : 8U;
    // Device_Type
    //  0 : "invalid"
    //  1 : "nio phone"
    //  2 : "ccc phone"
    Device_Type_t Device_Type : 8U;
    // SYNC_Code_Index
    uint8_t SYNC_Code_Index : 8U;
    // UWB_Session_Request
    //  0 : "No Request "
    //  1 : "Ranging Session Initiate"
    //  2 : "Ranging Session Suspend "
    //  3 : "Ranging Session Recovery "
    //  4 : "Ranging Session Teminate"
    //  5 : "Ranging Session Restart"
    UWB_Session_Req_t UWB_Session_Req : 8U;
    // Device_Index
    //  0 : "all"
    //  1 : "device1"
    //  2 : "device2"
    //  3 : "device3"
    //  4 : "device4"
    //  5 : "device5"
    Device_Index_t Device_Index : 8U;
    // Selected_PulseShape_Combo
    uint8_t Selected_PulseShape_Combo : 8U;
    // Selected_UWB_Channel
    uint8_t Selected_UWB_Channel : 8U;
    // Session_RAN_Multiplier
    uint8_t Session_RAN_Multiplier : 8U;
    // Number_Chaps_per_Slot
    uint8_t Number_Chaps_per_Slot : 8U;
    // Number_Responders_Nodes
    uint8_t Number_Responders_Nodes : 8U;
    // Number_Slots_per_Round
    uint8_t Number_Slots_per_Round : 8U;
    // Selected_Hopping_Configurate_Bitmask
    uint8_t Selected_Hopping_Config_Bitmask : 8U;
    uint8_t Selected_UWB_Config_Id_0 : 8U;
    uint8_t Selected_UWB_Config_Id_1 : 8U;
    uint8_t Selected_DK_Protocol_Version_0 : 8U;
    uint8_t Selected_DK_Protocol_Version_1 : 8U;
    uint8_t UWB_Session_Id_0 : 8U;
    uint8_t UWB_Session_Id_1 : 8U;
    uint8_t UWB_Session_Id_2 : 8U;
    uint8_t UWB_Session_Id_3 : 8U;
    uint8_t STS_Index0_0 : 8U;
    uint8_t STS_Index0_1 : 8U;
    uint8_t STS_Index0_2 : 8U;
    uint8_t STS_Index0_3 : 8U;
    uint8_t UWB_Time0_Index0_0_0 : 8U;
    uint8_t UWB_Time0_Index0_0_1 : 8U;
    uint8_t UWB_Time0_Index0_0_2 : 8U;
    uint8_t UWB_Time0_Index0_0_3 : 8U;
    uint8_t UWB_Time0_Index0_4_0 : 8U;
    uint8_t UWB_Time0_Index0_4_1 : 8U;
    uint8_t UWB_Time0_Index0_4_2 : 8U;
    uint8_t UWB_Time0_Index0_4_3 : 8U;
    uint8_t HOP_Mode_Key_0 : 8U;
    uint8_t HOP_Mode_Key_1 : 8U;
    uint8_t HOP_Mode_Key_2 : 8U;
    uint8_t HOP_Mode_Key_3 : 8U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    uint32_t reserved6 : 8U;
    uint32_t reserved7 : 8U;
    uint32_t reserved8 : 8U;
    uint32_t reserved9 : 8U;
    uint32_t reserved10 : 8U;
    uint32_t reserved11 : 8U;
  }sig;
} UWB_Info_Ntf_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  UWB_Info_Ntf_un msg;
}UWB_Info_Ntf_msg;

// def @UWB_Info_Ntf_Sup CAN Message (547  0x223)
#define UWB_Info_Ntf_Sup_IDE (0U)
#define UWB_Info_Ntf_Sup_DLC (48U)
#define UWB_Info_Ntf_Sup_CANID (0x223U)

typedef union
{
  uint8_t data[48u];
  struct
  {
    uint8_t UWB_Session_Id_Sup_0 : 8U;
    uint8_t UWB_Session_Id_Sup_1 : 8U;
    uint8_t UWB_Session_Id_Sup_2 : 8U;
    uint8_t UWB_Session_Id_Sup_3 : 8U;
    uint8_t URSK_0_0 : 8U;
    uint8_t URSK_0_1 : 8U;
    uint8_t URSK_0_2 : 8U;
    uint8_t URSK_0_3 : 8U;
    uint8_t URSK_4_0 : 8U;
    uint8_t URSK_4_1 : 8U;
    uint8_t URSK_4_2 : 8U;
    uint8_t URSK_4_3 : 8U;
    uint8_t URSK_8_0 : 8U;
    uint8_t URSK_8_1 : 8U;
    uint8_t URSK_8_2 : 8U;
    uint8_t URSK_8_3 : 8U;
    uint8_t URSK_12_0 : 8U;
    uint8_t URSK_12_1 : 8U;
    uint8_t URSK_12_2 : 8U;
    uint8_t URSK_12_3 : 8U;
    uint8_t URSK_16_0 : 8U;
    uint8_t URSK_16_1 : 8U;
    uint8_t URSK_16_2 : 8U;
    uint8_t URSK_16_3 : 8U;
    uint8_t URSK_20_0 : 8U;
    uint8_t URSK_20_1 : 8U;
    uint8_t URSK_20_2 : 8U;
    uint8_t URSK_20_3 : 8U;
    uint8_t URSK_24_0 : 8U;
    uint8_t URSK_24_1 : 8U;
    uint8_t URSK_24_2 : 8U;
    uint8_t URSK_24_3 : 8U;
    uint8_t URSK_28_0 : 8U;
    uint8_t URSK_28_1 : 8U;
    uint8_t URSK_28_2 : 8U;
    uint8_t URSK_28_3 : 8U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    uint32_t reserved6 : 8U;
    uint32_t reserved7 : 8U;
    uint32_t reserved8 : 8U;
    uint32_t reserved9 : 8U;
    uint32_t reserved10 : 8U;
    uint32_t reserved11 : 8U;
  }sig;
} UWB_Info_Ntf_Sup_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  UWB_Info_Ntf_Sup_un msg;
}UWB_Info_Ntf_Sup_msg;

// def @IotHubSts CAN Message (589  0x24d)
#define IotHubSts_IDE (0U)
#define IotHubSts_DLC (12U)
#define IotHubSts_CANID (0x24dU)
#define IotHubSts_CYC (100U)

typedef enum
{
    ScanFlag_scan_disabled_108 = 0U,
    ScanFlag_scan_enabled_109 = 1U,
    ScanFlag_RFU_110 = 2U,
    ScanFlag_RFU_111 = 3U
}ScanFlag_t;

typedef union
{
  uint8_t data[12u];
  struct
  {
    uint8_t IotHubAddress_0 : 8U;
    uint8_t IotHubAddress_1 : 8U;
    uint8_t IotHubAddress_2 : 8U;
    uint8_t IotHubAddress_3 : 8U;
    uint8_t IotHubAddress_4 : 8U;
    uint8_t IotHubAddress_5 : 8U;
    // indicating IotHub capability
    uint8_t NetworkCapability : 8U;
    // scan flag
    //  0 : "scan disabled"
    //  1 : "scan enabled"
    //  2 : "RFU"
    //  3 : "RFU"
    ScanFlag_t ScanFlag : 2U;
    uint32_t reserved0 : 6U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
  }sig;
} IotHubSts_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  IotHubSts_un msg;
}IotHubSts_msg;

// def @ZONE_296 CAN Message (662  0x296)
#define ZONE_296_IDE (0U)
#define ZONE_296_DLC (8U)
#define ZONE_296_CANID (0x296U)
#define ZONE_296_CYC (100U)
// signal: @LDTAftRnTimReq_Sr_phy
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_LDTAftRnTimReq_Sr_phy_CovFactor (3)
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_LDTAftRnTimReq_Sr_phy_toS(x) ( (uint8_t) ((x) / (3)) )
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_LDTAftRnTimReq_Sr_phy_fromS(x) ( ((x) * (3)) )

typedef enum
{
    UWBLDTRstCmd_Sr_No_Request_112 = 0U,
    UWBLDTRstCmd_Sr_Request_113 = 1U,
    UWBLDTRstCmd_Sr_Reserved_114 = 2U,
    UWBLDTRstCmd_Sr_Invalid_115 = 3U
}UWBLDTRstCmd_Sr_t;

typedef enum
{
    No_Request_116 = 0U,
    Request_117 = 1U,
    Reserved_118 = 2U,
    Invalid_119 = 3U
}LDTBackNoiseLrnCmd_Sr_t;

typedef enum
{
    UWBLDTEnReq_Sr_EnableAfter_Run_120 = 0U,
    UWBLDTEnReq_Sr_EnableNormal_121 = 1U,
    UWBLDTEnReq_Sr_Disable_122 = 2U,
    UWBLDTEnReq_Sr_No_Request_123 = 3U,
    UWBLDTEnReq_Sr_Prepare_124 = 4U,
    UWBLDTEnReq_Sr_Invalid_125 = 5U
}UWBLDTEnReq_Sr_t;

typedef enum
{
    Invalid_126 = 63U
}LDTDetCyclReq_Sr_t;

typedef enum
{
    LDTWkEnReq_Sr_Enable_127 = 0U,
    LDTWkEnReq_Sr_Disable_128 = 1U,
    LDTWkEnReq_Sr_Invalid_129 = 2U,
    LDTWkEnReq_Sr_Reserved_130 = 3U
}LDTWkEnReq_Sr_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    // UWB Living Detecting Reset Command
    //  0 : "No Request"
    //  1 : "Request"
    //  2 : "Reserved"
    //  3 : "Invalid"
    UWBLDTRstCmd_Sr_t UWBLDTRstCmd_Sr : 2U;
    uint32_t reserved2 : 2U;
    // UWB Living Detecting Background Noise Learning Command
    //  0 : "No Request"
    //  1 : "Request"
    //  2 : "Reserved"
    //  3 : "Invalid"
    LDTBackNoiseLrnCmd_Sr_t LDTBackNoiseLrnCmd_Sr : 2U;
    uint32_t reserved3 : 2U;
    // UWB Living Detecting enable Request
    //  0 : "Enable+After Run"
    //  1 : "Enable+Normal"
    //  2 : "Disable"
    //  3 : "No Request"
    //  4 : "Prepare"
    //  5 : "Invalid"
    UWBLDTEnReq_Sr_t UWBLDTEnReq_Sr : 3U;
    uint32_t reserved4 : 5U;

    uint32_t reserved5 : 8U;
    uint8_t LDTAftRnTimReq_Sr_phy : 8U;// Factor= 3
    // UWB Living Detecting after run detect cycle Request
    //  63 : "Invalid"
    LDTDetCyclReq_Sr_t LDTDetCyclReq_Sr : 6U;
    // UWB Living Detecting wake up enable Request
    //  0 : "Enable"
    //  1 : "Disable"
    //  2 : "Invalid "
    //  3 : "Reserved"
    LDTWkEnReq_Sr_t LDTWkEnReq_Sr : 2U;
    uint32_t reserved6 : 8U;
  }sig;
} ZONE_296_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  ZONE_296_un msg;
}ZONE_296_msg;

// def @BTA_SRR_297 CAN Message (663  0x297)
#define BTA_SRR_297_IDE (0U)
#define BTA_SRR_297_DLC (20U)
#define BTA_SRR_297_CANID (0x297U)
#define BTA_SRR_297_CYC (100U)
// signal: @LDTAftRnTimRsp_Sr_phy
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_LDTAftRnTimRsp_Sr_phy_CovFactor (3)
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_LDTAftRnTimRsp_Sr_phy_toS(x) ( (uint8_t) ((x) / (3)) )
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_LDTAftRnTimRsp_Sr_phy_fromS(x) ( ((x) * (3)) )

typedef enum
{
    AdltBrthRt_Sr_Detecting_131 = 126U,
    AdltBrthRt_Sr_Invalid_132 = 127U
}AdltBrthRt_Sr_t;

typedef enum
{
    ChildBrthRt_Sr_Detecting_133 = 126U,
    ChildBrthRt_Sr_Invalid_134 = 127U
}ChildBrthRt_Sr_t;

typedef enum
{
    Invalid_135 = 63U
}LDTDetCyclRsp_Sr_t;

typedef enum
{
    LDTWkEnRsp_Sr_Enable_136 = 0U,
    LDTWkEnRsp_Sr_Disable_137 = 1U,
    LDTWkEnRsp_Sr_Invalid_138 = 2U,
    LDTWkEnRsp_Sr_Reserved_139 = 3U
}LDTWkEnRsp_Sr_t;

typedef enum
{
    ChildMotnSts_Sr_lying_140 = 0U,
    ChildMotnSts_Sr_siting_141 = 1U,
    ChildMotnSts_Sr_Moving_142 = 2U,
    ChildMotnSts_Sr_Detecting_143 = 3U,
    ChildMotnSts_Sr_Invalid_144 = 4U
}ChildMotnSts_Sr_t;

typedef enum
{
    Sleeping_145 = 0U,
    Awake_146 = 1U,
    Detecting_147 = 2U,
    Invalid_148 = 3U
}ChildPysclSts_Sr_t;

typedef enum
{
    ChildLDTRslt_Sr_No_occupant_149 = 0U,
    ChildLDTRslt_Sr_Detecting_150 = 1U,
    ChildLDTRslt_Sr_Adult_only_151 = 2U,
    ChildLDTRslt_Sr_Adult_and_child_152 = 3U,
    ChildLDTRslt_Sr_Child_only_153 = 4U,
    ChildLDTRslt_Sr_Invalid_154 = 5U
}ChildLDTRslt_Sr_t;

typedef enum
{
    None_155 = 0U,
    Present_156 = 1U,
    Detecting_157 = 2U,
    Invalid_158 = 3U
}ChildLctnStsOval_Sr_t;

typedef enum
{
    None_159 = 0U,
    Present_160 = 1U,
    Detecting_161 = 2U,
    Invalid_162 = 3U
}ChildLctnStsFL_Sr_t;

typedef enum
{
    None_163 = 0U,
    Present_164 = 1U,
    Detecting_165 = 2U,
    Invalid_166 = 3U
}ChildLctnStsFR_Sr_t;

typedef enum
{
    None_167 = 0U,
    Present_168 = 1U,
    Detecting_169 = 2U,
    Invalid_170 = 3U
}ChildLctnSts2dL_Sr_t;

typedef enum
{
    None_171 = 0U,
    Present_172 = 1U,
    Detecting_173 = 2U,
    Invalid_174 = 3U
}ChildLctnSts2dM_Sr_t;

typedef enum
{
    None_175 = 0U,
    Present_176 = 1U,
    Detecting_177 = 2U,
    Invalid_178 = 3U
}ChildLctnSts2dR_Sr_t;

typedef enum
{
    None_179 = 0U,
    Present_180 = 1U,
    Detecting_181 = 2U,
    Invalid_182 = 3U
}ChildLctnSts3dL_Sr_t;

typedef enum
{
    None_183 = 0U,
    Present_184 = 1U,
    Detecting_185 = 2U,
    Invalid_186 = 3U
}ChildLctnSts3dR_Sr_t;

typedef enum
{
    None_187 = 0U,
    Present_188 = 1U,
    Detecting_189 = 2U,
    Invalid_190 = 3U
}AdltLctnStsOval_Sr_t;

typedef enum
{
    None_191 = 0U,
    Present_192 = 1U,
    Detecting_193 = 2U,
    Invalid_194 = 3U
}AdltLctnStsFL_Sr_t;

typedef enum
{
    None_195 = 0U,
    Present_196 = 1U,
    Detecting_197 = 2U,
    Invalid_198 = 3U
}AdltLctnStsFR_Sr_t;

typedef enum
{
    None_199 = 0U,
    Present_200 = 1U,
    Detecting_201 = 2U,
    Invalid_202 = 3U
}AdltLctnSts2dL_Sr_t;

typedef enum
{
    None_203 = 0U,
    Present_204 = 1U,
    Detecting_205 = 2U,
    Invalid_206 = 3U
}AdltLctnSts2dM_Sr_t;

typedef enum
{
    None_207 = 0U,
    Present_208 = 1U,
    Detecting_209 = 2U,
    Invalid_210 = 3U
}AdltLctnSts2dR_Sr_t;

typedef enum
{
    None_211 = 0U,
    Present_212 = 1U,
    Detecting_213 = 2U,
    Invalid_214 = 3U
}AdltLctnSts3dL_Sr_t;

typedef enum
{
    None_215 = 0U,
    Present_216 = 1U,
    Detecting_217 = 2U,
    Invalid_218 = 3U
}AdltLctnSts3dR_Sr_t;

typedef enum
{
    UWBLDTEnRsp_Sr_EnableAfter_Run_219 = 0U,
    UWBLDTEnRsp_Sr_EnableNormal_220 = 1U,
    UWBLDTEnRsp_Sr_Disable_221 = 2U,
    UWBLDTEnRsp_Sr_No_Request_222 = 3U,
    UWBLDTEnRsp_Sr_Preparing_223 = 4U,
    UWBLDTEnRsp_Sr_Invalid_224 = 5U,
    UWBLDTEnRsp_Sr_Radar_Error_225 = 6U,
    UWBLDTEnRsp_Sr_Comunication_Error_226 = 7U,
    UWBLDTEnRsp_Sr_Other_Error_227 = 8U
}UWBLDTEnRsp_Sr_t;

typedef enum
{
    UWBLDTRstRsp_Sr_No_Response_228 = 0U,
    UWBLDTRstRsp_Sr_Positive_Response_229 = 1U,
    UWBLDTRstRsp_Sr_Negtive_Response_230 = 2U,
    UWBLDTRstRsp_Sr_Invalid_231 = 3U
}UWBLDTRstRsp_Sr_t;

typedef enum
{
    Range0_232 = 0U,
    Range1_233 = 1U,
    Range2_234 = 2U,
    Range3_235 = 3U,
    Range4_236 = 4U,
    Invalid_237 = 5U
}ChildLDTAgeRange_Sr_t;

typedef enum
{
    No_Response_238 = 0U,
    Positive_Response_239 = 1U,
    Negtive_Response_240 = 2U,
    Invalid_241 = 3U
}LDTBackNoiseLrnRsp_Sr_t;

typedef union
{
  uint8_t data[20u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint8_t LDTAftRnTimRsp_Sr_phy : 8U;// Factor= 3
    // Adult breath rate
    //  126 : "Detecting"
    //  127 : "Invalid"
    AdltBrthRt_Sr_t AdltBrthRt_Sr : 7U;
    uint32_t reserved4 : 1U;
    // Child breath rate
    //  126 : "Detecting"
    //  127 : "Invalid"
    ChildBrthRt_Sr_t ChildBrthRt_Sr : 7U;
    uint32_t reserved5 : 1U;
    // UWB Living Detecting after run detect cycle Response
    //  63 : "Invalid"
    LDTDetCyclRsp_Sr_t LDTDetCyclRsp_Sr : 6U;
    // UWB Living Detecting wake up enable Response
    //  0 : "Enable"
    //  1 : "Disable"
    //  2 : "Invalid "
    //  3 : "Reserved"
    LDTWkEnRsp_Sr_t LDTWkEnRsp_Sr : 2U;
    // Child motion Status
    //  0 : "lying"
    //  1 : "siting"
    //  2 : "Moving"
    //  3 : "Detecting"
    //  4 : "Invalid"
    ChildMotnSts_Sr_t ChildMotnSts_Sr : 4U;
    // Child physiological Status
    //  0 : "Sleeping"
    //  1 : "Awake"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildPysclSts_Sr_t ChildPysclSts_Sr : 4U;
    // Child Living Detecting identify result
    //  0 : "No occupant"
    //  1 : "Detecting"
    //  2 : "Adult only"
    //  3 : "Adult and child"
    //  4 : "Child only"
    //  5 : "Invalid"
    ChildLDTRslt_Sr_t ChildLDTRslt_Sr : 4U;
    uint32_t reserved6 : 2U;
    // Child location status Over all
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnStsOval_Sr_t ChildLctnStsOval_Sr : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnStsFL_Sr_t ChildLctnStsFL_Sr : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnStsFR_Sr_t ChildLctnStsFR_Sr : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnSts2dL_Sr_t ChildLctnSts2dL_Sr : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnSts2dM_Sr_t ChildLctnSts2dM_Sr : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnSts2dR_Sr_t ChildLctnSts2dR_Sr : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnSts3dL_Sr_t ChildLctnSts3dL_Sr : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnSts3dR_Sr_t ChildLctnSts3dR_Sr : 2U;
    // Adult location status Over all
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnStsOval_Sr_t AdltLctnStsOval_Sr : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnStsFL_Sr_t AdltLctnStsFL_Sr : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnStsFR_Sr_t AdltLctnStsFR_Sr : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnSts2dL_Sr_t AdltLctnSts2dL_Sr : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnSts2dM_Sr_t AdltLctnSts2dM_Sr : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnSts2dR_Sr_t AdltLctnSts2dR_Sr : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnSts3dL_Sr_t AdltLctnSts3dL_Sr : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnSts3dR_Sr_t AdltLctnSts3dR_Sr : 2U;
    uint32_t reserved7 : 2U;
    // UWB Living Detecting enable Response
    //  0 : "Enable+After Run"
    //  1 : "Enable+Normal"
    //  2 : "Disable"
    //  3 : "No Request"
    //  4 : "Preparing"
    //  5 : "Invalid"
    //  6 : "Radar Error"
    //  7 : "Comunication Error"
    //  8 : "Other Error"
    UWBLDTEnRsp_Sr_t UWBLDTEnRsp_Sr : 4U;
    // UWB Living Detecting Reset Command Response
    //  0 : "No Response"
    //  1 : "Positive Response"
    //  2 : "Negtive Response"
    //  3 : "Invalid"
    UWBLDTRstRsp_Sr_t UWBLDTRstRsp_Sr : 3U;
    uint32_t reserved8 : 1U;
    // Child Living Detecting Identify Age Range
    //  0 : "Range0"
    //  1 : "Range1"
    //  2 : "Range2"
    //  3 : "Range3"
    //  4 : "Range4"
    //  5 : "Invalid"
    ChildLDTAgeRange_Sr_t ChildLDTAgeRange_Sr : 4U;
    // UWB Living Detecting Background Noise Learning Command Response
    //  0 : "No Response"
    //  1 : "Positive Response"
    //  2 : "Negtive Response"
    //  3 : "Invalid"
    LDTBackNoiseLrnRsp_Sr_t LDTBackNoiseLrnRsp_Sr : 3U;
    uint32_t reserved9 : 1U;

    uint32_t reserved10 : 8U;
    uint32_t reserved11 : 8U;
    uint32_t reserved12 : 8U;
    uint32_t reserved13 : 8U;
  }sig;
} BTA_SRR_297_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA_SRR_297_un msg;
}BTA_SRR_297_msg;

// def @Device_IRK_Group_1 CAN Message (779  0x30b)
#define Device_IRK_Group_1_IDE (0U)
#define Device_IRK_Group_1_DLC (64U)
#define Device_IRK_Group_1_CANID (0x30bU)
#define Device_IRK_Group_1_CYC (100U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t SmartKey_Dev1_IRK_0_0 : 8U;
    uint8_t SmartKey_Dev1_IRK_0_1 : 8U;
    uint8_t SmartKey_Dev1_IRK_0_2 : 8U;
    uint8_t SmartKey_Dev1_IRK_0_3 : 8U;
    uint8_t SmartKey_Dev1_IRK_4_0 : 8U;
    uint8_t SmartKey_Dev1_IRK_4_1 : 8U;
    uint8_t SmartKey_Dev1_IRK_4_2 : 8U;
    uint8_t SmartKey_Dev1_IRK_4_3 : 8U;
    uint8_t SmartKey_Dev1_IRK_8_0 : 8U;
    uint8_t SmartKey_Dev1_IRK_8_1 : 8U;
    uint8_t SmartKey_Dev1_IRK_8_2 : 8U;
    uint8_t SmartKey_Dev1_IRK_8_3 : 8U;
    uint8_t SmartKey_Dev1_IRK_12_0 : 8U;
    uint8_t SmartKey_Dev1_IRK_12_1 : 8U;
    uint8_t SmartKey_Dev1_IRK_12_2 : 8U;
    uint8_t SmartKey_Dev1_IRK_12_3 : 8U;
    uint8_t SmartKey_Dev2_IRK_0_0 : 8U;
    uint8_t SmartKey_Dev2_IRK_0_1 : 8U;
    uint8_t SmartKey_Dev2_IRK_0_2 : 8U;
    uint8_t SmartKey_Dev2_IRK_0_3 : 8U;
    uint8_t SmartKey_Dev2_IRK_4_0 : 8U;
    uint8_t SmartKey_Dev2_IRK_4_1 : 8U;
    uint8_t SmartKey_Dev2_IRK_4_2 : 8U;
    uint8_t SmartKey_Dev2_IRK_4_3 : 8U;
    uint8_t SmartKey_Dev2_IRK_8_0 : 8U;
    uint8_t SmartKey_Dev2_IRK_8_1 : 8U;
    uint8_t SmartKey_Dev2_IRK_8_2 : 8U;
    uint8_t SmartKey_Dev2_IRK_8_3 : 8U;
    uint8_t SmartKey_Dev2_IRK_12_0 : 8U;
    uint8_t SmartKey_Dev2_IRK_12_1 : 8U;
    uint8_t SmartKey_Dev2_IRK_12_2 : 8U;
    uint8_t SmartKey_Dev2_IRK_12_3 : 8U;
    uint8_t SmartKey_Dev3_IRK_0_0 : 8U;
    uint8_t SmartKey_Dev3_IRK_0_1 : 8U;
    uint8_t SmartKey_Dev3_IRK_0_2 : 8U;
    uint8_t SmartKey_Dev3_IRK_0_3 : 8U;
    uint8_t SmartKey_Dev3_IRK_4_0 : 8U;
    uint8_t SmartKey_Dev3_IRK_4_1 : 8U;
    uint8_t SmartKey_Dev3_IRK_4_2 : 8U;
    uint8_t SmartKey_Dev3_IRK_4_3 : 8U;
    uint8_t SmartKey_Dev3_IRK_8_0 : 8U;
    uint8_t SmartKey_Dev3_IRK_8_1 : 8U;
    uint8_t SmartKey_Dev3_IRK_8_2 : 8U;
    uint8_t SmartKey_Dev3_IRK_8_3 : 8U;
    uint8_t SmartKey_Dev3_IRK_12_0 : 8U;
    uint8_t SmartKey_Dev3_IRK_12_1 : 8U;
    uint8_t SmartKey_Dev3_IRK_12_2 : 8U;
    uint8_t SmartKey_Dev3_IRK_12_3 : 8U;
    uint8_t SmartKey_Dev4_IRK_0_0 : 8U;
    uint8_t SmartKey_Dev4_IRK_0_1 : 8U;
    uint8_t SmartKey_Dev4_IRK_0_2 : 8U;
    uint8_t SmartKey_Dev4_IRK_0_3 : 8U;
    uint8_t SmartKey_Dev4_IRK_4_0 : 8U;
    uint8_t SmartKey_Dev4_IRK_4_1 : 8U;
    uint8_t SmartKey_Dev4_IRK_4_2 : 8U;
    uint8_t SmartKey_Dev4_IRK_4_3 : 8U;
    uint8_t SmartKey_Dev4_IRK_8_0 : 8U;
    uint8_t SmartKey_Dev4_IRK_8_1 : 8U;
    uint8_t SmartKey_Dev4_IRK_8_2 : 8U;
    uint8_t SmartKey_Dev4_IRK_8_3 : 8U;
    uint8_t SmartKey_Dev4_IRK_12_0 : 8U;
    uint8_t SmartKey_Dev4_IRK_12_1 : 8U;
    uint8_t SmartKey_Dev4_IRK_12_2 : 8U;
    uint8_t SmartKey_Dev4_IRK_12_3 : 8U;
  }sig;
} Device_IRK_Group_1_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  Device_IRK_Group_1_un msg;
}Device_IRK_Group_1_msg;

// def @Device_IRK_Group_2 CAN Message (780  0x30c)
#define Device_IRK_Group_2_IDE (0U)
#define Device_IRK_Group_2_DLC (64U)
#define Device_IRK_Group_2_CANID (0x30cU)
#define Device_IRK_Group_2_CYC (100U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t SmartKey_Dev5_IRK_0_0 : 8U;
    uint8_t SmartKey_Dev5_IRK_0_1 : 8U;
    uint8_t SmartKey_Dev5_IRK_0_2 : 8U;
    uint8_t SmartKey_Dev5_IRK_0_3 : 8U;
    uint8_t SmartKey_Dev5_IRK_4_0 : 8U;
    uint8_t SmartKey_Dev5_IRK_4_1 : 8U;
    uint8_t SmartKey_Dev5_IRK_4_2 : 8U;
    uint8_t SmartKey_Dev5_IRK_4_3 : 8U;
    uint8_t SmartKey_Dev5_IRK_8_0 : 8U;
    uint8_t SmartKey_Dev5_IRK_8_1 : 8U;
    uint8_t SmartKey_Dev5_IRK_8_2 : 8U;
    uint8_t SmartKey_Dev5_IRK_8_3 : 8U;
    uint8_t SmartKey_Dev5_IRK_12_0 : 8U;
    uint8_t SmartKey_Dev5_IRK_12_1 : 8U;
    uint8_t SmartKey_Dev5_IRK_12_2 : 8U;
    uint8_t SmartKey_Dev5_IRK_12_3 : 8U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    uint32_t reserved6 : 8U;
    uint32_t reserved7 : 8U;
    uint32_t reserved8 : 8U;
    uint32_t reserved9 : 8U;
    uint32_t reserved10 : 8U;
    uint32_t reserved11 : 8U;
    uint32_t reserved12 : 8U;
    uint32_t reserved13 : 8U;
    uint32_t reserved14 : 8U;
    uint32_t reserved15 : 8U;
    uint32_t reserved16 : 8U;
    uint32_t reserved17 : 8U;
    uint32_t reserved18 : 8U;
    uint32_t reserved19 : 8U;
    uint32_t reserved20 : 8U;
    uint32_t reserved21 : 8U;
    uint32_t reserved22 : 8U;
    uint32_t reserved23 : 8U;
    uint32_t reserved24 : 8U;
    uint32_t reserved25 : 8U;
    uint32_t reserved26 : 8U;
    uint32_t reserved27 : 8U;
    uint32_t reserved28 : 8U;
    uint32_t reserved29 : 8U;
    uint32_t reserved30 : 8U;
    uint32_t reserved31 : 8U;
    uint32_t reserved32 : 8U;
    uint32_t reserved33 : 8U;
    uint32_t reserved34 : 8U;
    uint32_t reserved35 : 8U;
    uint32_t reserved36 : 8U;
    uint32_t reserved37 : 8U;
    uint32_t reserved38 : 8U;
    uint32_t reserved39 : 8U;
    uint32_t reserved40 : 8U;
    uint32_t reserved41 : 8U;
    uint32_t reserved42 : 8U;
    uint32_t reserved43 : 8U;
    uint32_t reserved44 : 8U;
    uint32_t reserved45 : 8U;
    uint32_t reserved46 : 8U;
    uint32_t reserved47 : 8U;
  }sig;
} Device_IRK_Group_2_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  Device_IRK_Group_2_un msg;
}Device_IRK_Group_2_msg;

// def @BTA4_FusnData CAN Message (782  0x30e)
#define BTA4_FusnData_IDE (0U)
#define BTA4_FusnData_DLC (64U)
#define BTA4_FusnData_CANID (0x30eU)
#define BTA4_FusnData_CYC (100U)

typedef enum
{
    Invalid_242 = 255U
}FusnData_2rlSt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_243 = 255U
}FusnData_2rlSt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_244 = 255U
}FusnData_2rlSt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_245 = 255U
}FusnData_2rlSt_Reserved_Sr_t;

typedef enum
{
    Invalid_246 = 255U
}FusnData_2rmSt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_247 = 255U
}FusnData_2rmSt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_248 = 255U
}FusnData_2rmSt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_249 = 255U
}FusnData_2rmSt_Reserved_Sr_t;

typedef enum
{
    Invalid_250 = 255U
}FusnData_2rrSt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_251 = 255U
}FusnData_2rrSt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_252 = 255U
}FusnData_2rrSt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_253 = 255U
}FusnData_2rrSt_Reserved_Sr_t;

typedef enum
{
    Invalid_254 = 255U
}FusnData_2rlFt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_255 = 255U
}FusnData_2rlFt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_256 = 255U
}FusnData_2rlFt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_257 = 255U
}FusnData_2rlFt_Reserved_Sr_t;

typedef enum
{
    Invalid_258 = 255U
}FusnData_2rmFt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_259 = 255U
}FusnData_2rmFt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_260 = 255U
}FusnData_2rmFt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_261 = 255U
}FusnData_2rmFt_Reserved_Sr_t;

typedef enum
{
    Invalid_262 = 255U
}FusnData_2rrFt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_263 = 255U
}FusnData_2rrFt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_264 = 255U
}FusnData_2rrFt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_265 = 255U
}FusnData_2rrFt_Reserved_Sr_t;

typedef union
{
  uint8_t data[64u];
  struct
  {
    // Child Living Detecting Identify SRR and SRF Fusion Data_Breath Energy_2 Row Left Seat
    //  255 : "Invalid"
    FusnData_2rlSt_BrthEngy_Sr_t FusnData_2rlSt_BrthEngy_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Adult Breath Rate_2 Row Left Seat
    //  255 : "Invalid"
    FusnData_2rlSt_AdltBrthRate_Sr_t FusnData_2rlSt_AdltBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Children Breath Rate_2 Row Left Seat
    //  255 : "Invalid"
    FusnData_2rlSt_ChildBrthRate_Sr_t FusnData_2rlSt_ChildBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Reserved_2 Row Left Seat
    //  255 : "Invalid"
    FusnData_2rlSt_Reserved_Sr_t FusnData_2rlSt_Reserved_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Breath Energy_2 Row Middle Seat
    //  255 : "Invalid"
    FusnData_2rmSt_BrthEngy_Sr_t FusnData_2rmSt_BrthEngy_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Adult Breath Rate_2 Row Middle Seat
    //  255 : "Invalid"
    FusnData_2rmSt_AdltBrthRate_Sr_t FusnData_2rmSt_AdltBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Children Breath Rate_2 Row Middle Seat
    //  255 : "Invalid"
    FusnData_2rmSt_ChildBrthRate_Sr_t FusnData_2rmSt_ChildBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Reserved_2 Row Middle Seat
    //  255 : "Invalid"
    FusnData_2rmSt_Reserved_Sr_t FusnData_2rmSt_Reserved_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Breath Energy_2 Row Right Seat
    //  255 : "Invalid"
    FusnData_2rrSt_BrthEngy_Sr_t FusnData_2rrSt_BrthEngy_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Adult Breath Rate_2 Row Right Seat
    //  255 : "Invalid"
    FusnData_2rrSt_AdltBrthRate_Sr_t FusnData_2rrSt_AdltBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Children Breath Rate_2 Row Right Seat
    //  255 : "Invalid"
    FusnData_2rrSt_ChildBrthRate_Sr_t FusnData_2rrSt_ChildBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Reserved_2 Row Right Seat
    //  255 : "Invalid"
    FusnData_2rrSt_Reserved_Sr_t FusnData_2rrSt_Reserved_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Breath Energy_2 Row Left Foot
    //  255 : "Invalid"
    FusnData_2rlFt_BrthEngy_Sr_t FusnData_2rlFt_BrthEngy_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Adult Breath Rate_2 Row Left Foot
    //  255 : "Invalid"
    FusnData_2rlFt_AdltBrthRate_Sr_t FusnData_2rlFt_AdltBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Children Breath Rate_2 Row Left Foot
    //  255 : "Invalid"
    FusnData_2rlFt_ChildBrthRate_Sr_t FusnData_2rlFt_ChildBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Reserved_2 Row Left Foot
    //  255 : "Invalid"
    FusnData_2rlFt_Reserved_Sr_t FusnData_2rlFt_Reserved_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Breath Energy_2 Row Middle Foot
    //  255 : "Invalid"
    FusnData_2rmFt_BrthEngy_Sr_t FusnData_2rmFt_BrthEngy_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Adult Breath Rate
    //  255 : "Invalid"
    FusnData_2rmFt_AdltBrthRate_Sr_t FusnData_2rmFt_AdltBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Children Breath Rate_2 Row Middle Foot
    //  255 : "Invalid"
    FusnData_2rmFt_ChildBrthRate_Sr_t FusnData_2rmFt_ChildBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Reserved_2 Row Middle Foot
    //  255 : "Invalid"
    FusnData_2rmFt_Reserved_Sr_t FusnData_2rmFt_Reserved_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Breath Energy_2 Row Right Foot
    //  255 : "Invalid"
    FusnData_2rrFt_BrthEngy_Sr_t FusnData_2rrFt_BrthEngy_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Adult Breath Rate_2 Row Right Foot
    //  255 : "Invalid"
    FusnData_2rrFt_AdltBrthRate_Sr_t FusnData_2rrFt_AdltBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Children Breath Rate_2 Row Right Foot
    //  255 : "Invalid"
    FusnData_2rrFt_ChildBrthRate_Sr_t FusnData_2rrFt_ChildBrthRate_Sr : 8U;
    // Child Living Detecting Identify SRR and SRF Fusion Data_Reserved_2 Row Right Foot
    //  255 : "Invalid"
    FusnData_2rrFt_Reserved_Sr_t FusnData_2rrFt_Reserved_Sr : 8U;
    uint8_t FusnData_2rlSt_AdltDistance_Sr_0 : 8U;
    uint8_t FusnData_2rlSt_AdltDistance_Sr_1 : 8U;
    uint8_t FusnData_2rlSt_ChildDistance_Sr_0 : 8U;
    uint8_t FusnData_2rlSt_ChildDistance_Sr_1 : 8U;
    uint8_t FusnData_2rmSt_AdltDistance_Sr_0 : 8U;
    uint8_t FusnData_2rmSt_AdltDistance_Sr_1 : 8U;
    uint8_t FusnData_2rmSt_ChildDistance_Sr_0 : 8U;
    uint8_t FusnData_2rmSt_ChildDistance_Sr_1 : 8U;
    uint8_t FusnData_2rrSt_AdltDistance_Sr_0 : 8U;
    uint8_t FusnData_2rrSt_AdltDistance_Sr_1 : 8U;
    uint8_t FusnData_2rrSt_ChildDistance_Sr_0 : 8U;
    uint8_t FusnData_2rrSt_ChildDistance_Sr_1 : 8U;
    uint8_t FusnData_2rlFt_AdltDistance_Sr_0 : 8U;
    uint8_t FusnData_2rlFt_AdltDistance_Sr_1 : 8U;
    uint8_t FusnData_2rlFt_ChildDistance_Sr_0 : 8U;
    uint8_t FusnData_2rlFt_ChildDistance_Sr_1 : 8U;
    uint8_t FusnData_2rmFt_AdltDistance_Sr_0 : 8U;
    uint8_t FusnData_2rmFt_AdltDistance_Sr_1 : 8U;
    uint8_t FusnData_2rmFt_ChildDistance_Sr_0 : 8U;
    uint8_t FusnData_2rmFt_ChildDistance_Sr_1 : 8U;
    uint8_t FusnData_2rrFt_AdltDistance_Sr_0 : 8U;
    uint8_t FusnData_2rrFt_AdltDistance_Sr_1 : 8U;
    uint8_t FusnData_2rrFt_ChildDistance_Sr_0 : 8U;
    uint8_t FusnData_2rrFt_ChildDistance_Sr_1 : 8U;
    uint8_t FusnData__Reserved1_Sr_0 : 8U;
    uint8_t FusnData__Reserved1_Sr_1 : 8U;
    uint8_t FusnData__Reserved1_Sr_2 : 8U;
    uint8_t FusnData__Reserved1_Sr_3 : 8U;
    uint8_t FusnData__Reserved1_Sr_4 : 8U;
    uint8_t FusnData__Reserved1_Sr_5 : 8U;
    uint8_t FusnData__Reserved1_Sr_6 : 8U;
    uint8_t FusnData__Reserved1_Sr_7 : 8U;
    uint8_t FusnData__Reserved2_Sr_0 : 8U;
    uint8_t FusnData__Reserved2_Sr_1 : 8U;
    uint8_t FusnData__Reserved2_Sr_2 : 8U;
    uint8_t FusnData__Reserved2_Sr_3 : 8U;
    uint8_t FusnData__Reserved2_Sr_4 : 8U;
    uint8_t FusnData__Reserved2_Sr_5 : 8U;
    uint8_t FusnData__Reserved2_Sr_6 : 8U;
    uint8_t FusnData__Reserved2_Sr_7 : 8U;
  }sig;
} BTA4_FusnData_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA4_FusnData_un msg;
}BTA4_FusnData_msg;

// def @ZONE_3AA CAN Message (938  0x3aa)
#define ZONE_3AA_IDE (0U)
#define ZONE_3AA_DLC (8U)
#define ZONE_3AA_CANID (0x3aaU)
#define ZONE_3AA_CYC (1000U)

typedef union
{
  uint8_t data[8u];
  struct
  {
    uint8_t VehOdometer_0 : 8U; // Unit:'km'
    uint8_t VehOdometer_1 : 8U; // Unit:'km'
    uint8_t VehOdometer_2 : 8U; // Unit:'km'
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
  }sig;
} ZONE_3AA_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  ZONE_3AA_un msg;
}ZONE_3AA_msg;

// def @ZONE_3B2 CAN Message (946  0x3b2)
#define ZONE_3B2_IDE (0U)
#define ZONE_3B2_DLC (8U)
#define ZONE_3B2_CANID (0x3b2U)
#define ZONE_3B2_CYC (1000U)
// signal: @Yr_phy
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_Yr_phy_CovFactor (1)
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_Yr_phy_toS(x) ( (uint8_t) ((x) - (2000)) )
#define ALPS_NT3_BTA_SRR_1229_BL051_V_1_0_Yr_phy_fromS(x) ( ((x) + (2000)) )

typedef union
{
  uint8_t data[8u];
  struct
  {
    uint8_t Yr_phy : 8U;// Offset= 2000               Unit:'Year'
    // Month
    uint8_t Mth : 4U; // Unit:'Month'
    uint32_t reserved0 : 4U;
    // Day
    uint8_t Day : 5U; // Unit:'Day'
    uint32_t reserved1 : 3U;
    // Hour
    uint8_t Hr : 5U; // Unit:'Hour'
    uint32_t reserved2 : 3U;
    // Minute
    uint8_t Min : 6U; // Unit:'Minute'
    uint32_t reserved3 : 2U;
    // Second
    uint8_t Sec : 6U; // Unit:'Second'
    uint32_t reserved4 : 2U;
    uint8_t Msec_0 : 2U; // Unit:'Millisecond'
    uint8_t Msec_1 : 8U; // Unit:'Millisecond'
    uint32_t reserved5 : 6U;
  }sig;
} ZONE_3B2_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  ZONE_3B2_un msg;
}ZONE_3B2_msg;

// def @BTA4_Log_Data CAN Message (1173 0x495)
#define BTA4_Log_Data_IDE (0U)
#define BTA4_Log_Data_DLC (64U)
#define BTA4_Log_Data_CANID (0x495U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    // BTA4 Log Level
    uint8_t BTA4_Log_Level : 8U;
    uint8_t BTA4_Log_Sender_0 : 8U;
    uint8_t BTA4_Log_Sender_1 : 8U;
    uint8_t BTA4_Log_Sender_2 : 8U;
    uint8_t BTA4_Log_Data1_0 : 8U;
    uint8_t BTA4_Log_Data1_1 : 8U;
    uint8_t BTA4_Log_Data1_2 : 8U;
    uint8_t BTA4_Log_Data1_3 : 8U;
    uint8_t BTA4_Log_Data2_0 : 8U;
    uint8_t BTA4_Log_Data2_1 : 8U;
    uint8_t BTA4_Log_Data2_2 : 8U;
    uint8_t BTA4_Log_Data2_3 : 8U;
    uint8_t BTA4_Log_Data3_0 : 8U;
    uint8_t BTA4_Log_Data3_1 : 8U;
    uint8_t BTA4_Log_Data3_2 : 8U;
    uint8_t BTA4_Log_Data3_3 : 8U;
    uint8_t BTA4_Log_Data4_0 : 8U;
    uint8_t BTA4_Log_Data4_1 : 8U;
    uint8_t BTA4_Log_Data4_2 : 8U;
    uint8_t BTA4_Log_Data4_3 : 8U;
    uint8_t BTA4_Log_Data5_0 : 8U;
    uint8_t BTA4_Log_Data5_1 : 8U;
    uint8_t BTA4_Log_Data5_2 : 8U;
    uint8_t BTA4_Log_Data5_3 : 8U;
    uint8_t BTA4_Log_Data6_0 : 8U;
    uint8_t BTA4_Log_Data6_1 : 8U;
    uint8_t BTA4_Log_Data6_2 : 8U;
    uint8_t BTA4_Log_Data6_3 : 8U;
    uint8_t BTA4_Log_Data7_0 : 8U;
    uint8_t BTA4_Log_Data7_1 : 8U;
    uint8_t BTA4_Log_Data7_2 : 8U;
    uint8_t BTA4_Log_Data7_3 : 8U;
    uint8_t BTA4_Log_Data8_0 : 8U;
    uint8_t BTA4_Log_Data8_1 : 8U;
    uint8_t BTA4_Log_Data8_2 : 8U;
    uint8_t BTA4_Log_Data8_3 : 8U;
    uint8_t BTA4_Log_Data9_0 : 8U;
    uint8_t BTA4_Log_Data9_1 : 8U;
    uint8_t BTA4_Log_Data9_2 : 8U;
    uint8_t BTA4_Log_Data9_3 : 8U;
    uint8_t BTA4_Log_Data10_0 : 8U;
    uint8_t BTA4_Log_Data10_1 : 8U;
    uint8_t BTA4_Log_Data10_2 : 8U;
    uint8_t BTA4_Log_Data10_3 : 8U;
    uint8_t BTA4_Log_Data11_0 : 8U;
    uint8_t BTA4_Log_Data11_1 : 8U;
    uint8_t BTA4_Log_Data11_2 : 8U;
    uint8_t BTA4_Log_Data11_3 : 8U;
    uint8_t BTA4_Log_Data12_0 : 8U;
    uint8_t BTA4_Log_Data12_1 : 8U;
    uint8_t BTA4_Log_Data12_2 : 8U;
    uint8_t BTA4_Log_Data12_3 : 8U;
    uint8_t BTA4_Log_Data13_0 : 8U;
    uint8_t BTA4_Log_Data13_1 : 8U;
    uint8_t BTA4_Log_Data13_2 : 8U;
    uint8_t BTA4_Log_Data13_3 : 8U;
    uint8_t BTA4_Log_Data14_0 : 8U;
    uint8_t BTA4_Log_Data14_1 : 8U;
    uint8_t BTA4_Log_Data14_2 : 8U;
    uint8_t BTA4_Log_Data14_3 : 8U;
    uint8_t BTA4_Log_Data15_0 : 8U;
    uint8_t BTA4_Log_Data15_1 : 8U;
    uint8_t BTA4_Log_Data15_2 : 8U;
    uint8_t BTA4_Log_Data15_3 : 8U;
  }sig;
} BTA4_Log_Data_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA4_Log_Data_un msg;
}BTA4_Log_Data_msg;

// def @IotConnectSts CAN Message (1179 0x49b)
#define IotConnectSts_IDE (0U)
#define IotConnectSts_DLC (64U)
#define IotConnectSts_CANID (0x49bU)

typedef enum
{
    ConnectSts_disconnect_266 = 0U,
    ConnectSts_connect_267 = 1U,
    ConnectSts_RFU_268 = 2U,
    ConnectSts_RFU_269 = 3U
}ConnectSts_t;

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t ScanDeviceAddr_0 : 8U;
    uint8_t ScanDeviceAddr_1 : 8U;
    uint8_t ScanDeviceAddr_2 : 8U;
    uint8_t ScanDeviceAddr_3 : 8U;
    uint8_t ScanDeviceAddr_4 : 8U;
    uint8_t ScanDeviceAddr_5 : 8U;
    uint8_t ScanBroadcastData_0_0 : 8U;
    uint8_t ScanBroadcastData_0_1 : 8U;
    uint8_t ScanBroadcastData_0_2 : 8U;
    uint8_t ScanBroadcastData_0_3 : 8U;
    uint8_t ScanBroadcastData_0_4 : 8U;
    uint8_t ScanBroadcastData_0_5 : 8U;
    uint8_t ScanBroadcastData_0_6 : 8U;
    uint8_t ScanBroadcastData_0_7 : 8U;
    uint8_t ScanBroadcastData_1_0 : 8U;
    uint8_t ScanBroadcastData_1_1 : 8U;
    uint8_t ScanBroadcastData_1_2 : 8U;
    uint8_t ScanBroadcastData_1_3 : 8U;
    uint8_t ScanBroadcastData_1_4 : 8U;
    uint8_t ScanBroadcastData_1_5 : 8U;
    uint8_t ScanBroadcastData_1_6 : 8U;
    uint8_t ScanBroadcastData_1_7 : 8U;
    uint8_t ScanBroadcastData_2_0 : 8U;
    uint8_t ScanBroadcastData_2_1 : 8U;
    uint8_t ScanBroadcastData_2_2 : 8U;
    uint8_t ScanBroadcastData_2_3 : 8U;
    uint8_t ScanBroadcastData_2_4 : 8U;
    uint8_t ScanBroadcastData_2_5 : 8U;
    uint8_t ScanBroadcastData_2_6 : 8U;
    uint8_t ScanBroadcastData_2_7 : 8U;
    uint8_t ScanBroadcastData_3_0 : 8U;
    uint8_t ScanBroadcastData_3_1 : 8U;
    uint8_t ScanBroadcastData_3_2 : 8U;
    uint8_t ScanBroadcastData_3_3 : 8U;
    uint8_t ScanBroadcastData_3_4 : 8U;
    uint8_t ScanBroadcastData_3_5 : 8U;
    uint8_t ScanBroadcastData_3_6 : 8U;
    uint8_t ConnectStsDeviceAddr_0 : 8U;
    uint8_t ConnectStsDeviceAddr_1 : 8U;
    uint8_t ConnectStsDeviceAddr_2 : 8U;
    uint8_t ConnectStsDeviceAddr_3 : 8U;
    uint8_t ConnectStsDeviceAddr_4 : 8U;
    uint8_t ConnectStsDeviceAddr_5 : 8U;
    uint16_t ConnectStsMtu_0 : 8U;
    // connected status
    //  0 : "disconnect"
    //  1 : "connect"
    //  2 : "RFU"
    //  3 : "RFU"
    ConnectSts_t ConnectSts : 4U;
    uint16_t ConnectStsMtu_1 : 4U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    uint32_t reserved6 : 8U;
    uint32_t reserved7 : 8U;
    uint32_t reserved8 : 8U;
    uint32_t reserved9 : 8U;
    uint32_t reserved10 : 8U;
    uint32_t reserved11 : 8U;
    uint32_t reserved12 : 8U;
    uint32_t reserved13 : 8U;
    uint32_t reserved14 : 8U;
    uint32_t reserved15 : 8U;
    uint32_t reserved16 : 8U;
    uint32_t reserved17 : 8U;
    uint32_t reserved18 : 8U;
  }sig;
} IotConnectSts_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  IotConnectSts_un msg;
}IotConnectSts_msg;

// def @IotDownLinkData CAN Message (1180 0x49c)
#define IotDownLinkData_IDE (0U)
#define IotDownLinkData_DLC (64U)
#define IotDownLinkData_CANID (0x49cU)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t IotDownLinkData_0_0 : 8U;
    uint8_t IotDownLinkData_0_1 : 8U;
    uint8_t IotDownLinkData_0_2 : 8U;
    uint8_t IotDownLinkData_0_3 : 8U;
    uint8_t IotDownLinkData_0_4 : 8U;
    uint8_t IotDownLinkData_0_5 : 8U;
    uint8_t IotDownLinkData_0_6 : 8U;
    uint8_t IotDownLinkData_0_7 : 8U;
    uint8_t IotDownLinkData_1_0 : 8U;
    uint8_t IotDownLinkData_1_1 : 8U;
    uint8_t IotDownLinkData_1_2 : 8U;
    uint8_t IotDownLinkData_1_3 : 8U;
    uint8_t IotDownLinkData_1_4 : 8U;
    uint8_t IotDownLinkData_1_5 : 8U;
    uint8_t IotDownLinkData_1_6 : 8U;
    uint8_t IotDownLinkData_1_7 : 8U;
    uint8_t IotDownLinkData_2_0 : 8U;
    uint8_t IotDownLinkData_2_1 : 8U;
    uint8_t IotDownLinkData_2_2 : 8U;
    uint8_t IotDownLinkData_2_3 : 8U;
    uint8_t IotDownLinkData_2_4 : 8U;
    uint8_t IotDownLinkData_2_5 : 8U;
    uint8_t IotDownLinkData_2_6 : 8U;
    uint8_t IotDownLinkData_2_7 : 8U;
    uint8_t IotDownLinkData_3_0 : 8U;
    uint8_t IotDownLinkData_3_1 : 8U;
    uint8_t IotDownLinkData_3_2 : 8U;
    uint8_t IotDownLinkData_3_3 : 8U;
    uint8_t IotDownLinkData_3_4 : 8U;
    uint8_t IotDownLinkData_3_5 : 8U;
    uint8_t IotDownLinkData_3_6 : 8U;
    uint8_t IotDownLinkData_3_7 : 8U;
    uint8_t IotDownLinkData_4_0 : 8U;
    uint8_t IotDownLinkData_4_1 : 8U;
    uint8_t IotDownLinkData_4_2 : 8U;
    uint8_t IotDownLinkData_4_3 : 8U;
    uint8_t IotDownLinkData_4_4 : 8U;
    uint8_t IotDownLinkData_4_5 : 8U;
    uint8_t IotDownLinkData_4_6 : 8U;
    uint8_t IotDownLinkData_4_7 : 8U;
    uint8_t IotDownLinkData_5_0 : 8U;
    uint8_t IotDownLinkData_5_1 : 8U;
    uint8_t IotDownLinkData_5_2 : 8U;
    uint8_t IotDownLinkData_5_3 : 8U;
    uint8_t IotDownLinkData_5_4 : 8U;
    uint8_t IotDownLinkData_5_5 : 8U;
    uint8_t IotDownLinkData_5_6 : 8U;
    uint8_t IotDownLinkData_5_7 : 8U;
    uint8_t IotDownLinkData_6_0 : 8U;
    uint8_t IotDownLinkData_6_1 : 8U;
    uint8_t IotDownLinkData_6_2 : 8U;
    uint8_t IotDownLinkData_6_3 : 8U;
    uint8_t IotDownLinkData_6_4 : 8U;
    uint8_t IotDownLinkData_6_5 : 8U;
    uint8_t IotDownLinkData_6_6 : 8U;
    uint8_t IotDownLinkData_6_7 : 8U;
    uint8_t IotDownLinkData_7_0 : 8U;
    uint8_t IotDownLinkData_7_1 : 8U;
    uint8_t IotDownLinkData_7_2 : 8U;
    uint8_t IotDownLinkData_7_3 : 8U;
    uint8_t IotDownLinkData_7_4 : 8U;
    uint8_t IotDownLinkData_7_5 : 8U;
    uint8_t IotDownLinkData_7_6 : 8U;
    uint8_t IotDownLinkData_7_7 : 8U;
  }sig;
} IotDownLinkData_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  IotDownLinkData_un msg;
}IotDownLinkData_msg;

// def @IotUpLinkFlowCtrl CAN Message (1181 0x49d)
#define IotUpLinkFlowCtrl_IDE (0U)
#define IotUpLinkFlowCtrl_DLC (64U)
#define IotUpLinkFlowCtrl_CANID (0x49dU)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t IotUpLinkFlowCtrl_0_0 : 8U;
    uint8_t IotUpLinkFlowCtrl_0_1 : 8U;
    uint8_t IotUpLinkFlowCtrl_0_2 : 8U;
    uint8_t IotUpLinkFlowCtrl_0_3 : 8U;
    uint8_t IotUpLinkFlowCtrl_0_4 : 8U;
    uint8_t IotUpLinkFlowCtrl_0_5 : 8U;
    uint8_t IotUpLinkFlowCtrl_0_6 : 8U;
    uint8_t IotUpLinkFlowCtrl_0_7 : 8U;
    uint8_t IotUpLinkFlowCtrl_1_0 : 8U;
    uint8_t IotUpLinkFlowCtrl_1_1 : 8U;
    uint8_t IotUpLinkFlowCtrl_1_2 : 8U;
    uint8_t IotUpLinkFlowCtrl_1_3 : 8U;
    uint8_t IotUpLinkFlowCtrl_1_4 : 8U;
    uint8_t IotUpLinkFlowCtrl_1_5 : 8U;
    uint8_t IotUpLinkFlowCtrl_1_6 : 8U;
    uint8_t IotUpLinkFlowCtrl_1_7 : 8U;
    uint8_t IotUpLinkFlowCtrl_2_0 : 8U;
    uint8_t IotUpLinkFlowCtrl_2_1 : 8U;
    uint8_t IotUpLinkFlowCtrl_2_2 : 8U;
    uint8_t IotUpLinkFlowCtrl_2_3 : 8U;
    uint8_t IotUpLinkFlowCtrl_2_4 : 8U;
    uint8_t IotUpLinkFlowCtrl_2_5 : 8U;
    uint8_t IotUpLinkFlowCtrl_2_6 : 8U;
    uint8_t IotUpLinkFlowCtrl_2_7 : 8U;
    uint8_t IotUpLinkFlowCtrl_3_0 : 8U;
    uint8_t IotUpLinkFlowCtrl_3_1 : 8U;
    uint8_t IotUpLinkFlowCtrl_3_2 : 8U;
    uint8_t IotUpLinkFlowCtrl_3_3 : 8U;
    uint8_t IotUpLinkFlowCtrl_3_4 : 8U;
    uint8_t IotUpLinkFlowCtrl_3_5 : 8U;
    uint8_t IotUpLinkFlowCtrl_3_6 : 8U;
    uint8_t IotUpLinkFlowCtrl_3_7 : 8U;
    uint8_t IotUpLinkFlowCtrl_4_0 : 8U;
    uint8_t IotUpLinkFlowCtrl_4_1 : 8U;
    uint8_t IotUpLinkFlowCtrl_4_2 : 8U;
    uint8_t IotUpLinkFlowCtrl_4_3 : 8U;
    uint8_t IotUpLinkFlowCtrl_4_4 : 8U;
    uint8_t IotUpLinkFlowCtrl_4_5 : 8U;
    uint8_t IotUpLinkFlowCtrl_4_6 : 8U;
    uint8_t IotUpLinkFlowCtrl_4_7 : 8U;
    uint8_t IotUpLinkFlowCtrl_5_0 : 8U;
    uint8_t IotUpLinkFlowCtrl_5_1 : 8U;
    uint8_t IotUpLinkFlowCtrl_5_2 : 8U;
    uint8_t IotUpLinkFlowCtrl_5_3 : 8U;
    uint8_t IotUpLinkFlowCtrl_5_4 : 8U;
    uint8_t IotUpLinkFlowCtrl_5_5 : 8U;
    uint8_t IotUpLinkFlowCtrl_5_6 : 8U;
    uint8_t IotUpLinkFlowCtrl_5_7 : 8U;
    uint8_t IotUpLinkFlowCtrl_6_0 : 8U;
    uint8_t IotUpLinkFlowCtrl_6_1 : 8U;
    uint8_t IotUpLinkFlowCtrl_6_2 : 8U;
    uint8_t IotUpLinkFlowCtrl_6_3 : 8U;
    uint8_t IotUpLinkFlowCtrl_6_4 : 8U;
    uint8_t IotUpLinkFlowCtrl_6_5 : 8U;
    uint8_t IotUpLinkFlowCtrl_6_6 : 8U;
    uint8_t IotUpLinkFlowCtrl_6_7 : 8U;
    uint8_t IotUpLinkFlowCtrl_7_0 : 8U;
    uint8_t IotUpLinkFlowCtrl_7_1 : 8U;
    uint8_t IotUpLinkFlowCtrl_7_2 : 8U;
    uint8_t IotUpLinkFlowCtrl_7_3 : 8U;
    uint8_t IotUpLinkFlowCtrl_7_4 : 8U;
    uint8_t IotUpLinkFlowCtrl_7_5 : 8U;
    uint8_t IotUpLinkFlowCtrl_7_6 : 8U;
    uint8_t IotUpLinkFlowCtrl_7_7 : 8U;
  }sig;
} IotUpLinkFlowCtrl_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  IotUpLinkFlowCtrl_un msg;
}IotUpLinkFlowCtrl_msg;

// def @IotUplinkData CAN Message (1182 0x49e)
#define IotUplinkData_IDE (0U)
#define IotUplinkData_DLC (64U)
#define IotUplinkData_CANID (0x49eU)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t IotUplinkData_0_0 : 8U;
    uint8_t IotUplinkData_0_1 : 8U;
    uint8_t IotUplinkData_0_2 : 8U;
    uint8_t IotUplinkData_0_3 : 8U;
    uint8_t IotUplinkData_0_4 : 8U;
    uint8_t IotUplinkData_0_5 : 8U;
    uint8_t IotUplinkData_0_6 : 8U;
    uint8_t IotUplinkData_0_7 : 8U;
    uint8_t IotUplinkData_1_0 : 8U;
    uint8_t IotUplinkData_1_1 : 8U;
    uint8_t IotUplinkData_1_2 : 8U;
    uint8_t IotUplinkData_1_3 : 8U;
    uint8_t IotUplinkData_1_4 : 8U;
    uint8_t IotUplinkData_1_5 : 8U;
    uint8_t IotUplinkData_1_6 : 8U;
    uint8_t IotUplinkData_1_7 : 8U;
    uint8_t IotUplinkData_2_0 : 8U;
    uint8_t IotUplinkData_2_1 : 8U;
    uint8_t IotUplinkData_2_2 : 8U;
    uint8_t IotUplinkData_2_3 : 8U;
    uint8_t IotUplinkData_2_4 : 8U;
    uint8_t IotUplinkData_2_5 : 8U;
    uint8_t IotUplinkData_2_6 : 8U;
    uint8_t IotUplinkData_2_7 : 8U;
    uint8_t IotUplinkData_3_0 : 8U;
    uint8_t IotUplinkData_3_1 : 8U;
    uint8_t IotUplinkData_3_2 : 8U;
    uint8_t IotUplinkData_3_3 : 8U;
    uint8_t IotUplinkData_3_4 : 8U;
    uint8_t IotUplinkData_3_5 : 8U;
    uint8_t IotUplinkData_3_6 : 8U;
    uint8_t IotUplinkData_3_7 : 8U;
    uint8_t IotUplinkData_4_0 : 8U;
    uint8_t IotUplinkData_4_1 : 8U;
    uint8_t IotUplinkData_4_2 : 8U;
    uint8_t IotUplinkData_4_3 : 8U;
    uint8_t IotUplinkData_4_4 : 8U;
    uint8_t IotUplinkData_4_5 : 8U;
    uint8_t IotUplinkData_4_6 : 8U;
    uint8_t IotUplinkData_4_7 : 8U;
    uint8_t IotUplinkData_5_0 : 8U;
    uint8_t IotUplinkData_5_1 : 8U;
    uint8_t IotUplinkData_5_2 : 8U;
    uint8_t IotUplinkData_5_3 : 8U;
    uint8_t IotUplinkData_5_4 : 8U;
    uint8_t IotUplinkData_5_5 : 8U;
    uint8_t IotUplinkData_5_6 : 8U;
    uint8_t IotUplinkData_5_7 : 8U;
    uint8_t IotUplinkData_6_0 : 8U;
    uint8_t IotUplinkData_6_1 : 8U;
    uint8_t IotUplinkData_6_2 : 8U;
    uint8_t IotUplinkData_6_3 : 8U;
    uint8_t IotUplinkData_6_4 : 8U;
    uint8_t IotUplinkData_6_5 : 8U;
    uint8_t IotUplinkData_6_6 : 8U;
    uint8_t IotUplinkData_6_7 : 8U;
    uint8_t IotUplinkData_7_0 : 8U;
    uint8_t IotUplinkData_7_1 : 8U;
    uint8_t IotUplinkData_7_2 : 8U;
    uint8_t IotUplinkData_7_3 : 8U;
    uint8_t IotUplinkData_7_4 : 8U;
    uint8_t IotUplinkData_7_5 : 8U;
    uint8_t IotUplinkData_7_6 : 8U;
    uint8_t IotUplinkData_7_7 : 8U;
  }sig;
} IotUplinkData_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  IotUplinkData_un msg;
}IotUplinkData_msg;

// def @IotDownLinkFlowCtrl CAN Message (1183 0x49f)
#define IotDownLinkFlowCtrl_IDE (0U)
#define IotDownLinkFlowCtrl_DLC (64U)
#define IotDownLinkFlowCtrl_CANID (0x49fU)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t IotDownLinkFlowCtrl_0_0 : 8U;
    uint8_t IotDownLinkFlowCtrl_0_1 : 8U;
    uint8_t IotDownLinkFlowCtrl_0_2 : 8U;
    uint8_t IotDownLinkFlowCtrl_0_3 : 8U;
    uint8_t IotDownLinkFlowCtrl_0_4 : 8U;
    uint8_t IotDownLinkFlowCtrl_0_5 : 8U;
    uint8_t IotDownLinkFlowCtrl_0_6 : 8U;
    uint8_t IotDownLinkFlowCtrl_0_7 : 8U;
    uint8_t IotDownLinkFlowCtrl_1_0 : 8U;
    uint8_t IotDownLinkFlowCtrl_1_1 : 8U;
    uint8_t IotDownLinkFlowCtrl_1_2 : 8U;
    uint8_t IotDownLinkFlowCtrl_1_3 : 8U;
    uint8_t IotDownLinkFlowCtrl_1_4 : 8U;
    uint8_t IotDownLinkFlowCtrl_1_5 : 8U;
    uint8_t IotDownLinkFlowCtrl_1_6 : 8U;
    uint8_t IotDownLinkFlowCtrl_1_7 : 8U;
    uint8_t IotDownLinkFlowCtrl_2_0 : 8U;
    uint8_t IotDownLinkFlowCtrl_2_1 : 8U;
    uint8_t IotDownLinkFlowCtrl_2_2 : 8U;
    uint8_t IotDownLinkFlowCtrl_2_3 : 8U;
    uint8_t IotDownLinkFlowCtrl_2_4 : 8U;
    uint8_t IotDownLinkFlowCtrl_2_5 : 8U;
    uint8_t IotDownLinkFlowCtrl_2_6 : 8U;
    uint8_t IotDownLinkFlowCtrl_2_7 : 8U;
    uint8_t IotDownLinkFlowCtrl_3_0 : 8U;
    uint8_t IotDownLinkFlowCtrl_3_1 : 8U;
    uint8_t IotDownLinkFlowCtrl_3_2 : 8U;
    uint8_t IotDownLinkFlowCtrl_3_3 : 8U;
    uint8_t IotDownLinkFlowCtrl_3_4 : 8U;
    uint8_t IotDownLinkFlowCtrl_3_5 : 8U;
    uint8_t IotDownLinkFlowCtrl_3_6 : 8U;
    uint8_t IotDownLinkFlowCtrl_3_7 : 8U;
    uint8_t IotDownLinkFlowCtrl_4_0 : 8U;
    uint8_t IotDownLinkFlowCtrl_4_1 : 8U;
    uint8_t IotDownLinkFlowCtrl_4_2 : 8U;
    uint8_t IotDownLinkFlowCtrl_4_3 : 8U;
    uint8_t IotDownLinkFlowCtrl_4_4 : 8U;
    uint8_t IotDownLinkFlowCtrl_4_5 : 8U;
    uint8_t IotDownLinkFlowCtrl_4_6 : 8U;
    uint8_t IotDownLinkFlowCtrl_4_7 : 8U;
    uint8_t IotDownLinkFlowCtrl_5_0 : 8U;
    uint8_t IotDownLinkFlowCtrl_5_1 : 8U;
    uint8_t IotDownLinkFlowCtrl_5_2 : 8U;
    uint8_t IotDownLinkFlowCtrl_5_3 : 8U;
    uint8_t IotDownLinkFlowCtrl_5_4 : 8U;
    uint8_t IotDownLinkFlowCtrl_5_5 : 8U;
    uint8_t IotDownLinkFlowCtrl_5_6 : 8U;
    uint8_t IotDownLinkFlowCtrl_5_7 : 8U;
    uint8_t IotDownLinkFlowCtrl_6_0 : 8U;
    uint8_t IotDownLinkFlowCtrl_6_1 : 8U;
    uint8_t IotDownLinkFlowCtrl_6_2 : 8U;
    uint8_t IotDownLinkFlowCtrl_6_3 : 8U;
    uint8_t IotDownLinkFlowCtrl_6_4 : 8U;
    uint8_t IotDownLinkFlowCtrl_6_5 : 8U;
    uint8_t IotDownLinkFlowCtrl_6_6 : 8U;
    uint8_t IotDownLinkFlowCtrl_6_7 : 8U;
    uint8_t IotDownLinkFlowCtrl_7_0 : 8U;
    uint8_t IotDownLinkFlowCtrl_7_1 : 8U;
    uint8_t IotDownLinkFlowCtrl_7_2 : 8U;
    uint8_t IotDownLinkFlowCtrl_7_3 : 8U;
    uint8_t IotDownLinkFlowCtrl_7_4 : 8U;
    uint8_t IotDownLinkFlowCtrl_7_5 : 8U;
    uint8_t IotDownLinkFlowCtrl_7_6 : 8U;
    uint8_t IotDownLinkFlowCtrl_7_7 : 8U;
  }sig;
} IotDownLinkFlowCtrl_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  IotDownLinkFlowCtrl_un msg;
}IotDownLinkFlowCtrl_msg;

// def @IotScanConnectReq CAN Message (1202 0x4b2)
#define IotScanConnectReq_IDE (0U)
#define IotScanConnectReq_DLC (8U)
#define IotScanConnectReq_CANID (0x4b2U)

typedef enum
{
    ScanReq_disable_scan_270 = 0U,
    ScanReq_enable_scan_271 = 1U,
    ScanReq_RFU_272 = 2U,
    ScanReq_RFU_273 = 3U
}ScanReq_t;

typedef enum
{
    disconnect_274 = 0U,
    connect_275 = 1U,
    RFU_276 = 2U,
    RFU_277 = 3U
}ConnectReqAction_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    uint32_t reserved0 : 4U;
    // scan request
    //  0 : "disable scan"
    //  1 : "enable scan"
    //  2 : "RFU"
    //  3 : "RFU"
    ScanReq_t ScanReq : 2U;
    // request connect
    //  0 : "disconnect"
    //  1 : "connect"
    //  2 : "RFU"
    //  3 : "RFU"
    ConnectReqAction_t ConnectReqAction : 2U;
    uint32_t reserved1 : 8U;
    uint8_t ConnectReqDeviceAddr_0 : 8U;
    uint8_t ConnectReqDeviceAddr_1 : 8U;
    uint8_t ConnectReqDeviceAddr_2 : 8U;
    uint8_t ConnectReqDeviceAddr_3 : 8U;
    uint8_t ConnectReqDeviceAddr_4 : 8U;
    uint8_t ConnectReqDeviceAddr_5 : 8U;
  }sig;
} IotScanConnectReq_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  IotScanConnectReq_un msg;
}IotScanConnectReq_msg;

// def @BTA_SRR_FactoryRsp CAN Message (1274 0x4fa)
#define BTA_SRR_FactoryRsp_IDE (0U)
#define BTA_SRR_FactoryRsp_DLC (64U)
#define BTA_SRR_FactoryRsp_CANID (0x4faU)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t BTA_SRR_FactoryRsp_0_0 : 8U;
    uint8_t BTA_SRR_FactoryRsp_0_1 : 8U;
    uint8_t BTA_SRR_FactoryRsp_0_2 : 8U;
    uint8_t BTA_SRR_FactoryRsp_0_3 : 8U;
    uint8_t BTA_SRR_FactoryRsp_0_4 : 8U;
    uint8_t BTA_SRR_FactoryRsp_0_5 : 8U;
    uint8_t BTA_SRR_FactoryRsp_0_6 : 8U;
    uint8_t BTA_SRR_FactoryRsp_0_7 : 8U;
    uint8_t BTA_SRR_FactoryRsp_1_0 : 8U;
    uint8_t BTA_SRR_FactoryRsp_1_1 : 8U;
    uint8_t BTA_SRR_FactoryRsp_1_2 : 8U;
    uint8_t BTA_SRR_FactoryRsp_1_3 : 8U;
    uint8_t BTA_SRR_FactoryRsp_1_4 : 8U;
    uint8_t BTA_SRR_FactoryRsp_1_5 : 8U;
    uint8_t BTA_SRR_FactoryRsp_1_6 : 8U;
    uint8_t BTA_SRR_FactoryRsp_1_7 : 8U;
    uint8_t BTA_SRR_FactoryRsp_2_0 : 8U;
    uint8_t BTA_SRR_FactoryRsp_2_1 : 8U;
    uint8_t BTA_SRR_FactoryRsp_2_2 : 8U;
    uint8_t BTA_SRR_FactoryRsp_2_3 : 8U;
    uint8_t BTA_SRR_FactoryRsp_2_4 : 8U;
    uint8_t BTA_SRR_FactoryRsp_2_5 : 8U;
    uint8_t BTA_SRR_FactoryRsp_2_6 : 8U;
    uint8_t BTA_SRR_FactoryRsp_2_7 : 8U;
    uint8_t BTA_SRR_FactoryRsp_3_0 : 8U;
    uint8_t BTA_SRR_FactoryRsp_3_1 : 8U;
    uint8_t BTA_SRR_FactoryRsp_3_2 : 8U;
    uint8_t BTA_SRR_FactoryRsp_3_3 : 8U;
    uint8_t BTA_SRR_FactoryRsp_3_4 : 8U;
    uint8_t BTA_SRR_FactoryRsp_3_5 : 8U;
    uint8_t BTA_SRR_FactoryRsp_3_6 : 8U;
    uint8_t BTA_SRR_FactoryRsp_3_7 : 8U;
    uint8_t BTA_SRR_FactoryRsp_4_0 : 8U;
    uint8_t BTA_SRR_FactoryRsp_4_1 : 8U;
    uint8_t BTA_SRR_FactoryRsp_4_2 : 8U;
    uint8_t BTA_SRR_FactoryRsp_4_3 : 8U;
    uint8_t BTA_SRR_FactoryRsp_4_4 : 8U;
    uint8_t BTA_SRR_FactoryRsp_4_5 : 8U;
    uint8_t BTA_SRR_FactoryRsp_4_6 : 8U;
    uint8_t BTA_SRR_FactoryRsp_4_7 : 8U;
    uint8_t BTA_SRR_FactoryRsp_5_0 : 8U;
    uint8_t BTA_SRR_FactoryRsp_5_1 : 8U;
    uint8_t BTA_SRR_FactoryRsp_5_2 : 8U;
    uint8_t BTA_SRR_FactoryRsp_5_3 : 8U;
    uint8_t BTA_SRR_FactoryRsp_5_4 : 8U;
    uint8_t BTA_SRR_FactoryRsp_5_5 : 8U;
    uint8_t BTA_SRR_FactoryRsp_5_6 : 8U;
    uint8_t BTA_SRR_FactoryRsp_5_7 : 8U;
    uint8_t BTA_SRR_FactoryRsp_6_0 : 8U;
    uint8_t BTA_SRR_FactoryRsp_6_1 : 8U;
    uint8_t BTA_SRR_FactoryRsp_6_2 : 8U;
    uint8_t BTA_SRR_FactoryRsp_6_3 : 8U;
    uint8_t BTA_SRR_FactoryRsp_6_4 : 8U;
    uint8_t BTA_SRR_FactoryRsp_6_5 : 8U;
    uint8_t BTA_SRR_FactoryRsp_6_6 : 8U;
    uint8_t BTA_SRR_FactoryRsp_6_7 : 8U;
    uint8_t BTA_SRR_FactoryRsp_7_0 : 8U;
    uint8_t BTA_SRR_FactoryRsp_7_1 : 8U;
    uint8_t BTA_SRR_FactoryRsp_7_2 : 8U;
    uint8_t BTA_SRR_FactoryRsp_7_3 : 8U;
    uint8_t BTA_SRR_FactoryRsp_7_4 : 8U;
    uint8_t BTA_SRR_FactoryRsp_7_5 : 8U;
    uint8_t BTA_SRR_FactoryRsp_7_6 : 8U;
    uint8_t BTA_SRR_FactoryRsp_7_7 : 8U;
  }sig;
} BTA_SRR_FactoryRsp_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA_SRR_FactoryRsp_un msg;
}BTA_SRR_FactoryRsp_msg;

// def @BTA_SRR_FactoryReq CAN Message (1275 0x4fb)
#define BTA_SRR_FactoryReq_IDE (0U)
#define BTA_SRR_FactoryReq_DLC (64U)
#define BTA_SRR_FactoryReq_CANID (0x4fbU)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t BTA_SRR_FactoryReq_0_0 : 8U;
    uint8_t BTA_SRR_FactoryReq_0_1 : 8U;
    uint8_t BTA_SRR_FactoryReq_0_2 : 8U;
    uint8_t BTA_SRR_FactoryReq_0_3 : 8U;
    uint8_t BTA_SRR_FactoryReq_0_4 : 8U;
    uint8_t BTA_SRR_FactoryReq_0_5 : 8U;
    uint8_t BTA_SRR_FactoryReq_0_6 : 8U;
    uint8_t BTA_SRR_FactoryReq_0_7 : 8U;
    uint8_t BTA_SRR_FactoryReq_1_0 : 8U;
    uint8_t BTA_SRR_FactoryReq_1_1 : 8U;
    uint8_t BTA_SRR_FactoryReq_1_2 : 8U;
    uint8_t BTA_SRR_FactoryReq_1_3 : 8U;
    uint8_t BTA_SRR_FactoryReq_1_4 : 8U;
    uint8_t BTA_SRR_FactoryReq_1_5 : 8U;
    uint8_t BTA_SRR_FactoryReq_1_6 : 8U;
    uint8_t BTA_SRR_FactoryReq_1_7 : 8U;
    uint8_t BTA_SRR_FactoryReq_2_0 : 8U;
    uint8_t BTA_SRR_FactoryReq_2_1 : 8U;
    uint8_t BTA_SRR_FactoryReq_2_2 : 8U;
    uint8_t BTA_SRR_FactoryReq_2_3 : 8U;
    uint8_t BTA_SRR_FactoryReq_2_4 : 8U;
    uint8_t BTA_SRR_FactoryReq_2_5 : 8U;
    uint8_t BTA_SRR_FactoryReq_2_6 : 8U;
    uint8_t BTA_SRR_FactoryReq_2_7 : 8U;
    uint8_t BTA_SRR_FactoryReq_3_0 : 8U;
    uint8_t BTA_SRR_FactoryReq_3_1 : 8U;
    uint8_t BTA_SRR_FactoryReq_3_2 : 8U;
    uint8_t BTA_SRR_FactoryReq_3_3 : 8U;
    uint8_t BTA_SRR_FactoryReq_3_4 : 8U;
    uint8_t BTA_SRR_FactoryReq_3_5 : 8U;
    uint8_t BTA_SRR_FactoryReq_3_6 : 8U;
    uint8_t BTA_SRR_FactoryReq_3_7 : 8U;
    uint8_t BTA_SRR_FactoryReq_4_0 : 8U;
    uint8_t BTA_SRR_FactoryReq_4_1 : 8U;
    uint8_t BTA_SRR_FactoryReq_4_2 : 8U;
    uint8_t BTA_SRR_FactoryReq_4_3 : 8U;
    uint8_t BTA_SRR_FactoryReq_4_4 : 8U;
    uint8_t BTA_SRR_FactoryReq_4_5 : 8U;
    uint8_t BTA_SRR_FactoryReq_4_6 : 8U;
    uint8_t BTA_SRR_FactoryReq_4_7 : 8U;
    uint8_t BTA_SRR_FactoryReq_5_0 : 8U;
    uint8_t BTA_SRR_FactoryReq_5_1 : 8U;
    uint8_t BTA_SRR_FactoryReq_5_2 : 8U;
    uint8_t BTA_SRR_FactoryReq_5_3 : 8U;
    uint8_t BTA_SRR_FactoryReq_5_4 : 8U;
    uint8_t BTA_SRR_FactoryReq_5_5 : 8U;
    uint8_t BTA_SRR_FactoryReq_5_6 : 8U;
    uint8_t BTA_SRR_FactoryReq_5_7 : 8U;
    uint8_t BTA_SRR_FactoryReq_6_0 : 8U;
    uint8_t BTA_SRR_FactoryReq_6_1 : 8U;
    uint8_t BTA_SRR_FactoryReq_6_2 : 8U;
    uint8_t BTA_SRR_FactoryReq_6_3 : 8U;
    uint8_t BTA_SRR_FactoryReq_6_4 : 8U;
    uint8_t BTA_SRR_FactoryReq_6_5 : 8U;
    uint8_t BTA_SRR_FactoryReq_6_6 : 8U;
    uint8_t BTA_SRR_FactoryReq_6_7 : 8U;
    uint8_t BTA_SRR_FactoryReq_7_0 : 8U;
    uint8_t BTA_SRR_FactoryReq_7_1 : 8U;
    uint8_t BTA_SRR_FactoryReq_7_2 : 8U;
    uint8_t BTA_SRR_FactoryReq_7_3 : 8U;
    uint8_t BTA_SRR_FactoryReq_7_4 : 8U;
    uint8_t BTA_SRR_FactoryReq_7_5 : 8U;
    uint8_t BTA_SRR_FactoryReq_7_6 : 8U;
    uint8_t BTA_SRR_FactoryReq_7_7 : 8U;
  }sig;
} BTA_SRR_FactoryReq_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA_SRR_FactoryReq_un msg;
}BTA_SRR_FactoryReq_msg;

// def @NM_ZONE_LEM CAN Message (1285 0x505)
#define NM_ZONE_LEM_IDE (0U)
#define NM_ZONE_LEM_DLC (8U)
#define NM_ZONE_LEM_CANID (0x505U)

typedef enum
{
    No_repeat_message_request_278 = 0U,
    Repeat_message_request_279 = 1U
}RepMsgReq_ZONE_LEM_t;

typedef enum
{
    PN1_ZONE_LEM_Not_awake_280 = 0U,
    PN1_ZONE_LEM_Awake_281 = 1U
}PN1_ZONE_LEM_t;

typedef enum
{
    PN2_ZONE_LEM_Not_awake_282 = 0U,
    PN2_ZONE_LEM_Awake_283 = 1U
}PN2_ZONE_LEM_t;

typedef enum
{
    PN3_ZONE_LEM_Not_awake_284 = 0U,
    PN3_ZONE_LEM_Awake_285 = 1U
}PN3_ZONE_LEM_t;

typedef enum
{
    PN4_ZONE_LEM_Not_awake_286 = 0U,
    PN4_ZONE_LEM_Awake_287 = 1U
}PN4_ZONE_LEM_t;

typedef enum
{
    PN5_ZONE_LEM_Not_awake_288 = 0U,
    PN5_ZONE_LEM_Awake_289 = 1U
}PN5_ZONE_LEM_t;

typedef enum
{
    PN6_ZONE_LEM_Not_awake_290 = 0U,
    PN6_ZONE_LEM_Awake_291 = 1U
}PN6_ZONE_LEM_t;

typedef enum
{
    PN7_ZONE_LEM_Not_awake_292 = 0U,
    PN7_ZONE_LEM_Awake_293 = 1U
}PN7_ZONE_LEM_t;

typedef enum
{
    PN8_ZONE_LEM_Not_awake_294 = 0U,
    PN8_ZONE_LEM_Awake_295 = 1U
}PN8_ZONE_LEM_t;

typedef enum
{
    PN9_ZONE_LEM_Not_awake_296 = 0U,
    PN9_ZONE_LEM_Awake_297 = 1U
}PN9_ZONE_LEM_t;

typedef enum
{
    PN10_ZONE_LEM_Not_awake_298 = 0U,
    PN10_ZONE_LEM_Awake_299 = 1U
}PN10_ZONE_LEM_t;

typedef enum
{
    PN11_ZONE_LEM_Not_awake_300 = 0U,
    PN11_ZONE_LEM_Awake_301 = 1U
}PN11_ZONE_LEM_t;

typedef enum
{
    PN12_ZONE_LEM_Not_awake_302 = 0U,
    PN12_ZONE_LEM_Awake_303 = 1U
}PN12_ZONE_LEM_t;

typedef enum
{
    PN13_ZONE_LEM_Not_awake_304 = 0U,
    PN13_ZONE_LEM_Awake_305 = 1U
}PN13_ZONE_LEM_t;

typedef enum
{
    PN14_ZONE_LEM_Not_awake_306 = 0U,
    PN14_ZONE_LEM_Awake_307 = 1U
}PN14_ZONE_LEM_t;

typedef enum
{
    PN15_ZONE_LEM_Not_awake_308 = 0U,
    PN15_ZONE_LEM_Awake_309 = 1U
}PN15_ZONE_LEM_t;

typedef enum
{
    PN16_ZONE_LEM_Not_awake_310 = 0U,
    PN16_ZONE_LEM_Awake_311 = 1U
}PN16_ZONE_LEM_t;

typedef enum
{
    PN17_ZONE_LEM_Not_awake_312 = 0U,
    PN17_ZONE_LEM_Awake_313 = 1U
}PN17_ZONE_LEM_t;

typedef enum
{
    PN18_ZONE_LEM_Not_awake_314 = 0U,
    PN18_ZONE_LEM_Awake_315 = 1U
}PN18_ZONE_LEM_t;

typedef enum
{
    PN19_ZONE_LEM_Not_awake_316 = 0U,
    PN19_ZONE_LEM_Awake_317 = 1U
}PN19_ZONE_LEM_t;

typedef enum
{
    PN20_ZONE_LEM_Not_awake_318 = 0U,
    PN20_ZONE_LEM_Awake_319 = 1U
}PN20_ZONE_LEM_t;

typedef enum
{
    PN21_ZONE_LEM_Not_awake_320 = 0U,
    PN21_ZONE_LEM_Awake_321 = 1U
}PN21_ZONE_LEM_t;

typedef enum
{
    PN22_ZONE_LEM_Not_awake_322 = 0U,
    PN22_ZONE_LEM_Awake_323 = 1U
}PN22_ZONE_LEM_t;

typedef enum
{
    PN23_ZONE_LEM_Not_awake_324 = 0U,
    PN23_ZONE_LEM_Awake_325 = 1U
}PN23_ZONE_LEM_t;

typedef enum
{
    PN24_ZONE_LEM_Not_awake_326 = 0U,
    PN24_ZONE_LEM_Awake_327 = 1U
}PN24_ZONE_LEM_t;

typedef enum
{
    PN25_ZONE_LEM_Not_awake_328 = 0U,
    PN25_ZONE_LEM_Awake_329 = 1U
}PN25_ZONE_LEM_t;

typedef enum
{
    PN26_ZONE_LEM_Not_awake_330 = 0U,
    PN26_ZONE_LEM_Awake_331 = 1U
}PN26_ZONE_LEM_t;

typedef enum
{
    PN27_ZONE_LEM_Not_awake_332 = 0U,
    PN27_ZONE_LEM_Awake_333 = 1U
}PN27_ZONE_LEM_t;

typedef enum
{
    PN28_ZONE_LEM_Not_awake_334 = 0U,
    PN28_ZONE_LEM_Awake_335 = 1U
}PN28_ZONE_LEM_t;

typedef enum
{
    PN29_ZONE_LEM_Not_awake_336 = 0U,
    PN29_ZONE_LEM_Awake_337 = 1U
}PN29_ZONE_LEM_t;

typedef enum
{
    PN30_ZONE_LEM_Not_awake_338 = 0U,
    PN30_ZONE_LEM_Awake_339 = 1U
}PN30_ZONE_LEM_t;

typedef enum
{
    PN31_ZONE_LEM_Not_awake_340 = 0U,
    PN31_ZONE_LEM_Awake_341 = 1U
}PN31_ZONE_LEM_t;

typedef enum
{
    PN32_ZONE_LEM_Not_awake_342 = 0U,
    PN32_ZONE_LEM_Awake_343 = 1U
}PN32_ZONE_LEM_t;

typedef enum
{
    PN33_ZONE_LEM_Not_awake_344 = 0U,
    PN33_ZONE_LEM_Awake_345 = 1U
}PN33_ZONE_LEM_t;

typedef enum
{
    PN34_ZONE_LEM_Not_awake_346 = 0U,
    PN34_ZONE_LEM_Awake_347 = 1U
}PN34_ZONE_LEM_t;

typedef enum
{
    PN35_ZONE_LEM_Not_awake_348 = 0U,
    PN35_ZONE_LEM_Awake_349 = 1U
}PN35_ZONE_LEM_t;

typedef enum
{
    PN36_ZONE_LEM_Not_awake_350 = 0U,
    PN36_ZONE_LEM_Awake_351 = 1U
}PN36_ZONE_LEM_t;

typedef enum
{
    PN37_ZONE_LEM_Not_awake_352 = 0U,
    PN37_ZONE_LEM_Awake_353 = 1U
}PN37_ZONE_LEM_t;

typedef enum
{
    PN38_ZONE_LEM_Not_awake_354 = 0U,
    PN38_ZONE_LEM_Awake_355 = 1U
}PN38_ZONE_LEM_t;

typedef enum
{
    PN39_ZONE_LEM_Not_awake_356 = 0U,
    PN39_ZONE_LEM_Awake_357 = 1U
}PN39_ZONE_LEM_t;

typedef enum
{
    PN40_ZONE_LEM_Not_awake_358 = 0U,
    PN40_ZONE_LEM_Awake_359 = 1U
}PN40_ZONE_LEM_t;

typedef enum
{
    PN41_ZONE_LEM_Not_awake_360 = 0U,
    PN41_ZONE_LEM_Awake_361 = 1U
}PN41_ZONE_LEM_t;

typedef enum
{
    PN42_ZONE_LEM_Not_awake_362 = 0U,
    PN42_ZONE_LEM_Awake_363 = 1U
}PN42_ZONE_LEM_t;

typedef enum
{
    PN43_ZONE_LEM_Not_awake_364 = 0U,
    PN43_ZONE_LEM_Awake_365 = 1U
}PN43_ZONE_LEM_t;

typedef enum
{
    PN44_ZONE_LEM_Not_awake_366 = 0U,
    PN44_ZONE_LEM_Awake_367 = 1U
}PN44_ZONE_LEM_t;

typedef enum
{
    PN45_ZONE_LEM_Not_awake_368 = 0U,
    PN45_ZONE_LEM_Awake_369 = 1U
}PN45_ZONE_LEM_t;

typedef enum
{
    PN46_ZONE_LEM_Not_awake_370 = 0U,
    PN46_ZONE_LEM_Awake_371 = 1U
}PN46_ZONE_LEM_t;

typedef enum
{
    PN47_ZONE_LEM_Not_awake_372 = 0U,
    PN47_ZONE_LEM_Awake_373 = 1U
}PN47_ZONE_LEM_t;

typedef enum
{
    PN48_ZONE_LEM_Not_awake_374 = 0U,
    PN48_ZONE_LEM_Awake_375 = 1U
}PN48_ZONE_LEM_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    // Source node identifier ZONE_L
    uint8_t ScrNodId_ZONE_LEM : 8U;
    // Control bit vector, Repeat Message Request
    //  0 : "No repeat message request"
    //  1 : "Repeat message request"
    RepMsgReq_ZONE_LEM_t RepMsgReq_ZONE_LEM : 1U;
    // Control bit vector, Network management coordination - Reserved
    uint8_t NetMngtCoorn_ZONE_LEM : 2U;
    // Control bit vector, Network management coordinator sleep ready - Reserved
    uint8_t NetMngtCoorrSleepRdy_ZONE_LEM : 1U;
    // Control bit vector, Active wakeup - Reserved
    uint8_t ActvWakeup_ZONE_LEM : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit1_ZONE_LEM : 1U;
    // Control bit vector, Partial network information - Reserved
    uint8_t PtlNetInfo_ZONE_LEM : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit2_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN1_ZONE_LEM_t PN1_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN2_ZONE_LEM_t PN2_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN3_ZONE_LEM_t PN3_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN4_ZONE_LEM_t PN4_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN5_ZONE_LEM_t PN5_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN6_ZONE_LEM_t PN6_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN7_ZONE_LEM_t PN7_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN8_ZONE_LEM_t PN8_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN9_ZONE_LEM_t PN9_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN10_ZONE_LEM_t PN10_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN11_ZONE_LEM_t PN11_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN12_ZONE_LEM_t PN12_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN13_ZONE_LEM_t PN13_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN14_ZONE_LEM_t PN14_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN15_ZONE_LEM_t PN15_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN16_ZONE_LEM_t PN16_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN17_ZONE_LEM_t PN17_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN18_ZONE_LEM_t PN18_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN19_ZONE_LEM_t PN19_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN20_ZONE_LEM_t PN20_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN21_ZONE_LEM_t PN21_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN22_ZONE_LEM_t PN22_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN23_ZONE_LEM_t PN23_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN24_ZONE_LEM_t PN24_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN25_ZONE_LEM_t PN25_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN26_ZONE_LEM_t PN26_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN27_ZONE_LEM_t PN27_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN28_ZONE_LEM_t PN28_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN29_ZONE_LEM_t PN29_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN30_ZONE_LEM_t PN30_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN31_ZONE_LEM_t PN31_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN32_ZONE_LEM_t PN32_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN33_ZONE_LEM_t PN33_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN34_ZONE_LEM_t PN34_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN35_ZONE_LEM_t PN35_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN36_ZONE_LEM_t PN36_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN37_ZONE_LEM_t PN37_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN38_ZONE_LEM_t PN38_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN39_ZONE_LEM_t PN39_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN40_ZONE_LEM_t PN40_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN41_ZONE_LEM_t PN41_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN42_ZONE_LEM_t PN42_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN43_ZONE_LEM_t PN43_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN44_ZONE_LEM_t PN44_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN45_ZONE_LEM_t PN45_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN46_ZONE_LEM_t PN46_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN47_ZONE_LEM_t PN47_ZONE_LEM : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN48_ZONE_LEM_t PN48_ZONE_LEM : 1U;
  }sig;
} NM_ZONE_LEM_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  NM_ZONE_LEM_un msg;
}NM_ZONE_LEM_msg;

// def @NM_BTA1 CAN Message (1392 0x570)
#define NM_BTA1_IDE (0U)
#define NM_BTA1_DLC (8U)
#define NM_BTA1_CANID (0x570U)

typedef enum
{
    RepMsgReq_BTA1_No_repeat_message_request_376 = 0U,
    RepMsgReq_BTA1_Repeat_message_request_377 = 1U
}RepMsgReq_BTA1_t;

typedef enum
{
    PN1_BTA1_Not_awake_378 = 0U,
    PN1_BTA1_Awake_379 = 1U
}PN1_BTA1_t;

typedef enum
{
    PN2_BTA1_Not_awake_380 = 0U,
    PN2_BTA1_Awake_381 = 1U
}PN2_BTA1_t;

typedef enum
{
    PN3_BTA1_Not_awake_382 = 0U,
    PN3_BTA1_Awake_383 = 1U
}PN3_BTA1_t;

typedef enum
{
    PN4_BTA1_Not_awake_384 = 0U,
    PN4_BTA1_Awake_385 = 1U
}PN4_BTA1_t;

typedef enum
{
    PN5_BTA1_Not_awake_386 = 0U,
    PN5_BTA1_Awake_387 = 1U
}PN5_BTA1_t;

typedef enum
{
    PN6_BTA1_Not_awake_388 = 0U,
    PN6_BTA1_Awake_389 = 1U
}PN6_BTA1_t;

typedef enum
{
    PN7_BTA1_Not_awake_390 = 0U,
    PN7_BTA1_Awake_391 = 1U
}PN7_BTA1_t;

typedef enum
{
    PN8_BTA1_Not_awake_392 = 0U,
    PN8_BTA1_Awake_393 = 1U
}PN8_BTA1_t;

typedef enum
{
    PN9_BTA1_Not_awake_394 = 0U,
    PN9_BTA1_Awake_395 = 1U
}PN9_BTA1_t;

typedef enum
{
    PN10_BTA1_Not_awake_396 = 0U,
    PN10_BTA1_Awake_397 = 1U
}PN10_BTA1_t;

typedef enum
{
    PN11_BTA1_Not_awake_398 = 0U,
    PN11_BTA1_Awake_399 = 1U
}PN11_BTA1_t;

typedef enum
{
    PN12_BTA1_Not_awake_400 = 0U,
    PN12_BTA1_Awake_401 = 1U
}PN12_BTA1_t;

typedef enum
{
    PN13_BTA1_Not_awake_402 = 0U,
    PN13_BTA1_Awake_403 = 1U
}PN13_BTA1_t;

typedef enum
{
    PN14_BTA1_Not_awake_404 = 0U,
    PN14_BTA1_Awake_405 = 1U
}PN14_BTA1_t;

typedef enum
{
    PN15_BTA1_Not_awake_406 = 0U,
    PN15_BTA1_Awake_407 = 1U
}PN15_BTA1_t;

typedef enum
{
    PN16_BTA1_Not_awake_408 = 0U,
    PN16_BTA1_Awake_409 = 1U
}PN16_BTA1_t;

typedef enum
{
    PN17_BTA1_Not_awake_410 = 0U,
    PN17_BTA1_Awake_411 = 1U
}PN17_BTA1_t;

typedef enum
{
    PN18_BTA1_Not_awake_412 = 0U,
    PN18_BTA1_Awake_413 = 1U
}PN18_BTA1_t;

typedef enum
{
    PN19_BTA1_Not_awake_414 = 0U,
    PN19_BTA1_Awake_415 = 1U
}PN19_BTA1_t;

typedef enum
{
    PN20_BTA1_Not_awake_416 = 0U,
    PN20_BTA1_Awake_417 = 1U
}PN20_BTA1_t;

typedef enum
{
    PN21_BTA1_Not_awake_418 = 0U,
    PN21_BTA1_Awake_419 = 1U
}PN21_BTA1_t;

typedef enum
{
    PN22_BTA1_Not_awake_420 = 0U,
    PN22_BTA1_Awake_421 = 1U
}PN22_BTA1_t;

typedef enum
{
    PN23_BTA1_Not_awake_422 = 0U,
    PN23_BTA1_Awake_423 = 1U
}PN23_BTA1_t;

typedef enum
{
    PN24_BTA1_Not_awake_424 = 0U,
    PN24_BTA1_Awake_425 = 1U
}PN24_BTA1_t;

typedef enum
{
    PN25_BTA1_Not_awake_426 = 0U,
    PN25_BTA1_Awake_427 = 1U
}PN25_BTA1_t;

typedef enum
{
    PN26_BTA1_Not_awake_428 = 0U,
    PN26_BTA1_Awake_429 = 1U
}PN26_BTA1_t;

typedef enum
{
    PN27_BTA1_Not_awake_430 = 0U,
    PN27_BTA1_Awake_431 = 1U
}PN27_BTA1_t;

typedef enum
{
    PN28_BTA1_Not_awake_432 = 0U,
    PN28_BTA1_Awake_433 = 1U
}PN28_BTA1_t;

typedef enum
{
    PN29_BTA1_Not_awake_434 = 0U,
    PN29_BTA1_Awake_435 = 1U
}PN29_BTA1_t;

typedef enum
{
    PN30_BTA1_Not_awake_436 = 0U,
    PN30_BTA1_Awake_437 = 1U
}PN30_BTA1_t;

typedef enum
{
    PN31_BTA1_Not_awake_438 = 0U,
    PN31_BTA1_Awake_439 = 1U
}PN31_BTA1_t;

typedef enum
{
    PN32_BTA1_Not_awake_440 = 0U,
    PN32_BTA1_Awake_441 = 1U
}PN32_BTA1_t;

typedef enum
{
    PN33_BTA1_Not_awake_442 = 0U,
    PN33_BTA1_Awake_443 = 1U
}PN33_BTA1_t;

typedef enum
{
    PN34_BTA1_Not_awake_444 = 0U,
    PN34_BTA1_Awake_445 = 1U
}PN34_BTA1_t;

typedef enum
{
    PN35_BTA1_Not_awake_446 = 0U,
    PN35_BTA1_Awake_447 = 1U
}PN35_BTA1_t;

typedef enum
{
    PN36_BTA1_Not_awake_448 = 0U,
    PN36_BTA1_Awake_449 = 1U
}PN36_BTA1_t;

typedef enum
{
    PN37_BTA1_Not_awake_450 = 0U,
    PN37_BTA1_Awake_451 = 1U
}PN37_BTA1_t;

typedef enum
{
    PN38_BTA1_Not_awake_452 = 0U,
    PN38_BTA1_Awake_453 = 1U
}PN38_BTA1_t;

typedef enum
{
    PN39_BTA1_Not_awake_454 = 0U,
    PN39_BTA1_Awake_455 = 1U
}PN39_BTA1_t;

typedef enum
{
    PN40_BTA1_Not_awake_456 = 0U,
    PN40_BTA1_Awake_457 = 1U
}PN40_BTA1_t;

typedef enum
{
    PN41_BTA1_Not_awake_458 = 0U,
    PN41_BTA1_Awake_459 = 1U
}PN41_BTA1_t;

typedef enum
{
    PN42_BTA1_Not_awake_460 = 0U,
    PN42_BTA1_Awake_461 = 1U
}PN42_BTA1_t;

typedef enum
{
    PN43_BTA1_Not_awake_462 = 0U,
    PN43_BTA1_Awake_463 = 1U
}PN43_BTA1_t;

typedef enum
{
    PN44_BTA1_Not_awake_464 = 0U,
    PN44_BTA1_Awake_465 = 1U
}PN44_BTA1_t;

typedef enum
{
    PN45_BTA1_Not_awake_466 = 0U,
    PN45_BTA1_Awake_467 = 1U
}PN45_BTA1_t;

typedef enum
{
    PN46_BTA1_Not_awake_468 = 0U,
    PN46_BTA1_Awake_469 = 1U
}PN46_BTA1_t;

typedef enum
{
    PN47_BTA1_Not_awake_470 = 0U,
    PN47_BTA1_Awake_471 = 1U
}PN47_BTA1_t;

typedef enum
{
    PN48_BTA1_Not_awake_472 = 0U,
    PN48_BTA1_Awake_473 = 1U
}PN48_BTA1_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    // Source node identifier BTA_SRF
    uint8_t ScrNodId_BTA1 : 8U;
    // Control bit vector, Repeat Message Request
    //  0 : "No repeat message request"
    //  1 : "Repeat message request"
    RepMsgReq_BTA1_t RepMsgReq_BTA1 : 1U;
    // Control bit vector, Network management coordination - Reserved
    uint8_t NetMngtCoorn_BTA1 : 2U;
    // Control bit vector, Network management coordinator sleep ready - Reserved
    uint8_t NetMngtCoorrSleepRdy_BTA1 : 1U;
    // Control bit vector, Active wakeup - Reserved
    uint8_t ActvWakeup_BTA1 : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit1_BTA1 : 1U;
    // Control bit vector, Partial network information - Reserved
    uint8_t PtlNetInfo_BTA1 : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit2_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN1_BTA1_t PN1_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN2_BTA1_t PN2_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN3_BTA1_t PN3_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN4_BTA1_t PN4_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN5_BTA1_t PN5_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN6_BTA1_t PN6_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN7_BTA1_t PN7_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN8_BTA1_t PN8_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN9_BTA1_t PN9_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN10_BTA1_t PN10_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN11_BTA1_t PN11_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN12_BTA1_t PN12_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN13_BTA1_t PN13_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN14_BTA1_t PN14_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN15_BTA1_t PN15_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN16_BTA1_t PN16_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN17_BTA1_t PN17_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN18_BTA1_t PN18_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN19_BTA1_t PN19_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN20_BTA1_t PN20_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN21_BTA1_t PN21_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN22_BTA1_t PN22_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN23_BTA1_t PN23_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN24_BTA1_t PN24_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN25_BTA1_t PN25_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN26_BTA1_t PN26_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN27_BTA1_t PN27_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN28_BTA1_t PN28_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN29_BTA1_t PN29_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN30_BTA1_t PN30_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN31_BTA1_t PN31_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN32_BTA1_t PN32_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN33_BTA1_t PN33_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN34_BTA1_t PN34_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN35_BTA1_t PN35_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN36_BTA1_t PN36_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN37_BTA1_t PN37_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN38_BTA1_t PN38_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN39_BTA1_t PN39_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN40_BTA1_t PN40_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN41_BTA1_t PN41_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN42_BTA1_t PN42_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN43_BTA1_t PN43_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN44_BTA1_t PN44_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN45_BTA1_t PN45_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN46_BTA1_t PN46_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN47_BTA1_t PN47_BTA1 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN48_BTA1_t PN48_BTA1 : 1U;
  }sig;
} NM_BTA1_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  NM_BTA1_un msg;
}NM_BTA1_msg;

// def @NM_BTA2 CAN Message (1393 0x571)
#define NM_BTA2_IDE (0U)
#define NM_BTA2_DLC (8U)
#define NM_BTA2_CANID (0x571U)

typedef enum
{
    RepMsgReq_BTA2_No_repeat_message_request_474 = 0U,
    RepMsgReq_BTA2_Repeat_message_request_475 = 1U
}RepMsgReq_BTA2_t;

typedef enum
{
    PN1_BTA2_Not_awake_476 = 0U,
    PN1_BTA2_Awake_477 = 1U
}PN1_BTA2_t;

typedef enum
{
    PN2_BTA2_Not_awake_478 = 0U,
    PN2_BTA2_Awake_479 = 1U
}PN2_BTA2_t;

typedef enum
{
    PN3_BTA2_Not_awake_480 = 0U,
    PN3_BTA2_Awake_481 = 1U
}PN3_BTA2_t;

typedef enum
{
    PN4_BTA2_Not_awake_482 = 0U,
    PN4_BTA2_Awake_483 = 1U
}PN4_BTA2_t;

typedef enum
{
    PN5_BTA2_Not_awake_484 = 0U,
    PN5_BTA2_Awake_485 = 1U
}PN5_BTA2_t;

typedef enum
{
    PN6_BTA2_Not_awake_486 = 0U,
    PN6_BTA2_Awake_487 = 1U
}PN6_BTA2_t;

typedef enum
{
    PN7_BTA2_Not_awake_488 = 0U,
    PN7_BTA2_Awake_489 = 1U
}PN7_BTA2_t;

typedef enum
{
    PN8_BTA2_Not_awake_490 = 0U,
    PN8_BTA2_Awake_491 = 1U
}PN8_BTA2_t;

typedef enum
{
    PN9_BTA2_Not_awake_492 = 0U,
    PN9_BTA2_Awake_493 = 1U
}PN9_BTA2_t;

typedef enum
{
    PN10_BTA2_Not_awake_494 = 0U,
    PN10_BTA2_Awake_495 = 1U
}PN10_BTA2_t;

typedef enum
{
    PN11_BTA2_Not_awake_496 = 0U,
    PN11_BTA2_Awake_497 = 1U
}PN11_BTA2_t;

typedef enum
{
    PN12_BTA2_Not_awake_498 = 0U,
    PN12_BTA2_Awake_499 = 1U
}PN12_BTA2_t;

typedef enum
{
    PN13_BTA2_Not_awake_500 = 0U,
    PN13_BTA2_Awake_501 = 1U
}PN13_BTA2_t;

typedef enum
{
    PN14_BTA2_Not_awake_502 = 0U,
    PN14_BTA2_Awake_503 = 1U
}PN14_BTA2_t;

typedef enum
{
    PN15_BTA2_Not_awake_504 = 0U,
    PN15_BTA2_Awake_505 = 1U
}PN15_BTA2_t;

typedef enum
{
    PN16_BTA2_Not_awake_506 = 0U,
    PN16_BTA2_Awake_507 = 1U
}PN16_BTA2_t;

typedef enum
{
    PN17_BTA2_Not_awake_508 = 0U,
    PN17_BTA2_Awake_509 = 1U
}PN17_BTA2_t;

typedef enum
{
    PN18_BTA2_Not_awake_510 = 0U,
    PN18_BTA2_Awake_511 = 1U
}PN18_BTA2_t;

typedef enum
{
    PN19_BTA2_Not_awake_512 = 0U,
    PN19_BTA2_Awake_513 = 1U
}PN19_BTA2_t;

typedef enum
{
    PN20_BTA2_Not_awake_514 = 0U,
    PN20_BTA2_Awake_515 = 1U
}PN20_BTA2_t;

typedef enum
{
    PN21_BTA2_Not_awake_516 = 0U,
    PN21_BTA2_Awake_517 = 1U
}PN21_BTA2_t;

typedef enum
{
    PN22_BTA2_Not_awake_518 = 0U,
    PN22_BTA2_Awake_519 = 1U
}PN22_BTA2_t;

typedef enum
{
    PN23_BTA2_Not_awake_520 = 0U,
    PN23_BTA2_Awake_521 = 1U
}PN23_BTA2_t;

typedef enum
{
    PN24_BTA2_Not_awake_522 = 0U,
    PN24_BTA2_Awake_523 = 1U
}PN24_BTA2_t;

typedef enum
{
    PN25_BTA2_Not_awake_524 = 0U,
    PN25_BTA2_Awake_525 = 1U
}PN25_BTA2_t;

typedef enum
{
    PN26_BTA2_Not_awake_526 = 0U,
    PN26_BTA2_Awake_527 = 1U
}PN26_BTA2_t;

typedef enum
{
    PN27_BTA2_Not_awake_528 = 0U,
    PN27_BTA2_Awake_529 = 1U
}PN27_BTA2_t;

typedef enum
{
    PN28_BTA2_Not_awake_530 = 0U,
    PN28_BTA2_Awake_531 = 1U
}PN28_BTA2_t;

typedef enum
{
    PN29_BTA2_Not_awake_532 = 0U,
    PN29_BTA2_Awake_533 = 1U
}PN29_BTA2_t;

typedef enum
{
    PN30_BTA2_Not_awake_534 = 0U,
    PN30_BTA2_Awake_535 = 1U
}PN30_BTA2_t;

typedef enum
{
    PN31_BTA2_Not_awake_536 = 0U,
    PN31_BTA2_Awake_537 = 1U
}PN31_BTA2_t;

typedef enum
{
    PN32_BTA2_Not_awake_538 = 0U,
    PN32_BTA2_Awake_539 = 1U
}PN32_BTA2_t;

typedef enum
{
    PN33_BTA2_Not_awake_540 = 0U,
    PN33_BTA2_Awake_541 = 1U
}PN33_BTA2_t;

typedef enum
{
    PN34_BTA2_Not_awake_542 = 0U,
    PN34_BTA2_Awake_543 = 1U
}PN34_BTA2_t;

typedef enum
{
    PN35_BTA2_Not_awake_544 = 0U,
    PN35_BTA2_Awake_545 = 1U
}PN35_BTA2_t;

typedef enum
{
    PN36_BTA2_Not_awake_546 = 0U,
    PN36_BTA2_Awake_547 = 1U
}PN36_BTA2_t;

typedef enum
{
    PN37_BTA2_Not_awake_548 = 0U,
    PN37_BTA2_Awake_549 = 1U
}PN37_BTA2_t;

typedef enum
{
    PN38_BTA2_Not_awake_550 = 0U,
    PN38_BTA2_Awake_551 = 1U
}PN38_BTA2_t;

typedef enum
{
    PN39_BTA2_Not_awake_552 = 0U,
    PN39_BTA2_Awake_553 = 1U
}PN39_BTA2_t;

typedef enum
{
    PN40_BTA2_Not_awake_554 = 0U,
    PN40_BTA2_Awake_555 = 1U
}PN40_BTA2_t;

typedef enum
{
    PN41_BTA2_Not_awake_556 = 0U,
    PN41_BTA2_Awake_557 = 1U
}PN41_BTA2_t;

typedef enum
{
    PN42_BTA2_Not_awake_558 = 0U,
    PN42_BTA2_Awake_559 = 1U
}PN42_BTA2_t;

typedef enum
{
    PN43_BTA2_Not_awake_560 = 0U,
    PN43_BTA2_Awake_561 = 1U
}PN43_BTA2_t;

typedef enum
{
    PN44_BTA2_Not_awake_562 = 0U,
    PN44_BTA2_Awake_563 = 1U
}PN44_BTA2_t;

typedef enum
{
    PN45_BTA2_Not_awake_564 = 0U,
    PN45_BTA2_Awake_565 = 1U
}PN45_BTA2_t;

typedef enum
{
    PN46_BTA2_Not_awake_566 = 0U,
    PN46_BTA2_Awake_567 = 1U
}PN46_BTA2_t;

typedef enum
{
    PN47_BTA2_Not_awake_568 = 0U,
    PN47_BTA2_Awake_569 = 1U
}PN47_BTA2_t;

typedef enum
{
    PN48_BTA2_Not_awake_570 = 0U,
    PN48_BTA2_Awake_571 = 1U
}PN48_BTA2_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    // Source node identifier BTA_L
    uint8_t ScrNodId_BTA2 : 8U;
    // Control bit vector, Repeat Message Request
    //  0 : "No repeat message request"
    //  1 : "Repeat message request"
    RepMsgReq_BTA2_t RepMsgReq_BTA2 : 1U;
    // Control bit vector, Network management coordination - Reserved
    uint8_t NetMngtCoorn_BTA2 : 2U;
    // Control bit vector, Network management coordinator sleep ready - Reserved
    uint8_t NetMngtCoorrSleepRdy_BTA2 : 1U;
    // Control bit vector, Active wakeup - Reserved
    uint8_t ActvWakeup_BTA2 : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit1_BTA2 : 1U;
    // Control bit vector, Partial network information - Reserved
    uint8_t PtlNetInfo_BTA2 : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit2_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN1_BTA2_t PN1_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN2_BTA2_t PN2_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN3_BTA2_t PN3_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN4_BTA2_t PN4_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN5_BTA2_t PN5_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN6_BTA2_t PN6_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN7_BTA2_t PN7_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN8_BTA2_t PN8_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN9_BTA2_t PN9_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN10_BTA2_t PN10_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN11_BTA2_t PN11_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN12_BTA2_t PN12_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN13_BTA2_t PN13_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN14_BTA2_t PN14_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN15_BTA2_t PN15_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN16_BTA2_t PN16_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN17_BTA2_t PN17_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN18_BTA2_t PN18_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN19_BTA2_t PN19_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN20_BTA2_t PN20_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN21_BTA2_t PN21_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN22_BTA2_t PN22_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN23_BTA2_t PN23_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN24_BTA2_t PN24_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN25_BTA2_t PN25_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN26_BTA2_t PN26_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN27_BTA2_t PN27_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN28_BTA2_t PN28_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN29_BTA2_t PN29_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN30_BTA2_t PN30_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN31_BTA2_t PN31_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN32_BTA2_t PN32_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN33_BTA2_t PN33_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN34_BTA2_t PN34_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN35_BTA2_t PN35_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN36_BTA2_t PN36_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN37_BTA2_t PN37_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN38_BTA2_t PN38_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN39_BTA2_t PN39_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN40_BTA2_t PN40_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN41_BTA2_t PN41_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN42_BTA2_t PN42_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN43_BTA2_t PN43_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN44_BTA2_t PN44_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN45_BTA2_t PN45_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN46_BTA2_t PN46_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN47_BTA2_t PN47_BTA2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN48_BTA2_t PN48_BTA2 : 1U;
  }sig;
} NM_BTA2_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  NM_BTA2_un msg;
}NM_BTA2_msg;

// def @NM_BTA3 CAN Message (1394 0x572)
#define NM_BTA3_IDE (0U)
#define NM_BTA3_DLC (8U)
#define NM_BTA3_CANID (0x572U)

typedef enum
{
    RepMsgReq_BTA3_No_repeat_message_request_572 = 0U,
    RepMsgReq_BTA3_Repeat_message_request_573 = 1U
}RepMsgReq_BTA3_t;

typedef enum
{
    PN1_BTA3_Not_awake_574 = 0U,
    PN1_BTA3_Awake_575 = 1U
}PN1_BTA3_t;

typedef enum
{
    PN2_BTA3_Not_awake_576 = 0U,
    PN2_BTA3_Awake_577 = 1U
}PN2_BTA3_t;

typedef enum
{
    PN3_BTA3_Not_awake_578 = 0U,
    PN3_BTA3_Awake_579 = 1U
}PN3_BTA3_t;

typedef enum
{
    PN4_BTA3_Not_awake_580 = 0U,
    PN4_BTA3_Awake_581 = 1U
}PN4_BTA3_t;

typedef enum
{
    PN5_BTA3_Not_awake_582 = 0U,
    PN5_BTA3_Awake_583 = 1U
}PN5_BTA3_t;

typedef enum
{
    PN6_BTA3_Not_awake_584 = 0U,
    PN6_BTA3_Awake_585 = 1U
}PN6_BTA3_t;

typedef enum
{
    PN7_BTA3_Not_awake_586 = 0U,
    PN7_BTA3_Awake_587 = 1U
}PN7_BTA3_t;

typedef enum
{
    PN8_BTA3_Not_awake_588 = 0U,
    PN8_BTA3_Awake_589 = 1U
}PN8_BTA3_t;

typedef enum
{
    PN9_BTA3_Not_awake_590 = 0U,
    PN9_BTA3_Awake_591 = 1U
}PN9_BTA3_t;

typedef enum
{
    PN10_BTA3_Not_awake_592 = 0U,
    PN10_BTA3_Awake_593 = 1U
}PN10_BTA3_t;

typedef enum
{
    PN11_BTA3_Not_awake_594 = 0U,
    PN11_BTA3_Awake_595 = 1U
}PN11_BTA3_t;

typedef enum
{
    PN12_BTA3_Not_awake_596 = 0U,
    PN12_BTA3_Awake_597 = 1U
}PN12_BTA3_t;

typedef enum
{
    PN13_BTA3_Not_awake_598 = 0U,
    PN13_BTA3_Awake_599 = 1U
}PN13_BTA3_t;

typedef enum
{
    PN14_BTA3_Not_awake_600 = 0U,
    PN14_BTA3_Awake_601 = 1U
}PN14_BTA3_t;

typedef enum
{
    PN15_BTA3_Not_awake_602 = 0U,
    PN15_BTA3_Awake_603 = 1U
}PN15_BTA3_t;

typedef enum
{
    PN16_BTA3_Not_awake_604 = 0U,
    PN16_BTA3_Awake_605 = 1U
}PN16_BTA3_t;

typedef enum
{
    PN17_BTA3_Not_awake_606 = 0U,
    PN17_BTA3_Awake_607 = 1U
}PN17_BTA3_t;

typedef enum
{
    PN18_BTA3_Not_awake_608 = 0U,
    PN18_BTA3_Awake_609 = 1U
}PN18_BTA3_t;

typedef enum
{
    PN19_BTA3_Not_awake_610 = 0U,
    PN19_BTA3_Awake_611 = 1U
}PN19_BTA3_t;

typedef enum
{
    PN20_BTA3_Not_awake_612 = 0U,
    PN20_BTA3_Awake_613 = 1U
}PN20_BTA3_t;

typedef enum
{
    PN21_BTA3_Not_awake_614 = 0U,
    PN21_BTA3_Awake_615 = 1U
}PN21_BTA3_t;

typedef enum
{
    PN22_BTA3_Not_awake_616 = 0U,
    PN22_BTA3_Awake_617 = 1U
}PN22_BTA3_t;

typedef enum
{
    PN23_BTA3_Not_awake_618 = 0U,
    PN23_BTA3_Awake_619 = 1U
}PN23_BTA3_t;

typedef enum
{
    PN24_BTA3_Not_awake_620 = 0U,
    PN24_BTA3_Awake_621 = 1U
}PN24_BTA3_t;

typedef enum
{
    PN25_BTA3_Not_awake_622 = 0U,
    PN25_BTA3_Awake_623 = 1U
}PN25_BTA3_t;

typedef enum
{
    PN26_BTA3_Not_awake_624 = 0U,
    PN26_BTA3_Awake_625 = 1U
}PN26_BTA3_t;

typedef enum
{
    PN27_BTA3_Not_awake_626 = 0U,
    PN27_BTA3_Awake_627 = 1U
}PN27_BTA3_t;

typedef enum
{
    PN28_BTA3_Not_awake_628 = 0U,
    PN28_BTA3_Awake_629 = 1U
}PN28_BTA3_t;

typedef enum
{
    PN29_BTA3_Not_awake_630 = 0U,
    PN29_BTA3_Awake_631 = 1U
}PN29_BTA3_t;

typedef enum
{
    PN30_BTA3_Not_awake_632 = 0U,
    PN30_BTA3_Awake_633 = 1U
}PN30_BTA3_t;

typedef enum
{
    PN31_BTA3_Not_awake_634 = 0U,
    PN31_BTA3_Awake_635 = 1U
}PN31_BTA3_t;

typedef enum
{
    PN32_BTA3_Not_awake_636 = 0U,
    PN32_BTA3_Awake_637 = 1U
}PN32_BTA3_t;

typedef enum
{
    PN33_BTA3_Not_awake_638 = 0U,
    PN33_BTA3_Awake_639 = 1U
}PN33_BTA3_t;

typedef enum
{
    PN34_BTA3_Not_awake_640 = 0U,
    PN34_BTA3_Awake_641 = 1U
}PN34_BTA3_t;

typedef enum
{
    PN35_BTA3_Not_awake_642 = 0U,
    PN35_BTA3_Awake_643 = 1U
}PN35_BTA3_t;

typedef enum
{
    PN36_BTA3_Not_awake_644 = 0U,
    PN36_BTA3_Awake_645 = 1U
}PN36_BTA3_t;

typedef enum
{
    PN37_BTA3_Not_awake_646 = 0U,
    PN37_BTA3_Awake_647 = 1U
}PN37_BTA3_t;

typedef enum
{
    PN38_BTA3_Not_awake_648 = 0U,
    PN38_BTA3_Awake_649 = 1U
}PN38_BTA3_t;

typedef enum
{
    PN39_BTA3_Not_awake_650 = 0U,
    PN39_BTA3_Awake_651 = 1U
}PN39_BTA3_t;

typedef enum
{
    PN40_BTA3_Not_awake_652 = 0U,
    PN40_BTA3_Awake_653 = 1U
}PN40_BTA3_t;

typedef enum
{
    PN41_BTA3_Not_awake_654 = 0U,
    PN41_BTA3_Awake_655 = 1U
}PN41_BTA3_t;

typedef enum
{
    PN42_BTA3_Not_awake_656 = 0U,
    PN42_BTA3_Awake_657 = 1U
}PN42_BTA3_t;

typedef enum
{
    PN43_BTA3_Not_awake_658 = 0U,
    PN43_BTA3_Awake_659 = 1U
}PN43_BTA3_t;

typedef enum
{
    PN44_BTA3_Not_awake_660 = 0U,
    PN44_BTA3_Awake_661 = 1U
}PN44_BTA3_t;

typedef enum
{
    PN45_BTA3_Not_awake_662 = 0U,
    PN45_BTA3_Awake_663 = 1U
}PN45_BTA3_t;

typedef enum
{
    PN46_BTA3_Not_awake_664 = 0U,
    PN46_BTA3_Awake_665 = 1U
}PN46_BTA3_t;

typedef enum
{
    PN47_BTA3_Not_awake_666 = 0U,
    PN47_BTA3_Awake_667 = 1U
}PN47_BTA3_t;

typedef enum
{
    PN48_BTA3_Not_awake_668 = 0U,
    PN48_BTA3_Awake_669 = 1U
}PN48_BTA3_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    // Source node identifier BTA_R
    uint8_t ScrNodId_BTA3 : 8U;
    // Control bit vector, Repeat Message Request
    //  0 : "No repeat message request"
    //  1 : "Repeat message request"
    RepMsgReq_BTA3_t RepMsgReq_BTA3 : 1U;
    // Control bit vector, Network management coordination - Reserved
    uint8_t NetMngtCoorn_BTA3 : 2U;
    // Control bit vector, Network management coordinator sleep ready - Reserved
    uint8_t NetMngtCoorrSleepRdy_BTA3 : 1U;
    // Control bit vector, Active wakeup - Reserved
    uint8_t ActvWakeup_BTA3 : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit1_BTA3 : 1U;
    // Control bit vector, Partial network information - Reserved
    uint8_t PtlNetInfo_BTA3 : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit2_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN1_BTA3_t PN1_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN2_BTA3_t PN2_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN3_BTA3_t PN3_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN4_BTA3_t PN4_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN5_BTA3_t PN5_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN6_BTA3_t PN6_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN7_BTA3_t PN7_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN8_BTA3_t PN8_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN9_BTA3_t PN9_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN10_BTA3_t PN10_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN11_BTA3_t PN11_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN12_BTA3_t PN12_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN13_BTA3_t PN13_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN14_BTA3_t PN14_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN15_BTA3_t PN15_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN16_BTA3_t PN16_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN17_BTA3_t PN17_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN18_BTA3_t PN18_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN19_BTA3_t PN19_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN20_BTA3_t PN20_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN21_BTA3_t PN21_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN22_BTA3_t PN22_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN23_BTA3_t PN23_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN24_BTA3_t PN24_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN25_BTA3_t PN25_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN26_BTA3_t PN26_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN27_BTA3_t PN27_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN28_BTA3_t PN28_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN29_BTA3_t PN29_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN30_BTA3_t PN30_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN31_BTA3_t PN31_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN32_BTA3_t PN32_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN33_BTA3_t PN33_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN34_BTA3_t PN34_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN35_BTA3_t PN35_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN36_BTA3_t PN36_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN37_BTA3_t PN37_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN38_BTA3_t PN38_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN39_BTA3_t PN39_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN40_BTA3_t PN40_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN41_BTA3_t PN41_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN42_BTA3_t PN42_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN43_BTA3_t PN43_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN44_BTA3_t PN44_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN45_BTA3_t PN45_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN46_BTA3_t PN46_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN47_BTA3_t PN47_BTA3 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN48_BTA3_t PN48_BTA3 : 1U;
  }sig;
} NM_BTA3_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  NM_BTA3_un msg;
}NM_BTA3_msg;

// def @NM_BTA4 CAN Message (1395 0x573)
#define NM_BTA4_IDE (0U)
#define NM_BTA4_DLC (8U)
#define NM_BTA4_CANID (0x573U)

typedef enum
{
    RepMsgReq_BTA4_No_repeat_message_request_670 = 0U,
    RepMsgReq_BTA4_Repeat_message_request_671 = 1U
}RepMsgReq_BTA4_t;

typedef enum
{
    PN1_BTA4_Not_awake_672 = 0U,
    PN1_BTA4_Awake_673 = 1U
}PN1_BTA4_t;

typedef enum
{
    PN2_BTA4_Not_awake_674 = 0U,
    PN2_BTA4_Awake_675 = 1U
}PN2_BTA4_t;

typedef enum
{
    PN3_BTA4_Not_awake_676 = 0U,
    PN3_BTA4_Awake_677 = 1U
}PN3_BTA4_t;

typedef enum
{
    PN4_BTA4_Not_awake_678 = 0U,
    PN4_BTA4_Awake_679 = 1U
}PN4_BTA4_t;

typedef enum
{
    PN5_BTA4_Not_awake_680 = 0U,
    PN5_BTA4_Awake_681 = 1U
}PN5_BTA4_t;

typedef enum
{
    PN6_BTA4_Not_awake_682 = 0U,
    PN6_BTA4_Awake_683 = 1U
}PN6_BTA4_t;

typedef enum
{
    PN7_BTA4_Not_awake_684 = 0U,
    PN7_BTA4_Awake_685 = 1U
}PN7_BTA4_t;

typedef enum
{
    PN8_BTA4_Not_awake_686 = 0U,
    PN8_BTA4_Awake_687 = 1U
}PN8_BTA4_t;

typedef enum
{
    PN9_BTA4_Not_awake_688 = 0U,
    PN9_BTA4_Awake_689 = 1U
}PN9_BTA4_t;

typedef enum
{
    PN10_BTA4_Not_awake_690 = 0U,
    PN10_BTA4_Awake_691 = 1U
}PN10_BTA4_t;

typedef enum
{
    PN11_BTA4_Not_awake_692 = 0U,
    PN11_BTA4_Awake_693 = 1U
}PN11_BTA4_t;

typedef enum
{
    PN12_BTA4_Not_awake_694 = 0U,
    PN12_BTA4_Awake_695 = 1U
}PN12_BTA4_t;

typedef enum
{
    PN13_BTA4_Not_awake_696 = 0U,
    PN13_BTA4_Awake_697 = 1U
}PN13_BTA4_t;

typedef enum
{
    PN14_BTA4_Not_awake_698 = 0U,
    PN14_BTA4_Awake_699 = 1U
}PN14_BTA4_t;

typedef enum
{
    PN15_BTA4_Not_awake_700 = 0U,
    PN15_BTA4_Awake_701 = 1U
}PN15_BTA4_t;

typedef enum
{
    PN16_BTA4_Not_awake_702 = 0U,
    PN16_BTA4_Awake_703 = 1U
}PN16_BTA4_t;

typedef enum
{
    PN17_BTA4_Not_awake_704 = 0U,
    PN17_BTA4_Awake_705 = 1U
}PN17_BTA4_t;

typedef enum
{
    PN18_BTA4_Not_awake_706 = 0U,
    PN18_BTA4_Awake_707 = 1U
}PN18_BTA4_t;

typedef enum
{
    PN19_BTA4_Not_awake_708 = 0U,
    PN19_BTA4_Awake_709 = 1U
}PN19_BTA4_t;

typedef enum
{
    PN20_BTA4_Not_awake_710 = 0U,
    PN20_BTA4_Awake_711 = 1U
}PN20_BTA4_t;

typedef enum
{
    PN21_BTA4_Not_awake_712 = 0U,
    PN21_BTA4_Awake_713 = 1U
}PN21_BTA4_t;

typedef enum
{
    PN22_BTA4_Not_awake_714 = 0U,
    PN22_BTA4_Awake_715 = 1U
}PN22_BTA4_t;

typedef enum
{
    PN23_BTA4_Not_awake_716 = 0U,
    PN23_BTA4_Awake_717 = 1U
}PN23_BTA4_t;

typedef enum
{
    PN24_BTA4_Not_awake_718 = 0U,
    PN24_BTA4_Awake_719 = 1U
}PN24_BTA4_t;

typedef enum
{
    PN25_BTA4_Not_awake_720 = 0U,
    PN25_BTA4_Awake_721 = 1U
}PN25_BTA4_t;

typedef enum
{
    PN26_BTA4_Not_awake_722 = 0U,
    PN26_BTA4_Awake_723 = 1U
}PN26_BTA4_t;

typedef enum
{
    PN27_BTA4_Not_awake_724 = 0U,
    PN27_BTA4_Awake_725 = 1U
}PN27_BTA4_t;

typedef enum
{
    PN28_BTA4_Not_awake_726 = 0U,
    PN28_BTA4_Awake_727 = 1U
}PN28_BTA4_t;

typedef enum
{
    PN29_BTA4_Not_awake_728 = 0U,
    PN29_BTA4_Awake_729 = 1U
}PN29_BTA4_t;

typedef enum
{
    PN30_BTA4_Not_awake_730 = 0U,
    PN30_BTA4_Awake_731 = 1U
}PN30_BTA4_t;

typedef enum
{
    PN31_BTA4_Not_awake_732 = 0U,
    PN31_BTA4_Awake_733 = 1U
}PN31_BTA4_t;

typedef enum
{
    PN32_BTA4_Not_awake_734 = 0U,
    PN32_BTA4_Awake_735 = 1U
}PN32_BTA4_t;

typedef enum
{
    PN33_BTA4_Not_awake_736 = 0U,
    PN33_BTA4_Awake_737 = 1U
}PN33_BTA4_t;

typedef enum
{
    PN34_BTA4_Not_awake_738 = 0U,
    PN34_BTA4_Awake_739 = 1U
}PN34_BTA4_t;

typedef enum
{
    PN35_BTA4_Not_awake_740 = 0U,
    PN35_BTA4_Awake_741 = 1U
}PN35_BTA4_t;

typedef enum
{
    PN36_BTA4_Not_awake_742 = 0U,
    PN36_BTA4_Awake_743 = 1U
}PN36_BTA4_t;

typedef enum
{
    PN37_BTA4_Not_awake_744 = 0U,
    PN37_BTA4_Awake_745 = 1U
}PN37_BTA4_t;

typedef enum
{
    PN38_BTA4_Not_awake_746 = 0U,
    PN38_BTA4_Awake_747 = 1U
}PN38_BTA4_t;

typedef enum
{
    PN39_BTA4_Not_awake_748 = 0U,
    PN39_BTA4_Awake_749 = 1U
}PN39_BTA4_t;

typedef enum
{
    PN40_BTA4_Not_awake_750 = 0U,
    PN40_BTA4_Awake_751 = 1U
}PN40_BTA4_t;

typedef enum
{
    PN41_BTA4_Not_awake_752 = 0U,
    PN41_BTA4_Awake_753 = 1U
}PN41_BTA4_t;

typedef enum
{
    PN42_BTA4_Not_awake_754 = 0U,
    PN42_BTA4_Awake_755 = 1U
}PN42_BTA4_t;

typedef enum
{
    PN43_BTA4_Not_awake_756 = 0U,
    PN43_BTA4_Awake_757 = 1U
}PN43_BTA4_t;

typedef enum
{
    PN44_BTA4_Not_awake_758 = 0U,
    PN44_BTA4_Awake_759 = 1U
}PN44_BTA4_t;

typedef enum
{
    PN45_BTA4_Not_awake_760 = 0U,
    PN45_BTA4_Awake_761 = 1U
}PN45_BTA4_t;

typedef enum
{
    PN46_BTA4_Not_awake_762 = 0U,
    PN46_BTA4_Awake_763 = 1U
}PN46_BTA4_t;

typedef enum
{
    PN47_BTA4_Not_awake_764 = 0U,
    PN47_BTA4_Awake_765 = 1U
}PN47_BTA4_t;

typedef enum
{
    PN48_BTA4_Not_awake_766 = 0U,
    PN48_BTA4_Awake_767 = 1U
}PN48_BTA4_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    // Source node identifier BTA_SRR
    uint8_t ScrNodId_BTA4 : 8U;
    // Control bit vector, Repeat Message Request
    //  0 : "No repeat message request"
    //  1 : "Repeat message request"
    RepMsgReq_BTA4_t RepMsgReq_BTA4 : 1U;
    // Control bit vector, Network management coordination - Reserved
    uint8_t NetMngtCoorn_BTA4 : 2U;
    // Control bit vector, Network management coordinator sleep ready - Reserved
    uint8_t NetMngtCoorrSleepRdy_BTA4 : 1U;
    // Control bit vector, Active wakeup - Reserved
    uint8_t ActvWakeup_BTA4 : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit1_BTA4 : 1U;
    // Control bit vector, Partial network information - Reserved
    uint8_t PtlNetInfo_BTA4 : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit2_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN1_BTA4_t PN1_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN2_BTA4_t PN2_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN3_BTA4_t PN3_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN4_BTA4_t PN4_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN5_BTA4_t PN5_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN6_BTA4_t PN6_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN7_BTA4_t PN7_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN8_BTA4_t PN8_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN9_BTA4_t PN9_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN10_BTA4_t PN10_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN11_BTA4_t PN11_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN12_BTA4_t PN12_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN13_BTA4_t PN13_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN14_BTA4_t PN14_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN15_BTA4_t PN15_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN16_BTA4_t PN16_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN17_BTA4_t PN17_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN18_BTA4_t PN18_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN19_BTA4_t PN19_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN20_BTA4_t PN20_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN21_BTA4_t PN21_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN22_BTA4_t PN22_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN23_BTA4_t PN23_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN24_BTA4_t PN24_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN25_BTA4_t PN25_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN26_BTA4_t PN26_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN27_BTA4_t PN27_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN28_BTA4_t PN28_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN29_BTA4_t PN29_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN30_BTA4_t PN30_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN31_BTA4_t PN31_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN32_BTA4_t PN32_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN33_BTA4_t PN33_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN34_BTA4_t PN34_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN35_BTA4_t PN35_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN36_BTA4_t PN36_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN37_BTA4_t PN37_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN38_BTA4_t PN38_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN39_BTA4_t PN39_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN40_BTA4_t PN40_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN41_BTA4_t PN41_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN42_BTA4_t PN42_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN43_BTA4_t PN43_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN44_BTA4_t PN44_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN45_BTA4_t PN45_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN46_BTA4_t PN46_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN47_BTA4_t PN47_BTA4 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN48_BTA4_t PN48_BTA4 : 1U;
  }sig;
} NM_BTA4_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  NM_BTA4_un msg;
}NM_BTA4_msg;

// def @NM_BTA5 CAN Message (1396 0x574)
#define NM_BTA5_IDE (0U)
#define NM_BTA5_DLC (8U)
#define NM_BTA5_CANID (0x574U)

typedef enum
{
    RepMsgReq_BTA5_No_repeat_message_request_768 = 0U,
    RepMsgReq_BTA5_Repeat_message_request_769 = 1U
}RepMsgReq_BTA5_t;

typedef enum
{
    PN1_BTA5_Not_awake_770 = 0U,
    PN1_BTA5_Awake_771 = 1U
}PN1_BTA5_t;

typedef enum
{
    PN2_BTA5_Not_awake_772 = 0U,
    PN2_BTA5_Awake_773 = 1U
}PN2_BTA5_t;

typedef enum
{
    PN3_BTA5_Not_awake_774 = 0U,
    PN3_BTA5_Awake_775 = 1U
}PN3_BTA5_t;

typedef enum
{
    PN4_BTA5_Not_awake_776 = 0U,
    PN4_BTA5_Awake_777 = 1U
}PN4_BTA5_t;

typedef enum
{
    PN5_BTA5_Not_awake_778 = 0U,
    PN5_BTA5_Awake_779 = 1U
}PN5_BTA5_t;

typedef enum
{
    PN6_BTA5_Not_awake_780 = 0U,
    PN6_BTA5_Awake_781 = 1U
}PN6_BTA5_t;

typedef enum
{
    PN7_BTA5_Not_awake_782 = 0U,
    PN7_BTA5_Awake_783 = 1U
}PN7_BTA5_t;

typedef enum
{
    PN8_BTA5_Not_awake_784 = 0U,
    PN8_BTA5_Awake_785 = 1U
}PN8_BTA5_t;

typedef enum
{
    PN9_BTA5_Not_awake_786 = 0U,
    PN9_BTA5_Awake_787 = 1U
}PN9_BTA5_t;

typedef enum
{
    PN10_BTA5_Not_awake_788 = 0U,
    PN10_BTA5_Awake_789 = 1U
}PN10_BTA5_t;

typedef enum
{
    PN11_BTA5_Not_awake_790 = 0U,
    PN11_BTA5_Awake_791 = 1U
}PN11_BTA5_t;

typedef enum
{
    PN12_BTA5_Not_awake_792 = 0U,
    PN12_BTA5_Awake_793 = 1U
}PN12_BTA5_t;

typedef enum
{
    PN13_BTA5_Not_awake_794 = 0U,
    PN13_BTA5_Awake_795 = 1U
}PN13_BTA5_t;

typedef enum
{
    PN14_BTA5_Not_awake_796 = 0U,
    PN14_BTA5_Awake_797 = 1U
}PN14_BTA5_t;

typedef enum
{
    PN15_BTA5_Not_awake_798 = 0U,
    PN15_BTA5_Awake_799 = 1U
}PN15_BTA5_t;

typedef enum
{
    PN16_BTA5_Not_awake_800 = 0U,
    PN16_BTA5_Awake_801 = 1U
}PN16_BTA5_t;

typedef enum
{
    PN17_BTA5_Not_awake_802 = 0U,
    PN17_BTA5_Awake_803 = 1U
}PN17_BTA5_t;

typedef enum
{
    PN18_BTA5_Not_awake_804 = 0U,
    PN18_BTA5_Awake_805 = 1U
}PN18_BTA5_t;

typedef enum
{
    PN19_BTA5_Not_awake_806 = 0U,
    PN19_BTA5_Awake_807 = 1U
}PN19_BTA5_t;

typedef enum
{
    PN20_BTA5_Not_awake_808 = 0U,
    PN20_BTA5_Awake_809 = 1U
}PN20_BTA5_t;

typedef enum
{
    PN21_BTA5_Not_awake_810 = 0U,
    PN21_BTA5_Awake_811 = 1U
}PN21_BTA5_t;

typedef enum
{
    PN22_BTA5_Not_awake_812 = 0U,
    PN22_BTA5_Awake_813 = 1U
}PN22_BTA5_t;

typedef enum
{
    PN23_BTA5_Not_awake_814 = 0U,
    PN23_BTA5_Awake_815 = 1U
}PN23_BTA5_t;

typedef enum
{
    PN24_BTA5_Not_awake_816 = 0U,
    PN24_BTA5_Awake_817 = 1U
}PN24_BTA5_t;

typedef enum
{
    PN25_BTA5_Not_awake_818 = 0U,
    PN25_BTA5_Awake_819 = 1U
}PN25_BTA5_t;

typedef enum
{
    PN26_BTA5_Not_awake_820 = 0U,
    PN26_BTA5_Awake_821 = 1U
}PN26_BTA5_t;

typedef enum
{
    PN27_BTA5_Not_awake_822 = 0U,
    PN27_BTA5_Awake_823 = 1U
}PN27_BTA5_t;

typedef enum
{
    PN28_BTA5_Not_awake_824 = 0U,
    PN28_BTA5_Awake_825 = 1U
}PN28_BTA5_t;

typedef enum
{
    PN29_BTA5_Not_awake_826 = 0U,
    PN29_BTA5_Awake_827 = 1U
}PN29_BTA5_t;

typedef enum
{
    PN30_BTA5_Not_awake_828 = 0U,
    PN30_BTA5_Awake_829 = 1U
}PN30_BTA5_t;

typedef enum
{
    PN31_BTA5_Not_awake_830 = 0U,
    PN31_BTA5_Awake_831 = 1U
}PN31_BTA5_t;

typedef enum
{
    PN32_BTA5_Not_awake_832 = 0U,
    PN32_BTA5_Awake_833 = 1U
}PN32_BTA5_t;

typedef enum
{
    PN33_BTA5_Not_awake_834 = 0U,
    PN33_BTA5_Awake_835 = 1U
}PN33_BTA5_t;

typedef enum
{
    PN34_BTA5_Not_awake_836 = 0U,
    PN34_BTA5_Awake_837 = 1U
}PN34_BTA5_t;

typedef enum
{
    PN35_BTA5_Not_awake_838 = 0U,
    PN35_BTA5_Awake_839 = 1U
}PN35_BTA5_t;

typedef enum
{
    PN36_BTA5_Not_awake_840 = 0U,
    PN36_BTA5_Awake_841 = 1U
}PN36_BTA5_t;

typedef enum
{
    PN37_BTA5_Not_awake_842 = 0U,
    PN37_BTA5_Awake_843 = 1U
}PN37_BTA5_t;

typedef enum
{
    PN38_BTA5_Not_awake_844 = 0U,
    PN38_BTA5_Awake_845 = 1U
}PN38_BTA5_t;

typedef enum
{
    PN39_BTA5_Not_awake_846 = 0U,
    PN39_BTA5_Awake_847 = 1U
}PN39_BTA5_t;

typedef enum
{
    PN40_BTA5_Not_awake_848 = 0U,
    PN40_BTA5_Awake_849 = 1U
}PN40_BTA5_t;

typedef enum
{
    PN41_BTA5_Not_awake_850 = 0U,
    PN41_BTA5_Awake_851 = 1U
}PN41_BTA5_t;

typedef enum
{
    PN42_BTA5_Not_awake_852 = 0U,
    PN42_BTA5_Awake_853 = 1U
}PN42_BTA5_t;

typedef enum
{
    PN43_BTA5_Not_awake_854 = 0U,
    PN43_BTA5_Awake_855 = 1U
}PN43_BTA5_t;

typedef enum
{
    PN44_BTA5_Not_awake_856 = 0U,
    PN44_BTA5_Awake_857 = 1U
}PN44_BTA5_t;

typedef enum
{
    PN45_BTA5_Not_awake_858 = 0U,
    PN45_BTA5_Awake_859 = 1U
}PN45_BTA5_t;

typedef enum
{
    PN46_BTA5_Not_awake_860 = 0U,
    PN46_BTA5_Awake_861 = 1U
}PN46_BTA5_t;

typedef enum
{
    PN47_BTA5_Not_awake_862 = 0U,
    PN47_BTA5_Awake_863 = 1U
}PN47_BTA5_t;

typedef enum
{
    PN48_BTA5_Not_awake_864 = 0U,
    PN48_BTA5_Awake_865 = 1U
}PN48_BTA5_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    // Source node identifier BTA_RB
    uint8_t ScrNodId_BTA5 : 8U;
    // Control bit vector, Repeat Message Request
    //  0 : "No repeat message request"
    //  1 : "Repeat message request"
    RepMsgReq_BTA5_t RepMsgReq_BTA5 : 1U;
    // Control bit vector, Network management coordination - Reserved
    uint8_t NetMngtCoorn_BTA5 : 2U;
    // Control bit vector, Network management coordinator sleep ready - Reserved
    uint8_t NetMngtCoorrSleepRdy_BTA5 : 1U;
    // Control bit vector, Active wakeup - Reserved
    uint8_t ActvWakeup_BTA5 : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit1_BTA5 : 1U;
    // Control bit vector, Partial network information - Reserved
    uint8_t PtlNetInfo_BTA5 : 1U;
    // Control bit vector, Reserved
    uint8_t ResdBit2_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN1_BTA5_t PN1_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN2_BTA5_t PN2_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN3_BTA5_t PN3_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN4_BTA5_t PN4_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN5_BTA5_t PN5_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN6_BTA5_t PN6_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN7_BTA5_t PN7_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN8_BTA5_t PN8_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN9_BTA5_t PN9_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN10_BTA5_t PN10_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN11_BTA5_t PN11_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN12_BTA5_t PN12_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN13_BTA5_t PN13_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN14_BTA5_t PN14_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN15_BTA5_t PN15_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN16_BTA5_t PN16_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN17_BTA5_t PN17_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN18_BTA5_t PN18_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN19_BTA5_t PN19_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN20_BTA5_t PN20_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN21_BTA5_t PN21_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN22_BTA5_t PN22_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN23_BTA5_t PN23_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN24_BTA5_t PN24_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN25_BTA5_t PN25_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN26_BTA5_t PN26_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN27_BTA5_t PN27_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN28_BTA5_t PN28_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN29_BTA5_t PN29_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN30_BTA5_t PN30_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN31_BTA5_t PN31_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN32_BTA5_t PN32_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN33_BTA5_t PN33_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN34_BTA5_t PN34_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN35_BTA5_t PN35_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN36_BTA5_t PN36_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN37_BTA5_t PN37_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN38_BTA5_t PN38_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN39_BTA5_t PN39_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN40_BTA5_t PN40_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN41_BTA5_t PN41_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN42_BTA5_t PN42_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN43_BTA5_t PN43_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN44_BTA5_t PN44_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN45_BTA5_t PN45_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN46_BTA5_t PN46_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN47_BTA5_t PN47_BTA5 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN48_BTA5_t PN48_BTA5 : 1U;
  }sig;
} NM_BTA5_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  NM_BTA5_un msg;
}NM_BTA5_msg;

// def @VC_Data_5E0 CAN Message (1504 0x5e0)
#define VC_Data_5E0_IDE (0U)
#define VC_Data_5E0_DLC (8U)
#define VC_Data_5E0_CANID (0x5e0U)
#define VC_Data_5E0_CYC (50U)

typedef union
{
  uint8_t data[8u];
  struct
  {
    // Data_CRC
    uint8_t VC_Data_5E0_CRC : 8U;
    // Data_MsgCntr
    uint8_t VC_Data_5E0_MsgCntr : 4U;
    uint32_t reserved0 : 4U;
    uint8_t VC_Data_5E0_Payload_0 : 8U;
    uint8_t VC_Data_5E0_Payload_1 : 8U;
    uint8_t VC_Data_5E0_Payload_2 : 8U;
    uint8_t VC_Data_5E0_Payload_3 : 8U;
    uint8_t VC_Data_5E0_Payload_4 : 8U;
    uint8_t VC_Data_5E0_Payload_5 : 8U;
  }sig;
} VC_Data_5E0_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  VC_Data_5E0_un msg;
}VC_Data_5E0_msg;

// def @DIAG_REQ_AllECU CAN Message (1537 0x601)
#define DIAG_REQ_AllECU_IDE (0U)
#define DIAG_REQ_AllECU_DLC (8U)
#define DIAG_REQ_AllECU_CANID (0x601U)

typedef union
{
  uint8_t data[8u];
  struct
  {
    // Diagnostic request data 0 for all ECUs (Functional address mode)
    uint8_t DiagReqData0_AllECU : 8U;
    // Diagnostic request data 1 for all ECUs (Functional address mode)
    uint8_t DiagReqData1_AllECU : 8U;
    // Diagnostic request data 2 for all ECUs (Functional address mode)
    uint8_t DiagReqData2_AllECU : 8U;
    // Diagnostic request data 3 for all ECUs (Functional address mode)
    uint8_t DiagReqData3_AllECU : 8U;
    // Diagnostic request data 4 for all ECUs (Functional address mode)
    uint8_t DiagReqData4_AllECU : 8U;
    // Diagnostic request data 5 for all ECUs (Functional address mode)
    uint8_t DiagReqData5_AllECU : 8U;
    // Diagnostic request data 6 for all ECUs (Functional address mode)
    uint8_t DiagReqData6_AllECU : 8U;
    // Diagnostic request data 7 for all ECUs (Functional address mode)
    uint8_t DiagReqData7_AllECU : 8U;
  }sig;
} DIAG_REQ_AllECU_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  DIAG_REQ_AllECU_un msg;
}DIAG_REQ_AllECU_msg;

// def @DIAG_REQ_BTA4 CAN Message (1651 0x673)
#define DIAG_REQ_BTA4_IDE (0U)
#define DIAG_REQ_BTA4_DLC (64U)
#define DIAG_REQ_BTA4_CANID (0x673U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t DiagReqData0_BTA4_0 : 8U;
    uint8_t DiagReqData0_BTA4_1 : 8U;
    uint8_t DiagReqData0_BTA4_2 : 8U;
    uint8_t DiagReqData0_BTA4_3 : 8U;
    uint8_t DiagReqData1_BTA4_0 : 8U;
    uint8_t DiagReqData1_BTA4_1 : 8U;
    uint8_t DiagReqData1_BTA4_2 : 8U;
    uint8_t DiagReqData1_BTA4_3 : 8U;
    uint8_t DiagReqData2_BTA4_0 : 8U;
    uint8_t DiagReqData2_BTA4_1 : 8U;
    uint8_t DiagReqData2_BTA4_2 : 8U;
    uint8_t DiagReqData2_BTA4_3 : 8U;
    uint8_t DiagReqData3_BTA4_0 : 8U;
    uint8_t DiagReqData3_BTA4_1 : 8U;
    uint8_t DiagReqData3_BTA4_2 : 8U;
    uint8_t DiagReqData3_BTA4_3 : 8U;
    uint8_t DiagReqData4_BTA4_0 : 8U;
    uint8_t DiagReqData4_BTA4_1 : 8U;
    uint8_t DiagReqData4_BTA4_2 : 8U;
    uint8_t DiagReqData4_BTA4_3 : 8U;
    uint8_t DiagReqData5_BTA4_0 : 8U;
    uint8_t DiagReqData5_BTA4_1 : 8U;
    uint8_t DiagReqData5_BTA4_2 : 8U;
    uint8_t DiagReqData5_BTA4_3 : 8U;
    uint8_t DiagReqData6_BTA4_0 : 8U;
    uint8_t DiagReqData6_BTA4_1 : 8U;
    uint8_t DiagReqData6_BTA4_2 : 8U;
    uint8_t DiagReqData6_BTA4_3 : 8U;
    uint8_t DiagReqData7_BTA4_0 : 8U;
    uint8_t DiagReqData7_BTA4_1 : 8U;
    uint8_t DiagReqData7_BTA4_2 : 8U;
    uint8_t DiagReqData7_BTA4_3 : 8U;
    uint8_t DiagReqData8_BTA4_0 : 8U;
    uint8_t DiagReqData8_BTA4_1 : 8U;
    uint8_t DiagReqData8_BTA4_2 : 8U;
    uint8_t DiagReqData8_BTA4_3 : 8U;
    uint8_t DiagReqData9_BTA4_0 : 8U;
    uint8_t DiagReqData9_BTA4_1 : 8U;
    uint8_t DiagReqData9_BTA4_2 : 8U;
    uint8_t DiagReqData9_BTA4_3 : 8U;
    uint8_t DiagReqData10_BTA4_0 : 8U;
    uint8_t DiagReqData10_BTA4_1 : 8U;
    uint8_t DiagReqData10_BTA4_2 : 8U;
    uint8_t DiagReqData10_BTA4_3 : 8U;
    uint8_t DiagReqData11_BTA4_0 : 8U;
    uint8_t DiagReqData11_BTA4_1 : 8U;
    uint8_t DiagReqData11_BTA4_2 : 8U;
    uint8_t DiagReqData11_BTA4_3 : 8U;
    uint8_t DiagReqData12_BTA4_0 : 8U;
    uint8_t DiagReqData12_BTA4_1 : 8U;
    uint8_t DiagReqData12_BTA4_2 : 8U;
    uint8_t DiagReqData12_BTA4_3 : 8U;
    uint8_t DiagReqData13_BTA4_0 : 8U;
    uint8_t DiagReqData13_BTA4_1 : 8U;
    uint8_t DiagReqData13_BTA4_2 : 8U;
    uint8_t DiagReqData13_BTA4_3 : 8U;
    uint8_t DiagReqData14_BTA4_0 : 8U;
    uint8_t DiagReqData14_BTA4_1 : 8U;
    uint8_t DiagReqData14_BTA4_2 : 8U;
    uint8_t DiagReqData14_BTA4_3 : 8U;
    uint8_t DiagReqData15_BTA4_0 : 8U;
    uint8_t DiagReqData15_BTA4_1 : 8U;
    uint8_t DiagReqData15_BTA4_2 : 8U;
    uint8_t DiagReqData15_BTA4_3 : 8U;
  }sig;
} DIAG_REQ_BTA4_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  DIAG_REQ_BTA4_un msg;
}DIAG_REQ_BTA4_msg;

// def @DIAG_RESP_BTA4 CAN Message (1779 0x6f3)
#define DIAG_RESP_BTA4_IDE (0U)
#define DIAG_RESP_BTA4_DLC (64U)
#define DIAG_RESP_BTA4_CANID (0x6f3U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t DiagRespData0_BTA4_0 : 8U;
    uint8_t DiagRespData0_BTA4_1 : 8U;
    uint8_t DiagRespData0_BTA4_2 : 8U;
    uint8_t DiagRespData0_BTA4_3 : 8U;
    uint8_t DiagRespData1_BTA4_0 : 8U;
    uint8_t DiagRespData1_BTA4_1 : 8U;
    uint8_t DiagRespData1_BTA4_2 : 8U;
    uint8_t DiagRespData1_BTA4_3 : 8U;
    uint8_t DiagRespData2_BTA4_0 : 8U;
    uint8_t DiagRespData2_BTA4_1 : 8U;
    uint8_t DiagRespData2_BTA4_2 : 8U;
    uint8_t DiagRespData2_BTA4_3 : 8U;
    uint8_t DiagRespData3_BTA4_0 : 8U;
    uint8_t DiagRespData3_BTA4_1 : 8U;
    uint8_t DiagRespData3_BTA4_2 : 8U;
    uint8_t DiagRespData3_BTA4_3 : 8U;
    uint8_t DiagRespData4_BTA4_0 : 8U;
    uint8_t DiagRespData4_BTA4_1 : 8U;
    uint8_t DiagRespData4_BTA4_2 : 8U;
    uint8_t DiagRespData4_BTA4_3 : 8U;
    uint8_t DiagRespData5_BTA4_0 : 8U;
    uint8_t DiagRespData5_BTA4_1 : 8U;
    uint8_t DiagRespData5_BTA4_2 : 8U;
    uint8_t DiagRespData5_BTA4_3 : 8U;
    uint8_t DiagRespData6_BTA4_0 : 8U;
    uint8_t DiagRespData6_BTA4_1 : 8U;
    uint8_t DiagRespData6_BTA4_2 : 8U;
    uint8_t DiagRespData6_BTA4_3 : 8U;
    uint8_t DiagRespData7_BTA4_0 : 8U;
    uint8_t DiagRespData7_BTA4_1 : 8U;
    uint8_t DiagRespData7_BTA4_2 : 8U;
    uint8_t DiagRespData7_BTA4_3 : 8U;
    uint8_t DiagRespData8_BTA4_0 : 8U;
    uint8_t DiagRespData8_BTA4_1 : 8U;
    uint8_t DiagRespData8_BTA4_2 : 8U;
    uint8_t DiagRespData8_BTA4_3 : 8U;
    uint8_t DiagRespData9_BTA4_0 : 8U;
    uint8_t DiagRespData9_BTA4_1 : 8U;
    uint8_t DiagRespData9_BTA4_2 : 8U;
    uint8_t DiagRespData9_BTA4_3 : 8U;
    uint8_t DiagRespData10_BTA4_0 : 8U;
    uint8_t DiagRespData10_BTA4_1 : 8U;
    uint8_t DiagRespData10_BTA4_2 : 8U;
    uint8_t DiagRespData10_BTA4_3 : 8U;
    uint8_t DiagRespData11_BTA4_0 : 8U;
    uint8_t DiagRespData11_BTA4_1 : 8U;
    uint8_t DiagRespData11_BTA4_2 : 8U;
    uint8_t DiagRespData11_BTA4_3 : 8U;
    uint8_t DiagRespData12_BTA4_0 : 8U;
    uint8_t DiagRespData12_BTA4_1 : 8U;
    uint8_t DiagRespData12_BTA4_2 : 8U;
    uint8_t DiagRespData12_BTA4_3 : 8U;
    uint8_t DiagRespData13_BTA4_0 : 8U;
    uint8_t DiagRespData13_BTA4_1 : 8U;
    uint8_t DiagRespData13_BTA4_2 : 8U;
    uint8_t DiagRespData13_BTA4_3 : 8U;
    uint8_t DiagRespData14_BTA4_0 : 8U;
    uint8_t DiagRespData14_BTA4_1 : 8U;
    uint8_t DiagRespData14_BTA4_2 : 8U;
    uint8_t DiagRespData14_BTA4_3 : 8U;
    uint8_t DiagRespData15_BTA4_0 : 8U;
    uint8_t DiagRespData15_BTA4_1 : 8U;
    uint8_t DiagRespData15_BTA4_2 : 8U;
    uint8_t DiagRespData15_BTA4_3 : 8U;
  }sig;
} DIAG_RESP_BTA4_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  DIAG_RESP_BTA4_un msg;
}DIAG_RESP_BTA4_msg;

extern BCU_11E_msg BCU_11E_t;
extern ZONE_13C_msg ZONE_13C_t;
extern BLE_SnifferParam_Dev1_msg BLE_SnifferParam_Dev1_t;
extern BLE_SnifferParam_Dev2_msg BLE_SnifferParam_Dev2_t;
extern BLE_SnifferParam_Dev3_msg BLE_SnifferParam_Dev3_t;
extern BLE_SnifferParam_Dev4_msg BLE_SnifferParam_Dev4_t;
extern BLE_SnifferParam_Dev5_msg BLE_SnifferParam_Dev5_t;
extern BTA4_RSSI_UWB_msg BTA4_RSSI_UWB_t;
extern UWB_Info_Ntf_msg UWB_Info_Ntf_t;
extern UWB_Info_Ntf_Sup_msg UWB_Info_Ntf_Sup_t;
extern IotHubSts_msg IotHubSts_t;
extern ZONE_296_msg ZONE_296_t;
extern BTA_SRR_297_msg BTA_SRR_297_t;
extern Device_IRK_Group_1_msg Device_IRK_Group_1_t;
extern Device_IRK_Group_2_msg Device_IRK_Group_2_t;
extern BTA4_FusnData_msg BTA4_FusnData_t;
extern ZONE_3AA_msg ZONE_3AA_t;
extern ZONE_3B2_msg ZONE_3B2_t;
extern BTA4_Log_Data_msg BTA4_Log_Data_t;
extern IotConnectSts_msg IotConnectSts_t;
extern IotDownLinkData_msg IotDownLinkData_t;
extern IotUpLinkFlowCtrl_msg IotUpLinkFlowCtrl_t;
extern IotUplinkData_msg IotUplinkData_t;
extern IotDownLinkFlowCtrl_msg IotDownLinkFlowCtrl_t;
extern IotScanConnectReq_msg IotScanConnectReq_t;
extern BTA_SRR_FactoryRsp_msg BTA_SRR_FactoryRsp_t;
extern BTA_SRR_FactoryReq_msg BTA_SRR_FactoryReq_t;
extern NM_ZONE_LEM_msg NM_ZONE_LEM_t;
extern NM_BTA1_msg NM_BTA1_t;
extern NM_BTA2_msg NM_BTA2_t;
extern NM_BTA3_msg NM_BTA3_t;
extern NM_BTA4_msg NM_BTA4_t;
extern NM_BTA5_msg NM_BTA5_t;
extern VC_Data_5E0_msg VC_Data_5E0_t;
extern DIAG_REQ_AllECU_msg DIAG_REQ_AllECU_t;
extern DIAG_REQ_BTA4_msg DIAG_REQ_BTA4_t;
extern DIAG_RESP_BTA4_msg DIAG_RESP_BTA4_t;


/*  Rx */
#define IL_VehSpd_Rdnt1_phy_RX  (\
	(BCU_11E_t.msg.sig.VehSpd_Rdnt1_phy_0 << 8U) | \
* (0.05625) + (0))

/* Tx */
#define IL_VehSpd_Rdnt1_phy_TX(x)  \
{ \
	BCU_11E_t.msg.sig.VehSpd_Rdnt1_phy_0 = ((x) >> 8U) & 31U; \
	BCU_11E_t.msg.sig.VehSpd_Rdnt1_phy_1 = (x) & 255U; \
}

/*  Rx */
#define IL_VehSpdSts_Rdnt1_RX  (BCU_11E_t.msg.sig.VehSpdSts_Rdnt1)

/*  Tx */
#define IL_VehSpdSts_Rdnt1_TX(x)  \
{ \
	BCU_11E_t.msg.sig.VehSpdSts_Rdnt1 = (x);\
}

/*  Rx */
#define IL_ZONE_13C_CRC_RX  (ZONE_13C_t.msg.sig.ZONE_13C_CRC)

/*  Tx */
#define IL_ZONE_13C_CRC_TX(x)  \
{ \
	ZONE_13C_t.msg.sig.ZONE_13C_CRC = (x);\
}

/*  Rx */
#define IL_ZONE_13C_MsgCntr_RX  (ZONE_13C_t.msg.sig.ZONE_13C_MsgCntr)

/*  Tx */
#define IL_ZONE_13C_MsgCntr_TX(x)  \
{ \
	ZONE_13C_t.msg.sig.ZONE_13C_MsgCntr = (x);\
}

/*  Rx */
#define IL_DrvState_RX  (ZONE_13C_t.msg.sig.DrvState)

/*  Tx */
#define IL_DrvState_TX(x)  \
{ \
	ZONE_13C_t.msg.sig.DrvState = (x);\
}

/*  Rx */
#define IL_OperatorState_RX  (ZONE_13C_t.msg.sig.OperatorState)

/*  Tx */
#define IL_OperatorState_TX(x)  \
{ \
	ZONE_13C_t.msg.sig.OperatorState = (x);\
}

/*  Rx */
#define IL_VehStateDetailed_RX  (ZONE_13C_t.msg.sig.VehStateDetailed)

/*  Tx */
#define IL_VehStateDetailed_TX(x)  \
{ \
	ZONE_13C_t.msg.sig.VehStateDetailed = (x);\
}

/*  Rx */
#define IL_ConnHopIncrement_Dev1_RX  (BLE_SnifferParam_Dev1_t.msg.sig.ConnHopIncrement_Dev1)

/*  Tx */
#define IL_ConnHopIncrement_Dev1_TX(x)  \
{ \
	BLE_SnifferParam_Dev1_t.msg.sig.ConnHopIncrement_Dev1 = (x);\
}

/*  Rx */
#define IL_TransmitWindowSize_Dev1_RX  (BLE_SnifferParam_Dev1_t.msg.sig.TransmitWindowSize_Dev1)

/*  Tx */
#define IL_TransmitWindowSize_Dev1_TX(x)  \
{ \
	BLE_SnifferParam_Dev1_t.msg.sig.TransmitWindowSize_Dev1 = (x);\
}

/*  Rx */
#define IL_TransmitWindowOffset_Dev1_RX  (\
	(BLE_SnifferParam_Dev1_t.msg.sig.TransmitWindowOffset_Dev1_0 << 8U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.TransmitWindowOffset_Dev1_1))

/* Tx */
#define IL_TransmitWindowOffset_Dev1_TX(x)  \
{ \
	BLE_SnifferParam_Dev1_t.msg.sig.TransmitWindowOffset_Dev1_0 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.TransmitWindowOffset_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnInterval_Dev1_RX  (\
	(BLE_SnifferParam_Dev1_t.msg.sig.ConnInterval_Dev1_0 << 8U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.ConnInterval_Dev1_1))

/* Tx */
#define IL_ConnInterval_Dev1_TX(x)  \
{ \
	BLE_SnifferParam_Dev1_t.msg.sig.ConnInterval_Dev1_0 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.ConnInterval_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnChannelMap_Dev1_RX  (\
	(BLE_SnifferParam_Dev1_t.msg.sig.ConnChannelMap_Dev1_0 << 32U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.ConnChannelMap_Dev1_1 << 24U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.ConnChannelMap_Dev1_2 << 16U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.ConnChannelMap_Dev1_3 << 8U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.ConnChannelMap_Dev1_4))

/* Tx */
#define IL_ConnChannelMap_Dev1_TX(x)  \
{ \
	BLE_SnifferParam_Dev1_t.msg.sig.ConnChannelMap_Dev1_0 = ((x) >> 32U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.ConnChannelMap_Dev1_1 = ((x) >> 24U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.ConnChannelMap_Dev1_2 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.ConnChannelMap_Dev1_3 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.ConnChannelMap_Dev1_4 = (x) & 255U; \
}

/*  Rx */
#define IL_CRCInitializatonValue_Dev1_RX  (\
	(BLE_SnifferParam_Dev1_t.msg.sig.CRCInitializatonValue_Dev1_0 << 16U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.CRCInitializatonValue_Dev1_1 << 8U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.CRCInitializatonValue_Dev1_2))

/* Tx */
#define IL_CRCInitializatonValue_Dev1_TX(x)  \
{ \
	BLE_SnifferParam_Dev1_t.msg.sig.CRCInitializatonValue_Dev1_0 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.CRCInitializatonValue_Dev1_1 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.CRCInitializatonValue_Dev1_2 = (x) & 255U; \
}

/*  Rx */
#define IL_AccessAddress_Dev1_RX  (\
	(BLE_SnifferParam_Dev1_t.msg.sig.AccessAddress_Dev1_0 << 24U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.AccessAddress_Dev1_1 << 16U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.AccessAddress_Dev1_2 << 8U) | \
	(BLE_SnifferParam_Dev1_t.msg.sig.AccessAddress_Dev1_3))

/* Tx */
#define IL_AccessAddress_Dev1_TX(x)  \
{ \
	BLE_SnifferParam_Dev1_t.msg.sig.AccessAddress_Dev1_0 = ((x) >> 24U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.AccessAddress_Dev1_1 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.AccessAddress_Dev1_2 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev1_t.msg.sig.AccessAddress_Dev1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_CurrentDatachannel_Dev1_RX  (BLE_SnifferParam_Dev1_t.msg.sig.CurrentDatachannel_Dev1)

/*  Tx */
#define IL_CurrentDatachannel_Dev1_TX(x)  \
{ \
	BLE_SnifferParam_Dev1_t.msg.sig.CurrentDatachannel_Dev1 = (x);\
}

/*  Rx */
#define IL_SniffReqest_Dev1_RX  (BLE_SnifferParam_Dev1_t.msg.sig.SniffReqest_Dev1)

/*  Tx */
#define IL_SniffReqest_Dev1_TX(x)  \
{ \
	BLE_SnifferParam_Dev1_t.msg.sig.SniffReqest_Dev1 = (x);\
}

/*  Rx */
#define IL_ConnHopIncrement_Dev2_RX  (BLE_SnifferParam_Dev2_t.msg.sig.ConnHopIncrement_Dev2)

/*  Tx */
#define IL_ConnHopIncrement_Dev2_TX(x)  \
{ \
	BLE_SnifferParam_Dev2_t.msg.sig.ConnHopIncrement_Dev2 = (x);\
}

/*  Rx */
#define IL_TransmitWindowSize_Dev2_RX  (BLE_SnifferParam_Dev2_t.msg.sig.TransmitWindowSize_Dev2)

/*  Tx */
#define IL_TransmitWindowSize_Dev2_TX(x)  \
{ \
	BLE_SnifferParam_Dev2_t.msg.sig.TransmitWindowSize_Dev2 = (x);\
}

/*  Rx */
#define IL_TransmitWindowOffset_Dev2_RX  (\
	(BLE_SnifferParam_Dev2_t.msg.sig.TransmitWindowOffset_Dev2_0 << 8U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.TransmitWindowOffset_Dev2_1))

/* Tx */
#define IL_TransmitWindowOffset_Dev2_TX(x)  \
{ \
	BLE_SnifferParam_Dev2_t.msg.sig.TransmitWindowOffset_Dev2_0 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.TransmitWindowOffset_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnInterval_Dev2_RX  (\
	(BLE_SnifferParam_Dev2_t.msg.sig.ConnInterval_Dev2_0 << 8U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.ConnInterval_Dev2_1))

/* Tx */
#define IL_ConnInterval_Dev2_TX(x)  \
{ \
	BLE_SnifferParam_Dev2_t.msg.sig.ConnInterval_Dev2_0 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.ConnInterval_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnChannelMap_Dev2_RX  (\
	(BLE_SnifferParam_Dev2_t.msg.sig.ConnChannelMap_Dev2_0 << 32U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.ConnChannelMap_Dev2_1 << 24U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.ConnChannelMap_Dev2_2 << 16U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.ConnChannelMap_Dev2_3 << 8U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.ConnChannelMap_Dev2_4))

/* Tx */
#define IL_ConnChannelMap_Dev2_TX(x)  \
{ \
	BLE_SnifferParam_Dev2_t.msg.sig.ConnChannelMap_Dev2_0 = ((x) >> 32U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.ConnChannelMap_Dev2_1 = ((x) >> 24U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.ConnChannelMap_Dev2_2 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.ConnChannelMap_Dev2_3 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.ConnChannelMap_Dev2_4 = (x) & 255U; \
}

/*  Rx */
#define IL_CRCInitializatonValue_Dev2_RX  (\
	(BLE_SnifferParam_Dev2_t.msg.sig.CRCInitializatonValue_Dev2_0 << 16U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.CRCInitializatonValue_Dev2_1 << 8U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.CRCInitializatonValue_Dev2_2))

/* Tx */
#define IL_CRCInitializatonValue_Dev2_TX(x)  \
{ \
	BLE_SnifferParam_Dev2_t.msg.sig.CRCInitializatonValue_Dev2_0 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.CRCInitializatonValue_Dev2_1 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.CRCInitializatonValue_Dev2_2 = (x) & 255U; \
}

/*  Rx */
#define IL_AccessAddress_Dev2_RX  (\
	(BLE_SnifferParam_Dev2_t.msg.sig.AccessAddress_Dev2_0 << 24U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.AccessAddress_Dev2_1 << 16U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.AccessAddress_Dev2_2 << 8U) | \
	(BLE_SnifferParam_Dev2_t.msg.sig.AccessAddress_Dev2_3))

/* Tx */
#define IL_AccessAddress_Dev2_TX(x)  \
{ \
	BLE_SnifferParam_Dev2_t.msg.sig.AccessAddress_Dev2_0 = ((x) >> 24U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.AccessAddress_Dev2_1 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.AccessAddress_Dev2_2 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev2_t.msg.sig.AccessAddress_Dev2_3 = (x) & 255U; \
}

/*  Rx */
#define IL_CurrentDatachannel_Dev2_RX  (BLE_SnifferParam_Dev2_t.msg.sig.CurrentDatachannel_Dev2)

/*  Tx */
#define IL_CurrentDatachannel_Dev2_TX(x)  \
{ \
	BLE_SnifferParam_Dev2_t.msg.sig.CurrentDatachannel_Dev2 = (x);\
}

/*  Rx */
#define IL_SniffReqest_Dev2_RX  (BLE_SnifferParam_Dev2_t.msg.sig.SniffReqest_Dev2)

/*  Tx */
#define IL_SniffReqest_Dev2_TX(x)  \
{ \
	BLE_SnifferParam_Dev2_t.msg.sig.SniffReqest_Dev2 = (x);\
}

/*  Rx */
#define IL_ConnHopIncrement_Dev3_RX  (BLE_SnifferParam_Dev3_t.msg.sig.ConnHopIncrement_Dev3)

/*  Tx */
#define IL_ConnHopIncrement_Dev3_TX(x)  \
{ \
	BLE_SnifferParam_Dev3_t.msg.sig.ConnHopIncrement_Dev3 = (x);\
}

/*  Rx */
#define IL_TransmitWindowSize_Dev3_RX  (BLE_SnifferParam_Dev3_t.msg.sig.TransmitWindowSize_Dev3)

/*  Tx */
#define IL_TransmitWindowSize_Dev3_TX(x)  \
{ \
	BLE_SnifferParam_Dev3_t.msg.sig.TransmitWindowSize_Dev3 = (x);\
}

/*  Rx */
#define IL_TransmitWindowOffset_Dev3_RX  (\
	(BLE_SnifferParam_Dev3_t.msg.sig.TransmitWindowOffset_Dev3_0 << 8U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.TransmitWindowOffset_Dev3_1))

/* Tx */
#define IL_TransmitWindowOffset_Dev3_TX(x)  \
{ \
	BLE_SnifferParam_Dev3_t.msg.sig.TransmitWindowOffset_Dev3_0 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.TransmitWindowOffset_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnInterval_Dev3_RX  (\
	(BLE_SnifferParam_Dev3_t.msg.sig.ConnInterval_Dev3_0 << 8U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.ConnInterval_Dev3_1))

/* Tx */
#define IL_ConnInterval_Dev3_TX(x)  \
{ \
	BLE_SnifferParam_Dev3_t.msg.sig.ConnInterval_Dev3_0 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.ConnInterval_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnChannelMap_Dev3_RX  (\
	(BLE_SnifferParam_Dev3_t.msg.sig.ConnChannelMap_Dev3_0 << 32U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.ConnChannelMap_Dev3_1 << 24U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.ConnChannelMap_Dev3_2 << 16U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.ConnChannelMap_Dev3_3 << 8U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.ConnChannelMap_Dev3_4))

/* Tx */
#define IL_ConnChannelMap_Dev3_TX(x)  \
{ \
	BLE_SnifferParam_Dev3_t.msg.sig.ConnChannelMap_Dev3_0 = ((x) >> 32U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.ConnChannelMap_Dev3_1 = ((x) >> 24U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.ConnChannelMap_Dev3_2 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.ConnChannelMap_Dev3_3 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.ConnChannelMap_Dev3_4 = (x) & 255U; \
}

/*  Rx */
#define IL_CRCInitializatonValue_Dev3_RX  (\
	(BLE_SnifferParam_Dev3_t.msg.sig.CRCInitializatonValue_Dev3_0 << 16U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.CRCInitializatonValue_Dev3_1 << 8U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.CRCInitializatonValue_Dev3_2))

/* Tx */
#define IL_CRCInitializatonValue_Dev3_TX(x)  \
{ \
	BLE_SnifferParam_Dev3_t.msg.sig.CRCInitializatonValue_Dev3_0 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.CRCInitializatonValue_Dev3_1 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.CRCInitializatonValue_Dev3_2 = (x) & 255U; \
}

/*  Rx */
#define IL_AccessAddress_Dev3_RX  (\
	(BLE_SnifferParam_Dev3_t.msg.sig.AccessAddress_Dev3_0 << 24U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.AccessAddress_Dev3_1 << 16U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.AccessAddress_Dev3_2 << 8U) | \
	(BLE_SnifferParam_Dev3_t.msg.sig.AccessAddress_Dev3_3))

/* Tx */
#define IL_AccessAddress_Dev3_TX(x)  \
{ \
	BLE_SnifferParam_Dev3_t.msg.sig.AccessAddress_Dev3_0 = ((x) >> 24U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.AccessAddress_Dev3_1 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.AccessAddress_Dev3_2 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev3_t.msg.sig.AccessAddress_Dev3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_CurrentDatachannel_Dev3_RX  (BLE_SnifferParam_Dev3_t.msg.sig.CurrentDatachannel_Dev3)

/*  Tx */
#define IL_CurrentDatachannel_Dev3_TX(x)  \
{ \
	BLE_SnifferParam_Dev3_t.msg.sig.CurrentDatachannel_Dev3 = (x);\
}

/*  Rx */
#define IL_SniffReqest_Dev3_RX  (BLE_SnifferParam_Dev3_t.msg.sig.SniffReqest_Dev3)

/*  Tx */
#define IL_SniffReqest_Dev3_TX(x)  \
{ \
	BLE_SnifferParam_Dev3_t.msg.sig.SniffReqest_Dev3 = (x);\
}

/*  Rx */
#define IL_ConnHopIncrement_Dev4_RX  (BLE_SnifferParam_Dev4_t.msg.sig.ConnHopIncrement_Dev4)

/*  Tx */
#define IL_ConnHopIncrement_Dev4_TX(x)  \
{ \
	BLE_SnifferParam_Dev4_t.msg.sig.ConnHopIncrement_Dev4 = (x);\
}

/*  Rx */
#define IL_TransmitWindowSize_Dev4_RX  (BLE_SnifferParam_Dev4_t.msg.sig.TransmitWindowSize_Dev4)

/*  Tx */
#define IL_TransmitWindowSize_Dev4_TX(x)  \
{ \
	BLE_SnifferParam_Dev4_t.msg.sig.TransmitWindowSize_Dev4 = (x);\
}

/*  Rx */
#define IL_TransmitWindowOffset_Dev4_RX  (\
	(BLE_SnifferParam_Dev4_t.msg.sig.TransmitWindowOffset_Dev4_0 << 8U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.TransmitWindowOffset_Dev4_1))

/* Tx */
#define IL_TransmitWindowOffset_Dev4_TX(x)  \
{ \
	BLE_SnifferParam_Dev4_t.msg.sig.TransmitWindowOffset_Dev4_0 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.TransmitWindowOffset_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnInterval_Dev4_RX  (\
	(BLE_SnifferParam_Dev4_t.msg.sig.ConnInterval_Dev4_0 << 8U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.ConnInterval_Dev4_1))

/* Tx */
#define IL_ConnInterval_Dev4_TX(x)  \
{ \
	BLE_SnifferParam_Dev4_t.msg.sig.ConnInterval_Dev4_0 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.ConnInterval_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnChannelMap_Dev4_RX  (\
	(BLE_SnifferParam_Dev4_t.msg.sig.ConnChannelMap_Dev4_0 << 32U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.ConnChannelMap_Dev4_1 << 24U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.ConnChannelMap_Dev4_2 << 16U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.ConnChannelMap_Dev4_3 << 8U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.ConnChannelMap_Dev4_4))

/* Tx */
#define IL_ConnChannelMap_Dev4_TX(x)  \
{ \
	BLE_SnifferParam_Dev4_t.msg.sig.ConnChannelMap_Dev4_0 = ((x) >> 32U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.ConnChannelMap_Dev4_1 = ((x) >> 24U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.ConnChannelMap_Dev4_2 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.ConnChannelMap_Dev4_3 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.ConnChannelMap_Dev4_4 = (x) & 255U; \
}

/*  Rx */
#define IL_CRCInitializatonValue_Dev4_RX  (\
	(BLE_SnifferParam_Dev4_t.msg.sig.CRCInitializatonValue_Dev4_0 << 16U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.CRCInitializatonValue_Dev4_1 << 8U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.CRCInitializatonValue_Dev4_2))

/* Tx */
#define IL_CRCInitializatonValue_Dev4_TX(x)  \
{ \
	BLE_SnifferParam_Dev4_t.msg.sig.CRCInitializatonValue_Dev4_0 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.CRCInitializatonValue_Dev4_1 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.CRCInitializatonValue_Dev4_2 = (x) & 255U; \
}

/*  Rx */
#define IL_AccessAddress_Dev4_RX  (\
	(BLE_SnifferParam_Dev4_t.msg.sig.AccessAddress_Dev4_0 << 24U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.AccessAddress_Dev4_1 << 16U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.AccessAddress_Dev4_2 << 8U) | \
	(BLE_SnifferParam_Dev4_t.msg.sig.AccessAddress_Dev4_3))

/* Tx */
#define IL_AccessAddress_Dev4_TX(x)  \
{ \
	BLE_SnifferParam_Dev4_t.msg.sig.AccessAddress_Dev4_0 = ((x) >> 24U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.AccessAddress_Dev4_1 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.AccessAddress_Dev4_2 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev4_t.msg.sig.AccessAddress_Dev4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_CurrentDatachannel_Dev4_RX  (BLE_SnifferParam_Dev4_t.msg.sig.CurrentDatachannel_Dev4)

/*  Tx */
#define IL_CurrentDatachannel_Dev4_TX(x)  \
{ \
	BLE_SnifferParam_Dev4_t.msg.sig.CurrentDatachannel_Dev4 = (x);\
}

/*  Rx */
#define IL_SniffReqest_Dev4_RX  (BLE_SnifferParam_Dev4_t.msg.sig.SniffReqest_Dev4)

/*  Tx */
#define IL_SniffReqest_Dev4_TX(x)  \
{ \
	BLE_SnifferParam_Dev4_t.msg.sig.SniffReqest_Dev4 = (x);\
}

/*  Rx */
#define IL_ConnHopIncrement_Dev5_RX  (BLE_SnifferParam_Dev5_t.msg.sig.ConnHopIncrement_Dev5)

/*  Tx */
#define IL_ConnHopIncrement_Dev5_TX(x)  \
{ \
	BLE_SnifferParam_Dev5_t.msg.sig.ConnHopIncrement_Dev5 = (x);\
}

/*  Rx */
#define IL_TransmitWindowSize_Dev5_RX  (BLE_SnifferParam_Dev5_t.msg.sig.TransmitWindowSize_Dev5)

/*  Tx */
#define IL_TransmitWindowSize_Dev5_TX(x)  \
{ \
	BLE_SnifferParam_Dev5_t.msg.sig.TransmitWindowSize_Dev5 = (x);\
}

/*  Rx */
#define IL_TransmitWindowOffset_Dev5_RX  (\
	(BLE_SnifferParam_Dev5_t.msg.sig.TransmitWindowOffset_Dev5_0 << 8U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.TransmitWindowOffset_Dev5_1))

/* Tx */
#define IL_TransmitWindowOffset_Dev5_TX(x)  \
{ \
	BLE_SnifferParam_Dev5_t.msg.sig.TransmitWindowOffset_Dev5_0 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.TransmitWindowOffset_Dev5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnInterval_Dev5_RX  (\
	(BLE_SnifferParam_Dev5_t.msg.sig.ConnInterval_Dev5_0 << 8U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.ConnInterval_Dev5_1))

/* Tx */
#define IL_ConnInterval_Dev5_TX(x)  \
{ \
	BLE_SnifferParam_Dev5_t.msg.sig.ConnInterval_Dev5_0 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.ConnInterval_Dev5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnChannelMap_Dev5_RX  (\
	(BLE_SnifferParam_Dev5_t.msg.sig.ConnChannelMap_Dev5_0 << 32U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.ConnChannelMap_Dev5_1 << 24U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.ConnChannelMap_Dev5_2 << 16U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.ConnChannelMap_Dev5_3 << 8U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.ConnChannelMap_Dev5_4))

/* Tx */
#define IL_ConnChannelMap_Dev5_TX(x)  \
{ \
	BLE_SnifferParam_Dev5_t.msg.sig.ConnChannelMap_Dev5_0 = ((x) >> 32U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.ConnChannelMap_Dev5_1 = ((x) >> 24U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.ConnChannelMap_Dev5_2 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.ConnChannelMap_Dev5_3 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.ConnChannelMap_Dev5_4 = (x) & 255U; \
}

/*  Rx */
#define IL_CRCInitializatonValue_Dev5_RX  (\
	(BLE_SnifferParam_Dev5_t.msg.sig.CRCInitializatonValue_Dev5_0 << 16U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.CRCInitializatonValue_Dev5_1 << 8U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.CRCInitializatonValue_Dev5_2))

/* Tx */
#define IL_CRCInitializatonValue_Dev5_TX(x)  \
{ \
	BLE_SnifferParam_Dev5_t.msg.sig.CRCInitializatonValue_Dev5_0 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.CRCInitializatonValue_Dev5_1 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.CRCInitializatonValue_Dev5_2 = (x) & 255U; \
}

/*  Rx */
#define IL_AccessAddress_Dev5_RX  (\
	(BLE_SnifferParam_Dev5_t.msg.sig.AccessAddress_Dev5_0 << 24U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.AccessAddress_Dev5_1 << 16U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.AccessAddress_Dev5_2 << 8U) | \
	(BLE_SnifferParam_Dev5_t.msg.sig.AccessAddress_Dev5_3))

/* Tx */
#define IL_AccessAddress_Dev5_TX(x)  \
{ \
	BLE_SnifferParam_Dev5_t.msg.sig.AccessAddress_Dev5_0 = ((x) >> 24U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.AccessAddress_Dev5_1 = ((x) >> 16U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.AccessAddress_Dev5_2 = ((x) >> 8U) & 255U; \
	BLE_SnifferParam_Dev5_t.msg.sig.AccessAddress_Dev5_3 = (x) & 255U; \
}

/*  Rx */
#define IL_CurrentDatachannel_Dev5_RX  (BLE_SnifferParam_Dev5_t.msg.sig.CurrentDatachannel_Dev5)

/*  Tx */
#define IL_CurrentDatachannel_Dev5_TX(x)  \
{ \
	BLE_SnifferParam_Dev5_t.msg.sig.CurrentDatachannel_Dev5 = (x);\
}

/*  Rx */
#define IL_SniffReqest_Dev5_RX  (BLE_SnifferParam_Dev5_t.msg.sig.SniffReqest_Dev5)

/*  Tx */
#define IL_SniffReqest_Dev5_TX(x)  \
{ \
	BLE_SnifferParam_Dev5_t.msg.sig.SniffReqest_Dev5 = (x);\
}

/*  Rx */
#define IL_BTA4_RSSI_Dev1_phy_RX  (BTA4_RSSI_UWB_t.msg.sig.BTA4_RSSI_Dev1_phy)

/*  Tx */
#define IL_BTA4_RSSI_Dev1_phy_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_RSSI_Dev1_phy = (x);\
}

/*  Rx */
#define IL_BTA4_RSSI_Dev2_phy_RX  (BTA4_RSSI_UWB_t.msg.sig.BTA4_RSSI_Dev2_phy)

/*  Tx */
#define IL_BTA4_RSSI_Dev2_phy_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_RSSI_Dev2_phy = (x);\
}

/*  Rx */
#define IL_BTA4_RSSI_Dev3_phy_RX  (BTA4_RSSI_UWB_t.msg.sig.BTA4_RSSI_Dev3_phy)

/*  Tx */
#define IL_BTA4_RSSI_Dev3_phy_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_RSSI_Dev3_phy = (x);\
}

/*  Rx */
#define IL_BTA4_RSSI_Dev4_phy_RX  (BTA4_RSSI_UWB_t.msg.sig.BTA4_RSSI_Dev4_phy)

/*  Tx */
#define IL_BTA4_RSSI_Dev4_phy_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_RSSI_Dev4_phy = (x);\
}

/*  Rx */
#define IL_BTA4_RSSI_Dev5_phy_RX  (BTA4_RSSI_UWB_t.msg.sig.BTA4_RSSI_Dev5_phy)

/*  Tx */
#define IL_BTA4_RSSI_Dev5_phy_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_RSSI_Dev5_phy = (x);\
}

/*  Rx */
#define IL_BTA4_UWB_Dist_Dev1_RX  (\
	(BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev1_0 << 8U) | \
	(BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev1_1))

/* Tx */
#define IL_BTA4_UWB_Dist_Dev1_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev1_0 = ((x) >> 8U) & 255U; \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_UWB_Dist_Dev2_RX  (\
	(BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev2_0 << 8U) | \
	(BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev2_1))

/* Tx */
#define IL_BTA4_UWB_Dist_Dev2_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev2_0 = ((x) >> 8U) & 255U; \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_UWB_Dist_Dev3_RX  (\
	(BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev3_0 << 8U) | \
	(BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev3_1))

/* Tx */
#define IL_BTA4_UWB_Dist_Dev3_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev3_0 = ((x) >> 8U) & 255U; \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_UWB_Dist_Dev4_RX  (\
	(BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev4_0 << 8U) | \
	(BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev4_1))

/* Tx */
#define IL_BTA4_UWB_Dist_Dev4_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev4_0 = ((x) >> 8U) & 255U; \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_UWB_Dist_Dev5_RX  (\
	(BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev5_0 << 8U) | \
	(BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev5_1))

/* Tx */
#define IL_BTA4_UWB_Dist_Dev5_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev5_0 = ((x) >> 8U) & 255U; \
	BTA4_RSSI_UWB_t.msg.sig.BTA4_UWB_Dist_Dev5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_UWB_Info_Sync_Sts_dev1_RX  (BTA4_RSSI_UWB_t.msg.sig.UWB_Info_Sync_Sts_dev1)

/*  Tx */
#define IL_UWB_Info_Sync_Sts_dev1_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.UWB_Info_Sync_Sts_dev1 = (x);\
}

/*  Rx */
#define IL_UWB_Info_Sync_Sts_dev2_RX  (BTA4_RSSI_UWB_t.msg.sig.UWB_Info_Sync_Sts_dev2)

/*  Tx */
#define IL_UWB_Info_Sync_Sts_dev2_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.UWB_Info_Sync_Sts_dev2 = (x);\
}

/*  Rx */
#define IL_UWB_Info_Sync_Sts_dev3_RX  (BTA4_RSSI_UWB_t.msg.sig.UWB_Info_Sync_Sts_dev3)

/*  Tx */
#define IL_UWB_Info_Sync_Sts_dev3_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.UWB_Info_Sync_Sts_dev3 = (x);\
}

/*  Rx */
#define IL_UWB_Info_Sync_Sts_dev4_RX  (BTA4_RSSI_UWB_t.msg.sig.UWB_Info_Sync_Sts_dev4)

/*  Tx */
#define IL_UWB_Info_Sync_Sts_dev4_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.UWB_Info_Sync_Sts_dev4 = (x);\
}

/*  Rx */
#define IL_UWB_Info_Sync_Sts_dev5_RX  (BTA4_RSSI_UWB_t.msg.sig.UWB_Info_Sync_Sts_dev5)

/*  Tx */
#define IL_UWB_Info_Sync_Sts_dev5_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.UWB_Info_Sync_Sts_dev5 = (x);\
}

/*  Rx */
#define IL_UWB_Ranging_Sts_dev1_RX  (BTA4_RSSI_UWB_t.msg.sig.UWB_Ranging_Sts_dev1)

/*  Tx */
#define IL_UWB_Ranging_Sts_dev1_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.UWB_Ranging_Sts_dev1 = (x);\
}

/*  Rx */
#define IL_UWB_Ranging_Sts_dev2_RX  (BTA4_RSSI_UWB_t.msg.sig.UWB_Ranging_Sts_dev2)

/*  Tx */
#define IL_UWB_Ranging_Sts_dev2_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.UWB_Ranging_Sts_dev2 = (x);\
}

/*  Rx */
#define IL_UWB_Ranging_Sts_dev3_RX  (BTA4_RSSI_UWB_t.msg.sig.UWB_Ranging_Sts_dev3)

/*  Tx */
#define IL_UWB_Ranging_Sts_dev3_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.UWB_Ranging_Sts_dev3 = (x);\
}

/*  Rx */
#define IL_UWB_Ranging_Sts_dev4_RX  (BTA4_RSSI_UWB_t.msg.sig.UWB_Ranging_Sts_dev4)

/*  Tx */
#define IL_UWB_Ranging_Sts_dev4_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.UWB_Ranging_Sts_dev4 = (x);\
}

/*  Rx */
#define IL_UWB_Ranging_Sts_dev5_RX  (BTA4_RSSI_UWB_t.msg.sig.UWB_Ranging_Sts_dev5)

/*  Tx */
#define IL_UWB_Ranging_Sts_dev5_TX(x)  \
{ \
	BTA4_RSSI_UWB_t.msg.sig.UWB_Ranging_Sts_dev5 = (x);\
}

/*  Rx */
#define IL_Checksum_RX  (UWB_Info_Ntf_t.msg.sig.Checksum)

/*  Tx */
#define IL_Checksum_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Checksum = (x);\
}

/*  Rx */
#define IL_Device_Type_RX  (UWB_Info_Ntf_t.msg.sig.Device_Type)

/*  Tx */
#define IL_Device_Type_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Device_Type = (x);\
}

/*  Rx */
#define IL_SYNC_Code_Index_RX  (UWB_Info_Ntf_t.msg.sig.SYNC_Code_Index)

/*  Tx */
#define IL_SYNC_Code_Index_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.SYNC_Code_Index = (x);\
}

/*  Rx */
#define IL_UWB_Session_Req_RX  (UWB_Info_Ntf_t.msg.sig.UWB_Session_Req)

/*  Tx */
#define IL_UWB_Session_Req_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.UWB_Session_Req = (x);\
}

/*  Rx */
#define IL_Device_Index_RX  (UWB_Info_Ntf_t.msg.sig.Device_Index)

/*  Tx */
#define IL_Device_Index_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Device_Index = (x);\
}

/*  Rx */
#define IL_Selected_PulseShape_Combo_RX  (UWB_Info_Ntf_t.msg.sig.Selected_PulseShape_Combo)

/*  Tx */
#define IL_Selected_PulseShape_Combo_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Selected_PulseShape_Combo = (x);\
}

/*  Rx */
#define IL_Selected_UWB_Channel_RX  (UWB_Info_Ntf_t.msg.sig.Selected_UWB_Channel)

/*  Tx */
#define IL_Selected_UWB_Channel_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Selected_UWB_Channel = (x);\
}

/*  Rx */
#define IL_Session_RAN_Multiplier_RX  (UWB_Info_Ntf_t.msg.sig.Session_RAN_Multiplier)

/*  Tx */
#define IL_Session_RAN_Multiplier_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Session_RAN_Multiplier = (x);\
}

/*  Rx */
#define IL_Number_Chaps_per_Slot_RX  (UWB_Info_Ntf_t.msg.sig.Number_Chaps_per_Slot)

/*  Tx */
#define IL_Number_Chaps_per_Slot_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Number_Chaps_per_Slot = (x);\
}

/*  Rx */
#define IL_Number_Responders_Nodes_RX  (UWB_Info_Ntf_t.msg.sig.Number_Responders_Nodes)

/*  Tx */
#define IL_Number_Responders_Nodes_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Number_Responders_Nodes = (x);\
}

/*  Rx */
#define IL_Number_Slots_per_Round_RX  (UWB_Info_Ntf_t.msg.sig.Number_Slots_per_Round)

/*  Tx */
#define IL_Number_Slots_per_Round_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Number_Slots_per_Round = (x);\
}

/*  Rx */
#define IL_Selected_Hopping_Config_Bitmask_RX  (UWB_Info_Ntf_t.msg.sig.Selected_Hopping_Config_Bitmask)

/*  Tx */
#define IL_Selected_Hopping_Config_Bitmask_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Selected_Hopping_Config_Bitmask = (x);\
}

/*  Rx */
#define IL_Selected_UWB_Config_Id_RX  (\
	(UWB_Info_Ntf_t.msg.sig.Selected_UWB_Config_Id_0 << 8U) | \
	(UWB_Info_Ntf_t.msg.sig.Selected_UWB_Config_Id_1))

/* Tx */
#define IL_Selected_UWB_Config_Id_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Selected_UWB_Config_Id_0 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.Selected_UWB_Config_Id_1 = (x) & 255U; \
}

/*  Rx */
#define IL_Selected_DK_Protocol_Version_RX  (\
	(UWB_Info_Ntf_t.msg.sig.Selected_DK_Protocol_Version_0 << 8U) | \
	(UWB_Info_Ntf_t.msg.sig.Selected_DK_Protocol_Version_1))

/* Tx */
#define IL_Selected_DK_Protocol_Version_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.Selected_DK_Protocol_Version_0 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.Selected_DK_Protocol_Version_1 = (x) & 255U; \
}

/*  Rx */
#define IL_UWB_Session_Id_RX  (\
	(UWB_Info_Ntf_t.msg.sig.UWB_Session_Id_0 << 24U) | \
	(UWB_Info_Ntf_t.msg.sig.UWB_Session_Id_1 << 16U) | \
	(UWB_Info_Ntf_t.msg.sig.UWB_Session_Id_2 << 8U) | \
	(UWB_Info_Ntf_t.msg.sig.UWB_Session_Id_3))

/* Tx */
#define IL_UWB_Session_Id_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.UWB_Session_Id_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.UWB_Session_Id_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.UWB_Session_Id_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.UWB_Session_Id_3 = (x) & 255U; \
}

/*  Rx */
#define IL_STS_Index0_RX  (\
	(UWB_Info_Ntf_t.msg.sig.STS_Index0_0 << 24U) | \
	(UWB_Info_Ntf_t.msg.sig.STS_Index0_1 << 16U) | \
	(UWB_Info_Ntf_t.msg.sig.STS_Index0_2 << 8U) | \
	(UWB_Info_Ntf_t.msg.sig.STS_Index0_3))

/* Tx */
#define IL_STS_Index0_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.STS_Index0_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.STS_Index0_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.STS_Index0_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.STS_Index0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_UWB_Time0_Index0_0_RX  (\
	(UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_0_0 << 24U) | \
	(UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_0_1 << 16U) | \
	(UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_0_2 << 8U) | \
	(UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_0_3))

/* Tx */
#define IL_UWB_Time0_Index0_0_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_0_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_0_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_0_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_UWB_Time0_Index0_4_RX  (\
	(UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_4_0 << 24U) | \
	(UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_4_1 << 16U) | \
	(UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_4_2 << 8U) | \
	(UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_4_3))

/* Tx */
#define IL_UWB_Time0_Index0_4_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_4_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_4_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_4_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.UWB_Time0_Index0_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_HOP_Mode_Key_RX  (\
	(UWB_Info_Ntf_t.msg.sig.HOP_Mode_Key_0 << 24U) | \
	(UWB_Info_Ntf_t.msg.sig.HOP_Mode_Key_1 << 16U) | \
	(UWB_Info_Ntf_t.msg.sig.HOP_Mode_Key_2 << 8U) | \
	(UWB_Info_Ntf_t.msg.sig.HOP_Mode_Key_3))

/* Tx */
#define IL_HOP_Mode_Key_TX(x)  \
{ \
	UWB_Info_Ntf_t.msg.sig.HOP_Mode_Key_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.HOP_Mode_Key_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.HOP_Mode_Key_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_t.msg.sig.HOP_Mode_Key_3 = (x) & 255U; \
}

/*  Rx */
#define IL_UWB_Session_Id_Sup_RX  (\
	(UWB_Info_Ntf_Sup_t.msg.sig.UWB_Session_Id_Sup_0 << 24U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.UWB_Session_Id_Sup_1 << 16U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.UWB_Session_Id_Sup_2 << 8U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.UWB_Session_Id_Sup_3))

/* Tx */
#define IL_UWB_Session_Id_Sup_TX(x)  \
{ \
	UWB_Info_Ntf_Sup_t.msg.sig.UWB_Session_Id_Sup_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.UWB_Session_Id_Sup_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.UWB_Session_Id_Sup_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.UWB_Session_Id_Sup_3 = (x) & 255U; \
}

/*  Rx */
#define IL_URSK_0_RX  (\
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_0_0 << 24U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_0_1 << 16U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_0_2 << 8U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_0_3))

/* Tx */
#define IL_URSK_0_TX(x)  \
{ \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_0_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_0_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_0_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_URSK_4_RX  (\
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_4_0 << 24U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_4_1 << 16U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_4_2 << 8U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_4_3))

/* Tx */
#define IL_URSK_4_TX(x)  \
{ \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_4_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_4_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_4_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_URSK_8_RX  (\
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_8_0 << 24U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_8_1 << 16U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_8_2 << 8U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_8_3))

/* Tx */
#define IL_URSK_8_TX(x)  \
{ \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_8_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_8_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_8_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_URSK_12_RX  (\
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_12_0 << 24U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_12_1 << 16U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_12_2 << 8U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_12_3))

/* Tx */
#define IL_URSK_12_TX(x)  \
{ \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_12_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_12_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_12_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_URSK_16_RX  (\
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_16_0 << 24U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_16_1 << 16U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_16_2 << 8U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_16_3))

/* Tx */
#define IL_URSK_16_TX(x)  \
{ \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_16_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_16_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_16_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_16_3 = (x) & 255U; \
}

/*  Rx */
#define IL_URSK_20_RX  (\
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_20_0 << 24U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_20_1 << 16U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_20_2 << 8U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_20_3))

/* Tx */
#define IL_URSK_20_TX(x)  \
{ \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_20_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_20_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_20_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_20_3 = (x) & 255U; \
}

/*  Rx */
#define IL_URSK_24_RX  (\
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_24_0 << 24U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_24_1 << 16U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_24_2 << 8U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_24_3))

/* Tx */
#define IL_URSK_24_TX(x)  \
{ \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_24_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_24_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_24_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_24_3 = (x) & 255U; \
}

/*  Rx */
#define IL_URSK_28_RX  (\
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_28_0 << 24U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_28_1 << 16U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_28_2 << 8U) | \
	(UWB_Info_Ntf_Sup_t.msg.sig.URSK_28_3))

/* Tx */
#define IL_URSK_28_TX(x)  \
{ \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_28_0 = ((x) >> 24U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_28_1 = ((x) >> 16U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_28_2 = ((x) >> 8U) & 255U; \
	UWB_Info_Ntf_Sup_t.msg.sig.URSK_28_3 = (x) & 255U; \
}

/*  Rx */
#define IL_IotHubAddress_RX  (\
	(IotHubSts_t.msg.sig.IotHubAddress_0 << 40U) | \
	(IotHubSts_t.msg.sig.IotHubAddress_1 << 32U) | \
	(IotHubSts_t.msg.sig.IotHubAddress_2 << 24U) | \
	(IotHubSts_t.msg.sig.IotHubAddress_3 << 16U) | \
	(IotHubSts_t.msg.sig.IotHubAddress_4 << 8U) | \
	(IotHubSts_t.msg.sig.IotHubAddress_5))

/* Tx */
#define IL_IotHubAddress_TX(x)  \
{ \
	IotHubSts_t.msg.sig.IotHubAddress_0 = ((x) >> 40U) & 255U; \
	IotHubSts_t.msg.sig.IotHubAddress_1 = ((x) >> 32U) & 255U; \
	IotHubSts_t.msg.sig.IotHubAddress_2 = ((x) >> 24U) & 255U; \
	IotHubSts_t.msg.sig.IotHubAddress_3 = ((x) >> 16U) & 255U; \
	IotHubSts_t.msg.sig.IotHubAddress_4 = ((x) >> 8U) & 255U; \
	IotHubSts_t.msg.sig.IotHubAddress_5 = (x) & 255U; \
}

/*  Rx */
#define IL_NetworkCapability_RX  (IotHubSts_t.msg.sig.NetworkCapability)

/*  Tx */
#define IL_NetworkCapability_TX(x)  \
{ \
	IotHubSts_t.msg.sig.NetworkCapability = (x);\
}

/*  Rx */
#define IL_ScanFlag_RX  (IotHubSts_t.msg.sig.ScanFlag)

/*  Tx */
#define IL_ScanFlag_TX(x)  \
{ \
	IotHubSts_t.msg.sig.ScanFlag = (x);\
}

/*  Rx */
#define IL_UWBLDTRstCmd_Sr_RX  (ZONE_296_t.msg.sig.UWBLDTRstCmd_Sr)

/*  Tx */
#define IL_UWBLDTRstCmd_Sr_TX(x)  \
{ \
	ZONE_296_t.msg.sig.UWBLDTRstCmd_Sr = (x);\
}

/*  Rx */
#define IL_LDTBackNoiseLrnCmd_Sr_RX  (ZONE_296_t.msg.sig.LDTBackNoiseLrnCmd_Sr)

/*  Tx */
#define IL_LDTBackNoiseLrnCmd_Sr_TX(x)  \
{ \
	ZONE_296_t.msg.sig.LDTBackNoiseLrnCmd_Sr = (x);\
}

/*  Rx */
#define IL_UWBLDTEnReq_Sr_RX  (ZONE_296_t.msg.sig.UWBLDTEnReq_Sr)

/*  Tx */
#define IL_UWBLDTEnReq_Sr_TX(x)  \
{ \
	ZONE_296_t.msg.sig.UWBLDTEnReq_Sr = (x);\
}

/*  Rx */
#define IL_LDTAftRnTimReq_Sr_phy_RX  (ZONE_296_t.msg.sig.LDTAftRnTimReq_Sr_phy)

/*  Tx */
#define IL_LDTAftRnTimReq_Sr_phy_TX(x)  \
{ \
	ZONE_296_t.msg.sig.LDTAftRnTimReq_Sr_phy = (x);\
}

/*  Rx */
#define IL_LDTDetCyclReq_Sr_RX  (ZONE_296_t.msg.sig.LDTDetCyclReq_Sr)

/*  Tx */
#define IL_LDTDetCyclReq_Sr_TX(x)  \
{ \
	ZONE_296_t.msg.sig.LDTDetCyclReq_Sr = (x);\
}

/*  Rx */
#define IL_LDTWkEnReq_Sr_RX  (ZONE_296_t.msg.sig.LDTWkEnReq_Sr)

/*  Tx */
#define IL_LDTWkEnReq_Sr_TX(x)  \
{ \
	ZONE_296_t.msg.sig.LDTWkEnReq_Sr = (x);\
}

/*  Rx */
#define IL_LDTAftRnTimRsp_Sr_phy_RX  (BTA_SRR_297_t.msg.sig.LDTAftRnTimRsp_Sr_phy)

/*  Tx */
#define IL_LDTAftRnTimRsp_Sr_phy_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.LDTAftRnTimRsp_Sr_phy = (x);\
}

/*  Rx */
#define IL_AdltBrthRt_Sr_RX  (BTA_SRR_297_t.msg.sig.AdltBrthRt_Sr)

/*  Tx */
#define IL_AdltBrthRt_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.AdltBrthRt_Sr = (x);\
}

/*  Rx */
#define IL_ChildBrthRt_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildBrthRt_Sr)

/*  Tx */
#define IL_ChildBrthRt_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildBrthRt_Sr = (x);\
}

/*  Rx */
#define IL_LDTDetCyclRsp_Sr_RX  (BTA_SRR_297_t.msg.sig.LDTDetCyclRsp_Sr)

/*  Tx */
#define IL_LDTDetCyclRsp_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.LDTDetCyclRsp_Sr = (x);\
}

/*  Rx */
#define IL_LDTWkEnRsp_Sr_RX  (BTA_SRR_297_t.msg.sig.LDTWkEnRsp_Sr)

/*  Tx */
#define IL_LDTWkEnRsp_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.LDTWkEnRsp_Sr = (x);\
}

/*  Rx */
#define IL_ChildMotnSts_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildMotnSts_Sr)

/*  Tx */
#define IL_ChildMotnSts_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildMotnSts_Sr = (x);\
}

/*  Rx */
#define IL_ChildPysclSts_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildPysclSts_Sr)

/*  Tx */
#define IL_ChildPysclSts_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildPysclSts_Sr = (x);\
}

/*  Rx */
#define IL_ChildLDTRslt_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildLDTRslt_Sr)

/*  Tx */
#define IL_ChildLDTRslt_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildLDTRslt_Sr = (x);\
}

/*  Rx */
#define IL_ChildLctnStsOval_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildLctnStsOval_Sr)

/*  Tx */
#define IL_ChildLctnStsOval_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildLctnStsOval_Sr = (x);\
}

/*  Rx */
#define IL_ChildLctnStsFL_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildLctnStsFL_Sr)

/*  Tx */
#define IL_ChildLctnStsFL_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildLctnStsFL_Sr = (x);\
}

/*  Rx */
#define IL_ChildLctnStsFR_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildLctnStsFR_Sr)

/*  Tx */
#define IL_ChildLctnStsFR_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildLctnStsFR_Sr = (x);\
}

/*  Rx */
#define IL_ChildLctnSts2dL_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildLctnSts2dL_Sr)

/*  Tx */
#define IL_ChildLctnSts2dL_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildLctnSts2dL_Sr = (x);\
}

/*  Rx */
#define IL_ChildLctnSts2dM_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildLctnSts2dM_Sr)

/*  Tx */
#define IL_ChildLctnSts2dM_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildLctnSts2dM_Sr = (x);\
}

/*  Rx */
#define IL_ChildLctnSts2dR_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildLctnSts2dR_Sr)

/*  Tx */
#define IL_ChildLctnSts2dR_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildLctnSts2dR_Sr = (x);\
}

/*  Rx */
#define IL_ChildLctnSts3dL_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildLctnSts3dL_Sr)

/*  Tx */
#define IL_ChildLctnSts3dL_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildLctnSts3dL_Sr = (x);\
}

/*  Rx */
#define IL_ChildLctnSts3dR_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildLctnSts3dR_Sr)

/*  Tx */
#define IL_ChildLctnSts3dR_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildLctnSts3dR_Sr = (x);\
}

/*  Rx */
#define IL_AdltLctnStsOval_Sr_RX  (BTA_SRR_297_t.msg.sig.AdltLctnStsOval_Sr)

/*  Tx */
#define IL_AdltLctnStsOval_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.AdltLctnStsOval_Sr = (x);\
}

/*  Rx */
#define IL_AdltLctnStsFL_Sr_RX  (BTA_SRR_297_t.msg.sig.AdltLctnStsFL_Sr)

/*  Tx */
#define IL_AdltLctnStsFL_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.AdltLctnStsFL_Sr = (x);\
}

/*  Rx */
#define IL_AdltLctnStsFR_Sr_RX  (BTA_SRR_297_t.msg.sig.AdltLctnStsFR_Sr)

/*  Tx */
#define IL_AdltLctnStsFR_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.AdltLctnStsFR_Sr = (x);\
}

/*  Rx */
#define IL_AdltLctnSts2dL_Sr_RX  (BTA_SRR_297_t.msg.sig.AdltLctnSts2dL_Sr)

/*  Tx */
#define IL_AdltLctnSts2dL_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.AdltLctnSts2dL_Sr = (x);\
}

/*  Rx */
#define IL_AdltLctnSts2dM_Sr_RX  (BTA_SRR_297_t.msg.sig.AdltLctnSts2dM_Sr)

/*  Tx */
#define IL_AdltLctnSts2dM_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.AdltLctnSts2dM_Sr = (x);\
}

/*  Rx */
#define IL_AdltLctnSts2dR_Sr_RX  (BTA_SRR_297_t.msg.sig.AdltLctnSts2dR_Sr)

/*  Tx */
#define IL_AdltLctnSts2dR_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.AdltLctnSts2dR_Sr = (x);\
}

/*  Rx */
#define IL_AdltLctnSts3dL_Sr_RX  (BTA_SRR_297_t.msg.sig.AdltLctnSts3dL_Sr)

/*  Tx */
#define IL_AdltLctnSts3dL_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.AdltLctnSts3dL_Sr = (x);\
}

/*  Rx */
#define IL_AdltLctnSts3dR_Sr_RX  (BTA_SRR_297_t.msg.sig.AdltLctnSts3dR_Sr)

/*  Tx */
#define IL_AdltLctnSts3dR_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.AdltLctnSts3dR_Sr = (x);\
}

/*  Rx */
#define IL_UWBLDTEnRsp_Sr_RX  (BTA_SRR_297_t.msg.sig.UWBLDTEnRsp_Sr)

/*  Tx */
#define IL_UWBLDTEnRsp_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.UWBLDTEnRsp_Sr = (x);\
}

/*  Rx */
#define IL_UWBLDTRstRsp_Sr_RX  (BTA_SRR_297_t.msg.sig.UWBLDTRstRsp_Sr)

/*  Tx */
#define IL_UWBLDTRstRsp_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.UWBLDTRstRsp_Sr = (x);\
}

/*  Rx */
#define IL_ChildLDTAgeRange_Sr_RX  (BTA_SRR_297_t.msg.sig.ChildLDTAgeRange_Sr)

/*  Tx */
#define IL_ChildLDTAgeRange_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.ChildLDTAgeRange_Sr = (x);\
}

/*  Rx */
#define IL_LDTBackNoiseLrnRsp_Sr_RX  (BTA_SRR_297_t.msg.sig.LDTBackNoiseLrnRsp_Sr)

/*  Tx */
#define IL_LDTBackNoiseLrnRsp_Sr_TX(x)  \
{ \
	BTA_SRR_297_t.msg.sig.LDTBackNoiseLrnRsp_Sr = (x);\
}

/*  Rx */
#define IL_SmartKey_Dev1_IRK_0_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_0_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_0_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_0_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_0_3))

/* Tx */
#define IL_SmartKey_Dev1_IRK_0_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_0_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_0_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_0_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev1_IRK_4_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_4_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_4_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_4_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_4_3))

/* Tx */
#define IL_SmartKey_Dev1_IRK_4_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_4_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_4_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_4_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev1_IRK_8_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_8_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_8_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_8_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_8_3))

/* Tx */
#define IL_SmartKey_Dev1_IRK_8_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_8_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_8_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_8_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev1_IRK_12_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_12_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_12_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_12_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_12_3))

/* Tx */
#define IL_SmartKey_Dev1_IRK_12_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_12_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_12_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_12_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev1_IRK_12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev2_IRK_0_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_0_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_0_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_0_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_0_3))

/* Tx */
#define IL_SmartKey_Dev2_IRK_0_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_0_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_0_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_0_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev2_IRK_4_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_4_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_4_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_4_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_4_3))

/* Tx */
#define IL_SmartKey_Dev2_IRK_4_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_4_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_4_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_4_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev2_IRK_8_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_8_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_8_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_8_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_8_3))

/* Tx */
#define IL_SmartKey_Dev2_IRK_8_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_8_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_8_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_8_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev2_IRK_12_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_12_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_12_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_12_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_12_3))

/* Tx */
#define IL_SmartKey_Dev2_IRK_12_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_12_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_12_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_12_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev2_IRK_12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev3_IRK_0_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_0_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_0_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_0_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_0_3))

/* Tx */
#define IL_SmartKey_Dev3_IRK_0_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_0_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_0_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_0_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev3_IRK_4_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_4_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_4_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_4_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_4_3))

/* Tx */
#define IL_SmartKey_Dev3_IRK_4_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_4_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_4_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_4_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev3_IRK_8_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_8_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_8_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_8_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_8_3))

/* Tx */
#define IL_SmartKey_Dev3_IRK_8_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_8_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_8_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_8_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev3_IRK_12_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_12_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_12_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_12_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_12_3))

/* Tx */
#define IL_SmartKey_Dev3_IRK_12_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_12_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_12_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_12_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev3_IRK_12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev4_IRK_0_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_0_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_0_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_0_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_0_3))

/* Tx */
#define IL_SmartKey_Dev4_IRK_0_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_0_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_0_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_0_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev4_IRK_4_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_4_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_4_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_4_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_4_3))

/* Tx */
#define IL_SmartKey_Dev4_IRK_4_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_4_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_4_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_4_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev4_IRK_8_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_8_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_8_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_8_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_8_3))

/* Tx */
#define IL_SmartKey_Dev4_IRK_8_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_8_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_8_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_8_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev4_IRK_12_RX  (\
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_12_0 << 24U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_12_1 << 16U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_12_2 << 8U) | \
	(Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_12_3))

/* Tx */
#define IL_SmartKey_Dev4_IRK_12_TX(x)  \
{ \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_12_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_12_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_12_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_1_t.msg.sig.SmartKey_Dev4_IRK_12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev5_IRK_0_RX  (\
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_0_0 << 24U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_0_1 << 16U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_0_2 << 8U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_0_3))

/* Tx */
#define IL_SmartKey_Dev5_IRK_0_TX(x)  \
{ \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_0_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_0_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_0_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev5_IRK_4_RX  (\
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_4_0 << 24U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_4_1 << 16U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_4_2 << 8U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_4_3))

/* Tx */
#define IL_SmartKey_Dev5_IRK_4_TX(x)  \
{ \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_4_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_4_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_4_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev5_IRK_8_RX  (\
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_8_0 << 24U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_8_1 << 16U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_8_2 << 8U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_8_3))

/* Tx */
#define IL_SmartKey_Dev5_IRK_8_TX(x)  \
{ \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_8_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_8_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_8_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmartKey_Dev5_IRK_12_RX  (\
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_12_0 << 24U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_12_1 << 16U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_12_2 << 8U) | \
	(Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_12_3))

/* Tx */
#define IL_SmartKey_Dev5_IRK_12_TX(x)  \
{ \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_12_0 = ((x) >> 24U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_12_1 = ((x) >> 16U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_12_2 = ((x) >> 8U) & 255U; \
	Device_IRK_Group_2_t.msg.sig.SmartKey_Dev5_IRK_12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rlSt_BrthEngy_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rlSt_BrthEngy_Sr)

/*  Tx */
#define IL_FusnData_2rlSt_BrthEngy_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlSt_BrthEngy_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rlSt_AdltBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rlSt_AdltBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rlSt_AdltBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlSt_AdltBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rlSt_ChildBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rlSt_ChildBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rlSt_ChildBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlSt_ChildBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rlSt_Reserved_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rlSt_Reserved_Sr)

/*  Tx */
#define IL_FusnData_2rlSt_Reserved_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlSt_Reserved_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rmSt_BrthEngy_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rmSt_BrthEngy_Sr)

/*  Tx */
#define IL_FusnData_2rmSt_BrthEngy_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmSt_BrthEngy_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rmSt_AdltBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rmSt_AdltBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rmSt_AdltBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmSt_AdltBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rmSt_ChildBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rmSt_ChildBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rmSt_ChildBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmSt_ChildBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rmSt_Reserved_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rmSt_Reserved_Sr)

/*  Tx */
#define IL_FusnData_2rmSt_Reserved_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmSt_Reserved_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rrSt_BrthEngy_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rrSt_BrthEngy_Sr)

/*  Tx */
#define IL_FusnData_2rrSt_BrthEngy_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrSt_BrthEngy_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rrSt_AdltBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rrSt_AdltBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rrSt_AdltBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrSt_AdltBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rrSt_ChildBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rrSt_ChildBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rrSt_ChildBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrSt_ChildBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rrSt_Reserved_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rrSt_Reserved_Sr)

/*  Tx */
#define IL_FusnData_2rrSt_Reserved_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrSt_Reserved_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rlFt_BrthEngy_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rlFt_BrthEngy_Sr)

/*  Tx */
#define IL_FusnData_2rlFt_BrthEngy_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlFt_BrthEngy_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rlFt_AdltBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rlFt_AdltBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rlFt_AdltBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlFt_AdltBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rlFt_ChildBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rlFt_ChildBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rlFt_ChildBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlFt_ChildBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rlFt_Reserved_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rlFt_Reserved_Sr)

/*  Tx */
#define IL_FusnData_2rlFt_Reserved_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlFt_Reserved_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rmFt_BrthEngy_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rmFt_BrthEngy_Sr)

/*  Tx */
#define IL_FusnData_2rmFt_BrthEngy_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmFt_BrthEngy_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rmFt_AdltBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rmFt_AdltBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rmFt_AdltBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmFt_AdltBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rmFt_ChildBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rmFt_ChildBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rmFt_ChildBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmFt_ChildBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rmFt_Reserved_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rmFt_Reserved_Sr)

/*  Tx */
#define IL_FusnData_2rmFt_Reserved_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmFt_Reserved_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rrFt_BrthEngy_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rrFt_BrthEngy_Sr)

/*  Tx */
#define IL_FusnData_2rrFt_BrthEngy_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrFt_BrthEngy_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rrFt_AdltBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rrFt_AdltBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rrFt_AdltBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrFt_AdltBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rrFt_ChildBrthRate_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rrFt_ChildBrthRate_Sr)

/*  Tx */
#define IL_FusnData_2rrFt_ChildBrthRate_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrFt_ChildBrthRate_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rrFt_Reserved_Sr_RX  (BTA4_FusnData_t.msg.sig.FusnData_2rrFt_Reserved_Sr)

/*  Tx */
#define IL_FusnData_2rrFt_Reserved_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrFt_Reserved_Sr = (x);\
}

/*  Rx */
#define IL_FusnData_2rlSt_AdltDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rlSt_AdltDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rlSt_AdltDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rlSt_AdltDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlSt_AdltDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rlSt_AdltDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rlSt_ChildDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rlSt_ChildDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rlSt_ChildDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rlSt_ChildDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlSt_ChildDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rlSt_ChildDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rmSt_AdltDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rmSt_AdltDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rmSt_AdltDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rmSt_AdltDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmSt_AdltDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rmSt_AdltDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rmSt_ChildDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rmSt_ChildDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rmSt_ChildDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rmSt_ChildDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmSt_ChildDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rmSt_ChildDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rrSt_AdltDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rrSt_AdltDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rrSt_AdltDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rrSt_AdltDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrSt_AdltDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rrSt_AdltDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rrSt_ChildDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rrSt_ChildDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rrSt_ChildDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rrSt_ChildDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrSt_ChildDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rrSt_ChildDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rlFt_AdltDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rlFt_AdltDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rlFt_AdltDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rlFt_AdltDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlFt_AdltDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rlFt_AdltDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rlFt_ChildDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rlFt_ChildDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rlFt_ChildDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rlFt_ChildDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rlFt_ChildDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rlFt_ChildDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rmFt_AdltDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rmFt_AdltDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rmFt_AdltDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rmFt_AdltDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmFt_AdltDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rmFt_AdltDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rmFt_ChildDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rmFt_ChildDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rmFt_ChildDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rmFt_ChildDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rmFt_ChildDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rmFt_ChildDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rrFt_AdltDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rrFt_AdltDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rrFt_AdltDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rrFt_AdltDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrFt_AdltDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rrFt_AdltDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData_2rrFt_ChildDistance_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData_2rrFt_ChildDistance_Sr_0 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData_2rrFt_ChildDistance_Sr_1))

/* Tx */
#define IL_FusnData_2rrFt_ChildDistance_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData_2rrFt_ChildDistance_Sr_0 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData_2rrFt_ChildDistance_Sr_1 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData__Reserved1_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_0 << 56U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_1 << 48U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_2 << 40U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_3 << 32U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_4 << 24U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_5 << 16U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_6 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_7))

/* Tx */
#define IL_FusnData__Reserved1_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_0 = ((x) >> 56U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_1 = ((x) >> 48U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_2 = ((x) >> 40U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_3 = ((x) >> 32U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_4 = ((x) >> 24U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_5 = ((x) >> 16U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_6 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved1_Sr_7 = (x) & 255U; \
}

/*  Rx */
#define IL_FusnData__Reserved2_Sr_RX  (\
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_0 << 56U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_1 << 48U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_2 << 40U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_3 << 32U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_4 << 24U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_5 << 16U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_6 << 8U) | \
	(BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_7))

/* Tx */
#define IL_FusnData__Reserved2_Sr_TX(x)  \
{ \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_0 = ((x) >> 56U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_1 = ((x) >> 48U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_2 = ((x) >> 40U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_3 = ((x) >> 32U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_4 = ((x) >> 24U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_5 = ((x) >> 16U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_6 = ((x) >> 8U) & 255U; \
	BTA4_FusnData_t.msg.sig.FusnData__Reserved2_Sr_7 = (x) & 255U; \
}

/*  Rx */
#define IL_VehOdometer_RX  (\
	(ZONE_3AA_t.msg.sig.VehOdometer_0 << 16U) | \
	(ZONE_3AA_t.msg.sig.VehOdometer_1 << 8U) | \
	(ZONE_3AA_t.msg.sig.VehOdometer_2))

/* Tx */
#define IL_VehOdometer_TX(x)  \
{ \
	ZONE_3AA_t.msg.sig.VehOdometer_0 = ((x) >> 16U) & 255U; \
	ZONE_3AA_t.msg.sig.VehOdometer_1 = ((x) >> 8U) & 255U; \
	ZONE_3AA_t.msg.sig.VehOdometer_2 = (x) & 255U; \
}

/*  Rx */
#define IL_Yr_phy_RX  (ZONE_3B2_t.msg.sig.Yr_phy)

/*  Tx */
#define IL_Yr_phy_TX(x)  \
{ \
	ZONE_3B2_t.msg.sig.Yr_phy = (x);\
}

/*  Rx */
#define IL_Mth_RX  (ZONE_3B2_t.msg.sig.Mth)

/*  Tx */
#define IL_Mth_TX(x)  \
{ \
	ZONE_3B2_t.msg.sig.Mth = (x);\
}

/*  Rx */
#define IL_Day_RX  (ZONE_3B2_t.msg.sig.Day)

/*  Tx */
#define IL_Day_TX(x)  \
{ \
	ZONE_3B2_t.msg.sig.Day = (x);\
}

/*  Rx */
#define IL_Hr_RX  (ZONE_3B2_t.msg.sig.Hr)

/*  Tx */
#define IL_Hr_TX(x)  \
{ \
	ZONE_3B2_t.msg.sig.Hr = (x);\
}

/*  Rx */
#define IL_Min_RX  (ZONE_3B2_t.msg.sig.Min)

/*  Tx */
#define IL_Min_TX(x)  \
{ \
	ZONE_3B2_t.msg.sig.Min = (x);\
}

/*  Rx */
#define IL_Sec_RX  (ZONE_3B2_t.msg.sig.Sec)

/*  Tx */
#define IL_Sec_TX(x)  \
{ \
	ZONE_3B2_t.msg.sig.Sec = (x);\
}

/*  Rx */
#define IL_Msec_RX  (\
	(ZONE_3B2_t.msg.sig.Msec_0 << 8U) | \
	(ZONE_3B2_t.msg.sig.Msec_1))

/* Tx */
#define IL_Msec_TX(x)  \
{ \
	ZONE_3B2_t.msg.sig.Msec_0 = ((x) >> 8U) & 3U; \
	ZONE_3B2_t.msg.sig.Msec_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Level_RX  (BTA4_Log_Data_t.msg.sig.BTA4_Log_Level)

/*  Tx */
#define IL_BTA4_Log_Level_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Level = (x);\
}

/*  Rx */
#define IL_BTA4_Log_Sender_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Sender_0 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Sender_1 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Sender_2))

/* Tx */
#define IL_BTA4_Log_Sender_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Sender_0 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Sender_1 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Sender_2 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data1_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data1_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data1_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data1_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data1_3))

/* Tx */
#define IL_BTA4_Log_Data1_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data1_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data1_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data1_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data2_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data2_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data2_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data2_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data2_3))

/* Tx */
#define IL_BTA4_Log_Data2_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data2_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data2_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data2_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data2_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data3_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data3_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data3_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data3_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data3_3))

/* Tx */
#define IL_BTA4_Log_Data3_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data3_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data3_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data3_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data4_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data4_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data4_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data4_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data4_3))

/* Tx */
#define IL_BTA4_Log_Data4_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data4_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data4_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data4_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data5_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data5_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data5_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data5_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data5_3))

/* Tx */
#define IL_BTA4_Log_Data5_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data5_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data5_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data5_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data5_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data6_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data6_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data6_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data6_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data6_3))

/* Tx */
#define IL_BTA4_Log_Data6_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data6_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data6_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data6_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data6_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data7_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data7_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data7_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data7_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data7_3))

/* Tx */
#define IL_BTA4_Log_Data7_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data7_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data7_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data7_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data7_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data8_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data8_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data8_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data8_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data8_3))

/* Tx */
#define IL_BTA4_Log_Data8_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data8_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data8_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data8_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data9_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data9_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data9_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data9_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data9_3))

/* Tx */
#define IL_BTA4_Log_Data9_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data9_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data9_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data9_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data9_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data10_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data10_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data10_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data10_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data10_3))

/* Tx */
#define IL_BTA4_Log_Data10_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data10_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data10_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data10_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data10_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data11_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data11_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data11_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data11_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data11_3))

/* Tx */
#define IL_BTA4_Log_Data11_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data11_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data11_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data11_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data11_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data12_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data12_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data12_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data12_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data12_3))

/* Tx */
#define IL_BTA4_Log_Data12_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data12_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data12_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data12_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data13_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data13_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data13_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data13_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data13_3))

/* Tx */
#define IL_BTA4_Log_Data13_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data13_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data13_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data13_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data13_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data14_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data14_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data14_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data14_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data14_3))

/* Tx */
#define IL_BTA4_Log_Data14_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data14_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data14_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data14_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data14_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA4_Log_Data15_RX  (\
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data15_0 << 24U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data15_1 << 16U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data15_2 << 8U) | \
	(BTA4_Log_Data_t.msg.sig.BTA4_Log_Data15_3))

/* Tx */
#define IL_BTA4_Log_Data15_TX(x)  \
{ \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data15_0 = ((x) >> 24U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data15_1 = ((x) >> 16U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data15_2 = ((x) >> 8U) & 255U; \
	BTA4_Log_Data_t.msg.sig.BTA4_Log_Data15_3 = (x) & 255U; \
}

/*  Rx */
#define IL_ScanDeviceAddr_RX  (\
	(IotConnectSts_t.msg.sig.ScanDeviceAddr_0 << 40U) | \
	(IotConnectSts_t.msg.sig.ScanDeviceAddr_1 << 32U) | \
	(IotConnectSts_t.msg.sig.ScanDeviceAddr_2 << 24U) | \
	(IotConnectSts_t.msg.sig.ScanDeviceAddr_3 << 16U) | \
	(IotConnectSts_t.msg.sig.ScanDeviceAddr_4 << 8U) | \
	(IotConnectSts_t.msg.sig.ScanDeviceAddr_5))

/* Tx */
#define IL_ScanDeviceAddr_TX(x)  \
{ \
	IotConnectSts_t.msg.sig.ScanDeviceAddr_0 = ((x) >> 40U) & 255U; \
	IotConnectSts_t.msg.sig.ScanDeviceAddr_1 = ((x) >> 32U) & 255U; \
	IotConnectSts_t.msg.sig.ScanDeviceAddr_2 = ((x) >> 24U) & 255U; \
	IotConnectSts_t.msg.sig.ScanDeviceAddr_3 = ((x) >> 16U) & 255U; \
	IotConnectSts_t.msg.sig.ScanDeviceAddr_4 = ((x) >> 8U) & 255U; \
	IotConnectSts_t.msg.sig.ScanDeviceAddr_5 = (x) & 255U; \
}

/*  Rx */
#define IL_ScanBroadcastData_0_RX  (\
	(IotConnectSts_t.msg.sig.ScanBroadcastData_0_0 << 56U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_0_1 << 48U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_0_2 << 40U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_0_3 << 32U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_0_4 << 24U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_0_5 << 16U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_0_6 << 8U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_0_7))

/* Tx */
#define IL_ScanBroadcastData_0_TX(x)  \
{ \
	IotConnectSts_t.msg.sig.ScanBroadcastData_0_0 = ((x) >> 56U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_0_1 = ((x) >> 48U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_0_2 = ((x) >> 40U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_0_3 = ((x) >> 32U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_0_4 = ((x) >> 24U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_0_5 = ((x) >> 16U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_0_6 = ((x) >> 8U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_ScanBroadcastData_1_RX  (\
	(IotConnectSts_t.msg.sig.ScanBroadcastData_1_0 << 56U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_1_1 << 48U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_1_2 << 40U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_1_3 << 32U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_1_4 << 24U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_1_5 << 16U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_1_6 << 8U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_1_7))

/* Tx */
#define IL_ScanBroadcastData_1_TX(x)  \
{ \
	IotConnectSts_t.msg.sig.ScanBroadcastData_1_0 = ((x) >> 56U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_1_1 = ((x) >> 48U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_1_2 = ((x) >> 40U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_1_3 = ((x) >> 32U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_1_4 = ((x) >> 24U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_1_5 = ((x) >> 16U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_1_6 = ((x) >> 8U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_ScanBroadcastData_2_RX  (\
	(IotConnectSts_t.msg.sig.ScanBroadcastData_2_0 << 56U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_2_1 << 48U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_2_2 << 40U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_2_3 << 32U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_2_4 << 24U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_2_5 << 16U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_2_6 << 8U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_2_7))

/* Tx */
#define IL_ScanBroadcastData_2_TX(x)  \
{ \
	IotConnectSts_t.msg.sig.ScanBroadcastData_2_0 = ((x) >> 56U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_2_1 = ((x) >> 48U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_2_2 = ((x) >> 40U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_2_3 = ((x) >> 32U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_2_4 = ((x) >> 24U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_2_5 = ((x) >> 16U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_2_6 = ((x) >> 8U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_ScanBroadcastData_3_RX  (\
	(IotConnectSts_t.msg.sig.ScanBroadcastData_3_0 << 48U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_3_1 << 40U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_3_2 << 32U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_3_3 << 24U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_3_4 << 16U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_3_5 << 8U) | \
	(IotConnectSts_t.msg.sig.ScanBroadcastData_3_6))

/* Tx */
#define IL_ScanBroadcastData_3_TX(x)  \
{ \
	IotConnectSts_t.msg.sig.ScanBroadcastData_3_0 = ((x) >> 48U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_3_1 = ((x) >> 40U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_3_2 = ((x) >> 32U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_3_3 = ((x) >> 24U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_3_4 = ((x) >> 16U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_3_5 = ((x) >> 8U) & 255U; \
	IotConnectSts_t.msg.sig.ScanBroadcastData_3_6 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnectStsDeviceAddr_RX  (\
	(IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_0 << 40U) | \
	(IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_1 << 32U) | \
	(IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_2 << 24U) | \
	(IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_3 << 16U) | \
	(IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_4 << 8U) | \
	(IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_5))

/* Tx */
#define IL_ConnectStsDeviceAddr_TX(x)  \
{ \
	IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_0 = ((x) >> 40U) & 255U; \
	IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_1 = ((x) >> 32U) & 255U; \
	IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_2 = ((x) >> 24U) & 255U; \
	IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_3 = ((x) >> 16U) & 255U; \
	IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_4 = ((x) >> 8U) & 255U; \
	IotConnectSts_t.msg.sig.ConnectStsDeviceAddr_5 = (x) & 255U; \
}

/*  Rx */
#define IL_ConnectStsMtu_RX  (\
	(IotConnectSts_t.msg.sig.ConnectStsMtu_0 << 4U) | \
	(IotConnectSts_t.msg.sig.ConnectStsMtu_0))

/* Tx */
#define IL_ConnectStsMtu_TX(x)  \
{ \
	IotConnectSts_t.msg.sig.ConnectStsMtu_0 = ((x) >> 4U) & 255U; \
	IotConnectSts_t.msg.sig.ConnectStsMtu_0 = (x) & 15U; \
}

/*  Rx */
#define IL_ConnectSts_RX  (IotConnectSts_t.msg.sig.ConnectSts)

/*  Tx */
#define IL_ConnectSts_TX(x)  \
{ \
	IotConnectSts_t.msg.sig.ConnectSts = (x);\
}

/*  Rx */
#define IL_IotDownLinkData_0_RX  (\
	(IotDownLinkData_t.msg.sig.IotDownLinkData_0_0 << 56U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_0_1 << 48U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_0_2 << 40U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_0_3 << 32U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_0_4 << 24U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_0_5 << 16U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_0_6 << 8U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_0_7))

/* Tx */
#define IL_IotDownLinkData_0_TX(x)  \
{ \
	IotDownLinkData_t.msg.sig.IotDownLinkData_0_0 = ((x) >> 56U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_0_1 = ((x) >> 48U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_0_2 = ((x) >> 40U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_0_3 = ((x) >> 32U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_0_4 = ((x) >> 24U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_0_5 = ((x) >> 16U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_0_6 = ((x) >> 8U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkData_1_RX  (\
	(IotDownLinkData_t.msg.sig.IotDownLinkData_1_0 << 56U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_1_1 << 48U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_1_2 << 40U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_1_3 << 32U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_1_4 << 24U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_1_5 << 16U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_1_6 << 8U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_1_7))

/* Tx */
#define IL_IotDownLinkData_1_TX(x)  \
{ \
	IotDownLinkData_t.msg.sig.IotDownLinkData_1_0 = ((x) >> 56U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_1_1 = ((x) >> 48U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_1_2 = ((x) >> 40U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_1_3 = ((x) >> 32U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_1_4 = ((x) >> 24U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_1_5 = ((x) >> 16U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_1_6 = ((x) >> 8U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkData_2_RX  (\
	(IotDownLinkData_t.msg.sig.IotDownLinkData_2_0 << 56U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_2_1 << 48U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_2_2 << 40U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_2_3 << 32U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_2_4 << 24U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_2_5 << 16U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_2_6 << 8U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_2_7))

/* Tx */
#define IL_IotDownLinkData_2_TX(x)  \
{ \
	IotDownLinkData_t.msg.sig.IotDownLinkData_2_0 = ((x) >> 56U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_2_1 = ((x) >> 48U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_2_2 = ((x) >> 40U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_2_3 = ((x) >> 32U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_2_4 = ((x) >> 24U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_2_5 = ((x) >> 16U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_2_6 = ((x) >> 8U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkData_3_RX  (\
	(IotDownLinkData_t.msg.sig.IotDownLinkData_3_0 << 56U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_3_1 << 48U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_3_2 << 40U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_3_3 << 32U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_3_4 << 24U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_3_5 << 16U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_3_6 << 8U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_3_7))

/* Tx */
#define IL_IotDownLinkData_3_TX(x)  \
{ \
	IotDownLinkData_t.msg.sig.IotDownLinkData_3_0 = ((x) >> 56U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_3_1 = ((x) >> 48U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_3_2 = ((x) >> 40U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_3_3 = ((x) >> 32U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_3_4 = ((x) >> 24U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_3_5 = ((x) >> 16U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_3_6 = ((x) >> 8U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkData_4_RX  (\
	(IotDownLinkData_t.msg.sig.IotDownLinkData_4_0 << 56U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_4_1 << 48U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_4_2 << 40U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_4_3 << 32U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_4_4 << 24U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_4_5 << 16U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_4_6 << 8U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_4_7))

/* Tx */
#define IL_IotDownLinkData_4_TX(x)  \
{ \
	IotDownLinkData_t.msg.sig.IotDownLinkData_4_0 = ((x) >> 56U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_4_1 = ((x) >> 48U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_4_2 = ((x) >> 40U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_4_3 = ((x) >> 32U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_4_4 = ((x) >> 24U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_4_5 = ((x) >> 16U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_4_6 = ((x) >> 8U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkData_5_RX  (\
	(IotDownLinkData_t.msg.sig.IotDownLinkData_5_0 << 56U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_5_1 << 48U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_5_2 << 40U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_5_3 << 32U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_5_4 << 24U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_5_5 << 16U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_5_6 << 8U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_5_7))

/* Tx */
#define IL_IotDownLinkData_5_TX(x)  \
{ \
	IotDownLinkData_t.msg.sig.IotDownLinkData_5_0 = ((x) >> 56U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_5_1 = ((x) >> 48U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_5_2 = ((x) >> 40U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_5_3 = ((x) >> 32U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_5_4 = ((x) >> 24U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_5_5 = ((x) >> 16U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_5_6 = ((x) >> 8U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkData_6_RX  (\
	(IotDownLinkData_t.msg.sig.IotDownLinkData_6_0 << 56U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_6_1 << 48U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_6_2 << 40U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_6_3 << 32U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_6_4 << 24U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_6_5 << 16U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_6_6 << 8U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_6_7))

/* Tx */
#define IL_IotDownLinkData_6_TX(x)  \
{ \
	IotDownLinkData_t.msg.sig.IotDownLinkData_6_0 = ((x) >> 56U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_6_1 = ((x) >> 48U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_6_2 = ((x) >> 40U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_6_3 = ((x) >> 32U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_6_4 = ((x) >> 24U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_6_5 = ((x) >> 16U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_6_6 = ((x) >> 8U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkData_7_RX  (\
	(IotDownLinkData_t.msg.sig.IotDownLinkData_7_0 << 56U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_7_1 << 48U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_7_2 << 40U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_7_3 << 32U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_7_4 << 24U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_7_5 << 16U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_7_6 << 8U) | \
	(IotDownLinkData_t.msg.sig.IotDownLinkData_7_7))

/* Tx */
#define IL_IotDownLinkData_7_TX(x)  \
{ \
	IotDownLinkData_t.msg.sig.IotDownLinkData_7_0 = ((x) >> 56U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_7_1 = ((x) >> 48U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_7_2 = ((x) >> 40U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_7_3 = ((x) >> 32U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_7_4 = ((x) >> 24U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_7_5 = ((x) >> 16U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_7_6 = ((x) >> 8U) & 255U; \
	IotDownLinkData_t.msg.sig.IotDownLinkData_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUpLinkFlowCtrl_0_RX  (\
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_0 << 56U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_1 << 48U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_2 << 40U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_3 << 32U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_4 << 24U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_5 << 16U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_6 << 8U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_7))

/* Tx */
#define IL_IotUpLinkFlowCtrl_0_TX(x)  \
{ \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_0 = ((x) >> 56U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_1 = ((x) >> 48U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_2 = ((x) >> 40U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_3 = ((x) >> 32U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_4 = ((x) >> 24U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_5 = ((x) >> 16U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_6 = ((x) >> 8U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUpLinkFlowCtrl_1_RX  (\
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_0 << 56U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_1 << 48U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_2 << 40U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_3 << 32U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_4 << 24U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_5 << 16U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_6 << 8U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_7))

/* Tx */
#define IL_IotUpLinkFlowCtrl_1_TX(x)  \
{ \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_0 = ((x) >> 56U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_1 = ((x) >> 48U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_2 = ((x) >> 40U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_3 = ((x) >> 32U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_4 = ((x) >> 24U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_5 = ((x) >> 16U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_6 = ((x) >> 8U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUpLinkFlowCtrl_2_RX  (\
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_0 << 56U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_1 << 48U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_2 << 40U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_3 << 32U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_4 << 24U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_5 << 16U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_6 << 8U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_7))

/* Tx */
#define IL_IotUpLinkFlowCtrl_2_TX(x)  \
{ \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_0 = ((x) >> 56U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_1 = ((x) >> 48U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_2 = ((x) >> 40U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_3 = ((x) >> 32U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_4 = ((x) >> 24U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_5 = ((x) >> 16U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_6 = ((x) >> 8U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUpLinkFlowCtrl_3_RX  (\
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_0 << 56U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_1 << 48U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_2 << 40U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_3 << 32U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_4 << 24U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_5 << 16U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_6 << 8U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_7))

/* Tx */
#define IL_IotUpLinkFlowCtrl_3_TX(x)  \
{ \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_0 = ((x) >> 56U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_1 = ((x) >> 48U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_2 = ((x) >> 40U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_3 = ((x) >> 32U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_4 = ((x) >> 24U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_5 = ((x) >> 16U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_6 = ((x) >> 8U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUpLinkFlowCtrl_4_RX  (\
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_0 << 56U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_1 << 48U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_2 << 40U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_3 << 32U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_4 << 24U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_5 << 16U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_6 << 8U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_7))

/* Tx */
#define IL_IotUpLinkFlowCtrl_4_TX(x)  \
{ \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_0 = ((x) >> 56U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_1 = ((x) >> 48U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_2 = ((x) >> 40U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_3 = ((x) >> 32U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_4 = ((x) >> 24U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_5 = ((x) >> 16U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_6 = ((x) >> 8U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUpLinkFlowCtrl_5_RX  (\
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_0 << 56U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_1 << 48U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_2 << 40U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_3 << 32U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_4 << 24U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_5 << 16U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_6 << 8U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_7))

/* Tx */
#define IL_IotUpLinkFlowCtrl_5_TX(x)  \
{ \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_0 = ((x) >> 56U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_1 = ((x) >> 48U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_2 = ((x) >> 40U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_3 = ((x) >> 32U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_4 = ((x) >> 24U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_5 = ((x) >> 16U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_6 = ((x) >> 8U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUpLinkFlowCtrl_6_RX  (\
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_0 << 56U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_1 << 48U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_2 << 40U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_3 << 32U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_4 << 24U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_5 << 16U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_6 << 8U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_7))

/* Tx */
#define IL_IotUpLinkFlowCtrl_6_TX(x)  \
{ \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_0 = ((x) >> 56U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_1 = ((x) >> 48U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_2 = ((x) >> 40U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_3 = ((x) >> 32U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_4 = ((x) >> 24U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_5 = ((x) >> 16U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_6 = ((x) >> 8U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUpLinkFlowCtrl_7_RX  (\
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_0 << 56U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_1 << 48U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_2 << 40U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_3 << 32U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_4 << 24U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_5 << 16U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_6 << 8U) | \
	(IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_7))

/* Tx */
#define IL_IotUpLinkFlowCtrl_7_TX(x)  \
{ \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_0 = ((x) >> 56U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_1 = ((x) >> 48U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_2 = ((x) >> 40U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_3 = ((x) >> 32U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_4 = ((x) >> 24U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_5 = ((x) >> 16U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_6 = ((x) >> 8U) & 255U; \
	IotUpLinkFlowCtrl_t.msg.sig.IotUpLinkFlowCtrl_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUplinkData_0_RX  (\
	(IotUplinkData_t.msg.sig.IotUplinkData_0_0 << 56U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_0_1 << 48U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_0_2 << 40U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_0_3 << 32U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_0_4 << 24U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_0_5 << 16U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_0_6 << 8U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_0_7))

/* Tx */
#define IL_IotUplinkData_0_TX(x)  \
{ \
	IotUplinkData_t.msg.sig.IotUplinkData_0_0 = ((x) >> 56U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_0_1 = ((x) >> 48U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_0_2 = ((x) >> 40U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_0_3 = ((x) >> 32U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_0_4 = ((x) >> 24U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_0_5 = ((x) >> 16U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_0_6 = ((x) >> 8U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUplinkData_1_RX  (\
	(IotUplinkData_t.msg.sig.IotUplinkData_1_0 << 56U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_1_1 << 48U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_1_2 << 40U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_1_3 << 32U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_1_4 << 24U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_1_5 << 16U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_1_6 << 8U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_1_7))

/* Tx */
#define IL_IotUplinkData_1_TX(x)  \
{ \
	IotUplinkData_t.msg.sig.IotUplinkData_1_0 = ((x) >> 56U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_1_1 = ((x) >> 48U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_1_2 = ((x) >> 40U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_1_3 = ((x) >> 32U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_1_4 = ((x) >> 24U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_1_5 = ((x) >> 16U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_1_6 = ((x) >> 8U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUplinkData_2_RX  (\
	(IotUplinkData_t.msg.sig.IotUplinkData_2_0 << 56U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_2_1 << 48U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_2_2 << 40U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_2_3 << 32U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_2_4 << 24U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_2_5 << 16U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_2_6 << 8U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_2_7))

/* Tx */
#define IL_IotUplinkData_2_TX(x)  \
{ \
	IotUplinkData_t.msg.sig.IotUplinkData_2_0 = ((x) >> 56U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_2_1 = ((x) >> 48U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_2_2 = ((x) >> 40U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_2_3 = ((x) >> 32U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_2_4 = ((x) >> 24U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_2_5 = ((x) >> 16U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_2_6 = ((x) >> 8U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUplinkData_3_RX  (\
	(IotUplinkData_t.msg.sig.IotUplinkData_3_0 << 56U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_3_1 << 48U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_3_2 << 40U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_3_3 << 32U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_3_4 << 24U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_3_5 << 16U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_3_6 << 8U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_3_7))

/* Tx */
#define IL_IotUplinkData_3_TX(x)  \
{ \
	IotUplinkData_t.msg.sig.IotUplinkData_3_0 = ((x) >> 56U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_3_1 = ((x) >> 48U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_3_2 = ((x) >> 40U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_3_3 = ((x) >> 32U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_3_4 = ((x) >> 24U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_3_5 = ((x) >> 16U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_3_6 = ((x) >> 8U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUplinkData_4_RX  (\
	(IotUplinkData_t.msg.sig.IotUplinkData_4_0 << 56U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_4_1 << 48U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_4_2 << 40U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_4_3 << 32U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_4_4 << 24U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_4_5 << 16U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_4_6 << 8U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_4_7))

/* Tx */
#define IL_IotUplinkData_4_TX(x)  \
{ \
	IotUplinkData_t.msg.sig.IotUplinkData_4_0 = ((x) >> 56U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_4_1 = ((x) >> 48U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_4_2 = ((x) >> 40U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_4_3 = ((x) >> 32U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_4_4 = ((x) >> 24U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_4_5 = ((x) >> 16U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_4_6 = ((x) >> 8U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUplinkData_5_RX  (\
	(IotUplinkData_t.msg.sig.IotUplinkData_5_0 << 56U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_5_1 << 48U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_5_2 << 40U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_5_3 << 32U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_5_4 << 24U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_5_5 << 16U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_5_6 << 8U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_5_7))

/* Tx */
#define IL_IotUplinkData_5_TX(x)  \
{ \
	IotUplinkData_t.msg.sig.IotUplinkData_5_0 = ((x) >> 56U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_5_1 = ((x) >> 48U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_5_2 = ((x) >> 40U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_5_3 = ((x) >> 32U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_5_4 = ((x) >> 24U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_5_5 = ((x) >> 16U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_5_6 = ((x) >> 8U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUplinkData_6_RX  (\
	(IotUplinkData_t.msg.sig.IotUplinkData_6_0 << 56U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_6_1 << 48U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_6_2 << 40U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_6_3 << 32U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_6_4 << 24U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_6_5 << 16U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_6_6 << 8U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_6_7))

/* Tx */
#define IL_IotUplinkData_6_TX(x)  \
{ \
	IotUplinkData_t.msg.sig.IotUplinkData_6_0 = ((x) >> 56U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_6_1 = ((x) >> 48U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_6_2 = ((x) >> 40U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_6_3 = ((x) >> 32U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_6_4 = ((x) >> 24U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_6_5 = ((x) >> 16U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_6_6 = ((x) >> 8U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotUplinkData_7_RX  (\
	(IotUplinkData_t.msg.sig.IotUplinkData_7_0 << 56U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_7_1 << 48U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_7_2 << 40U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_7_3 << 32U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_7_4 << 24U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_7_5 << 16U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_7_6 << 8U) | \
	(IotUplinkData_t.msg.sig.IotUplinkData_7_7))

/* Tx */
#define IL_IotUplinkData_7_TX(x)  \
{ \
	IotUplinkData_t.msg.sig.IotUplinkData_7_0 = ((x) >> 56U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_7_1 = ((x) >> 48U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_7_2 = ((x) >> 40U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_7_3 = ((x) >> 32U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_7_4 = ((x) >> 24U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_7_5 = ((x) >> 16U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_7_6 = ((x) >> 8U) & 255U; \
	IotUplinkData_t.msg.sig.IotUplinkData_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkFlowCtrl_0_RX  (\
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_0 << 56U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_1 << 48U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_2 << 40U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_3 << 32U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_4 << 24U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_5 << 16U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_6 << 8U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_7))

/* Tx */
#define IL_IotDownLinkFlowCtrl_0_TX(x)  \
{ \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_0 = ((x) >> 56U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_1 = ((x) >> 48U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_2 = ((x) >> 40U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_3 = ((x) >> 32U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_4 = ((x) >> 24U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_5 = ((x) >> 16U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_6 = ((x) >> 8U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkFlowCtrl_1_RX  (\
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_0 << 56U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_1 << 48U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_2 << 40U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_3 << 32U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_4 << 24U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_5 << 16U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_6 << 8U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_7))

/* Tx */
#define IL_IotDownLinkFlowCtrl_1_TX(x)  \
{ \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_0 = ((x) >> 56U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_1 = ((x) >> 48U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_2 = ((x) >> 40U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_3 = ((x) >> 32U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_4 = ((x) >> 24U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_5 = ((x) >> 16U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_6 = ((x) >> 8U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkFlowCtrl_2_RX  (\
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_0 << 56U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_1 << 48U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_2 << 40U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_3 << 32U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_4 << 24U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_5 << 16U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_6 << 8U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_7))

/* Tx */
#define IL_IotDownLinkFlowCtrl_2_TX(x)  \
{ \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_0 = ((x) >> 56U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_1 = ((x) >> 48U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_2 = ((x) >> 40U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_3 = ((x) >> 32U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_4 = ((x) >> 24U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_5 = ((x) >> 16U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_6 = ((x) >> 8U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkFlowCtrl_3_RX  (\
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_0 << 56U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_1 << 48U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_2 << 40U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_3 << 32U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_4 << 24U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_5 << 16U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_6 << 8U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_7))

/* Tx */
#define IL_IotDownLinkFlowCtrl_3_TX(x)  \
{ \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_0 = ((x) >> 56U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_1 = ((x) >> 48U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_2 = ((x) >> 40U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_3 = ((x) >> 32U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_4 = ((x) >> 24U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_5 = ((x) >> 16U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_6 = ((x) >> 8U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkFlowCtrl_4_RX  (\
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_0 << 56U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_1 << 48U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_2 << 40U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_3 << 32U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_4 << 24U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_5 << 16U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_6 << 8U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_7))

/* Tx */
#define IL_IotDownLinkFlowCtrl_4_TX(x)  \
{ \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_0 = ((x) >> 56U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_1 = ((x) >> 48U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_2 = ((x) >> 40U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_3 = ((x) >> 32U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_4 = ((x) >> 24U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_5 = ((x) >> 16U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_6 = ((x) >> 8U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkFlowCtrl_5_RX  (\
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_0 << 56U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_1 << 48U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_2 << 40U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_3 << 32U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_4 << 24U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_5 << 16U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_6 << 8U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_7))

/* Tx */
#define IL_IotDownLinkFlowCtrl_5_TX(x)  \
{ \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_0 = ((x) >> 56U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_1 = ((x) >> 48U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_2 = ((x) >> 40U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_3 = ((x) >> 32U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_4 = ((x) >> 24U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_5 = ((x) >> 16U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_6 = ((x) >> 8U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkFlowCtrl_6_RX  (\
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_0 << 56U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_1 << 48U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_2 << 40U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_3 << 32U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_4 << 24U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_5 << 16U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_6 << 8U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_7))

/* Tx */
#define IL_IotDownLinkFlowCtrl_6_TX(x)  \
{ \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_0 = ((x) >> 56U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_1 = ((x) >> 48U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_2 = ((x) >> 40U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_3 = ((x) >> 32U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_4 = ((x) >> 24U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_5 = ((x) >> 16U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_6 = ((x) >> 8U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_IotDownLinkFlowCtrl_7_RX  (\
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_0 << 56U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_1 << 48U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_2 << 40U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_3 << 32U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_4 << 24U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_5 << 16U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_6 << 8U) | \
	(IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_7))

/* Tx */
#define IL_IotDownLinkFlowCtrl_7_TX(x)  \
{ \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_0 = ((x) >> 56U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_1 = ((x) >> 48U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_2 = ((x) >> 40U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_3 = ((x) >> 32U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_4 = ((x) >> 24U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_5 = ((x) >> 16U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_6 = ((x) >> 8U) & 255U; \
	IotDownLinkFlowCtrl_t.msg.sig.IotDownLinkFlowCtrl_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_ScanReq_RX  (IotScanConnectReq_t.msg.sig.ScanReq)

/*  Tx */
#define IL_ScanReq_TX(x)  \
{ \
	IotScanConnectReq_t.msg.sig.ScanReq = (x);\
}

/*  Rx */
#define IL_ConnectReqAction_RX  (IotScanConnectReq_t.msg.sig.ConnectReqAction)

/*  Tx */
#define IL_ConnectReqAction_TX(x)  \
{ \
	IotScanConnectReq_t.msg.sig.ConnectReqAction = (x);\
}

/*  Rx */
#define IL_ConnectReqDeviceAddr_RX  (\
	(IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_0 << 40U) | \
	(IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_1 << 32U) | \
	(IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_2 << 24U) | \
	(IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_3 << 16U) | \
	(IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_4 << 8U) | \
	(IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_5))

/* Tx */
#define IL_ConnectReqDeviceAddr_TX(x)  \
{ \
	IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_0 = ((x) >> 40U) & 255U; \
	IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_1 = ((x) >> 32U) & 255U; \
	IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_2 = ((x) >> 24U) & 255U; \
	IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_3 = ((x) >> 16U) & 255U; \
	IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_4 = ((x) >> 8U) & 255U; \
	IotScanConnectReq_t.msg.sig.ConnectReqDeviceAddr_5 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryRsp_0_RX  (\
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_0 << 56U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_1 << 48U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_2 << 40U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_3 << 32U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_4 << 24U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_5 << 16U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_6 << 8U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_7))

/* Tx */
#define IL_BTA_SRR_FactoryRsp_0_TX(x)  \
{ \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryRsp_1_RX  (\
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_0 << 56U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_1 << 48U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_2 << 40U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_3 << 32U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_4 << 24U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_5 << 16U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_6 << 8U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_7))

/* Tx */
#define IL_BTA_SRR_FactoryRsp_1_TX(x)  \
{ \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryRsp_2_RX  (\
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_0 << 56U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_1 << 48U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_2 << 40U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_3 << 32U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_4 << 24U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_5 << 16U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_6 << 8U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_7))

/* Tx */
#define IL_BTA_SRR_FactoryRsp_2_TX(x)  \
{ \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryRsp_3_RX  (\
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_0 << 56U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_1 << 48U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_2 << 40U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_3 << 32U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_4 << 24U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_5 << 16U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_6 << 8U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_7))

/* Tx */
#define IL_BTA_SRR_FactoryRsp_3_TX(x)  \
{ \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryRsp_4_RX  (\
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_0 << 56U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_1 << 48U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_2 << 40U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_3 << 32U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_4 << 24U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_5 << 16U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_6 << 8U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_7))

/* Tx */
#define IL_BTA_SRR_FactoryRsp_4_TX(x)  \
{ \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryRsp_5_RX  (\
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_0 << 56U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_1 << 48U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_2 << 40U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_3 << 32U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_4 << 24U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_5 << 16U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_6 << 8U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_7))

/* Tx */
#define IL_BTA_SRR_FactoryRsp_5_TX(x)  \
{ \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryRsp_6_RX  (\
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_0 << 56U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_1 << 48U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_2 << 40U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_3 << 32U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_4 << 24U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_5 << 16U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_6 << 8U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_7))

/* Tx */
#define IL_BTA_SRR_FactoryRsp_6_TX(x)  \
{ \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryRsp_7_RX  (\
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_0 << 56U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_1 << 48U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_2 << 40U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_3 << 32U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_4 << 24U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_5 << 16U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_6 << 8U) | \
	(BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_7))

/* Tx */
#define IL_BTA_SRR_FactoryRsp_7_TX(x)  \
{ \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryRsp_t.msg.sig.BTA_SRR_FactoryRsp_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryReq_0_RX  (\
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_0 << 56U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_1 << 48U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_2 << 40U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_3 << 32U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_4 << 24U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_5 << 16U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_6 << 8U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_7))

/* Tx */
#define IL_BTA_SRR_FactoryReq_0_TX(x)  \
{ \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryReq_1_RX  (\
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_0 << 56U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_1 << 48U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_2 << 40U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_3 << 32U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_4 << 24U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_5 << 16U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_6 << 8U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_7))

/* Tx */
#define IL_BTA_SRR_FactoryReq_1_TX(x)  \
{ \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryReq_2_RX  (\
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_0 << 56U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_1 << 48U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_2 << 40U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_3 << 32U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_4 << 24U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_5 << 16U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_6 << 8U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_7))

/* Tx */
#define IL_BTA_SRR_FactoryReq_2_TX(x)  \
{ \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryReq_3_RX  (\
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_0 << 56U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_1 << 48U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_2 << 40U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_3 << 32U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_4 << 24U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_5 << 16U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_6 << 8U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_7))

/* Tx */
#define IL_BTA_SRR_FactoryReq_3_TX(x)  \
{ \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryReq_4_RX  (\
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_0 << 56U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_1 << 48U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_2 << 40U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_3 << 32U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_4 << 24U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_5 << 16U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_6 << 8U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_7))

/* Tx */
#define IL_BTA_SRR_FactoryReq_4_TX(x)  \
{ \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryReq_5_RX  (\
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_0 << 56U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_1 << 48U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_2 << 40U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_3 << 32U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_4 << 24U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_5 << 16U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_6 << 8U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_7))

/* Tx */
#define IL_BTA_SRR_FactoryReq_5_TX(x)  \
{ \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryReq_6_RX  (\
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_0 << 56U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_1 << 48U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_2 << 40U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_3 << 32U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_4 << 24U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_5 << 16U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_6 << 8U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_7))

/* Tx */
#define IL_BTA_SRR_FactoryReq_6_TX(x)  \
{ \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRR_FactoryReq_7_RX  (\
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_0 << 56U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_1 << 48U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_2 << 40U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_3 << 32U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_4 << 24U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_5 << 16U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_6 << 8U) | \
	(BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_7))

/* Tx */
#define IL_BTA_SRR_FactoryReq_7_TX(x)  \
{ \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_0 = ((x) >> 56U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_1 = ((x) >> 48U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_2 = ((x) >> 40U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_3 = ((x) >> 32U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_4 = ((x) >> 24U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_5 = ((x) >> 16U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_6 = ((x) >> 8U) & 255U; \
	BTA_SRR_FactoryReq_t.msg.sig.BTA_SRR_FactoryReq_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_ScrNodId_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.ScrNodId_ZONE_LEM)

/*  Tx */
#define IL_ScrNodId_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.ScrNodId_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_RepMsgReq_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.RepMsgReq_ZONE_LEM)

/*  Tx */
#define IL_RepMsgReq_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.RepMsgReq_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_NetMngtCoorn_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.NetMngtCoorn_ZONE_LEM)

/*  Tx */
#define IL_NetMngtCoorn_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.NetMngtCoorn_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_NetMngtCoorrSleepRdy_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.NetMngtCoorrSleepRdy_ZONE_LEM)

/*  Tx */
#define IL_NetMngtCoorrSleepRdy_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.NetMngtCoorrSleepRdy_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_ActvWakeup_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.ActvWakeup_ZONE_LEM)

/*  Tx */
#define IL_ActvWakeup_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.ActvWakeup_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_ResdBit1_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.ResdBit1_ZONE_LEM)

/*  Tx */
#define IL_ResdBit1_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.ResdBit1_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PtlNetInfo_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PtlNetInfo_ZONE_LEM)

/*  Tx */
#define IL_PtlNetInfo_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PtlNetInfo_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_ResdBit2_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.ResdBit2_ZONE_LEM)

/*  Tx */
#define IL_ResdBit2_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.ResdBit2_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN1_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN1_ZONE_LEM)

/*  Tx */
#define IL_PN1_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN1_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN2_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN2_ZONE_LEM)

/*  Tx */
#define IL_PN2_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN2_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN3_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN3_ZONE_LEM)

/*  Tx */
#define IL_PN3_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN3_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN4_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN4_ZONE_LEM)

/*  Tx */
#define IL_PN4_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN4_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN5_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN5_ZONE_LEM)

/*  Tx */
#define IL_PN5_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN5_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN6_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN6_ZONE_LEM)

/*  Tx */
#define IL_PN6_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN6_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN7_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN7_ZONE_LEM)

/*  Tx */
#define IL_PN7_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN7_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN8_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN8_ZONE_LEM)

/*  Tx */
#define IL_PN8_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN8_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN9_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN9_ZONE_LEM)

/*  Tx */
#define IL_PN9_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN9_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN10_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN10_ZONE_LEM)

/*  Tx */
#define IL_PN10_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN10_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN11_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN11_ZONE_LEM)

/*  Tx */
#define IL_PN11_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN11_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN12_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN12_ZONE_LEM)

/*  Tx */
#define IL_PN12_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN12_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN13_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN13_ZONE_LEM)

/*  Tx */
#define IL_PN13_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN13_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN14_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN14_ZONE_LEM)

/*  Tx */
#define IL_PN14_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN14_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN15_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN15_ZONE_LEM)

/*  Tx */
#define IL_PN15_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN15_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN16_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN16_ZONE_LEM)

/*  Tx */
#define IL_PN16_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN16_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN17_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN17_ZONE_LEM)

/*  Tx */
#define IL_PN17_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN17_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN18_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN18_ZONE_LEM)

/*  Tx */
#define IL_PN18_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN18_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN19_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN19_ZONE_LEM)

/*  Tx */
#define IL_PN19_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN19_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN20_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN20_ZONE_LEM)

/*  Tx */
#define IL_PN20_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN20_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN21_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN21_ZONE_LEM)

/*  Tx */
#define IL_PN21_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN21_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN22_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN22_ZONE_LEM)

/*  Tx */
#define IL_PN22_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN22_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN23_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN23_ZONE_LEM)

/*  Tx */
#define IL_PN23_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN23_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN24_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN24_ZONE_LEM)

/*  Tx */
#define IL_PN24_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN24_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN25_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN25_ZONE_LEM)

/*  Tx */
#define IL_PN25_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN25_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN26_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN26_ZONE_LEM)

/*  Tx */
#define IL_PN26_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN26_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN27_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN27_ZONE_LEM)

/*  Tx */
#define IL_PN27_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN27_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN28_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN28_ZONE_LEM)

/*  Tx */
#define IL_PN28_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN28_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN29_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN29_ZONE_LEM)

/*  Tx */
#define IL_PN29_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN29_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN30_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN30_ZONE_LEM)

/*  Tx */
#define IL_PN30_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN30_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN31_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN31_ZONE_LEM)

/*  Tx */
#define IL_PN31_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN31_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN32_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN32_ZONE_LEM)

/*  Tx */
#define IL_PN32_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN32_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN33_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN33_ZONE_LEM)

/*  Tx */
#define IL_PN33_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN33_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN34_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN34_ZONE_LEM)

/*  Tx */
#define IL_PN34_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN34_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN35_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN35_ZONE_LEM)

/*  Tx */
#define IL_PN35_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN35_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN36_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN36_ZONE_LEM)

/*  Tx */
#define IL_PN36_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN36_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN37_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN37_ZONE_LEM)

/*  Tx */
#define IL_PN37_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN37_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN38_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN38_ZONE_LEM)

/*  Tx */
#define IL_PN38_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN38_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN39_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN39_ZONE_LEM)

/*  Tx */
#define IL_PN39_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN39_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN40_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN40_ZONE_LEM)

/*  Tx */
#define IL_PN40_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN40_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN41_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN41_ZONE_LEM)

/*  Tx */
#define IL_PN41_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN41_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN42_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN42_ZONE_LEM)

/*  Tx */
#define IL_PN42_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN42_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN43_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN43_ZONE_LEM)

/*  Tx */
#define IL_PN43_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN43_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN44_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN44_ZONE_LEM)

/*  Tx */
#define IL_PN44_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN44_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN45_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN45_ZONE_LEM)

/*  Tx */
#define IL_PN45_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN45_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN46_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN46_ZONE_LEM)

/*  Tx */
#define IL_PN46_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN46_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN47_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN47_ZONE_LEM)

/*  Tx */
#define IL_PN47_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN47_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_PN48_ZONE_LEM_RX  (NM_ZONE_LEM_t.msg.sig.PN48_ZONE_LEM)

/*  Tx */
#define IL_PN48_ZONE_LEM_TX(x)  \
{ \
	NM_ZONE_LEM_t.msg.sig.PN48_ZONE_LEM = (x);\
}

/*  Rx */
#define IL_ScrNodId_BTA1_RX  (NM_BTA1_t.msg.sig.ScrNodId_BTA1)

/*  Tx */
#define IL_ScrNodId_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.ScrNodId_BTA1 = (x);\
}

/*  Rx */
#define IL_RepMsgReq_BTA1_RX  (NM_BTA1_t.msg.sig.RepMsgReq_BTA1)

/*  Tx */
#define IL_RepMsgReq_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.RepMsgReq_BTA1 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorn_BTA1_RX  (NM_BTA1_t.msg.sig.NetMngtCoorn_BTA1)

/*  Tx */
#define IL_NetMngtCoorn_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.NetMngtCoorn_BTA1 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorrSleepRdy_BTA1_RX  (NM_BTA1_t.msg.sig.NetMngtCoorrSleepRdy_BTA1)

/*  Tx */
#define IL_NetMngtCoorrSleepRdy_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.NetMngtCoorrSleepRdy_BTA1 = (x);\
}

/*  Rx */
#define IL_ActvWakeup_BTA1_RX  (NM_BTA1_t.msg.sig.ActvWakeup_BTA1)

/*  Tx */
#define IL_ActvWakeup_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.ActvWakeup_BTA1 = (x);\
}

/*  Rx */
#define IL_ResdBit1_BTA1_RX  (NM_BTA1_t.msg.sig.ResdBit1_BTA1)

/*  Tx */
#define IL_ResdBit1_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.ResdBit1_BTA1 = (x);\
}

/*  Rx */
#define IL_PtlNetInfo_BTA1_RX  (NM_BTA1_t.msg.sig.PtlNetInfo_BTA1)

/*  Tx */
#define IL_PtlNetInfo_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PtlNetInfo_BTA1 = (x);\
}

/*  Rx */
#define IL_ResdBit2_BTA1_RX  (NM_BTA1_t.msg.sig.ResdBit2_BTA1)

/*  Tx */
#define IL_ResdBit2_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.ResdBit2_BTA1 = (x);\
}

/*  Rx */
#define IL_PN1_BTA1_RX  (NM_BTA1_t.msg.sig.PN1_BTA1)

/*  Tx */
#define IL_PN1_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN1_BTA1 = (x);\
}

/*  Rx */
#define IL_PN2_BTA1_RX  (NM_BTA1_t.msg.sig.PN2_BTA1)

/*  Tx */
#define IL_PN2_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN2_BTA1 = (x);\
}

/*  Rx */
#define IL_PN3_BTA1_RX  (NM_BTA1_t.msg.sig.PN3_BTA1)

/*  Tx */
#define IL_PN3_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN3_BTA1 = (x);\
}

/*  Rx */
#define IL_PN4_BTA1_RX  (NM_BTA1_t.msg.sig.PN4_BTA1)

/*  Tx */
#define IL_PN4_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN4_BTA1 = (x);\
}

/*  Rx */
#define IL_PN5_BTA1_RX  (NM_BTA1_t.msg.sig.PN5_BTA1)

/*  Tx */
#define IL_PN5_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN5_BTA1 = (x);\
}

/*  Rx */
#define IL_PN6_BTA1_RX  (NM_BTA1_t.msg.sig.PN6_BTA1)

/*  Tx */
#define IL_PN6_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN6_BTA1 = (x);\
}

/*  Rx */
#define IL_PN7_BTA1_RX  (NM_BTA1_t.msg.sig.PN7_BTA1)

/*  Tx */
#define IL_PN7_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN7_BTA1 = (x);\
}

/*  Rx */
#define IL_PN8_BTA1_RX  (NM_BTA1_t.msg.sig.PN8_BTA1)

/*  Tx */
#define IL_PN8_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN8_BTA1 = (x);\
}

/*  Rx */
#define IL_PN9_BTA1_RX  (NM_BTA1_t.msg.sig.PN9_BTA1)

/*  Tx */
#define IL_PN9_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN9_BTA1 = (x);\
}

/*  Rx */
#define IL_PN10_BTA1_RX  (NM_BTA1_t.msg.sig.PN10_BTA1)

/*  Tx */
#define IL_PN10_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN10_BTA1 = (x);\
}

/*  Rx */
#define IL_PN11_BTA1_RX  (NM_BTA1_t.msg.sig.PN11_BTA1)

/*  Tx */
#define IL_PN11_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN11_BTA1 = (x);\
}

/*  Rx */
#define IL_PN12_BTA1_RX  (NM_BTA1_t.msg.sig.PN12_BTA1)

/*  Tx */
#define IL_PN12_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN12_BTA1 = (x);\
}

/*  Rx */
#define IL_PN13_BTA1_RX  (NM_BTA1_t.msg.sig.PN13_BTA1)

/*  Tx */
#define IL_PN13_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN13_BTA1 = (x);\
}

/*  Rx */
#define IL_PN14_BTA1_RX  (NM_BTA1_t.msg.sig.PN14_BTA1)

/*  Tx */
#define IL_PN14_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN14_BTA1 = (x);\
}

/*  Rx */
#define IL_PN15_BTA1_RX  (NM_BTA1_t.msg.sig.PN15_BTA1)

/*  Tx */
#define IL_PN15_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN15_BTA1 = (x);\
}

/*  Rx */
#define IL_PN16_BTA1_RX  (NM_BTA1_t.msg.sig.PN16_BTA1)

/*  Tx */
#define IL_PN16_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN16_BTA1 = (x);\
}

/*  Rx */
#define IL_PN17_BTA1_RX  (NM_BTA1_t.msg.sig.PN17_BTA1)

/*  Tx */
#define IL_PN17_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN17_BTA1 = (x);\
}

/*  Rx */
#define IL_PN18_BTA1_RX  (NM_BTA1_t.msg.sig.PN18_BTA1)

/*  Tx */
#define IL_PN18_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN18_BTA1 = (x);\
}

/*  Rx */
#define IL_PN19_BTA1_RX  (NM_BTA1_t.msg.sig.PN19_BTA1)

/*  Tx */
#define IL_PN19_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN19_BTA1 = (x);\
}

/*  Rx */
#define IL_PN20_BTA1_RX  (NM_BTA1_t.msg.sig.PN20_BTA1)

/*  Tx */
#define IL_PN20_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN20_BTA1 = (x);\
}

/*  Rx */
#define IL_PN21_BTA1_RX  (NM_BTA1_t.msg.sig.PN21_BTA1)

/*  Tx */
#define IL_PN21_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN21_BTA1 = (x);\
}

/*  Rx */
#define IL_PN22_BTA1_RX  (NM_BTA1_t.msg.sig.PN22_BTA1)

/*  Tx */
#define IL_PN22_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN22_BTA1 = (x);\
}

/*  Rx */
#define IL_PN23_BTA1_RX  (NM_BTA1_t.msg.sig.PN23_BTA1)

/*  Tx */
#define IL_PN23_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN23_BTA1 = (x);\
}

/*  Rx */
#define IL_PN24_BTA1_RX  (NM_BTA1_t.msg.sig.PN24_BTA1)

/*  Tx */
#define IL_PN24_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN24_BTA1 = (x);\
}

/*  Rx */
#define IL_PN25_BTA1_RX  (NM_BTA1_t.msg.sig.PN25_BTA1)

/*  Tx */
#define IL_PN25_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN25_BTA1 = (x);\
}

/*  Rx */
#define IL_PN26_BTA1_RX  (NM_BTA1_t.msg.sig.PN26_BTA1)

/*  Tx */
#define IL_PN26_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN26_BTA1 = (x);\
}

/*  Rx */
#define IL_PN27_BTA1_RX  (NM_BTA1_t.msg.sig.PN27_BTA1)

/*  Tx */
#define IL_PN27_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN27_BTA1 = (x);\
}

/*  Rx */
#define IL_PN28_BTA1_RX  (NM_BTA1_t.msg.sig.PN28_BTA1)

/*  Tx */
#define IL_PN28_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN28_BTA1 = (x);\
}

/*  Rx */
#define IL_PN29_BTA1_RX  (NM_BTA1_t.msg.sig.PN29_BTA1)

/*  Tx */
#define IL_PN29_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN29_BTA1 = (x);\
}

/*  Rx */
#define IL_PN30_BTA1_RX  (NM_BTA1_t.msg.sig.PN30_BTA1)

/*  Tx */
#define IL_PN30_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN30_BTA1 = (x);\
}

/*  Rx */
#define IL_PN31_BTA1_RX  (NM_BTA1_t.msg.sig.PN31_BTA1)

/*  Tx */
#define IL_PN31_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN31_BTA1 = (x);\
}

/*  Rx */
#define IL_PN32_BTA1_RX  (NM_BTA1_t.msg.sig.PN32_BTA1)

/*  Tx */
#define IL_PN32_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN32_BTA1 = (x);\
}

/*  Rx */
#define IL_PN33_BTA1_RX  (NM_BTA1_t.msg.sig.PN33_BTA1)

/*  Tx */
#define IL_PN33_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN33_BTA1 = (x);\
}

/*  Rx */
#define IL_PN34_BTA1_RX  (NM_BTA1_t.msg.sig.PN34_BTA1)

/*  Tx */
#define IL_PN34_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN34_BTA1 = (x);\
}

/*  Rx */
#define IL_PN35_BTA1_RX  (NM_BTA1_t.msg.sig.PN35_BTA1)

/*  Tx */
#define IL_PN35_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN35_BTA1 = (x);\
}

/*  Rx */
#define IL_PN36_BTA1_RX  (NM_BTA1_t.msg.sig.PN36_BTA1)

/*  Tx */
#define IL_PN36_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN36_BTA1 = (x);\
}

/*  Rx */
#define IL_PN37_BTA1_RX  (NM_BTA1_t.msg.sig.PN37_BTA1)

/*  Tx */
#define IL_PN37_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN37_BTA1 = (x);\
}

/*  Rx */
#define IL_PN38_BTA1_RX  (NM_BTA1_t.msg.sig.PN38_BTA1)

/*  Tx */
#define IL_PN38_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN38_BTA1 = (x);\
}

/*  Rx */
#define IL_PN39_BTA1_RX  (NM_BTA1_t.msg.sig.PN39_BTA1)

/*  Tx */
#define IL_PN39_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN39_BTA1 = (x);\
}

/*  Rx */
#define IL_PN40_BTA1_RX  (NM_BTA1_t.msg.sig.PN40_BTA1)

/*  Tx */
#define IL_PN40_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN40_BTA1 = (x);\
}

/*  Rx */
#define IL_PN41_BTA1_RX  (NM_BTA1_t.msg.sig.PN41_BTA1)

/*  Tx */
#define IL_PN41_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN41_BTA1 = (x);\
}

/*  Rx */
#define IL_PN42_BTA1_RX  (NM_BTA1_t.msg.sig.PN42_BTA1)

/*  Tx */
#define IL_PN42_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN42_BTA1 = (x);\
}

/*  Rx */
#define IL_PN43_BTA1_RX  (NM_BTA1_t.msg.sig.PN43_BTA1)

/*  Tx */
#define IL_PN43_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN43_BTA1 = (x);\
}

/*  Rx */
#define IL_PN44_BTA1_RX  (NM_BTA1_t.msg.sig.PN44_BTA1)

/*  Tx */
#define IL_PN44_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN44_BTA1 = (x);\
}

/*  Rx */
#define IL_PN45_BTA1_RX  (NM_BTA1_t.msg.sig.PN45_BTA1)

/*  Tx */
#define IL_PN45_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN45_BTA1 = (x);\
}

/*  Rx */
#define IL_PN46_BTA1_RX  (NM_BTA1_t.msg.sig.PN46_BTA1)

/*  Tx */
#define IL_PN46_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN46_BTA1 = (x);\
}

/*  Rx */
#define IL_PN47_BTA1_RX  (NM_BTA1_t.msg.sig.PN47_BTA1)

/*  Tx */
#define IL_PN47_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN47_BTA1 = (x);\
}

/*  Rx */
#define IL_PN48_BTA1_RX  (NM_BTA1_t.msg.sig.PN48_BTA1)

/*  Tx */
#define IL_PN48_BTA1_TX(x)  \
{ \
	NM_BTA1_t.msg.sig.PN48_BTA1 = (x);\
}

/*  Rx */
#define IL_ScrNodId_BTA2_RX  (NM_BTA2_t.msg.sig.ScrNodId_BTA2)

/*  Tx */
#define IL_ScrNodId_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.ScrNodId_BTA2 = (x);\
}

/*  Rx */
#define IL_RepMsgReq_BTA2_RX  (NM_BTA2_t.msg.sig.RepMsgReq_BTA2)

/*  Tx */
#define IL_RepMsgReq_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.RepMsgReq_BTA2 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorn_BTA2_RX  (NM_BTA2_t.msg.sig.NetMngtCoorn_BTA2)

/*  Tx */
#define IL_NetMngtCoorn_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.NetMngtCoorn_BTA2 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorrSleepRdy_BTA2_RX  (NM_BTA2_t.msg.sig.NetMngtCoorrSleepRdy_BTA2)

/*  Tx */
#define IL_NetMngtCoorrSleepRdy_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.NetMngtCoorrSleepRdy_BTA2 = (x);\
}

/*  Rx */
#define IL_ActvWakeup_BTA2_RX  (NM_BTA2_t.msg.sig.ActvWakeup_BTA2)

/*  Tx */
#define IL_ActvWakeup_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.ActvWakeup_BTA2 = (x);\
}

/*  Rx */
#define IL_ResdBit1_BTA2_RX  (NM_BTA2_t.msg.sig.ResdBit1_BTA2)

/*  Tx */
#define IL_ResdBit1_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.ResdBit1_BTA2 = (x);\
}

/*  Rx */
#define IL_PtlNetInfo_BTA2_RX  (NM_BTA2_t.msg.sig.PtlNetInfo_BTA2)

/*  Tx */
#define IL_PtlNetInfo_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PtlNetInfo_BTA2 = (x);\
}

/*  Rx */
#define IL_ResdBit2_BTA2_RX  (NM_BTA2_t.msg.sig.ResdBit2_BTA2)

/*  Tx */
#define IL_ResdBit2_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.ResdBit2_BTA2 = (x);\
}

/*  Rx */
#define IL_PN1_BTA2_RX  (NM_BTA2_t.msg.sig.PN1_BTA2)

/*  Tx */
#define IL_PN1_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN1_BTA2 = (x);\
}

/*  Rx */
#define IL_PN2_BTA2_RX  (NM_BTA2_t.msg.sig.PN2_BTA2)

/*  Tx */
#define IL_PN2_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN2_BTA2 = (x);\
}

/*  Rx */
#define IL_PN3_BTA2_RX  (NM_BTA2_t.msg.sig.PN3_BTA2)

/*  Tx */
#define IL_PN3_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN3_BTA2 = (x);\
}

/*  Rx */
#define IL_PN4_BTA2_RX  (NM_BTA2_t.msg.sig.PN4_BTA2)

/*  Tx */
#define IL_PN4_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN4_BTA2 = (x);\
}

/*  Rx */
#define IL_PN5_BTA2_RX  (NM_BTA2_t.msg.sig.PN5_BTA2)

/*  Tx */
#define IL_PN5_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN5_BTA2 = (x);\
}

/*  Rx */
#define IL_PN6_BTA2_RX  (NM_BTA2_t.msg.sig.PN6_BTA2)

/*  Tx */
#define IL_PN6_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN6_BTA2 = (x);\
}

/*  Rx */
#define IL_PN7_BTA2_RX  (NM_BTA2_t.msg.sig.PN7_BTA2)

/*  Tx */
#define IL_PN7_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN7_BTA2 = (x);\
}

/*  Rx */
#define IL_PN8_BTA2_RX  (NM_BTA2_t.msg.sig.PN8_BTA2)

/*  Tx */
#define IL_PN8_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN8_BTA2 = (x);\
}

/*  Rx */
#define IL_PN9_BTA2_RX  (NM_BTA2_t.msg.sig.PN9_BTA2)

/*  Tx */
#define IL_PN9_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN9_BTA2 = (x);\
}

/*  Rx */
#define IL_PN10_BTA2_RX  (NM_BTA2_t.msg.sig.PN10_BTA2)

/*  Tx */
#define IL_PN10_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN10_BTA2 = (x);\
}

/*  Rx */
#define IL_PN11_BTA2_RX  (NM_BTA2_t.msg.sig.PN11_BTA2)

/*  Tx */
#define IL_PN11_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN11_BTA2 = (x);\
}

/*  Rx */
#define IL_PN12_BTA2_RX  (NM_BTA2_t.msg.sig.PN12_BTA2)

/*  Tx */
#define IL_PN12_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN12_BTA2 = (x);\
}

/*  Rx */
#define IL_PN13_BTA2_RX  (NM_BTA2_t.msg.sig.PN13_BTA2)

/*  Tx */
#define IL_PN13_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN13_BTA2 = (x);\
}

/*  Rx */
#define IL_PN14_BTA2_RX  (NM_BTA2_t.msg.sig.PN14_BTA2)

/*  Tx */
#define IL_PN14_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN14_BTA2 = (x);\
}

/*  Rx */
#define IL_PN15_BTA2_RX  (NM_BTA2_t.msg.sig.PN15_BTA2)

/*  Tx */
#define IL_PN15_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN15_BTA2 = (x);\
}

/*  Rx */
#define IL_PN16_BTA2_RX  (NM_BTA2_t.msg.sig.PN16_BTA2)

/*  Tx */
#define IL_PN16_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN16_BTA2 = (x);\
}

/*  Rx */
#define IL_PN17_BTA2_RX  (NM_BTA2_t.msg.sig.PN17_BTA2)

/*  Tx */
#define IL_PN17_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN17_BTA2 = (x);\
}

/*  Rx */
#define IL_PN18_BTA2_RX  (NM_BTA2_t.msg.sig.PN18_BTA2)

/*  Tx */
#define IL_PN18_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN18_BTA2 = (x);\
}

/*  Rx */
#define IL_PN19_BTA2_RX  (NM_BTA2_t.msg.sig.PN19_BTA2)

/*  Tx */
#define IL_PN19_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN19_BTA2 = (x);\
}

/*  Rx */
#define IL_PN20_BTA2_RX  (NM_BTA2_t.msg.sig.PN20_BTA2)

/*  Tx */
#define IL_PN20_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN20_BTA2 = (x);\
}

/*  Rx */
#define IL_PN21_BTA2_RX  (NM_BTA2_t.msg.sig.PN21_BTA2)

/*  Tx */
#define IL_PN21_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN21_BTA2 = (x);\
}

/*  Rx */
#define IL_PN22_BTA2_RX  (NM_BTA2_t.msg.sig.PN22_BTA2)

/*  Tx */
#define IL_PN22_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN22_BTA2 = (x);\
}

/*  Rx */
#define IL_PN23_BTA2_RX  (NM_BTA2_t.msg.sig.PN23_BTA2)

/*  Tx */
#define IL_PN23_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN23_BTA2 = (x);\
}

/*  Rx */
#define IL_PN24_BTA2_RX  (NM_BTA2_t.msg.sig.PN24_BTA2)

/*  Tx */
#define IL_PN24_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN24_BTA2 = (x);\
}

/*  Rx */
#define IL_PN25_BTA2_RX  (NM_BTA2_t.msg.sig.PN25_BTA2)

/*  Tx */
#define IL_PN25_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN25_BTA2 = (x);\
}

/*  Rx */
#define IL_PN26_BTA2_RX  (NM_BTA2_t.msg.sig.PN26_BTA2)

/*  Tx */
#define IL_PN26_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN26_BTA2 = (x);\
}

/*  Rx */
#define IL_PN27_BTA2_RX  (NM_BTA2_t.msg.sig.PN27_BTA2)

/*  Tx */
#define IL_PN27_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN27_BTA2 = (x);\
}

/*  Rx */
#define IL_PN28_BTA2_RX  (NM_BTA2_t.msg.sig.PN28_BTA2)

/*  Tx */
#define IL_PN28_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN28_BTA2 = (x);\
}

/*  Rx */
#define IL_PN29_BTA2_RX  (NM_BTA2_t.msg.sig.PN29_BTA2)

/*  Tx */
#define IL_PN29_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN29_BTA2 = (x);\
}

/*  Rx */
#define IL_PN30_BTA2_RX  (NM_BTA2_t.msg.sig.PN30_BTA2)

/*  Tx */
#define IL_PN30_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN30_BTA2 = (x);\
}

/*  Rx */
#define IL_PN31_BTA2_RX  (NM_BTA2_t.msg.sig.PN31_BTA2)

/*  Tx */
#define IL_PN31_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN31_BTA2 = (x);\
}

/*  Rx */
#define IL_PN32_BTA2_RX  (NM_BTA2_t.msg.sig.PN32_BTA2)

/*  Tx */
#define IL_PN32_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN32_BTA2 = (x);\
}

/*  Rx */
#define IL_PN33_BTA2_RX  (NM_BTA2_t.msg.sig.PN33_BTA2)

/*  Tx */
#define IL_PN33_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN33_BTA2 = (x);\
}

/*  Rx */
#define IL_PN34_BTA2_RX  (NM_BTA2_t.msg.sig.PN34_BTA2)

/*  Tx */
#define IL_PN34_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN34_BTA2 = (x);\
}

/*  Rx */
#define IL_PN35_BTA2_RX  (NM_BTA2_t.msg.sig.PN35_BTA2)

/*  Tx */
#define IL_PN35_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN35_BTA2 = (x);\
}

/*  Rx */
#define IL_PN36_BTA2_RX  (NM_BTA2_t.msg.sig.PN36_BTA2)

/*  Tx */
#define IL_PN36_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN36_BTA2 = (x);\
}

/*  Rx */
#define IL_PN37_BTA2_RX  (NM_BTA2_t.msg.sig.PN37_BTA2)

/*  Tx */
#define IL_PN37_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN37_BTA2 = (x);\
}

/*  Rx */
#define IL_PN38_BTA2_RX  (NM_BTA2_t.msg.sig.PN38_BTA2)

/*  Tx */
#define IL_PN38_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN38_BTA2 = (x);\
}

/*  Rx */
#define IL_PN39_BTA2_RX  (NM_BTA2_t.msg.sig.PN39_BTA2)

/*  Tx */
#define IL_PN39_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN39_BTA2 = (x);\
}

/*  Rx */
#define IL_PN40_BTA2_RX  (NM_BTA2_t.msg.sig.PN40_BTA2)

/*  Tx */
#define IL_PN40_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN40_BTA2 = (x);\
}

/*  Rx */
#define IL_PN41_BTA2_RX  (NM_BTA2_t.msg.sig.PN41_BTA2)

/*  Tx */
#define IL_PN41_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN41_BTA2 = (x);\
}

/*  Rx */
#define IL_PN42_BTA2_RX  (NM_BTA2_t.msg.sig.PN42_BTA2)

/*  Tx */
#define IL_PN42_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN42_BTA2 = (x);\
}

/*  Rx */
#define IL_PN43_BTA2_RX  (NM_BTA2_t.msg.sig.PN43_BTA2)

/*  Tx */
#define IL_PN43_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN43_BTA2 = (x);\
}

/*  Rx */
#define IL_PN44_BTA2_RX  (NM_BTA2_t.msg.sig.PN44_BTA2)

/*  Tx */
#define IL_PN44_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN44_BTA2 = (x);\
}

/*  Rx */
#define IL_PN45_BTA2_RX  (NM_BTA2_t.msg.sig.PN45_BTA2)

/*  Tx */
#define IL_PN45_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN45_BTA2 = (x);\
}

/*  Rx */
#define IL_PN46_BTA2_RX  (NM_BTA2_t.msg.sig.PN46_BTA2)

/*  Tx */
#define IL_PN46_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN46_BTA2 = (x);\
}

/*  Rx */
#define IL_PN47_BTA2_RX  (NM_BTA2_t.msg.sig.PN47_BTA2)

/*  Tx */
#define IL_PN47_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN47_BTA2 = (x);\
}

/*  Rx */
#define IL_PN48_BTA2_RX  (NM_BTA2_t.msg.sig.PN48_BTA2)

/*  Tx */
#define IL_PN48_BTA2_TX(x)  \
{ \
	NM_BTA2_t.msg.sig.PN48_BTA2 = (x);\
}

/*  Rx */
#define IL_ScrNodId_BTA3_RX  (NM_BTA3_t.msg.sig.ScrNodId_BTA3)

/*  Tx */
#define IL_ScrNodId_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.ScrNodId_BTA3 = (x);\
}

/*  Rx */
#define IL_RepMsgReq_BTA3_RX  (NM_BTA3_t.msg.sig.RepMsgReq_BTA3)

/*  Tx */
#define IL_RepMsgReq_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.RepMsgReq_BTA3 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorn_BTA3_RX  (NM_BTA3_t.msg.sig.NetMngtCoorn_BTA3)

/*  Tx */
#define IL_NetMngtCoorn_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.NetMngtCoorn_BTA3 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorrSleepRdy_BTA3_RX  (NM_BTA3_t.msg.sig.NetMngtCoorrSleepRdy_BTA3)

/*  Tx */
#define IL_NetMngtCoorrSleepRdy_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.NetMngtCoorrSleepRdy_BTA3 = (x);\
}

/*  Rx */
#define IL_ActvWakeup_BTA3_RX  (NM_BTA3_t.msg.sig.ActvWakeup_BTA3)

/*  Tx */
#define IL_ActvWakeup_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.ActvWakeup_BTA3 = (x);\
}

/*  Rx */
#define IL_ResdBit1_BTA3_RX  (NM_BTA3_t.msg.sig.ResdBit1_BTA3)

/*  Tx */
#define IL_ResdBit1_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.ResdBit1_BTA3 = (x);\
}

/*  Rx */
#define IL_PtlNetInfo_BTA3_RX  (NM_BTA3_t.msg.sig.PtlNetInfo_BTA3)

/*  Tx */
#define IL_PtlNetInfo_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PtlNetInfo_BTA3 = (x);\
}

/*  Rx */
#define IL_ResdBit2_BTA3_RX  (NM_BTA3_t.msg.sig.ResdBit2_BTA3)

/*  Tx */
#define IL_ResdBit2_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.ResdBit2_BTA3 = (x);\
}

/*  Rx */
#define IL_PN1_BTA3_RX  (NM_BTA3_t.msg.sig.PN1_BTA3)

/*  Tx */
#define IL_PN1_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN1_BTA3 = (x);\
}

/*  Rx */
#define IL_PN2_BTA3_RX  (NM_BTA3_t.msg.sig.PN2_BTA3)

/*  Tx */
#define IL_PN2_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN2_BTA3 = (x);\
}

/*  Rx */
#define IL_PN3_BTA3_RX  (NM_BTA3_t.msg.sig.PN3_BTA3)

/*  Tx */
#define IL_PN3_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN3_BTA3 = (x);\
}

/*  Rx */
#define IL_PN4_BTA3_RX  (NM_BTA3_t.msg.sig.PN4_BTA3)

/*  Tx */
#define IL_PN4_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN4_BTA3 = (x);\
}

/*  Rx */
#define IL_PN5_BTA3_RX  (NM_BTA3_t.msg.sig.PN5_BTA3)

/*  Tx */
#define IL_PN5_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN5_BTA3 = (x);\
}

/*  Rx */
#define IL_PN6_BTA3_RX  (NM_BTA3_t.msg.sig.PN6_BTA3)

/*  Tx */
#define IL_PN6_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN6_BTA3 = (x);\
}

/*  Rx */
#define IL_PN7_BTA3_RX  (NM_BTA3_t.msg.sig.PN7_BTA3)

/*  Tx */
#define IL_PN7_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN7_BTA3 = (x);\
}

/*  Rx */
#define IL_PN8_BTA3_RX  (NM_BTA3_t.msg.sig.PN8_BTA3)

/*  Tx */
#define IL_PN8_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN8_BTA3 = (x);\
}

/*  Rx */
#define IL_PN9_BTA3_RX  (NM_BTA3_t.msg.sig.PN9_BTA3)

/*  Tx */
#define IL_PN9_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN9_BTA3 = (x);\
}

/*  Rx */
#define IL_PN10_BTA3_RX  (NM_BTA3_t.msg.sig.PN10_BTA3)

/*  Tx */
#define IL_PN10_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN10_BTA3 = (x);\
}

/*  Rx */
#define IL_PN11_BTA3_RX  (NM_BTA3_t.msg.sig.PN11_BTA3)

/*  Tx */
#define IL_PN11_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN11_BTA3 = (x);\
}

/*  Rx */
#define IL_PN12_BTA3_RX  (NM_BTA3_t.msg.sig.PN12_BTA3)

/*  Tx */
#define IL_PN12_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN12_BTA3 = (x);\
}

/*  Rx */
#define IL_PN13_BTA3_RX  (NM_BTA3_t.msg.sig.PN13_BTA3)

/*  Tx */
#define IL_PN13_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN13_BTA3 = (x);\
}

/*  Rx */
#define IL_PN14_BTA3_RX  (NM_BTA3_t.msg.sig.PN14_BTA3)

/*  Tx */
#define IL_PN14_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN14_BTA3 = (x);\
}

/*  Rx */
#define IL_PN15_BTA3_RX  (NM_BTA3_t.msg.sig.PN15_BTA3)

/*  Tx */
#define IL_PN15_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN15_BTA3 = (x);\
}

/*  Rx */
#define IL_PN16_BTA3_RX  (NM_BTA3_t.msg.sig.PN16_BTA3)

/*  Tx */
#define IL_PN16_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN16_BTA3 = (x);\
}

/*  Rx */
#define IL_PN17_BTA3_RX  (NM_BTA3_t.msg.sig.PN17_BTA3)

/*  Tx */
#define IL_PN17_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN17_BTA3 = (x);\
}

/*  Rx */
#define IL_PN18_BTA3_RX  (NM_BTA3_t.msg.sig.PN18_BTA3)

/*  Tx */
#define IL_PN18_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN18_BTA3 = (x);\
}

/*  Rx */
#define IL_PN19_BTA3_RX  (NM_BTA3_t.msg.sig.PN19_BTA3)

/*  Tx */
#define IL_PN19_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN19_BTA3 = (x);\
}

/*  Rx */
#define IL_PN20_BTA3_RX  (NM_BTA3_t.msg.sig.PN20_BTA3)

/*  Tx */
#define IL_PN20_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN20_BTA3 = (x);\
}

/*  Rx */
#define IL_PN21_BTA3_RX  (NM_BTA3_t.msg.sig.PN21_BTA3)

/*  Tx */
#define IL_PN21_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN21_BTA3 = (x);\
}

/*  Rx */
#define IL_PN22_BTA3_RX  (NM_BTA3_t.msg.sig.PN22_BTA3)

/*  Tx */
#define IL_PN22_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN22_BTA3 = (x);\
}

/*  Rx */
#define IL_PN23_BTA3_RX  (NM_BTA3_t.msg.sig.PN23_BTA3)

/*  Tx */
#define IL_PN23_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN23_BTA3 = (x);\
}

/*  Rx */
#define IL_PN24_BTA3_RX  (NM_BTA3_t.msg.sig.PN24_BTA3)

/*  Tx */
#define IL_PN24_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN24_BTA3 = (x);\
}

/*  Rx */
#define IL_PN25_BTA3_RX  (NM_BTA3_t.msg.sig.PN25_BTA3)

/*  Tx */
#define IL_PN25_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN25_BTA3 = (x);\
}

/*  Rx */
#define IL_PN26_BTA3_RX  (NM_BTA3_t.msg.sig.PN26_BTA3)

/*  Tx */
#define IL_PN26_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN26_BTA3 = (x);\
}

/*  Rx */
#define IL_PN27_BTA3_RX  (NM_BTA3_t.msg.sig.PN27_BTA3)

/*  Tx */
#define IL_PN27_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN27_BTA3 = (x);\
}

/*  Rx */
#define IL_PN28_BTA3_RX  (NM_BTA3_t.msg.sig.PN28_BTA3)

/*  Tx */
#define IL_PN28_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN28_BTA3 = (x);\
}

/*  Rx */
#define IL_PN29_BTA3_RX  (NM_BTA3_t.msg.sig.PN29_BTA3)

/*  Tx */
#define IL_PN29_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN29_BTA3 = (x);\
}

/*  Rx */
#define IL_PN30_BTA3_RX  (NM_BTA3_t.msg.sig.PN30_BTA3)

/*  Tx */
#define IL_PN30_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN30_BTA3 = (x);\
}

/*  Rx */
#define IL_PN31_BTA3_RX  (NM_BTA3_t.msg.sig.PN31_BTA3)

/*  Tx */
#define IL_PN31_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN31_BTA3 = (x);\
}

/*  Rx */
#define IL_PN32_BTA3_RX  (NM_BTA3_t.msg.sig.PN32_BTA3)

/*  Tx */
#define IL_PN32_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN32_BTA3 = (x);\
}

/*  Rx */
#define IL_PN33_BTA3_RX  (NM_BTA3_t.msg.sig.PN33_BTA3)

/*  Tx */
#define IL_PN33_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN33_BTA3 = (x);\
}

/*  Rx */
#define IL_PN34_BTA3_RX  (NM_BTA3_t.msg.sig.PN34_BTA3)

/*  Tx */
#define IL_PN34_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN34_BTA3 = (x);\
}

/*  Rx */
#define IL_PN35_BTA3_RX  (NM_BTA3_t.msg.sig.PN35_BTA3)

/*  Tx */
#define IL_PN35_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN35_BTA3 = (x);\
}

/*  Rx */
#define IL_PN36_BTA3_RX  (NM_BTA3_t.msg.sig.PN36_BTA3)

/*  Tx */
#define IL_PN36_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN36_BTA3 = (x);\
}

/*  Rx */
#define IL_PN37_BTA3_RX  (NM_BTA3_t.msg.sig.PN37_BTA3)

/*  Tx */
#define IL_PN37_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN37_BTA3 = (x);\
}

/*  Rx */
#define IL_PN38_BTA3_RX  (NM_BTA3_t.msg.sig.PN38_BTA3)

/*  Tx */
#define IL_PN38_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN38_BTA3 = (x);\
}

/*  Rx */
#define IL_PN39_BTA3_RX  (NM_BTA3_t.msg.sig.PN39_BTA3)

/*  Tx */
#define IL_PN39_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN39_BTA3 = (x);\
}

/*  Rx */
#define IL_PN40_BTA3_RX  (NM_BTA3_t.msg.sig.PN40_BTA3)

/*  Tx */
#define IL_PN40_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN40_BTA3 = (x);\
}

/*  Rx */
#define IL_PN41_BTA3_RX  (NM_BTA3_t.msg.sig.PN41_BTA3)

/*  Tx */
#define IL_PN41_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN41_BTA3 = (x);\
}

/*  Rx */
#define IL_PN42_BTA3_RX  (NM_BTA3_t.msg.sig.PN42_BTA3)

/*  Tx */
#define IL_PN42_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN42_BTA3 = (x);\
}

/*  Rx */
#define IL_PN43_BTA3_RX  (NM_BTA3_t.msg.sig.PN43_BTA3)

/*  Tx */
#define IL_PN43_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN43_BTA3 = (x);\
}

/*  Rx */
#define IL_PN44_BTA3_RX  (NM_BTA3_t.msg.sig.PN44_BTA3)

/*  Tx */
#define IL_PN44_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN44_BTA3 = (x);\
}

/*  Rx */
#define IL_PN45_BTA3_RX  (NM_BTA3_t.msg.sig.PN45_BTA3)

/*  Tx */
#define IL_PN45_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN45_BTA3 = (x);\
}

/*  Rx */
#define IL_PN46_BTA3_RX  (NM_BTA3_t.msg.sig.PN46_BTA3)

/*  Tx */
#define IL_PN46_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN46_BTA3 = (x);\
}

/*  Rx */
#define IL_PN47_BTA3_RX  (NM_BTA3_t.msg.sig.PN47_BTA3)

/*  Tx */
#define IL_PN47_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN47_BTA3 = (x);\
}

/*  Rx */
#define IL_PN48_BTA3_RX  (NM_BTA3_t.msg.sig.PN48_BTA3)

/*  Tx */
#define IL_PN48_BTA3_TX(x)  \
{ \
	NM_BTA3_t.msg.sig.PN48_BTA3 = (x);\
}

/*  Rx */
#define IL_ScrNodId_BTA4_RX  (NM_BTA4_t.msg.sig.ScrNodId_BTA4)

/*  Tx */
#define IL_ScrNodId_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.ScrNodId_BTA4 = (x);\
}

/*  Rx */
#define IL_RepMsgReq_BTA4_RX  (NM_BTA4_t.msg.sig.RepMsgReq_BTA4)

/*  Tx */
#define IL_RepMsgReq_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.RepMsgReq_BTA4 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorn_BTA4_RX  (NM_BTA4_t.msg.sig.NetMngtCoorn_BTA4)

/*  Tx */
#define IL_NetMngtCoorn_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.NetMngtCoorn_BTA4 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorrSleepRdy_BTA4_RX  (NM_BTA4_t.msg.sig.NetMngtCoorrSleepRdy_BTA4)

/*  Tx */
#define IL_NetMngtCoorrSleepRdy_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.NetMngtCoorrSleepRdy_BTA4 = (x);\
}

/*  Rx */
#define IL_ActvWakeup_BTA4_RX  (NM_BTA4_t.msg.sig.ActvWakeup_BTA4)

/*  Tx */
#define IL_ActvWakeup_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.ActvWakeup_BTA4 = (x);\
}

/*  Rx */
#define IL_ResdBit1_BTA4_RX  (NM_BTA4_t.msg.sig.ResdBit1_BTA4)

/*  Tx */
#define IL_ResdBit1_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.ResdBit1_BTA4 = (x);\
}

/*  Rx */
#define IL_PtlNetInfo_BTA4_RX  (NM_BTA4_t.msg.sig.PtlNetInfo_BTA4)

/*  Tx */
#define IL_PtlNetInfo_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PtlNetInfo_BTA4 = (x);\
}

/*  Rx */
#define IL_ResdBit2_BTA4_RX  (NM_BTA4_t.msg.sig.ResdBit2_BTA4)

/*  Tx */
#define IL_ResdBit2_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.ResdBit2_BTA4 = (x);\
}

/*  Rx */
#define IL_PN1_BTA4_RX  (NM_BTA4_t.msg.sig.PN1_BTA4)

/*  Tx */
#define IL_PN1_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN1_BTA4 = (x);\
}

/*  Rx */
#define IL_PN2_BTA4_RX  (NM_BTA4_t.msg.sig.PN2_BTA4)

/*  Tx */
#define IL_PN2_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN2_BTA4 = (x);\
}

/*  Rx */
#define IL_PN3_BTA4_RX  (NM_BTA4_t.msg.sig.PN3_BTA4)

/*  Tx */
#define IL_PN3_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN3_BTA4 = (x);\
}

/*  Rx */
#define IL_PN4_BTA4_RX  (NM_BTA4_t.msg.sig.PN4_BTA4)

/*  Tx */
#define IL_PN4_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN4_BTA4 = (x);\
}

/*  Rx */
#define IL_PN5_BTA4_RX  (NM_BTA4_t.msg.sig.PN5_BTA4)

/*  Tx */
#define IL_PN5_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN5_BTA4 = (x);\
}

/*  Rx */
#define IL_PN6_BTA4_RX  (NM_BTA4_t.msg.sig.PN6_BTA4)

/*  Tx */
#define IL_PN6_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN6_BTA4 = (x);\
}

/*  Rx */
#define IL_PN7_BTA4_RX  (NM_BTA4_t.msg.sig.PN7_BTA4)

/*  Tx */
#define IL_PN7_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN7_BTA4 = (x);\
}

/*  Rx */
#define IL_PN8_BTA4_RX  (NM_BTA4_t.msg.sig.PN8_BTA4)

/*  Tx */
#define IL_PN8_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN8_BTA4 = (x);\
}

/*  Rx */
#define IL_PN9_BTA4_RX  (NM_BTA4_t.msg.sig.PN9_BTA4)

/*  Tx */
#define IL_PN9_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN9_BTA4 = (x);\
}

/*  Rx */
#define IL_PN10_BTA4_RX  (NM_BTA4_t.msg.sig.PN10_BTA4)

/*  Tx */
#define IL_PN10_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN10_BTA4 = (x);\
}

/*  Rx */
#define IL_PN11_BTA4_RX  (NM_BTA4_t.msg.sig.PN11_BTA4)

/*  Tx */
#define IL_PN11_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN11_BTA4 = (x);\
}

/*  Rx */
#define IL_PN12_BTA4_RX  (NM_BTA4_t.msg.sig.PN12_BTA4)

/*  Tx */
#define IL_PN12_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN12_BTA4 = (x);\
}

/*  Rx */
#define IL_PN13_BTA4_RX  (NM_BTA4_t.msg.sig.PN13_BTA4)

/*  Tx */
#define IL_PN13_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN13_BTA4 = (x);\
}

/*  Rx */
#define IL_PN14_BTA4_RX  (NM_BTA4_t.msg.sig.PN14_BTA4)

/*  Tx */
#define IL_PN14_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN14_BTA4 = (x);\
}

/*  Rx */
#define IL_PN15_BTA4_RX  (NM_BTA4_t.msg.sig.PN15_BTA4)

/*  Tx */
#define IL_PN15_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN15_BTA4 = (x);\
}

/*  Rx */
#define IL_PN16_BTA4_RX  (NM_BTA4_t.msg.sig.PN16_BTA4)

/*  Tx */
#define IL_PN16_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN16_BTA4 = (x);\
}

/*  Rx */
#define IL_PN17_BTA4_RX  (NM_BTA4_t.msg.sig.PN17_BTA4)

/*  Tx */
#define IL_PN17_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN17_BTA4 = (x);\
}

/*  Rx */
#define IL_PN18_BTA4_RX  (NM_BTA4_t.msg.sig.PN18_BTA4)

/*  Tx */
#define IL_PN18_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN18_BTA4 = (x);\
}

/*  Rx */
#define IL_PN19_BTA4_RX  (NM_BTA4_t.msg.sig.PN19_BTA4)

/*  Tx */
#define IL_PN19_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN19_BTA4 = (x);\
}

/*  Rx */
#define IL_PN20_BTA4_RX  (NM_BTA4_t.msg.sig.PN20_BTA4)

/*  Tx */
#define IL_PN20_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN20_BTA4 = (x);\
}

/*  Rx */
#define IL_PN21_BTA4_RX  (NM_BTA4_t.msg.sig.PN21_BTA4)

/*  Tx */
#define IL_PN21_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN21_BTA4 = (x);\
}

/*  Rx */
#define IL_PN22_BTA4_RX  (NM_BTA4_t.msg.sig.PN22_BTA4)

/*  Tx */
#define IL_PN22_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN22_BTA4 = (x);\
}

/*  Rx */
#define IL_PN23_BTA4_RX  (NM_BTA4_t.msg.sig.PN23_BTA4)

/*  Tx */
#define IL_PN23_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN23_BTA4 = (x);\
}

/*  Rx */
#define IL_PN24_BTA4_RX  (NM_BTA4_t.msg.sig.PN24_BTA4)

/*  Tx */
#define IL_PN24_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN24_BTA4 = (x);\
}

/*  Rx */
#define IL_PN25_BTA4_RX  (NM_BTA4_t.msg.sig.PN25_BTA4)

/*  Tx */
#define IL_PN25_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN25_BTA4 = (x);\
}

/*  Rx */
#define IL_PN26_BTA4_RX  (NM_BTA4_t.msg.sig.PN26_BTA4)

/*  Tx */
#define IL_PN26_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN26_BTA4 = (x);\
}

/*  Rx */
#define IL_PN27_BTA4_RX  (NM_BTA4_t.msg.sig.PN27_BTA4)

/*  Tx */
#define IL_PN27_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN27_BTA4 = (x);\
}

/*  Rx */
#define IL_PN28_BTA4_RX  (NM_BTA4_t.msg.sig.PN28_BTA4)

/*  Tx */
#define IL_PN28_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN28_BTA4 = (x);\
}

/*  Rx */
#define IL_PN29_BTA4_RX  (NM_BTA4_t.msg.sig.PN29_BTA4)

/*  Tx */
#define IL_PN29_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN29_BTA4 = (x);\
}

/*  Rx */
#define IL_PN30_BTA4_RX  (NM_BTA4_t.msg.sig.PN30_BTA4)

/*  Tx */
#define IL_PN30_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN30_BTA4 = (x);\
}

/*  Rx */
#define IL_PN31_BTA4_RX  (NM_BTA4_t.msg.sig.PN31_BTA4)

/*  Tx */
#define IL_PN31_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN31_BTA4 = (x);\
}

/*  Rx */
#define IL_PN32_BTA4_RX  (NM_BTA4_t.msg.sig.PN32_BTA4)

/*  Tx */
#define IL_PN32_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN32_BTA4 = (x);\
}

/*  Rx */
#define IL_PN33_BTA4_RX  (NM_BTA4_t.msg.sig.PN33_BTA4)

/*  Tx */
#define IL_PN33_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN33_BTA4 = (x);\
}

/*  Rx */
#define IL_PN34_BTA4_RX  (NM_BTA4_t.msg.sig.PN34_BTA4)

/*  Tx */
#define IL_PN34_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN34_BTA4 = (x);\
}

/*  Rx */
#define IL_PN35_BTA4_RX  (NM_BTA4_t.msg.sig.PN35_BTA4)

/*  Tx */
#define IL_PN35_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN35_BTA4 = (x);\
}

/*  Rx */
#define IL_PN36_BTA4_RX  (NM_BTA4_t.msg.sig.PN36_BTA4)

/*  Tx */
#define IL_PN36_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN36_BTA4 = (x);\
}

/*  Rx */
#define IL_PN37_BTA4_RX  (NM_BTA4_t.msg.sig.PN37_BTA4)

/*  Tx */
#define IL_PN37_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN37_BTA4 = (x);\
}

/*  Rx */
#define IL_PN38_BTA4_RX  (NM_BTA4_t.msg.sig.PN38_BTA4)

/*  Tx */
#define IL_PN38_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN38_BTA4 = (x);\
}

/*  Rx */
#define IL_PN39_BTA4_RX  (NM_BTA4_t.msg.sig.PN39_BTA4)

/*  Tx */
#define IL_PN39_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN39_BTA4 = (x);\
}

/*  Rx */
#define IL_PN40_BTA4_RX  (NM_BTA4_t.msg.sig.PN40_BTA4)

/*  Tx */
#define IL_PN40_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN40_BTA4 = (x);\
}

/*  Rx */
#define IL_PN41_BTA4_RX  (NM_BTA4_t.msg.sig.PN41_BTA4)

/*  Tx */
#define IL_PN41_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN41_BTA4 = (x);\
}

/*  Rx */
#define IL_PN42_BTA4_RX  (NM_BTA4_t.msg.sig.PN42_BTA4)

/*  Tx */
#define IL_PN42_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN42_BTA4 = (x);\
}

/*  Rx */
#define IL_PN43_BTA4_RX  (NM_BTA4_t.msg.sig.PN43_BTA4)

/*  Tx */
#define IL_PN43_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN43_BTA4 = (x);\
}

/*  Rx */
#define IL_PN44_BTA4_RX  (NM_BTA4_t.msg.sig.PN44_BTA4)

/*  Tx */
#define IL_PN44_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN44_BTA4 = (x);\
}

/*  Rx */
#define IL_PN45_BTA4_RX  (NM_BTA4_t.msg.sig.PN45_BTA4)

/*  Tx */
#define IL_PN45_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN45_BTA4 = (x);\
}

/*  Rx */
#define IL_PN46_BTA4_RX  (NM_BTA4_t.msg.sig.PN46_BTA4)

/*  Tx */
#define IL_PN46_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN46_BTA4 = (x);\
}

/*  Rx */
#define IL_PN47_BTA4_RX  (NM_BTA4_t.msg.sig.PN47_BTA4)

/*  Tx */
#define IL_PN47_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN47_BTA4 = (x);\
}

/*  Rx */
#define IL_PN48_BTA4_RX  (NM_BTA4_t.msg.sig.PN48_BTA4)

/*  Tx */
#define IL_PN48_BTA4_TX(x)  \
{ \
	NM_BTA4_t.msg.sig.PN48_BTA4 = (x);\
}

/*  Rx */
#define IL_ScrNodId_BTA5_RX  (NM_BTA5_t.msg.sig.ScrNodId_BTA5)

/*  Tx */
#define IL_ScrNodId_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.ScrNodId_BTA5 = (x);\
}

/*  Rx */
#define IL_RepMsgReq_BTA5_RX  (NM_BTA5_t.msg.sig.RepMsgReq_BTA5)

/*  Tx */
#define IL_RepMsgReq_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.RepMsgReq_BTA5 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorn_BTA5_RX  (NM_BTA5_t.msg.sig.NetMngtCoorn_BTA5)

/*  Tx */
#define IL_NetMngtCoorn_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.NetMngtCoorn_BTA5 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorrSleepRdy_BTA5_RX  (NM_BTA5_t.msg.sig.NetMngtCoorrSleepRdy_BTA5)

/*  Tx */
#define IL_NetMngtCoorrSleepRdy_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.NetMngtCoorrSleepRdy_BTA5 = (x);\
}

/*  Rx */
#define IL_ActvWakeup_BTA5_RX  (NM_BTA5_t.msg.sig.ActvWakeup_BTA5)

/*  Tx */
#define IL_ActvWakeup_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.ActvWakeup_BTA5 = (x);\
}

/*  Rx */
#define IL_ResdBit1_BTA5_RX  (NM_BTA5_t.msg.sig.ResdBit1_BTA5)

/*  Tx */
#define IL_ResdBit1_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.ResdBit1_BTA5 = (x);\
}

/*  Rx */
#define IL_PtlNetInfo_BTA5_RX  (NM_BTA5_t.msg.sig.PtlNetInfo_BTA5)

/*  Tx */
#define IL_PtlNetInfo_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PtlNetInfo_BTA5 = (x);\
}

/*  Rx */
#define IL_ResdBit2_BTA5_RX  (NM_BTA5_t.msg.sig.ResdBit2_BTA5)

/*  Tx */
#define IL_ResdBit2_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.ResdBit2_BTA5 = (x);\
}

/*  Rx */
#define IL_PN1_BTA5_RX  (NM_BTA5_t.msg.sig.PN1_BTA5)

/*  Tx */
#define IL_PN1_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN1_BTA5 = (x);\
}

/*  Rx */
#define IL_PN2_BTA5_RX  (NM_BTA5_t.msg.sig.PN2_BTA5)

/*  Tx */
#define IL_PN2_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN2_BTA5 = (x);\
}

/*  Rx */
#define IL_PN3_BTA5_RX  (NM_BTA5_t.msg.sig.PN3_BTA5)

/*  Tx */
#define IL_PN3_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN3_BTA5 = (x);\
}

/*  Rx */
#define IL_PN4_BTA5_RX  (NM_BTA5_t.msg.sig.PN4_BTA5)

/*  Tx */
#define IL_PN4_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN4_BTA5 = (x);\
}

/*  Rx */
#define IL_PN5_BTA5_RX  (NM_BTA5_t.msg.sig.PN5_BTA5)

/*  Tx */
#define IL_PN5_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN5_BTA5 = (x);\
}

/*  Rx */
#define IL_PN6_BTA5_RX  (NM_BTA5_t.msg.sig.PN6_BTA5)

/*  Tx */
#define IL_PN6_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN6_BTA5 = (x);\
}

/*  Rx */
#define IL_PN7_BTA5_RX  (NM_BTA5_t.msg.sig.PN7_BTA5)

/*  Tx */
#define IL_PN7_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN7_BTA5 = (x);\
}

/*  Rx */
#define IL_PN8_BTA5_RX  (NM_BTA5_t.msg.sig.PN8_BTA5)

/*  Tx */
#define IL_PN8_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN8_BTA5 = (x);\
}

/*  Rx */
#define IL_PN9_BTA5_RX  (NM_BTA5_t.msg.sig.PN9_BTA5)

/*  Tx */
#define IL_PN9_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN9_BTA5 = (x);\
}

/*  Rx */
#define IL_PN10_BTA5_RX  (NM_BTA5_t.msg.sig.PN10_BTA5)

/*  Tx */
#define IL_PN10_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN10_BTA5 = (x);\
}

/*  Rx */
#define IL_PN11_BTA5_RX  (NM_BTA5_t.msg.sig.PN11_BTA5)

/*  Tx */
#define IL_PN11_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN11_BTA5 = (x);\
}

/*  Rx */
#define IL_PN12_BTA5_RX  (NM_BTA5_t.msg.sig.PN12_BTA5)

/*  Tx */
#define IL_PN12_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN12_BTA5 = (x);\
}

/*  Rx */
#define IL_PN13_BTA5_RX  (NM_BTA5_t.msg.sig.PN13_BTA5)

/*  Tx */
#define IL_PN13_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN13_BTA5 = (x);\
}

/*  Rx */
#define IL_PN14_BTA5_RX  (NM_BTA5_t.msg.sig.PN14_BTA5)

/*  Tx */
#define IL_PN14_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN14_BTA5 = (x);\
}

/*  Rx */
#define IL_PN15_BTA5_RX  (NM_BTA5_t.msg.sig.PN15_BTA5)

/*  Tx */
#define IL_PN15_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN15_BTA5 = (x);\
}

/*  Rx */
#define IL_PN16_BTA5_RX  (NM_BTA5_t.msg.sig.PN16_BTA5)

/*  Tx */
#define IL_PN16_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN16_BTA5 = (x);\
}

/*  Rx */
#define IL_PN17_BTA5_RX  (NM_BTA5_t.msg.sig.PN17_BTA5)

/*  Tx */
#define IL_PN17_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN17_BTA5 = (x);\
}

/*  Rx */
#define IL_PN18_BTA5_RX  (NM_BTA5_t.msg.sig.PN18_BTA5)

/*  Tx */
#define IL_PN18_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN18_BTA5 = (x);\
}

/*  Rx */
#define IL_PN19_BTA5_RX  (NM_BTA5_t.msg.sig.PN19_BTA5)

/*  Tx */
#define IL_PN19_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN19_BTA5 = (x);\
}

/*  Rx */
#define IL_PN20_BTA5_RX  (NM_BTA5_t.msg.sig.PN20_BTA5)

/*  Tx */
#define IL_PN20_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN20_BTA5 = (x);\
}

/*  Rx */
#define IL_PN21_BTA5_RX  (NM_BTA5_t.msg.sig.PN21_BTA5)

/*  Tx */
#define IL_PN21_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN21_BTA5 = (x);\
}

/*  Rx */
#define IL_PN22_BTA5_RX  (NM_BTA5_t.msg.sig.PN22_BTA5)

/*  Tx */
#define IL_PN22_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN22_BTA5 = (x);\
}

/*  Rx */
#define IL_PN23_BTA5_RX  (NM_BTA5_t.msg.sig.PN23_BTA5)

/*  Tx */
#define IL_PN23_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN23_BTA5 = (x);\
}

/*  Rx */
#define IL_PN24_BTA5_RX  (NM_BTA5_t.msg.sig.PN24_BTA5)

/*  Tx */
#define IL_PN24_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN24_BTA5 = (x);\
}

/*  Rx */
#define IL_PN25_BTA5_RX  (NM_BTA5_t.msg.sig.PN25_BTA5)

/*  Tx */
#define IL_PN25_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN25_BTA5 = (x);\
}

/*  Rx */
#define IL_PN26_BTA5_RX  (NM_BTA5_t.msg.sig.PN26_BTA5)

/*  Tx */
#define IL_PN26_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN26_BTA5 = (x);\
}

/*  Rx */
#define IL_PN27_BTA5_RX  (NM_BTA5_t.msg.sig.PN27_BTA5)

/*  Tx */
#define IL_PN27_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN27_BTA5 = (x);\
}

/*  Rx */
#define IL_PN28_BTA5_RX  (NM_BTA5_t.msg.sig.PN28_BTA5)

/*  Tx */
#define IL_PN28_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN28_BTA5 = (x);\
}

/*  Rx */
#define IL_PN29_BTA5_RX  (NM_BTA5_t.msg.sig.PN29_BTA5)

/*  Tx */
#define IL_PN29_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN29_BTA5 = (x);\
}

/*  Rx */
#define IL_PN30_BTA5_RX  (NM_BTA5_t.msg.sig.PN30_BTA5)

/*  Tx */
#define IL_PN30_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN30_BTA5 = (x);\
}

/*  Rx */
#define IL_PN31_BTA5_RX  (NM_BTA5_t.msg.sig.PN31_BTA5)

/*  Tx */
#define IL_PN31_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN31_BTA5 = (x);\
}

/*  Rx */
#define IL_PN32_BTA5_RX  (NM_BTA5_t.msg.sig.PN32_BTA5)

/*  Tx */
#define IL_PN32_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN32_BTA5 = (x);\
}

/*  Rx */
#define IL_PN33_BTA5_RX  (NM_BTA5_t.msg.sig.PN33_BTA5)

/*  Tx */
#define IL_PN33_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN33_BTA5 = (x);\
}

/*  Rx */
#define IL_PN34_BTA5_RX  (NM_BTA5_t.msg.sig.PN34_BTA5)

/*  Tx */
#define IL_PN34_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN34_BTA5 = (x);\
}

/*  Rx */
#define IL_PN35_BTA5_RX  (NM_BTA5_t.msg.sig.PN35_BTA5)

/*  Tx */
#define IL_PN35_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN35_BTA5 = (x);\
}

/*  Rx */
#define IL_PN36_BTA5_RX  (NM_BTA5_t.msg.sig.PN36_BTA5)

/*  Tx */
#define IL_PN36_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN36_BTA5 = (x);\
}

/*  Rx */
#define IL_PN37_BTA5_RX  (NM_BTA5_t.msg.sig.PN37_BTA5)

/*  Tx */
#define IL_PN37_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN37_BTA5 = (x);\
}

/*  Rx */
#define IL_PN38_BTA5_RX  (NM_BTA5_t.msg.sig.PN38_BTA5)

/*  Tx */
#define IL_PN38_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN38_BTA5 = (x);\
}

/*  Rx */
#define IL_PN39_BTA5_RX  (NM_BTA5_t.msg.sig.PN39_BTA5)

/*  Tx */
#define IL_PN39_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN39_BTA5 = (x);\
}

/*  Rx */
#define IL_PN40_BTA5_RX  (NM_BTA5_t.msg.sig.PN40_BTA5)

/*  Tx */
#define IL_PN40_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN40_BTA5 = (x);\
}

/*  Rx */
#define IL_PN41_BTA5_RX  (NM_BTA5_t.msg.sig.PN41_BTA5)

/*  Tx */
#define IL_PN41_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN41_BTA5 = (x);\
}

/*  Rx */
#define IL_PN42_BTA5_RX  (NM_BTA5_t.msg.sig.PN42_BTA5)

/*  Tx */
#define IL_PN42_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN42_BTA5 = (x);\
}

/*  Rx */
#define IL_PN43_BTA5_RX  (NM_BTA5_t.msg.sig.PN43_BTA5)

/*  Tx */
#define IL_PN43_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN43_BTA5 = (x);\
}

/*  Rx */
#define IL_PN44_BTA5_RX  (NM_BTA5_t.msg.sig.PN44_BTA5)

/*  Tx */
#define IL_PN44_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN44_BTA5 = (x);\
}

/*  Rx */
#define IL_PN45_BTA5_RX  (NM_BTA5_t.msg.sig.PN45_BTA5)

/*  Tx */
#define IL_PN45_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN45_BTA5 = (x);\
}

/*  Rx */
#define IL_PN46_BTA5_RX  (NM_BTA5_t.msg.sig.PN46_BTA5)

/*  Tx */
#define IL_PN46_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN46_BTA5 = (x);\
}

/*  Rx */
#define IL_PN47_BTA5_RX  (NM_BTA5_t.msg.sig.PN47_BTA5)

/*  Tx */
#define IL_PN47_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN47_BTA5 = (x);\
}

/*  Rx */
#define IL_PN48_BTA5_RX  (NM_BTA5_t.msg.sig.PN48_BTA5)

/*  Tx */
#define IL_PN48_BTA5_TX(x)  \
{ \
	NM_BTA5_t.msg.sig.PN48_BTA5 = (x);\
}

/*  Rx */
#define IL_VC_Data_5E0_CRC_RX  (VC_Data_5E0_t.msg.sig.VC_Data_5E0_CRC)

/*  Tx */
#define IL_VC_Data_5E0_CRC_TX(x)  \
{ \
	VC_Data_5E0_t.msg.sig.VC_Data_5E0_CRC = (x);\
}

/*  Rx */
#define IL_VC_Data_5E0_MsgCntr_RX  (VC_Data_5E0_t.msg.sig.VC_Data_5E0_MsgCntr)

/*  Tx */
#define IL_VC_Data_5E0_MsgCntr_TX(x)  \
{ \
	VC_Data_5E0_t.msg.sig.VC_Data_5E0_MsgCntr = (x);\
}

/*  Rx */
#define IL_VC_Data_5E0_Payload_RX  (\
	(VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_0 << 40U) | \
	(VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_1 << 32U) | \
	(VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_2 << 24U) | \
	(VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_3 << 16U) | \
	(VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_4 << 8U) | \
	(VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_5))

/* Tx */
#define IL_VC_Data_5E0_Payload_TX(x)  \
{ \
	VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_0 = ((x) >> 40U) & 255U; \
	VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_1 = ((x) >> 32U) & 255U; \
	VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_2 = ((x) >> 24U) & 255U; \
	VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_3 = ((x) >> 16U) & 255U; \
	VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_4 = ((x) >> 8U) & 255U; \
	VC_Data_5E0_t.msg.sig.VC_Data_5E0_Payload_5 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData0_AllECU_RX  (DIAG_REQ_AllECU_t.msg.sig.DiagReqData0_AllECU)

/*  Tx */
#define IL_DiagReqData0_AllECU_TX(x)  \
{ \
	DIAG_REQ_AllECU_t.msg.sig.DiagReqData0_AllECU = (x);\
}

/*  Rx */
#define IL_DiagReqData1_AllECU_RX  (DIAG_REQ_AllECU_t.msg.sig.DiagReqData1_AllECU)

/*  Tx */
#define IL_DiagReqData1_AllECU_TX(x)  \
{ \
	DIAG_REQ_AllECU_t.msg.sig.DiagReqData1_AllECU = (x);\
}

/*  Rx */
#define IL_DiagReqData2_AllECU_RX  (DIAG_REQ_AllECU_t.msg.sig.DiagReqData2_AllECU)

/*  Tx */
#define IL_DiagReqData2_AllECU_TX(x)  \
{ \
	DIAG_REQ_AllECU_t.msg.sig.DiagReqData2_AllECU = (x);\
}

/*  Rx */
#define IL_DiagReqData3_AllECU_RX  (DIAG_REQ_AllECU_t.msg.sig.DiagReqData3_AllECU)

/*  Tx */
#define IL_DiagReqData3_AllECU_TX(x)  \
{ \
	DIAG_REQ_AllECU_t.msg.sig.DiagReqData3_AllECU = (x);\
}

/*  Rx */
#define IL_DiagReqData4_AllECU_RX  (DIAG_REQ_AllECU_t.msg.sig.DiagReqData4_AllECU)

/*  Tx */
#define IL_DiagReqData4_AllECU_TX(x)  \
{ \
	DIAG_REQ_AllECU_t.msg.sig.DiagReqData4_AllECU = (x);\
}

/*  Rx */
#define IL_DiagReqData5_AllECU_RX  (DIAG_REQ_AllECU_t.msg.sig.DiagReqData5_AllECU)

/*  Tx */
#define IL_DiagReqData5_AllECU_TX(x)  \
{ \
	DIAG_REQ_AllECU_t.msg.sig.DiagReqData5_AllECU = (x);\
}

/*  Rx */
#define IL_DiagReqData6_AllECU_RX  (DIAG_REQ_AllECU_t.msg.sig.DiagReqData6_AllECU)

/*  Tx */
#define IL_DiagReqData6_AllECU_TX(x)  \
{ \
	DIAG_REQ_AllECU_t.msg.sig.DiagReqData6_AllECU = (x);\
}

/*  Rx */
#define IL_DiagReqData7_AllECU_RX  (DIAG_REQ_AllECU_t.msg.sig.DiagReqData7_AllECU)

/*  Tx */
#define IL_DiagReqData7_AllECU_TX(x)  \
{ \
	DIAG_REQ_AllECU_t.msg.sig.DiagReqData7_AllECU = (x);\
}

/*  Rx */
#define IL_DiagReqData0_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData0_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData0_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData0_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData0_BTA4_3))

/* Tx */
#define IL_DiagReqData0_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData0_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData0_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData0_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData0_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData1_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData1_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData1_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData1_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData1_BTA4_3))

/* Tx */
#define IL_DiagReqData1_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData1_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData1_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData1_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData1_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData2_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData2_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData2_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData2_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData2_BTA4_3))

/* Tx */
#define IL_DiagReqData2_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData2_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData2_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData2_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData2_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData3_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData3_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData3_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData3_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData3_BTA4_3))

/* Tx */
#define IL_DiagReqData3_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData3_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData3_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData3_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData3_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData4_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData4_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData4_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData4_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData4_BTA4_3))

/* Tx */
#define IL_DiagReqData4_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData4_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData4_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData4_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData4_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData5_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData5_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData5_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData5_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData5_BTA4_3))

/* Tx */
#define IL_DiagReqData5_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData5_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData5_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData5_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData5_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData6_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData6_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData6_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData6_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData6_BTA4_3))

/* Tx */
#define IL_DiagReqData6_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData6_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData6_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData6_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData6_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData7_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData7_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData7_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData7_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData7_BTA4_3))

/* Tx */
#define IL_DiagReqData7_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData7_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData7_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData7_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData7_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData8_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData8_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData8_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData8_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData8_BTA4_3))

/* Tx */
#define IL_DiagReqData8_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData8_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData8_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData8_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData8_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData9_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData9_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData9_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData9_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData9_BTA4_3))

/* Tx */
#define IL_DiagReqData9_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData9_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData9_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData9_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData9_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData10_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData10_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData10_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData10_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData10_BTA4_3))

/* Tx */
#define IL_DiagReqData10_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData10_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData10_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData10_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData10_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData11_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData11_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData11_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData11_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData11_BTA4_3))

/* Tx */
#define IL_DiagReqData11_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData11_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData11_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData11_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData11_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData12_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData12_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData12_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData12_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData12_BTA4_3))

/* Tx */
#define IL_DiagReqData12_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData12_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData12_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData12_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData12_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData13_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData13_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData13_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData13_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData13_BTA4_3))

/* Tx */
#define IL_DiagReqData13_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData13_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData13_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData13_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData13_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData14_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData14_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData14_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData14_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData14_BTA4_3))

/* Tx */
#define IL_DiagReqData14_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData14_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData14_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData14_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData14_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData15_BTA4_RX  (\
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData15_BTA4_0 << 24U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData15_BTA4_1 << 16U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData15_BTA4_2 << 8U) | \
	(DIAG_REQ_BTA4_t.msg.sig.DiagReqData15_BTA4_3))

/* Tx */
#define IL_DiagReqData15_BTA4_TX(x)  \
{ \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData15_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData15_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData15_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA4_t.msg.sig.DiagReqData15_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData0_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData0_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData0_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData0_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData0_BTA4_3))

/* Tx */
#define IL_DiagRespData0_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData0_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData0_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData0_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData0_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData1_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData1_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData1_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData1_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData1_BTA4_3))

/* Tx */
#define IL_DiagRespData1_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData1_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData1_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData1_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData1_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData2_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData2_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData2_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData2_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData2_BTA4_3))

/* Tx */
#define IL_DiagRespData2_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData2_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData2_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData2_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData2_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData3_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData3_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData3_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData3_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData3_BTA4_3))

/* Tx */
#define IL_DiagRespData3_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData3_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData3_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData3_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData3_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData4_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData4_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData4_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData4_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData4_BTA4_3))

/* Tx */
#define IL_DiagRespData4_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData4_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData4_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData4_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData4_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData5_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData5_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData5_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData5_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData5_BTA4_3))

/* Tx */
#define IL_DiagRespData5_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData5_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData5_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData5_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData5_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData6_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData6_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData6_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData6_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData6_BTA4_3))

/* Tx */
#define IL_DiagRespData6_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData6_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData6_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData6_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData6_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData7_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData7_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData7_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData7_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData7_BTA4_3))

/* Tx */
#define IL_DiagRespData7_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData7_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData7_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData7_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData7_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData8_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData8_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData8_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData8_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData8_BTA4_3))

/* Tx */
#define IL_DiagRespData8_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData8_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData8_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData8_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData8_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData9_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData9_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData9_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData9_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData9_BTA4_3))

/* Tx */
#define IL_DiagRespData9_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData9_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData9_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData9_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData9_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData10_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData10_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData10_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData10_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData10_BTA4_3))

/* Tx */
#define IL_DiagRespData10_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData10_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData10_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData10_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData10_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData11_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData11_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData11_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData11_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData11_BTA4_3))

/* Tx */
#define IL_DiagRespData11_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData11_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData11_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData11_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData11_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData12_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData12_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData12_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData12_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData12_BTA4_3))

/* Tx */
#define IL_DiagRespData12_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData12_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData12_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData12_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData12_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData13_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData13_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData13_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData13_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData13_BTA4_3))

/* Tx */
#define IL_DiagRespData13_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData13_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData13_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData13_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData13_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData14_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData14_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData14_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData14_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData14_BTA4_3))

/* Tx */
#define IL_DiagRespData14_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData14_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData14_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData14_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData14_BTA4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData15_BTA4_RX  (\
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData15_BTA4_0 << 24U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData15_BTA4_1 << 16U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData15_BTA4_2 << 8U) | \
	(DIAG_RESP_BTA4_t.msg.sig.DiagRespData15_BTA4_3))

/* Tx */
#define IL_DiagRespData15_BTA4_TX(x)  \
{ \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData15_BTA4_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData15_BTA4_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData15_BTA4_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA4_t.msg.sig.DiagRespData15_BTA4_3 = (x) & 255U; \
}

void can_msg_init(void);
void can_rx_handle(uint8_t *data, uint32_t can_id, uint8_t len);


#ifdef __cplusplus
}
#endif

#endif
