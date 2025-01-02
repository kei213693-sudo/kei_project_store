/**
 * @file timer_pwm_input.c
 * @brief 
 * @date Sat 24 Dec 2022 07:32:23 PM CST
 * @author Onmicro
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
#define TIM_STRUCT2INDEX(tim)   ((((uint32_t)tim) - HS_TIM0_BASE) / (HS_TIM1_BASE - HS_TIM0_BASE))
#define TIM_INDEX2STRUCT(i)     ((HS_TIM_Type *)(HS_TIM0_BASE + i * (HS_TIM1_BASE - HS_TIM0_BASE)))

// TIMER control register1 CR1
#define TIMER_CR1_CEN_MASK                 (1U << 0)                             ///< Counter Enable
#define TIMER_CR1_CEN                      (1U << 0)
#define TIMER_CR1_UDIS_MASK                (1U << 1)                             ///< Update disable
#define TIMER_CR1_UDIS                     (1U << 1)
#define TIMER_CR1_URS_MASK                 (1U << 2)                             ///< Update request source
#define TIMER_CR1_URS_INT_DMA              (1U << 2)                             ///< only counter overflow/underflow generates an update interupt or DMa request if enabled.
#define TIMER_CR1_OPM_MASK                 (1U << 3)                             ///< one pulse mode
#define TIMER_CR1_OPM                      (1U << 3)
#define TIMER_CR1_DIR_MASK                 (1U << 4)                             ///< direction
#define TIMER_CR1_DIR_DOWN_COUNTER         (1U << 4)
#define TIMER_CR1_DIR_UP_COUNTER           (0U << 4)
#define TIMER_CR1_CMS_MASK                 (3U << 5)                             ///< Center-aligned mdoe selection
#define TIMER_CR1_CMS_EDGE_ALIGNED         (0U << 5)                             ///< Edge-aligned mode
#define TIMER_CR1_CMS_CENTER_ALIGNED_1     (1U << 5)                             ///< Center-aligned mode 1
#define TIMER_CR1_CMS_CENTER_ALIGNED_2     (2U << 5)                             ///< Center-aligned mode 2
#define TIMER_CR1_CMS_CENTER_ALIGNED_3     (3U << 5)                             ///< Center-aligned mode 3
#define TIMER_CR1_APRE_MASK                (1U << 7)                             ///< Auto-reload preload enable mask
#define TIMER_CR1_APRE                     (1U << 7)
#define TIMER_CR1_CKD_MASK                 (3U << 8)                             ///< Clock division mask
#define TIMER_CR1_CKD_0                    (0U << 8)                             ///< tDTS = tCK_INT
#define TIMER_CR1_CKD_2                    (1U << 8)                             ///< tDTS = 2 * tCK_INT
#define TIMER_CR1_CKD_4                    (2U << 8)                             ///< tDTS = 4 * tCK_INT

// TIMER control register2 CR2
#define TIMER_CR2_CCPC_MASK                (1U << 0)                             ///< Capture/Compare preload control
#define TIMER_CR2_CCPC_ENABLE              (1U << 0)                             ///< CCxE, CCxNE and CCxM are preloaded
#define TIMER_CR2_CCUS_MASK                (1U << 2)                             ///< Capture/Compare control update selection
#define TIMER_CR2_CCDS_MASK                (1U << 3)                             ///< Capture/Compare DMA selection mask
#define TIMER_CR2_CCDS_CCx_EVENT_OCCUR     (0U << 3)                             ///< CCx DMA request sent when CCx event occurs
#define TIMER_CR2_CCDS_UPDATE_EVENT_OCCUR  (1U << 3)                             ///< CCx DMA request sent when update event occurs
#define TIMER_CR2_MMS_MASK                 (7U << 4)                             ///< Master mode selection
#define TIMER_CR2_TI0S_MASK                (1U << 7)                             ///< TI1 selection
#define TIMER_CR2_OISn_MASK(n)             (1U << (8 + (n)*2))
#define TIMER_CR2_OISn_LOW(n)              (0U << (8 + (n)*2))
#define TIMER_CR2_OISn_HIGH(n)             (1U << (8 + (n)*2))
#define TIMER_CR2_OISnN_MASK(n)            (1U << (9 + (n)*2))                   ///< Output Idle state 1 (OC1N output)
#define TIMER_CR2_OISnN_LOW(n)             (0U << (9 + (n)*2))
#define TIMER_CR2_OISnN_HIGH(n)            (1U << (9 + (n)*2))

// Timer slave mode control register SMCR
#define TIMER_SMCR_SMS_MASK                (7U << 0)                             ///< Slave mode selection
#define TIMER_SMCR_SMS_DIS                 (0U << 0)                             ///< Slave mode disabled
#define TIMER_SMCR_SMS_ENCODER_MODE_1      (1U << 0)                             ///< Encoder mode 1
#define TIMER_SMCR_SMS_ENCODER_MODE_2      (2U << 0)                             ///< Encoder mode 2
#define TIMER_SMCR_SMS_ENCODER_MODE_3      (3U << 0)                             ///< Encoder mode 3
#define TIMER_SMCR_SMS_RESET_MODE          (4U << 0)                             ///< Reset mode
#define TIMER_SMCR_SMS_GATED_MODE          (5U << 0)                             ///< Gated mode
#define TIMER_SMCR_SMS_TRIGER_MODE         (6U << 0)                             ///< trigger mode
#define TIMER_SMCR_SMS_EXT_CLOCK_MODE      (7U << 0)                             ///< external clock mode
#define TIMER_SMCR_TS_MASK                 (7U << 4)                             ///< Trigger selection
#define TIMER_SMCR_ITR0                    (0U << 4)
#define TIMER_SMCR_ITR1                    (1U << 4)
#define TIMER_SMCR_ITR2                    (2U << 4)
#define TIMER_SMCR_ITR3                    (3U << 4)
#define TIMER_SMCR_TI1F_ED                 (4U << 4)
#define TIMER_SMCR_TI1FP1                  (5U << 4)
#define TIMER_SMCR_TI1FP2                  (6U << 4)
#define TIMER_SMCR_ETRF                    (7U << 4)
#define TIMER_SMCR_MSM_MASK                (1U << 7)                             /// Master/slave mode
#define TIMER_SMCR_ETF_MASK                (15U << 8)                            /// External trigger filter
#define TIMER_SMCR_ETF_NON                 (0U << 8)
#define TIMER_SMCR_ETF_1_N_2               (1U << 8)
#define TIMER_SMCR_ETF_1_N_4               (2U << 8)
#define TIMER_SMCR_ETF_1_N_8               (3U << 8)
#define TIMER_SMCR_ETF_2_N_6               (4U << 8)
#define TIMER_SMCR_ETF_2_N_8               (5U << 8)
#define TIMER_SMCR_ETF_4_N_6               (6U << 8)
#define TIMER_SMCR_ETF_4_N_8               (7U << 8)
#define TIMER_SMCR_ETF_8_N_6               (8U << 8)
#define TIMER_SMCR_ETF_8_N_8               (9U << 8)
#define TIMER_SMCR_ETF_16_N_5              (10U << 8)
#define TIMER_SMCR_ETF_16_N_6              (11U << 8)
#define TIMER_SMCR_ETF_16_N_8              (12U << 8)
#define TIMER_SMCR_ETF_32_N_5              (13U << 8)
#define TIMER_SMCR_ETF_32_N_6              (14U << 8)
#define TIMER_SMCR_ETF_32_N_8              (15U << 8)
#define TIMER_SMCR_ETPS_MASK               (3U << 12)                            /// External trigger prescaler
#define TIMER_SMCR_DIV_1                   (0U << 12)
#define TIMER_SMCR_DIV_2                   (1U << 12)
#define TIMER_SMCR_DIV_4                   (2U << 12)
#define TIMER_SMCR_DIV_8                   (3U << 12)
#define TIMER_SMCR_ECE_MASK                (1U << 14)                            ///< External clock enable
#define TIMER_SMCR_EXT_CLK_MODE_2_DIS      (0U << 14)
#define TIMER_SMCR_EXT_CLK_MODE_2_EN       (1U << 14)
#define TIMER_SMCR_EXP_MASK                (1U << 15)
#define TIMER_SMCR_EXP_HIGH_RISING         (0U << 15)
#define TIMER_SMCR_EXP_LOW_FALLING         (1U << 15)

/// TIMER DMA/Interrupt enable register DIER
#define TIMER_DIER_UIE                     (1U << 0)                             /// Update interrupt enable
#define TIMER_DIER_CCnIE(n)                (1U << ((n) + 1U))                    /// Capture/Compare n interrupt enable
#define TIMER_DIER_COMIE                   (1U << 5)                             /// COM interrupt enable
#define TIMER_DIER_TIE                     (1U << 6)                             /// Trigger interrupt enable
#define TIMER_DIER_BIE                     (1U << 7)                             /// Break interrupt enable
#define TIMER_DIER_UDE                     (1U << 8)                             /// Update DMA request enable
#define TIMER_DIER_CCnDE(n)                (1U << ((n) + 9U))                    /// Capture/Compare n DMA request enable
#define TIMER_DIER_COMDE                   (1U << 13)                            /// COM DMA request enable
#define TIMER_DIER_TDE                     (1U << 14)                            /// Trigger DMA request enable

/// TIMER status register SR
#define TIMER_SR_UIF_MASK                  (1U << 0)                             /// Update interrupt flag
#define TIMER_SR_CC0IF_MASK                (1U << 1)                             /// Capture/Compare 0 interrupt flag
#define TIMER_SR_CC1IF_MASK                (1U << 2)                             /// Capture/Compare 1 interrupt flag
#define TIMER_SR_CC2IF_MASK                (1U << 3)                             /// Capture/Compare 2 interrupt flag
#define TIMER_SR_CC3IF_MASK                (1U << 4)                             /// Capture/Compare 3 interrupt flag
#define TIMER_SR_TIF_MASK                  (1U << 6)                             /// Trigger interrupt flag
#define TIMER_SR_CCnOF_MASK                (0x0F << 9)                           /// Capture/Compare overcapture flag
#define TIMER_SR_CC0OF_MASK                (1U << 9)                             /// Capture/Compare 0 overcapture flag
#define TIMER_SR_CC1OF_MASK                (1U << 10)                            /// Capture/Compare 1 overcapture flag
#define TIMER_SR_CC2OF_MASK                (1U << 11)                            /// Capture/Compare 2 overcapture flag
#define TIMER_SR_CC3OF_MASK                (1U << 12)                            /// Capture/Compare 3 overcapture flag

/// TIMER event generation register EGR
#define TIMER_EGR_UG                       (1U << 0)
#define TIMER_EGR_CCnG(n)                  (1U << ((n) + 1))
#define TIMER_EGR_COMG                     (1U << 5)
#define TIMER_EGR_TG                       (1U << 6)
#define TIMER_EGR_BG                       (1U << 7)

/// TIMER capture/compare mode registerx  CCMR1/CCMR2, n=[0,1,2,3]
// input capture mode
#define TIMER_CCMR_CCnS_MASK(n)              ((n%2) ? (3U << 8)  : (3U << 0))    /// Capture/Compare x selection
#define TIMER_CCMR_CCnS_OUT(n)               ((n%2) ? (0U << 8)  : (0U << 0))
#define TIMER_CCMR_CCnS_ICn2TIn(n)           ((n%2) ? (1U << 8)  : (1U << 0))    // lint -e506
#define TIMER_CCMR_CCnS_ICn2TIm(n)           ((n%2) ? (2U << 8)  : (2U << 0))
#define TIMER_CCMR_CCnS_ICn2TRC(n)           ((n%2) ? (3U << 8)  : (3U << 0))
#define TIMER_CCMR_ICnPSC_MASK(n)            ((n%2) ? (3U << 10) : (3U << 2))    /// Input capture 1 prescaler
#define TIMER_CCMR_ICnPSC_0(n)               ((n%2) ? (0U << 10) : (0U << 2))    /// Input capture 1 prescaler - no prescaler
#define TIMER_CCMR_ICnPSC_2(n)               ((n%2) ? (1U << 10) : (1U << 2))    /// Capture is done once every 2 events
#define TIMER_CCMR_ICnPSC_4(n)               ((n%2) ? (2U << 10) : (2U << 2))    /// Capture is done once every 4 events
#define TIMER_CCMR_ICnPSC_8(n)               ((n%2) ? (3U << 10) : (3U << 2))    /// Capture is done once every 8 events
#define TIMER_CCMR_ICnF_POS(n)               ((n%2) ?  12U : 4U)
#define TIMER_CCMR_ICnF_MASK(n)              ((n%2) ? (15U << 12): (15U << 4))   /// Input capture 1 filter
#define TIMER_CCMR_ICnF_0N0(n)               ((n%2) ? (0U << 12) : (0U << 4))    /// No filter
#define TIMER_CCMR_ICnF_0N2(n)               ((n%2) ? (1U << 12) : (1U << 4))    /// fSAMPLING=FCK_INT, N=2
#define TIMER_CCMR_ICnF_0N4(n)               ((n%2) ? (2U << 12) : (2U << 4))    /// fSAMPLING=FCK_INT, N=4
#define TIMER_CCMR_ICnF_0N8(n)               ((n%2) ? (3U << 12) : (3U << 4))    /// fSAMPLING=FCK_INT, N=8
#define TIMER_CCMR_ICnF_2N6(n)               ((n%2) ? (4U << 12) : (4U << 4))    /// fSAMPLING=fDTS/2,  N=6
#define TIMER_CCMR_ICnF_2N8(n)               ((n%2) ? (5U << 12) : (5U << 4))    /// fSAMPLING=fDTS/2,  N=8
#define TIMER_CCMR_ICnF_4N6(n)               ((n%2) ? (6U << 12) : (6U << 4))    /// fSAMPLING=fDTS/4,  N=6
#define TIMER_CCMR_ICnF_4N8(n)               ((n%2) ? (7U << 12) : (7U << 4))    /// fSAMPLING=fDTS/4,  N=8
#define TIMER_CCMR_ICnF_8N6(n)               ((n%2) ? (8U << 12) : (8U << 4))    /// fSAMPLING=fDTS/8,  N=6
#define TIMER_CCMR_ICnF_8N8(n)               ((n%2) ? (9U << 12) : (9U << 4))    /// fSAMPLING=fDTS/8,  N=8
#define TIMER_CCMR_ICnF_16N5(n)              ((n%2) ? (10U << 12): (10U << 4))   /// fSAMPLING=fDTS/16, N=5
#define TIMER_CCMR_ICnF_16N6(n)              ((n%2) ? (11U << 12): (11U << 4))   /// fSAMPLING=fDTS/16, N=6
#define TIMER_CCMR_ICnF_16N8(n)              ((n%2) ? (12U << 12): (12U << 4))   /// fSAMPLING=fDTS/16, N=8
#define TIMER_CCMR_ICnF_32N5(n)              ((n%2) ? (13U << 12): (13U << 4))   /// fSAMPLING=fDTS/32, N=5
#define TIMER_CCMR_ICnF_32N6(n)              ((n%2) ? (14U << 12): (14U << 4))   /// fSAMPLING=fDTS/32, N=6
#define TIMER_CCMR_ICnF_32N8(n)              ((n%2) ? (15U << 12): (15U << 4))   /// fSAMPLING=fDTS/32, N=8
// output compare mode
#define TIMER_CCMR_OCnFE(n)                  ((n%2) ? (1U << 10) : (1U << 2))    /// Output Compare n fast enable
#define TIMER_CCMR_OCnPE(n)                  ((n%2) ? (1U << 11) : (1U << 3))    /// Output Compare n preload enable
#define TIMER_CCMR_OCnM_POS(n)               ((n%2) ? 12U : 4U)
#define TIMER_CCMR_OCnM_MASK(n)              ((n%2) ? (7U << 12) : (7U << 4))    /// Output Compare n mode
#define TIMER_CCMR_OCnM_FROZEN(n)            ((n%2) ? (0U << 12) : (0U << 4))
#define TIMER_CCMR_OCnM_OCnREF_HIGH(n)       ((n%2) ? (1U << 12) : (1U << 4))
#define TIMER_CCMR_OCnM_OCnREF_LOW(n)        ((n%2) ? (2U << 12) : (2U << 4))
#define TIMER_CCMR_OCnM_OCnREF_TOGGLE(n)     ((n%2) ? (3U << 12) : (3U << 4))
#define TIMER_CCMR_OCnM_FORCE_LOW(n)         ((n%2) ? (4U << 12) : (4U << 4))
#define TIMER_CCMR_OCnM_FORCE_HIGH(n)        ((n%2) ? (5U << 12) : (5U << 4))
#define TIMER_CCMR_OCnM_PWM_MODE_1(n)        ((n%2) ? (6U << 12) : (6U << 4))
#define TIMER_CCMR_OCnM_PWM_MODE_2(n)        ((n%2) ? (7U << 12) : (7U << 4))
#define TIMER_CCMR_OCnCE_MASK(n)             ((n%2) ? (1U << 15) : (1U << 7))    /// Output Compare n clear enable mask

/// TIMER Capture/Compare enable register CCER, (n=[0,1,2,3])
#define TIMER_CCER_CCnE_MASK(n)              (1U << ((n) * 4 + 0U))              /// Capture/Compare Enable mask
#define TIMER_CCER_CCnE(n)                   (1U << ((n) * 4 + 0U))              /// Capture/Compare Enable
#define TIMER_CCER_CCnP_MASK(n)              (1U << ((n) * 4 + 1U))              /// Capture/Compare 1 output polarity
#define TIMER_CCER_CCnP_OC_HIGH(n)           (0U << ((n) * 4 + 1U))              /// OCn active high
#define TIMER_CCER_CCnP_OC_LOW(n)            (1U << ((n) * 4 + 1U))              /// OCn active low
#define TIMER_CCER_CCnP_IC_RISING(n)         (0U << ((n) * 4 + 1U))              /// ICn rising
#define TIMER_CCER_CCnP_IC_FALLING(n)        (1U << ((n) * 4 + 1U))              /// ICn falling
#define TIMER_CCER_CCnNE_MASK(n)             (1U << ((n) * 4 + 2U))              /// Capture/Compare n complementary output enable mask
#define TIMER_CCER_CCnNE(n)                  (1U << ((n) * 4 + 2U))              /// Capture/Compare n complementary output enable
#define TIMER_CCER_CCnNE_OFF(n)              (0U << ((n) * 4 + 2U))
#define TIMER_CCER_CCnNE_ON(n)               (1U << ((n) * 4 + 2U))
#define TIMER_CCER_CCnNP_MASK(n)             (1U << ((n) * 4 + 3U))              /// Capture/Compare n complementary output polarity
#define TIMER_CCER_CCnNP_HIGH(n)             (0U << ((n) * 4 + 3U))
#define TIMER_CCER_CCnNP_LOW(n)              (1U << ((n) * 4 + 3U))

/// TIMER break and dead-time register BDTR
#define TIMER_BDTR_DTG_MASK                     (0xFF << 0)
#define TIMER_BDTR_DTG_LOCK_MASK                (3U << 8)
#define TIMER_BDTR_DTG_LOCK_OFF                 (0U << 8)
#define TIMER_BDTR_DTG_LOCK_LEVEL_1             (1U << 8)
#define TIMER_BDTR_DTG_LOCK_LEVEL_2             (2U << 8)
#define TIMER_BDTR_DTG_LOCK_LEVEL_3             (3U << 8)
#define TIMER_BDTR_DTG_OSSI_MASK                (1U << 10)                       /// Off-state selection for idle mode
#define TIMER_BDTR_DTG_OSSR_MASK                (1U << 11)                       /// Off-state selection for run mode
#define TIMER_BDTR_DTG_BKE_MASK                 (1U << 12)                       /// Break enable
#define TIMER_BDTR_DTG_BREAK_DIS                (0U << 12)
#define TIMER_BDTR_DTG_BREAK_EN                 (1U << 12)
#define TIMER_BDTR_DTG_BKP_MASK                 (1U << 13)                       /// Break polarity
#define TIMER_BDTR_DTG_BKP_ACT_LOW              (0U << 13)
#define TIMER_BDTR_DTG_BKP_ACT_HIGH             (1U << 13)
#define TIMER_BDTR_AOE_MASK                     (1U << 14)                       /// Automatic output enable
#define TIMER_BDTR_MOE_MASK                     (1U << 15)                       /// Main output enable
#define TIMER_BDTR_MOE                          (1U << 15)

/// TIMER control register DCR
#define TIMER_DCR_DBA_MASK                      (0x1F << 0)                      /// DMA base address
#define TIMER_DCR_DBL_MASK                      (0x1F << 8)                      /// DMA burst length
#define TIMER_DCR_DBL(len)                      ((len) << 8)
#define TIMER_DCR_DBL_MAX                       (18)
#define TIM_ARR_MAX   0xFFFF

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
 * @brief  tim pwm input start
 *
 * @param[in] tim  tim
 * @param[in] psc  psc
 *
 * timer cnt: (uint16_t)tim->CNT;
 * edge cnt: (uint16_t)tim->CCR[1], (uint16_t)tim->CCR[2];
 *
 **/
