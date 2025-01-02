/**
 ****************************************************************************************
 *
 * @file appm_tmall.c
 *
 * @brief Tmall-genie extension based on ble mesh.
 *
 * Copyright (C) OnMicro 2019
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"          // SW configuration

#if (BLE_APP_PRESENT)

#if (APP_MESH_TMALL)

#include "mesh_defines.h"
#include "m_defines.h"
#include "m_tb.h"
#include "tinycrypt/sha256.h"
#include "tinycrypt/constants.h"
#include "app.h"
#include "tmall_led_demo.h"

/*
 * LOCAL VARIABLES DEFINITION
 ****************************************************************************************
 */

static const char hex2char[]={'0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

/*********************************************************************
 * MACROS
 */
#define TMALL_SELF_SUBS_ADR0_LIGHT     (0xC000)
#define TMALL_SELF_SUBS_ADR1_LIGHT     (0xCFFF)

/*
 * GLOBAL VARIABLES DEFINITION
 ****************************************************************************************
 */
// Default Tripple
tmall_tripple_t mesh_tmall_tripple = {
	.product_id  = 3788,
	.mac_addr    = {0x20,0x70,0x0c,0x6c,0x14,0x18},
	.secret      = {0x15,0xf3,0x5f,0x96,0x50,0x9d,0xb1,0x82,0x75,0xe3,0xbd,0xe0,0x67,0xc9,0x00,0xa2}, // #1};
};

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static void hextostring(const uint8_t *source, char *dest, int len, bool inverse)
{
    int i;

    if (inverse) {
        for (i = 0; i < len; i++) {
            dest[(len-i)*2-2] = hex2char[source[i]>>4];
            dest[(len-i)*2-1] = hex2char[source[i]&0xf];
        }
    }
    else {
        for (i = 0; i < len; i++) {
            dest[i*2+0] = hex2char[source[i]>>4];
            dest[i*2+1] = hex2char[source[i]&0xf];
        }
    }
    return;
}

#define PID_STRING_LEN 4
#define DEVICE_ADDR_LEN 6
#define STATIC_VALUE_LEN 100
/**
 * @brief Generate static OOB from tmall-genie tripple data, which is used as auth data for privisioning.
 * @note This code is cloned from tmall_model.c in AliOS-Things.
 */
static int bt_mesh_profile_calculate_digest(uint8_t *digest, const uint8_t *pid,
                                            const uint8_t *mac_addr, const uint8_t *secret)
{
    int ret = TC_CRYPTO_FAIL;
    char pid_string[9] = "";
    char mac_addr_string[13] = "";
    char secret_string[33] = "";
    char static_value[STATIC_VALUE_LEN] = { 0x00 }; // pid + ',' + mac + ',' + secret
    struct tc_sha256_state_struct sha256_ctx;

    // convert the hex byte stream to hex string
    hextostring(pid, pid_string, PID_STRING_LEN, true);
    //printf("pid string: %s\n", pid_string);
    hextostring(mac_addr, mac_addr_string, DEVICE_ADDR_LEN, true);
    //printf("mac_addr_string: %s\n", mac_addr_string);
    hextostring(secret, secret_string, 16, false);
    //printf("secret_string: %s\n", secret_string);

    strcat(static_value, pid_string);
    strcat(static_value, ",");
    strcat(static_value, mac_addr_string);
    strcat(static_value, ",");

    // generally we should ensure the secret lenght won't exceed the STATIC_VALUE_LEN
    if (strlen(secret_string) > (STATIC_VALUE_LEN - PID_STRING_LEN - DEVICE_ADDR_LEN - 2)) {
        //printf("secret length over size\n");
        return ret;
    } else {
        strcat(static_value, secret_string);
    }

    //printf("static oob: %s\n", static_value);

    /* calculate the sha256 of oob info and
     * fetch the top 16 bytes as static value
     */
    ret = tc_sha256_init(&sha256_ctx);
    if (ret != TC_CRYPTO_SUCCESS) {
        //printf("sha256 init fail\n");
        return ret;
    }

    ret = tc_sha256_update(&sha256_ctx, (uint8_t *)static_value, strlen(static_value));
    if (ret != TC_CRYPTO_SUCCESS) {
        //printf("sha256 udpate fail\n");
        return ret;
    }

    ret = tc_sha256_final(digest, &sha256_ctx);
    if (ret != TC_CRYPTO_SUCCESS) {
        //printf("sha256 final fail\n");
        return ret;
    }
    return ret;
}


static tmall_device_error_code_t app_tmall_attr_get(tmall_device_attr_type_t attr_type, uint8_t *p_attr_value, uint8_t *p_len_value)
{
    tmall_device_error_code_t err_code = TMALL_ERROR_CODE_SUCCESS;

    switch (attr_type) {
    case ATTR_TYPE_ONOFF:
        board_get_attr(ATTR_TYPE_ONOFF, p_attr_value);
        *p_len_value = 1;
        break;
    case ATTR_TYPE_MAIN_LAMP_ONOFF:
        board_get_attr(ATTR_TYPE_MAIN_LAMP_ONOFF, p_attr_value);
        *p_len_value = 1;
        break;
    case ATTR_TYPE_BG_LAMP_ONOFF:
        board_get_attr(ATTR_TYPE_BG_LAMP_ONOFF, p_attr_value);
        *p_len_value = 1;
        break;
    case ATTR_TYPE_LIGHTNESS:
        board_get_attr(ATTR_TYPE_LIGHTNESS, p_attr_value);
        *p_len_value = 2;
        break;
    case ATTR_TYPE_CCT:
        board_get_attr(ATTR_TYPE_CCT, p_attr_value);
        *p_len_value = 2;
        break;
    case ATTR_TYPE_DUV:
        board_get_attr(ATTR_TYPE_DUV, p_attr_value);
        *p_len_value = 2;
        break;
    case ATTR_TYPE_HSL:
        board_get_attr(ATTR_TYPE_HSL, p_attr_value);
        *p_len_value = 6;
        break;
    case ATTR_TYPE_SCENE_MODE:
        board_get_attr(ATTR_TYPE_SCENE_MODE, p_attr_value);
        *p_len_value = 2;
        break;
    case ATTR_TYPE_LEVEL:
        board_get_attr(ATTR_TYPE_LEVEL, p_attr_value);
        *p_len_value = 1;
        break;
    case ATTR_TYPE_FAN_DIRECTION:
        board_get_attr(ATTR_TYPE_FAN_DIRECTION, p_attr_value);
        *p_len_value = 1;
        break;
    default:
        err_code = TMALL_ERROR_CODE_ATTR_TYPE_NOT_SUPPORT;
        break;
    }
    return err_code;
}

#define TMALL_IND_TID_ONOFF     (0x33)
#define TMALL_IND_TID_LNS       (0x34)
#define TMALL_IND_TID_CCT       (0x35)
#define TMALL_IND_TID_HSL       (0x36)
#define TMALL_IND_TID_MLAMP     (0x37)
#define TMALL_IND_TID_BLAMP     (0x38)
#define TMALL_IND_TID_SCENE     (0x39)

static tmall_device_error_code_t app_tmall_attr_set(tmall_device_attr_type_t attr_type, uint8_t *p_attr_value, uint8_t *p_len_value)
{
    tmall_device_error_code_t err_code = TMALL_ERROR_CODE_SUCCESS;

    switch (attr_type) {
    case ATTR_TYPE_ONOFF:
        board_set_attr(ATTR_TYPE_ONOFF, p_attr_value);
        board_ind_attr(ATTR_TYPE_ONOFF,TMALL_IND_TID_ONOFF,p_attr_value,1);
        *p_len_value = 1;
        break;
    case ATTR_TYPE_MAIN_LAMP_ONOFF:
        board_set_attr(ATTR_TYPE_MAIN_LAMP_ONOFF, p_attr_value);
        board_ind_attr(ATTR_TYPE_MAIN_LAMP_ONOFF,TMALL_IND_TID_MLAMP,p_attr_value,1);
        *p_len_value = 1;
        break;
    case ATTR_TYPE_BG_LAMP_ONOFF:
        board_set_attr(ATTR_TYPE_BG_LAMP_ONOFF, p_attr_value);
        board_ind_attr(ATTR_TYPE_BG_LAMP_ONOFF,TMALL_IND_TID_BLAMP,p_attr_value,1);
        *p_len_value = 1;
        break;
    case ATTR_TYPE_LIGHTNESS:
        board_set_attr(ATTR_TYPE_LIGHTNESS, p_attr_value);
        board_ind_attr(ATTR_TYPE_LIGHTNESS,TMALL_IND_TID_LNS,p_attr_value,2);
        *p_len_value = 2;
        break;
    case ATTR_TYPE_CCT:
        board_set_attr(ATTR_TYPE_CCT, p_attr_value);
        board_ind_attr(ATTR_TYPE_CCT,TMALL_IND_TID_CCT,(uint8_t *)p_attr_value,2);
        *p_len_value = 2;
        break;
    case ATTR_TYPE_DUV:
        board_set_attr(ATTR_TYPE_DUV, p_attr_value);
        *p_len_value = 2;
        break;
    case ATTR_TYPE_HSL:
        board_set_attr(ATTR_TYPE_HSL, p_attr_value);
        board_ind_attr(ATTR_TYPE_HSL,TMALL_IND_TID_HSL,(uint8_t *)p_attr_value,6);
        *p_len_value = 6;
        break;
    case ATTR_TYPE_SCENE_MODE:
        board_set_attr(ATTR_TYPE_SCENE_MODE, p_attr_value);
        board_ind_attr(ATTR_TYPE_SCENE_MODE,TMALL_IND_TID_SCENE,(uint8_t *)p_attr_value,2);
        *p_len_value = 2;
        break;
    case ATTR_TYPE_LEVEL:
        board_set_attr(ATTR_TYPE_LEVEL, p_attr_value);
        *p_len_value = 1;
        break;
    case ATTR_TYPE_FAN_DIRECTION:
        board_set_attr(ATTR_TYPE_LEVEL, p_attr_value);
        *p_len_value = 1;
        break;

    case ATTR_TYPE_UNIX_TIME:
        tmall_set_unix_time(p_attr_value,p_len_value);
        break;
    case ATTR_TYPE_TIMED_ONESHOT:
        tmall_set_timer(p_attr_value,p_len_value, 1);
        break;
    case ATTR_TYPE_TIMED_PERIODIC:
        tmall_set_timer(p_attr_value,p_len_value, 2);
        break;
    case ATTR_TYPE_DEL_TIMER:
        tmall_del_timer(p_attr_value,p_len_value);
        break;

    default:
        err_code = TMALL_ERROR_CODE_ATTR_TYPE_NOT_SUPPORT;
        break;
    }
    NVDS_IND_HW_CTX_SAVE();
    return err_code;
}

static tmall_device_error_code_t app_tmall_attr_cfm(uint8_t *p_attr_value)
{
    tmall_device_error_code_t err_code = TMALL_ERROR_CODE_SUCCESS;
#if CONFIG_TMALL_IND
    board_cfm_attr(p_attr_value);
#endif
    return err_code;
}
/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 * @brief Generate device UUID, which format is defined by Tmall.
 */
void appm_tmall_init(void)
{
    uint8_t digest256[32];
    uint8_t mesh_dev_uuid[MESH_DEV_UUID_LEN] = {0xA8, 0x01, 0x51, 0xa8, 0x04, 0x00, 0x00, 0x41, 0xc0, 0xf8, 0x07, 0xda, 0x78, 0x00, 0x00, 0x00};
    app_mesh_auth_data_t mesh_prov_auth_data;

    /* Load the tripple from NVDS. */
#if (NVDS_SUPPORT)
    nvds_tag_len_t length = NVDS_LEN_MESH_TMALL_MAC;
    if (nvds_get(PARAM_ID_BD_ADDRESS, &length, &mesh_tmall_tripple.mac_addr[0]) != PARAM_OK)
    {
        /* Store the default tripple to NVDS. */
        nvds_put(NVDS_TAG_MESH_TMALL_PID, NVDS_LEN_MESH_TMALL_PID, (uint8_t *)&mesh_tmall_tripple.product_id);
        nvds_put(NVDS_TAG_MESH_TMALL_SEC, NVDS_LEN_MESH_TMALL_SEC, &mesh_tmall_tripple.secret[0]);
        nvds_put(NVDS_TAG_MESH_TMALL_MAC, NVDS_LEN_MESH_TMALL_MAC, &mesh_tmall_tripple.mac_addr[0]);
    }
    else
    {
        length = NVDS_LEN_MESH_TMALL_PID;
        nvds_get(NVDS_TAG_MESH_TMALL_PID, &length, (uint8_t *)&mesh_tmall_tripple.product_id);
        length = NVDS_LEN_MESH_TMALL_SEC;
        nvds_get(NVDS_TAG_MESH_TMALL_SEC, &length, &mesh_tmall_tripple.secret[0]);
    }

#endif

    bt_mesh_profile_calculate_digest(&digest256[0],
                                     (const uint8_t *)&mesh_tmall_tripple.product_id,
                                     &mesh_tmall_tripple.mac_addr[0],
                                     &mesh_tmall_tripple.secret[0]);
    /* RW stack uses little endian. */
    co_bswap((uint8_t *)&mesh_prov_auth_data, &digest256[0], M_OOB_AUTH_DATA_LEN);
    memcpy(&mesh_dev_uuid[7], &mesh_tmall_tripple.mac_addr[0], 6);
    memcpy(&mesh_dev_uuid[3], (const uint8_t *)&mesh_tmall_tripple.product_id, 4);
    // Don't touch device UUID for PTS setup.
    if (mesh_tmall_tripple.product_id != 1192) {
        /* FeatureFlag: UUID version 1 */
        mesh_dev_uuid[13] = 0x02;
    }
    app_mesh_prov_auth_data_set(mesh_prov_auth_data);
    app_mesh_node_id_set_uuid(mesh_dev_uuid);

    __LOG_XB(LOG_SRC_API, LOG_LEVEL_INFO, "Device UUID", mesh_dev_uuid, MESH_DEV_UUID_LEN);
}

void appm_tmall_self_bind(m_lid_t app_key_lid)
{
    m_lid_t model_lid;
    // Register Tmall attribute handler.
    mm_vnd_tmall_register_attr_handler(app_tmall_attr_get, app_tmall_attr_set, app_tmall_attr_cfm);

    // Don't do self-binding for PTS setup.
    if (mesh_tmall_tripple.product_id == 1192) {
        return;
    }

    // Self-bind all models except for Configuration Server.
    for (model_lid = 1; model_lid < m_tb_mio_get_nb_model(); model_lid++)
    {
        // Bind the application key with the model
        uint16_t status = m_tb_key_model_bind(app_key_lid, model_lid);
        if (status == MESH_ERR_NO_ERROR)
        {
            // Inform the Model IO manager
            // FIXME: Do not save Because bind when power on
            //m_tb_mio_bind(model_lid);
        }
        // Add the subscription address to the model
        m_tb_mio_add_subscription(model_lid,TMALL_SELF_SUBS_ADR0_LIGHT);
        m_tb_mio_add_subscription(model_lid,TMALL_SELF_SUBS_ADR1_LIGHT);
    }
}

#endif /* APP_MESH_TMALL */

#endif //(BLE_APP_PRESENT)

/// @} APP
