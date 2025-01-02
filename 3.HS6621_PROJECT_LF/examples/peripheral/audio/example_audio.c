/**
 * @file example_i2s.c
 * @brief example for I2S
 * @date 2017/07/12 16:15:33
 * @author liqiang
 *
 * @defgroup EXAMPLE_AUDIO Audio Example
 * @ingroup EXAMPLE
 * @brief example for Audio
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "ke_timer.h"
#include "co_utils.h"               // Common functions
#include "co_timer_pmu.h"
#include "co_debug.h"
#include "peripheral.h"
#include "codec_wm8753.h"


/*********************************************************************
 * MACROS
 */
#define AUDIO_PIN_I2C_CLK           33
#define AUDIO_PIN_I2C_DAT           18
    
#define AUDIO_PIN_I2S_TX_WS         16
#define AUDIO_PIN_I2S_TX_CLK        29
#define AUDIO_PIN_I2S_TX_DAT        27

/// i2s buffer
#define I2S_BLOCK_SIZE        256 //2048//2048//1024//256
#define I2S_DMA_BUFFER_SIZE   1024 //9216//8192//4096//1024
#define I2S_BLOCK_NUM         (I2S_DMA_BUFFER_SIZE/I2S_BLOCK_SIZE)

/*********************************************************************
 * LOCAL VARIABLES
 */
static void i2s_rx_dma_handler(dma_status_t status, uint32_t cur_src_addr, uint32_t cur_dst_addr, uint32_t xfer_size);
//static void i2s_tx_dma_handler(dma_status_t status, uint32_t cur_src_addr, uint32_t cur_dst_addr, uint32_t xfer_size);

/// i2c config
const i2c_sim_config_t i2c_config = {AUDIO_PIN_I2C_CLK, AUDIO_PIN_I2C_DAT, 10};

/// i2s config
#pragma pack(4)
int16_t   i2s_dma_buffer[I2S_DMA_BUFFER_SIZE];
#pragma pack()

//dma_fifo_t i2s_dma_rx_fifo;
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

//dma_fifo_t i2s_dma_tx_fifo;
static HS_DMA_CH_Type *i2s_dma_tx = NULL;
DMA_DECLARE_LLIP(i2s_dma_block_llip_tx, I2S_BLOCK_NUM);

const i2s_dma_config_t i2s_dma_tx_cfg = {
    .use_fifo       = true,
    .buffer         = i2s_dma_buffer,
    .buffer_len     = sizeof(i2s_dma_buffer),
    .block_llip     = i2s_dma_block_llip_tx,
    .block_num      = I2S_BLOCK_NUM,
    .callback       = NULL,
};
    

const i2s_config_t i2s_cfg = {
    .sample_rate  = I2S_SAMPLE_16K,
    .sample_width = I2S_BITWIDTH_16BIT,
    .ws_width     = I2S_BITWIDTH_16BIT,
    .workmode     = I2S_WORKMODE_SLAVE,
    .pcmmode      = I2S_PCMMODE_STEREO, //I2S_PCMMODE_MONO,
};

static uint8_t tx_start_flag = 0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/**
 * @brief i2s_dma_handler()
 *
 * @param[in] status  
 * @param[in] cur_src_addr  
 * @param[in] cur_dst_addr  
 * @param[in] xfer_size  
 *
 * @return 
 **/
static void i2s_rx_dma_handler(dma_status_t status, uint32_t cur_src_addr, uint32_t cur_dst_addr, uint32_t xfer_size)
{
    //uint32_t i2s_frame[I2S_BLOCK_SIZE/4];
    
    static uint8_t cnt = 0;

    //log_debug("rxdma(%d): %08X->%08X [%d]\n", status, cur_src_addr, cur_dst_addr, xfer_size);
    
    if(0 == tx_start_flag)
    {
        cnt++;
        if(cnt >= 2)
        {
            tx_start_flag = 1;
            dma_start_with_lli(i2s_dma_tx);
        }
    }
    //log_debug("*");
}

