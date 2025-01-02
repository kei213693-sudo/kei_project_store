/**
 ****************************************************************************************
 *
 * @file board_sniffer.c
 *
 * @brief BLE mesh board sniffer entry.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

/*********************************************************************
 * INCLUDES
 */

#include "m_defines.h"
#include "m_tb.h"
#include "m_lay.h"
#include "m_bearer.h"

#if CONFIG_APP_SNIFFER
/*********************************************************************
 * MACROS
 */
/// The length of plain fields in AdvData: ZJDK's uuid or OM's manufacture
#define LE_LIGHT_ADVDATA_ZJ_PLAIN_LEN   (5)
#define LE_LIGHT_ADVDATA_OM2_PLAIN_LEN  (5)
#define LE_LIGHT_ADVDATA_OM4_PLAIN_LEN  (7)
/// The length of 2.4G packet header: Telink or Onmicro
#define LE_LIGHT_ADVDATA_TE24G_HDR_LEN  (6)
#define LE_LIGHT_ADVDATA_OM24G_HDR_LEN  (8)
/// Advdata type
#define M_ADV_NONCONN_IND               (0x09) /* GAPM_REPORT_TYPE_ADV_LEG | GAPM_REPORT_INFO_COMPLETE_BIT */
#define M_ADV_IND                       (0x39) /* GAPM_REPORT_TYPE_ADV_LEG | GAPM_REPORT_INFO_COMPLETE_BIT
                                                | GAPM_REPORT_INFO_CONN_ADV_BIT | GAPM_REPORT_INFO_SCAN_ADV_BIT */
/*********************************************************************
 * TYPEDEFS
 */

/* ZJDK, ZGDK, OM.Light */
typedef struct cxw_frame {
    uint8_t phoneId[2];
    uint8_t groupId;
    uint8_t cmd;
    uint8_t tid;
    uint8_t data[3];
} cxw_frame_t;
#define CXW_24G_PREAMBLE3               0x710F55
#define CXW_24G_AA3                     0xCC55AA
#define CXW_CMD_clearCode               0xB0
#define CXW_CMD_close                   0xB2
#define CXW_CMD_open                    0xB3
#define CXW_CMD_pairCode                0xB4
#define CXW_CMD_setBrightness           0xB5 //{value3B}
#define CXW_CMD_setColorTemperature     0xB7 //{value3B}
#define CXW_CMD_setFanIndexDefault      0xD0
#define CXW_CMD_setFanIndex2            0xD1
#define CXW_CMD_setFanIndex1            0xD2
#define CXW_CMD_setFanIndex0            0xD8
#define CXW_CMD_setFanModeDefault       0xD9
#define CXW_CMD_setFanMode11            0xDA
#define CXW_CMD_setFanMode10            0xD7
#define CXW_CMD_setFanMode9             0xD6
#define CXW_CMD_setFanMode8             0xD5
#define CXW_CMD_setFanMode7             0xD4
#define CXW_CMD_setFanMode6             0xA7 //{1|2|3, 0x00, 0x00}
#define CXW_CMD_setFanMode4             0xA6 //{1|2,  0x00, 0x00}
#define CXW_CMD_setFanMode3             0xA1 //{0x19, 0x19, 0x00}
#define CXW_CMD_setFanMode2             0xA4 //{0x7B, 0x7B, 0x00}
#define CXW_CMD_setFanMode1             0xA2 //{0x00, 0xFF, 0x00}
#define CXW_CMD_setFanMode0             0xA3 //{0xFF, 0xFF, 0x00}
#define CXW_CMD_setMoreMode8            0xA9
#define CXW_CMD_setMoreMode7            0xA8
#define CXW_CMD_setMoreMode6            0xA7 //{1|2|3, 0x00, 0x00}
#define CXW_CMD_setMoreMode4            0xA6 //{1|2,  0x00, 0x00}
#define CXW_CMD_setMoreMode3            0xA1191900
#define CXW_CMD_setMoreMode2            0xA47B7B00
#define CXW_CMD_setMoreMode1            0xA200FF00
#define CXW_CMD_setMoreMode0            0xA3FFFF00
#define CXW_CMD_setTiming               0xA5 //{hour, min, sec}
#define CXW_CMD_cancelTiming            0x55

