/*
 * uciMessage.h
 *
 *  Created on: 2024年7月19日
 *      Author: F704010638_
 */

#ifndef NCJ29D6_UCIMESSAGE_H_
#define NCJ29D6_UCIMESSAGE_H_
#include <stdint.h>
#include "fsl_os_abstraction.h"
#include "SysTimer.h"
#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"
#define uciMsgTimerCreate(t)	(SysTimerCreate(t))
#define uciMsgTimerIsExpired(t)	(SysTimerIsExpired(t))


/** @brief Packing macro. */
#define __UCI_MSG_PACKED __attribute__((__packed__))

/** @brief Size of a UCI header in byte. */
#define PHSCA_UCIMSG_HEADER_LENGTH ((uint32_t)4u)

/**
 * @defgroup GID Command Group Identifiers (GID)
 * @{
 */

/** @brief UCI Core Group. */
#define PHSCA_UCIMSG_GROUP_ID_CORE ((uint8_t)0x0u)

/** @brief UCI UWB Session Config Group. */
#define PHSCA_UCIMSG_GROUP_ID_UWB_SESSION_CONFIG ((uint8_t)0x1u)

/** @brief UCI UWB Session control Group. */
#define PHSCA_UCIMSG_GROUP_ID_UWB_SESSION_CONTROL ((uint8_t)0x2u)

/** @brief UCI Proprietary Group. */
#define PHSCA_UCIMSG_GROUP_ID_PROPRIETARY ((uint8_t)0xEu)

#define PHSCA_UCIMSG_GROUP_ID_RANGE 	((uint8_t)0X04)

#define PHSCA_UCIMSG_GROUP_ID_CPD   	((uint8_t)0X05)

#define PHSCA_UCIMSG_GROUP_ID_CUSTOM 	((uint8_t)0X07)
/**@}*/

/**
 * @defgroup OID Opcode list for UCI Core Group (OID)
 * @{
 */

/** @brief Request for UWBD reset. */
#define PHSCA_UCIMSG_OID_CORE_RESET ((uint8_t)0x0u)

/** @brief UWBD notifies about the current state of the UWBD. */
#define PHSCA_UCIMSG_OID_CORE_DEVICE_STATUS_NTF ((uint8_t)0x1u)

/** @brief To get the UWBD device information. */
#define PHSCA_UCIMSG_OID_CORE_GET_DEVICE_INFO ((uint8_t)0x2u)

/** @brief To get the UWBD capability information. */
#define PHSCA_UCIMSG_OID_CORE_GET_CAPS_INFO ((uint8_t)0x3u)

/** @brief To set the UWBD device configurations. */
#define PHSCA_UCIMSG_OID_CORE_SET_CONFIG ((uint8_t)0x4u)

/** @brief To get the UWD device configurations. */
#define PHSCA_UCIMSG_OID_CORE_GET_CONFIG ((uint8_t)0x5u)

/** @brief Notifies about any generic errors in UWBD. */
#define PHSCA_UCIMSG_OID_CORE_GENERIC_ERROR_NTF ((uint8_t)0x7u)


/* Opcode list for UCI Session Config Group. */
/** @brief UWB session creation request for requested session type. */
#define PHSCA_UCIMSG_OID_SESSIONCFG_INIT ((uint8_t)0x0u)

/** @brief Request for destroying the requested UWB session. */
#define PHSCA_UCIMSG_OID_SESSIONCFG_DEINIT ((uint8_t)0x1u)

/** @brief Notifies the current UWB session state. */
#define PHSCA_UCIMSG_OID_SESSIONCFG_STATUS_NTF ((uint8_t)0x2u)

/** @brief Configuration request for UWB session with specified configurations. */
#define PHSCA_UCIMSG_OID_SESSIONCFG_SET_APP_CONFIG ((uint8_t)0x3u)

