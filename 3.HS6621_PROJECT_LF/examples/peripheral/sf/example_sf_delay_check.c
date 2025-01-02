/**
 * @file example_sf.c
 * @brief example for flash
 * @date 2017/07/12 17:53:27
 * @author liqiang
 *
 * @defgroup EXAMPLE_SF Flash Example
 * @ingroup EXAMPLE
 * @brief example for flash to check 'delay' param, ONLY for OM6621P
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

/*********************************************************************
 * MACROS
 */

// xflash as flash memory
#define SFS_XFLASH_HD_PIN   20
#define SFS_XFLASH_CSN_PIN  21
#define SFS_XFLASH_MISO_PIN 22
#define SFS_XFLASH_WP_PIN   23
#define SFS_XFLASH_CLK_PIN  24
#define SFS_XFLASH_MOSI_PIN 25
#define SFS_XFLASH_PWR_PIN  15

/*********************************************************************
 * GLOBAL VARIABLES
 */

int iflash_delay_64m, iflash_delay_128m, xflash_delay_128m;

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_sf(void)
{
    sf_delay_check_info_t info;

    // inside flash delay with no pll
    info.pll_mhz = 0;
    info.sf_freq_hz = 2;
    info.sf_width = 2;
    info.sf_check_addr = 0;
    info.sf_check_addr_real_data = NULL;
    info.is_use_cpft_data = true;
    iflash_delay_64m = sf_delay_check(HS_SF, 0, &info);
    uart_open(HS_UART0, 115200, UART_FLOW_CTRL_DISABLED, NULL);
    log_debug("iflash_delay_64m=%d\n", iflash_delay_64m);

    // inside flash delay with 128MHz pll
    info.pll_mhz = 128;
    info.sf_freq_hz = 2;
    info.sf_width = 2;
    info.sf_check_addr = 0;
    info.sf_check_addr_real_data = NULL;
    info.is_use_cpft_data = true;
    iflash_delay_128m = sf_delay_check(HS_SF, 0, &info);
    pmu_pll_startup_and_sfs_config(128, 2, 2, iflash_delay_128m);
    uart_open(HS_UART0, 115200, UART_FLOW_CTRL_DISABLED, NULL);
    log_debug("iflash_delay_128m=%d\n", iflash_delay_128m);

    // outside flash delay with 128MHz pll
    pinmux_config(SFS_XFLASH_CSN_PIN,  PINMUX_SFLASH1_CSN_CFG);
    pinmux_config(SFS_XFLASH_MISO_PIN, PINMUX_SFLASH1_SO_CFG);
    pinmux_config(SFS_XFLASH_CLK_PIN,  PINMUX_SFLASH1_CK_CFG);
    pinmux_config(SFS_XFLASH_MOSI_PIN, PINMUX_SFLASH1_SI_CFG);
    sf_config_t outside_sf_cfg = {
        .freq_hz = 1000000,
        .width = SF_WIDTH_2LINE,
        .delay = 5,
    };
    sf_enable(HS_SF1, 0);
    sf_config(HS_SF1, 0, &outside_sf_cfg);

    info.pll_mhz = 112;
    info.sf_freq_hz = 2;
    info.sf_width = 2;
    info.sf_check_addr = 0x3000;
    info.sf_check_addr_real_data = (uint8_t *)"\x88\x68\x00\x20\x55\x30\x80\x00\xFF\xFF\xFF\xFF\x63\x30\x80\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x6B\x30\x80\x00";
    info.is_use_cpft_data = false;
    xflash_delay_128m = sf_delay_check(HS_SF1, 0, &info);

    uart_open(HS_UART0, 115200, UART_FLOW_CTRL_DISABLED, NULL);
    log_debug("xflash_delay_128m=%d\n", xflash_delay_128m);
    sf_detect(HS_SF1, 0);
    log_debug("xflash ID: %06X\n", sf_id(HS_SF1, 0));
}

/** @} */