/*********************************************************************
 * CONSTANTS
 */

/* A user specific. */
static const uint8_t m_telink_button[]  = {0x1e, 0xFF, 0x5b, 0x55};
static const uint8_t m_hs6206_button[]  = {0x04, 0xFF, 0xF1, 0xFF};
static const uint8_t m_sc_app_pattern[] = {0x02, 0x01, 0x02, 0x1b, 0xff, 0xf0, 0xff, 0xb6, 0x5f, 0x2b, 0x5e, 0x00, 0xfc, 0x31, 0x51}; //SC w/ whiten 2.4G header

/* OnMicro specific. */
//static const uint8_t m_rw_app_pattern[] = {0x02, 0x01, 0x02, 0x1b, 0xff, 0xdd, 0xb2, 0xda, 0x6c, 0x9f, 0x01, 0x7a, 0x34}; //RW w/ whiten 2.4G header: P1=55 AA5=529c546e55 G2=5555
static const uint8_t m_hs_app_pattern[] = {0x02, 0x01, 0x02, 0x1b, 0xff, 0x22, 0xe1, 0x21, 0xb3, 0x39, 0xc9, 0xb2, 0xfc}; //HSLE: P1=AA AA5=9843AF0B46 G2=4646
static const uint8_t m_zj_app_pattern[] = {0x02, 0x01, 0x02, 0x11, 0x07, 0xf9, 0x08, 0x49}; //ZJDK w/ whiten 2.4G header
static const uint8_t m_om_app_pattern[] = {0x02, 0x01, 0x02, 0x15, 0xff, 0xf1, 0xff, 0xb6, 0x5f, 0x2b, 0x5e, 0x00, 0xfc, 0x31, 0x51}; //OMDK w/ whiten 2.4G header
static const uint8_t m_ex_uuid_pattern[] = {0x02, 0x01, 0x02, 0x11, 0x07, 0xf9, 0x08, 0x49, 0xb2, 0xce, 0x2c, 0x77, 0x2f, 0x98, 0x84, 0x16, 0x41, 0x14, 0x29, 0xb9, 0xfc}; //exclude a special data pattern

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8_t m_latest_adv_data[31];

/// Borrow Structure for Mesh Network Layer API functions(Don't Modfiy)
typedef struct m_lay_net_api_
{
    /// Send PDU API function
    m_lay_trans_send_api send;
} m_lay_net_api_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void cxw8_handle_board(uint8_t cmd,uint32_t attr_value);
extern int lelt_decrypt(uint8_t *pData, int length, int offset);

#if CONFIG_APP_SNIFFER_BEARER_RELAY
static void hsm_bearer_send_24g_force(uint8_t *enc_data,uint8_t len)
{
    uint16_t status = MESH_ERR_NO_ERROR;
    mesh_tb_buf_t *p_buf;

    status = mesh_tb_buf_alloc(&p_buf,2,len,0);
    if(status != MESH_ERR_NO_ERROR)
    {
        return;
    }
    // Copy data
    memcpy(MESH_TB_BUF_DATA(p_buf), enc_data, len);
    // Send the packet
    uint8_t* p_data = MESH_TB_BUF_DATA(p_buf);

    status = mal_adv_api.send(0xfe, M_ADV_INTERVAL, M_ADV_NB_TX, p_buf->data_len, p_data);
    if(status != MESH_ERR_NO_ERROR)
    {
        __LOG(LOG_SRC_APP, LOG_LEVEL_INFO,"Bearer can not relay 24G packet because %x\n",status);
    }
    mesh_tb_buf_release(p_buf);

    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO,"Bearer layer relay 2.4G packet\n");
}
#else
static void hsm_bearer_send_24g_force(uint8_t *enc_data,uint8_t len){}
#endif /*  CONFIG_APP_SNIFFER_BEARER_RELAY */