/** @brief To get the current configuration for specified UWB session. */
#define PHSCA_UCIMSG_OID_SESSIONCFG_GET_APP_CONFIG ((uint8_t)0x4u)

/** @brief To get the number of UWB session created in UWBD device. */
#define PHSCA_UCIMSG_OID_SESSIONCFG_GET_COUNT ((uint8_t)0x5u)

/** @brief To get the current state of the UWB session. */
#define PHSCA_UCIMSG_OID_SESSIONCFG_GET_STATE ((uint8_t)0x6u)

/** @brief To check possible RAN Multiplier value for a new session. */
#define PHSCA_UCIMSG_OID_SESSIONCFG_GET_RAN_MULTIPLIER_VAL ((uint8_t)0x20u)


/* Opcode list for UCI Session Control Group. */
/** @brief To activate the UWB ranging session. */
#define PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_START ((uint8_t)0x0u)

/** @brief To deactivate the UWB ranging session. */
#define PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_STOP ((uint8_t)0x1u)

/** @brief To get the number of ranging attempted. */
#define PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_GET_RANGING_COUNT ((uint8_t)0x3u)

/** @brief To get Range CCC Data Notification. */
#define PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_CCC_DATA_NTF ((uint8_t)0x20u)

/** @brief To resume the stopped session. */
#define PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_RESUME ((uint8_t)0x21u)

/** @brief To recover the stopped session. */
#define PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_RECOVER ((uint8_t)0x22u)


/* Opcode list for UCI Proprietary Group. */
/** @brief To send a log message. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_LOG_NTF ((uint8_t)0x00u)

/** @brief To download radio configuration. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_RADIO_CONFIG_DOWNLOAD ((uint8_t)0x11u)

/** @brief To Activate Software Updater. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_ACTIVATE_SWUP ((uint8_t)0x12u)

/** @brief To read the last reboot event reason. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_GET_DEVICE_REBOOT_REASON ((uint8_t)0x13u)

/** @brief To start test mode. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_START_TEST ((uint8_t)0x20u)

/** @brief To stop test mode. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_STOP_TEST ((uint8_t)0x21u)

/** @brief Get UWB Device timestamp for time synchronization. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_QUERY_UWB_TIMESTAMP ((uint8_t)0x23u)

/** @brief To enter HPD mode. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_DEV_SUSPEND ((uint8_t)0x24u)

/** @brief To notify Loopback result. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_LOOPBACK_NTF ((uint8_t)0x25u)

/** @brief To configure CfgPage values. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_SET_APP_CFG_PAGE_VALUES ((uint8_t)0x26u)

/** @brief To get information about all the UWB sessions. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_GET_ALL_SESSIONS ((uint8_t)0x27u)

/** @brief To read CfgPage values. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_GET_APP_CFG_PAGE_VALUES ((uint8_t)0x28u)

/** @brief To store key-protection key in key slot. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_STORE_KEYPROTECTION_KEY ((uint8_t)0x29u)

/** @brief To set epoch id. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_SET_EPOCH_ID ((uint8_t)0x2Au)

/** @brief To send user defined range data notification. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_USER_DEFINED_RANGE_DATA_NTF ((uint8_t)0x2Cu)

/** @brief To set radar configuration. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_SET_RADAR_CONFIG ((uint8_t)0x30u)

/** @brief To get radar configuration. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_GET_RADAR_CONFIG ((uint8_t)0x31u)

/** @brief To run radar calibration. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_RUN_RADAR_CALIBRATION ((uint8_t)0x32u)

/** @brief Store the Trusted Third Party key. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_TUSP_STORE_TTP_KEY ((uint8_t)0x38u)

/** @brief Get the random data for key encryption from the device. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_TUSP_GET_ANCHOR_REQUESTS ((uint8_t)0x39u)

/** @brief Provide the encrypted long term keys to the device for storing. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_TUSP_STORE_LONGTERM_PAIRING_KEYS ((uint8_t)0x3Au)

/** @brief Set the Epoch ID used in key and traffic encryption. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_TUSP_SET_EPOCH_ID ((uint8_t)0x3Bu)

/** @brief To send an encrypted range data notification. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_ENCRYPTED_RANGE_DATA_NTF ((uint8_t)0x3Cu)

/** @brief Securely transfer data from the SE to the UWBD. */
#define PHSCA_UCIMSG_OID_PROPRIETARY_TUSP_TRANSMIT_ENCRYPTED_DATA ((uint8_t)0x3Du)

