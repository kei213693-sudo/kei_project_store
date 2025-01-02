/**
 * @file cpft.c
 * @brief 
 * @date Fri 29 Sep 2017 05:29:25 PM CST
 * @author liqiang
 *
 * @addtogroup 
 * @ingroup 
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"
#include "co.h"
#include "cpft.h"
#include "mbr.h"

/*********************************************************************
 * MACROS
 */
#define CPFT_MAGIC_CODE 0x54465043

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */


/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 * @brief  cpft data get
 *
 * @param[in] cpft  cpft
 *
 * @return ok ?
 **/
bool cpft_data_get(cpft_data_t *cpft)
{
    uint32_t cpft_addr, cpft_len;
    uint16_t crc16;
    int err;

    sfs_enable();

    err = mbr_get_cpft(&cpft_addr, &cpft_len);
    if(err)
        return false;

    sfs_read(cpft_addr, cpft, sizeof(cpft_data_t));

    if(cpft->magic_code != CPFT_MAGIC_CODE)
        return false;

    if(cpft->length != sizeof(cpft_data_t)-6)
        return false;

    crc16 = co_crc16_ccitt(0, &cpft->length, cpft->length);
    if(crc16 != cpft->crc16)
        return false;

    return true;
}

/**
 * @brief  cpft data setup
 *
 * @param[in] cpft  cpft
 **/
void cpft_data_setup(cpft_data_t *cpft)
{
    float dvdd_1p0 = 1.0;
    float dcdc_1p27 = 1.27;
    float vcharge_1p0 = 1.0;
    float bg_trim_vref = 0.6;
    uint8_t pa_1p0 = (0<<3) | (0<<2) | (2<<0);  // bitmap6 [3]=mode_ldo_pa(always 0) [2]=v1p0_pa_vbat_h [1:0]=v1p0_pa_vbat

    if(cpft->bitmap & CPFT_ADC_CALIB_VALID_MASK)
        adc_set_calibarate_param(&cpft->adc_calib);

    if(cpft->bitmap & CPFT_DVDD_1P0_VALID_MASK)
        dvdd_1p0 = cpft->dvdd_1p0;

    if(cpft->bitmap & CPFT_DCDC_1P27_VALID_MASK)
        dcdc_1p27 = cpft->dcdc_1p27;

    if(cpft->bitmap & CPFT_VCHARGE_1P0_VALID_MASK)
        vcharge_1p0 = cpft->vcharge_1p0;

    if(cpft->bitmap & CPFT_PA_1P0_VALID_MASK)
        pa_1p0 = cpft->pa_1p0;

    if(cpft->bitmap & CPFT_ADC_TEMP_CALIB_NEW_VALID_MASK)
        adc_set_temperature_new_param(cpft->adc_temp_calib_new);

    if(cpft->bitmap & CPFT_BG_TRIM_VREF_VALID_MASK)
        bg_trim_vref = cpft->bg_trim_vref;

    calib_repiar_sys_voltage_set(dvdd_1p0, dcdc_1p27, vcharge_1p0, bg_trim_vref, pa_1p0);

#ifndef CONFIG_HS6621
    if(cpft->bitmap & CPFT_AU_SEL_BIAS_VALID_MASK)
        audio_set_calibarate_param(cpft->au_sel_bias);
#endif
}

/**
 * @brief  cpft setup
 **/
void cpft_setup(void)
{
    cpft_data_t cpft;
    bool ok;

    ok = cpft_data_get(&cpft);
    if(ok)
        cpft_data_setup(&cpft);
}

/** @} */


