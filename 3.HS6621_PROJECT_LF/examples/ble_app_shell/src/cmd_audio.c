/**
 ****************************************************************************************
 *
 * @file shell.c
 *
 * @brief Simple CLI shell application
 *
 * Copyright (C) RivieraWaves 2009-2015
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

#include "rwip_config.h"            // SW configuration

#include <stdio.h>
#include <string.h>

#if (CONFIG_APP_SHELL)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "ke_timer.h"
#include "co_utils.h"               // Common functions
#include "co_timer_pmu.h"
#include "co_debug.h"
#include "peripheral.h"
#include "shell.h"
#include "cmd.h"
#include "codec_wm8753.h"

#if (SHELL_CMD_USE_AUDIO)

/*
 * DEFINES
 ****************************************************************************************
 */
/// i2c pinmux for wm8753s
#define AUDIO_PIN_I2C_CLK           25 //33
#define AUDIO_PIN_I2C_DAT           18

#define AUDIO_PIN_I2S_TX_WS         16
#define AUDIO_PIN_I2S_TX_CLK        29
#define AUDIO_PIN_I2S_TX_DAT        27

/// i2s buffer
#define I2S_BLOCK_SIZE        256 //2048//2048//1024//256
#define I2S_DMA_BUFFER_SIZE   1024 //9216//8192//4096//1024
#define I2S_BLOCK_NUM         (I2S_DMA_BUFFER_SIZE/I2S_BLOCK_SIZE)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
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
/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
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

static void cmd_audio_codec_start(void)
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

static void cmd_audio_codec_stop(void)
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

