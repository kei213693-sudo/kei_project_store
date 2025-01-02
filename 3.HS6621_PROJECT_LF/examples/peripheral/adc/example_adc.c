/**
 * @file example_adc.c
 * @brief example for ADC
 * @date 2017/07/12 14:58:06
 * @author liqiang
 *
 * @defgroup EXAMPLE_ADC ADC Example
 * @ingroup EXAMPLE
 * @brief example for ADC
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "co.h"
#include "peripheral.h"

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_adc(void)
{
    uint16_t ret = 0;
    pinmux_config(7, PINMUX_ANALOG_CH7_PIN7_CFG);
    pinmux_config(8, PINMUX_ANALOG_CH0_PIN8_CFG);
    pinmux_config(9, PINMUX_ANALOG_CH1_PIN9_CFG);

    while(1)
    {
        log_debug("battery = %d\n", adc_battery_voltage_read(NULL));
        ret = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH7, ADC_PGA_GAIN_0P125, NULL, 10);
        log_debug("single battery = %d\n", ret);
        ret = adc_battery_voltage_read_by_dif_pin(ADC_CHANNEL_EXTERN_CH0, 
                                              ADC_CHANNEL_EXTERN_CH1,
                                              ADC_PGA_GAIN_0P125,
                                              NULL,
                                              10);
        log_debug("diff battery = %d\n", ret);
        
        // use API get voltage
        adc_channel_config_t channel_config;
        adc_config_t config;

        // 1. init adc
        adc_init();
        // 2. start adc
        adc_start();
        
        // 3. config the adc
        config.trigger_mode   = ADC_TRIGGER_MODDE_SW;
        config.trigger_res    = ADC_TRIG_RES_CHANNEL;
        config.trigger_count  = 0;
        config.hw_trigger_sel = ADC_HW_TIMER0_0;
        adc_config(&config);

        // 4. config adc chnnel
        channel_config.callback = NULL;
        channel_config.inp_gp   = ADC_CHANNEL_CHIP_BATTERY;
        channel_config.inn_gp   = ADC_CHANNEL_CHIP_VCM;
        channel_config.clk_sel  = ADC_CLK_SEL_2MHZ;
        channel_config.gtune_gp = ADC_GTUNE_GP_0P5;
        channel_config.scal_gp  = ADC_SCAL_GP_DISABLE;
        channel_config.vcm_gp   = ADC_VCM_SEL_000mV;
        adc_add_channel(&channel_config);
        
       // start adc convert
        adc_start_convert();

        int32_t sum = 0;
        for(int32_t i = 0; i < 10; i++)
        {
            // read channel 
            sum += adc_read_data_block(ADC_CHANNEL_CHIP_BATTERY, 1);
        }
        
        // stop adc convert
        adc_stop_convert();

        // delete channel
        adc_del_channel(ADC_CHANNEL_CHIP_BATTERY);
        // stop adc
        adc_stop();

        sum = sum/10;

        ret = adc_convert_channel_data(ADC_CHANNEL_CHIP_BATTERY, sum);
        log_debug("battery = %d\n", ret);
        
        co_delay_ms(1000);
    }
}

/** @} */