void tim_pwm_input_start(HS_TIM_Type *tim, uint32_t psc)
{
    int index = TIM_STRUCT2INDEX(tim);

    // Reset and Open clock
    register_set1(&HS_PSO->TIM_CFG[index], CPM_TIMER_SOFT_RESET_MASK);
    register_set0(&HS_PSO->TIM_CFG[index], CPM_TIMER_GATE_EN_MASK);
    HS_PSO_UPD_RDY();

    HS_TIM0->DIER  = 0;
    HS_TIM0->CR1   = TIMER_CR1_URS_MASK;
    HS_TIM0->PSC   = psc;
    HS_TIM0->ARR   = TIM_ARR_MAX;
    HS_TIM0->CNT   = 0;
    HS_TIM0->SR    = 0;

    // IC1->TI1, IC2->TI1
    HS_TIM0->CCMR1 = TIMER_CCMR_CCnS_ICn2TIn(0) | TIMER_CCMR_CCnS_ICn2TIm(1);
    // IC3->TI3, IC4->TI3
    HS_TIM0->CCMR2 = TIMER_CCMR_CCnS_ICn2TIn(0) | TIMER_CCMR_CCnS_ICn2TIm(1);
    // CC1 falling, CC2 rising edge
    HS_TIM0->CCER = TIMER_CCER_CCnP_OC_LOW(0) | TIMER_CCER_CCnP_OC_HIGH(1);
    // select trigger TI1FP1(Filtered Timer Input 1), slave mode--->reset mode
    HS_TIM0->SMCR = TIMER_SMCR_TI1FP1 | TIMER_SMCR_SMS_RESET_MODE;
    // enable CC1 and CC2 irq and update event
    HS_TIM0->DIER = TIMER_DIER_CCnIE(0) | TIMER_DIER_CCnIE(1) | TIMER_DIER_UIE;
    // enable capture
    HS_TIM0->CCER |= TIMER_CCER_CCnE(0) | TIMER_CCER_CCnE(1);
    // start
    HS_TIM0->CR1 |= TIMER_CR1_CEN;
}

/** @} */

