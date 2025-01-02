/**
 * @file calib_repair.h
 * @brief 
 * @date Fri, Mar 16, 2018  6:27:24 PM
 * @author liqiang
 *
 * @defgroup 
 * @ingroup 
 * @brief 
 * @details 
 *
 * @{
 */

#ifndef __CALIB_REPAIR_H__
#define __CALIB_REPAIR_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "features.h"
#include "peripheral.h"
#include "co.h"


/*********************************************************************
 * MACROS
 */
// HS6621P / HS6621CB
#if defined(CONFIG_HS6621P) || (defined(CONFIG_HS6621C) && (CONFIG_HARDWARE_VERSION >= HARDWARE_VERSION_B(1)))
#define CALIB_REPAIR_DCDC_V_NORMAL_T        12
#define CALIB_REPAIR_DCDC_V_HIGH_T          14
#define CALIB_REPAIR_DCDC_V_HIGH_TX_PWR     16
#define CALIB_REPAIR_DCDC_V_MAX             31
#else
#define CALIB_REPAIR_DCDC_V_NORMAL_T        4
#define CALIB_REPAIR_DCDC_V_HIGH_T          6
#define CALIB_REPAIR_DCDC_V_HIGH_TX_PWR     7
#define CALIB_REPAIR_DCDC_V_MAX             7
#endif
#define CALIB_REPAIR_LDO_V_NORMAL_T         0
#define CALIB_REPAIR_LDO_V_HIGH_T           2
#define CALIB_REPAIR_LDO_V_HIGH_TX_PWR      3
#define CALIB_REPAIR_LDO_V_MAX              3

/*********************************************************************
 * TYPEDEFS
 */
typedef struct
{
    // Fix RF bug: temperature
    int16_t temperature;

    // rf and rc temperature repiar
    uint32_t rc_rf_repair_delay_s;
    uint32_t rc_rf_repair_time;
    int16_t  rc_repair_temperature;
    int16_t  rf_repair_temperature;

    // rc32k temperature repair
    uint32_t rc32k_repair_delay_ms;
    uint32_t rc32k_repair_time;

    // xtal32m
    uint32_t xtal32m_cap;

    // sys voltage
    int8_t delta_dvdd_1p0;
    int8_t delta_vdd_1p2;
    int8_t delta_dcdc_1p27;
    int8_t delta_vcharge_1p0;
    uint8_t bg_trim_vref_0p6;
    uint8_t pa_1p0; // [3]=mode_ldo_pa(always 0) [2]=v1p0_pa_vbat_h [1:0]=v1p0_pa_vbat
}calib_repair_t;


/*********************************************************************
 * EXTERN VARIABLES
 */
extern calib_repair_t calib_repair_env;

/*********************************************************************
 * EXTERN FUNCTIONS
 */

/**
 * @brief  calib repair rc rf init
 **/
void calib_repair_rc_rf_init(void);

/**
 * @brief  calib repair rc rf temperature check
 *
 * @param[in] allow_repair_delay_ms  allow repair delay ms
 *
 * @return repaired
 **/
bool calib_repair_rc_rf_temperature_check(uint32_t allow_repair_delay_ms);

/**
 * @brief  calib repair rc32k temperature check
 *
 * @param[in] allow_repair_delay_ms  allow repair delay ms
 *
 * @return repaired
 **/
bool calib_repair_rc32k_temperature_check(uint32_t allow_repair_delay_ms);

/**
 * @brief  calib repiar rc rf temperature delay set
 *
 * @param[in] delay_ms  delay ms
 **/
void calib_repiar_rc_rf_temperature_delay_set(uint32_t delay_ms);

/**
 * @brief set repair_rc32k_temperature_delay
 *
 * @param[in] delay_ms  
 *
 * @return None
 **/
void calib_repair_rc32k_temperature_delay_set(uint32_t delay_ms);

/**
 * @brief  calib repiar sys voltage set
 *
 * @param[in] dvdd_1p0  dvdd 1p0
 * @param[in] dcdc_1p27  dcdc 1p27
 * @param[in] vcharge_1p0  vcharge 1p0
 * @param[in] bg_trim_vref  bg trim vref
 * @param[in] pa_1p0  pa 1p0
 **/
void calib_repiar_sys_voltage_set(float dvdd_1p0, float dcdc_1p27, float vcharge_1p0, float bg_trim_vref, uint8_t pa_1p0);

/**
 * @brief  calib repair value select
 *
 * @param[in] calc  calc
 * @param[in] min  min
 * @param[in] max  max
 *
 * @return select
 **/
int calib_repair_value_select(int calc, int min, int max);

#ifdef __cplusplus
}
#endif

#endif

/** @} */

