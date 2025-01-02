#include "rwip_config.h" // RW SW configuration

#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include "rwip.h"      // RW SW initialization
#include "dbg.h"
#include "peripheral.h"
#include "m_audio.h"
#include "app_hid.h"
#include "app_tspp_server.h"
#include "pmu.h"
#include "app_task.h"
#include "app_atvv.h"
#include "m_led.h"
#if CONFIG_AUDIO_ENABLE

#if (CONFIG_AUDIO_CODEC_SEL == 2)

#include "ima_enc.h"
#else
#include "sbc.h"
#endif

uint8_t is_audio_test_mode = 0;

extern void i2s_rx_dma_handler(dma_status_t status, uint32_t cur_src_addr, uint32_t cur_dst_addr, uint32_t xfer_size);

/// i2s buffer
#if (CONFIG_AUDIO_CODEC_SEL == 0)
#define I2S_BLOCK_SIZE        256//240//2048//1024//256
#define I2S_DMA_BUFFER_SIZE   1024//(240*4)//8192//4096//1024
#define I2S_BLOCK_NUM         (sizeof(i2s_dma_buffer)/I2S_BLOCK_SIZE)
#define PCM_FRAME_SIZE        256//240
#elif (CONFIG_AUDIO_CODEC_SEL == 2)
#define I2S_BLOCK_SIZE        512
#define I2S_DMA_BUFFER_SIZE   4096
#define I2S_BLOCK_NUM         (sizeof(i2s_dma_buffer)/I2S_BLOCK_SIZE)
#define PCM_FRAME_SIZE        512
#else
#define I2S_BLOCK_SIZE        240
#define I2S_DMA_BUFFER_SIZE   (240*4)
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
#if (CONFIG_AUDIO_CODEC_SEL == 2)
const i2s_config_t i2s_cfg = {
    .sample_rate  = I2S_SAMPLE_8K,
    .sample_width = I2S_BITWIDTH_16BIT,
    .ws_width     = I2S_BITWIDTH_16BIT,
    .workmode     = I2S_WORKMODE_SLAVE,
    .pcmmode      = I2S_PCMMODE_MONO,
};

#else
const i2s_config_t i2s_cfg = {
    .sample_rate  = I2S_SAMPLE_16K,
    .sample_width = I2S_BITWIDTH_16BIT,
    .ws_width     = I2S_BITWIDTH_16BIT,
    .workmode     = I2S_WORKMODE_SLAVE,
    .pcmmode      = I2S_PCMMODE_MONO,
};
#endif

static co_timer_t audio_data_delay_send_timer;
#define AUDIO_DATA_DELAY_SEND_TID         &audio_data_delay_send_timer

static co_timer_t audio_stop_delay_timer;
#define AUDIO_STOP_DELAY_TID         &audio_stop_delay_timer

#if (CONFIG_AUDIO_CODEC_SEL == 1)
static uint8_t         packet_start[20] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif
static uint8_t         packet_end[20]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
																					 
static bool            m_audio_start = 0;
static bool            audio_data_delay_flag = 0;
#if (PIN_DMIC_CLOCK != 0xFF)
static bool            m_dmic_use = 0;
#endif
//#define AUDIO_SOFT_IIR_PA

#ifdef AUDIO_SOFT_IIR_PA
static int coef_a=0x7B12; //100Hz
static int coef_b=0x7D89;
// int coef_a=0x7A1B; //120Hz
// int coef_b=0x7D0E;
 //int coef_a=0x71BE; //300Hz
 //int coef_b=0x78DF;
// int coef_a=0x690c; //500Hz
// int coef_b=0x7486;
static short X1 = 0;
static int Y1 = 0;

static uint8_t sw_pa(int16_t *src_data)
{
    int32_t data_temp;
    uint8_t result = 1; //1 for overflow
    data_temp = (int32_t)(*src_data);

    data_temp = data_temp << 4;
    if (data_temp < -32768)
    {
        data_temp = -32768;
    }
    else if (data_temp > 32767)
    {
        data_temp = 32767;
    }
    else
    {
        result = 0;
    }

    *src_data = data_temp;
    return result;
}

