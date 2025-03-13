/*
* Copyright (c) 2021，上海福宇龙汽车科技有限公司
* All rights reserved.
* @FilePath: ONVO_NT3_Lite_BTA_SRF_V_1_10_0.h
* @abstract:
* @Current_Version: V1.0.0
* @Date: 2024-10-17 13:03:32
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
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_VehSpd_Rdnt1_phy_0_CovFactor (0)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_VehSpd_Rdnt1_phy_0_toS(x) ( (uint8_t) ((x) / (0)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_VehSpd_Rdnt1_phy_0_fromS(x) ( ((x) * (0)) )
// signal: @VehSpd_Rdnt1_phy_1
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_VehSpd_Rdnt1_phy_1_CovFactor (0)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_VehSpd_Rdnt1_phy_1_toS(x) ( (uint8_t) ((x) / (0)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_VehSpd_Rdnt1_phy_1_fromS(x) ( ((x) * (0)) )

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

// def @Posn_Para_Set CAN Message (390  0x186)
#define Posn_Para_Set_IDE (0U)
#define Posn_Para_Set_DLC (16U)
#define Posn_Para_Set_CANID (0x186U)
#define Posn_Para_Set_CYC (20U)

typedef enum
{
    No_Request_18 = 0U,
    UWB_Posn_Mode_19 = 1U,
    BTA_Posn_Mode_20 = 2U,
    Power_Saving_Mode_21 = 3U,
    Enhanced_Off_Mode_22 = 4U,
    BLE_Posn_Mode_23 = 5U,
    Static_Mode_24 = 6U
}VDC_Posn_Mode_Set_Dev1_t;

typedef enum
{
    No_Request_25 = 0U,
    UWB_Posn_Mode_26 = 1U,
    BTA_Posn_Mode_27 = 2U,
    Power_Saving_Mode_28 = 3U,
    Enhanced_Off_Mode_29 = 4U,
    BLE_Posn_Mode_30 = 5U,
    Static_Mode_31 = 6U
}VDC_Posn_Mode_Set_Dev2_t;

typedef enum
{
    No_Request_32 = 0U,
    UWB_Posn_Mode_33 = 1U,
    BTA_Posn_Mode_34 = 2U,
    Power_Saving_Mode_35 = 3U,
    Enhanced_Off_Mode_36 = 4U,
    BLE_Posn_Mode_37 = 5U,
    Static_Mode_38 = 6U
}VDC_Posn_Mode_Set_Dev3_t;

typedef enum
{
    No_Request_39 = 0U,
    UWB_Posn_Mode_40 = 1U,
    BTA_Posn_Mode_41 = 2U,
    Power_Saving_Mode_42 = 3U,
    Enhanced_Off_Mode_43 = 4U,
    BLE_Posn_Mode_44 = 5U,
    Static_Mode_45 = 6U
}VDC_Posn_Mode_Set_Dev4_t;

typedef enum
{
    No_Request_46 = 0U,
    UWB_Posn_Mode_47 = 1U,
    BTA_Posn_Mode_48 = 2U,
    Power_Saving_Mode_49 = 3U,
    Enhanced_Off_Mode_50 = 4U,
    BLE_Posn_Mode_51 = 5U,
    Static_Mode_52 = 6U
}VDC_Posn_Mode_Set_Dev5_t;

typedef enum
{
    Disable_53 = 0U,
    Enable_54 = 1U,
    Reserved_55 = 2U,
    Invalid_56 = 3U
}NFC_Entry_Enable_REQ_t;

typedef union
{
  uint8_t data[16u];
  struct
  {
    // position mode set
    //  0 : "No Request"
    //  1 : "UWB Posn Mode"
    //  2 : "BTA Posn Mode"
    //  3 : "Power Saving Mode"
    //  4 : "Enhanced Off Mode"
    //  5 : "BLE Posn Mode"
    //  6 : "Static Mode"
    VDC_Posn_Mode_Set_Dev1_t VDC_Posn_Mode_Set_Dev1 : 8U;
    // position mode set
    //  0 : "No Request"
    //  1 : "UWB Posn Mode"
    //  2 : "BTA Posn Mode"
    //  3 : "Power Saving Mode"
    //  4 : "Enhanced Off Mode"
    //  5 : "BLE Posn Mode"
    //  6 : "Static Mode"
    VDC_Posn_Mode_Set_Dev2_t VDC_Posn_Mode_Set_Dev2 : 8U;
    // position mode set
    //  0 : "No Request"
    //  1 : "UWB Posn Mode"
    //  2 : "BTA Posn Mode"
    //  3 : "Power Saving Mode"
    //  4 : "Enhanced Off Mode"
    //  5 : "BLE Posn Mode"
    //  6 : "Static Mode"
    VDC_Posn_Mode_Set_Dev3_t VDC_Posn_Mode_Set_Dev3 : 8U;
    // position mode set
    //  0 : "No Request"
    //  1 : "UWB Posn Mode"
    //  2 : "BTA Posn Mode"
    //  3 : "Power Saving Mode"
    //  4 : "Enhanced Off Mode"
    //  5 : "BLE Posn Mode"
    //  6 : "Static Mode"
    VDC_Posn_Mode_Set_Dev4_t VDC_Posn_Mode_Set_Dev4 : 8U;
    // position mode set
    //  0 : "No Request"
    //  1 : "UWB Posn Mode"
    //  2 : "BTA Posn Mode"
    //  3 : "Power Saving Mode"
    //  4 : "Enhanced Off Mode"
    //  5 : "BLE Posn Mode"
    //  6 : "Static Mode"
    VDC_Posn_Mode_Set_Dev5_t VDC_Posn_Mode_Set_Dev5 : 8U;
    // Cyclic ratio
    uint8_t BLE_Cyc_Ratio_Dev1 : 8U;
    // Cyclic ratio
    uint8_t BLE_Cyc_Ratio_Dev2 : 8U;
    // Cyclic ratio
    uint8_t BLE_Cyc_Ratio_Dev3 : 8U;
    // Cyclic ratio
    uint8_t BLE_Cyc_Ratio_Dev4 : 8U;
    // Cyclic ratio
    uint8_t BLE_Cyc_Ratio_Dev5 : 8U;
    // Cyclic ratio
    uint8_t UWB_Cyc_Ratio_Dev1 : 8U;
    // Cyclic ratio
    uint8_t UWB_Cyc_Ratio_Dev2 : 8U;
    // Cyclic ratio
    uint8_t UWB_Cyc_Ratio_Dev3 : 8U;
    // Cyclic ratio
    uint8_t UWB_Cyc_Ratio_Dev4 : 8U;
    // Cyclic ratio
    uint8_t UWB_Cyc_Ratio_Dev5 : 8U;
    // NFC Entry Enable Request
    //  0 : "Disable"
    //  1 : "Enable"
    //  2 : "Reserved"
    //  3 : "Invalid"
    NFC_Entry_Enable_REQ_t NFC_Entry_Enable_REQ : 2U;
    uint32_t reserved0 : 6U;
  }sig;
} Posn_Para_Set_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  Posn_Para_Set_un msg;
}Posn_Para_Set_msg;

// def @BLE_SnifferParam_Dev1 CAN Message (458  0x1ca)
#define BLE_SnifferParam_Dev1_IDE (0U)
#define BLE_SnifferParam_Dev1_DLC (24U)
#define BLE_SnifferParam_Dev1_CANID (0x1caU)
#define BLE_SnifferParam_Dev1_CYC (100U)

typedef enum
{
    No_request_default_value_57 = 0U,
    Request_to_start_sniff_58 = 1U,
    Request_to_stop_sniff_59 = 2U,
    Reserved_60 = 3U
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
    No_request_default_value_61 = 0U,
    Request_to_start_sniff_62 = 1U,
    Request_to_stop_sniff_63 = 2U,
    Reserved_64 = 3U
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
    No_request_default_value_65 = 0U,
    Request_to_start_sniff_66 = 1U,
    Request_to_stop_sniff_67 = 2U,
    Reserved_68 = 3U
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
    No_request_default_value_69 = 0U,
    Request_to_start_sniff_70 = 1U,
    Request_to_stop_sniff_71 = 2U,
    Reserved_72 = 3U
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
    No_request_default_value_73 = 0U,
    Request_to_start_sniff_74 = 1U,
    Request_to_stop_sniff_75 = 2U,
    Reserved_76 = 3U
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

// def @CccDkReq CAN Message (498  0x1f2)
#define CccDkReq_IDE (0U)
#define CccDkReq_DLC (8U)
#define CccDkReq_CANID (0x1f2U)
#define CccDkReq_CYC (50U)

typedef enum
{
    BleOpModeSwReq_no_command_77 = 0U,
    BleOpModeSwReq_enter_op_mode_78 = 1U,
    BleOpModeSwReq_exit_op_mode_79 = 2U
}BleOpModeSwReq_t;

typedef enum
{
    no_command_80 = 0U,
    Disconnect_81 = 1U
}DisconnBleKeyReq_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    // BleOpModeSwReq
    //  0 : "no command"
    //  1 : "enter op mode"
    //  2 : "exit op mode"
    BleOpModeSwReq_t BleOpModeSwReq : 4U;
    uint32_t reserved0 : 4U;

    uint32_t reserved1 : 2U;
    // DisconnBleKeyReq
    //  0 : "no command"
    //  1 : "Disconnect "
    DisconnBleKeyReq_t DisconnBleKeyReq : 2U;
    uint32_t reserved2 : 4U;
    uint8_t DisconnBleKeyDevAddr_0_3_0 : 8U;
    uint8_t DisconnBleKeyDevAddr_0_3_1 : 8U;
    uint8_t DisconnBleKeyDevAddr_0_3_2 : 8U;
    uint8_t DisconnBleKeyDevAddr_0_3_3 : 8U;
    uint8_t DisconnBleKeyDevAddr_4_5_0 : 8U;
    uint8_t DisconnBleKeyDevAddr_4_5_1 : 8U;
  }sig;
} CccDkReq_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  CccDkReq_un msg;
}CccDkReq_msg;

// def @CccBleDkSts CAN Message (500  0x1f4)
#define CccBleDkSts_IDE (0U)
#define CccBleDkSts_DLC (24U)
#define CccBleDkSts_CANID (0x1f4U)
#define CccBleDkSts_CYC (50U)

typedef enum
{
    BleOpModeSts_Not_Op_mode_82 = 0U,
    BleOpModeSts_Op_mode_83 = 1U,
    BleOpModeSts_invalid_84 = 2U,
    BleOpModeSts_invalid_85 = 3U
}BleOpModeSts_t;

typedef enum
{
    UwbConfig_not_supported_86 = 0U,
    UwbConfig_supported_87 = 1U
}UwbConfig_t;

typedef enum
{
    LELRConfig_not_supported_88 = 0U,
    LELRConfig_supported_89 = 1U
}LELRConfig_t;

typedef union
{
  uint8_t data[24u];
  struct
  {
    // Ble owner pairing mode status
    //  0 : "Not Op mode"
    //  1 : "Op mode"
    //  2 : "invalid"
    //  3 : "invalid"
    BleOpModeSts_t BleOpModeSts : 4U;
    // uwb configuration
    //  0 : "not supported"
    //  1 : "supported"
    UwbConfig_t UwbConfig : 2U;
    // LELR configuration
    //  0 : "not supported"
    //  1 : "supported"
    LELRConfig_t LELRConfig : 2U;
    uint8_t BTA1_IRK_0_3_0 : 8U;
    uint8_t BTA1_IRK_0_3_1 : 8U;
    uint8_t BTA1_IRK_0_3_2 : 8U;
    uint8_t BTA1_IRK_0_3_3 : 8U;
    uint8_t BTA1_IRK_4_7_0 : 8U;
    uint8_t BTA1_IRK_4_7_1 : 8U;
    uint8_t BTA1_IRK_4_7_2 : 8U;
    uint8_t BTA1_IRK_4_7_3 : 8U;
    uint8_t BTA1_IRK_8_11_0 : 8U;
    uint8_t BTA1_IRK_8_11_1 : 8U;
    uint8_t BTA1_IRK_8_11_2 : 8U;
    uint8_t BTA1_IRK_8_11_3 : 8U;
    uint8_t BTA1_IRK_12_15_0 : 8U;
    uint8_t BTA1_IRK_12_15_1 : 8U;
    uint8_t BTA1_IRK_12_15_2 : 8U;
    uint8_t BTA1_IRK_12_15_3 : 8U;
    uint8_t BTA1_RSA_0_3_0 : 8U;
    uint8_t BTA1_RSA_0_3_1 : 8U;
    uint8_t BTA1_RSA_0_3_2 : 8U;
    uint8_t BTA1_RSA_0_3_3 : 8U;
    uint8_t BTA1_RSA_4_5_0 : 8U;
    uint8_t BTA1_RSA_4_5_1 : 8U;
    uint32_t reserved0 : 8U;
  }sig;
} CccBleDkSts_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  CccBleDkSts_un msg;
}CccBleDkSts_msg;

// def @BTA6_RSSI_UWB CAN Message (510  0x1fe)
#define BTA6_RSSI_UWB_IDE (0U)
#define BTA6_RSSI_UWB_DLC (32U)
#define BTA6_RSSI_UWB_CANID (0x1feU)
#define BTA6_RSSI_UWB_CYC (50U)
// signal: @BTA6_RSSI_Dev1_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev1_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev1_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev1_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA6_RSSI_Dev2_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev2_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev2_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev2_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA6_RSSI_Dev3_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev3_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev3_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev3_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA6_RSSI_Dev4_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev4_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev4_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev4_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA6_RSSI_Dev5_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev5_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev5_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA6_RSSI_Dev5_phy_fromS(x) ( ((x) + (-255)) )

typedef enum
{
    idle_90 = 0U,
    processing_91 = 1U,
    success_92 = 2U,
    fail_invalid_payload_length_93 = 3U,
    fail_invaild_checksum_94 = 4U
}UWBInfoSyncStsdev1_BTA6_t;

typedef enum
{
    idle_95 = 0U,
    processing_96 = 1U,
    success_97 = 2U,
    fail_invalid_payload_length_98 = 3U,
    fail_invaild_checksum_99 = 4U
}UWBInfoSyncStsdev2_BTA6_t;

typedef enum
{
    idle_100 = 0U,
    processing_101 = 1U,
    success_102 = 2U,
    fail_invalid_payload_length_103 = 3U,
    fail_invaild_checksum_104 = 4U
}UWBInfoSyncStsdev3_BTA6_t;

typedef enum
{
    idle_105 = 0U,
    processing_106 = 1U,
    success_107 = 2U,
    fail_invalid_payload_length_108 = 3U,
    fail_invaild_checksum_109 = 4U
}UWBInfoSyncStsdev4_BTA6_t;

typedef enum
{
    idle_110 = 0U,
    processing_111 = 1U,
    success_112 = 2U,
    fail_invalid_payload_length_113 = 3U,
    fail_invaild_checksum_114 = 4U
}UWBInfoSyncStsdev5_BTA6_t;

typedef enum
{
    none_115 = 0U,
    start_116 = 1U,
    stop_117 = 2U,
    suspend_118 = 3U,
    recovery_119 = 4U,
    restart_120 = 5U
}UWBRangingStsdev1_BTA6_t;

typedef enum
{
    none_121 = 0U,
    start_122 = 1U,
    stop_123 = 2U,
    suspend_124 = 3U,
    recovery_125 = 4U,
    restart_126 = 5U
}UWBRangingStsdev2_BTA6_t;

typedef enum
{
    none_127 = 0U,
    start_128 = 1U,
    stop_129 = 2U,
    suspend_130 = 3U,
    recovery_131 = 4U,
    restart_132 = 5U
}UWBRangingStsdev3_BTA6_t;

typedef enum
{
    none_133 = 0U,
    start_134 = 1U,
    stop_135 = 2U,
    suspend_136 = 3U,
    recovery_137 = 4U,
    restart_138 = 5U
}UWBRangingStsdev4_BTA6_t;

typedef enum
{
    none_139 = 0U,
    start_140 = 1U,
    stop_141 = 2U,
    suspend_142 = 3U,
    recovery_143 = 4U,
    restart_144 = 5U
}UWBRangingStsdev5_BTA6_t;

typedef union
{
  uint8_t data[32u];
  struct
  {
    uint32_t reserved0 : 8U;
    uint8_t BTA6_RSSI_Dev1_phy : 8U;// Offset= -255
    uint8_t BTA6_RSSI_Dev2_phy : 8U;// Offset= -255
    uint8_t BTA6_RSSI_Dev3_phy : 8U;// Offset= -255
    uint8_t BTA6_RSSI_Dev4_phy : 8U;// Offset= -255
    uint8_t BTA6_RSSI_Dev5_phy : 8U;// Offset= -255
    uint8_t BTA6_UWB_Dist_Dev1_0 : 8U;
    uint8_t BTA6_UWB_Dist_Dev1_1 : 8U;
    uint8_t BTA6_UWB_Dist_Dev2_0 : 8U;
    uint8_t BTA6_UWB_Dist_Dev2_1 : 8U;
    uint8_t BTA6_UWB_Dist_Dev3_0 : 8U;
    uint8_t BTA6_UWB_Dist_Dev3_1 : 8U;
    uint8_t BTA6_UWB_Dist_Dev4_0 : 8U;
    uint8_t BTA6_UWB_Dist_Dev4_1 : 8U;
    uint8_t BTA6_UWB_Dist_Dev5_0 : 8U;
    uint8_t BTA6_UWB_Dist_Dev5_1 : 8U;
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWBInfoSyncStsdev1_BTA6_t UWBInfoSyncStsdev1_BTA6 : 8U;
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWBInfoSyncStsdev2_BTA6_t UWBInfoSyncStsdev2_BTA6 : 8U;
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWBInfoSyncStsdev3_BTA6_t UWBInfoSyncStsdev3_BTA6 : 8U;
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWBInfoSyncStsdev4_BTA6_t UWBInfoSyncStsdev4_BTA6 : 8U;
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWBInfoSyncStsdev5_BTA6_t UWBInfoSyncStsdev5_BTA6 : 8U;
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWBRangingStsdev1_BTA6_t UWBRangingStsdev1_BTA6 : 8U;
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWBRangingStsdev2_BTA6_t UWBRangingStsdev2_BTA6 : 8U;
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWBRangingStsdev3_BTA6_t UWBRangingStsdev3_BTA6 : 8U;
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWBRangingStsdev4_BTA6_t UWBRangingStsdev4_BTA6 : 8U;
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWBRangingStsdev5_BTA6_t UWBRangingStsdev5_BTA6 : 8U;
    uint8_t BTA6_RSSI_Channel_Index_Dev2_0 : 2U;
    uint8_t BTA6_RSSI_Channel_Index_Dev1 : 6U;
    uint8_t BTA6_RSSI_Channel_Index_Dev3_0 : 4U;
    uint8_t BTA6_RSSI_Channel_Index_Dev2_1 : 4U;
    uint8_t BTA6_RSSI_Channel_Index_Dev4 : 6U;
    uint8_t BTA6_RSSI_Channel_Index_Dev3_1 : 2U;
    uint32_t reserved1 : 2U;
    uint8_t BTA6_RSSI_Channel_Index_Dev5 : 6U;
    uint32_t reserved2 : 8U;

    uint32_t reserved3 : 8U;
  }sig;
} BTA6_RSSI_UWB_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA6_RSSI_UWB_un msg;
}BTA6_RSSI_UWB_msg;

// def @BTA1_Dev5_Sts CAN Message (514  0x202)
#define BTA1_Dev5_Sts_IDE (0U)
#define BTA1_Dev5_Sts_DLC (32U)
#define BTA1_Dev5_Sts_CANID (0x202U)
#define BTA1_Dev5_Sts_CYC (50U)

typedef enum
{
    Invalid_145 = 0U,
    Static_146 = 1U,
    In_motion_147 = 2U,
    Reserved_148 = 3U
}SmtKey_MotionSts_Dev5_t;

typedef enum
{
    Normal_Running_149 = 0U,
    single_axes_failure_150 = 1U,
    Two_axes_failure_151 = 2U,
    Complete_failure_152 = 3U
}SmtKey_MotionSensorSts_Dev5_t;

typedef enum
{
    Unknown_153 = 0U,
    Lock_154 = 1U,
    Unlock_155 = 2U,
    Reserved_156 = 3U
}SmtKey_ScreenLockSts_Dev5_t;

typedef enum
{
    Invalid_157 = 0U,
    UWBPosnMode_158 = 1U,
    BTAPosnMode_159 = 2U,
    PowerSavingMode_160 = 3U,
    EnhancedOffMode_161 = 4U,
    BLEPosnMode_162 = 5U,
    StaticMode_163 = 6U
}SmtKey_PosnMode_Dev5_t;

typedef enum
{
    Unknown_164 = 0U,
    Key_Fob_165 = 1U,
    NIO_App_166 = 2U,
    CCC_Phone_Key_167 = 3U,
    NIO_Phone_Key_168 = 4U
}SmtKey_DevType_Dev5_t;

typedef enum
{
    Invalid_169 = 0U,
    no_prederived_URSK_170 = 1U,
    one_prederived_URSK_171 = 2U
}UWB_PredURSK_Sts_Dev5_t;

typedef enum
{
    Normal_172 = 0U,
    Battery_Low_173 = 1U
}Keyfob_IsBattLow_Dev5_t;

typedef enum
{
    Invalid_174 = 0U,
    Connected_wo_pairing_175 = 1U,
    Connected_with_notify_disabled_176 = 2U,
    Connected_with_notify_Normal_177 = 3U,
    FobPhone_initiate_disconnection_178 = 4U,
    BTM_MCU_Initiate_disconnection_179 = 5U,
    BTM_BLE_initiate_disconnection_180 = 6U,
    Phone_refuse_BLE_pairing_181 = 7U,
    PhoneFob_enable_notify_timeout_182 = 8U,
    PhoneFob_BLE_pairing_timeout_183 = 9U,
    PhoneFob_BLE_pairing_failed_184 = 10U
}BTA1_Conn_Sts_Dev5_t;

typedef union
{
  uint8_t data[32u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    //  0 : "Invalid"
    //  1 : "Static"
    //  2 : "In motion"
    //  3 : "Reserved"
    SmtKey_MotionSts_Dev5_t SmtKey_MotionSts_Dev5 : 2U;
    //  0 : "Normal Running"
    //  1 : "single axes failure"
    //  2 : "Two axes failure"
    //  3 : "Complete failure"
    SmtKey_MotionSensorSts_Dev5_t SmtKey_MotionSensorSts_Dev5 : 2U;
    //  0 : "Unknown"
    //  1 : "Lock"
    //  2 : "Unlock"
    //  3 : "Reserved"
    SmtKey_ScreenLockSts_Dev5_t SmtKey_ScreenLockSts_Dev5 : 2U;
    uint32_t reserved6 : 2U;
    //  0 : "Invalid "
    //  1 : "UWB_Posn_Mode"
    //  2 : "BTA_Posn_Mode"
    //  3 : "Power_Saving_Mode"
    //  4 : "Enhanced_Off_Mode"
    //  5 : "BLE_Posn_Mode"
    //  6 : "Static_Mode"
    SmtKey_PosnMode_Dev5_t SmtKey_PosnMode_Dev5 : 4U;
    //  0 : "Unknown"
    //  1 : "Key Fob"
    //  2 : "NIO App"
    //  3 : "CCC Phone Key"
    //  4 : "NIO Phone Key"
    SmtKey_DevType_Dev5_t SmtKey_DevType_Dev5 : 4U;
    //  0 : "Invalid"
    //  1 : "no pre-derived URSK"
    //  2 : "one pre-derived URSK"
    UWB_PredURSK_Sts_Dev5_t UWB_PredURSK_Sts_Dev5 : 4U;
    //  0 : "Normal"
    //  1 : "Battery Low"
    Keyfob_IsBattLow_Dev5_t Keyfob_IsBattLow_Dev5 : 4U;
    uint8_t UWB_Locn_Cnt_Dev5 : 8U;
    uint8_t Keyfob_Temp_Dev5 : 8U;
    //  0 : "Invalid"
    //  1 : "Connected w/o pairing"
    //  2 : "Connected with notify disabled"
    //  3 : "Connected with notify (Normal)"
    //  4 : "Fob/Phone initiate disconnection"
    //  5 : "BTM MCU Initiate disconnection"
    //  6 : "BTM BLE initiate disconnection"
    //  7 : "Phone refuse BLE pairing"
    //  8 : "Phone/Fob enable notify timeout"
    //  9 : "Phone/Fob BLE pairing timeout"
    //  10 : "Phone/Fob BLE pairing failed"
    BTA1_Conn_Sts_Dev5_t BTA1_Conn_Sts_Dev5 : 8U;
    uint8_t Keyfob_Batt_Vtg_Dev5_0 : 8U; // Unit:'mV'
    uint8_t Keyfob_Batt_Vtg_Dev5_1 : 8U; // Unit:'mV'
    uint8_t UWB_Locn_Interval_Dev5_0 : 8U; // Unit:'ms'
    uint8_t UWB_Locn_Interval_Dev5_1 : 8U; // Unit:'ms'
    uint8_t Keyfob_Batt_Pct_Dev5 : 8U;
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
  }sig;
} BTA1_Dev5_Sts_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA1_Dev5_Sts_un msg;
}BTA1_Dev5_Sts_msg;

// def @BLE_Common_Sts CAN Message (516  0x204)
#define BLE_Common_Sts_IDE (0U)
#define BLE_Common_Sts_DLC (32U)
#define BLE_Common_Sts_CANID (0x204U)
#define BLE_Common_Sts_CYC (50U)

typedef enum
{
    BTA1_Adv_Sts_Adv_Busy_185 = 0U,
    BTA1_Adv_Sts_Adv_Started_186 = 1U,
    BTA1_Adv_Sts_Adv_Stopped_187 = 2U
}BTA1_Adv_Sts_t;

typedef enum
{
    BTA1_Scan_Sts_Scan_Busy_188 = 0U,
    BTA1_Scan_Sts_Scan_Started_189 = 1U,
    BTA1_Scan_Sts_Scan_Stopped_190 = 2U
}BTA1_Scan_Sts_t;

typedef union
{
  uint8_t data[32u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    //  0 : "Adv Busy"
    //  1 : "Adv Started"
    //  2 : "Adv Stopped"
    BTA1_Adv_Sts_t BTA1_Adv_Sts : 4U;
    //  0 : "Scan Busy"
    //  1 : "Scan Started"
    //  2 : "Scan Stopped"
    BTA1_Scan_Sts_t BTA1_Scan_Sts : 4U;
    uint8_t BTA1_Scan_Interval_0 : 8U; // Unit:'ms'
    uint8_t BTA1_Scan_Interval_1 : 8U; // Unit:'ms'
    uint8_t BTA1_Adv_Interval_0 : 8U; // Unit:'ms'
    uint8_t BTA1_Adv_Interval_1 : 8U; // Unit:'ms'
    uint8_t BTA1_Error_Debug_Sts_0 : 8U;
    uint8_t BTA1_Error_Debug_Sts_1 : 8U;
    uint8_t BTA1_Error_Debug_Sts_2 : 8U;
    uint8_t BTA1_Error_Debug_Sts_3 : 8U;
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
  }sig;
} BLE_Common_Sts_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_Common_Sts_un msg;
}BLE_Common_Sts_msg;

// def @BTA1_Dev1_Sts CAN Message (517  0x205)
#define BTA1_Dev1_Sts_IDE (0U)
#define BTA1_Dev1_Sts_DLC (32U)
#define BTA1_Dev1_Sts_CANID (0x205U)
#define BTA1_Dev1_Sts_CYC (50U)
// signal: @Keyfob_Temp_Dev1_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev1_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev1_phy_toS(x) ( (uint8_t) ((x) - (-40)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev1_phy_fromS(x) ( ((x) + (-40)) )

typedef enum
{
    Invalid_191 = 0U,
    Static_192 = 1U,
    In_motion_193 = 2U,
    Reserved_194 = 3U
}SmtKey_MotionSts_Dev1_t;

typedef enum
{
    Normal_Running_195 = 0U,
    single_axes_failure_196 = 1U,
    Two_axes_failure_197 = 2U,
    Complete_failure_198 = 3U
}SmtKey_MotionSensorErrorSts_Dev1_t;

typedef enum
{
    Unknown_199 = 0U,
    Lock_200 = 1U,
    Unlock_201 = 2U,
    Reserved_202 = 3U
}SmtKey_ScreenLockSts_Dev1_t;

typedef enum
{
    Invalid_203 = 0U,
    UWBPosnMode_204 = 1U,
    BTAPosnMode_205 = 2U,
    PowerSavingMode_206 = 3U,
    EnhancedOffMode_207 = 4U,
    BLEPosnMode_208 = 5U,
    StaticMode_209 = 6U
}SmtKey_PosnMode_Dev1_t;

typedef enum
{
    Unknown_210 = 0U,
    Key_Fob_211 = 1U,
    NIO_App_212 = 2U,
    CCC_Phone_Key_213 = 3U,
    NIO_Phone_Key_214 = 4U
}SmtKey_DevType_Dev1_t;

typedef enum
{
    Invalid_215 = 0U,
    no_prederived_URSK_216 = 1U,
    one_prederived_URSK_217 = 2U
}UWB_PredURSK_Sts_Dev1_t;

typedef enum
{
    Normal_218 = 0U,
    Battery_Low_219 = 1U
}Keyfob_IsBattLow_Dev1_t;

typedef enum
{
    Invalid_220 = 0U,
    Connected_wo_pairing_221 = 1U,
    Connected_with_notify_disabled_222 = 2U,
    Connected_with_notify_Normal_223 = 3U,
    FobPhone_initiate_disconnection_224 = 4U,
    BTM_MCU_Initiate_disconnection_225 = 5U,
    BTM_BLE_initiate_disconnection_226 = 6U,
    Phone_refuse_BLE_pairing_227 = 7U,
    PhoneFob_enable_notify_timeout_228 = 8U,
    PhoneFob_BLE_pairing_timeout_229 = 9U,
    PhoneFob_BLE_pairing_failed_230 = 10U
}BTA1_Conn_Sts_Dev1_t;

typedef union
{
  uint8_t data[32u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    //  0 : "Invalid"
    //  1 : "Static"
    //  2 : "In motion"
    //  3 : "Reserved"
    SmtKey_MotionSts_Dev1_t SmtKey_MotionSts_Dev1 : 2U;
    //  0 : "Normal Running"
    //  1 : "single axes failure"
    //  2 : "Two axes failure"
    //  3 : "Complete failure"
    SmtKey_MotionSensorErrorSts_Dev1_t SmtKey_MotionSensorErrorSts_Dev1 : 2U;
    //  0 : "Unknown"
    //  1 : "Lock"
    //  2 : "Unlock"
    //  3 : "Reserved"
    SmtKey_ScreenLockSts_Dev1_t SmtKey_ScreenLockSts_Dev1 : 2U;
    uint32_t reserved6 : 2U;
    //  0 : "Invalid "
    //  1 : "UWB_Posn_Mode"
    //  2 : "BTA_Posn_Mode"
    //  3 : "Power_Saving_Mode"
    //  4 : "Enhanced_Off_Mode"
    //  5 : "BLE_Posn_Mode"
    //  6 : "Static_Mode"
    SmtKey_PosnMode_Dev1_t SmtKey_PosnMode_Dev1 : 4U;
    //  0 : "Unknown"
    //  1 : "Key Fob"
    //  2 : "NIO App"
    //  3 : "CCC Phone Key"
    //  4 : "NIO Phone Key"
    SmtKey_DevType_Dev1_t SmtKey_DevType_Dev1 : 4U;
    //  0 : "Invalid"
    //  1 : "no pre-derived URSK"
    //  2 : "one pre-derived URSK"
    UWB_PredURSK_Sts_Dev1_t UWB_PredURSK_Sts_Dev1 : 4U;
    //  0 : "Normal"
    //  1 : "Battery Low"
    Keyfob_IsBattLow_Dev1_t Keyfob_IsBattLow_Dev1 : 4U;
    uint8_t UWB_Locn_Cnt_Dev1 : 8U;
    uint8_t Keyfob_Temp_Dev1_phy : 8U;// Offset= -40
    //  0 : "Invalid"
    //  1 : "Connected w/o pairing"
    //  2 : "Connected with notify disabled"
    //  3 : "Connected with notify (Normal)"
    //  4 : "Fob/Phone initiate disconnection"
    //  5 : "BTM MCU Initiate disconnection"
    //  6 : "BTM BLE initiate disconnection"
    //  7 : "Phone refuse BLE pairing"
    //  8 : "Phone/Fob enable notify timeout"
    //  9 : "Phone/Fob BLE pairing timeout"
    //  10 : "Phone/Fob BLE pairing failed"
    BTA1_Conn_Sts_Dev1_t BTA1_Conn_Sts_Dev1 : 8U;
    uint8_t Keyfob_Batt_Vtg_Dev1_0 : 8U; // Unit:'mV'
    uint8_t Keyfob_Batt_Vtg_Dev1_1 : 8U; // Unit:'mV'
    uint8_t UWB_Locn_Interval_Dev1_0 : 8U; // Unit:'ms'
    uint8_t UWB_Locn_Interval_Dev1_1 : 8U; // Unit:'ms'
    uint8_t Keyfob_Batt_Pct_Dev1 : 8U;
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
  }sig;
} BTA1_Dev1_Sts_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA1_Dev1_Sts_un msg;
}BTA1_Dev1_Sts_msg;

// def @BTA1_Dev2_Sts CAN Message (518  0x206)
#define BTA1_Dev2_Sts_IDE (0U)
#define BTA1_Dev2_Sts_DLC (32U)
#define BTA1_Dev2_Sts_CANID (0x206U)
#define BTA1_Dev2_Sts_CYC (50U)
// signal: @Keyfob_Temp_Dev2_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev2_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev2_phy_toS(x) ( (uint8_t) ((x) - (-40)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev2_phy_fromS(x) ( ((x) + (-40)) )

typedef enum
{
    Invalid_231 = 0U,
    Static_232 = 1U,
    In_motion_233 = 2U,
    Reserved_234 = 3U
}SmtKey_MotionSts_Dev2_t;

typedef enum
{
    Normal_Running_235 = 0U,
    single_axes_failure_236 = 1U,
    Two_axes_failure_237 = 2U,
    Complete_failure_238 = 3U
}SmtKey_MotionSensorSts_Dev2_t;

typedef enum
{
    Unknown_239 = 0U,
    Lock_240 = 1U,
    Unlock_241 = 2U,
    Reserved_242 = 3U
}SmtKey_ScreenLockSts_Dev2_t;

typedef enum
{
    Invalid_243 = 0U,
    UWBPosnMode_244 = 1U,
    BTAPosnMode_245 = 2U,
    PowerSavingMode_246 = 3U,
    EnhancedOffMode_247 = 4U,
    BLEPosnMode_248 = 5U,
    StaticMode_249 = 6U
}SmtKey_PosnMode_Dev2_t;

typedef enum
{
    Unknown_250 = 0U,
    Key_Fob_251 = 1U,
    NIO_App_252 = 2U,
    CCC_Phone_Key_253 = 3U,
    NIO_Phone_Key_254 = 4U
}SmtKey_DevType_Dev2_t;

typedef enum
{
    Invalid_255 = 0U,
    no_prederived_URSK_256 = 1U,
    one_prederived_URSK_257 = 2U
}UWB_PredURSK_Sts_Dev2_t;

typedef enum
{
    Normal_258 = 0U,
    Battery_Low_259 = 1U
}Keyfob_IsBattLow_Dev2_t;

typedef enum
{
    Invalid_260 = 0U,
    Connected_wo_pairing_261 = 1U,
    Connected_with_notify_disabled_262 = 2U,
    Connected_with_notify_Normal_263 = 3U,
    FobPhone_initiate_disconnection_264 = 4U,
    BTM_MCU_Initiate_disconnection_265 = 5U,
    BTM_BLE_initiate_disconnection_266 = 6U,
    Phone_refuse_BLE_pairing_267 = 7U,
    PhoneFob_enable_notify_timeout_268 = 8U,
    PhoneFob_BLE_pairing_timeout_269 = 9U,
    PhoneFob_BLE_pairing_failed_270 = 10U
}BTA1_Conn_Sts_Dev2_t;

typedef union
{
  uint8_t data[32u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    //  0 : "Invalid"
    //  1 : "Static"
    //  2 : "In motion"
    //  3 : "Reserved"
    SmtKey_MotionSts_Dev2_t SmtKey_MotionSts_Dev2 : 2U;
    //  0 : "Normal Running"
    //  1 : "single axes failure"
    //  2 : "Two axes failure"
    //  3 : "Complete failure"
    SmtKey_MotionSensorSts_Dev2_t SmtKey_MotionSensorSts_Dev2 : 2U;
    //  0 : "Unknown"
    //  1 : "Lock"
    //  2 : "Unlock"
    //  3 : "Reserved"
    SmtKey_ScreenLockSts_Dev2_t SmtKey_ScreenLockSts_Dev2 : 2U;
    uint32_t reserved6 : 2U;
    //  0 : "Invalid "
    //  1 : "UWB_Posn_Mode"
    //  2 : "BTA_Posn_Mode"
    //  3 : "Power_Saving_Mode"
    //  4 : "Enhanced_Off_Mode"
    //  5 : "BLE_Posn_Mode"
    //  6 : "Static_Mode"
    SmtKey_PosnMode_Dev2_t SmtKey_PosnMode_Dev2 : 4U;
    //  0 : "Unknown"
    //  1 : "Key Fob"
    //  2 : "NIO App"
    //  3 : "CCC Phone Key"
    //  4 : "NIO Phone Key"
    SmtKey_DevType_Dev2_t SmtKey_DevType_Dev2 : 4U;
    //  0 : "Invalid"
    //  1 : "no pre-derived URSK"
    //  2 : "one pre-derived URSK"
    UWB_PredURSK_Sts_Dev2_t UWB_PredURSK_Sts_Dev2 : 4U;
    //  0 : "Normal"
    //  1 : "Battery Low"
    Keyfob_IsBattLow_Dev2_t Keyfob_IsBattLow_Dev2 : 4U;
    uint8_t UWB_Locn_Cnt_Dev2 : 8U;
    uint8_t Keyfob_Temp_Dev2_phy : 8U;// Offset= -40
    //  0 : "Invalid"
    //  1 : "Connected w/o pairing"
    //  2 : "Connected with notify disabled"
    //  3 : "Connected with notify (Normal)"
    //  4 : "Fob/Phone initiate disconnection"
    //  5 : "BTM MCU Initiate disconnection"
    //  6 : "BTM BLE initiate disconnection"
    //  7 : "Phone refuse BLE pairing"
    //  8 : "Phone/Fob enable notify timeout"
    //  9 : "Phone/Fob BLE pairing timeout"
    //  10 : "Phone/Fob BLE pairing failed"
    BTA1_Conn_Sts_Dev2_t BTA1_Conn_Sts_Dev2 : 8U;
    uint8_t Keyfob_Batt_Vtg_Dev2_0 : 8U; // Unit:'mV'
    uint8_t Keyfob_Batt_Vtg_Dev2_1 : 8U; // Unit:'mV'
    uint8_t UWB_Locn_Interval_Dev2_0 : 8U; // Unit:'ms'
    uint8_t UWB_Locn_Interval_Dev2_1 : 8U; // Unit:'ms'
    uint8_t Keyfob_Batt_Pct_Dev2 : 8U;
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
  }sig;
} BTA1_Dev2_Sts_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA1_Dev2_Sts_un msg;
}BTA1_Dev2_Sts_msg;

// def @BTA1_Dev3_Sts CAN Message (519  0x207)
#define BTA1_Dev3_Sts_IDE (0U)
#define BTA1_Dev3_Sts_DLC (32U)
#define BTA1_Dev3_Sts_CANID (0x207U)
#define BTA1_Dev3_Sts_CYC (50U)
// signal: @Keyfob_Temp_Dev3_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev3_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev3_phy_toS(x) ( (uint8_t) ((x) - (-40)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev3_phy_fromS(x) ( ((x) + (-40)) )

typedef enum
{
    Invalid_271 = 0U,
    Static_272 = 1U,
    In_motion_273 = 2U,
    Reserved_274 = 3U
}SmtKey_MotionSts_Dev3_t;

typedef enum
{
    Normal_Running_275 = 0U,
    single_axes_failure_276 = 1U,
    Two_axes_failure_277 = 2U,
    Complete_failure_278 = 3U
}SmtKey_MotionSensorSts_Dev3_t;

typedef enum
{
    Unknown_279 = 0U,
    Lock_280 = 1U,
    Unlock_281 = 2U,
    Reserved_282 = 3U
}SmtKey_ScreenLockSts_Dev3_t;

typedef enum
{
    Invalid_283 = 0U,
    UWBPosnMode_284 = 1U,
    BTAPosnMode_285 = 2U,
    PowerSavingMode_286 = 3U,
    EnhancedOffMode_287 = 4U,
    BLEPosnMode_288 = 5U,
    StaticMode_289 = 6U
}SmtKey_PosnMode_Dev3_t;

typedef enum
{
    Unknown_290 = 0U,
    Key_Fob_291 = 1U,
    NIO_App_292 = 2U,
    CCC_Phone_Key_293 = 3U,
    NIO_Phone_Key_294 = 4U
}SmtKey_DevType_Dev3_t;

typedef enum
{
    Invalid_295 = 0U,
    no_prederived_URSK_296 = 1U,
    one_prederived_URSK_297 = 2U
}UWB_PredURSK_Sts_Dev3_t;

typedef enum
{
    Normal_298 = 0U,
    Battery_Low_299 = 1U
}Keyfob_IsBattLow_Dev3_t;

typedef enum
{
    Invalid_300 = 0U,
    Connected_wo_pairing_301 = 1U,
    Connected_with_notify_disabled_302 = 2U,
    Connected_with_notify_Normal_303 = 3U,
    FobPhone_initiate_disconnection_304 = 4U,
    BTM_MCU_Initiate_disconnection_305 = 5U,
    BTM_BLE_initiate_disconnection_306 = 6U,
    Phone_refuse_BLE_pairing_307 = 7U,
    PhoneFob_enable_notify_timeout_308 = 8U,
    PhoneFob_BLE_pairing_timeout_309 = 9U,
    PhoneFob_BLE_pairing_failed_310 = 10U
}BTA1_Conn_Sts_Dev3_t;

typedef union
{
  uint8_t data[32u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    //  0 : "Invalid"
    //  1 : "Static"
    //  2 : "In motion"
    //  3 : "Reserved"
    SmtKey_MotionSts_Dev3_t SmtKey_MotionSts_Dev3 : 2U;
    //  0 : "Normal Running"
    //  1 : "single axes failure"
    //  2 : "Two axes failure"
    //  3 : "Complete failure"
    SmtKey_MotionSensorSts_Dev3_t SmtKey_MotionSensorSts_Dev3 : 2U;
    //  0 : "Unknown"
    //  1 : "Lock"
    //  2 : "Unlock"
    //  3 : "Reserved"
    SmtKey_ScreenLockSts_Dev3_t SmtKey_ScreenLockSts_Dev3 : 2U;
    uint32_t reserved6 : 2U;
    //  0 : "Invalid "
    //  1 : "UWB_Posn_Mode"
    //  2 : "BTA_Posn_Mode"
    //  3 : "Power_Saving_Mode"
    //  4 : "Enhanced_Off_Mode"
    //  5 : "BLE_Posn_Mode"
    //  6 : "Static_Mode"
    SmtKey_PosnMode_Dev3_t SmtKey_PosnMode_Dev3 : 4U;
    //  0 : "Unknown"
    //  1 : "Key Fob"
    //  2 : "NIO App"
    //  3 : "CCC Phone Key"
    //  4 : "NIO Phone Key"
    SmtKey_DevType_Dev3_t SmtKey_DevType_Dev3 : 4U;
    //  0 : "Invalid"
    //  1 : "no pre-derived URSK"
    //  2 : "one pre-derived URSK"
    UWB_PredURSK_Sts_Dev3_t UWB_PredURSK_Sts_Dev3 : 4U;
    //  0 : "Normal"
    //  1 : "Battery Low"
    Keyfob_IsBattLow_Dev3_t Keyfob_IsBattLow_Dev3 : 4U;
    uint8_t UWB_Locn_Cnt_Dev3 : 8U;
    uint8_t Keyfob_Temp_Dev3_phy : 8U;// Offset= -40
    //  0 : "Invalid"
    //  1 : "Connected w/o pairing"
    //  2 : "Connected with notify disabled"
    //  3 : "Connected with notify (Normal)"
    //  4 : "Fob/Phone initiate disconnection"
    //  5 : "BTM MCU Initiate disconnection"
    //  6 : "BTM BLE initiate disconnection"
    //  7 : "Phone refuse BLE pairing"
    //  8 : "Phone/Fob enable notify timeout"
    //  9 : "Phone/Fob BLE pairing timeout"
    //  10 : "Phone/Fob BLE pairing failed"
    BTA1_Conn_Sts_Dev3_t BTA1_Conn_Sts_Dev3 : 8U;
    uint8_t Keyfob_Batt_Vtg_Dev3_0 : 8U; // Unit:'mV'
    uint8_t Keyfob_Batt_Vtg_Dev3_1 : 8U; // Unit:'mV'
    uint8_t UWB_Locn_Interval_Dev3_0 : 8U; // Unit:'ms'
    uint8_t UWB_Locn_Interval_Dev3_1 : 8U; // Unit:'ms'
    uint8_t Keyfob_Batt_Pct_Dev3 : 8U;
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
  }sig;
} BTA1_Dev3_Sts_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA1_Dev3_Sts_un msg;
}BTA1_Dev3_Sts_msg;

// def @BTA1_Dev4_Sts CAN Message (520  0x208)
#define BTA1_Dev4_Sts_IDE (0U)
#define BTA1_Dev4_Sts_DLC (32U)
#define BTA1_Dev4_Sts_CANID (0x208U)
#define BTA1_Dev4_Sts_CYC (50U)
// signal: @Keyfob_Temp_Dev4_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev4_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev4_phy_toS(x) ( (uint8_t) ((x) - (-40)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Keyfob_Temp_Dev4_phy_fromS(x) ( ((x) + (-40)) )

typedef enum
{
    Invalid_311 = 0U,
    Static_312 = 1U,
    In_motion_313 = 2U,
    Reserved_314 = 3U
}SmtKey_MotionSts_Dev4_t;

typedef enum
{
    Normal_Running_315 = 0U,
    single_axes_failure_316 = 1U,
    Two_axes_failure_317 = 2U,
    Complete_failure_318 = 3U
}SmtKey_MotionSensorSts_Dev4_t;

typedef enum
{
    Unknown_319 = 0U,
    Lock_320 = 1U,
    Unlock_321 = 2U,
    Reserved_322 = 3U
}SmtKey_ScreenLockSts_Dev4_t;

typedef enum
{
    Invalid_323 = 0U,
    UWBPosnMode_324 = 1U,
    BTAPosnMode_325 = 2U,
    PowerSavingMode_326 = 3U,
    EnhancedOffMode_327 = 4U,
    BLEPosnMode_328 = 5U,
    StaticMode_329 = 6U
}SmtKey_PosnMode_Dev4_t;

typedef enum
{
    Unknown_330 = 0U,
    Key_Fob_331 = 1U,
    NIO_App_332 = 2U,
    CCC_Phone_Key_333 = 3U,
    NIO_Phone_Key_334 = 4U
}SmtKey_DevType_Dev4_t;

typedef enum
{
    Invalid_335 = 0U,
    no_prederived_URSK_336 = 1U,
    one_prederived_URSK_337 = 2U
}UWB_PredURSK_Sts_Dev4_t;

typedef enum
{
    Normal_338 = 0U,
    Battery_Low_339 = 1U
}Keyfob_IsBattLow_Dev4_t;

typedef enum
{
    Invalid_340 = 0U,
    Connected_wo_pairing_341 = 1U,
    Connected_with_notify_disabled_342 = 2U,
    Connected_with_notify_Normal_343 = 3U,
    FobPhone_initiate_disconnection_344 = 4U,
    BTM_MCU_Initiate_disconnection_345 = 5U,
    BTM_BLE_initiate_disconnection_346 = 6U,
    Phone_refuse_BLE_pairing_347 = 7U,
    PhoneFob_enable_notify_timeout_348 = 8U,
    PhoneFob_BLE_pairing_timeout_349 = 9U,
    PhoneFob_BLE_pairing_failed_350 = 10U
}BTA1_Conn_Sts_Dev4_t;

typedef union
{
  uint8_t data[32u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
    //  0 : "Invalid"
    //  1 : "Static"
    //  2 : "In motion"
    //  3 : "Reserved"
    SmtKey_MotionSts_Dev4_t SmtKey_MotionSts_Dev4 : 2U;
    //  0 : "Normal Running"
    //  1 : "single axes failure"
    //  2 : "Two axes failure"
    //  3 : "Complete failure"
    SmtKey_MotionSensorSts_Dev4_t SmtKey_MotionSensorSts_Dev4 : 2U;
    //  0 : "Unknown"
    //  1 : "Lock"
    //  2 : "Unlock"
    //  3 : "Reserved"
    SmtKey_ScreenLockSts_Dev4_t SmtKey_ScreenLockSts_Dev4 : 2U;
    uint32_t reserved6 : 2U;
    //  0 : "Invalid "
    //  1 : "UWB_Posn_Mode"
    //  2 : "BTA_Posn_Mode"
    //  3 : "Power_Saving_Mode"
    //  4 : "Enhanced_Off_Mode"
    //  5 : "BLE_Posn_Mode"
    //  6 : "Static_Mode"
    SmtKey_PosnMode_Dev4_t SmtKey_PosnMode_Dev4 : 4U;
    //  0 : "Unknown"
    //  1 : "Key Fob"
    //  2 : "NIO App"
    //  3 : "CCC Phone Key"
    //  4 : "NIO Phone Key"
    SmtKey_DevType_Dev4_t SmtKey_DevType_Dev4 : 4U;
    //  0 : "Invalid"
    //  1 : "no pre-derived URSK"
    //  2 : "one pre-derived URSK"
    UWB_PredURSK_Sts_Dev4_t UWB_PredURSK_Sts_Dev4 : 4U;
    //  0 : "Normal"
    //  1 : "Battery Low"
    Keyfob_IsBattLow_Dev4_t Keyfob_IsBattLow_Dev4 : 4U;
    uint8_t UWB_Locn_Cnt_Dev4 : 8U;
    uint8_t Keyfob_Temp_Dev4_phy : 8U;// Offset= -40
    //  0 : "Invalid"
    //  1 : "Connected w/o pairing"
    //  2 : "Connected with notify disabled"
    //  3 : "Connected with notify (Normal)"
    //  4 : "Fob/Phone initiate disconnection"
    //  5 : "BTM MCU Initiate disconnection"
    //  6 : "BTM BLE initiate disconnection"
    //  7 : "Phone refuse BLE pairing"
    //  8 : "Phone/Fob enable notify timeout"
    //  9 : "Phone/Fob BLE pairing timeout"
    //  10 : "Phone/Fob BLE pairing failed"
    BTA1_Conn_Sts_Dev4_t BTA1_Conn_Sts_Dev4 : 8U;
    uint8_t Keyfob_Batt_Vtg_Dev4_0 : 8U; // Unit:'mV'
    uint8_t Keyfob_Batt_Vtg_Dev4_1 : 8U; // Unit:'mV'
    uint8_t UWB_Locn_Interval_Dev4_0 : 8U; // Unit:'ms'
    uint8_t UWB_Locn_Interval_Dev4_1 : 8U; // Unit:'ms'
    uint8_t Keyfob_Batt_Pct_Dev4 : 8U;
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
  }sig;
} BTA1_Dev4_Sts_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA1_Dev4_Sts_un msg;
}BTA1_Dev4_Sts_msg;

// def @BTA1_RSSI_UWB CAN Message (522  0x20a)
#define BTA1_RSSI_UWB_IDE (0U)
#define BTA1_RSSI_UWB_DLC (16U)
#define BTA1_RSSI_UWB_CANID (0x20aU)
#define BTA1_RSSI_UWB_CYC (50U)
// signal: @BTA1_RSSI_Dev1_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev1_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev1_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev1_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA1_RSSI_Dev2_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev2_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev2_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev2_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA1_RSSI_Dev3_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev3_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev3_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev3_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA1_RSSI_Dev4_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev4_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev4_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev4_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA1_RSSI_Dev5_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev5_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev5_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA1_RSSI_Dev5_phy_fromS(x) ( ((x) + (-255)) )

typedef union
{
  uint8_t data[16u];
  struct
  {
    uint8_t BTA1_RSSI_Dev1_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA1_RSSI_Dev2_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA1_RSSI_Dev3_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA1_RSSI_Dev4_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA1_RSSI_Dev5_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA1_UWB_Dist_Dev1_0 : 8U; // Unit:'cm'
    uint8_t BTA1_UWB_Dist_Dev1_1 : 8U; // Unit:'cm'
    uint8_t BTA1_UWB_Dist_Dev2_0 : 8U; // Unit:'cm'
    uint8_t BTA1_UWB_Dist_Dev2_1 : 8U; // Unit:'cm'
    uint8_t BTA1_UWB_Dist_Dev3_0 : 8U; // Unit:'cm'
    uint8_t BTA1_UWB_Dist_Dev3_1 : 8U; // Unit:'cm'
    uint8_t BTA1_UWB_Dist_Dev4_0 : 8U; // Unit:'cm'
    uint8_t BTA1_UWB_Dist_Dev4_1 : 8U; // Unit:'cm'
    uint8_t BTA1_UWB_Dist_Dev5_0 : 8U; // Unit:'cm'
    uint8_t BTA1_UWB_Dist_Dev5_1 : 8U; // Unit:'cm'
    uint32_t reserved0 : 8U;
  }sig;
} BTA1_RSSI_UWB_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA1_RSSI_UWB_un msg;
}BTA1_RSSI_UWB_msg;

// def @BTA2_RSSI_UWB CAN Message (523  0x20b)
#define BTA2_RSSI_UWB_IDE (0U)
#define BTA2_RSSI_UWB_DLC (16U)
#define BTA2_RSSI_UWB_CANID (0x20bU)
#define BTA2_RSSI_UWB_CYC (50U)
// signal: @BTA2_RSSI_Dev1_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev1_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev1_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev1_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA2_RSSI_Dev2_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev2_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev2_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev2_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA2_RSSI_Dev3_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev3_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev3_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev3_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA2_RSSI_Dev4_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev4_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev4_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev4_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA2_RSSI_Dev5_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev5_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev5_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA2_RSSI_Dev5_phy_fromS(x) ( ((x) + (-255)) )

typedef union
{
  uint8_t data[16u];
  struct
  {
    uint8_t BTA2_RSSI_Dev1_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA2_RSSI_Dev2_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA2_RSSI_Dev3_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA2_RSSI_Dev4_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA2_RSSI_Dev5_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA2_UWB_Dist_Dev1_0 : 8U; // Unit:'cm'
    uint8_t BTA2_UWB_Dist_Dev1_1 : 8U; // Unit:'cm'
    uint8_t BTA2_UWB_Dist_Dev2_0 : 8U; // Unit:'cm'
    uint8_t BTA2_UWB_Dist_Dev2_1 : 8U; // Unit:'cm'
    uint8_t BTA2_UWB_Dist_Dev3_0 : 8U; // Unit:'cm'
    uint8_t BTA2_UWB_Dist_Dev3_1 : 8U; // Unit:'cm'
    uint8_t BTA2_UWB_Dist_Dev4_0 : 8U; // Unit:'cm'
    uint8_t BTA2_UWB_Dist_Dev4_1 : 8U; // Unit:'cm'
    uint8_t BTA2_UWB_Dist_Dev5_0 : 8U; // Unit:'cm'
    uint8_t BTA2_UWB_Dist_Dev5_1 : 8U; // Unit:'cm'
    uint32_t reserved0 : 8U;
  }sig;
} BTA2_RSSI_UWB_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA2_RSSI_UWB_un msg;
}BTA2_RSSI_UWB_msg;

// def @BTA3_RSSI_UWB CAN Message (524  0x20c)
#define BTA3_RSSI_UWB_IDE (0U)
#define BTA3_RSSI_UWB_DLC (16U)
#define BTA3_RSSI_UWB_CANID (0x20cU)
#define BTA3_RSSI_UWB_CYC (50U)
// signal: @BTA3_RSSI_Dev1_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev1_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev1_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev1_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA3_RSSI_Dev2_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev2_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev2_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev2_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA3_RSSI_Dev3_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev3_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev3_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev3_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA3_RSSI_Dev4_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev4_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev4_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev4_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA3_RSSI_Dev5_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev5_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev5_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA3_RSSI_Dev5_phy_fromS(x) ( ((x) + (-255)) )

typedef union
{
  uint8_t data[16u];
  struct
  {
    uint8_t BTA3_RSSI_Dev1_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA3_RSSI_Dev2_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA3_RSSI_Dev3_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA3_RSSI_Dev4_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA3_RSSI_Dev5_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA3_UWB_Dist_Dev1_0 : 8U; // Unit:'cm'
    uint8_t BTA3_UWB_Dist_Dev1_1 : 8U; // Unit:'cm'
    uint8_t BTA3_UWB_Dist_Dev2_0 : 8U; // Unit:'cm'
    uint8_t BTA3_UWB_Dist_Dev2_1 : 8U; // Unit:'cm'
    uint8_t BTA3_UWB_Dist_Dev3_0 : 8U; // Unit:'cm'
    uint8_t BTA3_UWB_Dist_Dev3_1 : 8U; // Unit:'cm'
    uint8_t BTA3_UWB_Dist_Dev4_0 : 8U; // Unit:'cm'
    uint8_t BTA3_UWB_Dist_Dev4_1 : 8U; // Unit:'cm'
    uint8_t BTA3_UWB_Dist_Dev5_0 : 8U; // Unit:'cm'
    uint8_t BTA3_UWB_Dist_Dev5_1 : 8U; // Unit:'cm'
    uint32_t reserved0 : 8U;
  }sig;
} BTA3_RSSI_UWB_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA3_RSSI_UWB_un msg;
}BTA3_RSSI_UWB_msg;

// def @BTA4_RSSI_UWB CAN Message (525  0x20d)
#define BTA4_RSSI_UWB_IDE (0U)
#define BTA4_RSSI_UWB_DLC (32U)
#define BTA4_RSSI_UWB_CANID (0x20dU)
#define BTA4_RSSI_UWB_CYC (50U)
// signal: @BTA4_RSSI_Dev1_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev1_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev1_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev1_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA4_RSSI_Dev2_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev2_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev2_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev2_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA4_RSSI_Dev3_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev3_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev3_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev3_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA4_RSSI_Dev4_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev4_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev4_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev4_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA4_RSSI_Dev5_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev5_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev5_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA4_RSSI_Dev5_phy_fromS(x) ( ((x) + (-255)) )

typedef enum
{
    idle_351 = 0U,
    processing_352 = 1U,
    success_353 = 2U,
    fail_invalid_payload_length_354 = 3U,
    fail_invaild_checksum_355 = 4U
}UWB_Info_Sync_Sts_dev1_t;

typedef enum
{
    idle_356 = 0U,
    processing_357 = 1U,
    success_358 = 2U,
    fail_invalid_payload_length_359 = 3U,
    fail_invaild_checksum_360 = 4U
}UWB_Info_Sync_Sts_dev2_t;

typedef enum
{
    idle_361 = 0U,
    processing_362 = 1U,
    success_363 = 2U,
    fail_invalid_payload_length_364 = 3U,
    fail_invaild_checksum_365 = 4U
}UWB_Info_Sync_Sts_dev3_t;

typedef enum
{
    idle_366 = 0U,
    processing_367 = 1U,
    success_368 = 2U,
    fail_invalid_payload_length_369 = 3U,
    fail_invaild_checksum_370 = 4U
}UWB_Info_Sync_Sts_dev4_t;

typedef enum
{
    idle_371 = 0U,
    processing_372 = 1U,
    success_373 = 2U,
    fail_invalid_payload_length_374 = 3U,
    fail_invaild_checksum_375 = 4U
}UWB_Info_Sync_Sts_dev5_t;

typedef enum
{
    none_376 = 0U,
    start_377 = 1U,
    stop_378 = 2U,
    suspend_379 = 3U,
    recovery_380 = 4U,
    restart_381 = 5U
}UWB_Ranging_Sts_dev1_t;

typedef enum
{
    none_382 = 0U,
    start_383 = 1U,
    stop_384 = 2U,
    suspend_385 = 3U,
    recovery_386 = 4U,
    restart_387 = 5U
}UWB_Ranging_Sts_dev2_t;

typedef enum
{
    none_388 = 0U,
    start_389 = 1U,
    stop_390 = 2U,
    suspend_391 = 3U,
    recovery_392 = 4U,
    restart_393 = 5U
}UWB_Ranging_Sts_dev3_t;

typedef enum
{
    none_394 = 0U,
    start_395 = 1U,
    stop_396 = 2U,
    suspend_397 = 3U,
    recovery_398 = 4U,
    restart_399 = 5U
}UWB_Ranging_Sts_dev4_t;

typedef enum
{
    none_400 = 0U,
    start_401 = 1U,
    stop_402 = 2U,
    suspend_403 = 3U,
    recovery_404 = 4U,
    restart_405 = 5U
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

// def @BTA5_RSSI_UWB CAN Message (526  0x20e)
#define BTA5_RSSI_UWB_IDE (0U)
#define BTA5_RSSI_UWB_DLC (16U)
#define BTA5_RSSI_UWB_CANID (0x20eU)
#define BTA5_RSSI_UWB_CYC (50U)
// signal: @BTA5_RSSI_Dev1_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev1_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev1_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev1_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA5_RSSI_Dev2_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev2_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev2_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev2_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA5_RSSI_Dev3_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev3_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev3_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev3_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA5_RSSI_Dev4_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev4_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev4_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev4_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA5_RSSI_Dev5_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev5_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev5_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA5_RSSI_Dev5_phy_fromS(x) ( ((x) + (-255)) )

typedef union
{
  uint8_t data[16u];
  struct
  {
    uint8_t BTA5_RSSI_Dev1_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA5_RSSI_Dev2_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA5_RSSI_Dev3_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA5_RSSI_Dev4_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA5_RSSI_Dev5_phy : 8U;// Offset= -255               Unit:'dbm'
    uint8_t BTA5_UWB_Dist_Dev1_0 : 8U; // Unit:'cm'
    uint8_t BTA5_UWB_Dist_Dev1_1 : 8U; // Unit:'cm'
    uint8_t BTA5_UWB_Dist_Dev2_0 : 8U; // Unit:'cm'
    uint8_t BTA5_UWB_Dist_Dev2_1 : 8U; // Unit:'cm'
    uint8_t BTA5_UWB_Dist_Dev3_0 : 8U; // Unit:'cm'
    uint8_t BTA5_UWB_Dist_Dev3_1 : 8U; // Unit:'cm'
    uint8_t BTA5_UWB_Dist_Dev4_0 : 8U; // Unit:'cm'
    uint8_t BTA5_UWB_Dist_Dev4_1 : 8U; // Unit:'cm'
    uint8_t BTA5_UWB_Dist_Dev5_0 : 8U; // Unit:'cm'
    uint8_t BTA5_UWB_Dist_Dev5_1 : 8U; // Unit:'cm'
    uint32_t reserved0 : 8U;
  }sig;
} BTA5_RSSI_UWB_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA5_RSSI_UWB_un msg;
}BTA5_RSSI_UWB_msg;

// def @UWB_Info_Ntf CAN Message (546  0x222)
#define UWB_Info_Ntf_IDE (0U)
#define UWB_Info_Ntf_DLC (48U)
#define UWB_Info_Ntf_CANID (0x222U)

typedef enum
{
    Device_Type_invalid_406 = 0U,
    Device_Type_nio_phone_407 = 1U,
    Device_Type_ccc_phone_408 = 2U
}Device_Type_t;

typedef enum
{
    UWB_Session_Req_No_Request_409 = 0U,
    UWB_Session_Req_Ranging_Session_Initiate_410 = 1U,
    UWB_Session_Req_Ranging_Session_Suspend_411 = 2U,
    UWB_Session_Req_Ranging_Session_Recovery_412 = 3U,
    UWB_Session_Req_Ranging_Session_Teminate_413 = 4U,
    UWB_Session_Req_Ranging_Session_Restart_414 = 5U
}UWB_Session_Req_t;

typedef enum
{
    Device_Index_all_415 = 0U,
    Device_Index_device1_416 = 1U,
    Device_Index_device2_417 = 2U,
    Device_Index_device3_418 = 3U,
    Device_Index_device4_419 = 4U,
    Device_Index_device5_420 = 5U
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

// def @BtaOutputRfData CAN Message (659  0x293)
#define BtaOutputRfData_IDE (0U)
#define BtaOutputRfData_DLC (16U)
#define BtaOutputRfData_CANID (0x293U)
// signal: @CurrentTPMSP_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_CurrentTPMSP_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_CurrentTPMSP_phy_toS(x) ( (uint8_t) ((x) / (1)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_CurrentTPMSP_phy_fromS(x) ( ((x) * (1)) )
// signal: @CurrentTPMST_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_CurrentTPMST_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_CurrentTPMST_phy_toS(x) ( (uint8_t) ((x) - (-60)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_CurrentTPMST_phy_fromS(x) ( ((x) + (-60)) )

typedef enum
{
    BalLbtChoose_min350ms_421 = 0U,
    BalLbtChoose_min1000ms_422 = 1U,
    BalLbtChoose_Reserved_423 = 2U,
    BalLbtChoose_Reserved_424 = 3U,
    BalLbtChoose_Reserved_425 = 4U,
    BalLbtChoose_Reserved_426 = 5U,
    BalLbtChoose_Reserved_427 = 6U,
    BalLbtChoose_Reserved_428 = 7U
}BalLbtChoose_t;

typedef enum
{
    Lef_Right_No_LR_function_429 = 0U,
    Lef_Right_AL_left_430 = 1U,
    Lef_Right_AL_right_431 = 2U,
    Lef_Right_Reserved_432 = 3U
}Lef_Right_t;

typedef enum
{
    FrameNumber_frame_1_433 = 0U,
    FrameNumber_frame_2_434 = 1U,
    FrameNumber_frame_3_435 = 2U,
    FrameNumber_Reserved_436 = 3U
}FrameNumber_t;

typedef enum
{
    SensorBALSts_AL_success_437 = 0U,
    SensorBALSts_AL_failure_438 = 1U
}SensorBALSts_t;

typedef enum
{
    WorkMode_Learn_ModeLF_Triggered_439 = 0U,
    WorkMode_Stationary_Mode_440 = 1U,
    WorkMode_ReMeasure_Data_Mode_441 = 2U,
    WorkMode_Rolling_Mode_442 = 3U,
    WorkMode_Enter_OFF_Mode_443 = 4U,
    WorkMode_Exit_OFF_Mode444 = 5U,
    WorkMode_Pressure_Triggered_445 = 6U,
    WorkMode_AutoLocation_Mode_446 = 7U,
    WorkMode_Delay_Mode_447 = 8U,
    WorkMode_Reserved_448 = 9U,
    WorkMode_Reserved_449 = 10U,
    WorkMode_Reserved_450 = 11U,
    WorkMode_Reserved_451 = 12U,
    WorkMode_Reserved_452 = 13U,
    WorkMode_Reserved_453 = 14U,
    WorkMode_Reserved_454 = 15U
}WorkMode_t;

typedef enum
{
    AlartSts_normal_455 = 0U,
    AlartSts_alert_456 = 1U
}AlartSts_t;

typedef enum
{
    LowBattery_battery_ok_457 = 0U,
    LowBattery_battery_low_23V_20_458 = 1U
}LowBattery_t;

typedef enum
{
    Voltage_Value_default_setting_459 = 0U,
    In_Customer_factory_mode_460 = 1U,
    LF_Random_check_code_461 = 2U,
    Accelerate_Value_462 = 3U
}AdvanceFunctionCode_t;

typedef union
{
  uint8_t data[16u];
  struct
  {
    uint8_t BtaOutputRfData_CRC_0 : 8U;
    uint8_t BtaOutputRfData_CRC_1 : 8U;
    uint32_t reserved0 : 8U;
    // Handbyte
    uint8_t Handbyte : 8U;
    // BAL LBT Choose
    //  0 : "min350ms"
    //  1 : "min1000ms"
    //  2 : "Reserved"
    //  3 : "Reserved"
    //  4 : "Reserved"
    //  5 : "Reserved"
    //  6 : "Reserved"
    //  7 : "Reserved"
    BalLbtChoose_t BalLbtChoose : 3U;
    // Left_Right
    //  0 : "No L/R function"
    //  1 : "AL left"
    //  2 : "AL right"
    //  3 : "Reserved"
    Lef_Right_t Lef_Right : 2U;
    // Frame number
    //  0 : "frame 1"
    //  1 : "frame 2"
    //  2 : "frame 3"
    //  3 : "Reserved"
    FrameNumber_t FrameNumber : 2U;
    // status of sensor BAL result
    //  0 : "AL success"
    //  1 : "AL failure"
    SensorBALSts_t SensorBALSts : 1U;
    // work mode
    //  0 : "Learn Mode - LF Triggered"
    //  1 : "Stationary Mode "
    //  2 : "Re-Measure Data Mode "
    //  3 : "Rolling Mode"
    //  4 : "Enter OFF Mode"
    //  5 : "Exit OFF Mode  "
    //  6 : "Pressure Triggered"
    //  7 : "AutoLocation Mode"
    //  8 : "Delay Mode"
    //  9 : "Reserved"
    //  16 : "Reserved"
    //  17 : "Reserved"
    //  18 : "Reserved"
    //  19 : "Reserved"
    //  20 : "Reserved"
    //  21 : "Reserved"
    WorkMode_t WorkMode : 4U;
    // Alart Flag
    //  0 : "normal "
    //  1 : "alert"
    AlartSts_t AlartSts : 1U;
    // Low Battery Indicator
    //  0 : "battery ok"
    //  1 : "battery low (<2.3V&> -20℃)"
    LowBattery_t LowBattery : 1U;
    // Meaning of the Advance Function code byt
    //  0 : "Voltage Value (default setting)"
    //  1 : "In Customer factory mode"
    //  2 : "LF Random check code "
    //  3 : "Accelerate Value"
    AdvanceFunctionCode_t AdvanceFunctionCode : 2U;
    // Current Sensor advance
    // Function code
    uint8_t AdvFunc0 : 8U;
    // Current Sensor ID3
    uint8_t sensorID3 : 8U;
    // Current Sensor ID2
    uint8_t sensorID2 : 8U;
    // Current Sensor ID1
    uint8_t sensorID1 : 8U;
    // Current Sensor ID0
    uint8_t SensorID0 : 8U;
    uint8_t CurrentTPMSP_phy : 8U;// Factor= 1               Unit:'kpa'
    uint8_t CurrentTPMST_phy : 8U;// Offset= -60                Unit:'Deg Cel'
    // RF Signal Rolling Counter
    uint8_t RFSignalRC : 8U;
    uint32_t reserved1 : 8U;

    uint32_t reserved2 : 8U;
  }sig;
} BtaOutputRfData_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BtaOutputRfData_un msg;
}BtaOutputRfData_msg;

// def @BtaOutputRfData1 CAN Message (660  0x294)
#define BtaOutputRfData1_IDE (0U)
#define BtaOutputRfData1_DLC (8U)
#define BtaOutputRfData1_CANID (0x294U)

typedef union
{
  uint8_t data[8u];
  struct
  {
    // The time that BTA transfer TPMS RF signal to ZONE_L (ms)
    uint8_t FrameTime : 8U;
    // RF Frame Time Rolling Counter
    uint8_t RFFrameTimeRC : 8U;
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint32_t reserved3 : 8U;
    uint32_t reserved4 : 8U;
    uint32_t reserved5 : 8U;
  }sig;
} BtaOutputRfData1_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BtaOutputRfData1_un msg;
}BtaOutputRfData1_msg;

// def @BTA_SRF_295 CAN Message (661  0x295)
#define BTA_SRF_295_IDE (0U)
#define BTA_SRF_295_DLC (20U)
#define BTA_SRF_295_CANID (0x295U)
#define BTA_SRF_295_CYC (100U)
// signal: @LDTAftRnTimRsp_Sf_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_LDTAftRnTimRsp_Sf_phy_CovFactor (3)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_LDTAftRnTimRsp_Sf_phy_toS(x) ( (uint8_t) ((x) / (3)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_LDTAftRnTimRsp_Sf_phy_fromS(x) ( ((x) * (3)) )

typedef enum
{
    No_Request_463 = 0U,
    Prestart_Success_464 = 1U,
    Prestart_Unsuccess_465 = 2U,
    Invalid_466 = 3U
}UWBPrestartRsp_Sf_t;

typedef enum
{
    AdltBrthRt_Sf_Detecting_467 = 126U,
    AdltBrthRt_Sf_Invalid_468 = 127U
}AdltBrthRt_Sf_t;

typedef enum
{
    ChildBrthRt_Sf_Detecting_469 = 126U,
    ChildBrthRt_Sf_Invalid_470 = 127U
}ChildBrthRt_Sf_t;

typedef enum
{
    Invalid_471 = 63U
}LDTDetCyclRsp_Sf_t;

typedef enum
{
    LDTWkEnRsp_Sf_Enable_472 = 0U,
    LDTWkEnRsp_Sf_Disable_473 = 1U,
    LDTWkEnRsp_Sf_Invalid_474 = 2U,
    LDTWkEnRsp_Sf_Reserved_475 = 3U
}LDTWkEnRsp_Sf_t;

typedef enum
{
    ChildMotnSts_Sf_lying_476 = 0U,
    ChildMotnSts_Sf_siting_477 = 1U,
    ChildMotnSts_Sf_Moving_478 = 2U,
    ChildMotnSts_Sf_Detecting_479 = 3U,
    ChildMotnSts_Sf_Invalid_480 = 4U
}ChildMotnSts_Sf_t;

typedef enum
{
    Sleeping_481 = 0U,
    Awake_482 = 1U,
    Detecting_483 = 2U,
    Invalid_484 = 3U
}ChildPysclSts_Sf_t;

typedef enum
{
    ChildLDTRslt_Sf_No_occupant_485 = 0U,
    ChildLDTRslt_Sf_Detecting_486 = 1U,
    ChildLDTRslt_Sf_Adult_only_487 = 2U,
    ChildLDTRslt_Sf_Adult_and_child_488 = 3U,
    ChildLDTRslt_Sf_Child_only_489 = 4U,
    ChildLDTRslt_Sf_Invalid_490 = 5U
}ChildLDTRslt_Sf_t;

typedef enum
{
    BASICONOFFSWTSTSINVALID_491 = 0U,
    BASICONOFFSWTSTSOFF_492 = 1U,
    BASICONOFFSWTSTSON_493 = 2U
}ChrgrGunGateOpenReq_t;

typedef enum
{
    None_494 = 0U,
    Present_495 = 1U,
    Detecting_496 = 2U,
    Invalid_497 = 3U
}ChildLctnStsOval_Sf_t;

typedef enum
{
    None_498 = 0U,
    Present_499 = 1U,
    Detecting_500 = 2U,
    Invalid_501 = 3U
}ChildLctnStsFL_Sf_t;

typedef enum
{
    None_502 = 0U,
    Present_503 = 1U,
    Detecting_504 = 2U,
    Invalid_505 = 3U
}ChildLctnStsFR_Sf_t;

typedef enum
{
    None_506 = 0U,
    Present_507 = 1U,
    Detecting_508 = 2U,
    Invalid_509 = 3U
}ChildLctnSts2dL_Sf_t;

typedef enum
{
    None_510 = 0U,
    Present_511 = 1U,
    Detecting_512 = 2U,
    Invalid_513 = 3U
}ChildLctnSts2dM_Sf_t;

typedef enum
{
    None_514 = 0U,
    Present_515 = 1U,
    Detecting_516 = 2U,
    Invalid_517 = 3U
}ChildLctnSts2dR_Sf_t;

typedef enum
{
    None_518 = 0U,
    Present_519 = 1U,
    Detecting_520 = 2U,
    Invalid_521 = 3U
}ChildLctnSts3dL_Sf_t;

typedef enum
{
    None_522 = 0U,
    Present_523 = 1U,
    Detecting_524 = 2U,
    Invalid_525 = 3U
}ChildLctnSts3dR_Sf_t;

typedef enum
{
    None_526 = 0U,
    Present_527 = 1U,
    Detecting_528 = 2U,
    Invalid_529 = 3U
}AdltLctnStsOval_Sf_t;

typedef enum
{
    None_530 = 0U,
    Present_531 = 1U,
    Detecting_532 = 2U,
    Invalid_533 = 3U
}AdltLctnStsFL_Sf_t;

typedef enum
{
    None_534 = 0U,
    Present_535 = 1U,
    Detecting_536 = 2U,
    Invalid_537 = 3U
}AdltLctnStsFR_Sf_t;

typedef enum
{
    None_538 = 0U,
    Present_539 = 1U,
    Detecting_540 = 2U,
    Invalid_541 = 3U
}AdltLctnSts2dL_Sf_t;

typedef enum
{
    None_542 = 0U,
    Present_543 = 1U,
    Detecting_544 = 2U,
    Invalid_545 = 3U
}AdltLctnSts2dM_Sf_t;

typedef enum
{
    None_546 = 0U,
    Present_547 = 1U,
    Detecting_548 = 2U,
    Invalid_549 = 3U
}AdltLctnSts2dR_Sf_t;

typedef enum
{
    None_550 = 0U,
    Present_551 = 1U,
    Detecting_552 = 2U,
    Invalid_553 = 3U
}AdltLctnSts3dL_Sf_t;

typedef enum
{
    None_554 = 0U,
    Present_555 = 1U,
    Detecting_556 = 2U,
    Invalid_557 = 3U
}AdltLctnSts3dR_Sf_t;

typedef enum
{
    UWBLDTEnRsp_Sf_EnableAfter_Run_558 = 0U,
    UWBLDTEnRsp_Sf_EnableNormal_559 = 1U,
    UWBLDTEnRsp_Sf_Disable_560 = 2U,
    UWBLDTEnRsp_Sf_No_Request_561 = 3U,
    UWBLDTEnRsp_Sf_Preparing_562 = 4U,
    UWBLDTEnRsp_Sf_Invalid_563 = 5U,
    UWBLDTEnRsp_Sf_Radar_Error_564 = 6U,
    UWBLDTEnRsp_Sf_Comunication_Error_565 = 7U,
    UWBLDTEnRsp_Sf_Other_Error_566 = 8U
}UWBLDTEnRsp_Sf_t;

typedef enum
{
    UWBLDTRstRsp_Sf_No_Response_567 = 0U,
    UWBLDTRstRsp_Sf_Positive_Response_568 = 1U,
    UWBLDTRstRsp_Sf_Negtive_Response_569 = 2U,
    UWBLDTRstRsp_Sf_Invalid_570 = 3U
}UWBLDTRstRsp_Sf_t;

typedef enum
{
    Range0_571 = 0U,
    Range1_572 = 1U,
    Range2_573 = 2U,
    Range3_574 = 3U,
    Range4_575 = 4U,
    Invalid_576 = 5U
}ChildLDTAgeRange_Sf_t;

typedef enum
{
    No_Response_577 = 0U,
    Positive_Response_578 = 1U,
    Negtive_Response_579 = 2U,
    Invalid_580 = 3U
}LDTBackNoiseLrnRsp_Sf_t;

typedef union
{
  uint8_t data[20u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    // UWB Living Detecting Prestart Command Response
    //  0 : "No Request"
    //  1 : "Prestart Success"
    //  2 : "Prestart Unsuccess"
    //  3 : "Invalid"
    UWBPrestartRsp_Sf_t UWBPrestartRsp_Sf : 3U;
    uint32_t reserved3 : 5U;
    uint8_t LDTAftRnTimRsp_Sf_phy : 8U;// Factor= 3
    // Adult breath rate
    //  126 : "Detecting"
    //  127 : "Invalid"
    AdltBrthRt_Sf_t AdltBrthRt_Sf : 7U;
    uint32_t reserved4 : 1U;
    // Child breath rate
    //  126 : "Detecting"
    //  127 : "Invalid"
    ChildBrthRt_Sf_t ChildBrthRt_Sf : 7U;
    uint32_t reserved5 : 1U;
    // UWB Living Detecting after run detect cycle Response
    //  63 : "Invalid"
    LDTDetCyclRsp_Sf_t LDTDetCyclRsp_Sf : 6U;
    // UWB Living Detecting wake up enable Response
    //  0 : "Enable"
    //  1 : "Disable"
    //  2 : "Invalid "
    //  3 : "Reserved"
    LDTWkEnRsp_Sf_t LDTWkEnRsp_Sf : 2U;
    // Child motion Status
    //  0 : "lying"
    //  1 : "siting"
    //  2 : "Moving"
    //  3 : "Detecting"
    //  4 : "Invalid"
    ChildMotnSts_Sf_t ChildMotnSts_Sf : 4U;
    // Child physiological Status
    //  0 : "Sleeping"
    //  1 : "Awake"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildPysclSts_Sf_t ChildPysclSts_Sf : 4U;
    // Child Living Detecting identify result
    //  0 : "No occupant"
    //  1 : "Detecting"
    //  2 : "Adult only"
    //  3 : "Adult and child"
    //  4 : "Child only"
    //  5 : "Invalid"
    ChildLDTRslt_Sf_t ChildLDTRslt_Sf : 4U;
    // Charger gun gate open request
    //  0 : "BASIC_ON_OFF_SWT_STS_INVALID"
    //  1 : "BASIC_ON_OFF_SWT_STS_OFF"
    //  2 : "BASIC_ON_OFF_SWT_STS_ON"
    ChrgrGunGateOpenReq_t ChrgrGunGateOpenReq : 2U;
    // Child location status Over all
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnStsOval_Sf_t ChildLctnStsOval_Sf : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnStsFL_Sf_t ChildLctnStsFL_Sf : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnStsFR_Sf_t ChildLctnStsFR_Sf : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnSts2dL_Sf_t ChildLctnSts2dL_Sf : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnSts2dM_Sf_t ChildLctnSts2dM_Sf : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnSts2dR_Sf_t ChildLctnSts2dR_Sf : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnSts3dL_Sf_t ChildLctnSts3dL_Sf : 2U;
    // Child location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    ChildLctnSts3dR_Sf_t ChildLctnSts3dR_Sf : 2U;
    // Adult location status Over all
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnStsOval_Sf_t AdltLctnStsOval_Sf : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnStsFL_Sf_t AdltLctnStsFL_Sf : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnStsFR_Sf_t AdltLctnStsFR_Sf : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnSts2dL_Sf_t AdltLctnSts2dL_Sf : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnSts2dM_Sf_t AdltLctnSts2dM_Sf : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnSts2dR_Sf_t AdltLctnSts2dR_Sf : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnSts3dL_Sf_t AdltLctnSts3dL_Sf : 2U;
    // Adult location status
    //  0 : "None"
    //  1 : "Present"
    //  2 : "Detecting"
    //  3 : "Invalid"
    AdltLctnSts3dR_Sf_t AdltLctnSts3dR_Sf : 2U;
    uint32_t reserved6 : 2U;
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
    UWBLDTEnRsp_Sf_t UWBLDTEnRsp_Sf : 4U;
    // UWB Living Detecting Reset Command Response
    //  0 : "No Response"
    //  1 : "Positive Response"
    //  2 : "Negtive Response"
    //  3 : "Invalid"
    UWBLDTRstRsp_Sf_t UWBLDTRstRsp_Sf : 3U;
    uint32_t reserved7 : 1U;
    // Child Living Detecting Identify Age Range
    //  0 : "Range0"
    //  1 : "Range1"
    //  2 : "Range2"
    //  3 : "Range3"
    //  4 : "Range4"
    //  5 : "Invalid"
    ChildLDTAgeRange_Sf_t ChildLDTAgeRange_Sf : 4U;
    // UWB Living Detecting Background Noise Learning Command Response
    //  0 : "No Response"
    //  1 : "Positive Response"
    //  2 : "Negtive Response"
    //  3 : "Invalid"
    LDTBackNoiseLrnRsp_Sf_t LDTBackNoiseLrnRsp_Sf : 3U;
    uint32_t reserved8 : 1U;

    uint32_t reserved9 : 8U;
    uint32_t reserved10 : 8U;
    uint32_t reserved11 : 8U;
    uint32_t reserved12 : 8U;
  }sig;
} BTA_SRF_295_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA_SRF_295_un msg;
}BTA_SRF_295_msg;

// def @ZONE_296 CAN Message (662  0x296)
#define ZONE_296_IDE (0U)
#define ZONE_296_DLC (8U)
#define ZONE_296_CANID (0x296U)
#define ZONE_296_CYC (100U)
// signal: @LDTAftRnTimReq_Sf_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_LDTAftRnTimReq_Sf_phy_CovFactor (3)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_LDTAftRnTimReq_Sf_phy_toS(x) ( (uint8_t) ((x) / (3)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_LDTAftRnTimReq_Sf_phy_fromS(x) ( ((x) * (3)) )

typedef enum
{
    No_Request_581 = 0U,
    Request_582 = 1U,
    Reserved_583 = 2U,
    Invalid_584 = 3U
}LDTBackNoiseLrnCmd_Sf_t;

typedef enum
{
    UWBPrestart_Sf_No_Request_585 = 0U,
    UWBPrestart_Sf_Prestart_586 = 1U,
    UWBPrestart_Sf_Invalid_587 = 2U,
    UWBPrestart_Sf_Reserved_588 = 3U
}UWBPrestart_Sf_t;

typedef enum
{
    UWBLDTEnReq_Sf_EnableAfter_Run_589 = 0U,
    UWBLDTEnReq_Sf_EnableNormal_590 = 1U,
    UWBLDTEnReq_Sf_Disable_591 = 2U,
    UWBLDTEnReq_Sf_No_Request_592 = 3U,
    UWBLDTEnReq_Sf_Prepare_593 = 4U,
    UWBLDTEnReq_Sf_Invalid_594 = 5U
}UWBLDTEnReq_Sf_t;

typedef enum
{
    UWBLDTRstCmd_Sf_No_Request_595 = 0U,
    UWBLDTRstCmd_Sf_Request_596 = 1U,
    UWBLDTRstCmd_Sf_Reserved_597 = 2U,
    UWBLDTRstCmd_Sf_Invalid_598 = 3U
}UWBLDTRstCmd_Sf_t;

typedef enum
{
    Invalid_599 = 63U
}LDTDetCyclReq_Sf_t;

typedef enum
{
    LDTWkEnReq_Sf_Enable_600 = 0U,
    LDTWkEnReq_Sf_Disable_601 = 1U,
    LDTWkEnReq_Sf_Invalid_602 = 2U,
    LDTWkEnReq_Sf_Reserved_603 = 3U
}LDTWkEnReq_Sf_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 2U;
    // UWB Living Detecting Background Noise Learning Command
    //  0 : "No Request"
    //  1 : "Request"
    //  2 : "Reserved"
    //  3 : "Invalid"
    LDTBackNoiseLrnCmd_Sf_t LDTBackNoiseLrnCmd_Sf : 2U;
    uint32_t reserved3 : 2U;
    // UWB Living Detecting Prestart
    //  0 : "No Request"
    //  1 : "Prestart"
    //  2 : "Invalid"
    //  3 : "Reserved"
    UWBPrestart_Sf_t UWBPrestart_Sf : 2U;
    uint32_t reserved4 : 3U;
    // UWB Living Detecting enable Request
    //  0 : "Enable+After Run"
    //  1 : "Enable+Normal"
    //  2 : "Disable"
    //  3 : "No Request"
    //  4 : "Prepare"
    //  5 : "Invalid"
    UWBLDTEnReq_Sf_t UWBLDTEnReq_Sf : 3U;
    // UWB Living Detecting Reset Command
    //  0 : "No Request"
    //  1 : "Request"
    //  2 : "Reserved"
    //  3 : "Invalid"
    UWBLDTRstCmd_Sf_t UWBLDTRstCmd_Sf : 2U;
    uint8_t LDTAftRnTimReq_Sf_phy : 8U;// Factor= 3
    uint32_t reserved5 : 8U;

    uint32_t reserved6 : 8U;
    // UWB Living Detecting after run detect cycle Request
    //  63 : "Invalid"
    LDTDetCyclReq_Sf_t LDTDetCyclReq_Sf : 6U;
    // UWB Living Detecting wake up enable Request
    //  0 : "Enable"
    //  1 : "Disable"
    //  2 : "Invalid "
    //  3 : "Reserved"
    LDTWkEnReq_Sf_t LDTWkEnReq_Sf : 2U;
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
    Invalid_604 = 255U
}FusnData_2rlSt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_605 = 255U
}FusnData_2rlSt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_606 = 255U
}FusnData_2rlSt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_607 = 255U
}FusnData_2rlSt_Reserved_Sr_t;

typedef enum
{
    Invalid_608 = 255U
}FusnData_2rmSt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_609 = 255U
}FusnData_2rmSt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_610 = 255U
}FusnData_2rmSt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_611 = 255U
}FusnData_2rmSt_Reserved_Sr_t;

typedef enum
{
    Invalid_612 = 255U
}FusnData_2rrSt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_613 = 255U
}FusnData_2rrSt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_614 = 255U
}FusnData_2rrSt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_615 = 255U
}FusnData_2rrSt_Reserved_Sr_t;

typedef enum
{
    Invalid_616 = 255U
}FusnData_2rlFt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_617 = 255U
}FusnData_2rlFt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_618 = 255U
}FusnData_2rlFt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_619 = 255U
}FusnData_2rlFt_Reserved_Sr_t;

typedef enum
{
    Invalid_620 = 255U
}FusnData_2rmFt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_621 = 255U
}FusnData_2rmFt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_622 = 255U
}FusnData_2rmFt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_623 = 255U
}FusnData_2rmFt_Reserved_Sr_t;

typedef enum
{
    Invalid_624 = 255U
}FusnData_2rrFt_BrthEngy_Sr_t;

typedef enum
{
    Invalid_625 = 255U
}FusnData_2rrFt_AdltBrthRate_Sr_t;

typedef enum
{
    Invalid_626 = 255U
}FusnData_2rrFt_ChildBrthRate_Sr_t;

typedef enum
{
    Invalid_627 = 255U
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

// def @BTA1_01 CAN Message (795  0x31b)
#define BTA1_01_IDE (0U)
#define BTA1_01_DLC (16U)
#define BTA1_01_CANID (0x31bU)
#define BTA1_01_CYC (200U)

typedef enum
{
    No_failure_628 = 0U,
    Flashing_failure_629 = 1U,
    reserved_630 = 2U,
    reserved_631 = 3U
}FlashingSts_BTA1_t;

typedef enum
{
    No_fault_632 = 0U,
    RSS_fault_detected_633 = 1U,
    reserved_634 = 2U,
    reserved_635 = 3U
}RSSAbnormalSts_BTA1_t;

typedef enum
{
    CANNetwork_BTA1_Repeat_message_state_636 = 0U,
    CANNetwork_BTA1_Normal_operation_state_637 = 1U,
    CANNetwork_BTA1_Ready_sleep_state_638 = 2U,
    CANNetwork_BTA1_Reserved_639 = 3U
}CANNetwork_BTA1_t;

typedef enum
{
    OS_BTA1_Operation_system_normal_640 = 0U,
    OS_BTA1_Operation_system_error_641 = 1U,
    OS_BTA1_Reserved_642 = 2U,
    OS_BTA1_Reserved_643 = 3U
}OS_BTA1_t;

typedef enum
{
    Function_in_progress_644 = 0U,
    Vehicle_status_not_parked_645 = 1U,
    Diagnostic_service_646 = 2U,
    Reserved_647 = 3U,
    Reserved_648 = 4U,
    Reserved_649 = 5U,
    Reserved_650 = 6U,
    Reserved_651 = 7U
}WUKeepSource_BTA1_t;

typedef enum
{
    WUSource_BTA1_Local_hardware_WU_652 = 0U,
    WUSource_BTA1_CAN_NM_Received_653 = 1U,
    WUSource_BTA1_LIN_WU_654 = 2U,
    WUSource_BTA1_Radio_Frequency_WU_655 = 3U,
    WUSource_BTA1_Reserved_656 = 4U,
    WUSource_BTA1_Reserved_657 = 5U,
    WUSource_BTA1_Reserved_658 = 6U,
    WUSource_BTA1_Reserved_659 = 7U
}WUSource_BTA1_t;

typedef enum
{
    SoftReset_BTA1_no_software_reset_660 = 0U,
    SoftReset_BTA1_diagnostic_reset_request_661 = 1U,
    SoftReset_BTA1_watch_dog_reset_external_pin_662 = 2U,
    SoftReset_BTA1_clock_abnormal_reset_663 = 3U,
    SoftReset_BTA1_internal_software_reset_664 = 4U,
    SoftReset_BTA1_reserved_665 = 5U,
    SoftReset_BTA1_reserved_666 = 6U,
    SoftReset_BTA1_reserved_667 = 7U
}SoftReset_BTA1_t;

typedef enum
{
    HardReset_BTA1_no_hard_reset_wake_up_power_on_668 = 0U,
    HardReset_BTA1_hard_reset_669 = 1U,
    HardReset_BTA1_reserved_670 = 2U,
    HardReset_BTA1_reserved_671 = 3U
}HardReset_BTA1_t;

typedef enum
{
    WD_BTA1_Feed_watchdog_normal_672 = 0U,
    WD_BTA1_Feed_watchdog_abnormal_673 = 1U,
    WD_BTA1_Reserved_674 = 2U,
    WD_BTA1_Reserved_675 = 3U
}WD_BTA1_t;

typedef enum
{
    CommDTCs_BTA1_Body_CAN_bus_off_676 = 0U,
    CommDTCs_BTA1_LIN_bus_short_to_battery_677 = 1U,
    CommDTCs_BTA1_LIN_bus_short_to_Ground_678 = 2U,
    CommDTCs_BTA1_Lost_communication_with_Zone_679 = 3U,
    CommDTCs_BTA1_Lost_communication_with_other_BTA_680 = 4U,
    CommDTCs_BTA1_reserved_681 = 5U,
    CommDTCs_BTA1_reserved_682 = 6U,
    CommDTCs_BTA1_reserved_683 = 7U
}CommDTCs_BTA1_t;

typedef enum
{
    No_error_684 = 0U,
    over_temperature_error_685 = 1U,
    short_circuitopen_circuit_error_686 = 2U,
    voltage_error_687 = 3U,
    SPI_error_688 = 4U,
    reserved_689 = 5U,
    reserved_690 = 6U,
    reserved_691 = 7U
}MCUSoCFault_BTA1_t;

typedef enum
{
    No_SPI_error_692 = 0U,
    SPI_error_693 = 1U,
    Reserved_694 = 2U,
    Reserved_695 = 3U
}MCUSoCSPICom_BTA1_t;

typedef enum
{
    Others_BTA1_No_error_696 = 0U,
    Others_BTA1_Other_DTCs_697 = 1U,
    Others_BTA1_Reserved_698 = 2U,
    Others_BTA1_Reserved_699 = 3U
}Others_BTA1_t;

typedef enum
{
    SBCFault_BTA1_No_error_700 = 0U,
    SBCFault_BTA1_over_temperature_error_701 = 1U,
    SBCFault_BTA1_short_circuitopen_circuit_error_702 = 2U,
    SBCFault_BTA1_voltage_error_703 = 3U,
    SBCFault_BTA1_SPI_error_704 = 4U,
    SBCFault_BTA1_reserved_705 = 5U,
    SBCFault_BTA1_reserved_706 = 6U,
    SBCFault_BTA1_reserved_707 = 7U
}SBCFault_BTA1_t;

typedef enum
{
    No_error_708 = 0U,
    over_temperature_error_709 = 1U,
    short_circuitopen_circuit_error_710 = 2U,
    voltage_error_711 = 3U,
    SPI_error_712 = 4U,
    reserved_713 = 5U,
    reserved_714 = 6U,
    reserved_715 = 7U
}Peripheral_Fault_BTA1_t;

typedef enum
{
    No_signals_received_716 = 0U,
    signals_check_sum_failed_717 = 1U,
    check_sum_success_718 = 2U,
    Reserved_719 = 3U
}EcuCRstatus_BTA1_t;

typedef enum
{
    No_error_720 = 0U,
    Power_supply_voltage_above_threshold_721 = 1U,
    Power_supply_voltage_below_threshold_722 = 2U,
    Reserved_723 = 3U
}PowerSupplyDTCs_BTA1_t;

typedef enum
{
    Idle_mode_724 = 0U,
    BTAWDL_725 = 1U,
    BTACPD_726 = 2U,
    BTATPMS_727 = 3U,
    reserved_728 = 4U,
    reserved_729 = 5U,
    reserved_730 = 6U,
    reserved_731 = 7U
}RFsysTDmode_BTA1_t;

typedef enum
{
    Idle_mode_732 = 0U,
    Sleep_mode_733 = 1U,
    polling_mode_734 = 2U,
    active_mode_735 = 3U,
    reserved_736 = 4U,
    reserved_737 = 5U,
    reserved_738 = 6U,
    reserved_739 = 7U
}Rfsendermode_BTA1_t;

typedef enum
{
    Idle_mode_740 = 0U,
    Sleep_mode_741 = 1U,
    polling_mode_742 = 2U,
    active_mode_743 = 3U,
    reserved_744 = 4U,
    reserved_745 = 5U,
    reserved_746 = 6U,
    reserved_747 = 7U
}Rfreceivermode_BTA1_t;

typedef enum
{
    No_authentication_748 = 0U,
    Authentication_failed_749 = 1U,
    Authentication_success_750 = 2U,
    Authentication_start_751 = 3U,
    Authentication_idle_752 = 4U,
    reserved_753 = 5U,
    reserved_754 = 6U,
    reserved_755 = 7U
}ECUAuthProstatus_BTA1_t;

typedef enum
{
    No_authentication_756 = 0U,
    vehicle_node_SK_ECU_ID_or_VIN_encryption_not_passed_757 = 1U,
    Outside_node_SK_not_passed_758 = 2U,
    Check_sum_not_passed_759 = 3U,
    Random_or_RSA_not_passed_760 = 4U,
    Response_time_out_761 = 5U,
    reserved_762 = 6U,
    reserved_763 = 7U
}ECUAuthFailReasons_BTA1_t;

typedef enum
{
    No_trigger_764 = 0U,
    Triggered_765 = 1U,
    Reserved_766 = 2U,
    Reserved_767 = 3U
}InterEEPTimes_BTA1_t;

typedef union
{
  uint8_t data[16u];
  struct
  {
    uint8_t ECUPowSuppVol_BTA1 : 8U;
    //  0 : "No failure"
    //  1 : "Flashing failure"
    //  2 : "reserved"
    //  3 : "reserved"
    FlashingSts_BTA1_t FlashingSts_BTA1 : 2U;
    //  0 : "No fault"
    //  1 : "RSS fault detected"
    //  2 : "reserved"
    //  3 : "reserved"
    RSSAbnormalSts_BTA1_t RSSAbnormalSts_BTA1 : 2U;
    uint32_t reserved0 : 2U;
    //  0 : "Repeat message state"
    //  1 : "Normal operation state"
    //  2 : "Ready sleep state"
    //  3 : "Reserved"
    CANNetwork_BTA1_t CANNetwork_BTA1 : 2U;
    //  0 : "Operation system normal"
    //  1 : "Operation system error"
    //  2 : "Reserved"
    //  3 : "Reserved"
    OS_BTA1_t OS_BTA1 : 2U;
    //  0 : "Function in progress"
    //  1 : "Vehicle status= not parked"
    //  2 : "Diagnostic service"
    //  3 : "Reserved"
    //  4 : "Reserved"
    //  5 : "Reserved"
    //  6 : "Reserved"
    //  7 : "Reserved"
    WUKeepSource_BTA1_t WUKeepSource_BTA1 : 3U;
    //  0 : "Local hardware WU"
    //  1 : "CAN NM Received"
    //  2 : "LIN WU"
    //  3 : "Radio Frequency WU"
    //  4 : "Reserved"
    //  5 : "Reserved"
    //  6 : "Reserved"
    //  7 : "Reserved"
    WUSource_BTA1_t WUSource_BTA1 : 3U;
    //  0 : "no software reset"
    //  1 : "diagnostic reset request"
    //  2 : "watch dog reset (external pin)"
    //  3 : "clock abnormal reset"
    //  4 : "internal software reset"
    //  5 : "reserved"
    //  6 : "reserved"
    //  7 : "reserved"
    SoftReset_BTA1_t SoftReset_BTA1 : 3U;
    //  0 : "no hard reset (wake up power on)"
    //  1 : "hard reset"
    //  2 : "reserved"
    //  3 : "reserved"
    HardReset_BTA1_t HardReset_BTA1 : 2U;
    //  0 : "Feed watchdog normal"
    //  1 : "Feed watchdog abnormal"
    //  2 : "Reserved"
    //  3 : "Reserved"
    WD_BTA1_t WD_BTA1 : 3U;
    //  0 : "Body CAN bus off"
    //  1 : "LIN bus short to battery"
    //  2 : "LIN bus short to Ground"
    //  3 : "Lost communication with Zone"
    //  4 : "Lost communication with other BTA"
    //  5 : "reserved"
    //  6 : "reserved"
    //  7 : "reserved"
    CommDTCs_BTA1_t CommDTCs_BTA1 : 3U;
    //  0 : "No error"
    //  1 : "over temperature error"
    //  2 : "short circuit/open circuit error"
    //  3 : "voltage error"
    //  4 : "SPI error"
    //  5 : "reserved"
    //  6 : "reserved"
    //  7 : "reserved"
    MCUSoCFault_BTA1_t MCUSoCFault_BTA1 : 3U;
    //  0 : "No SPI error"
    //  1 : "SPI error"
    //  2 : "Reserved"
    //  3 : "Reserved"
    MCUSoCSPICom_BTA1_t MCUSoCSPICom_BTA1 : 2U;
    uint8_t MCUSoCTemp_BTA1 : 8U;
    uint8_t PeripheralChipTempOne_BTA1 : 8U;
    uint8_t PeripheralChipTempTwo_BTA1 : 8U;
    //  0 : "No error"
    //  1 : "Other DTCs"
    //  2 : "Reserved"
    //  3 : "Reserved"
    Others_BTA1_t Others_BTA1 : 2U;
    //  0 : "No error"
    //  1 : "over temperature error"
    //  2 : "short circuit/open circuit error"
    //  3 : "voltage error"
    //  4 : "SPI error"
    //  5 : "reserved"
    //  6 : "reserved"
    //  7 : "reserved"
    SBCFault_BTA1_t SBCFault_BTA1 : 3U;
    //  0 : "No error"
    //  1 : "over temperature error"
    //  2 : "short circuit/open circuit error"
    //  3 : "voltage error"
    //  4 : "SPI error"
    //  5 : "reserved"
    //  6 : "reserved"
    //  7 : "reserved"
    Peripheral_Fault_BTA1_t Peripheral_Fault_BTA1 : 3U;
    uint32_t reserved1 : 1U;
    //  0 : "No signals received"
    //  1 : "signals check sum failed"
    //  2 : "check sum success"
    //  3 : "Reserved"
    EcuCRstatus_BTA1_t EcuCRstatus_BTA1 : 2U;
    //  0 : "No error"
    //  1 : "Power supply voltage above threshold"
    //  2 : "Power supply voltage below threshold"
    //  3 : "Reserved"
    PowerSupplyDTCs_BTA1_t PowerSupplyDTCs_BTA1 : 2U;
    //  0 : "Idle mode"
    //  1 : "BTA_WDL"
    //  2 : "BTA_CPD"
    //  3 : "BTA_TPMS"
    //  4 : "reserved"
    //  5 : "reserved"
    //  6 : "reserved"
    //  7 : "reserved"
    RFsysTDmode_BTA1_t RFsysTDmode_BTA1 : 3U;
    uint32_t reserved2 : 2U;
    //  0 : "Idle mode"
    //  1 : "Sleep mode"
    //  2 : "polling mode"
    //  3 : "active mode"
    //  4 : "reserved"
    //  5 : "reserved"
    //  6 : "reserved"
    //  7 : "reserved"
    Rfsendermode_BTA1_t Rfsendermode_BTA1 : 3U;
    //  0 : "Idle mode"
    //  1 : "Sleep mode"
    //  2 : "polling mode"
    //  3 : "active mode"
    //  4 : "reserved"
    //  5 : "reserved"
    //  6 : "reserved"
    //  7 : "reserved"
    Rfreceivermode_BTA1_t Rfreceivermode_BTA1 : 3U;
    uint8_t RfC1Rvalue_BTA1 : 8U;
    uint8_t RfC2Rvalue_BTA1 : 8U;
    uint32_t reserved3 : 2U;
    //  0 : "No authentication"
    //  1 : "Authentication failed"
    //  2 : "Authentication success"
    //  3 : "Authentication start"
    //  4 : "Authentication idle"
    //  5 : "reserved"
    //  6 : "reserved"
    //  7 : "reserved"
    ECUAuthProstatus_BTA1_t ECUAuthProstatus_BTA1 : 3U;
    //  0 : "No authentication"
    //  1 : "vehicle node SK (ECU ID or VIN encryption) not passed"
    //  2 : "Outside node SK not passed"
    //  3 : "Check sum not passed"
    //  4 : "Random or RSA not passed"
    //  5 : "Response time out"
    //  6 : "reserved"
    //  7 : "reserved"
    ECUAuthFailReasons_BTA1_t ECUAuthFailReasons_BTA1 : 3U;
    uint32_t reserved4 : 5U;
    //  0 : "No trigger"
    //  1 : "Triggered"
    //  2 : "Reserved"
    //  3 : "Reserved"
    InterEEPTimes_BTA1_t InterEEPTimes_BTA1 : 3U;
    uint32_t reserved5 : 8U;
  }sig;
} BTA1_01_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA1_01_un msg;
}BTA1_01_msg;

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
    uint8_t VehOdometer_0 : 8U; // Unit:'kM'
    uint8_t VehOdometer_1 : 8U; // Unit:'kM'
    uint8_t VehOdometer_2 : 8U; // Unit:'kM'
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
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Yr_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Yr_phy_toS(x) ( (uint8_t) ((x) - (2000)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_Yr_phy_fromS(x) ( ((x) + (2000)) )

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

// def @ZONE_L_TPMS_01 CAN Message (1153 0x481)
#define ZONE_L_TPMS_01_IDE (0U)
#define ZONE_L_TPMS_01_DLC (64U)
#define ZONE_L_TPMS_01_CANID (0x481U)
#define ZONE_L_TPMS_01_CYC (500U)

typedef union
{
  uint8_t data[64u];
  struct
  {
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
    uint8_t TpmsFilterID1_0 : 8U;
    uint8_t TpmsFilterID1_1 : 8U;
    uint8_t TpmsFilterID1_2 : 8U;
    uint8_t TpmsFilterID1_3 : 8U;
    uint8_t TpmsFilterID2_0 : 8U;
    uint8_t TpmsFilterID2_1 : 8U;
    uint8_t TpmsFilterID2_2 : 8U;
    uint8_t TpmsFilterID2_3 : 8U;
    uint8_t TpmsFilterID3_0 : 8U;
    uint8_t TpmsFilterID3_1 : 8U;
    uint8_t TpmsFilterID3_2 : 8U;
    uint8_t TpmsFilterID3_3 : 8U;
    uint8_t TpmsFilterID4_0 : 8U;
    uint8_t TpmsFilterID4_1 : 8U;
    uint8_t TpmsFilterID4_2 : 8U;
    uint8_t TpmsFilterID4_3 : 8U;
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
} ZONE_L_TPMS_01_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  ZONE_L_TPMS_01_un msg;
}ZONE_L_TPMS_01_msg;

// def @BTA1_Log_Data CAN Message (1170 0x492)
#define BTA1_Log_Data_IDE (0U)
#define BTA1_Log_Data_DLC (64U)
#define BTA1_Log_Data_CANID (0x492U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    // BTA1 Log Level
    uint8_t BTA1_Log_Level : 8U;
    uint8_t BTA1_Log_Sender_0 : 8U;
    uint8_t BTA1_Log_Sender_1 : 8U;
    uint8_t BTA1_Log_Sender_2 : 8U;
    uint8_t BTA1_Log_Data1_0 : 8U;
    uint8_t BTA1_Log_Data1_1 : 8U;
    uint8_t BTA1_Log_Data1_2 : 8U;
    uint8_t BTA1_Log_Data1_3 : 8U;
    uint8_t BTA1_Log_Data2_0 : 8U;
    uint8_t BTA1_Log_Data2_1 : 8U;
    uint8_t BTA1_Log_Data2_2 : 8U;
    uint8_t BTA1_Log_Data2_3 : 8U;
    uint8_t BTA1_Log_Data3_0 : 8U;
    uint8_t BTA1_Log_Data3_1 : 8U;
    uint8_t BTA1_Log_Data3_2 : 8U;
    uint8_t BTA1_Log_Data3_3 : 8U;
    uint8_t BTA1_Log_Data4_0 : 8U;
    uint8_t BTA1_Log_Data4_1 : 8U;
    uint8_t BTA1_Log_Data4_2 : 8U;
    uint8_t BTA1_Log_Data4_3 : 8U;
    uint8_t BTA1_Log_Data5_0 : 8U;
    uint8_t BTA1_Log_Data5_1 : 8U;
    uint8_t BTA1_Log_Data5_2 : 8U;
    uint8_t BTA1_Log_Data5_3 : 8U;
    uint8_t BTA1_Log_Data6_0 : 8U;
    uint8_t BTA1_Log_Data6_1 : 8U;
    uint8_t BTA1_Log_Data6_2 : 8U;
    uint8_t BTA1_Log_Data6_3 : 8U;
    uint8_t BTA1_Log_Data7_0 : 8U;
    uint8_t BTA1_Log_Data7_1 : 8U;
    uint8_t BTA1_Log_Data7_2 : 8U;
    uint8_t BTA1_Log_Data7_3 : 8U;
    uint8_t BTA1_Log_Data8_0 : 8U;
    uint8_t BTA1_Log_Data8_1 : 8U;
    uint8_t BTA1_Log_Data8_2 : 8U;
    uint8_t BTA1_Log_Data8_3 : 8U;
    uint8_t BTA1_Log_Data9_0 : 8U;
    uint8_t BTA1_Log_Data9_1 : 8U;
    uint8_t BTA1_Log_Data9_2 : 8U;
    uint8_t BTA1_Log_Data9_3 : 8U;
    uint8_t BTA1_Log_Data10_0 : 8U;
    uint8_t BTA1_Log_Data10_1 : 8U;
    uint8_t BTA1_Log_Data10_2 : 8U;
    uint8_t BTA1_Log_Data10_3 : 8U;
    uint8_t BTA1_Log_Data11_0 : 8U;
    uint8_t BTA1_Log_Data11_1 : 8U;
    uint8_t BTA1_Log_Data11_2 : 8U;
    uint8_t BTA1_Log_Data11_3 : 8U;
    uint8_t BTA1_Log_Data12_0 : 8U;
    uint8_t BTA1_Log_Data12_1 : 8U;
    uint8_t BTA1_Log_Data12_2 : 8U;
    uint8_t BTA1_Log_Data12_3 : 8U;
    uint8_t BTA1_Log_Data13_0 : 8U;
    uint8_t BTA1_Log_Data13_1 : 8U;
    uint8_t BTA1_Log_Data13_2 : 8U;
    uint8_t BTA1_Log_Data13_3 : 8U;
    uint8_t BTA1_Log_Data14_0 : 8U;
    uint8_t BTA1_Log_Data14_1 : 8U;
    uint8_t BTA1_Log_Data14_2 : 8U;
    uint8_t BTA1_Log_Data14_3 : 8U;
    uint8_t BTA1_Log_Data15_0 : 8U;
    uint8_t BTA1_Log_Data15_1 : 8U;
    uint8_t BTA1_Log_Data15_2 : 8U;
    uint8_t BTA1_Log_Data15_3 : 8U;
  }sig;
} BTA1_Log_Data_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA1_Log_Data_un msg;
}BTA1_Log_Data_msg;

// cantp max_size:300 flow_control_frame_id:1197
// def @CccDkBleCmd CAN Message (1196 0x4ac)
#define CccDkBleCmd_IDE (0U)
#define CccDkBleCmd_DLC (64U)
#define CccDkBleCmd_CANID (0x4acU)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t CccDkBleCmd_7_0 : 8U;
    uint8_t CccDkBleCmd_7_1 : 8U;
    uint8_t CccDkBleCmd_7_2 : 8U;
    uint8_t CccDkBleCmd_7_3 : 8U;
    uint8_t CccDkBleCmd_7_4 : 8U;
    uint8_t CccDkBleCmd_7_5 : 8U;
    uint8_t CccDkBleCmd_7_6 : 8U;
    uint8_t CccDkBleCmd_7_7 : 8U;
    uint8_t CccDkBleCmd_6_0 : 8U;
    uint8_t CccDkBleCmd_6_1 : 8U;
    uint8_t CccDkBleCmd_6_2 : 8U;
    uint8_t CccDkBleCmd_6_3 : 8U;
    uint8_t CccDkBleCmd_6_4 : 8U;
    uint8_t CccDkBleCmd_6_5 : 8U;
    uint8_t CccDkBleCmd_6_6 : 8U;
    uint8_t CccDkBleCmd_6_7 : 8U;
    uint8_t CccDkBleCmd_5_0 : 8U;
    uint8_t CccDkBleCmd_5_1 : 8U;
    uint8_t CccDkBleCmd_5_2 : 8U;
    uint8_t CccDkBleCmd_5_3 : 8U;
    uint8_t CccDkBleCmd_5_4 : 8U;
    uint8_t CccDkBleCmd_5_5 : 8U;
    uint8_t CccDkBleCmd_5_6 : 8U;
    uint8_t CccDkBleCmd_5_7 : 8U;
    uint8_t CccDkBleCmd_4_0 : 8U;
    uint8_t CccDkBleCmd_4_1 : 8U;
    uint8_t CccDkBleCmd_4_2 : 8U;
    uint8_t CccDkBleCmd_4_3 : 8U;
    uint8_t CccDkBleCmd_4_4 : 8U;
    uint8_t CccDkBleCmd_4_5 : 8U;
    uint8_t CccDkBleCmd_4_6 : 8U;
    uint8_t CccDkBleCmd_4_7 : 8U;
    uint8_t CccDkBleCmd_3_0 : 8U;
    uint8_t CccDkBleCmd_3_1 : 8U;
    uint8_t CccDkBleCmd_3_2 : 8U;
    uint8_t CccDkBleCmd_3_3 : 8U;
    uint8_t CccDkBleCmd_3_4 : 8U;
    uint8_t CccDkBleCmd_3_5 : 8U;
    uint8_t CccDkBleCmd_3_6 : 8U;
    uint8_t CccDkBleCmd_3_7 : 8U;
    uint8_t CccDkBleCmd_2_0 : 8U;
    uint8_t CccDkBleCmd_2_1 : 8U;
    uint8_t CccDkBleCmd_2_2 : 8U;
    uint8_t CccDkBleCmd_2_3 : 8U;
    uint8_t CccDkBleCmd_2_4 : 8U;
    uint8_t CccDkBleCmd_2_5 : 8U;
    uint8_t CccDkBleCmd_2_6 : 8U;
    uint8_t CccDkBleCmd_2_7 : 8U;
    uint8_t CccDkBleCmd_1_0 : 8U;
    uint8_t CccDkBleCmd_1_1 : 8U;
    uint8_t CccDkBleCmd_1_2 : 8U;
    uint8_t CccDkBleCmd_1_3 : 8U;
    uint8_t CccDkBleCmd_1_4 : 8U;
    uint8_t CccDkBleCmd_1_5 : 8U;
    uint8_t CccDkBleCmd_1_6 : 8U;
    uint8_t CccDkBleCmd_1_7 : 8U;
    uint8_t CccDkBleCmd_0_0 : 8U;
    uint8_t CccDkBleCmd_0_1 : 8U;
    uint8_t CccDkBleCmd_0_2 : 8U;
    uint8_t CccDkBleCmd_0_3 : 8U;
    uint8_t CccDkBleCmd_0_4 : 8U;
    uint8_t CccDkBleCmd_0_5 : 8U;
    uint8_t CccDkBleCmd_0_6 : 8U;
    uint8_t CccDkBleCmd_0_7 : 8U;
  }sig;
} CccDkBleCmd_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  CccDkBleCmd_un msg;
}CccDkBleCmd_msg;

// cantp max_size:300 flow_control_frame_id:1196
// def @CccDkBleCmdFlowCtrl CAN Message (1197 0x4ad)
#define CccDkBleCmdFlowCtrl_IDE (0U)
#define CccDkBleCmdFlowCtrl_DLC (64U)
#define CccDkBleCmdFlowCtrl_CANID (0x4adU)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t CccDkBleCmdFlowCtrl_7_0 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_7_1 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_7_2 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_7_3 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_7_4 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_7_5 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_7_6 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_7_7 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_6_0 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_6_1 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_6_2 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_6_3 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_6_4 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_6_5 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_6_6 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_6_7 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_5_0 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_5_1 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_5_2 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_5_3 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_5_4 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_5_5 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_5_6 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_5_7 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_4_0 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_4_1 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_4_2 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_4_3 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_4_4 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_4_5 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_4_6 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_4_7 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_3_0 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_3_1 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_3_2 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_3_3 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_3_4 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_3_5 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_3_6 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_3_7 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_2_0 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_2_1 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_2_2 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_2_3 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_2_4 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_2_5 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_2_6 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_2_7 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_1_0 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_1_1 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_1_2 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_1_3 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_1_4 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_1_5 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_1_6 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_1_7 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_0_0 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_0_1 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_0_2 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_0_3 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_0_4 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_0_5 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_0_6 : 8U;
    uint8_t CccDkBleCmdFlowCtrl_0_7 : 8U;
  }sig;
} CccDkBleCmdFlowCtrl_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  CccDkBleCmdFlowCtrl_un msg;
}CccDkBleCmdFlowCtrl_msg;

// cantp max_size:300 flow_control_frame_id:1199
// def @CccDkBleRsp CAN Message (1198 0x4ae)
#define CccDkBleRsp_IDE (0U)
#define CccDkBleRsp_DLC (64U)
#define CccDkBleRsp_CANID (0x4aeU)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t CccDkBleRsp_DATA_7_0 : 8U;
    uint8_t CccDkBleRsp_DATA_7_1 : 8U;
    uint8_t CccDkBleRsp_DATA_7_2 : 8U;
    uint8_t CccDkBleRsp_DATA_7_3 : 8U;
    uint8_t CccDkBleRsp_DATA_7_4 : 8U;
    uint8_t CccDkBleRsp_DATA_7_5 : 8U;
    uint8_t CccDkBleRsp_DATA_7_6 : 8U;
    uint8_t CccDkBleRsp_DATA_7_7 : 8U;
    uint8_t CccDkBleRsp_DATA_6_0 : 8U;
    uint8_t CccDkBleRsp_DATA_6_1 : 8U;
    uint8_t CccDkBleRsp_DATA_6_2 : 8U;
    uint8_t CccDkBleRsp_DATA_6_3 : 8U;
    uint8_t CccDkBleRsp_DATA_6_4 : 8U;
    uint8_t CccDkBleRsp_DATA_6_5 : 8U;
    uint8_t CccDkBleRsp_DATA_6_6 : 8U;
    uint8_t CccDkBleRsp_DATA_6_7 : 8U;
    uint8_t CccDkBleRsp_DATA_5_0 : 8U;
    uint8_t CccDkBleRsp_DATA_5_1 : 8U;
    uint8_t CccDkBleRsp_DATA_5_2 : 8U;
    uint8_t CccDkBleRsp_DATA_5_3 : 8U;
    uint8_t CccDkBleRsp_DATA_5_4 : 8U;
    uint8_t CccDkBleRsp_DATA_5_5 : 8U;
    uint8_t CccDkBleRsp_DATA_5_6 : 8U;
    uint8_t CccDkBleRsp_DATA_5_7 : 8U;
    uint8_t CccDkBleRsp_DATA_4_0 : 8U;
    uint8_t CccDkBleRsp_DATA_4_1 : 8U;
    uint8_t CccDkBleRsp_DATA_4_2 : 8U;
    uint8_t CccDkBleRsp_DATA_4_3 : 8U;
    uint8_t CccDkBleRsp_DATA_4_4 : 8U;
    uint8_t CccDkBleRsp_DATA_4_5 : 8U;
    uint8_t CccDkBleRsp_DATA_4_6 : 8U;
    uint8_t CccDkBleRsp_DATA_4_7 : 8U;
    uint8_t CccDkBleRsp_DATA_3_0 : 8U;
    uint8_t CccDkBleRsp_DATA_3_1 : 8U;
    uint8_t CccDkBleRsp_DATA_3_2 : 8U;
    uint8_t CccDkBleRsp_DATA_3_3 : 8U;
    uint8_t CccDkBleRsp_DATA_3_4 : 8U;
    uint8_t CccDkBleRsp_DATA_3_5 : 8U;
    uint8_t CccDkBleRsp_DATA_3_6 : 8U;
    uint8_t CccDkBleRsp_DATA_3_7 : 8U;
    uint8_t CccDkBleRsp_DATA_2_0 : 8U;
    uint8_t CccDkBleRsp_DATA_2_1 : 8U;
    uint8_t CccDkBleRsp_DATA_2_2 : 8U;
    uint8_t CccDkBleRsp_DATA_2_3 : 8U;
    uint8_t CccDkBleRsp_DATA_2_4 : 8U;
    uint8_t CccDkBleRsp_DATA_2_5 : 8U;
    uint8_t CccDkBleRsp_DATA_2_6 : 8U;
    uint8_t CccDkBleRsp_DATA_2_7 : 8U;
    uint8_t CccDkBleRsp_DATA_1_0 : 8U;
    uint8_t CccDkBleRsp_DATA_1_1 : 8U;
    uint8_t CccDkBleRsp_DATA_1_2 : 8U;
    uint8_t CccDkBleRsp_DATA_1_3 : 8U;
    uint8_t CccDkBleRsp_DATA_1_4 : 8U;
    uint8_t CccDkBleRsp_DATA_1_5 : 8U;
    uint8_t CccDkBleRsp_DATA_1_6 : 8U;
    uint8_t CccDkBleRsp_DATA_1_7 : 8U;
    uint8_t CccDkBleRsp_DATA_0_0 : 8U;
    uint8_t CccDkBleRsp_DATA_0_1 : 8U;
    uint8_t CccDkBleRsp_DATA_0_2 : 8U;
    uint8_t CccDkBleRsp_DATA_0_3 : 8U;
    uint8_t CccDkBleRsp_DATA_0_4 : 8U;
    uint8_t CccDkBleRsp_DATA_0_5 : 8U;
    uint8_t CccDkBleRsp_DATA_0_6 : 8U;
    uint8_t CccDkBleRsp_DATA_0_7 : 8U;
  }sig;
} CccDkBleRsp_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  CccDkBleRsp_un msg;
}CccDkBleRsp_msg;

// cantp max_size:300 flow_control_frame_id:1198
// def @CccDkBleRspFlowCtrl CAN Message (1199 0x4af)
#define CccDkBleRspFlowCtrl_IDE (0U)
#define CccDkBleRspFlowCtrl_DLC (64U)
#define CccDkBleRspFlowCtrl_CANID (0x4afU)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t CccDkBleRspFlowCtrl_7_0 : 8U;
    uint8_t CccDkBleRspFlowCtrl_7_1 : 8U;
    uint8_t CccDkBleRspFlowCtrl_7_2 : 8U;
    uint8_t CccDkBleRspFlowCtrl_7_3 : 8U;
    uint8_t CccDkBleRspFlowCtrl_7_4 : 8U;
    uint8_t CccDkBleRspFlowCtrl_7_5 : 8U;
    uint8_t CccDkBleRspFlowCtrl_7_6 : 8U;
    uint8_t CccDkBleRspFlowCtrl_7_7 : 8U;
    uint8_t CccDkBleRspFlowCtrl_6_0 : 8U;
    uint8_t CccDkBleRspFlowCtrl_6_1 : 8U;
    uint8_t CccDkBleRspFlowCtrl_6_2 : 8U;
    uint8_t CccDkBleRspFlowCtrl_6_3 : 8U;
    uint8_t CccDkBleRspFlowCtrl_6_4 : 8U;
    uint8_t CccDkBleRspFlowCtrl_6_5 : 8U;
    uint8_t CccDkBleRspFlowCtrl_6_6 : 8U;
    uint8_t CccDkBleRspFlowCtrl_6_7 : 8U;
    uint8_t CccDkBleRspFlowCtrl_5_0 : 8U;
    uint8_t CccDkBleRspFlowCtrl_5_1 : 8U;
    uint8_t CccDkBleRspFlowCtrl_5_2 : 8U;
    uint8_t CccDkBleRspFlowCtrl_5_3 : 8U;
    uint8_t CccDkBleRspFlowCtrl_5_4 : 8U;
    uint8_t CccDkBleRspFlowCtrl_5_5 : 8U;
    uint8_t CccDkBleRspFlowCtrl_5_6 : 8U;
    uint8_t CccDkBleRspFlowCtrl_5_7 : 8U;
    uint8_t CccDkBleRspFlowCtrl_4_0 : 8U;
    uint8_t CccDkBleRspFlowCtrl_4_1 : 8U;
    uint8_t CccDkBleRspFlowCtrl_4_2 : 8U;
    uint8_t CccDkBleRspFlowCtrl_4_3 : 8U;
    uint8_t CccDkBleRspFlowCtrl_4_4 : 8U;
    uint8_t CccDkBleRspFlowCtrl_4_5 : 8U;
    uint8_t CccDkBleRspFlowCtrl_4_6 : 8U;
    uint8_t CccDkBleRspFlowCtrl_4_7 : 8U;
    uint8_t CccDkBleRspFlowCtrl_3_0 : 8U;
    uint8_t CccDkBleRspFlowCtrl_3_1 : 8U;
    uint8_t CccDkBleRspFlowCtrl_3_2 : 8U;
    uint8_t CccDkBleRspFlowCtrl_3_3 : 8U;
    uint8_t CccDkBleRspFlowCtrl_3_4 : 8U;
    uint8_t CccDkBleRspFlowCtrl_3_5 : 8U;
    uint8_t CccDkBleRspFlowCtrl_3_6 : 8U;
    uint8_t CccDkBleRspFlowCtrl_3_7 : 8U;
    uint8_t CccDkBleRspFlowCtrl_2_0 : 8U;
    uint8_t CccDkBleRspFlowCtrl_2_1 : 8U;
    uint8_t CccDkBleRspFlowCtrl_2_2 : 8U;
    uint8_t CccDkBleRspFlowCtrl_2_3 : 8U;
    uint8_t CccDkBleRspFlowCtrl_2_4 : 8U;
    uint8_t CccDkBleRspFlowCtrl_2_5 : 8U;
    uint8_t CccDkBleRspFlowCtrl_2_6 : 8U;
    uint8_t CccDkBleRspFlowCtrl_2_7 : 8U;
    uint8_t CccDkBleRspFlowCtrl_1_0 : 8U;
    uint8_t CccDkBleRspFlowCtrl_1_1 : 8U;
    uint8_t CccDkBleRspFlowCtrl_1_2 : 8U;
    uint8_t CccDkBleRspFlowCtrl_1_3 : 8U;
    uint8_t CccDkBleRspFlowCtrl_1_4 : 8U;
    uint8_t CccDkBleRspFlowCtrl_1_5 : 8U;
    uint8_t CccDkBleRspFlowCtrl_1_6 : 8U;
    uint8_t CccDkBleRspFlowCtrl_1_7 : 8U;
    uint8_t CccDkBleRspFlowCtrl_0_0 : 8U;
    uint8_t CccDkBleRspFlowCtrl_0_1 : 8U;
    uint8_t CccDkBleRspFlowCtrl_0_2 : 8U;
    uint8_t CccDkBleRspFlowCtrl_0_3 : 8U;
    uint8_t CccDkBleRspFlowCtrl_0_4 : 8U;
    uint8_t CccDkBleRspFlowCtrl_0_5 : 8U;
    uint8_t CccDkBleRspFlowCtrl_0_6 : 8U;
    uint8_t CccDkBleRspFlowCtrl_0_7 : 8U;
  }sig;
} CccDkBleRspFlowCtrl_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  CccDkBleRspFlowCtrl_un msg;
}CccDkBleRspFlowCtrl_msg;

// def @BTA_02 CAN Message (1233 0x4d1)
#define BTA_02_IDE (0U)
#define BTA_02_DLC (64U)
#define BTA_02_CANID (0x4d1U)
#define BTA_02_CYC (500U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint32_t reserved0 : 8U;

    uint32_t reserved1 : 8U;
    uint32_t reserved2 : 8U;
    uint8_t Ibeacon_Adv_UUID_0_0 : 8U;
    uint8_t Ibeacon_Adv_UUID_0_1 : 8U;
    uint8_t Ibeacon_Adv_UUID_0_2 : 8U;
    uint8_t Ibeacon_Adv_UUID_0_3 : 8U;
    uint8_t Ibeacon_Adv_UUID_1_0 : 8U;
    uint8_t Ibeacon_Adv_UUID_1_1 : 8U;
    uint8_t Ibeacon_Adv_UUID_1_2 : 8U;
    uint8_t Ibeacon_Adv_UUID_1_3 : 8U;
    uint8_t Ibeacon_Adv_UUID_2_0 : 8U;
    uint8_t Ibeacon_Adv_UUID_2_1 : 8U;
    uint8_t Ibeacon_Adv_UUID_2_2 : 8U;
    uint8_t Ibeacon_Adv_UUID_2_3 : 8U;
    uint8_t Ibeacon_Adv_UUID_3_0 : 8U;
    uint8_t Ibeacon_Adv_UUID_3_1 : 8U;
    uint8_t Ibeacon_Adv_UUID_3_2 : 8U;
    uint8_t Ibeacon_Adv_UUID_3_3 : 8U;
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
} BTA_02_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA_02_un msg;
}BTA_02_msg;

// cantp max_size:200 flow_control_frame_id:1252
// def @BLE_Uplink_FlowCtrl CAN Message (1249 0x4e1)
#define BLE_Uplink_FlowCtrl_IDE (0U)
#define BLE_Uplink_FlowCtrl_DLC (64U)
#define BLE_Uplink_FlowCtrl_CANID (0x4e1U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t BLE_Uplink_FlowCtrl_0_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_0_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_0_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_0_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_1_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_1_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_1_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_1_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_2_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_2_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_2_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_2_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_3_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_3_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_3_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_3_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_4_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_4_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_4_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_4_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_5_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_5_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_5_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_5_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_6_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_6_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_6_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_6_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_7_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_7_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_7_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_7_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_8_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_8_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_8_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_8_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_9_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_9_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_9_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_9_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_10_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_10_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_10_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_10_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_11_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_11_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_11_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_11_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_12_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_12_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_12_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_12_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_13_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_13_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_13_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_13_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_14_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_14_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_14_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_14_3 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_15_0 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_15_1 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_15_2 : 8U;
    uint8_t BLE_Uplink_FlowCtrl_15_3 : 8U;
  }sig;
} BLE_Uplink_FlowCtrl_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_Uplink_FlowCtrl_un msg;
}BLE_Uplink_FlowCtrl_msg;

// cantp max_size:200 flow_control_frame_id:1253
// def @BLE_Downlink_Data CAN Message (1250 0x4e2)
#define BLE_Downlink_Data_IDE (0U)
#define BLE_Downlink_Data_DLC (64U)
#define BLE_Downlink_Data_CANID (0x4e2U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t BLE_Downlink_Data_0_0 : 8U;
    uint8_t BLE_Downlink_Data_0_1 : 8U;
    uint8_t BLE_Downlink_Data_0_2 : 8U;
    uint8_t BLE_Downlink_Data_0_3 : 8U;
    uint8_t BLE_Downlink_Data_1_0 : 8U;
    uint8_t BLE_Downlink_Data_1_1 : 8U;
    uint8_t BLE_Downlink_Data_1_2 : 8U;
    uint8_t BLE_Downlink_Data_1_3 : 8U;
    uint8_t BLE_Downlink_Data_2_0 : 8U;
    uint8_t BLE_Downlink_Data_2_1 : 8U;
    uint8_t BLE_Downlink_Data_2_2 : 8U;
    uint8_t BLE_Downlink_Data_2_3 : 8U;
    uint8_t BLE_Downlink_Data_3_0 : 8U;
    uint8_t BLE_Downlink_Data_3_1 : 8U;
    uint8_t BLE_Downlink_Data_3_2 : 8U;
    uint8_t BLE_Downlink_Data_3_3 : 8U;
    uint8_t BLE_Downlink_Data_4_0 : 8U;
    uint8_t BLE_Downlink_Data_4_1 : 8U;
    uint8_t BLE_Downlink_Data_4_2 : 8U;
    uint8_t BLE_Downlink_Data_4_3 : 8U;
    uint8_t BLE_Downlink_Data_5_0 : 8U;
    uint8_t BLE_Downlink_Data_5_1 : 8U;
    uint8_t BLE_Downlink_Data_5_2 : 8U;
    uint8_t BLE_Downlink_Data_5_3 : 8U;
    uint8_t BLE_Downlink_Data_6_0 : 8U;
    uint8_t BLE_Downlink_Data_6_1 : 8U;
    uint8_t BLE_Downlink_Data_6_2 : 8U;
    uint8_t BLE_Downlink_Data_6_3 : 8U;
    uint8_t BLE_Downlink_Data_7_0 : 8U;
    uint8_t BLE_Downlink_Data_7_1 : 8U;
    uint8_t BLE_Downlink_Data_7_2 : 8U;
    uint8_t BLE_Downlink_Data_7_3 : 8U;
    uint8_t BLE_Downlink_Data_8_0 : 8U;
    uint8_t BLE_Downlink_Data_8_1 : 8U;
    uint8_t BLE_Downlink_Data_8_2 : 8U;
    uint8_t BLE_Downlink_Data_8_3 : 8U;
    uint8_t BLE_Downlink_Data_9_0 : 8U;
    uint8_t BLE_Downlink_Data_9_1 : 8U;
    uint8_t BLE_Downlink_Data_9_2 : 8U;
    uint8_t BLE_Downlink_Data_9_3 : 8U;
    uint8_t BLE_Downlink_Data_10_0 : 8U;
    uint8_t BLE_Downlink_Data_10_1 : 8U;
    uint8_t BLE_Downlink_Data_10_2 : 8U;
    uint8_t BLE_Downlink_Data_10_3 : 8U;
    uint8_t BLE_Downlink_Data_11_0 : 8U;
    uint8_t BLE_Downlink_Data_11_1 : 8U;
    uint8_t BLE_Downlink_Data_11_2 : 8U;
    uint8_t BLE_Downlink_Data_11_3 : 8U;
    uint8_t BLE_Downlink_Data_12_0 : 8U;
    uint8_t BLE_Downlink_Data_12_1 : 8U;
    uint8_t BLE_Downlink_Data_12_2 : 8U;
    uint8_t BLE_Downlink_Data_12_3 : 8U;
    uint8_t BLE_Downlink_Data_13_0 : 8U;
    uint8_t BLE_Downlink_Data_13_1 : 8U;
    uint8_t BLE_Downlink_Data_13_2 : 8U;
    uint8_t BLE_Downlink_Data_13_3 : 8U;
    uint8_t BLE_Downlink_Data_14_0 : 8U;
    uint8_t BLE_Downlink_Data_14_1 : 8U;
    uint8_t BLE_Downlink_Data_14_2 : 8U;
    uint8_t BLE_Downlink_Data_14_3 : 8U;
    uint8_t BLE_Downlink_Data_15_0 : 8U;
    uint8_t BLE_Downlink_Data_15_1 : 8U;
    uint8_t BLE_Downlink_Data_15_2 : 8U;
    uint8_t BLE_Downlink_Data_15_3 : 8U;
  }sig;
} BLE_Downlink_Data_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_Downlink_Data_un msg;
}BLE_Downlink_Data_msg;

// def @BTA7_RSSI_UWB CAN Message (1251 0x4e3)
#define BTA7_RSSI_UWB_IDE (0U)
#define BTA7_RSSI_UWB_DLC (64U)
#define BTA7_RSSI_UWB_CANID (0x4e3U)
#define BTA7_RSSI_UWB_CYC (50U)
// signal: @BTA7_RSSI_Dev1_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev1_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev1_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev1_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA7_RSSI_Dev2_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev2_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev2_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev2_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA7_RSSI_Dev3_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev3_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev3_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev3_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA7_RSSI_Dev4_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev4_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev4_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev4_phy_fromS(x) ( ((x) + (-255)) )
// signal: @BTA7_RSSI_Dev5_phy
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev5_phy_CovFactor (1)
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev5_phy_toS(x) ( (uint8_t) ((x) - (-255)) )
#define ONVO_NT3_LITE_BTA_SRF_V_1_10_0_BTA7_RSSI_Dev5_phy_fromS(x) ( ((x) + (-255)) )

typedef enum
{
    idle_768 = 0U,
    processing_769 = 1U,
    success_770 = 2U,
    fail_invalid_payload_length_771 = 3U,
    fail_invaild_checksum_772 = 4U
}UWBInfoSyncStsdev1_BTA7_t;

typedef enum
{
    idle_773 = 0U,
    processing_774 = 1U,
    success_775 = 2U,
    fail_invalid_payload_length_776 = 3U,
    fail_invaild_checksum_777 = 4U
}UWBInfoSyncStsdev2_BTA7_t;

typedef enum
{
    idle_778 = 0U,
    processing_779 = 1U,
    success_780 = 2U,
    fail_invalid_payload_length_781 = 3U,
    fail_invaild_checksum_782 = 4U
}UWBInfoSyncStsdev3_BTA7_t;

typedef enum
{
    idle_783 = 0U,
    processing_784 = 1U,
    success_785 = 2U,
    fail_invalid_payload_length_786 = 3U,
    fail_invaild_checksum_787 = 4U
}UWBInfoSyncStsdev4_BTA7_t;

typedef enum
{
    idle_788 = 0U,
    processing_789 = 1U,
    success_790 = 2U,
    fail_invalid_payload_length_791 = 3U,
    fail_invaild_checksum_792 = 4U
}UWBInfoSyncStsdev5_BTA7_t;

typedef enum
{
    none_793 = 0U,
    start_794 = 1U,
    stop_795 = 2U,
    suspend_796 = 3U,
    recovery_797 = 4U,
    restart_798 = 5U
}UWBRangingStsdev1_BTA7_t;

typedef enum
{
    none_799 = 0U,
    start_800 = 1U,
    stop_801 = 2U,
    suspend_802 = 3U,
    recovery_803 = 4U,
    restart_804 = 5U
}UWBRangingStsdev2_BTA7_t;

typedef enum
{
    none_805 = 0U,
    start_806 = 1U,
    stop_807 = 2U,
    suspend_808 = 3U,
    recovery_809 = 4U,
    restart_810 = 5U
}UWBRangingStsdev3_BTA7_t;

typedef enum
{
    none_811 = 0U,
    start_812 = 1U,
    stop_813 = 2U,
    suspend_814 = 3U,
    recovery_815 = 4U,
    restart_816 = 5U
}UWBRangingStsdev4_BTA7_t;

typedef enum
{
    none_817 = 0U,
    start_818 = 1U,
    stop_819 = 2U,
    suspend_820 = 3U,
    recovery_821 = 4U,
    restart_822 = 5U
}UWBRangingStsdev5_BTA7_t;

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint32_t reserved0 : 8U;
    uint8_t BTA7_RSSI_Dev1_phy : 8U;// Offset= -255
    uint8_t BTA7_RSSI_Dev2_phy : 8U;// Offset= -255
    uint8_t BTA7_RSSI_Dev3_phy : 8U;// Offset= -255
    uint8_t BTA7_RSSI_Dev4_phy : 8U;// Offset= -255
    uint8_t BTA7_RSSI_Dev5_phy : 8U;// Offset= -255
    uint8_t BTA7_UWB_Dist_Dev1_0 : 8U;
    uint8_t BTA7_UWB_Dist_Dev1_1 : 8U;
    uint8_t BTA7_UWB_Dist_Dev2_0 : 8U;
    uint8_t BTA7_UWB_Dist_Dev2_1 : 8U;
    uint8_t BTA7_UWB_Dist_Dev3_0 : 8U;
    uint8_t BTA7_UWB_Dist_Dev3_1 : 8U;
    uint8_t BTA7_UWB_Dist_Dev4_0 : 8U;
    uint8_t BTA7_UWB_Dist_Dev4_1 : 8U;
    uint8_t BTA7_UWB_Dist_Dev5_0 : 8U;
    uint8_t BTA7_UWB_Dist_Dev5_1 : 8U;
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWBInfoSyncStsdev1_BTA7_t UWBInfoSyncStsdev1_BTA7 : 8U;
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWBInfoSyncStsdev2_BTA7_t UWBInfoSyncStsdev2_BTA7 : 8U;
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWBInfoSyncStsdev3_BTA7_t UWBInfoSyncStsdev3_BTA7 : 8U;
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWBInfoSyncStsdev4_BTA7_t UWBInfoSyncStsdev4_BTA7 : 8U;
    //  0 : "idle"
    //  1 : "processing"
    //  2 : "success"
    //  3 : "fail, invalid payload length"
    //  4 : "fail, invaild checksum"
    UWBInfoSyncStsdev5_BTA7_t UWBInfoSyncStsdev5_BTA7 : 8U;
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWBRangingStsdev1_BTA7_t UWBRangingStsdev1_BTA7 : 8U;
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWBRangingStsdev2_BTA7_t UWBRangingStsdev2_BTA7 : 8U;
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWBRangingStsdev3_BTA7_t UWBRangingStsdev3_BTA7 : 8U;
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWBRangingStsdev4_BTA7_t UWBRangingStsdev4_BTA7 : 8U;
    //  0 : "none"
    //  1 : "start"
    //  2 : "stop"
    //  3 : "suspend"
    //  4 : "recovery"
    //  5 : "restart"
    UWBRangingStsdev5_BTA7_t UWBRangingStsdev5_BTA7 : 8U;
    uint8_t BTA7_RSSI_Channel_Index_Dev2_0 : 2U;
    uint8_t BTA7_RSSI_Channel_Index_Dev1 : 6U;
    uint8_t BTA7_RSSI_Channel_Index_Dev3_0 : 4U;
    uint8_t BTA7_RSSI_Channel_Index_Dev2_1 : 4U;
    uint8_t BTA7_RSSI_Channel_Index_Dev4 : 6U;
    uint8_t BTA7_RSSI_Channel_Index_Dev3_1 : 2U;
    uint32_t reserved1 : 2U;
    uint8_t BTA7_RSSI_Channel_Index_Dev5 : 6U;
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
  }sig;
} BTA7_RSSI_UWB_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA7_RSSI_UWB_un msg;
}BTA7_RSSI_UWB_msg;

// cantp max_size:200 flow_control_frame_id:1249
// def @BLE_Uplink_Data CAN Message (1252 0x4e4)
#define BLE_Uplink_Data_IDE (0U)
#define BLE_Uplink_Data_DLC (64U)
#define BLE_Uplink_Data_CANID (0x4e4U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t BLE_Uplink_Data_0_0 : 8U;
    uint8_t BLE_Uplink_Data_0_1 : 8U;
    uint8_t BLE_Uplink_Data_0_2 : 8U;
    uint8_t BLE_Uplink_Data_0_3 : 8U;
    uint8_t BLE_Uplink_Data_1_0 : 8U;
    uint8_t BLE_Uplink_Data_1_1 : 8U;
    uint8_t BLE_Uplink_Data_1_2 : 8U;
    uint8_t BLE_Uplink_Data_1_3 : 8U;
    uint8_t BLE_Uplink_Data_2_0 : 8U;
    uint8_t BLE_Uplink_Data_2_1 : 8U;
    uint8_t BLE_Uplink_Data_2_2 : 8U;
    uint8_t BLE_Uplink_Data_2_3 : 8U;
    uint8_t BLE_Uplink_Data_3_0 : 8U;
    uint8_t BLE_Uplink_Data_3_1 : 8U;
    uint8_t BLE_Uplink_Data_3_2 : 8U;
    uint8_t BLE_Uplink_Data_3_3 : 8U;
    uint8_t BLE_Uplink_Data_4_0 : 8U;
    uint8_t BLE_Uplink_Data_4_1 : 8U;
    uint8_t BLE_Uplink_Data_4_2 : 8U;
    uint8_t BLE_Uplink_Data_4_3 : 8U;
    uint8_t BLE_Uplink_Data_5_0 : 8U;
    uint8_t BLE_Uplink_Data_5_1 : 8U;
    uint8_t BLE_Uplink_Data_5_2 : 8U;
    uint8_t BLE_Uplink_Data_5_3 : 8U;
    uint8_t BLE_Uplink_Data_6_0 : 8U;
    uint8_t BLE_Uplink_Data_6_1 : 8U;
    uint8_t BLE_Uplink_Data_6_2 : 8U;
    uint8_t BLE_Uplink_Data_6_3 : 8U;
    uint8_t BLE_Uplink_Data_7_0 : 8U;
    uint8_t BLE_Uplink_Data_7_1 : 8U;
    uint8_t BLE_Uplink_Data_7_2 : 8U;
    uint8_t BLE_Uplink_Data_7_3 : 8U;
    uint8_t BLE_Uplink_Data_8_0 : 8U;
    uint8_t BLE_Uplink_Data_8_1 : 8U;
    uint8_t BLE_Uplink_Data_8_2 : 8U;
    uint8_t BLE_Uplink_Data_8_3 : 8U;
    uint8_t BLE_Uplink_Data_9_0 : 8U;
    uint8_t BLE_Uplink_Data_9_1 : 8U;
    uint8_t BLE_Uplink_Data_9_2 : 8U;
    uint8_t BLE_Uplink_Data_9_3 : 8U;
    uint8_t BLE_Uplink_Data_10_0 : 8U;
    uint8_t BLE_Uplink_Data_10_1 : 8U;
    uint8_t BLE_Uplink_Data_10_2 : 8U;
    uint8_t BLE_Uplink_Data_10_3 : 8U;
    uint8_t BLE_Uplink_Data_11_0 : 8U;
    uint8_t BLE_Uplink_Data_11_1 : 8U;
    uint8_t BLE_Uplink_Data_11_2 : 8U;
    uint8_t BLE_Uplink_Data_11_3 : 8U;
    uint8_t BLE_Uplink_Data_12_0 : 8U;
    uint8_t BLE_Uplink_Data_12_1 : 8U;
    uint8_t BLE_Uplink_Data_12_2 : 8U;
    uint8_t BLE_Uplink_Data_12_3 : 8U;
    uint8_t BLE_Uplink_Data_13_0 : 8U;
    uint8_t BLE_Uplink_Data_13_1 : 8U;
    uint8_t BLE_Uplink_Data_13_2 : 8U;
    uint8_t BLE_Uplink_Data_13_3 : 8U;
    uint8_t BLE_Uplink_Data_14_0 : 8U;
    uint8_t BLE_Uplink_Data_14_1 : 8U;
    uint8_t BLE_Uplink_Data_14_2 : 8U;
    uint8_t BLE_Uplink_Data_14_3 : 8U;
    uint8_t BLE_Uplink_Data_15_0 : 8U;
    uint8_t BLE_Uplink_Data_15_1 : 8U;
    uint8_t BLE_Uplink_Data_15_2 : 8U;
    uint8_t BLE_Uplink_Data_15_3 : 8U;
  }sig;
} BLE_Uplink_Data_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_Uplink_Data_un msg;
}BLE_Uplink_Data_msg;

// cantp max_size:200 flow_control_frame_id:1250
// def @BLE_Downlink_FlowCtrl CAN Message (1253 0x4e5)
#define BLE_Downlink_FlowCtrl_IDE (0U)
#define BLE_Downlink_FlowCtrl_DLC (64U)
#define BLE_Downlink_FlowCtrl_CANID (0x4e5U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t BLE_Downlink_FlowCtrl_0_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_0_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_0_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_0_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_1_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_1_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_1_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_1_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_2_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_2_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_2_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_2_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_3_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_3_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_3_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_3_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_4_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_4_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_4_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_4_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_5_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_5_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_5_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_5_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_6_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_6_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_6_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_6_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_7_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_7_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_7_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_7_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_8_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_8_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_8_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_8_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_9_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_9_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_9_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_9_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_10_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_10_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_10_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_10_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_11_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_11_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_11_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_11_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_12_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_12_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_12_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_12_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_13_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_13_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_13_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_13_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_14_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_14_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_14_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_14_3 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_15_0 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_15_1 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_15_2 : 8U;
    uint8_t BLE_Downlink_FlowCtrl_15_3 : 8U;
  }sig;
} BLE_Downlink_FlowCtrl_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_Downlink_FlowCtrl_un msg;
}BLE_Downlink_FlowCtrl_msg;

// def @BLE_DK_REQ CAN Message (1259 0x4eb)
#define BLE_DK_REQ_IDE (0U)
#define BLE_DK_REQ_DLC (8U)
#define BLE_DK_REQ_CANID (0x4ebU)

typedef enum
{
    No_Request_823 = 0U,
    Terminate_Key_824 = 1U,
    Remove_Key_Permanently_825 = 2U,
    Recovery_Key_826 = 3U,
    Reserved_827 = 4U,
    Reserved_828 = 5U,
    Reserved_829 = 6U,
    Reserved_830 = 7U,
    Reserved_831 = 8U,
    Reserved_832 = 9U,
    Reserved_833 = 10U,
    Reserved_834 = 11U,
    Reserved_835 = 12U,
    Reserved_836 = 13U,
    Reserved_837 = 14U,
    Reserved_838 = 15U
}DK_Magnt_Req_Type_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    uint8_t DK_Magnt_Req_DevInfo_0 : 8U;
    uint8_t DK_Magnt_Req_DevInfo_1 : 8U;
    uint8_t DK_Magnt_Req_DevInfo_2 : 8U;
    uint8_t DK_Magnt_Req_DevInfo_3 : 8U;
    uint8_t DK_Magnt_Req_DevInfo_4 : 8U;
    uint8_t DK_Magnt_Req_DevInfo_5 : 8U;
    // digital key management request type
    //  0 : "No Request"
    //  1 : "Terminate Key"
    //  2 : "Remove Key Permanently"
    //  3 : "Recovery Key"
    //  4 : "Reserved"
    //  5 : "Reserved"
    //  6 : "Reserved"
    //  7 : "Reserved"
    //  8 : "Reserved"
    //  9 : "Reserved"
    //  10 : "Reserved"
    //  11 : "Reserved"
    //  12 : "Reserved"
    //  13 : "Reserved"
    //  14 : "Reserved"
    //  15 : "Reserved"
    DK_Magnt_Req_Type_t DK_Magnt_Req_Type : 4U;
    uint32_t reserved0 : 4U;

    uint32_t reserved1 : 8U;
  }sig;
} BLE_DK_REQ_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_DK_REQ_un msg;
}BLE_DK_REQ_msg;

// def @BLE_DK_RSP CAN Message (1263 0x4ef)
#define BLE_DK_RSP_IDE (0U)
#define BLE_DK_RSP_DLC (8U)
#define BLE_DK_RSP_CANID (0x4efU)

typedef enum
{
    DK_Magnt_Rsp_FR_None_839 = 0U,
    DK_Magnt_Rsp_FR_Device_not_existed_840 = 1U,
    DK_Magnt_Rsp_FR_Reserved_841 = 2U,
    DK_Magnt_Rsp_FR_Reserved_842 = 3U,
    DK_Magnt_Rsp_FR_Reserved_843 = 4U,
    DK_Magnt_Rsp_FR_Reserved_844 = 5U,
    DK_Magnt_Rsp_FR_Reserved_845 = 6U,
    DK_Magnt_Rsp_FR_Reserved_846 = 7U,
    DK_Magnt_Rsp_FR_Reserved_847 = 8U,
    DK_Magnt_Rsp_FR_Reserved_848 = 9U,
    DK_Magnt_Rsp_FR_Reserved_849 = 10U,
    DK_Magnt_Rsp_FR_Reserved_850 = 11U,
    DK_Magnt_Rsp_FR_Reserved_851 = 12U,
    DK_Magnt_Rsp_FR_Reserved_852 = 13U,
    DK_Magnt_Rsp_FR_Reserved_853 = 14U,
    DK_Magnt_Rsp_FR_Reserved_854 = 15U
}DK_Magnt_Rsp_FR_t;

typedef enum
{
    No_Request_855 = 0U,
    Terminate_Key_856 = 1U,
    Remove_Key_Permanently_857 = 2U,
    Recovery_Key_858 = 3U,
    Reserved_859 = 4U,
    Reserved_860 = 5U,
    Reserved_861 = 6U,
    Reserved_862 = 7U,
    Reserved_863 = 8U,
    Reserved_864 = 9U,
    Reserved_865 = 10U,
    Reserved_866 = 11U,
    Reserved_867 = 12U,
    Reserved_868 = 13U,
    Reserved_869 = 14U,
    Reserved_870 = 15U
}DK_Magnt_Rsp_ReqType_t;

typedef enum
{
    No_result_871 = 0U,
    Success_872 = 1U,
    Fail_873 = 2U,
    Reserved_874 = 3U,
    Reserved_875 = 4U,
    Reserved_876 = 5U,
    Reserved_877 = 6U,
    Reserved_878 = 7U,
    Reserved_879 = 8U,
    Reserved_880 = 9U,
    Reserved_881 = 10U,
    Reserved_882 = 11U,
    Reserved_883 = 12U,
    Reserved_884 = 13U,
    Reserved_885 = 14U,
    Reserved_886 = 15U
}DK_Magnt_Rsp_Result_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    uint8_t DK_Magnt_Rsp_DevInfo_0 : 8U;
    uint8_t DK_Magnt_Rsp_DevInfo_1 : 8U;
    uint8_t DK_Magnt_Rsp_DevInfo_2 : 8U;
    uint8_t DK_Magnt_Rsp_DevInfo_3 : 8U;
    uint8_t DK_Magnt_Rsp_DevInfo_4 : 8U;
    uint8_t DK_Magnt_Rsp_DevInfo_5 : 8U;
    // Digital Key management Response
    //  0 : "None"
    //  1 : "Device not existed"
    //  2 : "Reserved"
    //  3 : "Reserved"
    //  4 : "Reserved"
    //  5 : "Reserved"
    //  6 : "Reserved"
    //  7 : "Reserved"
    //  8 : "Reserved"
    //  9 : "Reserved"
    //  10 : "Reserved"
    //  11 : "Reserved"
    //  12 : "Reserved"
    //  13 : "Reserved"
    //  14 : "Reserved"
    //  15 : "Reserved"
    DK_Magnt_Rsp_FR_t DK_Magnt_Rsp_FR : 4U;
    // Digital Key management Response Result Type
    //  0 : "No Request"
    //  1 : "Terminate Key"
    //  2 : "Remove Key Permanently"
    //  3 : "Recovery Key"
    //  4 : "Reserved"
    //  5 : "Reserved"
    //  6 : "Reserved"
    //  7 : "Reserved"
    //  8 : "Reserved"
    //  9 : "Reserved"
    //  10 : "Reserved"
    //  11 : "Reserved"
    //  12 : "Reserved"
    //  13 : "Reserved"
    //  14 : "Reserved"
    //  15 : "Reserved"
    DK_Magnt_Rsp_ReqType_t DK_Magnt_Rsp_ReqType : 4U;
    // Digital Key management Response Result
    //  0 : "No result"
    //  1 : "Success"
    //  2 : "Fail"
    //  3 : "Reserved"
    //  4 : "Reserved"
    //  5 : "Reserved"
    //  6 : "Reserved"
    //  7 : "Reserved"
    //  8 : "Reserved"
    //  9 : "Reserved"
    //  10 : "Reserved"
    //  11 : "Reserved"
    //  12 : "Reserved"
    //  13 : "Reserved"
    //  14 : "Reserved"
    //  15 : "Reserved"
    DK_Magnt_Rsp_Result_t DK_Magnt_Rsp_Result : 4U;
    uint32_t reserved0 : 4U;
  }sig;
} BLE_DK_RSP_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BLE_DK_RSP_un msg;
}BLE_DK_RSP_msg;

// def @BTA_SRF_FactoryRsp CAN Message (1272 0x4f8)
#define BTA_SRF_FactoryRsp_IDE (0U)
#define BTA_SRF_FactoryRsp_DLC (64U)
#define BTA_SRF_FactoryRsp_CANID (0x4f8U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t BTA_SRF_FactoryRsp_0_0 : 8U;
    uint8_t BTA_SRF_FactoryRsp_0_1 : 8U;
    uint8_t BTA_SRF_FactoryRsp_0_2 : 8U;
    uint8_t BTA_SRF_FactoryRsp_0_3 : 8U;
    uint8_t BTA_SRF_FactoryRsp_0_4 : 8U;
    uint8_t BTA_SRF_FactoryRsp_0_5 : 8U;
    uint8_t BTA_SRF_FactoryRsp_0_6 : 8U;
    uint8_t BTA_SRF_FactoryRsp_0_7 : 8U;
    uint8_t BTA_SRF_FactoryRsp_1_0 : 8U;
    uint8_t BTA_SRF_FactoryRsp_1_1 : 8U;
    uint8_t BTA_SRF_FactoryRsp_1_2 : 8U;
    uint8_t BTA_SRF_FactoryRsp_1_3 : 8U;
    uint8_t BTA_SRF_FactoryRsp_1_4 : 8U;
    uint8_t BTA_SRF_FactoryRsp_1_5 : 8U;
    uint8_t BTA_SRF_FactoryRsp_1_6 : 8U;
    uint8_t BTA_SRF_FactoryRsp_1_7 : 8U;
    uint8_t BTA_SRF_FactoryRsp_2_0 : 8U;
    uint8_t BTA_SRF_FactoryRsp_2_1 : 8U;
    uint8_t BTA_SRF_FactoryRsp_2_2 : 8U;
    uint8_t BTA_SRF_FactoryRsp_2_3 : 8U;
    uint8_t BTA_SRF_FactoryRsp_2_4 : 8U;
    uint8_t BTA_SRF_FactoryRsp_2_5 : 8U;
    uint8_t BTA_SRF_FactoryRsp_2_6 : 8U;
    uint8_t BTA_SRF_FactoryRsp_2_7 : 8U;
    uint8_t BTA_SRF_FactoryRsp_3_0 : 8U;
    uint8_t BTA_SRF_FactoryRsp_3_1 : 8U;
    uint8_t BTA_SRF_FactoryRsp_3_2 : 8U;
    uint8_t BTA_SRF_FactoryRsp_3_3 : 8U;
    uint8_t BTA_SRF_FactoryRsp_3_4 : 8U;
    uint8_t BTA_SRF_FactoryRsp_3_5 : 8U;
    uint8_t BTA_SRF_FactoryRsp_3_6 : 8U;
    uint8_t BTA_SRF_FactoryRsp_3_7 : 8U;
    uint8_t BTA_SRF_FactoryRsp_4_0 : 8U;
    uint8_t BTA_SRF_FactoryRsp_4_1 : 8U;
    uint8_t BTA_SRF_FactoryRsp_4_2 : 8U;
    uint8_t BTA_SRF_FactoryRsp_4_3 : 8U;
    uint8_t BTA_SRF_FactoryRsp_4_4 : 8U;
    uint8_t BTA_SRF_FactoryRsp_4_5 : 8U;
    uint8_t BTA_SRF_FactoryRsp_4_6 : 8U;
    uint8_t BTA_SRF_FactoryRsp_4_7 : 8U;
    uint8_t BTA_SRF_FactoryRsp_5_0 : 8U;
    uint8_t BTA_SRF_FactoryRsp_5_1 : 8U;
    uint8_t BTA_SRF_FactoryRsp_5_2 : 8U;
    uint8_t BTA_SRF_FactoryRsp_5_3 : 8U;
    uint8_t BTA_SRF_FactoryRsp_5_4 : 8U;
    uint8_t BTA_SRF_FactoryRsp_5_5 : 8U;
    uint8_t BTA_SRF_FactoryRsp_5_6 : 8U;
    uint8_t BTA_SRF_FactoryRsp_5_7 : 8U;
    uint8_t BTA_SRF_FactoryRsp_6_0 : 8U;
    uint8_t BTA_SRF_FactoryRsp_6_1 : 8U;
    uint8_t BTA_SRF_FactoryRsp_6_2 : 8U;
    uint8_t BTA_SRF_FactoryRsp_6_3 : 8U;
    uint8_t BTA_SRF_FactoryRsp_6_4 : 8U;
    uint8_t BTA_SRF_FactoryRsp_6_5 : 8U;
    uint8_t BTA_SRF_FactoryRsp_6_6 : 8U;
    uint8_t BTA_SRF_FactoryRsp_6_7 : 8U;
    uint8_t BTA_SRF_FactoryRsp_7_0 : 8U;
    uint8_t BTA_SRF_FactoryRsp_7_1 : 8U;
    uint8_t BTA_SRF_FactoryRsp_7_2 : 8U;
    uint8_t BTA_SRF_FactoryRsp_7_3 : 8U;
    uint8_t BTA_SRF_FactoryRsp_7_4 : 8U;
    uint8_t BTA_SRF_FactoryRsp_7_5 : 8U;
    uint8_t BTA_SRF_FactoryRsp_7_6 : 8U;
    uint8_t BTA_SRF_FactoryRsp_7_7 : 8U;
  }sig;
} BTA_SRF_FactoryRsp_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA_SRF_FactoryRsp_un msg;
}BTA_SRF_FactoryRsp_msg;

// def @BTA_SRF_FactoryReq CAN Message (1273 0x4f9)
#define BTA_SRF_FactoryReq_IDE (0U)
#define BTA_SRF_FactoryReq_DLC (64U)
#define BTA_SRF_FactoryReq_CANID (0x4f9U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t BTA_SRF_FactoryReq_0_0 : 8U;
    uint8_t BTA_SRF_FactoryReq_0_1 : 8U;
    uint8_t BTA_SRF_FactoryReq_0_2 : 8U;
    uint8_t BTA_SRF_FactoryReq_0_3 : 8U;
    uint8_t BTA_SRF_FactoryReq_0_4 : 8U;
    uint8_t BTA_SRF_FactoryReq_0_5 : 8U;
    uint8_t BTA_SRF_FactoryReq_0_6 : 8U;
    uint8_t BTA_SRF_FactoryReq_0_7 : 8U;
    uint8_t BTA_SRF_FactoryReq_1_0 : 8U;
    uint8_t BTA_SRF_FactoryReq_1_1 : 8U;
    uint8_t BTA_SRF_FactoryReq_1_2 : 8U;
    uint8_t BTA_SRF_FactoryReq_1_3 : 8U;
    uint8_t BTA_SRF_FactoryReq_1_4 : 8U;
    uint8_t BTA_SRF_FactoryReq_1_5 : 8U;
    uint8_t BTA_SRF_FactoryReq_1_6 : 8U;
    uint8_t BTA_SRF_FactoryReq_1_7 : 8U;
    uint8_t BTA_SRF_FactoryReq_2_0 : 8U;
    uint8_t BTA_SRF_FactoryReq_2_1 : 8U;
    uint8_t BTA_SRF_FactoryReq_2_2 : 8U;
    uint8_t BTA_SRF_FactoryReq_2_3 : 8U;
    uint8_t BTA_SRF_FactoryReq_2_4 : 8U;
    uint8_t BTA_SRF_FactoryReq_2_5 : 8U;
    uint8_t BTA_SRF_FactoryReq_2_6 : 8U;
    uint8_t BTA_SRF_FactoryReq_2_7 : 8U;
    uint8_t BTA_SRF_FactoryReq_3_0 : 8U;
    uint8_t BTA_SRF_FactoryReq_3_1 : 8U;
    uint8_t BTA_SRF_FactoryReq_3_2 : 8U;
    uint8_t BTA_SRF_FactoryReq_3_3 : 8U;
    uint8_t BTA_SRF_FactoryReq_3_4 : 8U;
    uint8_t BTA_SRF_FactoryReq_3_5 : 8U;
    uint8_t BTA_SRF_FactoryReq_3_6 : 8U;
    uint8_t BTA_SRF_FactoryReq_3_7 : 8U;
    uint8_t BTA_SRF_FactoryReq_4_0 : 8U;
    uint8_t BTA_SRF_FactoryReq_4_1 : 8U;
    uint8_t BTA_SRF_FactoryReq_4_2 : 8U;
    uint8_t BTA_SRF_FactoryReq_4_3 : 8U;
    uint8_t BTA_SRF_FactoryReq_4_4 : 8U;
    uint8_t BTA_SRF_FactoryReq_4_5 : 8U;
    uint8_t BTA_SRF_FactoryReq_4_6 : 8U;
    uint8_t BTA_SRF_FactoryReq_4_7 : 8U;
    uint8_t BTA_SRF_FactoryReq_5_0 : 8U;
    uint8_t BTA_SRF_FactoryReq_5_1 : 8U;
    uint8_t BTA_SRF_FactoryReq_5_2 : 8U;
    uint8_t BTA_SRF_FactoryReq_5_3 : 8U;
    uint8_t BTA_SRF_FactoryReq_5_4 : 8U;
    uint8_t BTA_SRF_FactoryReq_5_5 : 8U;
    uint8_t BTA_SRF_FactoryReq_5_6 : 8U;
    uint8_t BTA_SRF_FactoryReq_5_7 : 8U;
    uint8_t BTA_SRF_FactoryReq_6_0 : 8U;
    uint8_t BTA_SRF_FactoryReq_6_1 : 8U;
    uint8_t BTA_SRF_FactoryReq_6_2 : 8U;
    uint8_t BTA_SRF_FactoryReq_6_3 : 8U;
    uint8_t BTA_SRF_FactoryReq_6_4 : 8U;
    uint8_t BTA_SRF_FactoryReq_6_5 : 8U;
    uint8_t BTA_SRF_FactoryReq_6_6 : 8U;
    uint8_t BTA_SRF_FactoryReq_6_7 : 8U;
    uint8_t BTA_SRF_FactoryReq_7_0 : 8U;
    uint8_t BTA_SRF_FactoryReq_7_1 : 8U;
    uint8_t BTA_SRF_FactoryReq_7_2 : 8U;
    uint8_t BTA_SRF_FactoryReq_7_3 : 8U;
    uint8_t BTA_SRF_FactoryReq_7_4 : 8U;
    uint8_t BTA_SRF_FactoryReq_7_5 : 8U;
    uint8_t BTA_SRF_FactoryReq_7_6 : 8U;
    uint8_t BTA_SRF_FactoryReq_7_7 : 8U;
  }sig;
} BTA_SRF_FactoryReq_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  BTA_SRF_FactoryReq_un msg;
}BTA_SRF_FactoryReq_msg;

// def @NM_ZONE_LEM CAN Message (1285 0x505)
#define NM_ZONE_LEM_IDE (0U)
#define NM_ZONE_LEM_DLC (8U)
#define NM_ZONE_LEM_CANID (0x505U)

typedef enum
{
    No_repeat_message_request_887 = 0U,
    Repeat_message_request_888 = 1U
}RepMsgReq_ZONE_LEM_t;

typedef enum
{
    PN1_ZONE_LEM_Not_awake_889 = 0U,
    PN1_ZONE_LEM_Awake_890 = 1U
}PN1_ZONE_LEM_t;

typedef enum
{
    PN2_ZONE_LEM_Not_awake_891 = 0U,
    PN2_ZONE_LEM_Awake_892 = 1U
}PN2_ZONE_LEM_t;

typedef enum
{
    PN3_ZONE_LEM_Not_awake_893 = 0U,
    PN3_ZONE_LEM_Awake_894 = 1U
}PN3_ZONE_LEM_t;

typedef enum
{
    PN4_ZONE_LEM_Not_awake_895 = 0U,
    PN4_ZONE_LEM_Awake_896 = 1U
}PN4_ZONE_LEM_t;

typedef enum
{
    PN5_ZONE_LEM_Not_awake_897 = 0U,
    PN5_ZONE_LEM_Awake_898 = 1U
}PN5_ZONE_LEM_t;

typedef enum
{
    PN6_ZONE_LEM_Not_awake_899 = 0U,
    PN6_ZONE_LEM_Awake_900 = 1U
}PN6_ZONE_LEM_t;

typedef enum
{
    PN7_ZONE_LEM_Not_awake_901 = 0U,
    PN7_ZONE_LEM_Awake_902 = 1U
}PN7_ZONE_LEM_t;

typedef enum
{
    PN8_ZONE_LEM_Not_awake_903 = 0U,
    PN8_ZONE_LEM_Awake_904 = 1U
}PN8_ZONE_LEM_t;

typedef enum
{
    PN9_ZONE_LEM_Not_awake_905 = 0U,
    PN9_ZONE_LEM_Awake_906 = 1U
}PN9_ZONE_LEM_t;

typedef enum
{
    PN10_ZONE_LEM_Not_awake_907 = 0U,
    PN10_ZONE_LEM_Awake_908 = 1U
}PN10_ZONE_LEM_t;

typedef enum
{
    PN11_ZONE_LEM_Not_awake_909 = 0U,
    PN11_ZONE_LEM_Awake_910 = 1U
}PN11_ZONE_LEM_t;

typedef enum
{
    PN12_ZONE_LEM_Not_awake_911 = 0U,
    PN12_ZONE_LEM_Awake_912 = 1U
}PN12_ZONE_LEM_t;

typedef enum
{
    PN13_ZONE_LEM_Not_awake_913 = 0U,
    PN13_ZONE_LEM_Awake_914 = 1U
}PN13_ZONE_LEM_t;

typedef enum
{
    PN14_ZONE_LEM_Not_awake_915 = 0U,
    PN14_ZONE_LEM_Awake_916 = 1U
}PN14_ZONE_LEM_t;

typedef enum
{
    PN15_ZONE_LEM_Not_awake_917 = 0U,
    PN15_ZONE_LEM_Awake_918 = 1U
}PN15_ZONE_LEM_t;

typedef enum
{
    PN16_ZONE_LEM_Not_awake_919 = 0U,
    PN16_ZONE_LEM_Awake_920 = 1U
}PN16_ZONE_LEM_t;

typedef enum
{
    PN17_ZONE_LEM_Not_awake_921 = 0U,
    PN17_ZONE_LEM_Awake_922 = 1U
}PN17_ZONE_LEM_t;

typedef enum
{
    PN18_ZONE_LEM_Not_awake_923 = 0U,
    PN18_ZONE_LEM_Awake_924 = 1U
}PN18_ZONE_LEM_t;

typedef enum
{
    PN19_ZONE_LEM_Not_awake_925 = 0U,
    PN19_ZONE_LEM_Awake_926 = 1U
}PN19_ZONE_LEM_t;

typedef enum
{
    PN20_ZONE_LEM_Not_awake_927 = 0U,
    PN20_ZONE_LEM_Awake_928 = 1U
}PN20_ZONE_LEM_t;

typedef enum
{
    PN21_ZONE_LEM_Not_awake_929 = 0U,
    PN21_ZONE_LEM_Awake_930 = 1U
}PN21_ZONE_LEM_t;

typedef enum
{
    PN22_ZONE_LEM_Not_awake_931 = 0U,
    PN22_ZONE_LEM_Awake_932 = 1U
}PN22_ZONE_LEM_t;

typedef enum
{
    PN23_ZONE_LEM_Not_awake_933 = 0U,
    PN23_ZONE_LEM_Awake_934 = 1U
}PN23_ZONE_LEM_t;

typedef enum
{
    PN24_ZONE_LEM_Not_awake_935 = 0U,
    PN24_ZONE_LEM_Awake_936 = 1U
}PN24_ZONE_LEM_t;

typedef enum
{
    PN25_ZONE_LEM_Not_awake_937 = 0U,
    PN25_ZONE_LEM_Awake_938 = 1U
}PN25_ZONE_LEM_t;

typedef enum
{
    PN26_ZONE_LEM_Not_awake_939 = 0U,
    PN26_ZONE_LEM_Awake_940 = 1U
}PN26_ZONE_LEM_t;

typedef enum
{
    PN27_ZONE_LEM_Not_awake_941 = 0U,
    PN27_ZONE_LEM_Awake_942 = 1U
}PN27_ZONE_LEM_t;

typedef enum
{
    PN28_ZONE_LEM_Not_awake_943 = 0U,
    PN28_ZONE_LEM_Awake_944 = 1U
}PN28_ZONE_LEM_t;

typedef enum
{
    PN29_ZONE_LEM_Not_awake_945 = 0U,
    PN29_ZONE_LEM_Awake_946 = 1U
}PN29_ZONE_LEM_t;

typedef enum
{
    PN30_ZONE_LEM_Not_awake_947 = 0U,
    PN30_ZONE_LEM_Awake_948 = 1U
}PN30_ZONE_LEM_t;

typedef enum
{
    PN31_ZONE_LEM_Not_awake_949 = 0U,
    PN31_ZONE_LEM_Awake_950 = 1U
}PN31_ZONE_LEM_t;

typedef enum
{
    PN32_ZONE_LEM_Not_awake_951 = 0U,
    PN32_ZONE_LEM_Awake_952 = 1U
}PN32_ZONE_LEM_t;

typedef enum
{
    PN33_ZONE_LEM_Not_awake_953 = 0U,
    PN33_ZONE_LEM_Awake_954 = 1U
}PN33_ZONE_LEM_t;

typedef enum
{
    PN34_ZONE_LEM_Not_awake_955 = 0U,
    PN34_ZONE_LEM_Awake_956 = 1U
}PN34_ZONE_LEM_t;

typedef enum
{
    PN35_ZONE_LEM_Not_awake_957 = 0U,
    PN35_ZONE_LEM_Awake_958 = 1U
}PN35_ZONE_LEM_t;

typedef enum
{
    PN36_ZONE_LEM_Not_awake_959 = 0U,
    PN36_ZONE_LEM_Awake_960 = 1U
}PN36_ZONE_LEM_t;

typedef enum
{
    PN37_ZONE_LEM_Not_awake_961 = 0U,
    PN37_ZONE_LEM_Awake_962 = 1U
}PN37_ZONE_LEM_t;

typedef enum
{
    PN38_ZONE_LEM_Not_awake_963 = 0U,
    PN38_ZONE_LEM_Awake_964 = 1U
}PN38_ZONE_LEM_t;

typedef enum
{
    PN39_ZONE_LEM_Not_awake_965 = 0U,
    PN39_ZONE_LEM_Awake_966 = 1U
}PN39_ZONE_LEM_t;

typedef enum
{
    PN40_ZONE_LEM_Not_awake_967 = 0U,
    PN40_ZONE_LEM_Awake_968 = 1U
}PN40_ZONE_LEM_t;

typedef enum
{
    PN41_ZONE_LEM_Not_awake_969 = 0U,
    PN41_ZONE_LEM_Awake_970 = 1U
}PN41_ZONE_LEM_t;

typedef enum
{
    PN42_ZONE_LEM_Not_awake_971 = 0U,
    PN42_ZONE_LEM_Awake_972 = 1U
}PN42_ZONE_LEM_t;

typedef enum
{
    PN43_ZONE_LEM_Not_awake_973 = 0U,
    PN43_ZONE_LEM_Awake_974 = 1U
}PN43_ZONE_LEM_t;

typedef enum
{
    PN44_ZONE_LEM_Not_awake_975 = 0U,
    PN44_ZONE_LEM_Awake_976 = 1U
}PN44_ZONE_LEM_t;

typedef enum
{
    PN45_ZONE_LEM_Not_awake_977 = 0U,
    PN45_ZONE_LEM_Awake_978 = 1U
}PN45_ZONE_LEM_t;

typedef enum
{
    PN46_ZONE_LEM_Not_awake_979 = 0U,
    PN46_ZONE_LEM_Awake_980 = 1U
}PN46_ZONE_LEM_t;

typedef enum
{
    PN47_ZONE_LEM_Not_awake_981 = 0U,
    PN47_ZONE_LEM_Awake_982 = 1U
}PN47_ZONE_LEM_t;

typedef enum
{
    PN48_ZONE_LEM_Not_awake_983 = 0U,
    PN48_ZONE_LEM_Awake_984 = 1U
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
    RepMsgReq_BTA1_No_repeat_message_request_985 = 0U,
    RepMsgReq_BTA1_Repeat_message_request_986 = 1U
}RepMsgReq_BTA1_t;

typedef enum
{
    PN1_BTA1_Not_awake_987 = 0U,
    PN1_BTA1_Awake_988 = 1U
}PN1_BTA1_t;

typedef enum
{
    PN2_BTA1_Not_awake_989 = 0U,
    PN2_BTA1_Awake_990 = 1U
}PN2_BTA1_t;

typedef enum
{
    PN3_BTA1_Not_awake_991 = 0U,
    PN3_BTA1_Awake_992 = 1U
}PN3_BTA1_t;

typedef enum
{
    PN4_BTA1_Not_awake_993 = 0U,
    PN4_BTA1_Awake_994 = 1U
}PN4_BTA1_t;

typedef enum
{
    PN5_BTA1_Not_awake_995 = 0U,
    PN5_BTA1_Awake_996 = 1U
}PN5_BTA1_t;

typedef enum
{
    PN6_BTA1_Not_awake_997 = 0U,
    PN6_BTA1_Awake_998 = 1U
}PN6_BTA1_t;

typedef enum
{
    PN7_BTA1_Not_awake_999 = 0U,
    PN7_BTA1_Awake_1000 = 1U
}PN7_BTA1_t;

typedef enum
{
    PN8_BTA1_Not_awake_1001 = 0U,
    PN8_BTA1_Awake_1002 = 1U
}PN8_BTA1_t;

typedef enum
{
    PN9_BTA1_Not_awake_1003 = 0U,
    PN9_BTA1_Awake_1004 = 1U
}PN9_BTA1_t;

typedef enum
{
    PN10_BTA1_Not_awake_1005 = 0U,
    PN10_BTA1_Awake_1006 = 1U
}PN10_BTA1_t;

typedef enum
{
    PN11_BTA1_Not_awake_1007 = 0U,
    PN11_BTA1_Awake_1008 = 1U
}PN11_BTA1_t;

typedef enum
{
    PN12_BTA1_Not_awake_1009 = 0U,
    PN12_BTA1_Awake_1010 = 1U
}PN12_BTA1_t;

typedef enum
{
    PN13_BTA1_Not_awake_1011 = 0U,
    PN13_BTA1_Awake_1012 = 1U
}PN13_BTA1_t;

typedef enum
{
    PN14_BTA1_Not_awake_1013 = 0U,
    PN14_BTA1_Awake_1014 = 1U
}PN14_BTA1_t;

typedef enum
{
    PN15_BTA1_Not_awake_1015 = 0U,
    PN15_BTA1_Awake_1016 = 1U
}PN15_BTA1_t;

typedef enum
{
    PN16_BTA1_Not_awake_1017 = 0U,
    PN16_BTA1_Awake_1018 = 1U
}PN16_BTA1_t;

typedef enum
{
    PN17_BTA1_Not_awake_1019 = 0U,
    PN17_BTA1_Awake_1020 = 1U
}PN17_BTA1_t;

typedef enum
{
    PN18_BTA1_Not_awake_1021 = 0U,
    PN18_BTA1_Awake_1022 = 1U
}PN18_BTA1_t;

typedef enum
{
    PN19_BTA1_Not_awake_1023 = 0U,
    PN19_BTA1_Awake_1024 = 1U
}PN19_BTA1_t;

typedef enum
{
    PN20_BTA1_Not_awake_1025 = 0U,
    PN20_BTA1_Awake_1026 = 1U
}PN20_BTA1_t;

typedef enum
{
    PN21_BTA1_Not_awake_1027 = 0U,
    PN21_BTA1_Awake_1028 = 1U
}PN21_BTA1_t;

typedef enum
{
    PN22_BTA1_Not_awake_1029 = 0U,
    PN22_BTA1_Awake_1030 = 1U
}PN22_BTA1_t;

typedef enum
{
    PN23_BTA1_Not_awake_1031 = 0U,
    PN23_BTA1_Awake_1032 = 1U
}PN23_BTA1_t;

typedef enum
{
    PN24_BTA1_Not_awake_1033 = 0U,
    PN24_BTA1_Awake_1034 = 1U
}PN24_BTA1_t;

typedef enum
{
    PN25_BTA1_Not_awake_1035 = 0U,
    PN25_BTA1_Awake_1036 = 1U
}PN25_BTA1_t;

typedef enum
{
    PN26_BTA1_Not_awake_1037 = 0U,
    PN26_BTA1_Awake_1038 = 1U
}PN26_BTA1_t;

typedef enum
{
    PN27_BTA1_Not_awake_1039 = 0U,
    PN27_BTA1_Awake_1040 = 1U
}PN27_BTA1_t;

typedef enum
{
    PN28_BTA1_Not_awake_1041 = 0U,
    PN28_BTA1_Awake_1042 = 1U
}PN28_BTA1_t;

typedef enum
{
    PN29_BTA1_Not_awake_1043 = 0U,
    PN29_BTA1_Awake_1044 = 1U
}PN29_BTA1_t;

typedef enum
{
    PN30_BTA1_Not_awake_1045 = 0U,
    PN30_BTA1_Awake_1046 = 1U
}PN30_BTA1_t;

typedef enum
{
    PN31_BTA1_Not_awake_1047 = 0U,
    PN31_BTA1_Awake_1048 = 1U
}PN31_BTA1_t;

typedef enum
{
    PN32_BTA1_Not_awake_1049 = 0U,
    PN32_BTA1_Awake_1050 = 1U
}PN32_BTA1_t;

typedef enum
{
    PN33_BTA1_Not_awake_1051 = 0U,
    PN33_BTA1_Awake_1052 = 1U
}PN33_BTA1_t;

typedef enum
{
    PN34_BTA1_Not_awake_1053 = 0U,
    PN34_BTA1_Awake_1054 = 1U
}PN34_BTA1_t;

typedef enum
{
    PN35_BTA1_Not_awake_1055 = 0U,
    PN35_BTA1_Awake_1056 = 1U
}PN35_BTA1_t;

typedef enum
{
    PN36_BTA1_Not_awake_1057 = 0U,
    PN36_BTA1_Awake_1058 = 1U
}PN36_BTA1_t;

typedef enum
{
    PN37_BTA1_Not_awake_1059 = 0U,
    PN37_BTA1_Awake_1060 = 1U
}PN37_BTA1_t;

typedef enum
{
    PN38_BTA1_Not_awake_1061 = 0U,
    PN38_BTA1_Awake_1062 = 1U
}PN38_BTA1_t;

typedef enum
{
    PN39_BTA1_Not_awake_1063 = 0U,
    PN39_BTA1_Awake_1064 = 1U
}PN39_BTA1_t;

typedef enum
{
    PN40_BTA1_Not_awake_1065 = 0U,
    PN40_BTA1_Awake_1066 = 1U
}PN40_BTA1_t;

typedef enum
{
    PN41_BTA1_Not_awake_1067 = 0U,
    PN41_BTA1_Awake_1068 = 1U
}PN41_BTA1_t;

typedef enum
{
    PN42_BTA1_Not_awake_1069 = 0U,
    PN42_BTA1_Awake_1070 = 1U
}PN42_BTA1_t;

typedef enum
{
    PN43_BTA1_Not_awake_1071 = 0U,
    PN43_BTA1_Awake_1072 = 1U
}PN43_BTA1_t;

typedef enum
{
    PN44_BTA1_Not_awake_1073 = 0U,
    PN44_BTA1_Awake_1074 = 1U
}PN44_BTA1_t;

typedef enum
{
    PN45_BTA1_Not_awake_1075 = 0U,
    PN45_BTA1_Awake_1076 = 1U
}PN45_BTA1_t;

typedef enum
{
    PN46_BTA1_Not_awake_1077 = 0U,
    PN46_BTA1_Awake_1078 = 1U
}PN46_BTA1_t;

typedef enum
{
    PN47_BTA1_Not_awake_1079 = 0U,
    PN47_BTA1_Awake_1080 = 1U
}PN47_BTA1_t;

typedef enum
{
    PN48_BTA1_Not_awake_1081 = 0U,
    PN48_BTA1_Awake_1082 = 1U
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
    RepMsgReq_BTA2_No_repeat_message_request_1083 = 0U,
    RepMsgReq_BTA2_Repeat_message_request_1084 = 1U
}RepMsgReq_BTA2_t;

typedef enum
{
    PN1_BTA2_Not_awake_1085 = 0U,
    PN1_BTA2_Awake_1086 = 1U
}PN1_BTA2_t;

typedef enum
{
    PN2_BTA2_Not_awake_1087 = 0U,
    PN2_BTA2_Awake_1088 = 1U
}PN2_BTA2_t;

typedef enum
{
    PN3_BTA2_Not_awake_1089 = 0U,
    PN3_BTA2_Awake_1090 = 1U
}PN3_BTA2_t;

typedef enum
{
    PN4_BTA2_Not_awake_1091 = 0U,
    PN4_BTA2_Awake_1092 = 1U
}PN4_BTA2_t;

typedef enum
{
    PN5_BTA2_Not_awake_1093 = 0U,
    PN5_BTA2_Awake_1094 = 1U
}PN5_BTA2_t;

typedef enum
{
    PN6_BTA2_Not_awake_1095 = 0U,
    PN6_BTA2_Awake_1096 = 1U
}PN6_BTA2_t;

typedef enum
{
    PN7_BTA2_Not_awake_1097 = 0U,
    PN7_BTA2_Awake_1098 = 1U
}PN7_BTA2_t;

typedef enum
{
    PN8_BTA2_Not_awake_1099 = 0U,
    PN8_BTA2_Awake_1100 = 1U
}PN8_BTA2_t;

typedef enum
{
    PN9_BTA2_Not_awake_1101 = 0U,
    PN9_BTA2_Awake_1102 = 1U
}PN9_BTA2_t;

typedef enum
{
    PN10_BTA2_Not_awake_1103 = 0U,
    PN10_BTA2_Awake_1104 = 1U
}PN10_BTA2_t;

typedef enum
{
    PN11_BTA2_Not_awake_1105 = 0U,
    PN11_BTA2_Awake_1106 = 1U
}PN11_BTA2_t;

typedef enum
{
    PN12_BTA2_Not_awake_1107 = 0U,
    PN12_BTA2_Awake_1108 = 1U
}PN12_BTA2_t;

typedef enum
{
    PN13_BTA2_Not_awake_1109 = 0U,
    PN13_BTA2_Awake_1110 = 1U
}PN13_BTA2_t;

typedef enum
{
    PN14_BTA2_Not_awake_1111 = 0U,
    PN14_BTA2_Awake_1112 = 1U
}PN14_BTA2_t;

typedef enum
{
    PN15_BTA2_Not_awake_1113 = 0U,
    PN15_BTA2_Awake_1114 = 1U
}PN15_BTA2_t;

typedef enum
{
    PN16_BTA2_Not_awake_1115 = 0U,
    PN16_BTA2_Awake_1116 = 1U
}PN16_BTA2_t;

typedef enum
{
    PN17_BTA2_Not_awake_1117 = 0U,
    PN17_BTA2_Awake_1118 = 1U
}PN17_BTA2_t;

typedef enum
{
    PN18_BTA2_Not_awake_1119 = 0U,
    PN18_BTA2_Awake_1120 = 1U
}PN18_BTA2_t;

typedef enum
{
    PN19_BTA2_Not_awake_1121 = 0U,
    PN19_BTA2_Awake_1122 = 1U
}PN19_BTA2_t;

typedef enum
{
    PN20_BTA2_Not_awake_1123 = 0U,
    PN20_BTA2_Awake_1124 = 1U
}PN20_BTA2_t;

typedef enum
{
    PN21_BTA2_Not_awake_1125 = 0U,
    PN21_BTA2_Awake_1126 = 1U
}PN21_BTA2_t;

typedef enum
{
    PN22_BTA2_Not_awake_1127 = 0U,
    PN22_BTA2_Awake_1128 = 1U
}PN22_BTA2_t;

typedef enum
{
    PN23_BTA2_Not_awake_1129 = 0U,
    PN23_BTA2_Awake_1130 = 1U
}PN23_BTA2_t;

typedef enum
{
    PN24_BTA2_Not_awake_1131 = 0U,
    PN24_BTA2_Awake_1132 = 1U
}PN24_BTA2_t;

typedef enum
{
    PN25_BTA2_Not_awake_1133 = 0U,
    PN25_BTA2_Awake_1134 = 1U
}PN25_BTA2_t;

typedef enum
{
    PN26_BTA2_Not_awake_1135 = 0U,
    PN26_BTA2_Awake_1136 = 1U
}PN26_BTA2_t;

typedef enum
{
    PN27_BTA2_Not_awake_1137 = 0U,
    PN27_BTA2_Awake_1138 = 1U
}PN27_BTA2_t;

typedef enum
{
    PN28_BTA2_Not_awake_1139 = 0U,
    PN28_BTA2_Awake_1140 = 1U
}PN28_BTA2_t;

typedef enum
{
    PN29_BTA2_Not_awake_1141 = 0U,
    PN29_BTA2_Awake_1142 = 1U
}PN29_BTA2_t;

typedef enum
{
    PN30_BTA2_Not_awake_1143 = 0U,
    PN30_BTA2_Awake_1144 = 1U
}PN30_BTA2_t;

typedef enum
{
    PN31_BTA2_Not_awake_1145 = 0U,
    PN31_BTA2_Awake_1146 = 1U
}PN31_BTA2_t;

typedef enum
{
    PN32_BTA2_Not_awake_1147 = 0U,
    PN32_BTA2_Awake_1148 = 1U
}PN32_BTA2_t;

typedef enum
{
    PN33_BTA2_Not_awake_1149 = 0U,
    PN33_BTA2_Awake_1150 = 1U
}PN33_BTA2_t;

typedef enum
{
    PN34_BTA2_Not_awake_1151 = 0U,
    PN34_BTA2_Awake_1152 = 1U
}PN34_BTA2_t;

typedef enum
{
    PN35_BTA2_Not_awake_1153 = 0U,
    PN35_BTA2_Awake_1154 = 1U
}PN35_BTA2_t;

typedef enum
{
    PN36_BTA2_Not_awake_1155 = 0U,
    PN36_BTA2_Awake_1156 = 1U
}PN36_BTA2_t;

typedef enum
{
    PN37_BTA2_Not_awake_1157 = 0U,
    PN37_BTA2_Awake_1158 = 1U
}PN37_BTA2_t;

typedef enum
{
    PN38_BTA2_Not_awake_1159 = 0U,
    PN38_BTA2_Awake_1160 = 1U
}PN38_BTA2_t;

typedef enum
{
    PN39_BTA2_Not_awake_1161 = 0U,
    PN39_BTA2_Awake_1162 = 1U
}PN39_BTA2_t;

typedef enum
{
    PN40_BTA2_Not_awake_1163 = 0U,
    PN40_BTA2_Awake_1164 = 1U
}PN40_BTA2_t;

typedef enum
{
    PN41_BTA2_Not_awake_1165 = 0U,
    PN41_BTA2_Awake_1166 = 1U
}PN41_BTA2_t;

typedef enum
{
    PN42_BTA2_Not_awake_1167 = 0U,
    PN42_BTA2_Awake_1168 = 1U
}PN42_BTA2_t;

typedef enum
{
    PN43_BTA2_Not_awake_1169 = 0U,
    PN43_BTA2_Awake_1170 = 1U
}PN43_BTA2_t;

typedef enum
{
    PN44_BTA2_Not_awake_1171 = 0U,
    PN44_BTA2_Awake_1172 = 1U
}PN44_BTA2_t;

typedef enum
{
    PN45_BTA2_Not_awake_1173 = 0U,
    PN45_BTA2_Awake_1174 = 1U
}PN45_BTA2_t;

typedef enum
{
    PN46_BTA2_Not_awake_1175 = 0U,
    PN46_BTA2_Awake_1176 = 1U
}PN46_BTA2_t;

typedef enum
{
    PN47_BTA2_Not_awake_1177 = 0U,
    PN47_BTA2_Awake_1178 = 1U
}PN47_BTA2_t;

typedef enum
{
    PN48_BTA2_Not_awake_1179 = 0U,
    PN48_BTA2_Awake_1180 = 1U
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
    RepMsgReq_BTA3_No_repeat_message_request_1181 = 0U,
    RepMsgReq_BTA3_Repeat_message_request_1182 = 1U
}RepMsgReq_BTA3_t;

typedef enum
{
    PN1_BTA3_Not_awake_1183 = 0U,
    PN1_BTA3_Awake_1184 = 1U
}PN1_BTA3_t;

typedef enum
{
    PN2_BTA3_Not_awake_1185 = 0U,
    PN2_BTA3_Awake_1186 = 1U
}PN2_BTA3_t;

typedef enum
{
    PN3_BTA3_Not_awake_1187 = 0U,
    PN3_BTA3_Awake_1188 = 1U
}PN3_BTA3_t;

typedef enum
{
    PN4_BTA3_Not_awake_1189 = 0U,
    PN4_BTA3_Awake_1190 = 1U
}PN4_BTA3_t;

typedef enum
{
    PN5_BTA3_Not_awake_1191 = 0U,
    PN5_BTA3_Awake_1192 = 1U
}PN5_BTA3_t;

typedef enum
{
    PN6_BTA3_Not_awake_1193 = 0U,
    PN6_BTA3_Awake_1194 = 1U
}PN6_BTA3_t;

typedef enum
{
    PN7_BTA3_Not_awake_1195 = 0U,
    PN7_BTA3_Awake_1196 = 1U
}PN7_BTA3_t;

typedef enum
{
    PN8_BTA3_Not_awake_1197 = 0U,
    PN8_BTA3_Awake_1198 = 1U
}PN8_BTA3_t;

typedef enum
{
    PN9_BTA3_Not_awake_1199 = 0U,
    PN9_BTA3_Awake_1200 = 1U
}PN9_BTA3_t;

typedef enum
{
    PN10_BTA3_Not_awake_1201 = 0U,
    PN10_BTA3_Awake_1202 = 1U
}PN10_BTA3_t;

typedef enum
{
    PN11_BTA3_Not_awake_1203 = 0U,
    PN11_BTA3_Awake_1204 = 1U
}PN11_BTA3_t;

typedef enum
{
    PN12_BTA3_Not_awake_1205 = 0U,
    PN12_BTA3_Awake_1206 = 1U
}PN12_BTA3_t;

typedef enum
{
    PN13_BTA3_Not_awake_1207 = 0U,
    PN13_BTA3_Awake_1208 = 1U
}PN13_BTA3_t;

typedef enum
{
    PN14_BTA3_Not_awake_1209 = 0U,
    PN14_BTA3_Awake_1210 = 1U
}PN14_BTA3_t;

typedef enum
{
    PN15_BTA3_Not_awake_1211 = 0U,
    PN15_BTA3_Awake_1212 = 1U
}PN15_BTA3_t;

typedef enum
{
    PN16_BTA3_Not_awake_1213 = 0U,
    PN16_BTA3_Awake_1214 = 1U
}PN16_BTA3_t;

typedef enum
{
    PN17_BTA3_Not_awake_1215 = 0U,
    PN17_BTA3_Awake_1216 = 1U
}PN17_BTA3_t;

typedef enum
{
    PN18_BTA3_Not_awake_1217 = 0U,
    PN18_BTA3_Awake_1218 = 1U
}PN18_BTA3_t;

typedef enum
{
    PN19_BTA3_Not_awake_1219 = 0U,
    PN19_BTA3_Awake_1220 = 1U
}PN19_BTA3_t;

typedef enum
{
    PN20_BTA3_Not_awake_1221 = 0U,
    PN20_BTA3_Awake_1222 = 1U
}PN20_BTA3_t;

typedef enum
{
    PN21_BTA3_Not_awake_1223 = 0U,
    PN21_BTA3_Awake_1224 = 1U
}PN21_BTA3_t;

typedef enum
{
    PN22_BTA3_Not_awake_1225 = 0U,
    PN22_BTA3_Awake_1226 = 1U
}PN22_BTA3_t;

typedef enum
{
    PN23_BTA3_Not_awake_1227 = 0U,
    PN23_BTA3_Awake_1228 = 1U
}PN23_BTA3_t;

typedef enum
{
    PN24_BTA3_Not_awake_1229 = 0U,
    PN24_BTA3_Awake_1230 = 1U
}PN24_BTA3_t;

typedef enum
{
    PN25_BTA3_Not_awake_1231 = 0U,
    PN25_BTA3_Awake_1232 = 1U
}PN25_BTA3_t;

typedef enum
{
    PN26_BTA3_Not_awake_1233 = 0U,
    PN26_BTA3_Awake_1234 = 1U
}PN26_BTA3_t;

typedef enum
{
    PN27_BTA3_Not_awake_1235 = 0U,
    PN27_BTA3_Awake_1236 = 1U
}PN27_BTA3_t;

typedef enum
{
    PN28_BTA3_Not_awake_1237 = 0U,
    PN28_BTA3_Awake_1238 = 1U
}PN28_BTA3_t;

typedef enum
{
    PN29_BTA3_Not_awake_1239 = 0U,
    PN29_BTA3_Awake_1240 = 1U
}PN29_BTA3_t;

typedef enum
{
    PN30_BTA3_Not_awake_1241 = 0U,
    PN30_BTA3_Awake_1242 = 1U
}PN30_BTA3_t;

typedef enum
{
    PN31_BTA3_Not_awake_1243 = 0U,
    PN31_BTA3_Awake_1244 = 1U
}PN31_BTA3_t;

typedef enum
{
    PN32_BTA3_Not_awake_1245 = 0U,
    PN32_BTA3_Awake_1246 = 1U
}PN32_BTA3_t;

typedef enum
{
    PN33_BTA3_Not_awake_1247 = 0U,
    PN33_BTA3_Awake_1248 = 1U
}PN33_BTA3_t;

typedef enum
{
    PN34_BTA3_Not_awake_1249 = 0U,
    PN34_BTA3_Awake_1250 = 1U
}PN34_BTA3_t;

typedef enum
{
    PN35_BTA3_Not_awake_1251 = 0U,
    PN35_BTA3_Awake_1252 = 1U
}PN35_BTA3_t;

typedef enum
{
    PN36_BTA3_Not_awake_1253 = 0U,
    PN36_BTA3_Awake_1254 = 1U
}PN36_BTA3_t;

typedef enum
{
    PN37_BTA3_Not_awake_1255 = 0U,
    PN37_BTA3_Awake_1256 = 1U
}PN37_BTA3_t;

typedef enum
{
    PN38_BTA3_Not_awake_1257 = 0U,
    PN38_BTA3_Awake_1258 = 1U
}PN38_BTA3_t;

typedef enum
{
    PN39_BTA3_Not_awake_1259 = 0U,
    PN39_BTA3_Awake_1260 = 1U
}PN39_BTA3_t;

typedef enum
{
    PN40_BTA3_Not_awake_1261 = 0U,
    PN40_BTA3_Awake_1262 = 1U
}PN40_BTA3_t;

typedef enum
{
    PN41_BTA3_Not_awake_1263 = 0U,
    PN41_BTA3_Awake_1264 = 1U
}PN41_BTA3_t;

typedef enum
{
    PN42_BTA3_Not_awake_1265 = 0U,
    PN42_BTA3_Awake_1266 = 1U
}PN42_BTA3_t;

typedef enum
{
    PN43_BTA3_Not_awake_1267 = 0U,
    PN43_BTA3_Awake_1268 = 1U
}PN43_BTA3_t;

typedef enum
{
    PN44_BTA3_Not_awake_1269 = 0U,
    PN44_BTA3_Awake_1270 = 1U
}PN44_BTA3_t;

typedef enum
{
    PN45_BTA3_Not_awake_1271 = 0U,
    PN45_BTA3_Awake_1272 = 1U
}PN45_BTA3_t;

typedef enum
{
    PN46_BTA3_Not_awake_1273 = 0U,
    PN46_BTA3_Awake_1274 = 1U
}PN46_BTA3_t;

typedef enum
{
    PN47_BTA3_Not_awake_1275 = 0U,
    PN47_BTA3_Awake_1276 = 1U
}PN47_BTA3_t;

typedef enum
{
    PN48_BTA3_Not_awake_1277 = 0U,
    PN48_BTA3_Awake_1278 = 1U
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
    RepMsgReq_BTA4_No_repeat_message_request_1279 = 0U,
    RepMsgReq_BTA4_Repeat_message_request_1280 = 1U
}RepMsgReq_BTA4_t;

typedef enum
{
    PN1_BTA4_Not_awake_1281 = 0U,
    PN1_BTA4_Awake_1282 = 1U
}PN1_BTA4_t;

typedef enum
{
    PN2_BTA4_Not_awake_1283 = 0U,
    PN2_BTA4_Awake_1284 = 1U
}PN2_BTA4_t;

typedef enum
{
    PN3_BTA4_Not_awake_1285 = 0U,
    PN3_BTA4_Awake_1286 = 1U
}PN3_BTA4_t;

typedef enum
{
    PN4_BTA4_Not_awake_1287 = 0U,
    PN4_BTA4_Awake_1288 = 1U
}PN4_BTA4_t;

typedef enum
{
    PN5_BTA4_Not_awake_1289 = 0U,
    PN5_BTA4_Awake_1290 = 1U
}PN5_BTA4_t;

typedef enum
{
    PN6_BTA4_Not_awake_1291 = 0U,
    PN6_BTA4_Awake_1292 = 1U
}PN6_BTA4_t;

typedef enum
{
    PN7_BTA4_Not_awake_1293 = 0U,
    PN7_BTA4_Awake_1294 = 1U
}PN7_BTA4_t;

typedef enum
{
    PN8_BTA4_Not_awake_1295 = 0U,
    PN8_BTA4_Awake_1296 = 1U
}PN8_BTA4_t;

typedef enum
{
    PN9_BTA4_Not_awake_1297 = 0U,
    PN9_BTA4_Awake_1298 = 1U
}PN9_BTA4_t;

typedef enum
{
    PN10_BTA4_Not_awake_1299 = 0U,
    PN10_BTA4_Awake_1300 = 1U
}PN10_BTA4_t;

typedef enum
{
    PN11_BTA4_Not_awake_1301 = 0U,
    PN11_BTA4_Awake_1302 = 1U
}PN11_BTA4_t;

typedef enum
{
    PN12_BTA4_Not_awake_1303 = 0U,
    PN12_BTA4_Awake_1304 = 1U
}PN12_BTA4_t;

typedef enum
{
    PN13_BTA4_Not_awake_1305 = 0U,
    PN13_BTA4_Awake_1306 = 1U
}PN13_BTA4_t;

typedef enum
{
    PN14_BTA4_Not_awake_1307 = 0U,
    PN14_BTA4_Awake_1308 = 1U
}PN14_BTA4_t;

typedef enum
{
    PN15_BTA4_Not_awake_1309 = 0U,
    PN15_BTA4_Awake_1310 = 1U
}PN15_BTA4_t;

typedef enum
{
    PN16_BTA4_Not_awake_1311 = 0U,
    PN16_BTA4_Awake_1312 = 1U
}PN16_BTA4_t;

typedef enum
{
    PN17_BTA4_Not_awake_1313 = 0U,
    PN17_BTA4_Awake_1314 = 1U
}PN17_BTA4_t;

typedef enum
{
    PN18_BTA4_Not_awake_1315 = 0U,
    PN18_BTA4_Awake_1316 = 1U
}PN18_BTA4_t;

typedef enum
{
    PN19_BTA4_Not_awake_1317 = 0U,
    PN19_BTA4_Awake_1318 = 1U
}PN19_BTA4_t;

typedef enum
{
    PN20_BTA4_Not_awake_1319 = 0U,
    PN20_BTA4_Awake_1320 = 1U
}PN20_BTA4_t;

typedef enum
{
    PN21_BTA4_Not_awake_1321 = 0U,
    PN21_BTA4_Awake_1322 = 1U
}PN21_BTA4_t;

typedef enum
{
    PN22_BTA4_Not_awake_1323 = 0U,
    PN22_BTA4_Awake_1324 = 1U
}PN22_BTA4_t;

typedef enum
{
    PN23_BTA4_Not_awake_1325 = 0U,
    PN23_BTA4_Awake_1326 = 1U
}PN23_BTA4_t;

typedef enum
{
    PN24_BTA4_Not_awake_1327 = 0U,
    PN24_BTA4_Awake_1328 = 1U
}PN24_BTA4_t;

typedef enum
{
    PN25_BTA4_Not_awake_1329 = 0U,
    PN25_BTA4_Awake_1330 = 1U
}PN25_BTA4_t;

typedef enum
{
    PN26_BTA4_Not_awake_1331 = 0U,
    PN26_BTA4_Awake_1332 = 1U
}PN26_BTA4_t;

typedef enum
{
    PN27_BTA4_Not_awake_1333 = 0U,
    PN27_BTA4_Awake_1334 = 1U
}PN27_BTA4_t;

typedef enum
{
    PN28_BTA4_Not_awake_1335 = 0U,
    PN28_BTA4_Awake_1336 = 1U
}PN28_BTA4_t;

typedef enum
{
    PN29_BTA4_Not_awake_1337 = 0U,
    PN29_BTA4_Awake_1338 = 1U
}PN29_BTA4_t;

typedef enum
{
    PN30_BTA4_Not_awake_1339 = 0U,
    PN30_BTA4_Awake_1340 = 1U
}PN30_BTA4_t;

typedef enum
{
    PN31_BTA4_Not_awake_1341 = 0U,
    PN31_BTA4_Awake_1342 = 1U
}PN31_BTA4_t;

typedef enum
{
    PN32_BTA4_Not_awake_1343 = 0U,
    PN32_BTA4_Awake_1344 = 1U
}PN32_BTA4_t;

typedef enum
{
    PN33_BTA4_Not_awake_1345 = 0U,
    PN33_BTA4_Awake_1346 = 1U
}PN33_BTA4_t;

typedef enum
{
    PN34_BTA4_Not_awake_1347 = 0U,
    PN34_BTA4_Awake_1348 = 1U
}PN34_BTA4_t;

typedef enum
{
    PN35_BTA4_Not_awake_1349 = 0U,
    PN35_BTA4_Awake_1350 = 1U
}PN35_BTA4_t;

typedef enum
{
    PN36_BTA4_Not_awake_1351 = 0U,
    PN36_BTA4_Awake_1352 = 1U
}PN36_BTA4_t;

typedef enum
{
    PN37_BTA4_Not_awake_1353 = 0U,
    PN37_BTA4_Awake_1354 = 1U
}PN37_BTA4_t;

typedef enum
{
    PN38_BTA4_Not_awake_1355 = 0U,
    PN38_BTA4_Awake_1356 = 1U
}PN38_BTA4_t;

typedef enum
{
    PN39_BTA4_Not_awake_1357 = 0U,
    PN39_BTA4_Awake_1358 = 1U
}PN39_BTA4_t;

typedef enum
{
    PN40_BTA4_Not_awake_1359 = 0U,
    PN40_BTA4_Awake_1360 = 1U
}PN40_BTA4_t;

typedef enum
{
    PN41_BTA4_Not_awake_1361 = 0U,
    PN41_BTA4_Awake_1362 = 1U
}PN41_BTA4_t;

typedef enum
{
    PN42_BTA4_Not_awake_1363 = 0U,
    PN42_BTA4_Awake_1364 = 1U
}PN42_BTA4_t;

typedef enum
{
    PN43_BTA4_Not_awake_1365 = 0U,
    PN43_BTA4_Awake_1366 = 1U
}PN43_BTA4_t;

typedef enum
{
    PN44_BTA4_Not_awake_1367 = 0U,
    PN44_BTA4_Awake_1368 = 1U
}PN44_BTA4_t;

typedef enum
{
    PN45_BTA4_Not_awake_1369 = 0U,
    PN45_BTA4_Awake_1370 = 1U
}PN45_BTA4_t;

typedef enum
{
    PN46_BTA4_Not_awake_1371 = 0U,
    PN46_BTA4_Awake_1372 = 1U
}PN46_BTA4_t;

typedef enum
{
    PN47_BTA4_Not_awake_1373 = 0U,
    PN47_BTA4_Awake_1374 = 1U
}PN47_BTA4_t;

typedef enum
{
    PN48_BTA4_Not_awake_1375 = 0U,
    PN48_BTA4_Awake_1376 = 1U
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
    RepMsgReq_BTA5_No_repeat_message_request_1377 = 0U,
    RepMsgReq_BTA5_Repeat_message_request_1378 = 1U
}RepMsgReq_BTA5_t;

typedef enum
{
    PN1_BTA5_Not_awake_1379 = 0U,
    PN1_BTA5_Awake_1380 = 1U
}PN1_BTA5_t;

typedef enum
{
    PN2_BTA5_Not_awake_1381 = 0U,
    PN2_BTA5_Awake_1382 = 1U
}PN2_BTA5_t;

typedef enum
{
    PN3_BTA5_Not_awake_1383 = 0U,
    PN3_BTA5_Awake_1384 = 1U
}PN3_BTA5_t;

typedef enum
{
    PN4_BTA5_Not_awake_1385 = 0U,
    PN4_BTA5_Awake_1386 = 1U
}PN4_BTA5_t;

typedef enum
{
    PN5_BTA5_Not_awake_1387 = 0U,
    PN5_BTA5_Awake_1388 = 1U
}PN5_BTA5_t;

typedef enum
{
    PN6_BTA5_Not_awake_1389 = 0U,
    PN6_BTA5_Awake_1390 = 1U
}PN6_BTA5_t;

typedef enum
{
    PN7_BTA5_Not_awake_1391 = 0U,
    PN7_BTA5_Awake_1392 = 1U
}PN7_BTA5_t;

typedef enum
{
    PN8_BTA5_Not_awake_1393 = 0U,
    PN8_BTA5_Awake_1394 = 1U
}PN8_BTA5_t;

typedef enum
{
    PN9_BTA5_Not_awake_1395 = 0U,
    PN9_BTA5_Awake_1396 = 1U
}PN9_BTA5_t;

typedef enum
{
    PN10_BTA5_Not_awake_1397 = 0U,
    PN10_BTA5_Awake_1398 = 1U
}PN10_BTA5_t;

typedef enum
{
    PN11_BTA5_Not_awake_1399 = 0U,
    PN11_BTA5_Awake_1400 = 1U
}PN11_BTA5_t;

typedef enum
{
    PN12_BTA5_Not_awake_1401 = 0U,
    PN12_BTA5_Awake_1402 = 1U
}PN12_BTA5_t;

typedef enum
{
    PN13_BTA5_Not_awake_1403 = 0U,
    PN13_BTA5_Awake_1404 = 1U
}PN13_BTA5_t;

typedef enum
{
    PN14_BTA5_Not_awake_1405 = 0U,
    PN14_BTA5_Awake_1406 = 1U
}PN14_BTA5_t;

typedef enum
{
    PN15_BTA5_Not_awake_1407 = 0U,
    PN15_BTA5_Awake_1408 = 1U
}PN15_BTA5_t;

typedef enum
{
    PN16_BTA5_Not_awake_1409 = 0U,
    PN16_BTA5_Awake_1410 = 1U
}PN16_BTA5_t;

typedef enum
{
    PN17_BTA5_Not_awake_1411 = 0U,
    PN17_BTA5_Awake_1412 = 1U
}PN17_BTA5_t;

typedef enum
{
    PN18_BTA5_Not_awake_1413 = 0U,
    PN18_BTA5_Awake_1414 = 1U
}PN18_BTA5_t;

typedef enum
{
    PN19_BTA5_Not_awake_1415 = 0U,
    PN19_BTA5_Awake_1416 = 1U
}PN19_BTA5_t;

typedef enum
{
    PN20_BTA5_Not_awake_1417 = 0U,
    PN20_BTA5_Awake_1418 = 1U
}PN20_BTA5_t;

typedef enum
{
    PN21_BTA5_Not_awake_1419 = 0U,
    PN21_BTA5_Awake_1420 = 1U
}PN21_BTA5_t;

typedef enum
{
    PN22_BTA5_Not_awake_1421 = 0U,
    PN22_BTA5_Awake_1422 = 1U
}PN22_BTA5_t;

typedef enum
{
    PN23_BTA5_Not_awake_1423 = 0U,
    PN23_BTA5_Awake_1424 = 1U
}PN23_BTA5_t;

typedef enum
{
    PN24_BTA5_Not_awake_1425 = 0U,
    PN24_BTA5_Awake_1426 = 1U
}PN24_BTA5_t;

typedef enum
{
    PN25_BTA5_Not_awake_1427 = 0U,
    PN25_BTA5_Awake_1428 = 1U
}PN25_BTA5_t;

typedef enum
{
    PN26_BTA5_Not_awake_1429 = 0U,
    PN26_BTA5_Awake_1430 = 1U
}PN26_BTA5_t;

typedef enum
{
    PN27_BTA5_Not_awake_1431 = 0U,
    PN27_BTA5_Awake_1432 = 1U
}PN27_BTA5_t;

typedef enum
{
    PN28_BTA5_Not_awake_1433 = 0U,
    PN28_BTA5_Awake_1434 = 1U
}PN28_BTA5_t;

typedef enum
{
    PN29_BTA5_Not_awake_1435 = 0U,
    PN29_BTA5_Awake_1436 = 1U
}PN29_BTA5_t;

typedef enum
{
    PN30_BTA5_Not_awake_1437 = 0U,
    PN30_BTA5_Awake_1438 = 1U
}PN30_BTA5_t;

typedef enum
{
    PN31_BTA5_Not_awake_1439 = 0U,
    PN31_BTA5_Awake_1440 = 1U
}PN31_BTA5_t;

typedef enum
{
    PN32_BTA5_Not_awake_1441 = 0U,
    PN32_BTA5_Awake_1442 = 1U
}PN32_BTA5_t;

typedef enum
{
    PN33_BTA5_Not_awake_1443 = 0U,
    PN33_BTA5_Awake_1444 = 1U
}PN33_BTA5_t;

typedef enum
{
    PN34_BTA5_Not_awake_1445 = 0U,
    PN34_BTA5_Awake_1446 = 1U
}PN34_BTA5_t;

typedef enum
{
    PN35_BTA5_Not_awake_1447 = 0U,
    PN35_BTA5_Awake_1448 = 1U
}PN35_BTA5_t;

typedef enum
{
    PN36_BTA5_Not_awake_1449 = 0U,
    PN36_BTA5_Awake_1450 = 1U
}PN36_BTA5_t;

typedef enum
{
    PN37_BTA5_Not_awake_1451 = 0U,
    PN37_BTA5_Awake_1452 = 1U
}PN37_BTA5_t;

typedef enum
{
    PN38_BTA5_Not_awake_1453 = 0U,
    PN38_BTA5_Awake_1454 = 1U
}PN38_BTA5_t;

typedef enum
{
    PN39_BTA5_Not_awake_1455 = 0U,
    PN39_BTA5_Awake_1456 = 1U
}PN39_BTA5_t;

typedef enum
{
    PN40_BTA5_Not_awake_1457 = 0U,
    PN40_BTA5_Awake_1458 = 1U
}PN40_BTA5_t;

typedef enum
{
    PN41_BTA5_Not_awake_1459 = 0U,
    PN41_BTA5_Awake_1460 = 1U
}PN41_BTA5_t;

typedef enum
{
    PN42_BTA5_Not_awake_1461 = 0U,
    PN42_BTA5_Awake_1462 = 1U
}PN42_BTA5_t;

typedef enum
{
    PN43_BTA5_Not_awake_1463 = 0U,
    PN43_BTA5_Awake_1464 = 1U
}PN43_BTA5_t;

typedef enum
{
    PN44_BTA5_Not_awake_1465 = 0U,
    PN44_BTA5_Awake_1466 = 1U
}PN44_BTA5_t;

typedef enum
{
    PN45_BTA5_Not_awake_1467 = 0U,
    PN45_BTA5_Awake_1468 = 1U
}PN45_BTA5_t;

typedef enum
{
    PN46_BTA5_Not_awake_1469 = 0U,
    PN46_BTA5_Awake_1470 = 1U
}PN46_BTA5_t;

typedef enum
{
    PN47_BTA5_Not_awake_1471 = 0U,
    PN47_BTA5_Awake_1472 = 1U
}PN47_BTA5_t;

typedef enum
{
    PN48_BTA5_Not_awake_1473 = 0U,
    PN48_BTA5_Awake_1474 = 1U
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

// def @NM_BTA_RB2 CAN Message (1397 0x575)
#define NM_BTA_RB2_IDE (0U)
#define NM_BTA_RB2_DLC (8U)
#define NM_BTA_RB2_CANID (0x575U)

typedef enum
{
    No_repeat_message_request_1475 = 0U,
    Repeat_message_request_1476 = 1U
}RepMsgReq_BTA_RB2_t;

typedef enum
{
    PN1_BTA_RB2_Not_awake_1477 = 0U,
    PN1_BTA_RB2_Awake_1478 = 1U
}PN1_BTA_RB2_t;

typedef enum
{
    PN2_BTA_RB2_Not_awake_1479 = 0U,
    PN2_BTA_RB2_Awake_1480 = 1U
}PN2_BTA_RB2_t;

typedef enum
{
    PN3_BTA_RB2_Not_awake_1481 = 0U,
    PN3_BTA_RB2_Awake_1482 = 1U
}PN3_BTA_RB2_t;

typedef enum
{
    PN4_BTA_RB2_Not_awake_1483 = 0U,
    PN4_BTA_RB2_Awake_1484 = 1U
}PN4_BTA_RB2_t;

typedef enum
{
    PN5_BTA_RB2_Not_awake_1485 = 0U,
    PN5_BTA_RB2_Awake_1486 = 1U
}PN5_BTA_RB2_t;

typedef enum
{
    PN6_BTA_RB2_Not_awake_1487 = 0U,
    PN6_BTA_RB2_Awake_1488 = 1U
}PN6_BTA_RB2_t;

typedef enum
{
    PN7_BTA_RB2_Not_awake_1489 = 0U,
    PN7_BTA_RB2_Awake_1490 = 1U
}PN7_BTA_RB2_t;

typedef enum
{
    PN8_BTA_RB2_Not_awake_1491 = 0U,
    PN8_BTA_RB2_Awake_1492 = 1U
}PN8_BTA_RB2_t;

typedef enum
{
    PN9_BTA_RB2_Not_awake_1493 = 0U,
    PN9_BTA_RB2_Awake_1494 = 1U
}PN9_BTA_RB2_t;

typedef enum
{
    PN10_BTA_RB2_Not_awake_1495 = 0U,
    PN10_BTA_RB2_Awake_1496 = 1U
}PN10_BTA_RB2_t;

typedef enum
{
    PN11_BTA_RB2_Not_awake_1497 = 0U,
    PN11_BTA_RB2_Awake_1498 = 1U
}PN11_BTA_RB2_t;

typedef enum
{
    PN12_BTA_RB2_Not_awake_1499 = 0U,
    PN12_BTA_RB2_Awake_1500 = 1U
}PN12_BTA_RB2_t;

typedef enum
{
    PN13_BTA_RB2_Not_awake_1501 = 0U,
    PN13_BTA_RB2_Awake_1502 = 1U
}PN13_BTA_RB2_t;

typedef enum
{
    PN14_BTA_RB2_Not_awake_1503 = 0U,
    PN14_BTA_RB2_Awake_1504 = 1U
}PN14_BTA_RB2_t;

typedef enum
{
    PN15_BTA_RB2_Not_awake_1505 = 0U,
    PN15_BTA_RB2_Awake_1506 = 1U
}PN15_BTA_RB2_t;

typedef enum
{
    PN16_BTA_RB2_Not_awake_1507 = 0U,
    PN16_BTA_RB2_Awake_1508 = 1U
}PN16_BTA_RB2_t;

typedef enum
{
    PN17_BTA_RB2_Not_awake_1509 = 0U,
    PN17_BTA_RB2_Awake_1510 = 1U
}PN17_BTA_RB2_t;

typedef enum
{
    PN18_BTA_RB2_Not_awake_1511 = 0U,
    PN18_BTA_RB2_Awake_1512 = 1U
}PN18_BTA_RB2_t;

typedef enum
{
    PN19_BTA_RB2_Not_awake_1513 = 0U,
    PN19_BTA_RB2_Awake_1514 = 1U
}PN19_BTA_RB2_t;

typedef enum
{
    PN20_BTA_RB2_Not_awake_1515 = 0U,
    PN20_BTA_RB2_Awake_1516 = 1U
}PN20_BTA_RB2_t;

typedef enum
{
    PN21_BTA_RB2_Not_awake_1517 = 0U,
    PN21_BTA_RB2_Awake_1518 = 1U
}PN21_BTA_RB2_t;

typedef enum
{
    PN22_BTA_RB2_Not_awake_1519 = 0U,
    PN22_BTA_RB2_Awake_1520 = 1U
}PN22_BTA_RB2_t;

typedef enum
{
    PN23_BTA_RB2_Not_awake_1521 = 0U,
    PN23_BTA_RB2_Awake_1522 = 1U
}PN23_BTA_RB2_t;

typedef enum
{
    PN24_BTA_RB2_Not_awake_1523 = 0U,
    PN24_BTA_RB2_Awake_1524 = 1U
}PN24_BTA_RB2_t;

typedef enum
{
    PN25_BTA_RB2_Not_awake_1525 = 0U,
    PN25_BTA_RB2_Awake_1526 = 1U
}PN25_BTA_RB2_t;

typedef enum
{
    PN26_BTA_RB2_Not_awake_1527 = 0U,
    PN26_BTA_RB2_Awake_1528 = 1U
}PN26_BTA_RB2_t;

typedef enum
{
    PN27_BTA_RB2_Not_awake_1529 = 0U,
    PN27_BTA_RB2_Awake_1530 = 1U
}PN27_BTA_RB2_t;

typedef enum
{
    PN28_BTA_RB2_Not_awake_1531 = 0U,
    PN28_BTA_RB2_Awake_1532 = 1U
}PN28_BTA_RB2_t;

typedef enum
{
    PN29_BTA_RB2_Not_awake_1533 = 0U,
    PN29_BTA_RB2_Awake_1534 = 1U
}PN29_BTA_RB2_t;

typedef enum
{
    PN30_BTA_RB2_Not_awake_1535 = 0U,
    PN30_BTA_RB2_Awake_1536 = 1U
}PN30_BTA_RB2_t;

typedef enum
{
    PN31_BTA_RB2_Not_awake_1537 = 0U,
    PN31_BTA_RB2_Awake_1538 = 1U
}PN31_BTA_RB2_t;

typedef enum
{
    PN32_BTA_RB2_Not_awake_1539 = 0U,
    PN32_BTA_RB2_Awake_1540 = 1U
}PN32_BTA_RB2_t;

typedef enum
{
    PN33_BTA_RB2_Not_awake_1541 = 0U,
    PN33_BTA_RB2_Awake_1542 = 1U
}PN33_BTA_RB2_t;

typedef enum
{
    PN34_BTA_RB2_Not_awake_1543 = 0U,
    PN34_BTA_RB2_Awake_1544 = 1U
}PN34_BTA_RB2_t;

typedef enum
{
    PN35_BTA_RB2_Not_awake_1545 = 0U,
    PN35_BTA_RB2_Awake_1546 = 1U
}PN35_BTA_RB2_t;

typedef enum
{
    PN36_BTA_RB2_Not_awake_1547 = 0U,
    PN36_BTA_RB2_Awake_1548 = 1U
}PN36_BTA_RB2_t;

typedef enum
{
    PN37_BTA_RB2_Not_awake_1549 = 0U,
    PN37_BTA_RB2_Awake_1550 = 1U
}PN37_BTA_RB2_t;

typedef enum
{
    PN38_BTA_RB2_Not_awake_1551 = 0U,
    PN38_BTA_RB2_Awake_1552 = 1U
}PN38_BTA_RB2_t;

typedef enum
{
    PN39_BTA_RB2_Not_awake_1553 = 0U,
    PN39_BTA_RB2_Awake_1554 = 1U
}PN39_BTA_RB2_t;

typedef enum
{
    PN40_BTA_RB2_Not_awake_1555 = 0U,
    PN40_BTA_RB2_Awake_1556 = 1U
}PN40_BTA_RB2_t;

typedef enum
{
    PN41_BTA_RB2_Not_awake_1557 = 0U,
    PN41_BTA_RB2_Awake_1558 = 1U
}PN41_BTA_RB2_t;

typedef enum
{
    PN42_BTA_RB2_Not_awake_1559 = 0U,
    PN42_BTA_RB2_Awake_1560 = 1U
}PN42_BTA_RB2_t;

typedef enum
{
    PN43_BTA_RB2_Not_awake_1561 = 0U,
    PN43_BTA_RB2_Awake_1562 = 1U
}PN43_BTA_RB2_t;

typedef enum
{
    PN44_BTA_RB2_Not_awake_1563 = 0U,
    PN44_BTA_RB2_Awake_1564 = 1U
}PN44_BTA_RB2_t;

typedef enum
{
    PN45_BTA_RB2_Not_awake_1565 = 0U,
    PN45_BTA_RB2_Awake_1566 = 1U
}PN45_BTA_RB2_t;

typedef enum
{
    PN46_BTA_RB2_Not_awake_1567 = 0U,
    PN46_BTA_RB2_Awake_1568 = 1U
}PN46_BTA_RB2_t;

typedef enum
{
    PN47_BTA_RB2_Not_awake_1569 = 0U,
    PN47_BTA_RB2_Awake_1570 = 1U
}PN47_BTA_RB2_t;

typedef enum
{
    PN48_BTA_RB2_Not_awake_1571 = 0U,
    PN48_BTA_RB2_Awake_1572 = 1U
}PN48_BTA_RB2_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    // Source node identifier DCM_FL
    uint8_t ScrNodId_BTA_RB2 : 8U;
    // Control bit vector DCM_FL Repeat Message Request
    //  0 : "No repeat message request"
    //  1 : "Repeat message request"
    RepMsgReq_BTA_RB2_t RepMsgReq_BTA_RB2 : 1U;
    // Control bit vector Network management coordination - Reserved
    uint8_t NetMngtCoorn_BTA_RB2 : 2U;
    // Control bit vector Network management coordinator sleep ready - Reserved
    uint8_t NetMngtCoorrSleepRdy_BTA_RB2 : 1U;
    // Control bit vector Active wakeup - Reserved
    uint8_t ActvWakeup_BTA_RB2 : 1U;
    // Control bit vector DCM_FL Reserved
    uint8_t ResdBit1_BTA_RB2 : 1U;
    // Control bit vector DCM_FL Partial network information - Reserved
    uint8_t PtlNetInfo_BTA_RB2 : 1U;
    // Control bit vector DCM_FL Reserved
    uint8_t ResdBit2_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN1_BTA_RB2_t PN1_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN2_BTA_RB2_t PN2_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN3_BTA_RB2_t PN3_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN4_BTA_RB2_t PN4_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN5_BTA_RB2_t PN5_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN6_BTA_RB2_t PN6_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN7_BTA_RB2_t PN7_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN8_BTA_RB2_t PN8_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN9_BTA_RB2_t PN9_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN10_BTA_RB2_t PN10_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN11_BTA_RB2_t PN11_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN12_BTA_RB2_t PN12_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN13_BTA_RB2_t PN13_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN14_BTA_RB2_t PN14_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN15_BTA_RB2_t PN15_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN16_BTA_RB2_t PN16_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN17_BTA_RB2_t PN17_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN18_BTA_RB2_t PN18_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN19_BTA_RB2_t PN19_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN20_BTA_RB2_t PN20_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN21_BTA_RB2_t PN21_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN22_BTA_RB2_t PN22_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN23_BTA_RB2_t PN23_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN24_BTA_RB2_t PN24_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN25_BTA_RB2_t PN25_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN26_BTA_RB2_t PN26_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN27_BTA_RB2_t PN27_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN28_BTA_RB2_t PN28_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN29_BTA_RB2_t PN29_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN30_BTA_RB2_t PN30_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN31_BTA_RB2_t PN31_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN32_BTA_RB2_t PN32_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN33_BTA_RB2_t PN33_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN34_BTA_RB2_t PN34_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN35_BTA_RB2_t PN35_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN36_BTA_RB2_t PN36_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN37_BTA_RB2_t PN37_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN38_BTA_RB2_t PN38_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN39_BTA_RB2_t PN39_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN40_BTA_RB2_t PN40_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN41_BTA_RB2_t PN41_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN42_BTA_RB2_t PN42_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN43_BTA_RB2_t PN43_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN44_BTA_RB2_t PN44_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN45_BTA_RB2_t PN45_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN46_BTA_RB2_t PN46_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN47_BTA_RB2_t PN47_BTA_RB2 : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN48_BTA_RB2_t PN48_BTA_RB2 : 1U;
  }sig;
} NM_BTA_RB2_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  NM_BTA_RB2_un msg;
}NM_BTA_RB2_msg;

// def @NM_BTA_FB CAN Message (1404 0x57c)
#define NM_BTA_FB_IDE (0U)
#define NM_BTA_FB_DLC (8U)
#define NM_BTA_FB_CANID (0x57cU)

typedef enum
{
    No_repeat_message_request_1573 = 0U,
    Repeat_message_request_1574 = 1U
}RepMsgReq_BTA_FB_t;

typedef enum
{
    PN1_BTA_FB_Not_awake_1575 = 0U,
    PN1_BTA_FB_Awake_1576 = 1U
}PN1_BTA_FB_t;

typedef enum
{
    PN2_BTA_FB_Not_awake_1577 = 0U,
    PN2_BTA_FB_Awake_1578 = 1U
}PN2_BTA_FB_t;

typedef enum
{
    PN3_BTA_FB_Not_awake_1579 = 0U,
    PN3_BTA_FB_Awake_1580 = 1U
}PN3_BTA_FB_t;

typedef enum
{
    PN4_BTA_FB_Not_awake_1581 = 0U,
    PN4_BTA_FB_Awake_1582 = 1U
}PN4_BTA_FB_t;

typedef enum
{
    PN5_BTA_FB_Not_awake_1583 = 0U,
    PN5_BTA_FB_Awake_1584 = 1U
}PN5_BTA_FB_t;

typedef enum
{
    PN6_BTA_FB_Not_awake_1585 = 0U,
    PN6_BTA_FB_Awake_1586 = 1U
}PN6_BTA_FB_t;

typedef enum
{
    PN7_BTA_FB_Not_awake_1587 = 0U,
    PN7_BTA_FB_Awake_1588 = 1U
}PN7_BTA_FB_t;

typedef enum
{
    PN8_BTA_FB_Not_awake_1589 = 0U,
    PN8_BTA_FB_Awake_1590 = 1U
}PN8_BTA_FB_t;

typedef enum
{
    PN9_BTA_FB_Not_awake_1591 = 0U,
    PN9_BTA_FB_Awake_1592 = 1U
}PN9_BTA_FB_t;

typedef enum
{
    PN10_BTA_FB_Not_awake_1593 = 0U,
    PN10_BTA_FB_Awake_1594 = 1U
}PN10_BTA_FB_t;

typedef enum
{
    PN11_BTA_FB_Not_awake_1595 = 0U,
    PN11_BTA_FB_Awake_1596 = 1U
}PN11_BTA_FB_t;

typedef enum
{
    PN12_BTA_FB_Not_awake_1597 = 0U,
    PN12_BTA_FB_Awake_1598 = 1U
}PN12_BTA_FB_t;

typedef enum
{
    PN13_BTA_FB_Not_awake_1599 = 0U,
    PN13_BTA_FB_Awake_1600 = 1U
}PN13_BTA_FB_t;

typedef enum
{
    PN14_BTA_FB_Not_awake_1601 = 0U,
    PN14_BTA_FB_Awake_1602 = 1U
}PN14_BTA_FB_t;

typedef enum
{
    PN15_BTA_FB_Not_awake_1603 = 0U,
    PN15_BTA_FB_Awake_1604 = 1U
}PN15_BTA_FB_t;

typedef enum
{
    PN16_BTA_FB_Not_awake_1605 = 0U,
    PN16_BTA_FB_Awake_1606 = 1U
}PN16_BTA_FB_t;

typedef enum
{
    PN17_BTA_FB_Not_awake_1607 = 0U,
    PN17_BTA_FB_Awake_1608 = 1U
}PN17_BTA_FB_t;

typedef enum
{
    PN18_BTA_FB_Not_awake_1609 = 0U,
    PN18_BTA_FB_Awake_1610 = 1U
}PN18_BTA_FB_t;

typedef enum
{
    PN19_BTA_FB_Not_awake_1611 = 0U,
    PN19_BTA_FB_Awake_1612 = 1U
}PN19_BTA_FB_t;

typedef enum
{
    PN20_BTA_FB_Not_awake_1613 = 0U,
    PN20_BTA_FB_Awake_1614 = 1U
}PN20_BTA_FB_t;

typedef enum
{
    PN21_BTA_FB_Not_awake_1615 = 0U,
    PN21_BTA_FB_Awake_1616 = 1U
}PN21_BTA_FB_t;

typedef enum
{
    PN22_BTA_FB_Not_awake_1617 = 0U,
    PN22_BTA_FB_Awake_1618 = 1U
}PN22_BTA_FB_t;

typedef enum
{
    PN23_BTA_FB_Not_awake_1619 = 0U,
    PN23_BTA_FB_Awake_1620 = 1U
}PN23_BTA_FB_t;

typedef enum
{
    PN24_BTA_FB_Not_awake_1621 = 0U,
    PN24_BTA_FB_Awake_1622 = 1U
}PN24_BTA_FB_t;

typedef enum
{
    PN25_BTA_FB_Not_awake_1623 = 0U,
    PN25_BTA_FB_Awake_1624 = 1U
}PN25_BTA_FB_t;

typedef enum
{
    PN26_BTA_FB_Not_awake_1625 = 0U,
    PN26_BTA_FB_Awake_1626 = 1U
}PN26_BTA_FB_t;

typedef enum
{
    PN27_BTA_FB_Not_awake_1627 = 0U,
    PN27_BTA_FB_Awake_1628 = 1U
}PN27_BTA_FB_t;

typedef enum
{
    PN28_BTA_FB_Not_awake_1629 = 0U,
    PN28_BTA_FB_Awake_1630 = 1U
}PN28_BTA_FB_t;

typedef enum
{
    PN29_BTA_FB_Not_awake_1631 = 0U,
    PN29_BTA_FB_Awake_1632 = 1U
}PN29_BTA_FB_t;

typedef enum
{
    PN30_BTA_FB_Not_awake_1633 = 0U,
    PN30_BTA_FB_Awake_1634 = 1U
}PN30_BTA_FB_t;

typedef enum
{
    PN31_BTA_FB_Not_awake_1635 = 0U,
    PN31_BTA_FB_Awake_1636 = 1U
}PN31_BTA_FB_t;

typedef enum
{
    PN32_BTA_FB_Not_awake_1637 = 0U,
    PN32_BTA_FB_Awake_1638 = 1U
}PN32_BTA_FB_t;

typedef enum
{
    PN33_BTA_FB_Not_awake_1639 = 0U,
    PN33_BTA_FB_Awake_1640 = 1U
}PN33_BTA_FB_t;

typedef enum
{
    PN34_BTA_FB_Not_awake_1641 = 0U,
    PN34_BTA_FB_Awake_1642 = 1U
}PN34_BTA_FB_t;

typedef enum
{
    PN35_BTA_FB_Not_awake_1643 = 0U,
    PN35_BTA_FB_Awake_1644 = 1U
}PN35_BTA_FB_t;

typedef enum
{
    PN36_BTA_FB_Not_awake_1645 = 0U,
    PN36_BTA_FB_Awake_1646 = 1U
}PN36_BTA_FB_t;

typedef enum
{
    PN37_BTA_FB_Not_awake_1647 = 0U,
    PN37_BTA_FB_Awake_1648 = 1U
}PN37_BTA_FB_t;

typedef enum
{
    PN38_BTA_FB_Not_awake_1649 = 0U,
    PN38_BTA_FB_Awake_1650 = 1U
}PN38_BTA_FB_t;

typedef enum
{
    PN39_BTA_FB_Not_awake_1651 = 0U,
    PN39_BTA_FB_Awake_1652 = 1U
}PN39_BTA_FB_t;

typedef enum
{
    PN40_BTA_FB_Not_awake_1653 = 0U,
    PN40_BTA_FB_Awake_1654 = 1U
}PN40_BTA_FB_t;

typedef enum
{
    PN41_BTA_FB_Not_awake_1655 = 0U,
    PN41_BTA_FB_Awake_1656 = 1U
}PN41_BTA_FB_t;

typedef enum
{
    PN42_BTA_FB_Not_awake_1657 = 0U,
    PN42_BTA_FB_Awake_1658 = 1U
}PN42_BTA_FB_t;

typedef enum
{
    PN43_BTA_FB_Not_awake_1659 = 0U,
    PN43_BTA_FB_Awake_1660 = 1U
}PN43_BTA_FB_t;

typedef enum
{
    PN44_BTA_FB_Not_awake_1661 = 0U,
    PN44_BTA_FB_Awake_1662 = 1U
}PN44_BTA_FB_t;

typedef enum
{
    PN45_BTA_FB_Not_awake_1663 = 0U,
    PN45_BTA_FB_Awake_1664 = 1U
}PN45_BTA_FB_t;

typedef enum
{
    PN46_BTA_FB_Not_awake_1665 = 0U,
    PN46_BTA_FB_Awake_1666 = 1U
}PN46_BTA_FB_t;

typedef enum
{
    PN47_BTA_FB_Not_awake_1667 = 0U,
    PN47_BTA_FB_Awake_1668 = 1U
}PN47_BTA_FB_t;

typedef enum
{
    PN48_BTA_FB_Not_awake_1669 = 0U,
    PN48_BTA_FB_Awake_1670 = 1U
}PN48_BTA_FB_t;

typedef union
{
  uint8_t data[8u];
  struct
  {
    // Source node identifier DCM_FL
    uint8_t ScrNodId_BTA_FB : 8U;
    // Control bit vector DCM_FL Repeat Message Request
    //  0 : "No repeat message request"
    //  1 : "Repeat message request"
    RepMsgReq_BTA_FB_t RepMsgReq_BTA_FB : 1U;
    // Control bit vector Network management coordination - Reserved
    uint8_t NetMngtCoorn_BTA_FB : 2U;
    // Control bit vector Network management coordinator sleep ready - Reserved
    uint8_t NetMngtCoorrSleepRdy_BTA_FB : 1U;
    // Control bit vector Active wakeup - Reserved
    uint8_t ActvWakeup_BTA_FB : 1U;
    // Control bit vector DCM_FL Reserved
    uint8_t ResdBit1_BTA_FB : 1U;
    // Control bit vector DCM_FL Partial network information - Reserved
    uint8_t PtlNetInfo_BTA_FB : 1U;
    // Control bit vector DCM_FL Reserved
    uint8_t ResdBit2_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN1_BTA_FB_t PN1_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN2_BTA_FB_t PN2_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN3_BTA_FB_t PN3_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN4_BTA_FB_t PN4_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN5_BTA_FB_t PN5_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN6_BTA_FB_t PN6_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN7_BTA_FB_t PN7_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN8_BTA_FB_t PN8_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN9_BTA_FB_t PN9_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN10_BTA_FB_t PN10_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN11_BTA_FB_t PN11_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN12_BTA_FB_t PN12_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN13_BTA_FB_t PN13_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN14_BTA_FB_t PN14_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN15_BTA_FB_t PN15_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN16_BTA_FB_t PN16_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN17_BTA_FB_t PN17_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN18_BTA_FB_t PN18_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN19_BTA_FB_t PN19_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN20_BTA_FB_t PN20_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN21_BTA_FB_t PN21_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN22_BTA_FB_t PN22_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN23_BTA_FB_t PN23_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN24_BTA_FB_t PN24_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN25_BTA_FB_t PN25_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN26_BTA_FB_t PN26_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN27_BTA_FB_t PN27_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN28_BTA_FB_t PN28_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN29_BTA_FB_t PN29_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN30_BTA_FB_t PN30_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN31_BTA_FB_t PN31_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN32_BTA_FB_t PN32_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN33_BTA_FB_t PN33_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN34_BTA_FB_t PN34_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN35_BTA_FB_t PN35_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN36_BTA_FB_t PN36_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN37_BTA_FB_t PN37_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN38_BTA_FB_t PN38_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN39_BTA_FB_t PN39_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN40_BTA_FB_t PN40_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN41_BTA_FB_t PN41_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN42_BTA_FB_t PN42_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN43_BTA_FB_t PN43_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN44_BTA_FB_t PN44_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN45_BTA_FB_t PN45_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN46_BTA_FB_t PN46_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN47_BTA_FB_t PN47_BTA_FB : 1U;
    //  0 : "Not awake"
    //  1 : "Awake"
    PN48_BTA_FB_t PN48_BTA_FB : 1U;
  }sig;
} NM_BTA_FB_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  NM_BTA_FB_un msg;
}NM_BTA_FB_msg;

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

// def @DIAG_REQ_BTA1 CAN Message (1648 0x670)
#define DIAG_REQ_BTA1_IDE (0U)
#define DIAG_REQ_BTA1_DLC (64U)
#define DIAG_REQ_BTA1_CANID (0x670U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t DiagReqData0_BTA1_0 : 8U;
    uint8_t DiagReqData0_BTA1_1 : 8U;
    uint8_t DiagReqData0_BTA1_2 : 8U;
    uint8_t DiagReqData0_BTA1_3 : 8U;
    uint8_t DiagReqData1_BTA1_0 : 8U;
    uint8_t DiagReqData1_BTA1_1 : 8U;
    uint8_t DiagReqData1_BTA1_2 : 8U;
    uint8_t DiagReqData1_BTA1_3 : 8U;
    uint8_t DiagReqData2_BTA1_0 : 8U;
    uint8_t DiagReqData2_BTA1_1 : 8U;
    uint8_t DiagReqData2_BTA1_2 : 8U;
    uint8_t DiagReqData2_BTA1_3 : 8U;
    uint8_t DiagReqData3_BTA1_0 : 8U;
    uint8_t DiagReqData3_BTA1_1 : 8U;
    uint8_t DiagReqData3_BTA1_2 : 8U;
    uint8_t DiagReqData3_BTA1_3 : 8U;
    uint8_t DiagReqData4_BTA1_0 : 8U;
    uint8_t DiagReqData4_BTA1_1 : 8U;
    uint8_t DiagReqData4_BTA1_2 : 8U;
    uint8_t DiagReqData4_BTA1_3 : 8U;
    uint8_t DiagReqData5_BTA1_0 : 8U;
    uint8_t DiagReqData5_BTA1_1 : 8U;
    uint8_t DiagReqData5_BTA1_2 : 8U;
    uint8_t DiagReqData5_BTA1_3 : 8U;
    uint8_t DiagReqData6_BTA1_0 : 8U;
    uint8_t DiagReqData6_BTA1_1 : 8U;
    uint8_t DiagReqData6_BTA1_2 : 8U;
    uint8_t DiagReqData6_BTA1_3 : 8U;
    uint8_t DiagReqData7_BTA1_0 : 8U;
    uint8_t DiagReqData7_BTA1_1 : 8U;
    uint8_t DiagReqData7_BTA1_2 : 8U;
    uint8_t DiagReqData7_BTA1_3 : 8U;
    uint8_t DiagReqData8_BTA1_0 : 8U;
    uint8_t DiagReqData8_BTA1_1 : 8U;
    uint8_t DiagReqData8_BTA1_2 : 8U;
    uint8_t DiagReqData8_BTA1_3 : 8U;
    uint8_t DiagReqData9_BTA1_0 : 8U;
    uint8_t DiagReqData9_BTA1_1 : 8U;
    uint8_t DiagReqData9_BTA1_2 : 8U;
    uint8_t DiagReqData9_BTA1_3 : 8U;
    uint8_t DiagReqData10_BTA1_0 : 8U;
    uint8_t DiagReqData10_BTA1_1 : 8U;
    uint8_t DiagReqData10_BTA1_2 : 8U;
    uint8_t DiagReqData10_BTA1_3 : 8U;
    uint8_t DiagReqData11_BTA1_0 : 8U;
    uint8_t DiagReqData11_BTA1_1 : 8U;
    uint8_t DiagReqData11_BTA1_2 : 8U;
    uint8_t DiagReqData11_BTA1_3 : 8U;
    uint8_t DiagReqData12_BTA1_0 : 8U;
    uint8_t DiagReqData12_BTA1_1 : 8U;
    uint8_t DiagReqData12_BTA1_2 : 8U;
    uint8_t DiagReqData12_BTA1_3 : 8U;
    uint8_t DiagReqData13_BTA1_0 : 8U;
    uint8_t DiagReqData13_BTA1_1 : 8U;
    uint8_t DiagReqData13_BTA1_2 : 8U;
    uint8_t DiagReqData13_BTA1_3 : 8U;
    uint8_t DiagReqData14_BTA1_0 : 8U;
    uint8_t DiagReqData14_BTA1_1 : 8U;
    uint8_t DiagReqData14_BTA1_2 : 8U;
    uint8_t DiagReqData14_BTA1_3 : 8U;
    uint8_t DiagReqData15_BTA1_0 : 8U;
    uint8_t DiagReqData15_BTA1_1 : 8U;
    uint8_t DiagReqData15_BTA1_2 : 8U;
    uint8_t DiagReqData15_BTA1_3 : 8U;
  }sig;
} DIAG_REQ_BTA1_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  DIAG_REQ_BTA1_un msg;
}DIAG_REQ_BTA1_msg;

// def @DIAG_RESP_BTA1 CAN Message (1776 0x6f0)
#define DIAG_RESP_BTA1_IDE (0U)
#define DIAG_RESP_BTA1_DLC (64U)
#define DIAG_RESP_BTA1_CANID (0x6f0U)

typedef union
{
  uint8_t data[64u];
  struct
  {
    uint8_t DiagRespData0_BTA1_0 : 8U;
    uint8_t DiagRespData0_BTA1_1 : 8U;
    uint8_t DiagRespData0_BTA1_2 : 8U;
    uint8_t DiagRespData0_BTA1_3 : 8U;
    uint8_t DiagRespData1_BTA1_0 : 8U;
    uint8_t DiagRespData1_BTA1_1 : 8U;
    uint8_t DiagRespData1_BTA1_2 : 8U;
    uint8_t DiagRespData1_BTA1_3 : 8U;
    uint8_t DiagRespData2_BTA1_0 : 8U;
    uint8_t DiagRespData2_BTA1_1 : 8U;
    uint8_t DiagRespData2_BTA1_2 : 8U;
    uint8_t DiagRespData2_BTA1_3 : 8U;
    uint8_t DiagRespData3_BTA1_0 : 8U;
    uint8_t DiagRespData3_BTA1_1 : 8U;
    uint8_t DiagRespData3_BTA1_2 : 8U;
    uint8_t DiagRespData3_BTA1_3 : 8U;
    uint8_t DiagRespData4_BTA1_0 : 8U;
    uint8_t DiagRespData4_BTA1_1 : 8U;
    uint8_t DiagRespData4_BTA1_2 : 8U;
    uint8_t DiagRespData4_BTA1_3 : 8U;
    uint8_t DiagRespData5_BTA1_0 : 8U;
    uint8_t DiagRespData5_BTA1_1 : 8U;
    uint8_t DiagRespData5_BTA1_2 : 8U;
    uint8_t DiagRespData5_BTA1_3 : 8U;
    uint8_t DiagRespData6_BTA1_0 : 8U;
    uint8_t DiagRespData6_BTA1_1 : 8U;
    uint8_t DiagRespData6_BTA1_2 : 8U;
    uint8_t DiagRespData6_BTA1_3 : 8U;
    uint8_t DiagRespData7_BTA1_0 : 8U;
    uint8_t DiagRespData7_BTA1_1 : 8U;
    uint8_t DiagRespData7_BTA1_2 : 8U;
    uint8_t DiagRespData7_BTA1_3 : 8U;
    uint8_t DiagRespData8_BTA1_0 : 8U;
    uint8_t DiagRespData8_BTA1_1 : 8U;
    uint8_t DiagRespData8_BTA1_2 : 8U;
    uint8_t DiagRespData8_BTA1_3 : 8U;
    uint8_t DiagRespData9_BTA1_0 : 8U;
    uint8_t DiagRespData9_BTA1_1 : 8U;
    uint8_t DiagRespData9_BTA1_2 : 8U;
    uint8_t DiagRespData9_BTA1_3 : 8U;
    uint8_t DiagRespData10_BTA1_0 : 8U;
    uint8_t DiagRespData10_BTA1_1 : 8U;
    uint8_t DiagRespData10_BTA1_2 : 8U;
    uint8_t DiagRespData10_BTA1_3 : 8U;
    uint8_t DiagRespData11_BTA1_0 : 8U;
    uint8_t DiagRespData11_BTA1_1 : 8U;
    uint8_t DiagRespData11_BTA1_2 : 8U;
    uint8_t DiagRespData11_BTA1_3 : 8U;
    uint8_t DiagRespData12_BTA1_0 : 8U;
    uint8_t DiagRespData12_BTA1_1 : 8U;
    uint8_t DiagRespData12_BTA1_2 : 8U;
    uint8_t DiagRespData12_BTA1_3 : 8U;
    uint8_t DiagRespData13_BTA1_0 : 8U;
    uint8_t DiagRespData13_BTA1_1 : 8U;
    uint8_t DiagRespData13_BTA1_2 : 8U;
    uint8_t DiagRespData13_BTA1_3 : 8U;
    uint8_t DiagRespData14_BTA1_0 : 8U;
    uint8_t DiagRespData14_BTA1_1 : 8U;
    uint8_t DiagRespData14_BTA1_2 : 8U;
    uint8_t DiagRespData14_BTA1_3 : 8U;
    uint8_t DiagRespData15_BTA1_0 : 8U;
    uint8_t DiagRespData15_BTA1_1 : 8U;
    uint8_t DiagRespData15_BTA1_2 : 8U;
    uint8_t DiagRespData15_BTA1_3 : 8U;
  }sig;
} DIAG_RESP_BTA1_un;

typedef struct
{
  uint8_t  DLC;
  uint32_t ID;
  uint16_t cycle;
  uint8_t  On_line_flag;
  uint16_t RecvMsgCnt;
  uint32_t MsgTimeOut_Time;
  uint32_t MsgTimeOut_PreT;

  DIAG_RESP_BTA1_un msg;
}DIAG_RESP_BTA1_msg;

extern BCU_11E_msg BCU_11E_t;
extern ZONE_13C_msg ZONE_13C_t;
extern Posn_Para_Set_msg Posn_Para_Set_t;
extern BLE_SnifferParam_Dev1_msg BLE_SnifferParam_Dev1_t;
extern BLE_SnifferParam_Dev2_msg BLE_SnifferParam_Dev2_t;
extern BLE_SnifferParam_Dev3_msg BLE_SnifferParam_Dev3_t;
extern BLE_SnifferParam_Dev4_msg BLE_SnifferParam_Dev4_t;
extern BLE_SnifferParam_Dev5_msg BLE_SnifferParam_Dev5_t;
extern CccDkReq_msg CccDkReq_t;
extern CccBleDkSts_msg CccBleDkSts_t;
extern BTA6_RSSI_UWB_msg BTA6_RSSI_UWB_t;
extern BTA1_Dev5_Sts_msg BTA1_Dev5_Sts_t;
extern BLE_Common_Sts_msg BLE_Common_Sts_t;
extern BTA1_Dev1_Sts_msg BTA1_Dev1_Sts_t;
extern BTA1_Dev2_Sts_msg BTA1_Dev2_Sts_t;
extern BTA1_Dev3_Sts_msg BTA1_Dev3_Sts_t;
extern BTA1_Dev4_Sts_msg BTA1_Dev4_Sts_t;
extern BTA1_RSSI_UWB_msg BTA1_RSSI_UWB_t;
extern BTA2_RSSI_UWB_msg BTA2_RSSI_UWB_t;
extern BTA3_RSSI_UWB_msg BTA3_RSSI_UWB_t;
extern BTA4_RSSI_UWB_msg BTA4_RSSI_UWB_t;
extern BTA5_RSSI_UWB_msg BTA5_RSSI_UWB_t;
extern UWB_Info_Ntf_msg UWB_Info_Ntf_t;
extern UWB_Info_Ntf_Sup_msg UWB_Info_Ntf_Sup_t;
extern BtaOutputRfData_msg BtaOutputRfData_t;
extern BtaOutputRfData1_msg BtaOutputRfData1_t;
extern BTA_SRF_295_msg BTA_SRF_295_t;
extern ZONE_296_msg ZONE_296_t;
extern Device_IRK_Group_1_msg Device_IRK_Group_1_t;
extern Device_IRK_Group_2_msg Device_IRK_Group_2_t;
extern BTA4_FusnData_msg BTA4_FusnData_t;
extern BTA1_01_msg BTA1_01_t;
extern ZONE_3AA_msg ZONE_3AA_t;
extern ZONE_3B2_msg ZONE_3B2_t;
extern ZONE_L_TPMS_01_msg ZONE_L_TPMS_01_t;
extern BTA1_Log_Data_msg BTA1_Log_Data_t;
extern CccDkBleCmd_msg CccDkBleCmd_t;
extern CccDkBleCmdFlowCtrl_msg CccDkBleCmdFlowCtrl_t;
extern CccDkBleRsp_msg CccDkBleRsp_t;
extern CccDkBleRspFlowCtrl_msg CccDkBleRspFlowCtrl_t;
extern BTA_02_msg BTA_02_t;
extern BLE_Uplink_FlowCtrl_msg BLE_Uplink_FlowCtrl_t;
extern BLE_Downlink_Data_msg BLE_Downlink_Data_t;
extern BTA7_RSSI_UWB_msg BTA7_RSSI_UWB_t;
extern BLE_Uplink_Data_msg BLE_Uplink_Data_t;
extern BLE_Downlink_FlowCtrl_msg BLE_Downlink_FlowCtrl_t;
extern BLE_DK_REQ_msg BLE_DK_REQ_t;
extern BLE_DK_RSP_msg BLE_DK_RSP_t;
extern BTA_SRF_FactoryRsp_msg BTA_SRF_FactoryRsp_t;
extern BTA_SRF_FactoryReq_msg BTA_SRF_FactoryReq_t;
extern NM_ZONE_LEM_msg NM_ZONE_LEM_t;
extern NM_BTA1_msg NM_BTA1_t;
extern NM_BTA2_msg NM_BTA2_t;
extern NM_BTA3_msg NM_BTA3_t;
extern NM_BTA4_msg NM_BTA4_t;
extern NM_BTA5_msg NM_BTA5_t;
extern NM_BTA_RB2_msg NM_BTA_RB2_t;
extern NM_BTA_FB_msg NM_BTA_FB_t;
extern VC_Data_5E0_msg VC_Data_5E0_t;
extern DIAG_REQ_AllECU_msg DIAG_REQ_AllECU_t;
extern DIAG_REQ_BTA1_msg DIAG_REQ_BTA1_t;
extern DIAG_RESP_BTA1_msg DIAG_RESP_BTA1_t;


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
#define IL_VDC_Posn_Mode_Set_Dev1_RX  (Posn_Para_Set_t.msg.sig.VDC_Posn_Mode_Set_Dev1)

/*  Tx */
#define IL_VDC_Posn_Mode_Set_Dev1_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.VDC_Posn_Mode_Set_Dev1 = (x);\
}

/*  Rx */
#define IL_VDC_Posn_Mode_Set_Dev2_RX  (Posn_Para_Set_t.msg.sig.VDC_Posn_Mode_Set_Dev2)

/*  Tx */
#define IL_VDC_Posn_Mode_Set_Dev2_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.VDC_Posn_Mode_Set_Dev2 = (x);\
}

/*  Rx */
#define IL_VDC_Posn_Mode_Set_Dev3_RX  (Posn_Para_Set_t.msg.sig.VDC_Posn_Mode_Set_Dev3)

/*  Tx */
#define IL_VDC_Posn_Mode_Set_Dev3_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.VDC_Posn_Mode_Set_Dev3 = (x);\
}

/*  Rx */
#define IL_VDC_Posn_Mode_Set_Dev4_RX  (Posn_Para_Set_t.msg.sig.VDC_Posn_Mode_Set_Dev4)

/*  Tx */
#define IL_VDC_Posn_Mode_Set_Dev4_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.VDC_Posn_Mode_Set_Dev4 = (x);\
}

/*  Rx */
#define IL_VDC_Posn_Mode_Set_Dev5_RX  (Posn_Para_Set_t.msg.sig.VDC_Posn_Mode_Set_Dev5)

/*  Tx */
#define IL_VDC_Posn_Mode_Set_Dev5_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.VDC_Posn_Mode_Set_Dev5 = (x);\
}

/*  Rx */
#define IL_BLE_Cyc_Ratio_Dev1_RX  (Posn_Para_Set_t.msg.sig.BLE_Cyc_Ratio_Dev1)

/*  Tx */
#define IL_BLE_Cyc_Ratio_Dev1_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.BLE_Cyc_Ratio_Dev1 = (x);\
}

/*  Rx */
#define IL_BLE_Cyc_Ratio_Dev2_RX  (Posn_Para_Set_t.msg.sig.BLE_Cyc_Ratio_Dev2)

/*  Tx */
#define IL_BLE_Cyc_Ratio_Dev2_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.BLE_Cyc_Ratio_Dev2 = (x);\
}

/*  Rx */
#define IL_BLE_Cyc_Ratio_Dev3_RX  (Posn_Para_Set_t.msg.sig.BLE_Cyc_Ratio_Dev3)

/*  Tx */
#define IL_BLE_Cyc_Ratio_Dev3_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.BLE_Cyc_Ratio_Dev3 = (x);\
}

/*  Rx */
#define IL_BLE_Cyc_Ratio_Dev4_RX  (Posn_Para_Set_t.msg.sig.BLE_Cyc_Ratio_Dev4)

/*  Tx */
#define IL_BLE_Cyc_Ratio_Dev4_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.BLE_Cyc_Ratio_Dev4 = (x);\
}

/*  Rx */
#define IL_BLE_Cyc_Ratio_Dev5_RX  (Posn_Para_Set_t.msg.sig.BLE_Cyc_Ratio_Dev5)

/*  Tx */
#define IL_BLE_Cyc_Ratio_Dev5_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.BLE_Cyc_Ratio_Dev5 = (x);\
}

/*  Rx */
#define IL_UWB_Cyc_Ratio_Dev1_RX  (Posn_Para_Set_t.msg.sig.UWB_Cyc_Ratio_Dev1)

/*  Tx */
#define IL_UWB_Cyc_Ratio_Dev1_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.UWB_Cyc_Ratio_Dev1 = (x);\
}

/*  Rx */
#define IL_UWB_Cyc_Ratio_Dev2_RX  (Posn_Para_Set_t.msg.sig.UWB_Cyc_Ratio_Dev2)

/*  Tx */
#define IL_UWB_Cyc_Ratio_Dev2_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.UWB_Cyc_Ratio_Dev2 = (x);\
}

/*  Rx */
#define IL_UWB_Cyc_Ratio_Dev3_RX  (Posn_Para_Set_t.msg.sig.UWB_Cyc_Ratio_Dev3)

/*  Tx */
#define IL_UWB_Cyc_Ratio_Dev3_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.UWB_Cyc_Ratio_Dev3 = (x);\
}

/*  Rx */
#define IL_UWB_Cyc_Ratio_Dev4_RX  (Posn_Para_Set_t.msg.sig.UWB_Cyc_Ratio_Dev4)

/*  Tx */
#define IL_UWB_Cyc_Ratio_Dev4_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.UWB_Cyc_Ratio_Dev4 = (x);\
}

/*  Rx */
#define IL_UWB_Cyc_Ratio_Dev5_RX  (Posn_Para_Set_t.msg.sig.UWB_Cyc_Ratio_Dev5)

/*  Tx */
#define IL_UWB_Cyc_Ratio_Dev5_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.UWB_Cyc_Ratio_Dev5 = (x);\
}

/*  Rx */
#define IL_NFC_Entry_Enable_REQ_RX  (Posn_Para_Set_t.msg.sig.NFC_Entry_Enable_REQ)

/*  Tx */
#define IL_NFC_Entry_Enable_REQ_TX(x)  \
{ \
	Posn_Para_Set_t.msg.sig.NFC_Entry_Enable_REQ = (x);\
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
#define IL_BleOpModeSwReq_RX  (CccDkReq_t.msg.sig.BleOpModeSwReq)

/*  Tx */
#define IL_BleOpModeSwReq_TX(x)  \
{ \
	CccDkReq_t.msg.sig.BleOpModeSwReq = (x);\
}

/*  Rx */
#define IL_DisconnBleKeyReq_RX  (CccDkReq_t.msg.sig.DisconnBleKeyReq)

/*  Tx */
#define IL_DisconnBleKeyReq_TX(x)  \
{ \
	CccDkReq_t.msg.sig.DisconnBleKeyReq = (x);\
}

/*  Rx */
#define IL_DisconnBleKeyDevAddr_0_3_RX  (\
	(CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_0_3_0 << 24U) | \
	(CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_0_3_1 << 16U) | \
	(CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_0_3_2 << 8U) | \
	(CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_0_3_3))

/* Tx */
#define IL_DisconnBleKeyDevAddr_0_3_TX(x)  \
{ \
	CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_0_3_0 = ((x) >> 24U) & 255U; \
	CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_0_3_1 = ((x) >> 16U) & 255U; \
	CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_0_3_2 = ((x) >> 8U) & 255U; \
	CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_0_3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DisconnBleKeyDevAddr_4_5_RX  (\
	(CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_4_5_0 << 8U) | \
	(CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_4_5_1))

/* Tx */
#define IL_DisconnBleKeyDevAddr_4_5_TX(x)  \
{ \
	CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_4_5_0 = ((x) >> 8U) & 255U; \
	CccDkReq_t.msg.sig.DisconnBleKeyDevAddr_4_5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BleOpModeSts_RX  (CccBleDkSts_t.msg.sig.BleOpModeSts)

/*  Tx */
#define IL_BleOpModeSts_TX(x)  \
{ \
	CccBleDkSts_t.msg.sig.BleOpModeSts = (x);\
}

/*  Rx */
#define IL_UwbConfig_RX  (CccBleDkSts_t.msg.sig.UwbConfig)

/*  Tx */
#define IL_UwbConfig_TX(x)  \
{ \
	CccBleDkSts_t.msg.sig.UwbConfig = (x);\
}

/*  Rx */
#define IL_LELRConfig_RX  (CccBleDkSts_t.msg.sig.LELRConfig)

/*  Tx */
#define IL_LELRConfig_TX(x)  \
{ \
	CccBleDkSts_t.msg.sig.LELRConfig = (x);\
}

/*  Rx */
#define IL_BTA1_IRK_0_3_RX  (\
	(CccBleDkSts_t.msg.sig.BTA1_IRK_0_3_0 << 24U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_0_3_1 << 16U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_0_3_2 << 8U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_0_3_3))

/* Tx */
#define IL_BTA1_IRK_0_3_TX(x)  \
{ \
	CccBleDkSts_t.msg.sig.BTA1_IRK_0_3_0 = ((x) >> 24U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_0_3_1 = ((x) >> 16U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_0_3_2 = ((x) >> 8U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_0_3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_IRK_4_7_RX  (\
	(CccBleDkSts_t.msg.sig.BTA1_IRK_4_7_0 << 24U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_4_7_1 << 16U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_4_7_2 << 8U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_4_7_3))

/* Tx */
#define IL_BTA1_IRK_4_7_TX(x)  \
{ \
	CccBleDkSts_t.msg.sig.BTA1_IRK_4_7_0 = ((x) >> 24U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_4_7_1 = ((x) >> 16U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_4_7_2 = ((x) >> 8U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_4_7_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_IRK_8_11_RX  (\
	(CccBleDkSts_t.msg.sig.BTA1_IRK_8_11_0 << 24U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_8_11_1 << 16U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_8_11_2 << 8U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_8_11_3))

/* Tx */
#define IL_BTA1_IRK_8_11_TX(x)  \
{ \
	CccBleDkSts_t.msg.sig.BTA1_IRK_8_11_0 = ((x) >> 24U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_8_11_1 = ((x) >> 16U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_8_11_2 = ((x) >> 8U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_8_11_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_IRK_12_15_RX  (\
	(CccBleDkSts_t.msg.sig.BTA1_IRK_12_15_0 << 24U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_12_15_1 << 16U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_12_15_2 << 8U) | \
	(CccBleDkSts_t.msg.sig.BTA1_IRK_12_15_3))

/* Tx */
#define IL_BTA1_IRK_12_15_TX(x)  \
{ \
	CccBleDkSts_t.msg.sig.BTA1_IRK_12_15_0 = ((x) >> 24U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_12_15_1 = ((x) >> 16U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_12_15_2 = ((x) >> 8U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_IRK_12_15_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_RSA_0_3_RX  (\
	(CccBleDkSts_t.msg.sig.BTA1_RSA_0_3_0 << 24U) | \
	(CccBleDkSts_t.msg.sig.BTA1_RSA_0_3_1 << 16U) | \
	(CccBleDkSts_t.msg.sig.BTA1_RSA_0_3_2 << 8U) | \
	(CccBleDkSts_t.msg.sig.BTA1_RSA_0_3_3))

/* Tx */
#define IL_BTA1_RSA_0_3_TX(x)  \
{ \
	CccBleDkSts_t.msg.sig.BTA1_RSA_0_3_0 = ((x) >> 24U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_RSA_0_3_1 = ((x) >> 16U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_RSA_0_3_2 = ((x) >> 8U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_RSA_0_3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_RSA_4_5_RX  (\
	(CccBleDkSts_t.msg.sig.BTA1_RSA_4_5_0 << 8U) | \
	(CccBleDkSts_t.msg.sig.BTA1_RSA_4_5_1))

/* Tx */
#define IL_BTA1_RSA_4_5_TX(x)  \
{ \
	CccBleDkSts_t.msg.sig.BTA1_RSA_4_5_0 = ((x) >> 8U) & 255U; \
	CccBleDkSts_t.msg.sig.BTA1_RSA_4_5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA6_RSSI_Dev1_phy_RX  (BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Dev1_phy)

/*  Tx */
#define IL_BTA6_RSSI_Dev1_phy_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Dev1_phy = (x);\
}

/*  Rx */
#define IL_BTA6_RSSI_Dev2_phy_RX  (BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Dev2_phy)

/*  Tx */
#define IL_BTA6_RSSI_Dev2_phy_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Dev2_phy = (x);\
}

/*  Rx */
#define IL_BTA6_RSSI_Dev3_phy_RX  (BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Dev3_phy)

/*  Tx */
#define IL_BTA6_RSSI_Dev3_phy_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Dev3_phy = (x);\
}

/*  Rx */
#define IL_BTA6_RSSI_Dev4_phy_RX  (BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Dev4_phy)

/*  Tx */
#define IL_BTA6_RSSI_Dev4_phy_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Dev4_phy = (x);\
}

/*  Rx */
#define IL_BTA6_RSSI_Dev5_phy_RX  (BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Dev5_phy)

/*  Tx */
#define IL_BTA6_RSSI_Dev5_phy_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Dev5_phy = (x);\
}

/*  Rx */
#define IL_BTA6_UWB_Dist_Dev1_RX  (\
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev1_0 << 8U) | \
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev1_1))

/* Tx */
#define IL_BTA6_UWB_Dist_Dev1_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev1_0 = ((x) >> 8U) & 255U; \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA6_UWB_Dist_Dev2_RX  (\
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev2_0 << 8U) | \
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev2_1))

/* Tx */
#define IL_BTA6_UWB_Dist_Dev2_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev2_0 = ((x) >> 8U) & 255U; \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA6_UWB_Dist_Dev3_RX  (\
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev3_0 << 8U) | \
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev3_1))

/* Tx */
#define IL_BTA6_UWB_Dist_Dev3_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev3_0 = ((x) >> 8U) & 255U; \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA6_UWB_Dist_Dev4_RX  (\
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev4_0 << 8U) | \
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev4_1))

/* Tx */
#define IL_BTA6_UWB_Dist_Dev4_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev4_0 = ((x) >> 8U) & 255U; \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA6_UWB_Dist_Dev5_RX  (\
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev5_0 << 8U) | \
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev5_1))

/* Tx */
#define IL_BTA6_UWB_Dist_Dev5_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev5_0 = ((x) >> 8U) & 255U; \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_UWB_Dist_Dev5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_UWBInfoSyncStsdev1_BTA6_RX  (BTA6_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev1_BTA6)

/*  Tx */
#define IL_UWBInfoSyncStsdev1_BTA6_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev1_BTA6 = (x);\
}

/*  Rx */
#define IL_UWBInfoSyncStsdev2_BTA6_RX  (BTA6_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev2_BTA6)

/*  Tx */
#define IL_UWBInfoSyncStsdev2_BTA6_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev2_BTA6 = (x);\
}

/*  Rx */
#define IL_UWBInfoSyncStsdev3_BTA6_RX  (BTA6_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev3_BTA6)

/*  Tx */
#define IL_UWBInfoSyncStsdev3_BTA6_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev3_BTA6 = (x);\
}

/*  Rx */
#define IL_UWBInfoSyncStsdev4_BTA6_RX  (BTA6_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev4_BTA6)

/*  Tx */
#define IL_UWBInfoSyncStsdev4_BTA6_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev4_BTA6 = (x);\
}

/*  Rx */
#define IL_UWBInfoSyncStsdev5_BTA6_RX  (BTA6_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev5_BTA6)

/*  Tx */
#define IL_UWBInfoSyncStsdev5_BTA6_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev5_BTA6 = (x);\
}

/*  Rx */
#define IL_UWBRangingStsdev1_BTA6_RX  (BTA6_RSSI_UWB_t.msg.sig.UWBRangingStsdev1_BTA6)

/*  Tx */
#define IL_UWBRangingStsdev1_BTA6_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.UWBRangingStsdev1_BTA6 = (x);\
}

/*  Rx */
#define IL_UWBRangingStsdev2_BTA6_RX  (BTA6_RSSI_UWB_t.msg.sig.UWBRangingStsdev2_BTA6)

/*  Tx */
#define IL_UWBRangingStsdev2_BTA6_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.UWBRangingStsdev2_BTA6 = (x);\
}

/*  Rx */
#define IL_UWBRangingStsdev3_BTA6_RX  (BTA6_RSSI_UWB_t.msg.sig.UWBRangingStsdev3_BTA6)

/*  Tx */
#define IL_UWBRangingStsdev3_BTA6_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.UWBRangingStsdev3_BTA6 = (x);\
}

/*  Rx */
#define IL_UWBRangingStsdev4_BTA6_RX  (BTA6_RSSI_UWB_t.msg.sig.UWBRangingStsdev4_BTA6)

/*  Tx */
#define IL_UWBRangingStsdev4_BTA6_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.UWBRangingStsdev4_BTA6 = (x);\
}

/*  Rx */
#define IL_UWBRangingStsdev5_BTA6_RX  (BTA6_RSSI_UWB_t.msg.sig.UWBRangingStsdev5_BTA6)

/*  Tx */
#define IL_UWBRangingStsdev5_BTA6_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.UWBRangingStsdev5_BTA6 = (x);\
}

/*  Rx */
#define IL_BTA6_RSSI_Channel_Index_Dev2_RX  (\
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev2_0 << 4U) | \
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev2_0))

/* Tx */
#define IL_BTA6_RSSI_Channel_Index_Dev2_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev2_0 = ((x) >> 4U) & 3U; \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev2_0 = (x) & 15U; \
}

/*  Rx */
#define IL_BTA6_RSSI_Channel_Index_Dev1_RX  (BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev1)

/*  Tx */
#define IL_BTA6_RSSI_Channel_Index_Dev1_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev1 = (x);\
}

/*  Rx */
#define IL_BTA6_RSSI_Channel_Index_Dev3_RX  (\
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev3_0 << 2U) | \
	(BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev3_0))

/* Tx */
#define IL_BTA6_RSSI_Channel_Index_Dev3_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev3_0 = ((x) >> 2U) & 15U; \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev3_0 = (x) & 3U; \
}

/*  Rx */
#define IL_BTA6_RSSI_Channel_Index_Dev4_RX  (BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev4)

/*  Tx */
#define IL_BTA6_RSSI_Channel_Index_Dev4_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev4 = (x);\
}

/*  Rx */
#define IL_BTA6_RSSI_Channel_Index_Dev5_RX  (BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev5)

/*  Tx */
#define IL_BTA6_RSSI_Channel_Index_Dev5_TX(x)  \
{ \
	BTA6_RSSI_UWB_t.msg.sig.BTA6_RSSI_Channel_Index_Dev5 = (x);\
}

/*  Rx */
#define IL_SmtKey_MotionSts_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.SmtKey_MotionSts_Dev5)

/*  Tx */
#define IL_SmtKey_MotionSts_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.SmtKey_MotionSts_Dev5 = (x);\
}

/*  Rx */
#define IL_SmtKey_MotionSensorSts_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.SmtKey_MotionSensorSts_Dev5)

/*  Tx */
#define IL_SmtKey_MotionSensorSts_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.SmtKey_MotionSensorSts_Dev5 = (x);\
}

/*  Rx */
#define IL_SmtKey_ScreenLockSts_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.SmtKey_ScreenLockSts_Dev5)

/*  Tx */
#define IL_SmtKey_ScreenLockSts_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.SmtKey_ScreenLockSts_Dev5 = (x);\
}

/*  Rx */
#define IL_SmtKey_PosnMode_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.SmtKey_PosnMode_Dev5)

/*  Tx */
#define IL_SmtKey_PosnMode_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.SmtKey_PosnMode_Dev5 = (x);\
}

/*  Rx */
#define IL_SmtKey_DevType_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.SmtKey_DevType_Dev5)

/*  Tx */
#define IL_SmtKey_DevType_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.SmtKey_DevType_Dev5 = (x);\
}

/*  Rx */
#define IL_UWB_PredURSK_Sts_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.UWB_PredURSK_Sts_Dev5)

/*  Tx */
#define IL_UWB_PredURSK_Sts_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.UWB_PredURSK_Sts_Dev5 = (x);\
}

/*  Rx */
#define IL_Keyfob_IsBattLow_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.Keyfob_IsBattLow_Dev5)

/*  Tx */
#define IL_Keyfob_IsBattLow_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.Keyfob_IsBattLow_Dev5 = (x);\
}

/*  Rx */
#define IL_UWB_Locn_Cnt_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.UWB_Locn_Cnt_Dev5)

/*  Tx */
#define IL_UWB_Locn_Cnt_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.UWB_Locn_Cnt_Dev5 = (x);\
}

/*  Rx */
#define IL_Keyfob_Temp_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.Keyfob_Temp_Dev5)

/*  Tx */
#define IL_Keyfob_Temp_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.Keyfob_Temp_Dev5 = (x);\
}

/*  Rx */
#define IL_BTA1_Conn_Sts_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.BTA1_Conn_Sts_Dev5)

/*  Tx */
#define IL_BTA1_Conn_Sts_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.BTA1_Conn_Sts_Dev5 = (x);\
}

/*  Rx */
#define IL_Keyfob_Batt_Vtg_Dev5_RX  (\
	(BTA1_Dev5_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev5_0 << 8U) | \
	(BTA1_Dev5_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev5_1))

/* Tx */
#define IL_Keyfob_Batt_Vtg_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev5_0 = ((x) >> 8U) & 255U; \
	BTA1_Dev5_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_UWB_Locn_Interval_Dev5_RX  (\
	(BTA1_Dev5_Sts_t.msg.sig.UWB_Locn_Interval_Dev5_0 << 8U) | \
	(BTA1_Dev5_Sts_t.msg.sig.UWB_Locn_Interval_Dev5_1))

/* Tx */
#define IL_UWB_Locn_Interval_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.UWB_Locn_Interval_Dev5_0 = ((x) >> 8U) & 255U; \
	BTA1_Dev5_Sts_t.msg.sig.UWB_Locn_Interval_Dev5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_Keyfob_Batt_Pct_Dev5_RX  (BTA1_Dev5_Sts_t.msg.sig.Keyfob_Batt_Pct_Dev5)

/*  Tx */
#define IL_Keyfob_Batt_Pct_Dev5_TX(x)  \
{ \
	BTA1_Dev5_Sts_t.msg.sig.Keyfob_Batt_Pct_Dev5 = (x);\
}

/*  Rx */
#define IL_BTA1_Adv_Sts_RX  (BLE_Common_Sts_t.msg.sig.BTA1_Adv_Sts)

/*  Tx */
#define IL_BTA1_Adv_Sts_TX(x)  \
{ \
	BLE_Common_Sts_t.msg.sig.BTA1_Adv_Sts = (x);\
}

/*  Rx */
#define IL_BTA1_Scan_Sts_RX  (BLE_Common_Sts_t.msg.sig.BTA1_Scan_Sts)

/*  Tx */
#define IL_BTA1_Scan_Sts_TX(x)  \
{ \
	BLE_Common_Sts_t.msg.sig.BTA1_Scan_Sts = (x);\
}

/*  Rx */
#define IL_BTA1_Scan_Interval_RX  (\
	(BLE_Common_Sts_t.msg.sig.BTA1_Scan_Interval_0 << 8U) | \
	(BLE_Common_Sts_t.msg.sig.BTA1_Scan_Interval_1))

/* Tx */
#define IL_BTA1_Scan_Interval_TX(x)  \
{ \
	BLE_Common_Sts_t.msg.sig.BTA1_Scan_Interval_0 = ((x) >> 8U) & 255U; \
	BLE_Common_Sts_t.msg.sig.BTA1_Scan_Interval_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Adv_Interval_RX  (\
	(BLE_Common_Sts_t.msg.sig.BTA1_Adv_Interval_0 << 8U) | \
	(BLE_Common_Sts_t.msg.sig.BTA1_Adv_Interval_1))

/* Tx */
#define IL_BTA1_Adv_Interval_TX(x)  \
{ \
	BLE_Common_Sts_t.msg.sig.BTA1_Adv_Interval_0 = ((x) >> 8U) & 255U; \
	BLE_Common_Sts_t.msg.sig.BTA1_Adv_Interval_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Error_Debug_Sts_RX  (\
	(BLE_Common_Sts_t.msg.sig.BTA1_Error_Debug_Sts_0 << 24U) | \
	(BLE_Common_Sts_t.msg.sig.BTA1_Error_Debug_Sts_1 << 16U) | \
	(BLE_Common_Sts_t.msg.sig.BTA1_Error_Debug_Sts_2 << 8U) | \
	(BLE_Common_Sts_t.msg.sig.BTA1_Error_Debug_Sts_3))

/* Tx */
#define IL_BTA1_Error_Debug_Sts_TX(x)  \
{ \
	BLE_Common_Sts_t.msg.sig.BTA1_Error_Debug_Sts_0 = ((x) >> 24U) & 255U; \
	BLE_Common_Sts_t.msg.sig.BTA1_Error_Debug_Sts_1 = ((x) >> 16U) & 255U; \
	BLE_Common_Sts_t.msg.sig.BTA1_Error_Debug_Sts_2 = ((x) >> 8U) & 255U; \
	BLE_Common_Sts_t.msg.sig.BTA1_Error_Debug_Sts_3 = (x) & 255U; \
}

/*  Rx */
#define IL_SmtKey_MotionSts_Dev1_RX  (BTA1_Dev1_Sts_t.msg.sig.SmtKey_MotionSts_Dev1)

/*  Tx */
#define IL_SmtKey_MotionSts_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.SmtKey_MotionSts_Dev1 = (x);\
}

/*  Rx */
#define IL_SmtKey_MotionSensorErrorSts_Dev1_RX  (BTA1_Dev1_Sts_t.msg.sig.SmtKey_MotionSensorErrorSts_Dev1)

/*  Tx */
#define IL_SmtKey_MotionSensorErrorSts_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.SmtKey_MotionSensorErrorSts_Dev1 = (x);\
}

/*  Rx */
#define IL_SmtKey_ScreenLockSts_Dev1_RX  (BTA1_Dev1_Sts_t.msg.sig.SmtKey_ScreenLockSts_Dev1)

/*  Tx */
#define IL_SmtKey_ScreenLockSts_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.SmtKey_ScreenLockSts_Dev1 = (x);\
}

/*  Rx */
#define IL_SmtKey_PosnMode_Dev1_RX  (BTA1_Dev1_Sts_t.msg.sig.SmtKey_PosnMode_Dev1)

/*  Tx */
#define IL_SmtKey_PosnMode_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.SmtKey_PosnMode_Dev1 = (x);\
}

/*  Rx */
#define IL_SmtKey_DevType_Dev1_RX  (BTA1_Dev1_Sts_t.msg.sig.SmtKey_DevType_Dev1)

/*  Tx */
#define IL_SmtKey_DevType_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.SmtKey_DevType_Dev1 = (x);\
}

/*  Rx */
#define IL_UWB_PredURSK_Sts_Dev1_RX  (BTA1_Dev1_Sts_t.msg.sig.UWB_PredURSK_Sts_Dev1)

/*  Tx */
#define IL_UWB_PredURSK_Sts_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.UWB_PredURSK_Sts_Dev1 = (x);\
}

/*  Rx */
#define IL_Keyfob_IsBattLow_Dev1_RX  (BTA1_Dev1_Sts_t.msg.sig.Keyfob_IsBattLow_Dev1)

/*  Tx */
#define IL_Keyfob_IsBattLow_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.Keyfob_IsBattLow_Dev1 = (x);\
}

/*  Rx */
#define IL_UWB_Locn_Cnt_Dev1_RX  (BTA1_Dev1_Sts_t.msg.sig.UWB_Locn_Cnt_Dev1)

/*  Tx */
#define IL_UWB_Locn_Cnt_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.UWB_Locn_Cnt_Dev1 = (x);\
}

/*  Rx */
#define IL_Keyfob_Temp_Dev1_phy_RX  (BTA1_Dev1_Sts_t.msg.sig.Keyfob_Temp_Dev1_phy)

/*  Tx */
#define IL_Keyfob_Temp_Dev1_phy_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.Keyfob_Temp_Dev1_phy = (x);\
}

/*  Rx */
#define IL_BTA1_Conn_Sts_Dev1_RX  (BTA1_Dev1_Sts_t.msg.sig.BTA1_Conn_Sts_Dev1)

/*  Tx */
#define IL_BTA1_Conn_Sts_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.BTA1_Conn_Sts_Dev1 = (x);\
}

/*  Rx */
#define IL_Keyfob_Batt_Vtg_Dev1_RX  (\
	(BTA1_Dev1_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev1_0 << 8U) | \
	(BTA1_Dev1_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev1_1))

/* Tx */
#define IL_Keyfob_Batt_Vtg_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev1_0 = ((x) >> 8U) & 255U; \
	BTA1_Dev1_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_UWB_Locn_Interval_Dev1_RX  (\
	(BTA1_Dev1_Sts_t.msg.sig.UWB_Locn_Interval_Dev1_0 << 8U) | \
	(BTA1_Dev1_Sts_t.msg.sig.UWB_Locn_Interval_Dev1_1))

/* Tx */
#define IL_UWB_Locn_Interval_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.UWB_Locn_Interval_Dev1_0 = ((x) >> 8U) & 255U; \
	BTA1_Dev1_Sts_t.msg.sig.UWB_Locn_Interval_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_Keyfob_Batt_Pct_Dev1_RX  (BTA1_Dev1_Sts_t.msg.sig.Keyfob_Batt_Pct_Dev1)

/*  Tx */
#define IL_Keyfob_Batt_Pct_Dev1_TX(x)  \
{ \
	BTA1_Dev1_Sts_t.msg.sig.Keyfob_Batt_Pct_Dev1 = (x);\
}

/*  Rx */
#define IL_SmtKey_MotionSts_Dev2_RX  (BTA1_Dev2_Sts_t.msg.sig.SmtKey_MotionSts_Dev2)

/*  Tx */
#define IL_SmtKey_MotionSts_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.SmtKey_MotionSts_Dev2 = (x);\
}

/*  Rx */
#define IL_SmtKey_MotionSensorSts_Dev2_RX  (BTA1_Dev2_Sts_t.msg.sig.SmtKey_MotionSensorSts_Dev2)

/*  Tx */
#define IL_SmtKey_MotionSensorSts_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.SmtKey_MotionSensorSts_Dev2 = (x);\
}

/*  Rx */
#define IL_SmtKey_ScreenLockSts_Dev2_RX  (BTA1_Dev2_Sts_t.msg.sig.SmtKey_ScreenLockSts_Dev2)

/*  Tx */
#define IL_SmtKey_ScreenLockSts_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.SmtKey_ScreenLockSts_Dev2 = (x);\
}

/*  Rx */
#define IL_SmtKey_PosnMode_Dev2_RX  (BTA1_Dev2_Sts_t.msg.sig.SmtKey_PosnMode_Dev2)

/*  Tx */
#define IL_SmtKey_PosnMode_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.SmtKey_PosnMode_Dev2 = (x);\
}

/*  Rx */
#define IL_SmtKey_DevType_Dev2_RX  (BTA1_Dev2_Sts_t.msg.sig.SmtKey_DevType_Dev2)

/*  Tx */
#define IL_SmtKey_DevType_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.SmtKey_DevType_Dev2 = (x);\
}

/*  Rx */
#define IL_UWB_PredURSK_Sts_Dev2_RX  (BTA1_Dev2_Sts_t.msg.sig.UWB_PredURSK_Sts_Dev2)

/*  Tx */
#define IL_UWB_PredURSK_Sts_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.UWB_PredURSK_Sts_Dev2 = (x);\
}

/*  Rx */
#define IL_Keyfob_IsBattLow_Dev2_RX  (BTA1_Dev2_Sts_t.msg.sig.Keyfob_IsBattLow_Dev2)

/*  Tx */
#define IL_Keyfob_IsBattLow_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.Keyfob_IsBattLow_Dev2 = (x);\
}

/*  Rx */
#define IL_UWB_Locn_Cnt_Dev2_RX  (BTA1_Dev2_Sts_t.msg.sig.UWB_Locn_Cnt_Dev2)

/*  Tx */
#define IL_UWB_Locn_Cnt_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.UWB_Locn_Cnt_Dev2 = (x);\
}

/*  Rx */
#define IL_Keyfob_Temp_Dev2_phy_RX  (BTA1_Dev2_Sts_t.msg.sig.Keyfob_Temp_Dev2_phy)

/*  Tx */
#define IL_Keyfob_Temp_Dev2_phy_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.Keyfob_Temp_Dev2_phy = (x);\
}

/*  Rx */
#define IL_BTA1_Conn_Sts_Dev2_RX  (BTA1_Dev2_Sts_t.msg.sig.BTA1_Conn_Sts_Dev2)

/*  Tx */
#define IL_BTA1_Conn_Sts_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.BTA1_Conn_Sts_Dev2 = (x);\
}

/*  Rx */
#define IL_Keyfob_Batt_Vtg_Dev2_RX  (\
	(BTA1_Dev2_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev2_0 << 8U) | \
	(BTA1_Dev2_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev2_1))

/* Tx */
#define IL_Keyfob_Batt_Vtg_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev2_0 = ((x) >> 8U) & 255U; \
	BTA1_Dev2_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_UWB_Locn_Interval_Dev2_RX  (\
	(BTA1_Dev2_Sts_t.msg.sig.UWB_Locn_Interval_Dev2_0 << 8U) | \
	(BTA1_Dev2_Sts_t.msg.sig.UWB_Locn_Interval_Dev2_1))

/* Tx */
#define IL_UWB_Locn_Interval_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.UWB_Locn_Interval_Dev2_0 = ((x) >> 8U) & 255U; \
	BTA1_Dev2_Sts_t.msg.sig.UWB_Locn_Interval_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_Keyfob_Batt_Pct_Dev2_RX  (BTA1_Dev2_Sts_t.msg.sig.Keyfob_Batt_Pct_Dev2)

/*  Tx */
#define IL_Keyfob_Batt_Pct_Dev2_TX(x)  \
{ \
	BTA1_Dev2_Sts_t.msg.sig.Keyfob_Batt_Pct_Dev2 = (x);\
}

/*  Rx */
#define IL_SmtKey_MotionSts_Dev3_RX  (BTA1_Dev3_Sts_t.msg.sig.SmtKey_MotionSts_Dev3)

/*  Tx */
#define IL_SmtKey_MotionSts_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.SmtKey_MotionSts_Dev3 = (x);\
}

/*  Rx */
#define IL_SmtKey_MotionSensorSts_Dev3_RX  (BTA1_Dev3_Sts_t.msg.sig.SmtKey_MotionSensorSts_Dev3)

/*  Tx */
#define IL_SmtKey_MotionSensorSts_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.SmtKey_MotionSensorSts_Dev3 = (x);\
}

/*  Rx */
#define IL_SmtKey_ScreenLockSts_Dev3_RX  (BTA1_Dev3_Sts_t.msg.sig.SmtKey_ScreenLockSts_Dev3)

/*  Tx */
#define IL_SmtKey_ScreenLockSts_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.SmtKey_ScreenLockSts_Dev3 = (x);\
}

/*  Rx */
#define IL_SmtKey_PosnMode_Dev3_RX  (BTA1_Dev3_Sts_t.msg.sig.SmtKey_PosnMode_Dev3)

/*  Tx */
#define IL_SmtKey_PosnMode_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.SmtKey_PosnMode_Dev3 = (x);\
}

/*  Rx */
#define IL_SmtKey_DevType_Dev3_RX  (BTA1_Dev3_Sts_t.msg.sig.SmtKey_DevType_Dev3)

/*  Tx */
#define IL_SmtKey_DevType_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.SmtKey_DevType_Dev3 = (x);\
}

/*  Rx */
#define IL_UWB_PredURSK_Sts_Dev3_RX  (BTA1_Dev3_Sts_t.msg.sig.UWB_PredURSK_Sts_Dev3)

/*  Tx */
#define IL_UWB_PredURSK_Sts_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.UWB_PredURSK_Sts_Dev3 = (x);\
}

/*  Rx */
#define IL_Keyfob_IsBattLow_Dev3_RX  (BTA1_Dev3_Sts_t.msg.sig.Keyfob_IsBattLow_Dev3)

/*  Tx */
#define IL_Keyfob_IsBattLow_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.Keyfob_IsBattLow_Dev3 = (x);\
}

/*  Rx */
#define IL_UWB_Locn_Cnt_Dev3_RX  (BTA1_Dev3_Sts_t.msg.sig.UWB_Locn_Cnt_Dev3)

/*  Tx */
#define IL_UWB_Locn_Cnt_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.UWB_Locn_Cnt_Dev3 = (x);\
}

/*  Rx */
#define IL_Keyfob_Temp_Dev3_phy_RX  (BTA1_Dev3_Sts_t.msg.sig.Keyfob_Temp_Dev3_phy)

/*  Tx */
#define IL_Keyfob_Temp_Dev3_phy_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.Keyfob_Temp_Dev3_phy = (x);\
}

/*  Rx */
#define IL_BTA1_Conn_Sts_Dev3_RX  (BTA1_Dev3_Sts_t.msg.sig.BTA1_Conn_Sts_Dev3)

/*  Tx */
#define IL_BTA1_Conn_Sts_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.BTA1_Conn_Sts_Dev3 = (x);\
}

/*  Rx */
#define IL_Keyfob_Batt_Vtg_Dev3_RX  (\
	(BTA1_Dev3_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev3_0 << 8U) | \
	(BTA1_Dev3_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev3_1))

/* Tx */
#define IL_Keyfob_Batt_Vtg_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev3_0 = ((x) >> 8U) & 255U; \
	BTA1_Dev3_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_UWB_Locn_Interval_Dev3_RX  (\
	(BTA1_Dev3_Sts_t.msg.sig.UWB_Locn_Interval_Dev3_0 << 8U) | \
	(BTA1_Dev3_Sts_t.msg.sig.UWB_Locn_Interval_Dev3_1))

/* Tx */
#define IL_UWB_Locn_Interval_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.UWB_Locn_Interval_Dev3_0 = ((x) >> 8U) & 255U; \
	BTA1_Dev3_Sts_t.msg.sig.UWB_Locn_Interval_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_Keyfob_Batt_Pct_Dev3_RX  (BTA1_Dev3_Sts_t.msg.sig.Keyfob_Batt_Pct_Dev3)

/*  Tx */
#define IL_Keyfob_Batt_Pct_Dev3_TX(x)  \
{ \
	BTA1_Dev3_Sts_t.msg.sig.Keyfob_Batt_Pct_Dev3 = (x);\
}

/*  Rx */
#define IL_SmtKey_MotionSts_Dev4_RX  (BTA1_Dev4_Sts_t.msg.sig.SmtKey_MotionSts_Dev4)

/*  Tx */
#define IL_SmtKey_MotionSts_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.SmtKey_MotionSts_Dev4 = (x);\
}

/*  Rx */
#define IL_SmtKey_MotionSensorSts_Dev4_RX  (BTA1_Dev4_Sts_t.msg.sig.SmtKey_MotionSensorSts_Dev4)

/*  Tx */
#define IL_SmtKey_MotionSensorSts_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.SmtKey_MotionSensorSts_Dev4 = (x);\
}

/*  Rx */
#define IL_SmtKey_ScreenLockSts_Dev4_RX  (BTA1_Dev4_Sts_t.msg.sig.SmtKey_ScreenLockSts_Dev4)

/*  Tx */
#define IL_SmtKey_ScreenLockSts_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.SmtKey_ScreenLockSts_Dev4 = (x);\
}

/*  Rx */
#define IL_SmtKey_PosnMode_Dev4_RX  (BTA1_Dev4_Sts_t.msg.sig.SmtKey_PosnMode_Dev4)

/*  Tx */
#define IL_SmtKey_PosnMode_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.SmtKey_PosnMode_Dev4 = (x);\
}

/*  Rx */
#define IL_SmtKey_DevType_Dev4_RX  (BTA1_Dev4_Sts_t.msg.sig.SmtKey_DevType_Dev4)

/*  Tx */
#define IL_SmtKey_DevType_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.SmtKey_DevType_Dev4 = (x);\
}

/*  Rx */
#define IL_UWB_PredURSK_Sts_Dev4_RX  (BTA1_Dev4_Sts_t.msg.sig.UWB_PredURSK_Sts_Dev4)

/*  Tx */
#define IL_UWB_PredURSK_Sts_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.UWB_PredURSK_Sts_Dev4 = (x);\
}

/*  Rx */
#define IL_Keyfob_IsBattLow_Dev4_RX  (BTA1_Dev4_Sts_t.msg.sig.Keyfob_IsBattLow_Dev4)

/*  Tx */
#define IL_Keyfob_IsBattLow_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.Keyfob_IsBattLow_Dev4 = (x);\
}

/*  Rx */
#define IL_UWB_Locn_Cnt_Dev4_RX  (BTA1_Dev4_Sts_t.msg.sig.UWB_Locn_Cnt_Dev4)

/*  Tx */
#define IL_UWB_Locn_Cnt_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.UWB_Locn_Cnt_Dev4 = (x);\
}

/*  Rx */
#define IL_Keyfob_Temp_Dev4_phy_RX  (BTA1_Dev4_Sts_t.msg.sig.Keyfob_Temp_Dev4_phy)

/*  Tx */
#define IL_Keyfob_Temp_Dev4_phy_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.Keyfob_Temp_Dev4_phy = (x);\
}

/*  Rx */
#define IL_BTA1_Conn_Sts_Dev4_RX  (BTA1_Dev4_Sts_t.msg.sig.BTA1_Conn_Sts_Dev4)

/*  Tx */
#define IL_BTA1_Conn_Sts_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.BTA1_Conn_Sts_Dev4 = (x);\
}

/*  Rx */
#define IL_Keyfob_Batt_Vtg_Dev4_RX  (\
	(BTA1_Dev4_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev4_0 << 8U) | \
	(BTA1_Dev4_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev4_1))

/* Tx */
#define IL_Keyfob_Batt_Vtg_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev4_0 = ((x) >> 8U) & 255U; \
	BTA1_Dev4_Sts_t.msg.sig.Keyfob_Batt_Vtg_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_UWB_Locn_Interval_Dev4_RX  (\
	(BTA1_Dev4_Sts_t.msg.sig.UWB_Locn_Interval_Dev4_0 << 8U) | \
	(BTA1_Dev4_Sts_t.msg.sig.UWB_Locn_Interval_Dev4_1))

/* Tx */
#define IL_UWB_Locn_Interval_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.UWB_Locn_Interval_Dev4_0 = ((x) >> 8U) & 255U; \
	BTA1_Dev4_Sts_t.msg.sig.UWB_Locn_Interval_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_Keyfob_Batt_Pct_Dev4_RX  (BTA1_Dev4_Sts_t.msg.sig.Keyfob_Batt_Pct_Dev4)

/*  Tx */
#define IL_Keyfob_Batt_Pct_Dev4_TX(x)  \
{ \
	BTA1_Dev4_Sts_t.msg.sig.Keyfob_Batt_Pct_Dev4 = (x);\
}

/*  Rx */
#define IL_BTA1_RSSI_Dev1_phy_RX  (BTA1_RSSI_UWB_t.msg.sig.BTA1_RSSI_Dev1_phy)

/*  Tx */
#define IL_BTA1_RSSI_Dev1_phy_TX(x)  \
{ \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_RSSI_Dev1_phy = (x);\
}

/*  Rx */
#define IL_BTA1_RSSI_Dev2_phy_RX  (BTA1_RSSI_UWB_t.msg.sig.BTA1_RSSI_Dev2_phy)

/*  Tx */
#define IL_BTA1_RSSI_Dev2_phy_TX(x)  \
{ \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_RSSI_Dev2_phy = (x);\
}

/*  Rx */
#define IL_BTA1_RSSI_Dev3_phy_RX  (BTA1_RSSI_UWB_t.msg.sig.BTA1_RSSI_Dev3_phy)

/*  Tx */
#define IL_BTA1_RSSI_Dev3_phy_TX(x)  \
{ \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_RSSI_Dev3_phy = (x);\
}

/*  Rx */
#define IL_BTA1_RSSI_Dev4_phy_RX  (BTA1_RSSI_UWB_t.msg.sig.BTA1_RSSI_Dev4_phy)

/*  Tx */
#define IL_BTA1_RSSI_Dev4_phy_TX(x)  \
{ \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_RSSI_Dev4_phy = (x);\
}

/*  Rx */
#define IL_BTA1_RSSI_Dev5_phy_RX  (BTA1_RSSI_UWB_t.msg.sig.BTA1_RSSI_Dev5_phy)

/*  Tx */
#define IL_BTA1_RSSI_Dev5_phy_TX(x)  \
{ \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_RSSI_Dev5_phy = (x);\
}

/*  Rx */
#define IL_BTA1_UWB_Dist_Dev1_RX  (\
	(BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev1_0 << 8U) | \
	(BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev1_1))

/* Tx */
#define IL_BTA1_UWB_Dist_Dev1_TX(x)  \
{ \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev1_0 = ((x) >> 8U) & 255U; \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_UWB_Dist_Dev2_RX  (\
	(BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev2_0 << 8U) | \
	(BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev2_1))

/* Tx */
#define IL_BTA1_UWB_Dist_Dev2_TX(x)  \
{ \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev2_0 = ((x) >> 8U) & 255U; \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_UWB_Dist_Dev3_RX  (\
	(BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev3_0 << 8U) | \
	(BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev3_1))

/* Tx */
#define IL_BTA1_UWB_Dist_Dev3_TX(x)  \
{ \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev3_0 = ((x) >> 8U) & 255U; \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_UWB_Dist_Dev4_RX  (\
	(BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev4_0 << 8U) | \
	(BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev4_1))

/* Tx */
#define IL_BTA1_UWB_Dist_Dev4_TX(x)  \
{ \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev4_0 = ((x) >> 8U) & 255U; \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_UWB_Dist_Dev5_RX  (\
	(BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev5_0 << 8U) | \
	(BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev5_1))

/* Tx */
#define IL_BTA1_UWB_Dist_Dev5_TX(x)  \
{ \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev5_0 = ((x) >> 8U) & 255U; \
	BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA2_RSSI_Dev1_phy_RX  (BTA2_RSSI_UWB_t.msg.sig.BTA2_RSSI_Dev1_phy)

/*  Tx */
#define IL_BTA2_RSSI_Dev1_phy_TX(x)  \
{ \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_RSSI_Dev1_phy = (x);\
}

/*  Rx */
#define IL_BTA2_RSSI_Dev2_phy_RX  (BTA2_RSSI_UWB_t.msg.sig.BTA2_RSSI_Dev2_phy)

/*  Tx */
#define IL_BTA2_RSSI_Dev2_phy_TX(x)  \
{ \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_RSSI_Dev2_phy = (x);\
}

/*  Rx */
#define IL_BTA2_RSSI_Dev3_phy_RX  (BTA2_RSSI_UWB_t.msg.sig.BTA2_RSSI_Dev3_phy)

/*  Tx */
#define IL_BTA2_RSSI_Dev3_phy_TX(x)  \
{ \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_RSSI_Dev3_phy = (x);\
}

/*  Rx */
#define IL_BTA2_RSSI_Dev4_phy_RX  (BTA2_RSSI_UWB_t.msg.sig.BTA2_RSSI_Dev4_phy)

/*  Tx */
#define IL_BTA2_RSSI_Dev4_phy_TX(x)  \
{ \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_RSSI_Dev4_phy = (x);\
}

/*  Rx */
#define IL_BTA2_RSSI_Dev5_phy_RX  (BTA2_RSSI_UWB_t.msg.sig.BTA2_RSSI_Dev5_phy)

/*  Tx */
#define IL_BTA2_RSSI_Dev5_phy_TX(x)  \
{ \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_RSSI_Dev5_phy = (x);\
}

/*  Rx */
#define IL_BTA2_UWB_Dist_Dev1_RX  (\
	(BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev1_0 << 8U) | \
	(BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev1_1))

/* Tx */
#define IL_BTA2_UWB_Dist_Dev1_TX(x)  \
{ \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev1_0 = ((x) >> 8U) & 255U; \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA2_UWB_Dist_Dev2_RX  (\
	(BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev2_0 << 8U) | \
	(BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev2_1))

/* Tx */
#define IL_BTA2_UWB_Dist_Dev2_TX(x)  \
{ \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev2_0 = ((x) >> 8U) & 255U; \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA2_UWB_Dist_Dev3_RX  (\
	(BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev3_0 << 8U) | \
	(BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev3_1))

/* Tx */
#define IL_BTA2_UWB_Dist_Dev3_TX(x)  \
{ \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev3_0 = ((x) >> 8U) & 255U; \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA2_UWB_Dist_Dev4_RX  (\
	(BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev4_0 << 8U) | \
	(BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev4_1))

/* Tx */
#define IL_BTA2_UWB_Dist_Dev4_TX(x)  \
{ \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev4_0 = ((x) >> 8U) & 255U; \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA2_UWB_Dist_Dev5_RX  (\
	(BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev5_0 << 8U) | \
	(BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev5_1))

/* Tx */
#define IL_BTA2_UWB_Dist_Dev5_TX(x)  \
{ \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev5_0 = ((x) >> 8U) & 255U; \
	BTA2_RSSI_UWB_t.msg.sig.BTA2_UWB_Dist_Dev5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA3_RSSI_Dev1_phy_RX  (BTA3_RSSI_UWB_t.msg.sig.BTA3_RSSI_Dev1_phy)

/*  Tx */
#define IL_BTA3_RSSI_Dev1_phy_TX(x)  \
{ \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_RSSI_Dev1_phy = (x);\
}

/*  Rx */
#define IL_BTA3_RSSI_Dev2_phy_RX  (BTA3_RSSI_UWB_t.msg.sig.BTA3_RSSI_Dev2_phy)

/*  Tx */
#define IL_BTA3_RSSI_Dev2_phy_TX(x)  \
{ \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_RSSI_Dev2_phy = (x);\
}

/*  Rx */
#define IL_BTA3_RSSI_Dev3_phy_RX  (BTA3_RSSI_UWB_t.msg.sig.BTA3_RSSI_Dev3_phy)

/*  Tx */
#define IL_BTA3_RSSI_Dev3_phy_TX(x)  \
{ \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_RSSI_Dev3_phy = (x);\
}

/*  Rx */
#define IL_BTA3_RSSI_Dev4_phy_RX  (BTA3_RSSI_UWB_t.msg.sig.BTA3_RSSI_Dev4_phy)

/*  Tx */
#define IL_BTA3_RSSI_Dev4_phy_TX(x)  \
{ \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_RSSI_Dev4_phy = (x);\
}

/*  Rx */
#define IL_BTA3_RSSI_Dev5_phy_RX  (BTA3_RSSI_UWB_t.msg.sig.BTA3_RSSI_Dev5_phy)

/*  Tx */
#define IL_BTA3_RSSI_Dev5_phy_TX(x)  \
{ \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_RSSI_Dev5_phy = (x);\
}

/*  Rx */
#define IL_BTA3_UWB_Dist_Dev1_RX  (\
	(BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev1_0 << 8U) | \
	(BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev1_1))

/* Tx */
#define IL_BTA3_UWB_Dist_Dev1_TX(x)  \
{ \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev1_0 = ((x) >> 8U) & 255U; \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA3_UWB_Dist_Dev2_RX  (\
	(BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev2_0 << 8U) | \
	(BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev2_1))

/* Tx */
#define IL_BTA3_UWB_Dist_Dev2_TX(x)  \
{ \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev2_0 = ((x) >> 8U) & 255U; \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA3_UWB_Dist_Dev3_RX  (\
	(BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev3_0 << 8U) | \
	(BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev3_1))

/* Tx */
#define IL_BTA3_UWB_Dist_Dev3_TX(x)  \
{ \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev3_0 = ((x) >> 8U) & 255U; \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA3_UWB_Dist_Dev4_RX  (\
	(BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev4_0 << 8U) | \
	(BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev4_1))

/* Tx */
#define IL_BTA3_UWB_Dist_Dev4_TX(x)  \
{ \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev4_0 = ((x) >> 8U) & 255U; \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA3_UWB_Dist_Dev5_RX  (\
	(BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev5_0 << 8U) | \
	(BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev5_1))

/* Tx */
#define IL_BTA3_UWB_Dist_Dev5_TX(x)  \
{ \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev5_0 = ((x) >> 8U) & 255U; \
	BTA3_RSSI_UWB_t.msg.sig.BTA3_UWB_Dist_Dev5_1 = (x) & 255U; \
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
#define IL_BTA5_RSSI_Dev1_phy_RX  (BTA5_RSSI_UWB_t.msg.sig.BTA5_RSSI_Dev1_phy)

/*  Tx */
#define IL_BTA5_RSSI_Dev1_phy_TX(x)  \
{ \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_RSSI_Dev1_phy = (x);\
}

/*  Rx */
#define IL_BTA5_RSSI_Dev2_phy_RX  (BTA5_RSSI_UWB_t.msg.sig.BTA5_RSSI_Dev2_phy)

/*  Tx */
#define IL_BTA5_RSSI_Dev2_phy_TX(x)  \
{ \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_RSSI_Dev2_phy = (x);\
}

/*  Rx */
#define IL_BTA5_RSSI_Dev3_phy_RX  (BTA5_RSSI_UWB_t.msg.sig.BTA5_RSSI_Dev3_phy)

/*  Tx */
#define IL_BTA5_RSSI_Dev3_phy_TX(x)  \
{ \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_RSSI_Dev3_phy = (x);\
}

/*  Rx */
#define IL_BTA5_RSSI_Dev4_phy_RX  (BTA5_RSSI_UWB_t.msg.sig.BTA5_RSSI_Dev4_phy)

/*  Tx */
#define IL_BTA5_RSSI_Dev4_phy_TX(x)  \
{ \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_RSSI_Dev4_phy = (x);\
}

/*  Rx */
#define IL_BTA5_RSSI_Dev5_phy_RX  (BTA5_RSSI_UWB_t.msg.sig.BTA5_RSSI_Dev5_phy)

/*  Tx */
#define IL_BTA5_RSSI_Dev5_phy_TX(x)  \
{ \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_RSSI_Dev5_phy = (x);\
}

/*  Rx */
#define IL_BTA5_UWB_Dist_Dev1_RX  (\
	(BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev1_0 << 8U) | \
	(BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev1_1))

/* Tx */
#define IL_BTA5_UWB_Dist_Dev1_TX(x)  \
{ \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev1_0 = ((x) >> 8U) & 255U; \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA5_UWB_Dist_Dev2_RX  (\
	(BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev2_0 << 8U) | \
	(BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev2_1))

/* Tx */
#define IL_BTA5_UWB_Dist_Dev2_TX(x)  \
{ \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev2_0 = ((x) >> 8U) & 255U; \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA5_UWB_Dist_Dev3_RX  (\
	(BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev3_0 << 8U) | \
	(BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev3_1))

/* Tx */
#define IL_BTA5_UWB_Dist_Dev3_TX(x)  \
{ \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev3_0 = ((x) >> 8U) & 255U; \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA5_UWB_Dist_Dev4_RX  (\
	(BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev4_0 << 8U) | \
	(BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev4_1))

/* Tx */
#define IL_BTA5_UWB_Dist_Dev4_TX(x)  \
{ \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev4_0 = ((x) >> 8U) & 255U; \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA5_UWB_Dist_Dev5_RX  (\
	(BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev5_0 << 8U) | \
	(BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev5_1))

/* Tx */
#define IL_BTA5_UWB_Dist_Dev5_TX(x)  \
{ \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev5_0 = ((x) >> 8U) & 255U; \
	BTA5_RSSI_UWB_t.msg.sig.BTA5_UWB_Dist_Dev5_1 = (x) & 255U; \
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
#define IL_BtaOutputRfData_CRC_RX  (\
	(BtaOutputRfData_t.msg.sig.BtaOutputRfData_CRC_0 << 8U) | \
	(BtaOutputRfData_t.msg.sig.BtaOutputRfData_CRC_1))

/* Tx */
#define IL_BtaOutputRfData_CRC_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.BtaOutputRfData_CRC_0 = ((x) >> 8U) & 255U; \
	BtaOutputRfData_t.msg.sig.BtaOutputRfData_CRC_1 = (x) & 255U; \
}

/*  Rx */
#define IL_Handbyte_RX  (BtaOutputRfData_t.msg.sig.Handbyte)

/*  Tx */
#define IL_Handbyte_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.Handbyte = (x);\
}

/*  Rx */
#define IL_BalLbtChoose_RX  (BtaOutputRfData_t.msg.sig.BalLbtChoose)

/*  Tx */
#define IL_BalLbtChoose_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.BalLbtChoose = (x);\
}

/*  Rx */
#define IL_Lef_Right_RX  (BtaOutputRfData_t.msg.sig.Lef_Right)

/*  Tx */
#define IL_Lef_Right_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.Lef_Right = (x);\
}

/*  Rx */
#define IL_FrameNumber_RX  (BtaOutputRfData_t.msg.sig.FrameNumber)

/*  Tx */
#define IL_FrameNumber_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.FrameNumber = (x);\
}

/*  Rx */
#define IL_SensorBALSts_RX  (BtaOutputRfData_t.msg.sig.SensorBALSts)

/*  Tx */
#define IL_SensorBALSts_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.SensorBALSts = (x);\
}

/*  Rx */
#define IL_WorkMode_RX  (BtaOutputRfData_t.msg.sig.WorkMode)

/*  Tx */
#define IL_WorkMode_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.WorkMode = (x);\
}

/*  Rx */
#define IL_AlartSts_RX  (BtaOutputRfData_t.msg.sig.AlartSts)

/*  Tx */
#define IL_AlartSts_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.AlartSts = (x);\
}

/*  Rx */
#define IL_LowBattery_RX  (BtaOutputRfData_t.msg.sig.LowBattery)

/*  Tx */
#define IL_LowBattery_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.LowBattery = (x);\
}

/*  Rx */
#define IL_AdvanceFunctionCode_RX  (BtaOutputRfData_t.msg.sig.AdvanceFunctionCode)

/*  Tx */
#define IL_AdvanceFunctionCode_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.AdvanceFunctionCode = (x);\
}

/*  Rx */
#define IL_AdvFunc0_RX  (BtaOutputRfData_t.msg.sig.AdvFunc0)

/*  Tx */
#define IL_AdvFunc0_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.AdvFunc0 = (x);\
}

/*  Rx */
#define IL_sensorID3_RX  (BtaOutputRfData_t.msg.sig.sensorID3)

/*  Tx */
#define IL_sensorID3_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.sensorID3 = (x);\
}

/*  Rx */
#define IL_sensorID2_RX  (BtaOutputRfData_t.msg.sig.sensorID2)

/*  Tx */
#define IL_sensorID2_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.sensorID2 = (x);\
}

/*  Rx */
#define IL_sensorID1_RX  (BtaOutputRfData_t.msg.sig.sensorID1)

/*  Tx */
#define IL_sensorID1_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.sensorID1 = (x);\
}

/*  Rx */
#define IL_SensorID0_RX  (BtaOutputRfData_t.msg.sig.SensorID0)

/*  Tx */
#define IL_SensorID0_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.SensorID0 = (x);\
}

/*  Rx */
#define IL_CurrentTPMSP_phy_RX  (BtaOutputRfData_t.msg.sig.CurrentTPMSP_phy)

/*  Tx */
#define IL_CurrentTPMSP_phy_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.CurrentTPMSP_phy = (x);\
}

/*  Rx */
#define IL_CurrentTPMST_phy_RX  (BtaOutputRfData_t.msg.sig.CurrentTPMST_phy)

/*  Tx */
#define IL_CurrentTPMST_phy_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.CurrentTPMST_phy = (x);\
}

/*  Rx */
#define IL_RFSignalRC_RX  (BtaOutputRfData_t.msg.sig.RFSignalRC)

/*  Tx */
#define IL_RFSignalRC_TX(x)  \
{ \
	BtaOutputRfData_t.msg.sig.RFSignalRC = (x);\
}

/*  Rx */
#define IL_FrameTime_RX  (BtaOutputRfData1_t.msg.sig.FrameTime)

/*  Tx */
#define IL_FrameTime_TX(x)  \
{ \
	BtaOutputRfData1_t.msg.sig.FrameTime = (x);\
}

/*  Rx */
#define IL_RFFrameTimeRC_RX  (BtaOutputRfData1_t.msg.sig.RFFrameTimeRC)

/*  Tx */
#define IL_RFFrameTimeRC_TX(x)  \
{ \
	BtaOutputRfData1_t.msg.sig.RFFrameTimeRC = (x);\
}

/*  Rx */
#define IL_UWBPrestartRsp_Sf_RX  (BTA_SRF_295_t.msg.sig.UWBPrestartRsp_Sf)

/*  Tx */
#define IL_UWBPrestartRsp_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.UWBPrestartRsp_Sf = (x);\
}

/*  Rx */
#define IL_LDTAftRnTimRsp_Sf_phy_RX  (BTA_SRF_295_t.msg.sig.LDTAftRnTimRsp_Sf_phy)

/*  Tx */
#define IL_LDTAftRnTimRsp_Sf_phy_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.LDTAftRnTimRsp_Sf_phy = (x);\
}

/*  Rx */
#define IL_AdltBrthRt_Sf_RX  (BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf)

/*  Tx */
#define IL_AdltBrthRt_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.AdltBrthRt_Sf = (x);\
}

/*  Rx */
#define IL_ChildBrthRt_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf)

/*  Tx */
#define IL_ChildBrthRt_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildBrthRt_Sf = (x);\
}

/*  Rx */
#define IL_LDTDetCyclRsp_Sf_RX  (BTA_SRF_295_t.msg.sig.LDTDetCyclRsp_Sf)

/*  Tx */
#define IL_LDTDetCyclRsp_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.LDTDetCyclRsp_Sf = (x);\
}

/*  Rx */
#define IL_LDTWkEnRsp_Sf_RX  (BTA_SRF_295_t.msg.sig.LDTWkEnRsp_Sf)

/*  Tx */
#define IL_LDTWkEnRsp_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.LDTWkEnRsp_Sf = (x);\
}

/*  Rx */
#define IL_ChildMotnSts_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildMotnSts_Sf)

/*  Tx */
#define IL_ChildMotnSts_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildMotnSts_Sf = (x);\
}

/*  Rx */
#define IL_ChildPysclSts_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildPysclSts_Sf)

/*  Tx */
#define IL_ChildPysclSts_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildPysclSts_Sf = (x);\
}

/*  Rx */
#define IL_ChildLDTRslt_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf)

/*  Tx */
#define IL_ChildLDTRslt_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildLDTRslt_Sf = (x);\
}

/*  Rx */
#define IL_ChrgrGunGateOpenReq_RX  (BTA_SRF_295_t.msg.sig.ChrgrGunGateOpenReq)

/*  Tx */
#define IL_ChrgrGunGateOpenReq_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChrgrGunGateOpenReq = (x);\
}

/*  Rx */
#define IL_ChildLctnStsOval_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildLctnStsOval_Sf)

/*  Tx */
#define IL_ChildLctnStsOval_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildLctnStsOval_Sf = (x);\
}

/*  Rx */
#define IL_ChildLctnStsFL_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildLctnStsFL_Sf)

/*  Tx */
#define IL_ChildLctnStsFL_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildLctnStsFL_Sf = (x);\
}

/*  Rx */
#define IL_ChildLctnStsFR_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildLctnStsFR_Sf)

/*  Tx */
#define IL_ChildLctnStsFR_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildLctnStsFR_Sf = (x);\
}

/*  Rx */
#define IL_ChildLctnSts2dL_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildLctnSts2dL_Sf)

/*  Tx */
#define IL_ChildLctnSts2dL_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildLctnSts2dL_Sf = (x);\
}

/*  Rx */
#define IL_ChildLctnSts2dM_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildLctnSts2dM_Sf)

/*  Tx */
#define IL_ChildLctnSts2dM_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildLctnSts2dM_Sf = (x);\
}

/*  Rx */
#define IL_ChildLctnSts2dR_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildLctnSts2dR_Sf)

/*  Tx */
#define IL_ChildLctnSts2dR_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildLctnSts2dR_Sf = (x);\
}

/*  Rx */
#define IL_ChildLctnSts3dL_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildLctnSts3dL_Sf)

/*  Tx */
#define IL_ChildLctnSts3dL_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildLctnSts3dL_Sf = (x);\
}

/*  Rx */
#define IL_ChildLctnSts3dR_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildLctnSts3dR_Sf)

/*  Tx */
#define IL_ChildLctnSts3dR_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildLctnSts3dR_Sf = (x);\
}

/*  Rx */
#define IL_AdltLctnStsOval_Sf_RX  (BTA_SRF_295_t.msg.sig.AdltLctnStsOval_Sf)

/*  Tx */
#define IL_AdltLctnStsOval_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.AdltLctnStsOval_Sf = (x);\
}

/*  Rx */
#define IL_AdltLctnStsFL_Sf_RX  (BTA_SRF_295_t.msg.sig.AdltLctnStsFL_Sf)

/*  Tx */
#define IL_AdltLctnStsFL_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.AdltLctnStsFL_Sf = (x);\
}

/*  Rx */
#define IL_AdltLctnStsFR_Sf_RX  (BTA_SRF_295_t.msg.sig.AdltLctnStsFR_Sf)

/*  Tx */
#define IL_AdltLctnStsFR_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.AdltLctnStsFR_Sf = (x);\
}

/*  Rx */
#define IL_AdltLctnSts2dL_Sf_RX  (BTA_SRF_295_t.msg.sig.AdltLctnSts2dL_Sf)

/*  Tx */
#define IL_AdltLctnSts2dL_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.AdltLctnSts2dL_Sf = (x);\
}

/*  Rx */
#define IL_AdltLctnSts2dM_Sf_RX  (BTA_SRF_295_t.msg.sig.AdltLctnSts2dM_Sf)

/*  Tx */
#define IL_AdltLctnSts2dM_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.AdltLctnSts2dM_Sf = (x);\
}

/*  Rx */
#define IL_AdltLctnSts2dR_Sf_RX  (BTA_SRF_295_t.msg.sig.AdltLctnSts2dR_Sf)

/*  Tx */
#define IL_AdltLctnSts2dR_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.AdltLctnSts2dR_Sf = (x);\
}

/*  Rx */
#define IL_AdltLctnSts3dL_Sf_RX  (BTA_SRF_295_t.msg.sig.AdltLctnSts3dL_Sf)

/*  Tx */
#define IL_AdltLctnSts3dL_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.AdltLctnSts3dL_Sf = (x);\
}

/*  Rx */
#define IL_AdltLctnSts3dR_Sf_RX  (BTA_SRF_295_t.msg.sig.AdltLctnSts3dR_Sf)

/*  Tx */
#define IL_AdltLctnSts3dR_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.AdltLctnSts3dR_Sf = (x);\
}

/*  Rx */
#define IL_UWBLDTEnRsp_Sf_RX  (BTA_SRF_295_t.msg.sig.UWBLDTEnRsp_Sf)

/*  Tx */
#define IL_UWBLDTEnRsp_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.UWBLDTEnRsp_Sf = (x);\
}

/*  Rx */
#define IL_UWBLDTRstRsp_Sf_RX  (BTA_SRF_295_t.msg.sig.UWBLDTRstRsp_Sf)

/*  Tx */
#define IL_UWBLDTRstRsp_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.UWBLDTRstRsp_Sf = (x);\
}

/*  Rx */
#define IL_ChildLDTAgeRange_Sf_RX  (BTA_SRF_295_t.msg.sig.ChildLDTAgeRange_Sf)

/*  Tx */
#define IL_ChildLDTAgeRange_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.ChildLDTAgeRange_Sf = (x);\
}

/*  Rx */
#define IL_LDTBackNoiseLrnRsp_Sf_RX  (BTA_SRF_295_t.msg.sig.LDTBackNoiseLrnRsp_Sf)

/*  Tx */
#define IL_LDTBackNoiseLrnRsp_Sf_TX(x)  \
{ \
	BTA_SRF_295_t.msg.sig.LDTBackNoiseLrnRsp_Sf = (x);\
}

/*  Rx */
#define IL_LDTBackNoiseLrnCmd_Sf_RX  (ZONE_296_t.msg.sig.LDTBackNoiseLrnCmd_Sf)

/*  Tx */
#define IL_LDTBackNoiseLrnCmd_Sf_TX(x)  \
{ \
	ZONE_296_t.msg.sig.LDTBackNoiseLrnCmd_Sf = (x);\
}

/*  Rx */
#define IL_UWBPrestart_Sf_RX  (ZONE_296_t.msg.sig.UWBPrestart_Sf)

/*  Tx */
#define IL_UWBPrestart_Sf_TX(x)  \
{ \
	ZONE_296_t.msg.sig.UWBPrestart_Sf = (x);\
}

/*  Rx */
#define IL_UWBLDTEnReq_Sf_RX  (ZONE_296_t.msg.sig.UWBLDTEnReq_Sf)

/*  Tx */
#define IL_UWBLDTEnReq_Sf_TX(x)  \
{ \
	ZONE_296_t.msg.sig.UWBLDTEnReq_Sf = (x);\
}

/*  Rx */
#define IL_UWBLDTRstCmd_Sf_RX  (ZONE_296_t.msg.sig.UWBLDTRstCmd_Sf)

/*  Tx */
#define IL_UWBLDTRstCmd_Sf_TX(x)  \
{ \
	ZONE_296_t.msg.sig.UWBLDTRstCmd_Sf = (x);\
}

/*  Rx */
#define IL_LDTAftRnTimReq_Sf_phy_RX  (ZONE_296_t.msg.sig.LDTAftRnTimReq_Sf_phy)

/*  Tx */
#define IL_LDTAftRnTimReq_Sf_phy_TX(x)  \
{ \
	ZONE_296_t.msg.sig.LDTAftRnTimReq_Sf_phy = (x);\
}

/*  Rx */
#define IL_LDTDetCyclReq_Sf_RX  (ZONE_296_t.msg.sig.LDTDetCyclReq_Sf)

/*  Tx */
#define IL_LDTDetCyclReq_Sf_TX(x)  \
{ \
	ZONE_296_t.msg.sig.LDTDetCyclReq_Sf = (x);\
}

/*  Rx */
#define IL_LDTWkEnReq_Sf_RX  (ZONE_296_t.msg.sig.LDTWkEnReq_Sf)

/*  Tx */
#define IL_LDTWkEnReq_Sf_TX(x)  \
{ \
	ZONE_296_t.msg.sig.LDTWkEnReq_Sf = (x);\
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
#define IL_ECUPowSuppVol_BTA1_RX  (BTA1_01_t.msg.sig.ECUPowSuppVol_BTA1)

/*  Tx */
#define IL_ECUPowSuppVol_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.ECUPowSuppVol_BTA1 = (x);\
}

/*  Rx */
#define IL_FlashingSts_BTA1_RX  (BTA1_01_t.msg.sig.FlashingSts_BTA1)

/*  Tx */
#define IL_FlashingSts_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.FlashingSts_BTA1 = (x);\
}

/*  Rx */
#define IL_RSSAbnormalSts_BTA1_RX  (BTA1_01_t.msg.sig.RSSAbnormalSts_BTA1)

/*  Tx */
#define IL_RSSAbnormalSts_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.RSSAbnormalSts_BTA1 = (x);\
}

/*  Rx */
#define IL_CANNetwork_BTA1_RX  (BTA1_01_t.msg.sig.CANNetwork_BTA1)

/*  Tx */
#define IL_CANNetwork_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.CANNetwork_BTA1 = (x);\
}

/*  Rx */
#define IL_OS_BTA1_RX  (BTA1_01_t.msg.sig.OS_BTA1)

/*  Tx */
#define IL_OS_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.OS_BTA1 = (x);\
}

/*  Rx */
#define IL_WUKeepSource_BTA1_RX  (BTA1_01_t.msg.sig.WUKeepSource_BTA1)

/*  Tx */
#define IL_WUKeepSource_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.WUKeepSource_BTA1 = (x);\
}

/*  Rx */
#define IL_WUSource_BTA1_RX  (BTA1_01_t.msg.sig.WUSource_BTA1)

/*  Tx */
#define IL_WUSource_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.WUSource_BTA1 = (x);\
}

/*  Rx */
#define IL_SoftReset_BTA1_RX  (BTA1_01_t.msg.sig.SoftReset_BTA1)

/*  Tx */
#define IL_SoftReset_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.SoftReset_BTA1 = (x);\
}

/*  Rx */
#define IL_HardReset_BTA1_RX  (BTA1_01_t.msg.sig.HardReset_BTA1)

/*  Tx */
#define IL_HardReset_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.HardReset_BTA1 = (x);\
}

/*  Rx */
#define IL_WD_BTA1_RX  (BTA1_01_t.msg.sig.WD_BTA1)

/*  Tx */
#define IL_WD_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.WD_BTA1 = (x);\
}

/*  Rx */
#define IL_CommDTCs_BTA1_RX  (BTA1_01_t.msg.sig.CommDTCs_BTA1)

/*  Tx */
#define IL_CommDTCs_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.CommDTCs_BTA1 = (x);\
}

/*  Rx */
#define IL_MCUSoCFault_BTA1_RX  (BTA1_01_t.msg.sig.MCUSoCFault_BTA1)

/*  Tx */
#define IL_MCUSoCFault_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.MCUSoCFault_BTA1 = (x);\
}

/*  Rx */
#define IL_MCUSoCSPICom_BTA1_RX  (BTA1_01_t.msg.sig.MCUSoCSPICom_BTA1)

/*  Tx */
#define IL_MCUSoCSPICom_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.MCUSoCSPICom_BTA1 = (x);\
}

/*  Rx */
#define IL_MCUSoCTemp_BTA1_RX  (BTA1_01_t.msg.sig.MCUSoCTemp_BTA1)

/*  Tx */
#define IL_MCUSoCTemp_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.MCUSoCTemp_BTA1 = (x);\
}

/*  Rx */
#define IL_PeripheralChipTempOne_BTA1_RX  (BTA1_01_t.msg.sig.PeripheralChipTempOne_BTA1)

/*  Tx */
#define IL_PeripheralChipTempOne_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.PeripheralChipTempOne_BTA1 = (x);\
}

/*  Rx */
#define IL_PeripheralChipTempTwo_BTA1_RX  (BTA1_01_t.msg.sig.PeripheralChipTempTwo_BTA1)

/*  Tx */
#define IL_PeripheralChipTempTwo_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.PeripheralChipTempTwo_BTA1 = (x);\
}

/*  Rx */
#define IL_Others_BTA1_RX  (BTA1_01_t.msg.sig.Others_BTA1)

/*  Tx */
#define IL_Others_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.Others_BTA1 = (x);\
}

/*  Rx */
#define IL_SBCFault_BTA1_RX  (BTA1_01_t.msg.sig.SBCFault_BTA1)

/*  Tx */
#define IL_SBCFault_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.SBCFault_BTA1 = (x);\
}

/*  Rx */
#define IL_Peripheral_Fault_BTA1_RX  (BTA1_01_t.msg.sig.Peripheral_Fault_BTA1)

/*  Tx */
#define IL_Peripheral_Fault_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.Peripheral_Fault_BTA1 = (x);\
}

/*  Rx */
#define IL_EcuCRstatus_BTA1_RX  (BTA1_01_t.msg.sig.EcuCRstatus_BTA1)

/*  Tx */
#define IL_EcuCRstatus_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.EcuCRstatus_BTA1 = (x);\
}

/*  Rx */
#define IL_PowerSupplyDTCs_BTA1_RX  (BTA1_01_t.msg.sig.PowerSupplyDTCs_BTA1)

/*  Tx */
#define IL_PowerSupplyDTCs_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.PowerSupplyDTCs_BTA1 = (x);\
}

/*  Rx */
#define IL_RFsysTDmode_BTA1_RX  (BTA1_01_t.msg.sig.RFsysTDmode_BTA1)

/*  Tx */
#define IL_RFsysTDmode_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.RFsysTDmode_BTA1 = (x);\
}

/*  Rx */
#define IL_Rfsendermode_BTA1_RX  (BTA1_01_t.msg.sig.Rfsendermode_BTA1)

/*  Tx */
#define IL_Rfsendermode_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.Rfsendermode_BTA1 = (x);\
}

/*  Rx */
#define IL_Rfreceivermode_BTA1_RX  (BTA1_01_t.msg.sig.Rfreceivermode_BTA1)

/*  Tx */
#define IL_Rfreceivermode_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.Rfreceivermode_BTA1 = (x);\
}

/*  Rx */
#define IL_RfC1Rvalue_BTA1_RX  (BTA1_01_t.msg.sig.RfC1Rvalue_BTA1)

/*  Tx */
#define IL_RfC1Rvalue_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.RfC1Rvalue_BTA1 = (x);\
}

/*  Rx */
#define IL_RfC2Rvalue_BTA1_RX  (BTA1_01_t.msg.sig.RfC2Rvalue_BTA1)

/*  Tx */
#define IL_RfC2Rvalue_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.RfC2Rvalue_BTA1 = (x);\
}

/*  Rx */
#define IL_ECUAuthProstatus_BTA1_RX  (BTA1_01_t.msg.sig.ECUAuthProstatus_BTA1)

/*  Tx */
#define IL_ECUAuthProstatus_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.ECUAuthProstatus_BTA1 = (x);\
}

/*  Rx */
#define IL_ECUAuthFailReasons_BTA1_RX  (BTA1_01_t.msg.sig.ECUAuthFailReasons_BTA1)

/*  Tx */
#define IL_ECUAuthFailReasons_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.ECUAuthFailReasons_BTA1 = (x);\
}

/*  Rx */
#define IL_InterEEPTimes_BTA1_RX  (BTA1_01_t.msg.sig.InterEEPTimes_BTA1)

/*  Tx */
#define IL_InterEEPTimes_BTA1_TX(x)  \
{ \
	BTA1_01_t.msg.sig.InterEEPTimes_BTA1 = (x);\
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
#define IL_TpmsFilterID1_RX  (\
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID1_0 << 24U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID1_1 << 16U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID1_2 << 8U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID1_3))

/* Tx */
#define IL_TpmsFilterID1_TX(x)  \
{ \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID1_0 = ((x) >> 24U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID1_1 = ((x) >> 16U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID1_2 = ((x) >> 8U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_TpmsFilterID2_RX  (\
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID2_0 << 24U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID2_1 << 16U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID2_2 << 8U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID2_3))

/* Tx */
#define IL_TpmsFilterID2_TX(x)  \
{ \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID2_0 = ((x) >> 24U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID2_1 = ((x) >> 16U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID2_2 = ((x) >> 8U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID2_3 = (x) & 255U; \
}

/*  Rx */
#define IL_TpmsFilterID3_RX  (\
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID3_0 << 24U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID3_1 << 16U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID3_2 << 8U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID3_3))

/* Tx */
#define IL_TpmsFilterID3_TX(x)  \
{ \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID3_0 = ((x) >> 24U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID3_1 = ((x) >> 16U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID3_2 = ((x) >> 8U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_TpmsFilterID4_RX  (\
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID4_0 << 24U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID4_1 << 16U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID4_2 << 8U) | \
	(ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID4_3))

/* Tx */
#define IL_TpmsFilterID4_TX(x)  \
{ \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID4_0 = ((x) >> 24U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID4_1 = ((x) >> 16U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID4_2 = ((x) >> 8U) & 255U; \
	ZONE_L_TPMS_01_t.msg.sig.TpmsFilterID4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Level_RX  (BTA1_Log_Data_t.msg.sig.BTA1_Log_Level)

/*  Tx */
#define IL_BTA1_Log_Level_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Level = (x);\
}

/*  Rx */
#define IL_BTA1_Log_Sender_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Sender_0 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Sender_1 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Sender_2))

/* Tx */
#define IL_BTA1_Log_Sender_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Sender_0 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Sender_1 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Sender_2 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data1_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data1_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data1_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data1_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data1_3))

/* Tx */
#define IL_BTA1_Log_Data1_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data1_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data1_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data1_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data2_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data2_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data2_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data2_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data2_3))

/* Tx */
#define IL_BTA1_Log_Data2_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data2_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data2_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data2_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data2_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data3_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data3_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data3_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data3_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data3_3))

/* Tx */
#define IL_BTA1_Log_Data3_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data3_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data3_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data3_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data4_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data4_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data4_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data4_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data4_3))

/* Tx */
#define IL_BTA1_Log_Data4_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data4_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data4_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data4_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data5_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data5_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data5_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data5_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data5_3))

/* Tx */
#define IL_BTA1_Log_Data5_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data5_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data5_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data5_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data5_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data6_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data6_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data6_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data6_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data6_3))

/* Tx */
#define IL_BTA1_Log_Data6_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data6_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data6_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data6_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data6_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data7_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data7_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data7_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data7_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data7_3))

/* Tx */
#define IL_BTA1_Log_Data7_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data7_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data7_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data7_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data7_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data8_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data8_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data8_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data8_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data8_3))

/* Tx */
#define IL_BTA1_Log_Data8_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data8_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data8_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data8_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data9_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data9_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data9_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data9_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data9_3))

/* Tx */
#define IL_BTA1_Log_Data9_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data9_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data9_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data9_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data9_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data10_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data10_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data10_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data10_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data10_3))

/* Tx */
#define IL_BTA1_Log_Data10_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data10_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data10_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data10_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data10_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data11_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data11_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data11_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data11_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data11_3))

/* Tx */
#define IL_BTA1_Log_Data11_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data11_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data11_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data11_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data11_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data12_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data12_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data12_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data12_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data12_3))

/* Tx */
#define IL_BTA1_Log_Data12_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data12_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data12_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data12_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data13_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data13_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data13_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data13_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data13_3))

/* Tx */
#define IL_BTA1_Log_Data13_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data13_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data13_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data13_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data13_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data14_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data14_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data14_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data14_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data14_3))

/* Tx */
#define IL_BTA1_Log_Data14_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data14_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data14_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data14_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data14_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA1_Log_Data15_RX  (\
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data15_0 << 24U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data15_1 << 16U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data15_2 << 8U) | \
	(BTA1_Log_Data_t.msg.sig.BTA1_Log_Data15_3))

/* Tx */
#define IL_BTA1_Log_Data15_TX(x)  \
{ \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data15_0 = ((x) >> 24U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data15_1 = ((x) >> 16U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data15_2 = ((x) >> 8U) & 255U; \
	BTA1_Log_Data_t.msg.sig.BTA1_Log_Data15_3 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmd_7_RX  (\
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_0 << 56U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_1 << 48U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_2 << 40U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_3 << 32U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_4 << 24U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_5 << 16U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_6 << 8U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_7))

/* Tx */
#define IL_CccDkBleCmd_7_TX(x)  \
{ \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmd_6_RX  (\
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_0 << 56U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_1 << 48U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_2 << 40U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_3 << 32U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_4 << 24U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_5 << 16U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_6 << 8U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_7))

/* Tx */
#define IL_CccDkBleCmd_6_TX(x)  \
{ \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmd_5_RX  (\
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_0 << 56U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_1 << 48U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_2 << 40U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_3 << 32U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_4 << 24U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_5 << 16U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_6 << 8U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_7))

/* Tx */
#define IL_CccDkBleCmd_5_TX(x)  \
{ \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmd_4_RX  (\
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_0 << 56U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_1 << 48U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_2 << 40U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_3 << 32U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_4 << 24U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_5 << 16U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_6 << 8U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_7))

/* Tx */
#define IL_CccDkBleCmd_4_TX(x)  \
{ \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmd_3_RX  (\
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_0 << 56U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_1 << 48U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_2 << 40U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_3 << 32U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_4 << 24U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_5 << 16U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_6 << 8U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_7))

/* Tx */
#define IL_CccDkBleCmd_3_TX(x)  \
{ \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmd_2_RX  (\
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_0 << 56U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_1 << 48U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_2 << 40U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_3 << 32U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_4 << 24U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_5 << 16U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_6 << 8U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_7))

/* Tx */
#define IL_CccDkBleCmd_2_TX(x)  \
{ \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmd_1_RX  (\
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_0 << 56U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_1 << 48U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_2 << 40U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_3 << 32U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_4 << 24U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_5 << 16U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_6 << 8U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_7))

/* Tx */
#define IL_CccDkBleCmd_1_TX(x)  \
{ \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmd_0_RX  (\
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_0 << 56U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_1 << 48U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_2 << 40U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_3 << 32U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_4 << 24U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_5 << 16U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_6 << 8U) | \
	(CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_7))

/* Tx */
#define IL_CccDkBleCmd_0_TX(x)  \
{ \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmd_t.msg.sig.CccDkBleCmd_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmdFlowCtrl_7_RX  (\
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_0 << 56U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_1 << 48U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_2 << 40U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_3 << 32U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_4 << 24U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_5 << 16U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_6 << 8U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_7))

/* Tx */
#define IL_CccDkBleCmdFlowCtrl_7_TX(x)  \
{ \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmdFlowCtrl_6_RX  (\
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_0 << 56U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_1 << 48U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_2 << 40U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_3 << 32U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_4 << 24U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_5 << 16U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_6 << 8U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_7))

/* Tx */
#define IL_CccDkBleCmdFlowCtrl_6_TX(x)  \
{ \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmdFlowCtrl_5_RX  (\
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_0 << 56U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_1 << 48U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_2 << 40U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_3 << 32U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_4 << 24U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_5 << 16U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_6 << 8U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_7))

/* Tx */
#define IL_CccDkBleCmdFlowCtrl_5_TX(x)  \
{ \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmdFlowCtrl_4_RX  (\
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_0 << 56U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_1 << 48U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_2 << 40U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_3 << 32U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_4 << 24U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_5 << 16U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_6 << 8U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_7))

/* Tx */
#define IL_CccDkBleCmdFlowCtrl_4_TX(x)  \
{ \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmdFlowCtrl_3_RX  (\
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_0 << 56U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_1 << 48U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_2 << 40U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_3 << 32U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_4 << 24U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_5 << 16U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_6 << 8U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_7))

/* Tx */
#define IL_CccDkBleCmdFlowCtrl_3_TX(x)  \
{ \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmdFlowCtrl_2_RX  (\
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_0 << 56U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_1 << 48U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_2 << 40U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_3 << 32U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_4 << 24U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_5 << 16U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_6 << 8U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_7))

/* Tx */
#define IL_CccDkBleCmdFlowCtrl_2_TX(x)  \
{ \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmdFlowCtrl_1_RX  (\
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_0 << 56U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_1 << 48U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_2 << 40U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_3 << 32U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_4 << 24U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_5 << 16U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_6 << 8U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_7))

/* Tx */
#define IL_CccDkBleCmdFlowCtrl_1_TX(x)  \
{ \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleCmdFlowCtrl_0_RX  (\
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_0 << 56U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_1 << 48U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_2 << 40U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_3 << 32U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_4 << 24U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_5 << 16U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_6 << 8U) | \
	(CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_7))

/* Tx */
#define IL_CccDkBleCmdFlowCtrl_0_TX(x)  \
{ \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_0 = ((x) >> 56U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_1 = ((x) >> 48U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_2 = ((x) >> 40U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_3 = ((x) >> 32U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_4 = ((x) >> 24U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_5 = ((x) >> 16U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_6 = ((x) >> 8U) & 255U; \
	CccDkBleCmdFlowCtrl_t.msg.sig.CccDkBleCmdFlowCtrl_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRsp_DATA_7_RX  (\
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_0 << 56U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_1 << 48U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_2 << 40U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_3 << 32U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_4 << 24U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_5 << 16U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_6 << 8U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_7))

/* Tx */
#define IL_CccDkBleRsp_DATA_7_TX(x)  \
{ \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_0 = ((x) >> 56U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_1 = ((x) >> 48U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_2 = ((x) >> 40U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_3 = ((x) >> 32U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_4 = ((x) >> 24U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_5 = ((x) >> 16U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_6 = ((x) >> 8U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRsp_DATA_6_RX  (\
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_0 << 56U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_1 << 48U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_2 << 40U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_3 << 32U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_4 << 24U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_5 << 16U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_6 << 8U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_7))

/* Tx */
#define IL_CccDkBleRsp_DATA_6_TX(x)  \
{ \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_0 = ((x) >> 56U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_1 = ((x) >> 48U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_2 = ((x) >> 40U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_3 = ((x) >> 32U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_4 = ((x) >> 24U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_5 = ((x) >> 16U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_6 = ((x) >> 8U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRsp_DATA_5_RX  (\
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_0 << 56U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_1 << 48U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_2 << 40U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_3 << 32U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_4 << 24U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_5 << 16U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_6 << 8U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_7))

/* Tx */
#define IL_CccDkBleRsp_DATA_5_TX(x)  \
{ \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_0 = ((x) >> 56U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_1 = ((x) >> 48U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_2 = ((x) >> 40U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_3 = ((x) >> 32U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_4 = ((x) >> 24U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_5 = ((x) >> 16U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_6 = ((x) >> 8U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRsp_DATA_4_RX  (\
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_0 << 56U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_1 << 48U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_2 << 40U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_3 << 32U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_4 << 24U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_5 << 16U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_6 << 8U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_7))

/* Tx */
#define IL_CccDkBleRsp_DATA_4_TX(x)  \
{ \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_0 = ((x) >> 56U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_1 = ((x) >> 48U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_2 = ((x) >> 40U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_3 = ((x) >> 32U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_4 = ((x) >> 24U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_5 = ((x) >> 16U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_6 = ((x) >> 8U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRsp_DATA_3_RX  (\
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_0 << 56U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_1 << 48U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_2 << 40U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_3 << 32U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_4 << 24U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_5 << 16U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_6 << 8U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_7))

/* Tx */
#define IL_CccDkBleRsp_DATA_3_TX(x)  \
{ \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_0 = ((x) >> 56U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_1 = ((x) >> 48U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_2 = ((x) >> 40U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_3 = ((x) >> 32U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_4 = ((x) >> 24U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_5 = ((x) >> 16U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_6 = ((x) >> 8U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRsp_DATA_2_RX  (\
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_0 << 56U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_1 << 48U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_2 << 40U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_3 << 32U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_4 << 24U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_5 << 16U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_6 << 8U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_7))

/* Tx */
#define IL_CccDkBleRsp_DATA_2_TX(x)  \
{ \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_0 = ((x) >> 56U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_1 = ((x) >> 48U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_2 = ((x) >> 40U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_3 = ((x) >> 32U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_4 = ((x) >> 24U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_5 = ((x) >> 16U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_6 = ((x) >> 8U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRsp_DATA_1_RX  (\
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_0 << 56U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_1 << 48U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_2 << 40U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_3 << 32U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_4 << 24U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_5 << 16U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_6 << 8U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_7))

/* Tx */
#define IL_CccDkBleRsp_DATA_1_TX(x)  \
{ \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_0 = ((x) >> 56U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_1 = ((x) >> 48U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_2 = ((x) >> 40U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_3 = ((x) >> 32U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_4 = ((x) >> 24U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_5 = ((x) >> 16U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_6 = ((x) >> 8U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRsp_DATA_0_RX  (\
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_0 << 56U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_1 << 48U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_2 << 40U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_3 << 32U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_4 << 24U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_5 << 16U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_6 << 8U) | \
	(CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_7))

/* Tx */
#define IL_CccDkBleRsp_DATA_0_TX(x)  \
{ \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_0 = ((x) >> 56U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_1 = ((x) >> 48U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_2 = ((x) >> 40U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_3 = ((x) >> 32U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_4 = ((x) >> 24U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_5 = ((x) >> 16U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_6 = ((x) >> 8U) & 255U; \
	CccDkBleRsp_t.msg.sig.CccDkBleRsp_DATA_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRspFlowCtrl_7_RX  (\
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_0 << 56U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_1 << 48U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_2 << 40U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_3 << 32U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_4 << 24U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_5 << 16U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_6 << 8U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_7))

/* Tx */
#define IL_CccDkBleRspFlowCtrl_7_TX(x)  \
{ \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_0 = ((x) >> 56U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_1 = ((x) >> 48U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_2 = ((x) >> 40U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_3 = ((x) >> 32U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_4 = ((x) >> 24U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_5 = ((x) >> 16U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_6 = ((x) >> 8U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRspFlowCtrl_6_RX  (\
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_0 << 56U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_1 << 48U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_2 << 40U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_3 << 32U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_4 << 24U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_5 << 16U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_6 << 8U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_7))

/* Tx */
#define IL_CccDkBleRspFlowCtrl_6_TX(x)  \
{ \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_0 = ((x) >> 56U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_1 = ((x) >> 48U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_2 = ((x) >> 40U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_3 = ((x) >> 32U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_4 = ((x) >> 24U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_5 = ((x) >> 16U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_6 = ((x) >> 8U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRspFlowCtrl_5_RX  (\
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_0 << 56U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_1 << 48U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_2 << 40U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_3 << 32U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_4 << 24U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_5 << 16U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_6 << 8U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_7))

/* Tx */
#define IL_CccDkBleRspFlowCtrl_5_TX(x)  \
{ \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_0 = ((x) >> 56U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_1 = ((x) >> 48U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_2 = ((x) >> 40U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_3 = ((x) >> 32U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_4 = ((x) >> 24U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_5 = ((x) >> 16U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_6 = ((x) >> 8U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRspFlowCtrl_4_RX  (\
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_0 << 56U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_1 << 48U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_2 << 40U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_3 << 32U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_4 << 24U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_5 << 16U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_6 << 8U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_7))

/* Tx */
#define IL_CccDkBleRspFlowCtrl_4_TX(x)  \
{ \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_0 = ((x) >> 56U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_1 = ((x) >> 48U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_2 = ((x) >> 40U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_3 = ((x) >> 32U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_4 = ((x) >> 24U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_5 = ((x) >> 16U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_6 = ((x) >> 8U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRspFlowCtrl_3_RX  (\
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_0 << 56U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_1 << 48U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_2 << 40U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_3 << 32U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_4 << 24U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_5 << 16U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_6 << 8U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_7))

/* Tx */
#define IL_CccDkBleRspFlowCtrl_3_TX(x)  \
{ \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_0 = ((x) >> 56U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_1 = ((x) >> 48U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_2 = ((x) >> 40U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_3 = ((x) >> 32U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_4 = ((x) >> 24U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_5 = ((x) >> 16U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_6 = ((x) >> 8U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRspFlowCtrl_2_RX  (\
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_0 << 56U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_1 << 48U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_2 << 40U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_3 << 32U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_4 << 24U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_5 << 16U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_6 << 8U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_7))

/* Tx */
#define IL_CccDkBleRspFlowCtrl_2_TX(x)  \
{ \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_0 = ((x) >> 56U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_1 = ((x) >> 48U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_2 = ((x) >> 40U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_3 = ((x) >> 32U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_4 = ((x) >> 24U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_5 = ((x) >> 16U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_6 = ((x) >> 8U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRspFlowCtrl_1_RX  (\
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_0 << 56U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_1 << 48U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_2 << 40U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_3 << 32U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_4 << 24U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_5 << 16U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_6 << 8U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_7))

/* Tx */
#define IL_CccDkBleRspFlowCtrl_1_TX(x)  \
{ \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_0 = ((x) >> 56U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_1 = ((x) >> 48U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_2 = ((x) >> 40U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_3 = ((x) >> 32U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_4 = ((x) >> 24U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_5 = ((x) >> 16U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_6 = ((x) >> 8U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_CccDkBleRspFlowCtrl_0_RX  (\
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_0 << 56U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_1 << 48U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_2 << 40U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_3 << 32U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_4 << 24U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_5 << 16U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_6 << 8U) | \
	(CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_7))

/* Tx */
#define IL_CccDkBleRspFlowCtrl_0_TX(x)  \
{ \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_0 = ((x) >> 56U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_1 = ((x) >> 48U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_2 = ((x) >> 40U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_3 = ((x) >> 32U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_4 = ((x) >> 24U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_5 = ((x) >> 16U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_6 = ((x) >> 8U) & 255U; \
	CccDkBleRspFlowCtrl_t.msg.sig.CccDkBleRspFlowCtrl_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_Ibeacon_Adv_UUID_0_RX  (\
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_0_0 << 24U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_0_1 << 16U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_0_2 << 8U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_0_3))

/* Tx */
#define IL_Ibeacon_Adv_UUID_0_TX(x)  \
{ \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_0_0 = ((x) >> 24U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_0_1 = ((x) >> 16U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_0_2 = ((x) >> 8U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_Ibeacon_Adv_UUID_1_RX  (\
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_1_0 << 24U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_1_1 << 16U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_1_2 << 8U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_1_3))

/* Tx */
#define IL_Ibeacon_Adv_UUID_1_TX(x)  \
{ \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_1_0 = ((x) >> 24U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_1_1 = ((x) >> 16U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_1_2 = ((x) >> 8U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_Ibeacon_Adv_UUID_2_RX  (\
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_2_0 << 24U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_2_1 << 16U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_2_2 << 8U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_2_3))

/* Tx */
#define IL_Ibeacon_Adv_UUID_2_TX(x)  \
{ \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_2_0 = ((x) >> 24U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_2_1 = ((x) >> 16U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_2_2 = ((x) >> 8U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_2_3 = (x) & 255U; \
}

/*  Rx */
#define IL_Ibeacon_Adv_UUID_3_RX  (\
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_3_0 << 24U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_3_1 << 16U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_3_2 << 8U) | \
	(BTA_02_t.msg.sig.Ibeacon_Adv_UUID_3_3))

/* Tx */
#define IL_Ibeacon_Adv_UUID_3_TX(x)  \
{ \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_3_0 = ((x) >> 24U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_3_1 = ((x) >> 16U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_3_2 = ((x) >> 8U) & 255U; \
	BTA_02_t.msg.sig.Ibeacon_Adv_UUID_3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_0_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_0_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_0_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_0_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_0_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_0_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_0_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_0_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_0_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_1_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_1_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_1_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_1_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_1_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_1_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_1_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_1_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_1_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_2_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_2_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_2_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_2_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_2_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_2_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_2_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_2_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_2_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_2_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_3_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_3_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_3_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_3_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_3_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_3_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_3_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_3_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_3_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_4_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_4_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_4_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_4_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_4_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_4_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_4_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_4_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_4_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_5_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_5_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_5_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_5_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_5_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_5_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_5_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_5_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_5_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_5_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_6_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_6_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_6_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_6_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_6_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_6_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_6_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_6_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_6_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_6_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_7_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_7_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_7_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_7_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_7_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_7_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_7_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_7_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_7_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_7_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_8_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_8_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_8_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_8_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_8_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_8_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_8_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_8_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_8_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_9_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_9_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_9_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_9_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_9_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_9_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_9_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_9_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_9_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_9_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_10_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_10_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_10_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_10_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_10_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_10_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_10_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_10_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_10_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_10_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_11_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_11_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_11_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_11_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_11_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_11_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_11_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_11_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_11_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_11_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_12_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_12_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_12_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_12_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_12_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_12_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_12_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_12_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_12_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_13_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_13_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_13_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_13_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_13_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_13_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_13_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_13_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_13_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_13_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_14_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_14_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_14_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_14_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_14_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_14_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_14_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_14_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_14_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_14_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_FlowCtrl_15_RX  (\
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_15_0 << 24U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_15_1 << 16U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_15_2 << 8U) | \
	(BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_15_3))

/* Tx */
#define IL_BLE_Uplink_FlowCtrl_15_TX(x)  \
{ \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_15_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_15_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_15_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_FlowCtrl_t.msg.sig.BLE_Uplink_FlowCtrl_15_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_0_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_0_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_0_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_0_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_0_3))

/* Tx */
#define IL_BLE_Downlink_Data_0_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_0_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_0_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_0_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_1_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_1_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_1_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_1_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_1_3))

/* Tx */
#define IL_BLE_Downlink_Data_1_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_1_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_1_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_1_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_2_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_2_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_2_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_2_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_2_3))

/* Tx */
#define IL_BLE_Downlink_Data_2_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_2_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_2_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_2_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_2_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_3_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_3_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_3_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_3_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_3_3))

/* Tx */
#define IL_BLE_Downlink_Data_3_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_3_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_3_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_3_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_4_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_4_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_4_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_4_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_4_3))

/* Tx */
#define IL_BLE_Downlink_Data_4_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_4_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_4_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_4_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_5_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_5_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_5_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_5_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_5_3))

/* Tx */
#define IL_BLE_Downlink_Data_5_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_5_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_5_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_5_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_5_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_6_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_6_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_6_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_6_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_6_3))

/* Tx */
#define IL_BLE_Downlink_Data_6_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_6_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_6_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_6_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_6_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_7_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_7_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_7_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_7_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_7_3))

/* Tx */
#define IL_BLE_Downlink_Data_7_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_7_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_7_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_7_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_7_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_8_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_8_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_8_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_8_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_8_3))

/* Tx */
#define IL_BLE_Downlink_Data_8_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_8_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_8_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_8_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_9_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_9_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_9_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_9_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_9_3))

/* Tx */
#define IL_BLE_Downlink_Data_9_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_9_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_9_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_9_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_9_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_10_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_10_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_10_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_10_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_10_3))

/* Tx */
#define IL_BLE_Downlink_Data_10_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_10_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_10_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_10_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_10_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_11_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_11_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_11_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_11_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_11_3))

/* Tx */
#define IL_BLE_Downlink_Data_11_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_11_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_11_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_11_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_11_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_12_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_12_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_12_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_12_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_12_3))

/* Tx */
#define IL_BLE_Downlink_Data_12_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_12_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_12_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_12_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_13_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_13_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_13_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_13_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_13_3))

/* Tx */
#define IL_BLE_Downlink_Data_13_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_13_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_13_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_13_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_13_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_14_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_14_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_14_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_14_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_14_3))

/* Tx */
#define IL_BLE_Downlink_Data_14_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_14_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_14_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_14_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_14_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_Data_15_RX  (\
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_15_0 << 24U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_15_1 << 16U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_15_2 << 8U) | \
	(BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_15_3))

/* Tx */
#define IL_BLE_Downlink_Data_15_TX(x)  \
{ \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_15_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_15_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_15_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_Data_t.msg.sig.BLE_Downlink_Data_15_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA7_RSSI_Dev1_phy_RX  (BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Dev1_phy)

/*  Tx */
#define IL_BTA7_RSSI_Dev1_phy_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Dev1_phy = (x);\
}

/*  Rx */
#define IL_BTA7_RSSI_Dev2_phy_RX  (BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Dev2_phy)

/*  Tx */
#define IL_BTA7_RSSI_Dev2_phy_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Dev2_phy = (x);\
}

/*  Rx */
#define IL_BTA7_RSSI_Dev3_phy_RX  (BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Dev3_phy)

/*  Tx */
#define IL_BTA7_RSSI_Dev3_phy_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Dev3_phy = (x);\
}

/*  Rx */
#define IL_BTA7_RSSI_Dev4_phy_RX  (BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Dev4_phy)

/*  Tx */
#define IL_BTA7_RSSI_Dev4_phy_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Dev4_phy = (x);\
}

/*  Rx */
#define IL_BTA7_RSSI_Dev5_phy_RX  (BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Dev5_phy)

/*  Tx */
#define IL_BTA7_RSSI_Dev5_phy_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Dev5_phy = (x);\
}

/*  Rx */
#define IL_BTA7_UWB_Dist_Dev1_RX  (\
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev1_0 << 8U) | \
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev1_1))

/* Tx */
#define IL_BTA7_UWB_Dist_Dev1_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev1_0 = ((x) >> 8U) & 255U; \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev1_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA7_UWB_Dist_Dev2_RX  (\
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev2_0 << 8U) | \
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev2_1))

/* Tx */
#define IL_BTA7_UWB_Dist_Dev2_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev2_0 = ((x) >> 8U) & 255U; \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev2_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA7_UWB_Dist_Dev3_RX  (\
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev3_0 << 8U) | \
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev3_1))

/* Tx */
#define IL_BTA7_UWB_Dist_Dev3_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev3_0 = ((x) >> 8U) & 255U; \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev3_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA7_UWB_Dist_Dev4_RX  (\
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev4_0 << 8U) | \
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev4_1))

/* Tx */
#define IL_BTA7_UWB_Dist_Dev4_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev4_0 = ((x) >> 8U) & 255U; \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev4_1 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA7_UWB_Dist_Dev5_RX  (\
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev5_0 << 8U) | \
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev5_1))

/* Tx */
#define IL_BTA7_UWB_Dist_Dev5_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev5_0 = ((x) >> 8U) & 255U; \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_UWB_Dist_Dev5_1 = (x) & 255U; \
}

/*  Rx */
#define IL_UWBInfoSyncStsdev1_BTA7_RX  (BTA7_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev1_BTA7)

/*  Tx */
#define IL_UWBInfoSyncStsdev1_BTA7_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev1_BTA7 = (x);\
}

/*  Rx */
#define IL_UWBInfoSyncStsdev2_BTA7_RX  (BTA7_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev2_BTA7)

/*  Tx */
#define IL_UWBInfoSyncStsdev2_BTA7_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev2_BTA7 = (x);\
}

/*  Rx */
#define IL_UWBInfoSyncStsdev3_BTA7_RX  (BTA7_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev3_BTA7)

/*  Tx */
#define IL_UWBInfoSyncStsdev3_BTA7_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev3_BTA7 = (x);\
}

/*  Rx */
#define IL_UWBInfoSyncStsdev4_BTA7_RX  (BTA7_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev4_BTA7)

/*  Tx */
#define IL_UWBInfoSyncStsdev4_BTA7_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev4_BTA7 = (x);\
}

/*  Rx */
#define IL_UWBInfoSyncStsdev5_BTA7_RX  (BTA7_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev5_BTA7)

/*  Tx */
#define IL_UWBInfoSyncStsdev5_BTA7_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.UWBInfoSyncStsdev5_BTA7 = (x);\
}

/*  Rx */
#define IL_UWBRangingStsdev1_BTA7_RX  (BTA7_RSSI_UWB_t.msg.sig.UWBRangingStsdev1_BTA7)

/*  Tx */
#define IL_UWBRangingStsdev1_BTA7_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.UWBRangingStsdev1_BTA7 = (x);\
}

/*  Rx */
#define IL_UWBRangingStsdev2_BTA7_RX  (BTA7_RSSI_UWB_t.msg.sig.UWBRangingStsdev2_BTA7)

/*  Tx */
#define IL_UWBRangingStsdev2_BTA7_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.UWBRangingStsdev2_BTA7 = (x);\
}

/*  Rx */
#define IL_UWBRangingStsdev3_BTA7_RX  (BTA7_RSSI_UWB_t.msg.sig.UWBRangingStsdev3_BTA7)

/*  Tx */
#define IL_UWBRangingStsdev3_BTA7_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.UWBRangingStsdev3_BTA7 = (x);\
}

/*  Rx */
#define IL_UWBRangingStsdev4_BTA7_RX  (BTA7_RSSI_UWB_t.msg.sig.UWBRangingStsdev4_BTA7)

/*  Tx */
#define IL_UWBRangingStsdev4_BTA7_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.UWBRangingStsdev4_BTA7 = (x);\
}

/*  Rx */
#define IL_UWBRangingStsdev5_BTA7_RX  (BTA7_RSSI_UWB_t.msg.sig.UWBRangingStsdev5_BTA7)

/*  Tx */
#define IL_UWBRangingStsdev5_BTA7_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.UWBRangingStsdev5_BTA7 = (x);\
}

/*  Rx */
#define IL_BTA7_RSSI_Channel_Index_Dev2_RX  (\
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev2_0 << 4U) | \
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev2_0))

/* Tx */
#define IL_BTA7_RSSI_Channel_Index_Dev2_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev2_0 = ((x) >> 4U) & 3U; \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev2_0 = (x) & 15U; \
}

/*  Rx */
#define IL_BTA7_RSSI_Channel_Index_Dev1_RX  (BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev1)

/*  Tx */
#define IL_BTA7_RSSI_Channel_Index_Dev1_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev1 = (x);\
}

/*  Rx */
#define IL_BTA7_RSSI_Channel_Index_Dev3_RX  (\
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev3_0 << 2U) | \
	(BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev3_0))

/* Tx */
#define IL_BTA7_RSSI_Channel_Index_Dev3_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev3_0 = ((x) >> 2U) & 15U; \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev3_0 = (x) & 3U; \
}

/*  Rx */
#define IL_BTA7_RSSI_Channel_Index_Dev4_RX  (BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev4)

/*  Tx */
#define IL_BTA7_RSSI_Channel_Index_Dev4_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev4 = (x);\
}

/*  Rx */
#define IL_BTA7_RSSI_Channel_Index_Dev5_RX  (BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev5)

/*  Tx */
#define IL_BTA7_RSSI_Channel_Index_Dev5_TX(x)  \
{ \
	BTA7_RSSI_UWB_t.msg.sig.BTA7_RSSI_Channel_Index_Dev5 = (x);\
}

/*  Rx */
#define IL_BLE_Uplink_Data_0_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_0_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_0_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_0_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_0_3))

/* Tx */
#define IL_BLE_Uplink_Data_0_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_0_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_0_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_0_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_1_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_1_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_1_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_1_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_1_3))

/* Tx */
#define IL_BLE_Uplink_Data_1_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_1_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_1_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_1_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_2_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_2_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_2_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_2_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_2_3))

/* Tx */
#define IL_BLE_Uplink_Data_2_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_2_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_2_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_2_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_2_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_3_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_3_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_3_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_3_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_3_3))

/* Tx */
#define IL_BLE_Uplink_Data_3_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_3_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_3_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_3_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_4_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_4_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_4_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_4_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_4_3))

/* Tx */
#define IL_BLE_Uplink_Data_4_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_4_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_4_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_4_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_5_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_5_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_5_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_5_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_5_3))

/* Tx */
#define IL_BLE_Uplink_Data_5_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_5_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_5_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_5_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_5_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_6_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_6_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_6_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_6_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_6_3))

/* Tx */
#define IL_BLE_Uplink_Data_6_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_6_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_6_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_6_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_6_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_7_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_7_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_7_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_7_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_7_3))

/* Tx */
#define IL_BLE_Uplink_Data_7_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_7_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_7_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_7_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_7_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_8_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_8_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_8_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_8_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_8_3))

/* Tx */
#define IL_BLE_Uplink_Data_8_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_8_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_8_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_8_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_9_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_9_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_9_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_9_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_9_3))

/* Tx */
#define IL_BLE_Uplink_Data_9_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_9_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_9_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_9_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_9_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_10_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_10_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_10_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_10_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_10_3))

/* Tx */
#define IL_BLE_Uplink_Data_10_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_10_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_10_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_10_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_10_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_11_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_11_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_11_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_11_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_11_3))

/* Tx */
#define IL_BLE_Uplink_Data_11_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_11_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_11_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_11_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_11_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_12_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_12_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_12_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_12_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_12_3))

/* Tx */
#define IL_BLE_Uplink_Data_12_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_12_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_12_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_12_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_13_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_13_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_13_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_13_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_13_3))

/* Tx */
#define IL_BLE_Uplink_Data_13_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_13_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_13_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_13_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_13_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_14_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_14_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_14_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_14_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_14_3))

/* Tx */
#define IL_BLE_Uplink_Data_14_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_14_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_14_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_14_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_14_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Uplink_Data_15_RX  (\
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_15_0 << 24U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_15_1 << 16U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_15_2 << 8U) | \
	(BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_15_3))

/* Tx */
#define IL_BLE_Uplink_Data_15_TX(x)  \
{ \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_15_0 = ((x) >> 24U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_15_1 = ((x) >> 16U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_15_2 = ((x) >> 8U) & 255U; \
	BLE_Uplink_Data_t.msg.sig.BLE_Uplink_Data_15_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_0_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_0_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_0_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_0_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_0_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_0_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_0_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_0_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_0_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_0_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_1_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_1_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_1_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_1_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_1_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_1_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_1_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_1_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_1_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_2_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_2_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_2_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_2_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_2_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_2_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_2_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_2_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_2_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_2_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_3_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_3_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_3_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_3_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_3_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_3_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_3_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_3_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_3_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_3_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_4_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_4_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_4_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_4_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_4_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_4_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_4_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_4_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_4_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_4_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_5_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_5_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_5_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_5_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_5_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_5_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_5_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_5_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_5_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_5_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_6_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_6_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_6_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_6_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_6_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_6_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_6_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_6_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_6_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_6_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_7_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_7_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_7_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_7_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_7_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_7_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_7_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_7_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_7_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_7_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_8_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_8_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_8_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_8_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_8_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_8_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_8_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_8_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_8_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_8_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_9_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_9_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_9_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_9_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_9_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_9_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_9_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_9_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_9_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_9_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_10_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_10_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_10_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_10_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_10_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_10_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_10_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_10_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_10_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_10_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_11_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_11_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_11_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_11_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_11_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_11_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_11_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_11_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_11_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_11_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_12_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_12_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_12_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_12_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_12_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_12_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_12_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_12_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_12_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_12_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_13_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_13_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_13_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_13_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_13_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_13_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_13_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_13_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_13_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_13_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_14_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_14_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_14_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_14_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_14_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_14_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_14_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_14_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_14_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_14_3 = (x) & 255U; \
}

/*  Rx */
#define IL_BLE_Downlink_FlowCtrl_15_RX  (\
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_15_0 << 24U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_15_1 << 16U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_15_2 << 8U) | \
	(BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_15_3))

/* Tx */
#define IL_BLE_Downlink_FlowCtrl_15_TX(x)  \
{ \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_15_0 = ((x) >> 24U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_15_1 = ((x) >> 16U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_15_2 = ((x) >> 8U) & 255U; \
	BLE_Downlink_FlowCtrl_t.msg.sig.BLE_Downlink_FlowCtrl_15_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DK_Magnt_Req_DevInfo_RX  (\
	(BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_0 << 40U) | \
	(BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_1 << 32U) | \
	(BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_2 << 24U) | \
	(BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_3 << 16U) | \
	(BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_4 << 8U) | \
	(BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_5))

/* Tx */
#define IL_DK_Magnt_Req_DevInfo_TX(x)  \
{ \
	BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_0 = ((x) >> 40U) & 255U; \
	BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_1 = ((x) >> 32U) & 255U; \
	BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_2 = ((x) >> 24U) & 255U; \
	BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_3 = ((x) >> 16U) & 255U; \
	BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_4 = ((x) >> 8U) & 255U; \
	BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_DevInfo_5 = (x) & 255U; \
}

/*  Rx */
#define IL_DK_Magnt_Req_Type_RX  (BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_Type)

/*  Tx */
#define IL_DK_Magnt_Req_Type_TX(x)  \
{ \
	BLE_DK_REQ_t.msg.sig.DK_Magnt_Req_Type = (x);\
}

/*  Rx */
#define IL_DK_Magnt_Rsp_DevInfo_RX  (\
	(BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_0 << 40U) | \
	(BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_1 << 32U) | \
	(BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_2 << 24U) | \
	(BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_3 << 16U) | \
	(BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_4 << 8U) | \
	(BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_5))

/* Tx */
#define IL_DK_Magnt_Rsp_DevInfo_TX(x)  \
{ \
	BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_0 = ((x) >> 40U) & 255U; \
	BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_1 = ((x) >> 32U) & 255U; \
	BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_2 = ((x) >> 24U) & 255U; \
	BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_3 = ((x) >> 16U) & 255U; \
	BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_4 = ((x) >> 8U) & 255U; \
	BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_DevInfo_5 = (x) & 255U; \
}

/*  Rx */
#define IL_DK_Magnt_Rsp_FR_RX  (BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_FR)

/*  Tx */
#define IL_DK_Magnt_Rsp_FR_TX(x)  \
{ \
	BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_FR = (x);\
}

/*  Rx */
#define IL_DK_Magnt_Rsp_ReqType_RX  (BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_ReqType)

/*  Tx */
#define IL_DK_Magnt_Rsp_ReqType_TX(x)  \
{ \
	BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_ReqType = (x);\
}

/*  Rx */
#define IL_DK_Magnt_Rsp_Result_RX  (BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_Result)

/*  Tx */
#define IL_DK_Magnt_Rsp_Result_TX(x)  \
{ \
	BLE_DK_RSP_t.msg.sig.DK_Magnt_Rsp_Result = (x);\
}

/*  Rx */
#define IL_BTA_SRF_FactoryRsp_0_RX  (\
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_0 << 56U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_1 << 48U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_2 << 40U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_3 << 32U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_4 << 24U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_5 << 16U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_6 << 8U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_7))

/* Tx */
#define IL_BTA_SRF_FactoryRsp_0_TX(x)  \
{ \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryRsp_1_RX  (\
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_0 << 56U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_1 << 48U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_2 << 40U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_3 << 32U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_4 << 24U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_5 << 16U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_6 << 8U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_7))

/* Tx */
#define IL_BTA_SRF_FactoryRsp_1_TX(x)  \
{ \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryRsp_2_RX  (\
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_0 << 56U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_1 << 48U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_2 << 40U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_3 << 32U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_4 << 24U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_5 << 16U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_6 << 8U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_7))

/* Tx */
#define IL_BTA_SRF_FactoryRsp_2_TX(x)  \
{ \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryRsp_3_RX  (\
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_0 << 56U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_1 << 48U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_2 << 40U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_3 << 32U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_4 << 24U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_5 << 16U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_6 << 8U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_7))

/* Tx */
#define IL_BTA_SRF_FactoryRsp_3_TX(x)  \
{ \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryRsp_4_RX  (\
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_0 << 56U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_1 << 48U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_2 << 40U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_3 << 32U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_4 << 24U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_5 << 16U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_6 << 8U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_7))

/* Tx */
#define IL_BTA_SRF_FactoryRsp_4_TX(x)  \
{ \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryRsp_5_RX  (\
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_0 << 56U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_1 << 48U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_2 << 40U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_3 << 32U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_4 << 24U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_5 << 16U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_6 << 8U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_7))

/* Tx */
#define IL_BTA_SRF_FactoryRsp_5_TX(x)  \
{ \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryRsp_6_RX  (\
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_0 << 56U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_1 << 48U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_2 << 40U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_3 << 32U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_4 << 24U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_5 << 16U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_6 << 8U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_7))

/* Tx */
#define IL_BTA_SRF_FactoryRsp_6_TX(x)  \
{ \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryRsp_7_RX  (\
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_0 << 56U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_1 << 48U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_2 << 40U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_3 << 32U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_4 << 24U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_5 << 16U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_6 << 8U) | \
	(BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_7))

/* Tx */
#define IL_BTA_SRF_FactoryRsp_7_TX(x)  \
{ \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryRsp_t.msg.sig.BTA_SRF_FactoryRsp_7_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryReq_0_RX  (\
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_0 << 56U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_1 << 48U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_2 << 40U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_3 << 32U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_4 << 24U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_5 << 16U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_6 << 8U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_7))

/* Tx */
#define IL_BTA_SRF_FactoryReq_0_TX(x)  \
{ \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_0_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryReq_1_RX  (\
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_0 << 56U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_1 << 48U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_2 << 40U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_3 << 32U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_4 << 24U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_5 << 16U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_6 << 8U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_7))

/* Tx */
#define IL_BTA_SRF_FactoryReq_1_TX(x)  \
{ \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_1_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryReq_2_RX  (\
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_0 << 56U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_1 << 48U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_2 << 40U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_3 << 32U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_4 << 24U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_5 << 16U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_6 << 8U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_7))

/* Tx */
#define IL_BTA_SRF_FactoryReq_2_TX(x)  \
{ \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_2_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryReq_3_RX  (\
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_0 << 56U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_1 << 48U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_2 << 40U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_3 << 32U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_4 << 24U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_5 << 16U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_6 << 8U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_7))

/* Tx */
#define IL_BTA_SRF_FactoryReq_3_TX(x)  \
{ \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_3_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryReq_4_RX  (\
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_0 << 56U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_1 << 48U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_2 << 40U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_3 << 32U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_4 << 24U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_5 << 16U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_6 << 8U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_7))

/* Tx */
#define IL_BTA_SRF_FactoryReq_4_TX(x)  \
{ \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_4_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryReq_5_RX  (\
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_0 << 56U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_1 << 48U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_2 << 40U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_3 << 32U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_4 << 24U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_5 << 16U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_6 << 8U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_7))

/* Tx */
#define IL_BTA_SRF_FactoryReq_5_TX(x)  \
{ \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_5_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryReq_6_RX  (\
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_0 << 56U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_1 << 48U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_2 << 40U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_3 << 32U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_4 << 24U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_5 << 16U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_6 << 8U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_7))

/* Tx */
#define IL_BTA_SRF_FactoryReq_6_TX(x)  \
{ \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_6_7 = (x) & 255U; \
}

/*  Rx */
#define IL_BTA_SRF_FactoryReq_7_RX  (\
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_0 << 56U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_1 << 48U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_2 << 40U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_3 << 32U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_4 << 24U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_5 << 16U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_6 << 8U) | \
	(BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_7))

/* Tx */
#define IL_BTA_SRF_FactoryReq_7_TX(x)  \
{ \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_0 = ((x) >> 56U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_1 = ((x) >> 48U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_2 = ((x) >> 40U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_3 = ((x) >> 32U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_4 = ((x) >> 24U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_5 = ((x) >> 16U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_6 = ((x) >> 8U) & 255U; \
	BTA_SRF_FactoryReq_t.msg.sig.BTA_SRF_FactoryReq_7_7 = (x) & 255U; \
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
#define IL_ScrNodId_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.ScrNodId_BTA_RB2)

/*  Tx */
#define IL_ScrNodId_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.ScrNodId_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_RepMsgReq_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.RepMsgReq_BTA_RB2)

/*  Tx */
#define IL_RepMsgReq_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.RepMsgReq_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorn_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.NetMngtCoorn_BTA_RB2)

/*  Tx */
#define IL_NetMngtCoorn_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.NetMngtCoorn_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_NetMngtCoorrSleepRdy_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.NetMngtCoorrSleepRdy_BTA_RB2)

/*  Tx */
#define IL_NetMngtCoorrSleepRdy_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.NetMngtCoorrSleepRdy_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_ActvWakeup_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.ActvWakeup_BTA_RB2)

/*  Tx */
#define IL_ActvWakeup_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.ActvWakeup_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_ResdBit1_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.ResdBit1_BTA_RB2)

/*  Tx */
#define IL_ResdBit1_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.ResdBit1_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PtlNetInfo_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PtlNetInfo_BTA_RB2)

/*  Tx */
#define IL_PtlNetInfo_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PtlNetInfo_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_ResdBit2_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.ResdBit2_BTA_RB2)

/*  Tx */
#define IL_ResdBit2_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.ResdBit2_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN1_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN1_BTA_RB2)

/*  Tx */
#define IL_PN1_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN1_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN2_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN2_BTA_RB2)

/*  Tx */
#define IL_PN2_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN2_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN3_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN3_BTA_RB2)

/*  Tx */
#define IL_PN3_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN3_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN4_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN4_BTA_RB2)

/*  Tx */
#define IL_PN4_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN4_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN5_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN5_BTA_RB2)

/*  Tx */
#define IL_PN5_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN5_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN6_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN6_BTA_RB2)

/*  Tx */
#define IL_PN6_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN6_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN7_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN7_BTA_RB2)

/*  Tx */
#define IL_PN7_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN7_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN8_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN8_BTA_RB2)

/*  Tx */
#define IL_PN8_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN8_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN9_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN9_BTA_RB2)

/*  Tx */
#define IL_PN9_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN9_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN10_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN10_BTA_RB2)

/*  Tx */
#define IL_PN10_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN10_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN11_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN11_BTA_RB2)

/*  Tx */
#define IL_PN11_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN11_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN12_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN12_BTA_RB2)

/*  Tx */
#define IL_PN12_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN12_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN13_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN13_BTA_RB2)

/*  Tx */
#define IL_PN13_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN13_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN14_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN14_BTA_RB2)

/*  Tx */
#define IL_PN14_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN14_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN15_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN15_BTA_RB2)

/*  Tx */
#define IL_PN15_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN15_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN16_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN16_BTA_RB2)

/*  Tx */
#define IL_PN16_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN16_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN17_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN17_BTA_RB2)

/*  Tx */
#define IL_PN17_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN17_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN18_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN18_BTA_RB2)

/*  Tx */
#define IL_PN18_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN18_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN19_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN19_BTA_RB2)

/*  Tx */
#define IL_PN19_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN19_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN20_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN20_BTA_RB2)

/*  Tx */
#define IL_PN20_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN20_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN21_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN21_BTA_RB2)

/*  Tx */
#define IL_PN21_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN21_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN22_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN22_BTA_RB2)

/*  Tx */
#define IL_PN22_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN22_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN23_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN23_BTA_RB2)

/*  Tx */
#define IL_PN23_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN23_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN24_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN24_BTA_RB2)

/*  Tx */
#define IL_PN24_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN24_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN25_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN25_BTA_RB2)

/*  Tx */
#define IL_PN25_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN25_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN26_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN26_BTA_RB2)

/*  Tx */
#define IL_PN26_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN26_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN27_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN27_BTA_RB2)

/*  Tx */
#define IL_PN27_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN27_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN28_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN28_BTA_RB2)

/*  Tx */
#define IL_PN28_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN28_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN29_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN29_BTA_RB2)

/*  Tx */
#define IL_PN29_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN29_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN30_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN30_BTA_RB2)

/*  Tx */
#define IL_PN30_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN30_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN31_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN31_BTA_RB2)

/*  Tx */
#define IL_PN31_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN31_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN32_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN32_BTA_RB2)

/*  Tx */
#define IL_PN32_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN32_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN33_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN33_BTA_RB2)

/*  Tx */
#define IL_PN33_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN33_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN34_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN34_BTA_RB2)

/*  Tx */
#define IL_PN34_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN34_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN35_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN35_BTA_RB2)

/*  Tx */
#define IL_PN35_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN35_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN36_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN36_BTA_RB2)

/*  Tx */
#define IL_PN36_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN36_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN37_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN37_BTA_RB2)

/*  Tx */
#define IL_PN37_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN37_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN38_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN38_BTA_RB2)

/*  Tx */
#define IL_PN38_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN38_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN39_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN39_BTA_RB2)

/*  Tx */
#define IL_PN39_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN39_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN40_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN40_BTA_RB2)

/*  Tx */
#define IL_PN40_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN40_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN41_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN41_BTA_RB2)

/*  Tx */
#define IL_PN41_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN41_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN42_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN42_BTA_RB2)

/*  Tx */
#define IL_PN42_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN42_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN43_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN43_BTA_RB2)

/*  Tx */
#define IL_PN43_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN43_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN44_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN44_BTA_RB2)

/*  Tx */
#define IL_PN44_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN44_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN45_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN45_BTA_RB2)

/*  Tx */
#define IL_PN45_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN45_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN46_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN46_BTA_RB2)

/*  Tx */
#define IL_PN46_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN46_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN47_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN47_BTA_RB2)

/*  Tx */
#define IL_PN47_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN47_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_PN48_BTA_RB2_RX  (NM_BTA_RB2_t.msg.sig.PN48_BTA_RB2)

/*  Tx */
#define IL_PN48_BTA_RB2_TX(x)  \
{ \
	NM_BTA_RB2_t.msg.sig.PN48_BTA_RB2 = (x);\
}

/*  Rx */
#define IL_ScrNodId_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.ScrNodId_BTA_FB)

/*  Tx */
#define IL_ScrNodId_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.ScrNodId_BTA_FB = (x);\
}

/*  Rx */
#define IL_RepMsgReq_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.RepMsgReq_BTA_FB)

/*  Tx */
#define IL_RepMsgReq_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.RepMsgReq_BTA_FB = (x);\
}

/*  Rx */
#define IL_NetMngtCoorn_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.NetMngtCoorn_BTA_FB)

/*  Tx */
#define IL_NetMngtCoorn_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.NetMngtCoorn_BTA_FB = (x);\
}

/*  Rx */
#define IL_NetMngtCoorrSleepRdy_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.NetMngtCoorrSleepRdy_BTA_FB)

/*  Tx */
#define IL_NetMngtCoorrSleepRdy_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.NetMngtCoorrSleepRdy_BTA_FB = (x);\
}

/*  Rx */
#define IL_ActvWakeup_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.ActvWakeup_BTA_FB)

/*  Tx */
#define IL_ActvWakeup_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.ActvWakeup_BTA_FB = (x);\
}

/*  Rx */
#define IL_ResdBit1_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.ResdBit1_BTA_FB)

/*  Tx */
#define IL_ResdBit1_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.ResdBit1_BTA_FB = (x);\
}

/*  Rx */
#define IL_PtlNetInfo_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PtlNetInfo_BTA_FB)

/*  Tx */
#define IL_PtlNetInfo_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PtlNetInfo_BTA_FB = (x);\
}

/*  Rx */
#define IL_ResdBit2_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.ResdBit2_BTA_FB)

/*  Tx */
#define IL_ResdBit2_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.ResdBit2_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN1_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN1_BTA_FB)

/*  Tx */
#define IL_PN1_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN1_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN2_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN2_BTA_FB)

/*  Tx */
#define IL_PN2_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN2_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN3_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN3_BTA_FB)

/*  Tx */
#define IL_PN3_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN3_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN4_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN4_BTA_FB)

/*  Tx */
#define IL_PN4_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN4_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN5_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN5_BTA_FB)

/*  Tx */
#define IL_PN5_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN5_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN6_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN6_BTA_FB)

/*  Tx */
#define IL_PN6_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN6_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN7_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN7_BTA_FB)

/*  Tx */
#define IL_PN7_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN7_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN8_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN8_BTA_FB)

/*  Tx */
#define IL_PN8_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN8_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN9_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN9_BTA_FB)

/*  Tx */
#define IL_PN9_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN9_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN10_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN10_BTA_FB)

/*  Tx */
#define IL_PN10_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN10_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN11_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN11_BTA_FB)

/*  Tx */
#define IL_PN11_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN11_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN12_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN12_BTA_FB)

/*  Tx */
#define IL_PN12_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN12_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN13_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN13_BTA_FB)

/*  Tx */
#define IL_PN13_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN13_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN14_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN14_BTA_FB)

/*  Tx */
#define IL_PN14_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN14_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN15_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN15_BTA_FB)

/*  Tx */
#define IL_PN15_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN15_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN16_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN16_BTA_FB)

/*  Tx */
#define IL_PN16_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN16_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN17_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN17_BTA_FB)

/*  Tx */
#define IL_PN17_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN17_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN18_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN18_BTA_FB)

/*  Tx */
#define IL_PN18_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN18_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN19_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN19_BTA_FB)

/*  Tx */
#define IL_PN19_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN19_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN20_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN20_BTA_FB)

/*  Tx */
#define IL_PN20_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN20_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN21_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN21_BTA_FB)

/*  Tx */
#define IL_PN21_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN21_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN22_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN22_BTA_FB)

/*  Tx */
#define IL_PN22_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN22_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN23_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN23_BTA_FB)

/*  Tx */
#define IL_PN23_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN23_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN24_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN24_BTA_FB)

/*  Tx */
#define IL_PN24_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN24_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN25_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN25_BTA_FB)

/*  Tx */
#define IL_PN25_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN25_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN26_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN26_BTA_FB)

/*  Tx */
#define IL_PN26_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN26_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN27_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN27_BTA_FB)

/*  Tx */
#define IL_PN27_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN27_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN28_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN28_BTA_FB)

/*  Tx */
#define IL_PN28_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN28_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN29_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN29_BTA_FB)

/*  Tx */
#define IL_PN29_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN29_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN30_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN30_BTA_FB)

/*  Tx */
#define IL_PN30_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN30_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN31_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN31_BTA_FB)

/*  Tx */
#define IL_PN31_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN31_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN32_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN32_BTA_FB)

/*  Tx */
#define IL_PN32_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN32_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN33_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN33_BTA_FB)

/*  Tx */
#define IL_PN33_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN33_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN34_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN34_BTA_FB)

/*  Tx */
#define IL_PN34_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN34_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN35_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN35_BTA_FB)

/*  Tx */
#define IL_PN35_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN35_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN36_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN36_BTA_FB)

/*  Tx */
#define IL_PN36_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN36_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN37_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN37_BTA_FB)

/*  Tx */
#define IL_PN37_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN37_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN38_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN38_BTA_FB)

/*  Tx */
#define IL_PN38_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN38_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN39_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN39_BTA_FB)

/*  Tx */
#define IL_PN39_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN39_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN40_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN40_BTA_FB)

/*  Tx */
#define IL_PN40_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN40_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN41_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN41_BTA_FB)

/*  Tx */
#define IL_PN41_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN41_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN42_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN42_BTA_FB)

/*  Tx */
#define IL_PN42_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN42_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN43_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN43_BTA_FB)

/*  Tx */
#define IL_PN43_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN43_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN44_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN44_BTA_FB)

/*  Tx */
#define IL_PN44_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN44_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN45_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN45_BTA_FB)

/*  Tx */
#define IL_PN45_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN45_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN46_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN46_BTA_FB)

/*  Tx */
#define IL_PN46_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN46_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN47_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN47_BTA_FB)

/*  Tx */
#define IL_PN47_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN47_BTA_FB = (x);\
}

/*  Rx */
#define IL_PN48_BTA_FB_RX  (NM_BTA_FB_t.msg.sig.PN48_BTA_FB)

/*  Tx */
#define IL_PN48_BTA_FB_TX(x)  \
{ \
	NM_BTA_FB_t.msg.sig.PN48_BTA_FB = (x);\
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
#define IL_DiagReqData0_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData0_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData0_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData0_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData0_BTA1_3))

/* Tx */
#define IL_DiagReqData0_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData0_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData0_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData0_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData0_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData1_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData1_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData1_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData1_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData1_BTA1_3))

/* Tx */
#define IL_DiagReqData1_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData1_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData1_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData1_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData1_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData2_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData2_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData2_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData2_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData2_BTA1_3))

/* Tx */
#define IL_DiagReqData2_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData2_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData2_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData2_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData2_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData3_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData3_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData3_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData3_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData3_BTA1_3))

/* Tx */
#define IL_DiagReqData3_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData3_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData3_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData3_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData3_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData4_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData4_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData4_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData4_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData4_BTA1_3))

/* Tx */
#define IL_DiagReqData4_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData4_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData4_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData4_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData4_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData5_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData5_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData5_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData5_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData5_BTA1_3))

/* Tx */
#define IL_DiagReqData5_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData5_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData5_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData5_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData5_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData6_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData6_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData6_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData6_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData6_BTA1_3))

/* Tx */
#define IL_DiagReqData6_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData6_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData6_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData6_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData6_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData7_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData7_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData7_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData7_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData7_BTA1_3))

/* Tx */
#define IL_DiagReqData7_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData7_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData7_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData7_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData7_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData8_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData8_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData8_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData8_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData8_BTA1_3))

/* Tx */
#define IL_DiagReqData8_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData8_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData8_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData8_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData8_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData9_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData9_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData9_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData9_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData9_BTA1_3))

/* Tx */
#define IL_DiagReqData9_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData9_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData9_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData9_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData9_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData10_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData10_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData10_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData10_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData10_BTA1_3))

/* Tx */
#define IL_DiagReqData10_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData10_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData10_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData10_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData10_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData11_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData11_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData11_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData11_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData11_BTA1_3))

/* Tx */
#define IL_DiagReqData11_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData11_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData11_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData11_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData11_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData12_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData12_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData12_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData12_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData12_BTA1_3))

/* Tx */
#define IL_DiagReqData12_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData12_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData12_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData12_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData12_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData13_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData13_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData13_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData13_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData13_BTA1_3))

/* Tx */
#define IL_DiagReqData13_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData13_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData13_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData13_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData13_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData14_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData14_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData14_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData14_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData14_BTA1_3))

/* Tx */
#define IL_DiagReqData14_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData14_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData14_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData14_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData14_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagReqData15_BTA1_RX  (\
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData15_BTA1_0 << 24U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData15_BTA1_1 << 16U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData15_BTA1_2 << 8U) | \
	(DIAG_REQ_BTA1_t.msg.sig.DiagReqData15_BTA1_3))

/* Tx */
#define IL_DiagReqData15_BTA1_TX(x)  \
{ \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData15_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData15_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData15_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_REQ_BTA1_t.msg.sig.DiagReqData15_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData0_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData0_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData0_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData0_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData0_BTA1_3))

/* Tx */
#define IL_DiagRespData0_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData0_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData0_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData0_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData0_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData1_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData1_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData1_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData1_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData1_BTA1_3))

/* Tx */
#define IL_DiagRespData1_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData1_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData1_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData1_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData1_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData2_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData2_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData2_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData2_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData2_BTA1_3))

/* Tx */
#define IL_DiagRespData2_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData2_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData2_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData2_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData2_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData3_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData3_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData3_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData3_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData3_BTA1_3))

/* Tx */
#define IL_DiagRespData3_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData3_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData3_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData3_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData3_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData4_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData4_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData4_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData4_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData4_BTA1_3))

/* Tx */
#define IL_DiagRespData4_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData4_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData4_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData4_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData4_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData5_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData5_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData5_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData5_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData5_BTA1_3))

/* Tx */
#define IL_DiagRespData5_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData5_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData5_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData5_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData5_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData6_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData6_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData6_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData6_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData6_BTA1_3))

/* Tx */
#define IL_DiagRespData6_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData6_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData6_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData6_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData6_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData7_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData7_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData7_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData7_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData7_BTA1_3))

/* Tx */
#define IL_DiagRespData7_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData7_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData7_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData7_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData7_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData8_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData8_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData8_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData8_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData8_BTA1_3))

/* Tx */
#define IL_DiagRespData8_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData8_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData8_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData8_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData8_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData9_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData9_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData9_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData9_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData9_BTA1_3))

/* Tx */
#define IL_DiagRespData9_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData9_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData9_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData9_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData9_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData10_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData10_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData10_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData10_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData10_BTA1_3))

/* Tx */
#define IL_DiagRespData10_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData10_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData10_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData10_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData10_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData11_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData11_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData11_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData11_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData11_BTA1_3))

/* Tx */
#define IL_DiagRespData11_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData11_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData11_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData11_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData11_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData12_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData12_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData12_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData12_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData12_BTA1_3))

/* Tx */
#define IL_DiagRespData12_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData12_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData12_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData12_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData12_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData13_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData13_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData13_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData13_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData13_BTA1_3))

/* Tx */
#define IL_DiagRespData13_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData13_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData13_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData13_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData13_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData14_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData14_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData14_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData14_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData14_BTA1_3))

/* Tx */
#define IL_DiagRespData14_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData14_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData14_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData14_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData14_BTA1_3 = (x) & 255U; \
}

/*  Rx */
#define IL_DiagRespData15_BTA1_RX  (\
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData15_BTA1_0 << 24U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData15_BTA1_1 << 16U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData15_BTA1_2 << 8U) | \
	(DIAG_RESP_BTA1_t.msg.sig.DiagRespData15_BTA1_3))

/* Tx */
#define IL_DiagRespData15_BTA1_TX(x)  \
{ \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData15_BTA1_0 = ((x) >> 24U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData15_BTA1_1 = ((x) >> 16U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData15_BTA1_2 = ((x) >> 8U) & 255U; \
	DIAG_RESP_BTA1_t.msg.sig.DiagRespData15_BTA1_3 = (x) & 255U; \
}

void can_msg_init(void);
void can_rx_handle(uint8_t *data, uint32_t can_id, uint8_t len);


#ifdef __cplusplus
}
#endif

#endif
