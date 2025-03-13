/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_confdefs.h
 * 摘要: Preprocessor definitions derived from the CAN Data Link Layer Interface's configuration.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDSCAN_CONFDEFS1_H
#define _UDSCAN_CONFDEFS1_H

#include <stdint.h>
#include "uds_api.h"
/*lint -e750*/

/******************************************************************************
*
* General definitions
*
******************************************************************************/

/* Enable aborting the ongoing transmission of a CAN message which is pending at the CAN
 * hardware module and not transmitted yet.
 *
 * [Note]:
 * This preprocessor definition is used when user application requires/can abort the ongoing
 * can message transmission in CAN hardware module.
 * If this functionality is not used, just undefine it.
 *
 * 0x01 - Enabling this functionality
 * 0x00 - This functionality disabled
 */
#define UDS0_CAN_HW_TX_ABORT		0x00u

/******************************************************************************
*
* CAN channel queue definitions
*
******************************************************************************/

/* Define number of channels for each UdsCan session
 *
 * one channel for receive, the other one for transmit
 */
#define UDS0_CAN_NUM_CHANNELS 		0x02u

/* Allocate buffers for Rx direction
 *
 * UDS0_CAN_RX_NUMBUF: number of buffers to store received messages from outside
 * UDS0_CAN_RX_START:  start position in buffers for Rx
 * UDS0_CAN_RX_END:	end position in buffers for Rx
 */
#define UDS0_CAN_RX_NUMBUF			0x10u
#define UDS0_CAN_RX_START			0x00u
#define UDS0_CAN_RX_END				(UDS0_CAN_RX_START + UDS0_CAN_RX_NUMBUF - 0x01u)

/* Allocate buffers for Tx direction
 *
 * UDS0_CAN_TX_NUMBUF: number of buffers to store transmit messages to outside
 * UDS0_CAN_TX_START:  start position in buffers for Tx
 * UDS0_CAN_TX_END:	end position in buffers for Tx
 */
#define UDS0_CAN_TX_NUMBUF			0x01u
#define UDS0_CAN_TX_START			(UDS0_CAN_RX_END   + 0x01u)
#define UDS0_CAN_TX_END				(UDS0_CAN_TX_START + UDS0_CAN_TX_NUMBUF - 0x01u)

#define UDS0_CAN_QUEUEALL_NUMBUF 	(UDS0_CAN_RX_NUMBUF + UDS0_CAN_TX_NUMBUF)

/* Define of CAN message IDs for Rx/Tx direction for each UdsTp session
 *
 * UDS0_CAN_RX_MSGID_PHY: message ID for Rx direction for UdsTp0 session using physical TA type.
 * UDS0_CAN_RX_MSGID_FNC: message ID for Rx direction for UdsTp0 session using functional TA type.
 * UDS0_CAN_TX_MSGID_PHY: message ID for Tx direction for UdsTp0 session using physical TA type.
 * UDS0_CAN_TX_MSGID_FNC: message ID for Tx direction for UdsTp0 session using functional TA type.
 *
 * For both a 11bit ID and a 29bit ID: bit0 of ID will be placed in bit0 of below defined values.
 *
 * [Note]:
 *	  The determination of these IDs are from N_AI information. We use staticly defined ID values
 *	  rather than dynamiclly calculated is because on one hand for an implementation of vehicle network,
 *    the N_AI information always predefined/fixed. Also using staticlly defined values saves CPU execution time.
 * 1. The Rx MSGIDs are used for checking received CAN messages by UdsCan module, and to allocate received messages
 *    to the dedicated buffers in case multi-session configured and used.
 * 2. The Tx MSGIDs are used to be copied into canID of Tx buffer of UdsCan_QueueBufPtr type by UdsTp module.
 */