#define PHSCA_UCIMSG_OID_RANGE_START ((uint8_t)0x00u)

#define PHSCA_UCIMSG_OID_RANGE_STOP ((uint8_t)0x01u)

#define PHSCA_UCIMSG_OID_RANGE_NOTIFY ((uint8_t)0x02u)

#define PHSCA_UCIMSG_OID_RANGE_QUERY ((uint8_t)0x03u)

#define PHSCA_UCIMSG_OID_RANGE_SESSION_PARA_CONFIG ((uint8_t)0x04u)

#define PHSCA_UCIMSG_OID_RANGE_URSK_CONFIG ((uint8_t)0x05u)

#define PHSCA_UCIMSG_OID_RANGE_BLEMACADD_CONFIG ((uint8_t)0x06u)

#define PHSCA_UCIMSG_OID_RANGE_RESTART_CONFIG  ((uint8_t)0x07u)

#define PHSCA_UCIMSG_OID_CPD_START ((uint8_t)0x00u)

#define PHSCA_UCIMSG_OID_CPD_STOP ((uint8_t)0x01u)

#define PHSCA_UCIMSG_OID_CPD_NOTIFY ((uint8_t)0x02u)

#define PHSCA_UCIMSG_OID_CPD_QUERY ((uint8_t)0x03u)

#define PHSCA_UCIMSG_OID_GET_PRODUCT_INIF ((uint8_t)0x00u)

#define PHSCA_UCIMSG_OID_LOWPOWER_ENABLE ((uint8_t)0x01u)
/**@}*/

/**
 * @defgroup statusCode UCI status code lists
 * @{
 */
/**
 * @brief Generic Status code definitions
 * @details All other values are RFU (Reserved for Future Use).
 */
