/*********************************************************************
 * INCLUDES
 */
#include "rwip_config.h" // RW SW configuration
#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include "rwip.h"      // RW SW initialization
#include "app.h"
#include "peripheral.h"


//APP_HANDLER(gatt_mtu_changed, msgid, param, dest_id, src_id);

/*********************************************************************
 * EXTERN FUNCTIONS
 */
void appm_update_param(uint8_t conidx, struct gapc_conn_param *conn_param);
void app_sec_send_security_req(uint8_t conidx);
void app_tspp_server_send_data(uint8_t conidx, uint8_t* pdata, uint16_t len);
void app_ancsc_start(void); // Call this when link encrypted
void app_ancsc_proform(uint32_t notif_uid, uint8_t act_id); /* ANCS_ACTION_ID_POSITIVE / NEGATIVE */

extern void i2s_rx_dma_handler(dma_status_t status, uint32_t cur_src_addr, uint32_t cur_dst_addr, uint32_t xfer_size);

/*****************************************************************************************/
static co_timer_t app_update_con_timer;

//0: not use ; 1: sbc;2;msbc
#define APP_USE_SBC 2
#ifdef APP_TEST_AUDIO_PLAY
#undef APP_USE_SBC
#define APP_USE_SBC 0
#endif

#if (APP_USE_SBC == 1)
#include "sbc.h"
/// i2s buffer
#define I2S_BLOCK_SIZE        256//2048//2048//1024//256
#define I2S_DMA_BUFFER_SIZE   1024//8192//4096//1024
#define I2S_BLOCK_NUM         (sizeof(i2s_dma_buffer)/I2S_BLOCK_SIZE)
#define PCM_FRAME_SIZE        256
#elif (APP_USE_SBC == 2)
#include "sbc.h"

#define I2S_BLOCK_SIZE        240
#define I2S_DMA_BUFFER_SIZE   (240*4)
#define I2S_BLOCK_NUM         (sizeof(i2s_dma_buffer)/I2S_BLOCK_SIZE)
#define PCM_FRAME_SIZE        240
#else
/// i2s buffer
#define I2S_BLOCK_SIZE        240//2048//2048//1024//256
#define I2S_DMA_BUFFER_SIZE   (240*4)//8192//4096//1024
#define I2S_BLOCK_NUM         (sizeof(i2s_dma_buffer)/I2S_BLOCK_SIZE)
#define PCM_FRAME_SIZE        240
#endif

#define AUDIO_DATA_LEN_PER_PACKET    60

/// i2s config
#pragma pack(4)
static int16_t   i2s_dma_buffer[I2S_DMA_BUFFER_SIZE];
#pragma pack()

static dma_fifo_t i2s_dma_rx_fifo;
static HS_DMA_CH_Type *i2s_dma_rx = NULL;
DMA_DECLARE_LLIP(i2s_dma_block_llip_rx, I2S_BLOCK_NUM);

const i2s_dma_config_t i2s_dma_rx_cfg = {
    .use_fifo       = true,
    .buffer         = i2s_dma_buffer,
    .buffer_len     = sizeof(i2s_dma_buffer),
    .block_llip     = i2s_dma_block_llip_rx,
    .block_num      = I2S_BLOCK_NUM,
    .callback       = i2s_rx_dma_handler,
};

#ifdef APP_TEST_AUDIO_PLAY
#include "codec_wm8753.h"
HS_DMA_CH_Type *i2s_dma_tx = NULL;
DMA_DECLARE_LLIP(i2s_dma_block_llip_tx, I2S_BLOCK_NUM);

const i2s_dma_config_t i2s_dma_tx_cfg = {
    .use_fifo       = true,
    .buffer         = i2s_dma_buffer,
    .buffer_len     = sizeof(i2s_dma_buffer),
    .block_llip     = i2s_dma_block_llip_tx,
    .block_num      = I2S_BLOCK_NUM,
    .callback       = NULL,
};
static uint8_t tx_start_flag = 0;

#endif    