static void _audio_codec_start(void)
{
    memset(&i2s_dma_buffer, 0, sizeof(i2s_dma_buffer));

    /// config i2s
    i2s_config(&i2s_cfg);
    i2s_rx_config(&i2s_cfg);

    /// dma setting
    dma_init();
    
    i2s_dma_tx = i2s_dma_config((HS_DMA_CH_Type *)NULL, &i2s_dma_tx_cfg);
    i2s_dma_rx = i2s_rx_dma_config((HS_DMA_CH_Type *)NULL, &i2s_dma_rx_cfg);
    
    /// start wm8753   
    i2c_open(I2C_MODE_MASTER, 100000);
    codec_wm8753_init(CODEC_WM8753_MASTER, CODEC_WM8753_SAMPLE_RATE_16KHZ, CODEC_WM8753_MCLK_24MHZ);

    /// start dm rx
    dma_start_with_lli(i2s_dma_rx);

    /// start i2s
    i2s_rx_start(&i2s_cfg);
        
    i2s_start();

    log_debug("audio start ....\r\n");
}

static void _audio_codec_stop(void)
{
    /// close i2c
    i2c_close();
    /// close i2s
    i2s_rx_stop();
    i2s_stop();
    /// stop dma
    dma_stop(i2s_dma_rx);
    dma_release(i2s_dma_rx);
    i2s_dma_rx = NULL;
    dma_stop(i2s_dma_tx);
    dma_release(i2s_dma_tx);
    i2s_dma_tx = NULL;

    tx_start_flag = 0;
    log_debug("audio stop\r\n");
}

void app_audio_open_callback(void* param)
{
    // audio adc setting
    audio_config_t config;
    config.pga_gain = AUDIO_MIC_IN_PGA_GAIN_30DB;
    config.sel_pga_input = AUDIO_SELECT_PAG_INPUT_GPIO3;
    config.adc_gain = AUDIO_ADC_GAIN_0DB;
    config.sig_or_diff = AUDIO_EN_PGA_SINGLE_IN_INNER_REF;
    audio_set_config(&config);

    /// codec config and start
    _audio_codec_start();

}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_audio(void)
{
    cpm_set_clock_div(CPM_ULP_RAM_CLK, 4); 
    pmu_pll_startup(96);
    
    uint8_t pin = 3;
    /// 1. audio adc pin setting
    pmu_pin_mode_set(BITMASK(pin), PMU_PIN_MODE_FLOAT);
    REGW0(&HS_PMU->GPIO_IE_CTRL, BITMASK((pin)));
    //HS_PMU->GPIO_IE_CTRL &= ~BITMASK(3); 
    pmu_pin_driven_current_set(BITMASK(pin), PMU_PIN_DRIVEN_CURRENT_2MA);

    
    /// wm8753 pin setting
    pinmux_config(AUDIO_PIN_I2C_CLK, PINMUX_I2C_MST_SCK_CFG);
    pinmux_config(AUDIO_PIN_I2C_DAT, PINMUX_I2C_MST_SDA_CFG);
    #ifndef CONFIG_GPIO1_ABSENT
    pmu_pin_driven_current_set_ex(BIT_MASK(AUDIO_PIN_I2C_CLK-32), PMU_PIN_DRIVEN_CURRENT_2MA);
    pmu_pin_mode_set_ex(BIT_MASK(AUDIO_PIN_I2C_CLK-32), PMU_PIN_MODE_OD);
    #else
    pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_I2C_CLK), PMU_PIN_DRIVEN_CURRENT_2MA);
    pmu_pin_mode_set(BIT_MASK(AUDIO_PIN_I2C_CLK), PMU_PIN_MODE_OD);
    #endif
    pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_I2C_DAT), PMU_PIN_DRIVEN_CURRENT_2MA);
    pmu_pin_mode_set(BIT_MASK(AUDIO_PIN_I2C_DAT), PMU_PIN_MODE_OD);
    
    /// codec i2s pin setting
    pinmux_config(AUDIO_PIN_I2S_TX_DAT, PINMUX_I2S_SDO_0_CFG);
    pinmux_config(AUDIO_PIN_I2S_TX_WS,  PINMUX_I2S_TX_WS_CFG);
    pinmux_config(AUDIO_PIN_I2S_TX_CLK,  PINMUX_I2S_TX_SCLK_CFG);
    pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_I2S_TX_DAT), PMU_PIN_DRIVEN_CURRENT_2MA);
    pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_I2S_TX_WS), PMU_PIN_DRIVEN_CURRENT_2MA);
    pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_I2S_TX_CLK), PMU_PIN_DRIVEN_CURRENT_2MA);
        
    /// start audio adc
    audio_open(app_audio_open_callback);

    while(1);
}

/** @} */

