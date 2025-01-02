/**
 * @file example_sf.c
 * @brief example for flash
 * @date 2017/07/12 17:53:27
 * @author liqiang
 *
 * @defgroup EXAMPLE_SF Flash Example
 * @ingroup EXAMPLE
 * @brief example for flash
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_sf(void)
{
    CO_ALIGN(8) uint8_t write_buffer[100];
    CO_ALIGN(8) uint8_t read_buffer[100];
    int i;

    for(i=0; i<100; ++i)
        write_buffer[i] = i;

    /*
     * Inside flash: sf=HS_SF, cs=0
     */

    // Enable flash
    sf_enable(HS_SF, 0);

    // Erase sector in 128K 
    sf_erase(HS_SF, 0, 128*1024, 4*1024);

    // Read 100 bytes in 128K, it should be all 0xFF
    sf_read(HS_SF, 0, 128*1024, read_buffer, 100);

    // Write 100 bytes to 128K
    sf_write(HS_SF, 0, 128*1024, write_buffer, 100);

    // Read 100 bytes in 128K, it should be same as write_buffer
    sf_read(HS_SF, 0, 128*1024, read_buffer, 100);

    /*
     * Outside flash: sf=HS_SF, cs=1
     */
    sf_config_t outside_sf_cfg = {
        .freq_hz = 1000000,
        .width = SF_WIDTH_2LINE,
        .delay = 5,
    };

    // pinmux (EVB)
    pinmux_config(21,  PINMUX_SFLASH0_CSN_CFG);
    pinmux_config(22,  PINMUX_SFLASH0_SO_CFG);
    pinmux_config(24, PINMUX_SFLASH0_CK_CFG);
    pinmux_config(25, PINMUX_SFLASH0_SI_CFG);

    // open
    sf_enable(HS_SF, 1);

    // config
    sf_config(HS_SF, 1, &outside_sf_cfg);

    // detect
    sf_detect(HS_SF, 1);

    // Erase sector in 128K 
    sf_erase(HS_SF, 1, 128*1024, 4*1024);

    // Read 100 bytes in 128K, it should be all 0xFF
    sf_read(HS_SF, 1, 128*1024, read_buffer, 100);

    // Write 100 bytes to 128K
    sf_write(HS_SF, 1, 128*1024, write_buffer, 100);

    // Read 100 bytes in 128K, it should be same as write_buffer
    sf_read(HS_SF, 1, 128*1024, read_buffer, 100);
}

/** @} */