typedef enum __UCI_MSG_PACKED
{
	/** @brief Success */
	PHSCA_UCIMSG_GENERIC_STATUS_OK = ((uint8_t)0x0u),

	/** @brief Intended operation is not supported in the current state. */
	PHSCA_UCIMSG_GENERIC_STATUS_REJECTED = ((uint8_t)0x1u),

	/** @brief Intended operation failed. */
	PHSCA_UCIMSG_GENERIC_STATUS_FAILED = ((uint8_t)0x2u),

	/** @brief UCI packet structure is not per specification. */
	PHSCA_UCIMSG_GENERIC_STATUS_SYNTAX_ERROR = ((uint8_t)0x3u),

	/** @brief Parameter ID is correct and value is not specified. */
	PHSCA_UCIMSG_GENERIC_STATUS_INVALID_PARAM = ((uint8_t)0x4u),

	/** @brief Parameter ID is correct and value is not in the proper range. */
	PHSCA_UCIMSG_GENERIC_STATUS_INVALID_RANGE = ((uint8_t)0x5u),

	/** @brief UCI packet payload size is not as per specification. */
	PHSCA_UCIMSG_GENERIC_STATUS_INVALID_MESSAGE_SIZE = ((uint8_t)0x6u),

	/** @brief UCI Group ID is not per specification. */
	PHSCA_UCIMSG_GENERIC_STATUS_UNKNOWN_GID = ((uint8_t)0x7u),

	/** @brief UCI Opcode ID is not per specification. */
	PHSCA_UCIMSG_GENERIC_STATUS_UNKNOWN_OID = ((uint8_t)0x8u),

	/** @brief Parameter is read only. */
	PHSCA_UCIMSG_GENERIC_STATUS_READ_ONLY = ((uint8_t)0x9u),

	/** @brief UWBD requests retransmission from Application Processor. */
	PHSCA_UCIMSG_GENERIC_STATUS_COMMAND_RETRY = ((uint8_t)0x0Au),

	/** @brief A session does not exist. */
	PHSCA_UCIMSG_STATUS_SESSION_NOT_EXIST = ((uint8_t)0x11u),

	/** @brief A session already exists. */
	PHSCA_UCIMSG_STATUS_SESSION_DUPLICATE = ((uint8_t)0x12u),

	/** @brief A session is active. */
	PHSCA_UCIMSG_STATUS_SESSION_ACTIVE = ((uint8_t)0x13u),

	/** @brief The maximum number of sessions has been reached. */
	PHSCA_UCIMSG_STATUS_MAX_SESSIONS_EXCEEDED = ((uint8_t)0x14u),

	/** @brief A session is not configured with the mandatory app configurations. */
	PHSCA_UCIMSG_STATUS_SESSION_NOT_CONFIGURED = ((uint8_t)0x15u),

	/** @brief Sessions are actively running on UWBD. */
	PHSCA_UCIMSG_STATUS_ACTIVE_SESSIONS_ONGOING = ((uint8_t)0x16u),

	/** @brief Indicates when multicast list is full during one to many ranging. */
	PHSCA_UCIMSG_STATUS_ERROR_MULTICAST_LIST_FULL = ((uint8_t)0x17u),

	/** @brief Indicates when short address is not available in multicast list. */
	PHSCA_UCIMSG_STATUS_ERROR_ADDRESS_NOT_FOUND = ((uint8_t)0x18u),

	/** @brief Indicates when short address is already present in multicast list. */
	PHSCA_UCIMSG_STATUS_ERROR_ADDRESS_ALREADY_PRESENT = ((uint8_t)0x19u),

	/** @brief A negative distance was measured. */
	PHSCA_UCIMSG_STATUS_OK_NEGATIVE_DISTANCE_REPORT = ((uint8_t)0x1Bu),

	/** @brief Failed to transmit a UWB packet. */
	PHSCA_UCIMSG_STATUS_RANGING_TX_FAILED = ((uint8_t)0x20u),

	/** @brief No UWB packet detected by the receiver. */
	PHSCA_UCIMSG_STATUS_RANGING_RX_TIMEOUT = ((uint8_t)0x21u),

	/** @brief UWB Packet channel decoding error. */
	PHSCA_UCIMSG_STATUS_RANGING_RX_PHY_DEC_FAILED = ((uint8_t)0x22u),

	/** @brief Failed to detect time of arrival of the UWB packet from CIR samples. */
	PHSCA_UCIMSG_STATUS_RANGING_RX_PHY_TOA_FAILED = ((uint8_t)0x23u),

	/** @brief UWB packet STS segment mismatch. */
	PHSCA_UCIMSG_STATUS_RANGING_RX_RX_PHY_STS_FAILED = ((uint8_t)0x24u),

	/** @brief UWB packet MAC CRC or syntax error. */
	PHSCA_UCIMSG_STATUS_RANGING_RX_RX_MAC_DEC_FAILED = ((uint8_t)0x25u),

	/** @brief UWB packet IE syntax error. */
	PHSCA_UCIMSG_STATUS_RANGING_RX_RX_MAC_IE_DEC_FAILED = ((uint8_t)0x26u),

	/** @brief UWB packet Expected IE missing in the packet. */
	PHSCA_UCIMSG_STATUS_RANGING_RX_RX_MAC_IE_MISSING = ((uint8_t)0x27u),

	/** @brief Requested responder slot index is not supported in the requested configuration. */
	STATUS_INVALID_RESPONDER_SLOT_INDEX = ((uint8_t)0xA0u),

	/** @brief Requested slot length is not supported in the requested configuration. */
	PHSCA_UCIMSG_STATUS_SLOT_LEN_NOT_SUPPORTED = ((uint8_t)0xA1u),

	/** @brief Number of slots is invalid in the requested configuration. */
	PHSCA_UCIMSG_STATUS_INVALID_SLOT_PER_RR = ((uint8_t)0xA2u),

	/** @brief Invalid STS index. */
	PHSCA_UCIMSG_STATUS_INVALID_STS_IDX = ((uint8_t)0xA3u),

	/** @brief The responder is in listen only mode. */
	PHSCA_UCIMSG_STATUS_RESPONDER_IN_LISTEN_ONLY_MODE = ((uint8_t)0xA4u),

	/** @brief The responder is in listen only mode. */
	PHSCA_UCIMSG_STATUS_INVALID_RESPONDER_LISTEN_ONLY_MODE = ((uint8_t)0xA8u),

	/** @brief Radar delay is invalid in the requested configuration. */
	PHSCA_UCIMSG_STATUS_INVALID_RADAR_DELAY = ((uint8_t)0x50u),

	/** @brief Radar slot index is invalid in the requested configuration. */
	PHSCA_UCIMSG_STATUS_INVALID_RADAR_SLOT_INDEX = ((uint8_t)0x51u),

	/** @brief One or more radar calibration steps are missing to do the requested radar operation. */
	PHSCA_UCIMSG_STATUS_ERROR_RADAR_CALIBRATION_MISSING = ((uint8_t)0x52u),

	/** @brief Activating the session failed, as another radar enabled session is already ongoing. */
	PHSCA_UCIMSG_STATUS_ERROR_RADAR_SESSION_ACTIVE = ((uint8_t)0x53u),

	/** @brief Activating the session failed, no radar configuration index was set. */
	PHSCA_UCIMSG_STATUS_ERROR_RADAR_CONFIGURATION_MISSING = ((uint8_t)0x54u),

	/** @brief Activating the session failed, a radio configuration is missing. */
	PHSCA_UCIMSG_STATUS_ERROR_RADIO_CONFIGURATION_MISSING = ((uint8_t)0x55u),

	/** @brief The selected responder slot index is invalid. */
	PHSCA_UCIMSG_STATUS_INVALID_RESPONDER_SLOT_INDEX = ((uint8_t)0xE0u),

	/** @brief The ranging consistency check failed. */
	PHSCA_UCIMSG_STATUS_RANGING_CONSISTENCY_CHECK_FAILED = ((uint8_t)0xE1u),

	/** @brief Ranging failed due a ranging watchdog timeout. */
	PHSCA_UCIMSG_STATUS_RANGING_WDT_TIMEOUT = ((uint8_t)0xE2u),

	/** @brief The configured adaptive hopping threshold is invalid. */
	PHSCA_UCMSG_STATUS_INVALID_ADAPTIVE_HOPPING_THRESHOLD = ((uint8_t)0xE3u),

	/** @brief Ranging failed because of a baseband error. */
	PHSCA_UCMSG_STATUS_RANGING_BASEBAND_ERROR = ((uint8_t)0xE4u),
} phscaUciMsg_GenericStatusCode_t;