static uint8_t pcm_data_amplify(uint8_t *pcm_data_buf, uint32_t data_len)
{
    uint32_t i;
    uint8_t result = 0; //0 for nothing,other num for overflow
    int16_t audio_date_temp = 0;

    for (i=0;i<data_len;i += 2)
    {
        audio_date_temp = (pcm_data_buf[i + 1] << 8) + pcm_data_buf[i];
        result += sw_pa(&audio_date_temp);
        pcm_data_buf[i] = (audio_date_temp & 0xff); 
        pcm_data_buf[i + 1] = (audio_date_temp >> 8); 
    }
    return result;
}

static short OneOrderIIR (int B0, int A0, int *y1, short x0, short *x1)
{
  int y0; //Q31
  __int64 tmp; //Q46
  tmp = 0;
  tmp += (__int64)B0 * (x0<<16);
  tmp -= (__int64)B0 * ((*x1)<<16);
  tmp += (__int64)A0 * (*y1);

  tmp = tmp>>15;

  if ( tmp >= 0x7fffffff) y0 = 0x7fffffff;
  else if ( tmp < -0x7fffffff) y0 = -0x7fffffff;
  else y0 = (int)tmp;

  *x1 = x0;
  *y1 = y0;
  y0=y0>>16;

  return (short)y0;
}
#endif
#if (CONFIG_AUDIO_CODEC_SEL != 2)
static void app_audio_send_data_to_phone(uint8_t *p_data, uint16_t len)
{
    switch(CONFIG_AUDIO_SEND_PATH)
    {
        case 0:
            #if (BLE_TSPP_SERVER)
            app_tspp_server_send_data(0, p_data, len);  
            #endif
            break;
        case 1:
            #if (BLE_APP_HID)
            app_hid_send_data_report(p_data, (uint8_t)len);
            #endif
            break;
        case 2:
            #if (BLE_ATVV_SERVER)
            app_atvv_server_send_data(0, p_data, (uint8_t)len);  
            #endif
            break;
        default:
        break;
    }
}
#endif

void app_audio_rx_cmd_handler(        struct app_audio_rx_cmd_t const *param)
{
    uint8_t output[PCM_FRAME_SIZE] = {0};
    uint8_t pcm_frame[PCM_FRAME_SIZE+1] = {0};
    uint16_t written = 0;
#ifdef AUDIO_SOFT_IIR_PA
    uint8_t pcm_frame_bypass[PCM_FRAME_SIZE+1] = {0};
    short y = 0;
    short xin = 0;
    int i = 0;
#endif
    
    #if (CONFIG_AUDIO_CODEC_SEL == 0)
    uint16_t len = 0;
    #endif
    
	#if (CONFIG_AUDIO_CODEC_SEL == 2)
	int16_t prevsample = 0;
    uint16_t previndex = 0;
	#endif
    if(!m_audio_start)
        return;
    
    while(dma_fifo_len(&i2s_dma_rx_fifo) >= PCM_FRAME_SIZE)
    {
        
        if(!m_audio_start)
            return;
        
        dma_fifo_out(&i2s_dma_rx_fifo, pcm_frame, PCM_FRAME_SIZE);
#ifdef AUDIO_SOFT_IIR_PA
        uint8_t *px = pcm_frame;
        uint8_t *py = pcm_frame_bypass;
        
        for(i = 0; i <(PCM_FRAME_SIZE>>1); i++) {
            xin = *px++;
            xin = xin + (*(px++)<<8);

            y = OneOrderIIR(coef_b, coef_a, &Y1, xin, &X1);

            *py++ = y & 0xFF;
            *py++ = (y>>8)&0xFF;
        }
#endif        
#if (CONFIG_AUDIO_CODEC_SEL == 0)
        len = 40;

#ifdef AUDIO_SOFT_IIR_PA  
        len = codec_encode(pcm_frame_bypass, PCM_FRAME_SIZE, output, sizeof(output));
#else
        len = codec_encode(pcm_frame, PCM_FRAME_SIZE, output, sizeof(output));
#endif
        //co_assert(len > 0);
        //log_debug("l=%d.w=%d\r\n", len, written);
        if(audio_data_delay_flag && len>0) {
            //app_audio_send_data_to_phone((uint8_t*)output, written); 
            app_audio_send_data_to_phone((uint8_t*)output, len); 
        }
#endif

#if (CONFIG_AUDIO_CODEC_SEL == 2)
        prevsample = ima_adpcm_get_prevsample();
        previndex = ima_adpcm_get_previndex();
        written = ima_adpcm_encode_proc_big((short*)pcm_frame, output, PCM_FRAME_SIZE/2);
//        log_debug("enc_len=%d\r\n", written);
        if(audio_data_delay_flag)
            app_atvv_notify_audio_frame(output, written, prevsample, previndex);
#endif

#if (CONFIG_AUDIO_CODEC_SEL == 1)
#ifdef AUDIO_SOFT_IIR_PA
        pcm_data_amplify(pcm_frame_bypass, PCM_FRAME_SIZE);
        written = codec_encode(pcm_frame_bypass, PCM_FRAME_SIZE, output, sizeof(output));
#else
        written = codec_encode(pcm_frame, PCM_FRAME_SIZE, output, sizeof(output));
#endif
        if(audio_data_delay_flag) {
	        if(is_audio_test_mode)
	        {
	            app_tspp_server_send_data(0, output, AUDIO_DATA_LEN_PER_PACKET);
	        }
	        else
	            app_audio_send_data_to_phone((uint8_t*)output, AUDIO_DATA_LEN_PER_PACKET);
#if 0
            app_audio_send_data_to_phone((uint8_t*)sample_audio_data[sample_audio_data_pos], AUDIO_DATA_LEN_PER_PACKET);
            sample_audio_data_pos++;
            if(sample_audio_data_pos >= sample_audio_data_count)
            {
                sample_audio_data_pos = 0;
            }
#endif
        }
#endif
    }
}

