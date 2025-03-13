/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_priv.h
 * 摘要: Internal definitions and declarations
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDSCAN_PRIV_H
#define _UDSCAN_PRIV_H

#include "uds_target.h"

/*lint -e749*//*lint -e750*//*lint -e751*//*lint -e754*/

/******************************************************************************
*
* Preprocessor definitions
*
******************************************************************************/

/******************************************************************************
*
* Type definitions
*
******************************************************************************/

/* This holds the state of an instance of UdsCan_QueueBuf_t.
 * The lower 4 bits contain a value from UdsCan_QueueBufStates_t. */
typedef UDS_STATE_TYPE uint8_t UdsCan_QueueBufProps_t;

typedef enum
{
    UDSCAN_TXRXFREE = 0x00u,        /* The buffer is available to store TX or RX data. */
									/* For a TX buffer this means a tx confirmation has been received. */
    UDSCAN_TXALLOC,                	/* The buffer has been allocated to store TX data, but it is not yet ready to be transmitted. */
    UDSCAN_TXNOTSENT,              	/* The buffer is ready to be transmitted. */
    UDSCAN_TXINPROGRESS,           	/* The buffer is being transmitted. */
    UDSCAN_RXDATA                  	/* The buffer is contains RX data. */
} UdsCan_QueueBufStates_t;

typedef struct
{
	uint32_t					canID;
	uint8_t					    dlc;
    uint8_t                  	msgBuffer[64];       /*-----------�޸ı�־------------*/
} UdsCan_QueueBuf_t;

typedef UDS_STATE_TYPE struct
{
    /* For a TX queue this represents the tail of the queue. The Uds Tp writes CAN messages into the queue
     * at this position in the session's buffer array.
     * For an RX queue this represents the head of the queue. The Uds Tp reads CAN messages from the queue
     * at this position in the session's buffer array. */
    uint8_t                  ctUdsPos;

    /* For a TX queue this represents the head of the queue. The CAN driver reads CAN messages from the queue
     * at this position in the session's buffer array.
     * For an RX queue this represents the tail of the queue. The CAN driver writes CAN messages into the queue
     * at this position in the session's buffer array. */
    uint8_t                  ctCanPos;
} UdsCan_QueuePos_t;
#ifdef NIO_PRJ
typedef UDS_CONFIG_TYPE struct	/*lint !e963*/
{
    uint8_t			idxStart;   /* The first element of this channel's queue space in the array pointed to by UdsCan_SessionCfg_t::pQueueBuffers */
    uint8_t			idxEnd;     /* The last element of this channel's queue space in the array pointed to by UdsCan_SessionCfg_t::pQueueBuffers */
    uint32_t		msgIdPhy;   /* The CAN message ID used for sending or receiving messages on this channel(physical addressing type). */
	uint32_t		msgIdFnc;	/* The CAN message ID used for sending or receiving messages on this channel(functional addressing type). */
} UdsCan_ChannelCfg_t;
#else
typedef  struct	/*lint !e963*/
{
    uint8_t			idxStart;   /* The first element of this channel's queue space in the array pointed to by UdsCan_SessionCfg_t::pQueueBuffers */
    uint8_t			idxEnd;     /* The last element of this channel's queue space in the array pointed to by UdsCan_SessionCfg_t::pQueueBuffers */
    uint32_t		msgIdPhy;   /* The CAN message ID used for sending or receiving messages on this channel(physical addressing type). */
	uint32_t		msgIdFnc;	/* The CAN message ID used for sending or receiving messages on this channel(functional addressing type). */
} UdsCan_ChannelCfg_t;
#endif
typedef UDS_CONFIG_TYPE struct	/*lint !e963*/
{
    UdsCan_ChannelCfg_t*        pChannelCfgs;       /* Points to an array with one entry each of Tx and Rx channel. */
    UdsCan_QueuePos_t*          pQueuePositions;    /* Points to an array with one entry per channel. */
    UdsCan_QueueBuf_t*          pQueueBuffers;      /* Points to an array with one entry per queue element. */
    UdsCan_QueueBufProps_t*     pQueueBufferStates; /* Points to an array with one entry per queue element. */
	uint8_t						numChannels;
} UdsCan_SessionCfg_t;

/******************************************************************************
*
* Variables
*
******************************************************************************/


/******************************************************************************
*
* Prototypes
*
******************************************************************************/


#endif /* _UDSCAN_PRIV_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