/**
 * @brief Various state values for SESSION_STATUS_NTF.
 */
typedef enum __UCI_MSG_PACKED
{
	/** @brief Session successfully initialized. */
	PHSCA_UCIMSG_SESSION_STATUS_NTF_INIT = ((uint8_t)0x00u),

	/** @brief Session successfully de-initialized. */
	PHSCA_UCIMSG_SESSION_STATUS_NTF_DEINIT = ((uint8_t)0x01u),

	/** @brief Session successfully activated. */
	PHSCA_UCIMSG_SESSION_STATUS_NTF_ACTIVE = ((uint8_t)0x02u),

	/** @brief Session ready for ranging. */
	PHSCA_UCIMSG_SESSION_STATUS_NTF_IDLE = ((uint8_t)0x03u),

	/** @brief Session error. */
	PHSCA_UCIMSG_SESSION_ERROR = ((uint8_t)0xFFu),
} phscaUciMsg_SessionStatusCode_t;

/**
 * @brief Reason Codes for session state change.
 */
typedef enum __UCI_MSG_PACKED
{
	/** @brief Session state change caused by a management command. */
	PHSCA_UCIMSG_SESSION_RSN_SESSION_MANAGE_CMD = ((uint8_t)0x00u),

	/** @brief Session terminated on maximal number of resync attempts. */
	PHSCA_UCIMSG_SESSION_RSN_TERMINATION_ON_MAX_RR_RETRY = ((uint8_t)0x01u),

	/** @brief Session terminated on reaching the maximal number of ranging blocks. */
	PHSCA_UCIMSG_SESSION_RSN_TERMINATION_ON_MAX_BLOCKS = ((uint8_t)0x02u),

	/** @brief Failure while fetching crypto keys. */
	PHSCA_UCIMSG_SESSION_RSN_KEY_FETCH_FAILURE = ((uint8_t)0x81u),

	/** @brief Baseband error. */
	PHSCA_UCIMSG_SESSION_RSN_BASEBAND_ERROR = ((uint8_t)0x84u),

	/** @brief Testmode terminated. */
	PHSCA_UCIMSG_SESSION_RSN_TESTMODE_TERMINATED = ((uint8_t)0x85u),

	/** @brief URSK expired. */
	PHSCA_UCIMSG_SESSION_RSN_URSK_EXPIRED = ((uint8_t)0xA0u),

	/** @brief Session terminated on reaching maximal STS index. */
	PHSCA_UCIMSG_SESSION_RSN_TERMINATION_ON_MAX_STS = ((uint8_t)0xA1u),
} phscaUciMsg_SessionStatusChangeReason_t;