#if CONFIG_APP_SNIFFER_NETWORK_RELAY

#define LEN_NET_RELAY_24G_ATTR  (8)
#define PATTERN_REALY_24G_ATTR  (0x55)

uint8_t hsm_network_rec_24g_realyed_setself_attr(uint8_t *rec_data,uint8_t len)
{
    uint32_t value;
    if((len != LEN_NET_RELAY_24G_ATTR) || (rec_data[0] != PATTERN_REALY_24G_ATTR))
    {
        __LOG(LOG_SRC_APP, LOG_LEVEL_INFO,"Invaild 2.4G packet\n");
        return false;
    }

    value = rec_data[7];
    value = (value << 8) + rec_data[6];
    value = (value << 8) + rec_data[5];
    value = (value << 8) + rec_data[4];
    cxw8_handle_board(rec_data[3],value);
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO,"Network layer relay 2.4G packet\n");
    return true;
}

static void hsm_network_send_24g_force(uint8_t cxw_type,uint32_t value)
{
    // Returned status
    uint16_t status = MESH_ERR_NO_ERROR;
    mesh_tb_buf_t *p_buf;
    uint32_t seq,iv_index;
    uint8_t send_data[LEN_NET_RELAY_24G_ATTR];
    uint8_t env_data[32];
    // Check provision state
    if(m_tb_state_get_prov_state() != M_TB_STATE_PROV_STATE_PROV)
    {
        __LOG(LOG_SRC_APP, LOG_LEVEL_INFO,"Network can not relay 24G packet because unproved state\n");
        return;
    }
    // Init send data
    // You can use send_data[0,1,2] to make group future
    send_data[0] = PATTERN_REALY_24G_ATTR;
    send_data[1] = 0xaa;
    send_data[2] = 0xcc;
    send_data[3] = cxw_type;
    send_data[4] = (uint8_t)(value);
    send_data[5] = (uint8_t)(value >> 8);
    send_data[6] = (uint8_t)(value >> 16);
    send_data[7] = (uint8_t)(value >> 24);

    status = mesh_tb_buf_alloc(&p_buf, 9, sizeof(send_data), 0);

    if(status != MESH_ERR_NO_ERROR)
    {
        return;
    }
    // Init env data
    memset(&env_data[0],0,sizeof(env_data));
    // env_data[0] = 0; // flags
    // Net key lid
    env_data[1] = M_TB_KEY_GET_NET_KEY_LID(0);
    // APP key lid
    env_data[2] = M_TB_KEY_MAX_NB_NET + M_TB_KEY_MAX_NB_DEV;
    // env_data[3] = 0; // model_lid
    // Seq
    m_tb_key_get_iv_seq(&seq,&iv_index);
    env_data[4] = (uint8_t)(seq);
    env_data[5] = (uint8_t)(seq >> 8);
    env_data[6] = (uint8_t)(seq >> 16);
    env_data[7] = (uint8_t)(seq >> 24);

    env_data[8]  = (uint8_t)(iv_index);
    env_data[9]  = (uint8_t)(iv_index >> 8);
    env_data[10] = (uint8_t)(iv_index >> 16);
    env_data[11] = (uint8_t)(iv_index >> 24);
    // Src
    uint16_t src = m_tb_mio_get_prim_addr();
    env_data[12] = (uint8_t)(src);
    env_data[13] = (uint8_t)(src >> 8);
    // Dst (0xffee)
    env_data[14] = (uint8_t)(ADDR_NETWORK_SEND_24G);
    env_data[15] = (uint8_t)(ADDR_NETWORK_SEND_24G >> 8);
    // TTL (0x64)
    env_data[16] = 0x64;

    // Copy provided environment
    memset(&p_buf->env[0], 0, MESH_TB_BUF_ENV_SIZE);
    memcpy(&p_buf->env[0], &env_data[0], sizeof(env_data));
    // Copy provided data
    memcpy(MESH_TB_BUF_DATA(p_buf), &send_data[0], sizeof(send_data));

    // Push the buffer
    extern const m_lay_net_api_t m_lay_net_api;
    status = m_lay_net_api.send(p_buf, 0);
    if(status != MESH_ERR_NO_ERROR)
    {
        __LOG(LOG_SRC_APP, LOG_LEVEL_INFO,"Can not relay 24G packet because %x\n",status);
    }
    mesh_tb_buf_release(p_buf);
}
#else
static void hsm_network_send_24g_force(uint8_t cxw_type,uint32_t value){}
uint8_t hsm_network_rec_24g_realyed_setself_attr(uint8_t *rec_data,uint8_t len){return false;}
#endif /* CONFIG_APP_SNIFFER_NETWORK_RELAY */

