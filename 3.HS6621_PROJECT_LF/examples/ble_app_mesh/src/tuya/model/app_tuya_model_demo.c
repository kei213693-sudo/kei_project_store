/**
 ****************************************************************************************
 *
 * @file app_tuya_model_demo.c
 *
 * @brief BLE mesh app tuya model register demo.
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
#include "rwip_config.h" // SW configuration
#include "app_tuya_model.h"
#include "app_tuya_model_demo.h"

#if APP_MESH_TUYA

/*********************************************************************
 * MACROS
 */
#define MM_TUYA_DEMO_MODEL_ID   0x1000

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */
/// TUYA mesh model layer environment structure
typedef struct mm_tuya_demo_env
{
    /// List of buffers containing meaasge to process
    co_list_t process_queue;

    /// Delayed job structure
    mal_djob_t djob;

    /// Local Model ID
    uint8_t model_lid;
}mm_tuya_demo_env_t;

/*********************************************************************
 * LOCAL VARIABLES
 */
/// Mesh model layer environment
static mm_tuya_demo_env_t *p_tuya_demo_env;

/*********************************************************************
 * LOCAL DECLARATION
 */
/// Prepare process next message
static void mm_tuya_demo_process_next(void);

/// Prepare ack data after handle receive data
static void mm_tuya_demo_send_ack(uint32_t opc, bool vendor,uint8_t *p_data, uint16_t data_len);

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
///TODO Process receive data handler
static void mm_tuya_demo_process_receice_handler(void *params)
{
    // Get buffer to handle
    mesh_tb_buf_t *p_buf = (mesh_tb_buf_t *)co_list_pick(&p_tuya_demo_env->process_queue);
    // Get buffer environment
    m_lay_buf_env_t *p_env = (m_lay_buf_env_t *)(&p_buf->env[0]);

    // Get Model Receive Data.
    uint8_t *p_data_in = MESH_TB_BUF_DATA(p_buf);
    // Data you want send.
    uint8_t *p_data_out = NULL;
    // Data you want send length.
    uint16_t p_data_out_len = 0;

    __LOG(LOG_SRC_MODEL, LOG_LEVEL_INFO, "[TUYA] SRC %x,DST %x,NET_LID %x,APP_LID %x,SEQ %x,TTL %x\n",
    p_env->src,p_env->dst,p_env->net_lid,p_env->app_lid,p_env->seq,p_env->ttl_ctl);

    __LOG(LOG_SRC_MODEL, LOG_LEVEL_INFO,"[TUYA] Rec Data Len %d,Data:%02x%02x\n",p_buf->data_len,*p_data_in,*(p_data_in+1));

    // TODO You can use follow variables handle data
    /*
     *
     * [uint16_t] p_buf->data_len : The length of the valid data received.
     * [uint8_t*] p_data_in : The valid data address received.
     * [uint32_t] p_env->u5.opcode : The OPCODE of the received data.
     * [uint16_t] p_env->src : Source address of the message.
     * [uint16_t] p_env->dst : Destination address of the message.
     * [uint8_t ] p_env->net_lid : Network key local index.
     * [uint8_t ] p_env->app_lid : Application key local index.
     * [uint32_t] p_env->seq : Sequence number.
     * [uint8_t ] p_env->ttl_ctl : bit[7] CTL: 1 = Control message | 0 = Access Message,bit[6:0] TTL: Time to leave value; range [0-127].
     *
    */
    // Delete Compiler Warnning

    // TODO You can send ack after handle receive data
    uint32_t opcode     = 0x0482;
    uint8_t out_dat[4]  = {0x12,0x34,0x56,0x78};
    p_data_out_len      = sizeof(out_dat);
    p_data_out          = out_dat;

    // Send Ack to client
    mm_tuya_demo_send_ack(opcode,0,p_data_out,p_data_out_len);

    // Whatever the result,you must skip to next message
    mm_tuya_demo_process_next();
}

/// Prepare ack data after handle receive data
static void mm_tuya_demo_send_ack(uint32_t opc, bool vendor, uint8_t *p_data_out, uint16_t data_out_len)
{
    // Pointer to the buffer that will contain the message
    mesh_tb_buf_t *p_buf;

    if (MESH_ERR_NO_ERROR == (mesh_tb_buf_alloc(&p_buf, vendor ? TUYA_MM_VENDOR_HEAD_LEN : TUYA_MM_SIG_HEAD_LEN, data_out_len, false))) {
        // Get pointer to data
        uint8_t *p_data = MESH_TB_BUF_DATA(p_buf);
        memcpy(p_data, p_data_out, data_out_len);
    }
    else return;

    // Send the ack message
    // Retrieve buffer containing the message for which a response is sent
    mesh_tb_buf_t *p_buf_req = (mesh_tb_buf_t *)co_list_pick(&p_tuya_demo_env->process_queue);
    // Get buffer environment
    m_lay_buf_env_t *p_env = (m_lay_buf_env_t *)&p_buf_req->env[0];
    // Use request source address as destination address
    uint16_t dst = p_env->src;

    // Send the ack message
    m_api_model_rsp_send(p_tuya_demo_env->model_lid, opc, 0, p_buf, p_env->app_lid, dst, false, false);
}