void i2s_rx_dma_handler(dma_status_t status, uint32_t cur_src_addr, uint32_t cur_dst_addr, uint32_t xfer_size)
{
    dma_fifo_set_in_pointer(&i2s_dma_rx_fifo, cur_dst_addr);

    struct app_audio_rx_cmd_t * cmd = KE_MSG_ALLOC(APP_AUDIO_RX_CMD,
            KE_BUILD_ID(TASK_APP, 0),
            KE_BUILD_ID(TASK_APP, 0),
            app_audio_rx_cmd_t);
    cmd->len = xfer_size*2;
    // Send the message
    ke_msg_send(cmd);
}

static void app_audio_pin_init(void)
{
    #if 1
    //audio control pin config
    pinmux_config(PIN_AUDIO_CTRL, PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(BIT_MASK(PIN_AUDIO_CTRL), PMU_PIN_MODE_PP);
    gpio_set_direction(BIT_MASK(PIN_AUDIO_CTRL), GPIO_OUTPUT);
    gpio_write(BIT_MASK(PIN_AUDIO_CTRL), GPIO_LOW);
    #endif

    #if ((PIN_DMIC_CLOCK != 0xFF) && (PIN_DMIC_DATA != 0xFF))
    {
        // dig mic
        /// 1. dmic pin setting
        pinmux_config(15,  PINMUX_TIMER0_BKIN_CFG);
        pinmux_config(17,  PINMUX_TIMER0_BKIN_CFG);
        pinmux_config(20,  PINMUX_TIMER0_BKIN_CFG);
        pinmux_config(PIN_DMIC_CLOCK, PINMUX_DMIC_CLK_CFG);
        pinmux_config(PIN_DMIC_DATA, PINMUX_DMIC_IN_CFG);
        
        if (PIN_DMIC_CLOCK > 32)
        {
            #ifndef CONFIG_GPIO1_ABSENT
            pmu_pin_driven_current_set_ex(BIT_MASK(PIN_DMIC_CLOCK-32), PMU_PIN_DRIVEN_CURRENT_2MA);
            #endif
        }
        else {
            pmu_pin_driven_current_set(BIT_MASK(PIN_DMIC_CLOCK), PMU_PIN_DRIVEN_CURRENT_2MA);
        }
        if (PIN_DMIC_CLOCK > 32)
        {
            #ifndef CONFIG_GPIO1_ABSENT
            pmu_pin_driven_current_set_ex(BIT_MASK(PIN_DMIC_DATA-32), PMU_PIN_DRIVEN_CURRENT_2MA);
            #endif
        }
        else {
            pmu_pin_driven_current_set(BIT_MASK(PIN_DMIC_DATA), PMU_PIN_DRIVEN_CURRENT_2MA);
        }
    }
    #endif
    // ana mic
    #if (PIN_MIC_P != 0xFF && PIN_MIC_P != 0)
    {
        pmu_pin_mode_set(BIT_MASK(PIN_MIC_P), PMU_PIN_MODE_FLOAT);
        REGW0(&HS_PMU->GPIO_IE_CTRL, BIT_MASK(PIN_MIC_P));
        pmu_pin_driven_current_set(BIT_MASK(PIN_MIC_P), PMU_PIN_DRIVEN_CURRENT_2MA);
    }
    #endif

    #if ( PIN_MIC_N != 0xFF && PIN_MIC_N != 0)
    {
        #if (PIN_DMIC_CLOCK != 0xFF)
        m_dmic_use = 1;
        #endif
        pmu_pin_mode_set(BIT_MASK(PIN_MIC_N), PMU_PIN_MODE_FLOAT);
        REGW0(&HS_PMU->GPIO_IE_CTRL, BIT_MASK(PIN_MIC_N));
        pmu_pin_driven_current_set(BIT_MASK(PIN_MIC_N), PMU_PIN_DRIVEN_CURRENT_2MA);
    }
    #endif
}

static void app_audio_codec_init(void)
{
    #if (CONFIG_AUDIO_CODEC_SEL == 0)
    codec_init(0, 16);
    #endif
    #if (CONFIG_AUDIO_CODEC_SEL == 1)
    {
        #if (BLE_APP_HID)
        app_hid_send_cmd_report(packet_start, sizeof(packet_start));
        #endif
        codec_init(1, 26);
    }
    #endif
    #if (CONFIG_AUDIO_CODEC_SEL == 2)
    {
        ima_adpcm_encode_init();
    }
    #endif
}
/**
* @brief Audio module initialization.
**/
void app_audio_init(void)
{
    #if (PIN_DMIC_CLOCK != 0xFF)
    m_dmic_use = 0;
    #endif
    memset(&i2s_dma_buffer, 0, sizeof(i2s_dma_buffer));
    
    app_audio_pin_init();
    app_audio_codec_init();
    #if 0//(CONFIG_AUDIO_SEND_PATH == 1)
    m_send_start = 0;
    co_fifo_init(&audio_fifo, send_buf, AUDIO_SEND_BUF_SIZE);
    #endif
}

static void audio_data_delay_timer_handler(co_timer_t *timer, void *param)
{
    audio_data_delay_flag = true;
	#if (APP_CUSTOMER_ID == 2 || APP_CUSTOMER_ID==3)
    led_on();
	#endif
}

void app_audio_open_callback(void* param)
{
    (void)param;
    audio_config_t config;
    config.pga_gain = AUDIO_MIC_IN_PGA_GAIN_30DB;
    config.adc_gain = AUDIO_ADC_GAIN_0DB;
    switch (PIN_MIC_P)
    {
        case 0:
            config.sel_pga_input = AUDIO_SELECT_PAG_INPUT_VIP_MIC_P;
            break;
        case 2:
            config.sel_pga_input = AUDIO_SELECT_PAG_INPUT_GPIO2;
            break;
        case 3:
            config.sel_pga_input = AUDIO_SELECT_PAG_INPUT_GPIO3;
            break;
        case 7:
            config.sel_pga_input = AUDIO_SELECT_PAG_INPUT_GPIO7;
            break;
        #if defined(CONFIG_HS6621) && (CONFIG_HARDWARE_VERSION>1)
        case 8:
            config.sel_pga_input = AUDIO_SELECT_PAG_INPUT_GPIO8;
            break;
        #endif
    }
    switch (PIN_MIC_N)
    {
        case 0:
            config.sig_or_diff = AUDIO_EN_PGA_SINGLE_IN_VIN_MIC_1;
            break;
        default:
            config.sig_or_diff = AUDIO_EN_PGA_SINGLE_IN_INNER_REF;
            break;
    }

    audio_set_config(&config);
    
    if ((PIN_DMIC_CLOCK != 0xFF) && (PIN_DMIC_DATA != 0xFF))
    {
        /// open dmic
        audio_dmic_config_t dmic_config;
#if defined(CONFIG_HS6621)
        dmic_config.clk = AUDIO_DMIC_CLOCK_SEL_2P4M;
#else
        dmic_config.clk = AUDIO_DMIC_CLOCK_SEL_2M;
#endif
        dmic_config.ctrl = AUDIO_DMIC_CLOCK_OUT;
        audio_dmic_open(&dmic_config);
    }

    i2s_rx_config(&i2s_cfg);
    
    /// dma setting
    dma_fifo_init(&i2s_dma_rx_fifo, i2s_dma_buffer, sizeof(i2s_dma_buffer));
    dma_init();
    
    i2s_dma_rx = i2s_rx_dma_config((HS_DMA_CH_Type *)NULL, &i2s_dma_rx_cfg);
    
    gpio_write(BIT_MASK(PIN_AUDIO_CTRL), GPIO_HIGH);
    dma_start_with_lli(i2s_dma_rx);
    /// start i2s
    i2s_rx_start(&i2s_cfg);
  
    co_timer_set(AUDIO_DATA_DELAY_SEND_TID, 300, TIMER_ONE_SHOT, audio_data_delay_timer_handler, NULL);
    
    log_debug("[AUDIO]: Audio start finish.\n");
}

/**
* @brief Start voice sampling.
**/
void app_audio_start(void)
{
    if(ke_state_get(TASK_APP) < APPM_CONNECTED) return;
    //sample_audio_data_pos = 0;
    //audio is already started
    if(m_audio_start)
    {
        co_timer_del(AUDIO_STOP_DELAY_TID);
        return;
    }
#ifdef CONFIG_HS6621
    cpm_set_clock_div(CPM_ULP_RAM_CLK, 4); 
    pmu_pll_startup(96);
#endif

    m_audio_start = true;

    audio_data_delay_flag = false;

    app_audio_init();
    
    //app_audio_send_data_to_phone(packet_start, sizeof(packet_start));
        
    /// start audio adc
    audio_open(app_audio_open_callback);

    log_debug("[AUDIO]: Audio start step 1.\n");
}

void disable_latency_timer_handler(co_timer_t *timer, void *param)
{
    static uint8_t count = 0;
    if(count++ < 100)
    {
        #if (BLE_APP_HID)
        //app_audio_send_data_to_phone((uint8_t*)packet_end, sizeof(packet_end));   
        app_hid_send_cmd_report((uint8_t*)packet_end, sizeof(packet_end));
        struct ps2_keyboard_msg report;
        memset(&report, 0, sizeof(struct ps2_keyboard_msg));
        report.report_id = 1;
        app_hid_send_keyboard_report(report);
        #endif
    }
    else
    {
        co_timer_del(AUDIO_STOP_DELAY_TID);
        count = 0;
        
        #if (CONFIG_AUDIO_SEND_PATH == 0)
        {
            uint8_t data[4]={0x11, 0x22, 0x33, 0x44};
            app_audio_send_data_to_phone(data, 4);
        }
        #endif
    }
}

/**
* @brief Stop voice sampling.
**/

void app_audio_stop(void)
{
    //audio is already stopped
    if(!m_audio_start) {
        return;
    }

    m_audio_start = false;
    audio_data_delay_flag = false;

    if (i2s_dma_rx != NULL) {
        /// close i2s
        i2s_rx_stop();

        /// stop dma
        dma_stop(i2s_dma_rx);
        dma_release(i2s_dma_rx);
        i2s_dma_rx = NULL;
    }

    audio_close();

    gpio_write(BIT_MASK(PIN_AUDIO_CTRL), GPIO_LOW);
    
    #if (CONFIG_AUDIO_SEND_PATH ==1)
    #if (BLE_APP_HID)
    app_hid_send_cmd_report((uint8_t*)packet_end, sizeof(packet_end));   
    struct ps2_keyboard_msg report;
    memset(&report, 0, sizeof(struct ps2_keyboard_msg));
    report.report_id = 1;
    app_hid_send_keyboard_report(report);
    #endif
    
    co_timer_set(AUDIO_STOP_DELAY_TID, 10, TIMER_REPEAT, disable_latency_timer_handler, NULL);
    #endif
    #if (CONFIG_AUDIO_SEND_PATH == 0)
    uint8_t data[4]={0x11, 0x22, 0x33, 0x44};
    app_audio_send_data_to_phone(data, 4);
    #endif
    
	#if (APP_CUSTOMER_ID == 2 || APP_CUSTOMER_ID==3)
    led_off();
	#endif
    log_debug("[AUDIO]: Audio stop.\n");
}

/**
* @brief Get audio state(start or stop).
**/
bool app_audio_state_get(void)
{
    return m_audio_start;
}

#endif /* #if CONFIG_AUDIO_ENABLE */