/**
 * @brief Various state values for SET_APP_CFG_PAGE_VALUES_NTF.
 */
typedef enum __UCI_MSG_PACKED
{
	/** @brief CfgPage values from flash or received with command have been applied successfully. */
	PHSCA_UCIMSG_CFGPAGEVAL_APPLIED = ((uint8_t)0x00u),

	/** @brief CfgPage values saved in NVM were corrupted and were overwritten with defaults. */
	PHSCA_UCIMSG_CFGPAGEVAL_CORRUPTED = ((uint8_t)0x01u),

	/** @brief Error while applying CfgPage values. */
	PHSCA_UCIMSG_CFGPAGEVAL_FAILURE = ((uint8_t)0x02u),

	/** @brief Error while applying groupdelay to CfgPage values. All other values are successfully updated. */
	PHSCA_UCIMSG_CFGPAGEVAL_FAILURE_GROUPDELAY = ((uint8_t)0x03u),

	/** @brief Error while accessing the NVM FLASH page. */
	PHSCA_UCIMSG_CFGPAGEVAL_NVM_ERROR = ((uint8_t)0x04u),

	/** @brief Error while setting the current limit value. */
	PHSCA_UCIMSG_CFGPAGEVAL_FAILURE_CURRENT_LIMITER = ((uint8_t)0x05u),

	/** @brief No application configuration page value notification. */
	PHSCA_UCIMSG_CFGPAGEVAL_NONE = ((uint8_t)0xFFu),
} phscaUciMsg_CfgPageValStatusCode_t;

/**
 * @brief Various state values for CONTROLLER_MULTICAST_LIST_UPDATE_NTF.
 */