static bool flag_sniffer_can_save = true;
static co_timer_t sniffer_save_timer;

static void sniffer_save_timer_handler(co_timer_t *timer, void *param)
{
    flag_sniffer_can_save = true;
    co_timer_del(&sniffer_save_timer);
}
static void board_set_attr_limit(uint32_t attr_value, uint16_t *value)
{
    board_set_attr(attr_value, value);
    if(flag_sniffer_can_save)
    {
        flag_sniffer_can_save = false;
        NVDS_IND_HW_CTX_SAVE();
        co_timer_set(&sniffer_save_timer,1000,TIMER_ONE_SHOT,sniffer_save_timer_handler,NULL);
    }
}

static void cxw8_handle_board(uint8_t cmd,uint32_t attr_value)
{
    uint16_t tmp;

    switch (cmd)
    {
        case CXW_CMD_open:
            tmp = 1;
            board_set_attr_limit(ATTR_TYPE_ONOFF, &tmp);
            break;
        case CXW_CMD_close:
            tmp = 0;
            board_set_attr_limit(ATTR_TYPE_ONOFF, &tmp);
            break;
        case CXW_CMD_setBrightness:
            tmp = (uint16_t)(attr_value * 65535/1000);
            board_set_attr_limit(ATTR_TYPE_LIGHTNESS, &tmp);
            break;
        case CXW_CMD_setColorTemperature:
            tmp = (uint16_t)(20000 - attr_value * 19200/1000);
            board_set_attr_limit(ATTR_TYPE_CCT, &tmp);
            break;
        default:
            break;
    }
}

static void cxw8_handle_frame(cxw_frame_t *p_frame)
{
    uint16_t attr_value;

    if(m_tb_state_get_prov_state() != M_TB_STATE_PROV_STATE_PROV)return;

    if ((0xFF != p_frame->groupId))return;

    attr_value = ((uint32_t)p_frame->data[1] << 8) + p_frame->data[2];
    // Handle frame
    cxw8_handle_board(p_frame->cmd,attr_value);
    // Rec form  APP,push to network layer buf
    hsm_network_send_24g_force(p_frame->cmd,attr_value);
}

static void s_handle_telink_button(uint8_t pdu_type, uint8_t *p_adv_data,uint8_t len)
{
}

static void s_handle_sc_app(uint8_t pdu_type, uint8_t *p_adv_data,uint8_t len)
{
}

static void s_handle_hs6206_button(uint8_t pdu_type, uint8_t *p_adv_data,uint8_t len)
{
}

static void s_handle_hs_app(uint8_t pdu_type, uint8_t *p_adv_data,uint8_t len)
{
}