void cmd_audio(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    if (argc == 0) {
print_help:
        log_debug(
                "Usage: audio [options]...\r\n"
                "    start s pin:                         start audio with single pin(analog mic)\r\n"
                "    start d pin:                         start audio with differential(analog mic)\r\n"
                "    start dmic pin pin                   start audio with digital mic, clk, data\r\n"
                "    stop:                                stop\r\n"
                "\r\n"
                "Note:\r\n"
                "    1. select input pin:\r\n"
                "         AUDIO_SELECT_PAG_INPUT_VIP_MIC_P   = 0, \r\n"
                "         AUDIO_SELECT_PAG_INPUT_GPIO1       = 1, GPIO1 INPUT mode.\r\n"
                "         AUDIO_SELECT_PAG_INPUT_GPIO2       = 2, GPIO2 INPUT mode.\r\n"
                "         AUDIO_SELECT_PAG_INPUT_GPIO3       = 3, GPIO3 INPUT mode. \r\n"
                "         AUDIO_SELECT_PAG_INPUT_GPIO4       = 4, GPIO4 INPUT mode. \r\n"
                "         AUDIO_SELECT_PAG_INPUT_GPIO5       = 5, GPIO5 INPUT mode. \r\n"
                "         AUDIO_SELECT_PAG_INPUT_GPIO6       = 6, GPIO6 INPUT mode. \r\n"
                "         AUDIO_SELECT_PAG_INPUT_GPIO7       = 7, GPIO7 INPUT mode. \r\n"
                "\r\n"
                "Example: \r\n"
                "    audio start s 3 \r\n"
                "    audio start d 0 \r\n"
                "    audio start dmic 19 21 \r\n"
                "    audio stop \r\n"
                );

        return;
    }
    else if (argc == 3 && strcmp(argv[0], "start") == 0) {
        /// use analog mic
        uint8_t pin = strtol(argv[2], NULL, 10);
        if (pin > 7) {
            log_debug("the input error! pin=%x\r\n", pin);
            return;
        }
        /// 1. audio adc pin setting
        pmu_pin_mode_set(BITMASK(pin), PMU_PIN_MODE_FLOAT);
        REGW0(&HS_PMU->GPIO_IE_CTRL, BITMASK((pin)));
        //HS_PMU->GPIO_IE_CTRL &= ~BITMASK(3); 
        pmu_pin_driven_current_set(BITMASK(pin), PMU_PIN_DRIVEN_CURRENT_2MA);

        
        /// wm8753 pin setting
        pinmux_config(AUDIO_PIN_I2C_CLK, PINMUX_I2C_MST_SCK_CFG);
        pinmux_config(AUDIO_PIN_I2C_DAT, PINMUX_I2C_MST_SDA_CFG);
        #if 0 //#ifndef CONFIG_GPIO1_ABSENT
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
        
        // 2. audio adc setting
        audio_config_t config;
        config.pga_gain = AUDIO_MIC_IN_PGA_GAIN_30DB;
        config.sel_pga_input = AUDIO_SELECT_PAG_INPUT_GPIO3;
        config.adc_gain = AUDIO_ADC_GAIN_0DB;

        if (strcmp(argv[1], "s") == 0) {
            config.sig_or_diff = AUDIO_EN_PGA_SINGLE_IN_INNER_REF;
        }
        else if (strcmp(argv[1], "d") == 0) {
            config.sig_or_diff = AUDIO_EN_PGA_SINGLE_IN_VIN_MIC_1;
        }
        else
            {
            goto print_help;
        }
        
        /// start audio adc
        audio_open(NULL);
        audio_set_config(&config);

        /// codec config and start
        cmd_audio_codec_start();
    }
    else if (argc == 4 && strcmp(argv[0], "start") == 0 && strcmp(argv[1], "dmic") == 0) {
        /// use dmic
        uint8_t pin_clk = strtol(argv[2], NULL, 10); // 19
        uint8_t pin_data = strtol(argv[3], NULL, 10); // 21

        /// 1. dmic pin setting
        pinmux_config(15,  PINMUX_TIMER0_BKIN_CFG);
        pinmux_config(17,  PINMUX_TIMER0_BKIN_CFG);
        pinmux_config(20,  PINMUX_TIMER0_BKIN_CFG);
        pinmux_config(pin_clk, PINMUX_DMIC_CLK_CFG);
        pinmux_config(pin_data, PINMUX_DMIC_IN_CFG);
        if (pin_clk > 32)
        {
            #ifndef CONFIG_GPIO1_ABSENT
            pmu_pin_driven_current_set_ex(BIT_MASK(pin_clk-32), PMU_PIN_DRIVEN_CURRENT_2MA);
            #endif
        }
        else {
            pmu_pin_driven_current_set(BIT_MASK(pin_clk), PMU_PIN_DRIVEN_CURRENT_2MA);
        }
        if (pin_data > 32)
        {
            #ifndef CONFIG_GPIO1_ABSENT
            pmu_pin_driven_current_set_ex(BIT_MASK(pin_data-32), PMU_PIN_DRIVEN_CURRENT_2MA);
            #endif
        }
        else {
            pmu_pin_driven_current_set(BIT_MASK(pin_data), PMU_PIN_DRIVEN_CURRENT_2MA);
        }

        
        /// wm8753 pin setting
        pinmux_config(AUDIO_PIN_I2C_CLK, PINMUX_I2C_MST_SCK_CFG);
        pinmux_config(AUDIO_PIN_I2C_DAT, PINMUX_I2C_MST_SDA_CFG);
        #if 0 //#ifndef CONFIG_GPIO1_ABSENT
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
        
       
        audio_config_t config1;
        config1.pga_gain = AUDIO_MIC_IN_PGA_GAIN_30DB;
        config1.sel_pga_input = AUDIO_SELECT_PAG_INPUT_GPIO3;
        config1.adc_gain = AUDIO_ADC_GAIN_0DB;
        config1.sig_or_diff = AUDIO_EN_PGA_SINGLE_IN_INNER_REF;
        
        /// start audio adc
        audio_open(NULL);
        audio_set_config(&config1);

        /// open dmic
        audio_dmic_config_t config;
//        config.clk = AUDIO_DMIC_CLOCK_SEL_2P4M;
        config.ctrl = AUDIO_DMIC_CLOCK_OUT;
        audio_dmic_open(&config);
    
        /// codec config and start
        cmd_audio_codec_start();

    }
    else if (strcmp(argv[0], "stop") == 0) {
        /// codec stop
        cmd_audio_codec_stop();
        /// audio close
        audio_close();

        pinmux_config(AUDIO_PIN_I2C_CLK, PINMUX_GPIO_MODE_CFG);
        pinmux_config(AUDIO_PIN_I2C_DAT, PINMUX_GPIO_MODE_CFG);
        #if 0 //#ifndef CONFIG_GPIO1_ABSENT
        pmu_pin_mode_set_ex(BIT_MASK(AUDIO_PIN_I2C_CLK-32), PMU_PIN_MODE_OD);
        #else
        pmu_pin_mode_set(BIT_MASK(AUDIO_PIN_I2C_CLK), PMU_PIN_MODE_OD);
        #endif
        pmu_pin_mode_set(BIT_MASK(AUDIO_PIN_I2C_DAT), PMU_PIN_MODE_OD);
        
        /// codec i2s pin setting
        pinmux_config(AUDIO_PIN_I2S_TX_DAT, PINMUX_GPIO_MODE_CFG);
        pinmux_config(AUDIO_PIN_I2S_TX_WS,  PINMUX_GPIO_MODE_CFG);
        pinmux_config(AUDIO_PIN_I2S_TX_CLK,  PINMUX_GPIO_MODE_CFG);
    }
    else {
        goto print_help;
    }
}
#endif

#endif //(CONFIG_APP_SHELL)

/// @} APP