/*NIO七个产品对应功能是否使能如果使能则定义否则关闭*/
//#define UdsApi_BTA_R_FUN_EN	    		(1u)
//#define UdsApi_BTA_L_FUN_EN				(1u)
/*后保左45度*/
//#define UdsApi_EXTERNAL_BTA_RB2_EN     	(1u)
//#define UdsApi_BTA_SRF_FUN_EN				(1u)
#define UdsApi_BTA_SRR_FUN_EN				(1u)
/*前保左45度*/
//#define UdsApi_EXTERNAL_BTA_FB_FUN_EN		(1u)
//#define UdsApi_BTA_RB_FUN_EN				(1u)

 /* currently, we will use request 0x7E0u and response 0x7E8u */
#ifdef  UdsApi_BTA_SRF_FUN_EN
	#define UDS0_CAN_RX_MSGID_PHY			(uint32_t)(0x670u)	/* TODO */ /* CANID_REQ_PHYS */
	#define UDS0_CAN_RX_MSGID_FNC			(uint32_t)(0x601u)	/* TODO */ /* CANID_REQ_FUNC */
	#define UDS0_CAN_TX_MSGID_PHY			(uint32_t)(0x6f0u)	/* TODO */ /* CANID_RES_PHYS */
#endif

#ifdef  UdsApi_BTA_R_FUN_EN
	#define UDS0_CAN_RX_MSGID_PHY			(uint32_t)(0x672u)	/* TODO */ /* CANID_REQ_PHYS */
	#define UDS0_CAN_RX_MSGID_FNC			(uint32_t)(0x601u)	/* TODO */ /* CANID_REQ_FUNC */
	#define UDS0_CAN_TX_MSGID_PHY			(uint32_t)(0x6f2u)	/* TODO */ /* CANID_RES_PHYS */
#endif

#ifdef  UdsApi_BTA_L_FUN_EN
#define UDS0_CAN_RX_MSGID_PHY			(uint32_t)(0x671u)	/* TODO */ /* CANID_REQ_PHYS */
#define UDS0_CAN_RX_MSGID_FNC			(uint32_t)(0x601u)	/* TODO */ /* CANID_REQ_FUNC */
#define UDS0_CAN_TX_MSGID_PHY			(uint32_t)(0x6f1u)	/* TODO */ /* CANID_RES_PHYS */
#endif


#ifdef  UdsApi_BTA_SRR_FUN_EN
#define UDS0_CAN_RX_MSGID_PHY			(uint32_t)(0x673u)	/* TODO */ /* CANID_REQ_PHYS */
#define UDS0_CAN_RX_MSGID_FNC			(uint32_t)(0x601u)	/* TODO */ /* CANID_REQ_FUNC */
#define UDS0_CAN_TX_MSGID_PHY			(uint32_t)(0x6f3u)	/* TODO */ /* CANID_RES_PHYS */
#endif

#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
#define UDS0_CAN_RX_MSGID_PHY			(uint32_t)(0x67cu)	/* TODO */ /* CANID_REQ_PHYS */
#define UDS0_CAN_RX_MSGID_FNC			(uint32_t)(0x601u)	/* TODO */ /* CANID_REQ_FUNC */
#define UDS0_CAN_TX_MSGID_PHY			(uint32_t)(0x6fcu)	/* TODO */ /* CANID_RES_PHYS */
#endif

#ifdef UdsApi_BTA_RB_FUN_EN
#define UDS0_CAN_RX_MSGID_PHY			(uint32_t)(0x674u)	/* TODO */ /* CANID_REQ_PHYS */
#define UDS0_CAN_RX_MSGID_FNC			(uint32_t)(0x601u)	/* TODO */ /* CANID_REQ_FUNC */
#define UDS0_CAN_TX_MSGID_PHY			(uint32_t)(0x6f4u)	/* TODO */ /* CANID_RES_PHYS */
#endif

#define UDS0_CAN_TX_MSGID_FNC			(uint32_t)(0x04u)	/* TODO */ /* for a server this will never be used */

#endif /* _UDSCAN_CONFDEFS_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
