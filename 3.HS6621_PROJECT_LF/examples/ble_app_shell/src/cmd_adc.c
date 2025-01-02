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
#if (SHELL_CMD_USE_ADC)

/*
 * DEFINES
 ****************************************************************************************
 */
#define BLOCK_SIZE                256
#define ADC_DMA_BUFFER_SIZE       1024
#define BLOCK_NUM                 (ADC_DMA_BUFFER_SIZE*4/BLOCK_SIZE)
DMA_DECLARE_LLIP(adc_dma_block_llip, BLOCK_NUM);


/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static uint32_t adc_dma_buffer[ADC_DMA_BUFFER_SIZE];
static dma_fifo_t dma_fifo;
static HS_DMA_CH_Type *adc_dma = NULL;


/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void _adc_callback(adc_event_t event, float adc_data)
{
    if (event&ADC_EVENT_CH_T_DONE)
    {
        log_debug("temperature = %f\n", adc_data);
    }
    else if (event&ADC_EVENT_CH_V_DONE)
    {
        log_debug("battery = %f\n", adc_data);
    }
    else if (event&ADC_EVENT_DMA_DONE)
    {
        adc_stop_convert();
        adc_stop();
        dma_stop(adc_dma);
        int32_t sum1 = 0;

        log_debug("dma data:\n");
        for(uint32_t i = 0; i < ADC_DMA_BUFFER_SIZE; ++i)
        {
            sum1 += (int16_t)adc_dma_buffer[i];
            if((i>0)&&(0 == (i%8)))
            {
                log_debug("\n");
            }
            log_debug("%08x ", adc_dma_buffer[i]);
        }
        log_debug("\n");

        sum1 = sum1/ADC_DMA_BUFFER_SIZE;
        log_debug("ch:5, %08x, %f\n", sum1, (float)(sum1)*0.8/2048.0);

        log_debug("adc_dma_buffer addr: %08x\n", adc_dma_buffer);
    }
    else
    {
        log_debug("event=%x, pin = %f\n", event, adc_data);
    }
}