/// Prepare process next message
static void mm_tuya_demo_process_next(void)
{
    // Pop first buffer in the process queue
    mesh_tb_buf_t *p_buf = (mesh_tb_buf_t *)co_list_pop_front(&p_tuya_demo_env->process_queue);

    // Release the buffer
    mesh_tb_buf_release(p_buf);

    // Process next buffer
    p_buf = (mesh_tb_buf_t *)co_list_pick(&p_tuya_demo_env->process_queue);

    if(p_buf != NULL)
    {
        // Delay handling of received message
        mal_djob_reg(&(p_tuya_demo_env->djob));
    }
}

/// Model receive data callback hanlder
static void mm_tuya_demo_cb_rx(m_lid_t mdl_lid, uint32_t opcode, m_api_buf_t *p_api_buf, m_lid_t app_key_lid,
                                 uint16_t src, int8_t rssi, bool not_relayed)
{
    // Data length
    uint16_t data_len = ((mesh_tb_buf_t *)p_api_buf)->data_len;
    // Allocate a buffer
    mesh_tb_buf_t *p_buf;

    // Allocate p_buf for process use
    if(MESH_ERR_NO_ERROR == mesh_tb_buf_alloc(&p_buf,0,data_len,0)) {

        // Copy content of the received buffer in the allocated one
        mesh_tb_buf_copy((mesh_tb_buf_t *)p_api_buf,p_buf,data_len,true);

        // Insert the buffer in the process queue
        co_list_push_back(&p_tuya_demo_env->process_queue, &p_buf->hdr);

        // Check if buffer can be processed now or if another buffer is currently processed
        if((mesh_tb_buf_t *)co_list_pick(&p_tuya_demo_env->process_queue) == p_buf) {
            // Delay handling of received message
            mal_djob_reg(&(p_tuya_demo_env->djob));
        }
    }
}

/// Model have sent data callback handler
static void mm_tuya_demo_cb_sent(m_lid_t mdl_lid, uint8_t tx_hdl, m_api_buf_t *p_api_buf, uint16_t status)
{
    // Release the buffer
    mesh_tb_buf_release((mesh_tb_buf_t *)p_api_buf);
}

/// Model receive satify callback handler
static void mm_tuya_demo_cb_opcode_check(m_lid_t mdl_lid, uint32_t opcode)
{
    uint16_t status = MESH_ERR_NOT_SUPPORTED;

    // TODO Check that opcode is a sig 2-byte opcode
    // MESH_IS_3_OCT_OPCODE(opcode)

    // Check that opcode is a vendor opcode in 3-byte
    if (MESH_IS_2_OCT_OPCODE(opcode)) {
        //TODO Check satify code.
        status = MESH_ERR_NO_ERROR;
    }
    m_api_model_opcode_status(mdl_lid,opcode,status);
}

/// Inform the appilication about the new pulication period
static void mm_tuya_demo_cb_publish_period(m_lid_t mdl_lid, uint32_t period_ms)
{
    // Inform the appilication about the new pulication period
    m_api_send_fault_period_ind(period_ms, 0);
}
/*********************************************************************
 * CONST VARIABLES
 */
/// TUYA Demo Model callback functions
const m_api_model_cb_t mm_tuya_demo_cb = {
    .cb_rx               = mm_tuya_demo_cb_rx,
    .cb_sent             = mm_tuya_demo_cb_sent,
    .cb_opcode_check     = mm_tuya_demo_cb_opcode_check,
    .cb_publish_period   = mm_tuya_demo_cb_publish_period,
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

uint16_t mm_tuya_demo_init(bool reset, void *p_env, const mm_cfg_t *p_cfg)
{
    // TODO:Modify base on actual model id
    bool is_vendor = 0;
    // TODO:Modify base on actual element id
    uint8_t elemt_id = 0;

    if(!reset) {
        // Get environment for mesh model
        p_tuya_demo_env = (mm_tuya_demo_env_t *)p_env;

        // Register the model
        m_api_register_model(MM_TUYA_DEMO_MODEL_ID,elemt_id,is_vendor,&mm_tuya_demo_cb,
                             &p_tuya_demo_env->model_lid);

        // Initialize delayed job
        p_tuya_demo_env->djob.cb = mm_tuya_demo_process_receice_handler;
    }
    else {
        p_tuya_demo_env = NULL;
    }

    // Return environment size
    return (sizeof(mm_tuya_demo_env_t));
}

uint16_t mm_tuya_demo_get_env_size(const mm_cfg_t *p_cfg)
{
    // Return environment size
    return (sizeof(mm_tuya_demo_env_t));
}

#endif /* APP_MESH_TUYA */