static void s_handle_zj_app(uint8_t pdu_type, uint8_t *p_adv_data,uint8_t len)
{
    uint8_t decode_dat[31];

    memcpy(&decode_dat[0],p_adv_data,len);

    hsm_bearer_send_24g_force(decode_dat,len);

    lelt_decrypt(&decode_dat[LE_LIGHT_ADVDATA_ZJ_PLAIN_LEN], p_adv_data[3] - 1, LE_LIGHT_ADVDATA_ZJ_PLAIN_LEN);
    //__LOG_XB(LOG_SRC_APP, LOG_LEVEL_INFO, "ZJ 2.4G albino",&decode_dat[0],len);

    cxw8_handle_frame((cxw_frame_t *)(&decode_dat[LE_LIGHT_ADVDATA_ZJ_PLAIN_LEN+LE_LIGHT_ADVDATA_TE24G_HDR_LEN]));

}

static void s_handle_om_app(uint8_t pdu_type, uint8_t *p_adv_data,uint8_t len)
{
    uint8_t decode_dat[31];

    memcpy(&decode_dat[0],p_adv_data,len);

    hsm_bearer_send_24g_force(decode_dat,len);

    lelt_decrypt(&decode_dat[LE_LIGHT_ADVDATA_OM4_PLAIN_LEN], p_adv_data[3] - 1, LE_LIGHT_ADVDATA_OM4_PLAIN_LEN);
    //__LOG_XB(LOG_SRC_APP, LOG_LEVEL_INFO, "OM 2.4G albino",&decode_dat[0],len);

    cxw8_handle_frame((cxw_frame_t *)(&decode_dat[LE_LIGHT_ADVDATA_OM4_PLAIN_LEN+LE_LIGHT_ADVDATA_OM24G_HDR_LEN]));
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
uint8_t board_api_sniffer_ctl(uint8_t pdu_type, uint8_t *p_adv_data,uint8_t len)
{
    /* Sanity check */
    if(len < 4)return false;
    if(len > 31)return false;
    if((pdu_type != M_ADV_IND) && (pdu_type != M_ADV_NONCONN_IND))return false;

    /* Omit a special UUID */
    if(0 == memcmp(&m_ex_uuid_pattern[3], &p_adv_data[3], len-3)) return false;

    /* Omit the redundant adv packet */
    if(0 == memcmp(m_latest_adv_data,p_adv_data,len))return false;

    /* Handle adv data */
    if(0 == memcmp(p_adv_data, m_telink_button, sizeof(m_telink_button)))
    {
        s_handle_telink_button(pdu_type, p_adv_data, len);
    }
    else if (0 == memcmp(&p_adv_data[3], &m_sc_app_pattern[3], sizeof(m_sc_app_pattern)-3))
    {
        s_handle_sc_app(pdu_type, p_adv_data, len);
    }
    else if (0 == memcmp(&p_adv_data[1], &m_hs6206_button[1], sizeof(m_hs6206_button)-1))
    {
        s_handle_hs6206_button(pdu_type, p_adv_data, len);
    }
    else if (0 == memcmp(&p_adv_data[3], &m_zj_app_pattern[3], sizeof(m_zj_app_pattern)-3))
    {
        s_handle_zj_app(pdu_type, p_adv_data, len);
    }
    else if (0 == memcmp(&p_adv_data[3], &m_om_app_pattern[3], sizeof(m_om_app_pattern)-3))
    {
        s_handle_om_app(pdu_type, p_adv_data, len);
    }
    else if (0 == memcmp(&p_adv_data[3], &m_hs_app_pattern[3], sizeof(m_hs_app_pattern)-3))
    {
        s_handle_hs_app(pdu_type, p_adv_data, len);
    }
    else
    {
        return false;
    }

    memcpy(m_latest_adv_data, p_adv_data, len);
    memset(m_latest_adv_data + len, 0, sizeof(m_latest_adv_data) - len);
    return true;
}
#else
uint8_t board_api_sniffer_ctl(uint8_t pdu_type, uint8_t *p_adv_data,uint8_t len)
{
    return false;
}
#endif /* #if CONFIG_APP_SNIFFER */
/** @} */