const i2s_config_t i2s_cfg = {
    .sample_rate  = I2S_SAMPLE_16K,
    .sample_width = I2S_BITWIDTH_16BIT,
    .ws_width     = I2S_BITWIDTH_16BIT,
    .workmode     = I2S_WORKMODE_SLAVE,
    .pcmmode      = I2S_PCMMODE_MONO,
};

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
 void app_audio_rx_cmd_handler(        struct app_audio_rx_cmd_t const *param)
{
    uint8_t output[PCM_FRAME_SIZE] = {0};
    uint8_t pcm_frame[PCM_FRAME_SIZE+1] = {0};
    uint16_t len = 0;
    uint8_t pos = 0;

    while(dma_fifo_len(&i2s_dma_rx_fifo) >= PCM_FRAME_SIZE)
    {
        dma_fifo_out(&i2s_dma_rx_fifo, pcm_frame, PCM_FRAME_SIZE);
        
#if (APP_USE_SBC == 1)
        len = codec_encode(pcm_frame, PCM_FRAME_SIZE, output, PCM_FRAME_SIZE);
        app_tspp_server_send_data(0, (uint8_t*)output, len);
#elif (APP_USE_SBC == 2)
        output[0] =0x01;
        switch(pos%4)
        {
            case 0:
                output[1] =0x08;
                break;
            case 1:
                output[1] =0x38;
                break;
            case 2:
                output[1] =0xC8;
                break;
            case 3:
                output[1] =0xF8;
                break;
            default:
                break;
        }
        if(pos >= 3) pos = 0;
        else pos++;
        len = codec_encode(pcm_frame, PCM_FRAME_SIZE, &output[2], sizeof(output)-2);
        //co_assert(len > 0);
        app_tspp_server_send_data(0, (uint8_t*)output, AUDIO_DATA_LEN_PER_PACKET);
#else
        app_tspp_server_send_data(0, pcm_frame, PCM_FRAME_SIZE);
#endif

    }
}
 
void i2s_rx_dma_handler(dma_status_t status, uint32_t cur_src_addr, uint32_t cur_dst_addr, uint32_t xfer_size)
{
    //log_debug("rxdma(%d): %08X->%08X [%d]\n", status, cur_src_addr, cur_dst_addr, xfer_size);  
#ifdef APP_TEST_AUDIO_PLAY
    static uint8_t cnt = 0;
    
    if(0 == tx_start_flag)
    {
        cnt++;
        if(cnt >= 2)
        {
            tx_start_flag = 1;
            dma_start_with_lli(i2s_dma_tx);
        }
    }
    log_debug("*");
#else

    dma_fifo_set_in_pointer(&i2s_dma_rx_fifo, cur_dst_addr);

    struct app_audio_rx_cmd_t * cmd = KE_MSG_ALLOC(APP_AUDIO_RX_CMD,
            KE_BUILD_ID(TASK_APP, 0),
            KE_BUILD_ID(TASK_APP, 0),
            app_audio_rx_cmd_t);
    cmd->len = xfer_size*2;
    // Send the message
    ke_msg_send(cmd);

#endif
    //log_debug("*");
}


void app_audio_open_callback(void* param)
{
    (void)param;
    audio_config_t config;
    config.pga_gain = AUDIO_MIC_IN_PGA_GAIN_30DB;
    config.adc_gain = AUDIO_ADC_GAIN_0DB;
    #if 0 //single 
    config.sel_pga_input = AUDIO_SELECT_PAG_INPUT_GPIO3;
    config.sig_or_diff = AUDIO_EN_PGA_SINGLE_IN_INNER_REF;
    #else // different
    config.sel_pga_input = AUDIO_SELECT_PAG_INPUT_VIP_MIC_P;
    config.sig_or_diff = AUDIO_EN_PGA_SINGLE_IN_VIN_MIC_1;
    #endif
    audio_set_config(&config);
    
#ifdef APP_AUDIO_USE_DIG_MIC
    /// open dmic
    audio_dmic_config_t dmic_config;
#if defined(CONFIG_HS6621)
    dmic_config.clk = AUDIO_DMIC_CLOCK_SEL_2P4M;
#else
    dmic_config.clk = AUDIO_DMIC_CLOCK_SEL_2M;
#endif
    dmic_config.ctrl = AUDIO_DMIC_CLOCK_OUT;
    audio_dmic_open(&dmic_config);
#endif

    memset(&i2s_dma_buffer, 0, sizeof(i2s_dma_buffer));

    /// config i2s
#ifdef APP_TEST_AUDIO_PLAY
    i2s_config(&i2s_cfg);
#endif

    i2s_rx_config(&i2s_cfg);

    /// dma setting
    dma_fifo_init(&i2s_dma_rx_fifo, i2s_dma_buffer, sizeof(i2s_dma_buffer));
    dma_init();

    i2s_dma_rx = i2s_rx_dma_config((HS_DMA_CH_Type *)NULL, &i2s_dma_rx_cfg);
    
#ifdef APP_TEST_AUDIO_PLAY    
    i2s_dma_tx = i2s_dma_config((HS_DMA_CH_Type *)NULL, &i2s_dma_tx_cfg); 
    
    /// start wm8753   
    i2c_open(I2C_MODE_MASTER, 100000);
    codec_wm8753_init(CODEC_WM8753_MASTER, CODEC_WM8753_SAMPLE_RATE_16KHZ, CODEC_WM8753_MCLK_24MHZ);
#endif
    /// start dm rx
    dma_start_with_lli(i2s_dma_rx);

    /// start i2s
    i2s_rx_start(&i2s_cfg);

#ifdef APP_TEST_AUDIO_PLAY         
    i2s_start();
#endif
    log_debug("[AUDIO]: Audio start finish.\n");
}

