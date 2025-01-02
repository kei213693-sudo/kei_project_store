/**
 * @file example_psram.c
 * @brief example for psram
 * @date 2017/07/12 17:53:27
 * @author tangtanglin
 *
 * @defgroup EXAMPLE_PSRAM PSRAM Example
 * @ingroup EXAMPLE
 * @brief example for psram
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"
#include "flash_outside.h"
/*********************************************************************
 * MACROS
 */
#define PICTURE_START_ADDR_OFF_IN_PSRAM    0x0000
#define PSRAM_SF                           HS_SF3

#define PIN_PSRAM_CS               42  //PB5
#define PIN_PSRAM_SO               41  //PB6
#define PIN_PSRAM_CK               39  //PB8
#define PIN_PSRAM_SI               38  //PB9
#define PIN_PSRAM_HD               37  //PB4
#define PIN_PSRAM_WP               40  //PB7

#define PSRAM_ID        0x000D5D1B
#define PSRAM_ID_2      0x000D5D1A

#define PICTURE_START_ADDR_OFF_IN_FLASH    0x0
#define PICTURE_1_START_ADDR_OFF_IN_FLASH  PICTURE_START_ADDR_OFF_IN_FLASH
#define PICTURE_2_START_ADDR_OFF_IN_FLASH  (PICTURE_1_START_ADDR_OFF_IN_FLASH+454*454*2)
#define PICTURE_3_START_ADDR_OFF_IN_FLASH  (PICTURE_2_START_ADDR_OFF_IN_FLASH+454*454*2)



#define PIN_SF_CS               21
#define PIN_SF_SO               22
#define PIN_SF_CK               24
#define PIN_SF_SI               25
#define PIN_SF_HD               20
#define PIN_SF_WP               23

#define FLASH_ID_OUTSIDE        0x00856014
#define FLASH_ID_2_OUTSIDE      0x00EB6014

#define FLASH_ID_INSIDE         0x00856013
#define FLASH_ID_2_INSIDE       0x00856015
#define FLASH_ID_3_INSIDE       0x00856014
#define FLASH_OUT_SIZE_RGB565   454*454*2

#define getreg8(a)           (*(volatile uint8_t *)(a))

/*********************************************************************
 * VARIABLES
 */

//sf_config_t outside_cfg = {64000000, SF_WIDTH_2LINE, 5};
sf_config_t outside_cfg = {64000000, SF_WIDTH_4LINE, 5};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void flash_out_init()
{
    pmu_pin_mode_set(BITMASK(PIN_SF_CS), PMU_PIN_MODE_PU);
    pmu_pin_mode_set(BITMASK(PIN_SF_SO), PMU_PIN_MODE_PD);

    pinmux_config(PIN_SF_CS, PINMUX_SFLASH1_CSN_CFG);
    pinmux_config(PIN_SF_SO, PINMUX_SFLASH1_SO_CFG);
    pinmux_config(PIN_SF_CK, PINMUX_SFLASH1_CK_CFG);
    pinmux_config(PIN_SF_SI, PINMUX_SFLASH1_SI_CFG);

    pinmux_config(PIN_SF_HD, PINMUX_SFLASH1_HD_CFG);
    pinmux_config(PIN_SF_WP, PINMUX_SFLASH1_WP_CFG);

    // open
    sf_enable(HS_SF1, 0);

    // config
    sf_config(HS_SF1, 0, &outside_cfg);
    sf_quad_enable(HS_SF1, 0, true);
    // Flash ID
    log_debug("SF1 OUT flash ID: %08X\n", sf_read_id(HS_SF1, 0));
}