typedef enum __UCI_MSG_PACKED
{
	/** @brief STATUS_OK_MULTICAST_LIST_UPDATE */
	PHSCA_UCIMSG_MULTICAST_LIST_UPDATE_STATUS_NTF_OK = ((uint8_t)0x00u),

	/** @brief STATUS_ERROR_MULTICAST_LIST_FULL */
	PHSCA_UCIMSG_MULTICAST_LIST_UPDATE_STATUS_NTF_ERROR_LIST_FULL = ((uint8_t)0x01u),

	/** @brief STATUS_ERROR_KEY_FETCH_FAIL (not supported) */
	PHSCA_UCIMSG_MULTICAST_LIST_UPDATE_STATUS_NTF_ERROR_KEY_FETCH_FAIL = ((uint8_t)0x02u),

	/** @brief STATUS_ERROR_SUB_SESSION_ID_NOT_FOUND (not supported) */
	PHSCA_UCIMSG_MULTICAST_LIST_UPDATE_STATUS_NTF_ERROR_SUB_SESSION_ID_NOT_FOUND = ((uint8_t)0x03u)
} phscaUciMsg_MulticastListUpdateStatusCode_t;

typedef enum __UCI_MSG_PACKED
{
	UCIMSG_WORK_STATE_IDLE = 0,
	UCIMSG_WORK_STATE_SEND_CMD,
	UCIMSG_WORK_STATE_WAIT_RSP,
	UCIMSG_WORK_STATE_RSP_TIMEOUT
}uciMsg_WorkStatus_t;

typedef struct __UCI_MSG_PACKED{
	uint16_t distacne[4];
	uint16_t rssiPoll;
	uint16_t rssiFinal;
}ucimsg_RangeFormat_t;

typedef struct __UCI_MSG_PACKED{
	uint8_t result;
	float   motion;
	float   frq;
	float 	amp_bfeature;/*!< 呼吸特征值*/
	float 	phs_bfeature;
}ucimsg_CpdFormat_t;

#define UCIMSG_PACK_LENGTH	512u
#define UCIMSG_CMD_BUFF_LEN	5
#define UCIMSG_RSP_BUFF_LEN	2
#define UCIMSG_NTF_BUFF_LEN	20
typedef struct __UCI_MSG_PACKED
{
	uint8_t	gid	:4;
	uint8_t pbf	:1;
	uint8_t	msgType :3;
	uint8_t oid	:6;
	uint8_t rfu1 :2;
	uint8_t payloadLength_H;
	uint8_t payloadLength;
	uint8_t payload[UCIMSG_PACK_LENGTH-4];
}uciMsg_PackFormat_t;

typedef struct __UCI_MSG_PACKED
{
	uint8_t sendHead;
	uint8_t sendHeadRoundCnt;
	uint8_t sendEnd;
	uint8_t sendEndRoundCnt;
	uint32_t waitRspTimeout_T;
	uint8_t waitRsping;
	union
	{
		uint8_t uciData[UCIMSG_PACK_LENGTH];
		uciMsg_PackFormat_t uciInfo;
	}uciMsg[UCIMSG_CMD_BUFF_LEN];
}uciMsg_UciMsgCMDPack_t;

typedef struct __UCI_MSG_PACKED
{
	uint8_t sendHead;
	uint8_t sendHeadRoundCnt;
	uint8_t sendEnd;
	uint8_t sendEndRoundCnt;
	uint8_t rspCanSending;
	uint32_t rspCanSendTimeout_T;
	union
	{
		uint8_t uciData[UCIMSG_PACK_LENGTH];
		uciMsg_PackFormat_t uciInfo;
	}uciMsg[UCIMSG_RSP_BUFF_LEN];
}uciMsg_UciMsgRSPPack_t;

typedef struct __UCI_MSG_PACKED
{
	uint8_t sendHead;
	uint8_t sendHeadRoundCnt;
	uint8_t sendEnd;
	uint8_t sendEndRoundCnt;
	uint8_t notifCanSending;
	uint32_t notifCanSendTimeout_T;
	union
	{
		uint8_t uciData[UCIMSG_PACK_LENGTH];
		uciMsg_PackFormat_t uciInfo;
	}uciMsg[UCIMSG_NTF_BUFF_LEN];
}uciMsg_UciMsgNTFPack_t;