void app_audio_open(void)
{
#ifdef CONFIG_HS6621
    //cpm_set_clock_div(CPM_ULP_RAM_CLK, 4); 
    pmu_pll_startup(96);
#endif

#ifdef CONFIG_HS6621C
    pmu_xtal32m_x2_startup();
#endif
	
#if (APP_USE_SBC == 1)
    codec_init(0, 16);
#elif (APP_USE_SBC == 2)
    codec_init(1, 26);
#endif

    /// start audio adc
    audio_open(app_audio_open_callback);
    

    log_debug("audio start\r\n");
}

void app_audio_close(void)
{
#ifdef APP_TEST_AUDIO_PLAY 
    i2c_close();
#endif
    /// close i2s
    i2s_rx_stop();
#ifdef APP_TEST_AUDIO_PLAY 
    i2s_stop();
#endif
    /// stop dma
    dma_stop(i2s_dma_rx);
    dma_release(i2s_dma_rx);
    i2s_dma_rx = NULL;
    
#ifdef APP_TEST_AUDIO_PLAY 
    dma_stop(i2s_dma_tx);
    dma_release(i2s_dma_tx);
    i2s_dma_tx = NULL;

    tx_start_flag = 0;
#endif
    
    audio_close();
}

static void app_gap_update_param(void)
{
#if 1
    struct gapc_conn_param conn_param;
#if 0 // interval 15ms iphone6s 12.3kBps; iphone8x 68kBps, mate9 13.2kBps
    conn_param.intv_min = 0x0c;
    conn_param.intv_max = 0x0c;
    conn_param.latency  = 0;
    conn_param.time_out = 100;
#else // android interval 7.5ms, huawei6x 4.9kBps; mate9 8.89kBps, 
    conn_param.intv_min = 0x18;
    conn_param.intv_max = 0x18;
    conn_param.latency  = 0;
    conn_param.time_out = 0x07D0;
#endif
    appm_update_param(0, &conn_param);
#endif
}

static void update_con_timer_handler(co_timer_t *timer, void *param)
{
    app_gap_update_param();
}

// gap handlers
void app_gap_connected_handler(struct gapc_connection_req_ind const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    log_debug_array_ex("ADDR", &param->peer_addr, 6);
    if(1){ // is android devices
        //app_sec_send_security_req(app_env.conidx);
    }
    co_timer_set(&app_update_con_timer, 1500, TIMER_ONE_SHOT, update_con_timer_handler, NULL);
}

void app_gap_disconnected_handler(struct gapc_disconnect_ind const *param)
{
    log_debug("%s@%d:0x%02x\n", __func__, __LINE__, param->reason);
}

void app_gap_param_update_ind_handler(struct gapc_param_updated_ind const *param)
{
    log_debug("%s@%d:%x/%d/%d\n", __func__, __LINE__, param->con_interval, param->con_latency, param->sup_to);
    if(param->con_interval < 0x18)
    {
        app_gap_update_param();
    }
}

void app_smp_encrypted_handler(void const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

void app_gatt_mtu_changed_handler(struct gattc_mtu_changed_ind const *param)
{
    log_debug("%s@%d:%d\n", __func__, __LINE__, param->mtu);
}

// tspp handlers
#if (BLE_TSPP_SERVER)
void app_tspp_empty_handler(void const *param)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
}

void app_tspp_full_handler(void const *param)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    log_debug("-F-\n");
}

void app_tspp_recv_handler(struct tspp_server_write_ind const *param)
{
    uint8_t conidx = 0;
    log_debug("[tspp] recv (%x)\n", param->length);
    if(param->value != NULL)// && param->length == 4)
    {
        if(param->value[0] == 0x01)
        {
            app_audio_open();
        }
        else if(param->value[0] == 0x00)
        {
            app_audio_close();

            // send end frame
            uint8_t data[4]={0x11, 0x22, 0x33, 0x44};
            app_tspp_server_send_data(conidx, data, 4);
        }
    }
}

void app_tspp_error_handler(struct tspp_server_error_evt const *param)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
}
#endif

#if BLE_APP_ANCSC
// ancs handlers
void app_ancs_recv_evt_handler(struct ancsc_notification_ind const *param)
{
    log_debug("ANCS Report event: id(%d),category(%d)\n", param->event_id, param->category_id);
}

// ancs handlers
void app_ancs_recv_msg_handler(struct ancsc_ntf_attr_ind const *param)
{
    log_debug("ANCS Report mesage: date(%s). %s \n", param->date, param->data);
}
#endif
