/**
 * @file peripheral.c
 * @brief 
 * @date Fri 01 Dec 2017 09:06:10 AM CST
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

/*********************************************************************
 * MACROS
 */


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
 * @brief peripheral lowpower store
 *
 * @return None
 **/
void peripheral_lowpower_enter(co_power_status_t power_status)
{
    cpm_store();
    gpio_store();
    rtc_store();

    // Gate not sleep module.
    // Avoid IE_CTRL generatting unexpect interrupt
    register_set1(&HS_PSO->TIM_CFG[0], CPM_GATE_EN_MASK);
    register_set1(&HS_PSO->TIM_CFG[1], CPM_GATE_EN_MASK);
    register_set1(&HS_PSO->TIM_CFG[2], CPM_GATE_EN_MASK);
    register_set1(&HS_PSO->UART0_CFG,  CPM_GATE_EN_MASK);
    register_set1(&HS_PSO->UART1_CFG,  CPM_GATE_EN_MASK);
    register_set1(&HS_PSO->I2C_CFG,    CPM_GATE_EN_MASK);
    register_set1(&HS_PSO->SPI0_CFG,   CPM_GATE_EN_MASK);
    register_set1(&HS_PSO->SPI1_CFG,   CPM_GATE_EN_MASK);
    register_set1(&HS_PSO->I2S_CFG,    CPM_GATE_EN_MASK);
    register_set1(&HS_PSO->QDEC_CFG,   CPM_GATE_EN_MASK);
    HS_PSO_UPD_RDY();

    sfs_lowpower_enter(power_status);
}

/**
 * @brief peripheral lowpower restore
 *
 * @return None
 **/
void peripheral_lowpower_leave(co_power_status_t power_status)
{
    cpm_restore();
    pinmux_restore();
    rtc_restore();
    encoder_restore();

#ifdef CONFIG_BB_SLEEP_FLOW_ATTACH_TO_SYSTEM
    // sleep mode move to co_pmu_lowpower_parallel_leave_handle()
    if (power_status == POWER_DEEP_SLEEP || !co_power_bb_sleep_flow_attach_to_system_mode_is_enabled())
#endif
    {
        gpio_restore();
        sfs_lowpower_leave(power_status);
    }
}

/** @} */


