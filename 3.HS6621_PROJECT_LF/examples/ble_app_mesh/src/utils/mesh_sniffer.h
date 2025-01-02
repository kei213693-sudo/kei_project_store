/**
 ****************************************************************************************
 *
 * @file mesh_sniffer.h
 *
 * @brief BLE Mesh Sniffer.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

#ifndef __MESH_SNIFFER_H__
#define __MESH_SNIFFER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define CONFIG_APP_SNIFFER 0

#if CONFIG_APP_SNIFFER
/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * MACROS
 */
// Enable network layer send 2.4G packet
#define CONFIG_APP_SNIFFER_NETWORK_RELAY    0
// Enable bearer layer send 2.4G packet
#define CONFIG_APP_SNIFFER_BEARER_RELAY     0
// Set address network use
#define ADDR_NETWORK_SEND_24G (0xFFEE)

/*********************************************************************
 *  SYS MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * EXTERN VARIABLES
 */

/*********************************************************************
 * EXTERN FUNCTIONS
 */

typedef unsigned char uint8_t;

uint8_t board_api_sniffer_ctl(uint8_t pdu_type, uint8_t *p_adv_data,uint8_t len);

uint8_t hsm_network_rec_24g_realyed_setself_attr(uint8_t *rec_data,uint8_t len);

#endif /* CONFIG_APP_SNIFFER */

#endif /* __MESH_SNIFFER_H__ */

/** @} */

