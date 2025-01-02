/**
 * @file tr_ble_sim.h
 * @brief 
 * @date Fri Dec 23 11:07:06 2022
 * @author liqiang
 *
 * @defgroup 
 * @ingroup 
 * @brief 
 * @details 
 *
 * @{
 */

#ifndef __TR_BLE_SIM_H__
#define __TR_BLE_SIM_H__

#ifdef __cplusplus
extern "C"
{ /*}*/
#endif

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"


/*********************************************************************
 * MACROS
 */
#define TR_BLE_RX_FIXED_LENGTH          12  // scan_req: 12bytes

// agc idle is sync detected
#define TR_BLE_SYNC_DETECTED()          ((HS_DAIF->DBG_REG & (0xF<<16)) == 0)

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * EXTERN VARIABLES
 */


/*********************************************************************
 * EXTERN FUNCTIONS
 */

/**
 * @brief  ll ble aa generate
 *
 * @param[in] aa  aa
 **/
void tr_ble_aa_set(uint32_t aa);

/**
 * @brief  ll ble packet generate
 *
 * Input:
 *   crc_seed = 0x555555
 *   channel_index = 37
 *   header0 = 0x40
 *   payload[] = {0x0D, 0xBF, 0x66, 0x21, 0xD0, 0xDD,   0x02, 0x01, 0x06, 0x03, 0x09, 0x58, 0x58};
 *   length = sizeof(payload)
 *
 * Output:
 *   Raw Data             40 0D 0D BF 66 21 D0 DD 02 01 06 03 09 58 58 53 A9 54
 *   Whitened Raw Data    CD DF 5A 1E 5B 86 B6 6D 77 30 17 4B 9F 2F A0 B0 EF BD
 *   Bitordered Raw Data  B3 FB 5A 78 DA 61 6D B6 EE 0C E8 D2 F9 F4 05 0D F7 BD
 *
 * @param[in] crc_seed  crc seed
 * @param[in] channel_index  channel index
 * @param[in] header0  header0
 * @param[in] payload  payload
 * @param[in] length  length
 * @param[in] packet  packet
 *
 * @return length
 **/
uint32_t tr_ble_pdu_generate(uint32_t crc_seed, uint32_t channel_index,
        uint8_t header0, const uint8_t *payload, uint8_t length, uint8_t packet[32]);

/**
 * @brief  ll ble packet parse
 *
 * @param[in] crc_seed  crc seed
 * @param[in] channel_index  channel index
 * @param[in] packet  packet
 * @param[in] length  length
 *
 * @return  length, -1: error
 **/
int tr_ble_packet_parse(uint32_t crc_seed, uint32_t channel_index, uint8_t packet[32], uint32_t length);

/**
 * @brief  init the 2.4g transerive when the event begin
 **/
void tr_ble_init(void);

/**
 * @brief  tr ble enable
 *
 * @param[in] enable  enable
 **/
void tr_ble_enable(bool enable);

#ifdef __cplusplus
/*{*/ }
#endif

#endif

/** @} */

