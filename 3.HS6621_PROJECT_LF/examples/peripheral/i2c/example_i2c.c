/**
 * @file example_i2c.c
 * @brief example for i2c
 * @date 2017/07/12 16:17:49
 * @author liqiang
 *
 * @defgroup EXAMPLE_I2C I2C Example
 * @ingroup EXAMPLE
 * @brief example for i2c
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

#define I2C_USE_GPIO_SIM
/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_i2c(void)
{
#ifdef I2C_USE_GPIO_SIM
    uint8_t acceleration_data=0, heartrate_data=0;
    bool acceleration_result, heartrate_result;

    pinmux_config(9, PINMUX_GPIO_MODE_CFG);
    pinmux_config(10, PINMUX_GPIO_MODE_CFG);
    pinmux_config(26, PINMUX_GPIO_MODE_CFG);
    pinmux_config(24, PINMUX_GPIO_MODE_CFG);

    pmu_pin_mode_set((1<<9)|(1<<10)|(1<<26)|(1<<24), PMU_PIN_MODE_OD);

    const i2c_sim_config_t i2c_acceleration_config = { 26, 24, 1};
    const i2c_sim_config_t i2c_heartrate_config = { 9,  10, 1};

    i2c_sim_config(&i2c_acceleration_config);
    i2c_sim_config(&i2c_heartrate_config);

    acceleration_result = i2c_sim_master_read_mem(&i2c_acceleration_config, 0x27, 0x01, 1, &acceleration_data, 1);
    heartrate_result = i2c_sim_master_read_mem(&i2c_heartrate_config, 0x24, 0x00, 1, &heartrate_data, 1);
#else
    uint8_t txbuf[100] = {0};
    uint8_t rxbuf[100] = {0};

    for(i=0;i<100;i++)
        txbuf[i]=i;

    pinmux_config(11, PINMUX_I2C_MST_SCK_CFG);
    pinmux_config(18, PINMUX_I2C_MST_SDA_CFG);

    pmu_pin_mode_set((1<<11)|(1<<18), PMU_PIN_MODE_OD);

    i2c_open(I2C_MODE_MASTER, 100000);

    co_verify(i2c_master_write_mem(0x50, 0x10, 2, txbuf, 30));
    co_delay_ms(10);
    co_verify(i2c_master_read_mem(0x50, 0x10, 2, rxbuf, 30));
#endif

    while(1);
}
/** @} */