typedef enum
{
		UWBPRESTART_REQ_NO_REQUEST = 0,
		UWBPRESTART_REQ_PRESTART,
		UWBPRESTART_REQ_INVALID,
		RESERVED,
} UWBPRESTART_REQUEST;

typedef enum
{
		UWBLDTEN_REQ_ENABLE_START_AFTERRUN = 0,
		UWBLDTEN_REQ_ENABLE_NORMAL,
		UWBLDTEN_REQ_DIABLE,
		UWBLDTEN_REQ_NO_REQUEST,
		UWBLDTEN_REQ_PREPARE,
		RESERVED_1,
		RESERVED_2,
		RESERVED_3,
} UWBLDTEN_REQUEST;

typedef enum
{
    CPD_RET_NONE = 0,   /*!< 无呼吸/运动*/
    CPD_RET_BREATH,     /*!< 检测到呼吸*/
    CPD_RET_MOTION      /*!< 检测到运动*/
}CPD_RetEnum;

extern uciMsg_UciMsgCMDPack_t uciMsg_Cmd;
extern uciMsg_UciMsgRSPPack_t uciMsg_Rsp;
extern uciMsg_UciMsgNTFPack_t uciMsg_Ntf;

extern ucimsg_RangeFormat_t rangeResult;
extern ucimsg_CpdFormat_t cpdResult;
extern uint8_t rangeStartingFlg;
extern uint8_t cpdStartingFlg;
extern uint8_t rangeResultUpdataFlag;
extern uint8_t cpdResultUpdataFlag;

extern uciMsg_WorkStatus_t uciMsg_GetUciMsgWorkState(void);

extern void uciMsg_SetUciMsgWorkState(uciMsg_WorkStatus_t state_val);

extern void uciMsg_UciMsgCmdTx();

extern void uciMsg_UciMsgErrorStateNotif(uint8_t State);

extern void uciMsg_cmdHandler(void);

extern void uciMsg_rspNotifHandler(void);

extern uint8_t UciMsgCore_Indication(uint8_t const opcodeId);

extern uint8_t uciMsgPropri_Indication(uint8_t const opcodeId);

extern uint8_t uciMsgRanging_IndicationRSP(uint8_t const opcodeId);

extern uint8_t uciMsgCPD_IndicationRSP(uint8_t const opcodeId);

extern uint8_t uciMsgCustom_IndicationRSP(uint8_t const opcodeId);

extern uint8_t uciMsgSession_Indication(uint8_t const opcodeId);

extern void uciMsgCPD_IndicationHandle(void);

extern void uciMsgRanging_IndicationHandle(void);

extern void uciMsgCustom_IndicationHandle(void);

extern uint8_t uciMsgCustom_rspTimeoutHandle(uint8_t const opcodeId);

extern uint8_t uciMsgCPD_rspTimeoutHandle(uint8_t const opcodeId);

extern uint8_t uciMsgRanging_rspTimeoutHandle(uint8_t const opcodeId);

/*UWB 访问控制接口*/
void uciMsgCustom_LowPowerEnableCMD(void);

void uciMsgCustom_GetProductInfoCMD(void);

void uciMsgRanging_RangeURSKcfgCMD(void);

void uciMsgRanging_RangeSessionParaCfgCMD(void);

void uciMsgRanging_RangeBleMacAddcfgCMD(void);

void uciMsgRanging_RangeStartCMD();

void uciMsgRanging_RangeStopCMD(void);

void uciMsgCPD_CPDStartCMD(void);

void uciMsgCPD_CPDStopCMD(void);

void uciMsg_Init(void);

#endif /* NCJ29D6_UCIMESSAGE_H_ */