void cmd_adc(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    // init adc 
    #if 0 //CONFIG_HARDWARE_VERSION == 1 //HS6621 A1
    //pinmux_config(0, PINMUX_ANALOG_CH0_PIN0_CFG);
    //pinmux_config(1, PINMUX_ANALOG_CH1_PIN1_CFG);
    pinmux_config(2, PINMUX_ANALOG_CH2_PIN2_CFG);
    pinmux_config(3, PINMUX_ANALOG_CH3_PIN3_CFG);
    pinmux_config(4, PINMUX_ANALOG_CH4_PIN4_CFG);
    //pinmux_config(5, PINMUX_ANALOG_CH5_PIN5_CFG);
    //pinmux_config(6, PINMUX_ANALOG_CH6_PIN6_CFG);
    pinmux_config(7, PINMUX_ANALOG_CH7_PIN7_CFG);
    #else //HS6621 A2 later or HS6621C
    pinmux_config(2, PINMUX_ANALOG_CH2_PIN2_CFG);
    pinmux_config(3, PINMUX_ANALOG_CH3_PIN3_CFG);
    pinmux_config(7, PINMUX_ANALOG_CH7_PIN7_CFG);
    pinmux_config(8, PINMUX_ANALOG_CH0_PIN8_CFG);
    pinmux_config(9, PINMUX_ANALOG_CH1_PIN9_CFG);
    pinmux_config(10, PINMUX_ANALOG_CH4_PIN10_CFG);
    pinmux_config(11, PINMUX_ANALOG_CH5_PIN11_CFG);
    pinmux_config(12, PINMUX_ANALOG_CH6_PIN12_CFG);
    #endif
    
    if (argc == 0) {
print_help:
        log_debug(
                "Usage: adc [options]...\r\n"
                "    cal:                               start calibrate.(first calibrate)\r\n"
                "    temp:                            Get current temperature in degree Celsius. 10 times\r\n"
                "    batt:                              Get current battery. 10 times\r\n"
                "    batt pin:                        Get current battery by single-ended. 10 times\r\n"
                "    batt pin pin:                  Get current battery by differential chanel. 10 times\r\n"
                "    batt dma                       Get current battery by GPIO2 with dma\r\n"
                "    batt dma debug            dma debug(FPGA)\r\n"
                "    auto                              auto test all extern channel, get current battery and temperature \r\n"
                "                                         GPIO8(CHO0)  connect 1.5v,  GPIO9(CHO1)   connect 1.8v, \r\n"
                "                                         GPIO2(CHO2)  connect GND, GPIO3(CHO3)   connect 1V, \r\n"
                "                                         GPIO10(CHO4) connect 1.8v,  GPIO11(CHO5) connect 3.3V, \r\n"
                "                                         GPIO12(CHO6) connect 3.3v,  GPIO7(CHO7)   connect 1.5v, \r\n"
                "\r\n"
                "Note:\r\n"
                "    1.when calibrate, GPIO2 connect GND, GPIO3 connect 1v, and vbat<=3.6v\r\n"
                "    2. Temperature data unit:\r\n"
                "       The unit is 10 degree Celsius.\r\n"
                "    3. Channels:\r\n"
                "         ADC_CHANNEL_CHIP_TEMPERATURE   = 0, \r\n"
                "         ADC_CHANNEL_CHIP_BATTERY       = 1, \r\n"
                "         ADC_CHANNEL_CHIP_VCM               = 2, \r\n"
                "         ADC_CHANNEL_EXTERN_CH0          = 3, CH0 INPUT mode. \r\n"
                "         ADC_CHANNEL_EXTERN_CH1          = 4, CH1 INPUT mode. \r\n"
                "         ADC_CHANNEL_EXTERN_CH2          = 5, CH2 INPUT mode. \r\n"
                "         ADC_CHANNEL_EXTERN_CH3          = 6, CH3 INPUT mode. \r\n"
                "         ADC_CHANNEL_EXTERN_CH4          = 7, CH4 INPUT mode. \r\n"
                "         ADC_CHANNEL_EXTERN_CH5          = 8, CH5 INPUT mode. \r\n"
                "         ADC_CHANNEL_EXTERN_CH6          = 9, CH6 INPUT mode. \r\n"
                "         ADC_CHANNEL_EXTERN_CH7          = 10, CH7 INPUT mode. \r\n"
                "         ADC_CHANNEL_VIP_CHG                 = 11, VIP_CHG INPUT mode. \r\n"
                "         ADC_CHANNEL_VCC_VBAT_CHG     = 12, VCC_VBAT_CHG INPUT mode. \r\n"
                "\r\n"
                "Example:      \r\n"
                "    adc cal     \r\n"
                "    adc temp  \r\n"
                "    adc batt   \r\n"
                "    adc batt 5 \r\n"
                "    adc batt 7 10\r\n"
                "    adc dma   \r\n"
                "    adc dma debug\r\n"
                "    adc auto\r\n"
                );

        return;
    }
    else if (strcmp(argv[0], "cal") == 0) {
        adc_cal_table_t table;
        log_debug("calibrate start\n");
        adc_calibrate(&table);
        adc_set_calibarate_param(&table);
        log_debug("calibrate stop\n");
        {
            // store calibrate date to flash
            #if 0
            #define ADC_FLASH_OFFSET   (1000*1024)
            #pragma pack(4)
            uint32_t rd_data[sizeof(adc_cal_table_t)];
            uint32_t wr_data[sizeof(adc_cal_table_t)];
            #pragma pack()
            sf_config_t sf_cfg = {
                .freq_hz = 6000000,
                .width = SF_WIDTH_1LINE,
                .delay = 0
            };

            memset(rd_data, 0, sizeof(rd_data));
            memset(wr_data, 0, sizeof(wr_data));
            memcpy(wr_data, &table, sizeof(adc_cal_table_t));
            sf_enable(HS_SF, 0);
            sf_config(HS_SF, 0, &sf_cfg);
            sf_detect(HS_SF, 0);
            sf_write(HS_SF, 0, ADC_FLASH_OFFSET, wr_data, sizeof(wr_data));
            sf_read(HS_SF, 0, ADC_FLASH_OFFSET, rd_data, sizeof(rd_data));
            if(0 != memcmp(wr_data, rd_data, sizeof(rd_data)))
            {
                log_debug("write calib data in flash error\n");
                co_assert(0);
            }
            #endif
        }
    }
    else if (strcmp(argv[0], "temp") == 0) {
        int16_t ret = adc_temperature_read(NULL);
        log_debug("temperature = %d\n", ret);
    }
    else if (strcmp(argv[0], "batt") == 0) {
        if (argc == 1) {
            int16_t ret = adc_battery_voltage_read(NULL);
            log_debug("battery = %d\n", ret);
        }
        else if (argc ==2) {
            uint8_t pin = strtol(argv[1], NULL, 10);
            int16_t ret = adc_battery_voltage_read_by_single_pin((adc_channel_t)pin, ADC_PGA_GAIN_0P25, NULL, 100);
            log_debug("battery = %d\n", ret);
        }
        else if (argc ==3) {
            uint8_t pin1 = strtol(argv[1], NULL, 10);
            uint8_t pin2 = strtol(argv[2], NULL, 10);
            int16_t ret = adc_battery_voltage_read_by_dif_pin((adc_channel_t)pin1, 
                                                            (adc_channel_t)pin2,
                                                            ADC_PGA_GAIN_0P25,
                                                            NULL,
                                                            100);
            log_debug("battery = %d\n", ret);
        }
        else {
            goto print_help;
        }
    }
    else if (strcmp(argv[0], "dma") == 0) {
        const adc_dma_config_t adc_dma_cfg = {
            .use_fifo       = false,
            .buffer         = adc_dma_buffer,
            .buffer_len     = ADC_DMA_BUFFER_SIZE*4,
            .block_llip     = adc_dma_block_llip,
            .block_num      = BLOCK_NUM,
            .callback       = NULL,
        };

        log_debug("adc dma test start\n");
        dma_fifo_init(&dma_fifo, adc_dma_buffer, ADC_DMA_BUFFER_SIZE*4);
        dma_init();

        adc_channel_config_t channel_config;
        adc_config_t config;
        uint32_t src_addr;
        adc_init();
        adc_start();
        
        config.trigger_mode   = ADC_TRIGGER_MODDE_SW;
        config.trigger_res    = ADC_TRIG_RES_CHANNEL;
        config.trigger_count  = 0;
        config.hw_trigger_sel = ADC_HW_TIMER0_0;
        adc_config(&config);

        channel_config.callback = (adc_callback_t)_adc_callback;
        channel_config.inp_gp   = ADC_CHANNEL_EXTERN_CH2;
        channel_config.inn_gp   = ADC_CHANNEL_CHIP_VCM;
        channel_config.clk_sel  = ADC_CLK_SEL_2MHZ;
        //channel_config.count    = 0;
        channel_config.gtune_gp = ADC_GTUNE_GP_0P5;
        channel_config.scal_gp  = ADC_SCAL_GP_ENABLE;
        channel_config.vcm_gp   = ADC_VCM_SEL_000mV;
        adc_add_channel(&channel_config);
        
        if (argc ==2 && strcmp(argv[1], "debug") == 0) {
            /// debug for dma on FPGA
            register_set(&HS_GPADC->ADC_CFG1, MASK_1REG(GPADC_SW_DBG,  1));
            register_set(&HS_GPADC->CMPN_CTRL, MASK_1REG(GPADC_COMPEN_EN, 0));
        }

        log_debug("config1=%x\n", HS_GPADC->ADC_CFG1);
        log_debug("reg0=%x\n", HS_GPADC->CH_0_CFG);
        log_debug("reg1=%x\n", HS_GPADC->CH_1_CFG);
        log_debug("reg2=%x\n", HS_GPADC->CH_2_CFG);
        log_debug("reg3=%x\n", HS_GPADC->CH_3_CFG);
        log_debug("reg4=%x\n", HS_GPADC->CH_4_CFG);
        /// the result value is (config*8-1)&0x0FFF
        log_debug("reg5=%x,%x\n", HS_GPADC->CH_5_CFG,(HS_GPADC->CH_5_CFG*8-1)&0x0FFF);
        log_debug("reg6=%x\n", HS_GPADC->CH_6_CFG);
        log_debug("reg7=%x\n", HS_GPADC->CH_7_CFG);
        log_debug("reg8=%x\n", HS_GPADC->CH_8_CFG);
        log_debug("reg9=%x\n", HS_GPADC->CH_9_CFG);
        log_debug("rega=%x\n", HS_GPADC->CH_A_CFG);
        log_debug("regb=%x\n", HS_GPADC->CH_B_CFG);
        log_debug("regc=%x\n", HS_GPADC->CH_C_CFG);
        
        adc_dma = adc_dma_config((HS_DMA_CH_Type *)NULL, &adc_dma_cfg, &src_addr);
        dma_start(adc_dma, src_addr, (uint32_t)adc_dma_buffer, ADC_DMA_BUFFER_SIZE);
        adc_start_convert();
        
        dma_wait_stop(adc_dma);
        
        adc_stop_convert();
        adc_stop();
        dma_stop(adc_dma);
        int32_t sum1 = 0;

        log_debug("dma data:\n");
        for(uint32_t i = 0; i < ADC_DMA_BUFFER_SIZE; ++i)
        {
            sum1 += (int16_t)adc_dma_buffer[i];
            if((i>0)&&(0 == (i%8)))
            {
                log_debug("\n");
            }
            log_debug("%08x ", adc_dma_buffer[i]);
        }
        log_debug("\n");

        sum1 = sum1/ADC_DMA_BUFFER_SIZE;
        log_debug("ch:5, %08x, %f\n", sum1, (float)(sum1)*0.8/2048.0);

        log_debug("adc_dma_buffer addr: %08x\n", adc_dma_buffer);
        
    }
    else if(strcmp(argv[0], "auto") == 0) {
        // 1. calibrate 
        adc_cal_table_t table;
        log_debug("calibrate start\n");
        adc_calibrate(&table);
        adc_set_calibarate_param(&table);
        log_debug("calibrate stop\n");

        log_debug("GPIO8(CHO0)  connect 1.5v,  GPIO9(CHO1)   connect 1.8v, \r\n"
                                " GPIO2(CHO2)  connect GND, GPIO3(CHO3)   connect 1V, \r\n"
                                " GPIO10(CHO4) connect 1.8v,  GPIO11(CHO5) connect 3.3V, \r\n"
                                " GPIO12(CHO6) connect 3.3v,  GPIO7(CHO7)   connect 1.5v, \r\n");

        // 2. get temperature
        int16_t ret = adc_temperature_read(NULL);
        log_debug("temperature=%d\n", ret);
        // 3. get battery
        ret = adc_battery_voltage_read(NULL);
        log_debug("battery=%d\n", ret);

        for(adc_pga_gain_t j=ADC_PGA_GAIN_0P125;j<=ADC_PGA_GAIN_4;j++)
        {
            ret = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH2, j, NULL, 10);
            log_debug("battery ch2(2),gain=%d,=%d\n", j-ADC_PGA_GAIN_0P125, ret);

            ret = adc_battery_voltage_read_by_dif_pin(ADC_CHANNEL_EXTERN_CH1, 
                                                  ADC_CHANNEL_EXTERN_CH0,
                                                  j,
                                                  NULL,
                                                  100);
            log_debug("battery diff(ch1(9)-ch0(8)= %d\n", ret);
        }

        for(adc_pga_gain_t j=ADC_PGA_GAIN_0P125;j<=ADC_PGA_GAIN_0P5;j++)
        {
            ret = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH0, j, NULL, 10);
            log_debug("battery ch0(8),gain=%d,=%d\n", j-ADC_PGA_GAIN_0P125, ret);
            ret = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH3, j, NULL, 10);
            log_debug("battery ch3(3),gain=%d,=%d\n", j-ADC_PGA_GAIN_0P125, ret);    
            ret = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH7, j, NULL, 10);
            log_debug("battery ch7(7),gain=%d,=%d\n", j-ADC_PGA_GAIN_0P125, ret);

            ret = adc_battery_voltage_read_by_dif_pin(ADC_CHANNEL_EXTERN_CH5, 
                                                  ADC_CHANNEL_EXTERN_CH4,
                                                  j,
                                                  NULL,
                                                  100);
             log_debug("battery diff(ch5(11)-ch4(10)= %d\n", ret);
        }
        for(adc_pga_gain_t j=ADC_PGA_GAIN_0P125;j<=ADC_PGA_GAIN_0P25;j++)
        {
            ret = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH1, j, NULL, 10);
            log_debug("battery ch1(9),gain=%d,=%d\n", j-ADC_PGA_GAIN_0P125, ret);
            ret = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH4, j, NULL, 10);
            log_debug("battery ch4(10),gain=%d,=%d\n", j-ADC_PGA_GAIN_0P125, ret);
        }

        ret = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH5, ADC_PGA_GAIN_0P125, NULL, 10);
        log_debug("battery ch5(11),gain=ADC_PGA_GAIN_0P125,=%d\n", ret);
        ret = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH6, ADC_PGA_GAIN_0P125, NULL, 10);
        log_debug("battery ch6(12),gain=ADC_PGA_GAIN_0P125,=%d\n", ret);
        
        // get temperature
        ret = adc_temperature_read(NULL);
        log_debug("temperature=%d\n", ret);
        // get battery
        ret = adc_battery_voltage_read(NULL);
        log_debug("battery=%d\n", ret);
    }
    else {
        goto print_help;
    }
}
#endif
#endif //(CONFIG_APP_SHELL)

/// @} APP

