/*

Copyright 2016-2020 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#include "genfsk_interface.h"
#include "genfsk_ll.h"
#include "genfsk_utils.h"

#include "fsl_os_abstraction.h"
#include "app_sniffer.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Interrupt handler for GENFSK LL. */
void GENFSK_InterruptHandler(void);

WEAK bool_t PWR_ClearRSIMDsmInt(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t mGENFSK_IrqDisableCnt = 1;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * Handler of GENFSK_IRQ_CTRL_TX_IRQ_MASK.
 * Set gGenfskTxEventFlag_c if event.
 */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
static inline void GENFSK_IRQTx(osa_event_flags_t *pEventFlags)
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
static inline void GENFSK_IRQTx(osaEventFlags_t *pEventFlags)
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
{
#if !defined(RADIO_IS_GEN_4P0) && !defined(RADIO_IS_GEN_4P5)
    GENFSK->T1_CMP &= ~GENFSK_T1_CMP_T1_CMP_EN_MASK;

    *pEventFlags |= gGenfskTxEventFlag_c;
#else
    GENFSK->XCVR_CFG &= ~GENFSK_XCVR_CFG_T1_CMP_EN_MASK;
#if defined(RADIO_IS_GEN_4P5)
    *pEventFlags |= gGenfskTxEventFlag_c;
#endif
#endif
}

//uint16_t Watermark = 0;
//uint16_t byteCount1 = 0;
//uint32_t u32packet_cfg = 0;
//uint32_t u32len_adj = 0;
//uint32_t u32enh_fea = 0;
//uint32_t u32misc1 = 0;
/*!
 * Handler of GENFSK_IRQ_CTRL_RX_IRQ_MASK.
 * Set gGenfskRxEventFlag_c.
 */
static inline void GENFSK_IRQRx(osa_event_flags_t *pEventFlags)
{
//一次接收事件，理论上都是先接收master，再接收slave，两者间隔时间较短
//但是实际上可能在一个连接间隔时间内数据包不止两个，例如刚建立连接时通信较为频繁
//所以要考虑如何根据当前接收到的数据来区分是master还是slave的数据？
//LL层数据包含rssi、Channel Index、PHY、Counter、Direction等信息
//可以根据LL层的flags数据的bit1的Direction来判断，但是这个sdk中找不到
//目前LL层数据只找到了rssi能从GENFSK->XCVR_CTRL找到，其他的都找不到存储位置

//	 Watermark = ((GENFSK->RX_WATERMARK & GENFSK_RX_WATERMARK_RX_WATERMARK_MASK) >> GENFSK_RX_WATERMARK_RX_WATERMARK_SHIFT);
//	 byteCount1 = ((GENFSK->XCVR_CTRL & GENFSK_XCVR_CTRL_LENGTH_EXT_MASK) >> GENFSK_XCVR_CTRL_LENGTH_EXT_SHIFT);
//     u32packet_cfg = GENFSK->PACKET_CFG;
//     u32len_adj = GENFSK->LENGTH_ADJ;
//     u32enh_fea = GENFSK->ENH_FEATURE;
//     u32misc1 = GENFSK->MISC1;
    
    if (genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskRxEvent)
    {
    #if (APP_SNIFFER_EN)
        if(packetFromRole == 1)
        {//master data
            packetFromRole = 0;

            masterRSSI = (int8_t)((GENFSK->XCVR_STS & GENFSK_XCVR_STS_RSSI_MASK) >> GENFSK_XCVR_STS_RSSI_SHIFT);
            masterbyteCount = ((GENFSK->RX_WATERMARK & GENFSK_RX_WATERMARK_BYTE_COUNTER_MASK) >> GENFSK_RX_WATERMARK_BYTE_COUNTER_SHIFT);
            FLib_MemSet(masterPacket,0,sizeof(masterPacket));//先清空
            FLib_MemCpy(masterPacket, RX_PACKET_RAM, masterbyteCount);

            GENFSK->XCVR_CTRL = 5;//0_0101b - RX Start Now
        }
        else
        {//slave data
            packetFromRole = 1;

            slaveRSSI =  (int8_t)((GENFSK->XCVR_STS & GENFSK_XCVR_STS_RSSI_MASK) >> GENFSK_XCVR_STS_RSSI_SHIFT);
            slavebyteCount = ((GENFSK->RX_WATERMARK & GENFSK_RX_WATERMARK_BYTE_COUNTER_MASK) >> GENFSK_RX_WATERMARK_BYTE_COUNTER_SHIFT);
            FLib_MemSet(slavePacket,0,sizeof(slavePacket));//先清空
            FLib_MemCpy(slavePacket, RX_PACKET_RAM, slavebyteCount);

            GENFSK_TimeCancelEvent(&rxTimeoutTimer);
            GENFSK->XCVR_CFG &=~ GENFSK_XCVR_CFG_T1_CMP_EN_MASK;//该bit置0
            *pEventFlags |= gGenfskRxEventFlag_c;//结束此次接收
        }
    #else
        GENFSK_TimeCancelEvent(&rxTimeoutTimer);
        #if !defined(RADIO_IS_GEN_4P0) && !defined(RADIO_IS_GEN_4P5)
        GENFSK->T1_CMP &= ~GENFSK_T1_CMP_T1_CMP_EN_MASK;
        #else
        GENFSK->XCVR_CFG &= ~GENFSK_XCVR_CFG_T1_CMP_EN_MASK;
        #endif
        *pEventFlags |= gGenfskRxEventFlag_c;//设置 Rx 完成事件
    #endif
    }
}

/*!
 * Handler of GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK.
 * Set gGenfskH0FailEventFlag_c | gGenfskLengthFailEventFlag_c | gGenfskH1FailEventFlag_c if gGenfskPromiscuousEvent is enabled.
 */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
static inline void GENFSK_IRQSequenceEnd(osa_event_flags_t *pEventFlags)
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
static inline void GENFSK_IRQSequenceEnd(osaEventFlags_t *pEventFlags)
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
{
    if ((genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskPromiscuousEvent) != 0U)
    {
        if ((GENFSK->PACKET_CFG & GENFSK_PACKET_CFG_H0_FAIL_MASK) != 0U)
        {
            *pEventFlags |= gGenfskH0FailEventFlag_c;
        }
        else if ((GENFSK->PACKET_CFG & GENFSK_PACKET_CFG_LENGTH_FAIL_MASK) != 0U)
        {
            *pEventFlags |= gGenfskLengthFailEventFlag_c;
        }
        else if ((GENFSK->PACKET_CFG & GENFSK_PACKET_CFG_H1_FAIL_MASK) != 0U)
        {
            *pEventFlags |= gGenfskH1FailEventFlag_c;
        }
        else
        {
            /* Do nothing. */
        }
#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
        /* Request a MWS release on sequence end interrupt currently used only in promiscuous mode. */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
        (void)OSA_EventSet((osa_event_handle_t)mGenfskTaskEvent, gGenfskMwsReleaseEventFlag_c);
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
        (void) OSA_EventSet(mGenfskTaskEvent, gGenfskMwsReleaseEventFlag_c);
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
#endif /* (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c) */
    }
}

/*!
 * Handler of GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_MASK.
 * Set gGenfskRxPllUnlockEventFlag_c event
 */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
static inline void GENFSK_IRQPllUnlock(osa_event_flags_t *pEventFlags)
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
static inline void GENFSK_IRQPllUnlock(osaEventFlags_t *pEventFlags)
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
{
    /* If current state is Rx or RxTx sequence, Abort and send event */
    if ((genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingRx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyRx)
#if defined (RADIO_IS_GEN_3P5)
        || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingRxTx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyRxTx)
#endif
            )
    {
        GENFSK_AbortAll();

        *pEventFlags |= gGenfskRxPllUnlockEventFlag_c;
    }

    /* If current state is Tx or TxRx sequence, Abort and send event */
    if ((genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingTx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyTx)
#if defined (RADIO_IS_GEN_3P5)
        || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingTxRx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyTxRx)
#endif
            )
    {
        GENFSK_AbortAll();

        *pEventFlags |= gGenfskTxPllUnlockEventFlag_c;
    }
}

/*!
 * Handler of GENFSK_IRQ_CTRL_WAKE_IRQ_MASK.
 * Set gGenfskWakeEventFlag_c if event is enabled.
 */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
static inline void GENFSK_IRQWake(osa_event_flags_t *pEventFlags)
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
static inline void GENFSK_IRQWake(osaEventFlags_t *pEventFlags)
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
{
    uint32_t timeAdjust = 0;
#if !defined(RADIO_IS_GEN_4P0) && !defined(RADIO_IS_GEN_4P5)
    GENFSK->EVENT_TMR = (timeAdjust) | GENFSK_EVENT_TMR_EVENT_TMR_ADD_MASK;
#else
    GENFSK->EVENT_TMR_ADD = timeAdjust;
#endif
    if ((genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskWakeEvent) != 0U)
    {
        *pEventFlags |= gGenfskWakeEventFlag_c;
    }
}

/*!
 * @brief Disable GENFSK global interrupt.
 *
 * This function disables GENFSK global interrupt.
 */
void GENFSK_ProtectFromXcvrInterrupt(void)
{
    volatile uint32_t temp;

    /* Enter critical section. */
    OSA_InterruptDisable();

    if (mGENFSK_IrqDisableCnt == 0U)
    {
        temp = GENFSK->IRQ_CTRL;
        temp &= ~(gGENFSK_AllIrqFlags | GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK);
        GENFSK->IRQ_CTRL = temp;
    }

    mGENFSK_IrqDisableCnt++;

    /* Exit critical section. */
    OSA_InterruptEnable();
}

/*!
 * @brief Enable GENFSK global interrupt.
 *
 * This function enables GENFSK global interrupt.
 */
void GENFSK_UnprotectFromXcvrInterrupt(void)
{
    volatile uint32_t temp;

    /* Enter critical section. */
    OSA_InterruptDisable();

    if (mGENFSK_IrqDisableCnt != 0U)
    {
        mGENFSK_IrqDisableCnt--;

        if (mGENFSK_IrqDisableCnt == 0U)
        {
            temp = GENFSK->IRQ_CTRL;
            temp &= ~gGENFSK_AllIrqFlags;
            temp |= GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK;
#if !defined (RADIO_IS_GEN_3P5) && !defined(RADIO_IS_GEN_4P0) && !defined(RADIO_IS_GEN_4P5)
            temp |= GENFSK_IRQ_CTRL_CRC_IGNORE_MASK;
#endif
            GENFSK->IRQ_CTRL = temp;
        }
    }

    /* Exit critical section. */
    OSA_InterruptEnable();
}

/*! @brief Disable GENFSK interrupts based on _GENFSK_interrupt_enable. */
void GENFSK_DisableInterrupts(uint32_t mask)
{
    uint32_t irqSts;

    mask &= gGENFSK_AllIrqEnable;

    irqSts = GENFSK->IRQ_CTRL;
    irqSts &= ~gGENFSK_AllIrqFlags;
    irqSts |= (mask >> 16);  /* Clear interrupt flags. */
    irqSts &= ~(mask);
    GENFSK->IRQ_CTRL = irqSts;
    genfskLocal[mGenfskActiveInstance].genfskRegs.irqCtrl = irqSts;
}

/*! @brief Enable GENFSK interrupts based on _GENFSK_interrupt_enable. */
void GENFSK_EnableInterrupts(uint32_t mask)
{
    uint32_t irqSts;

    mask &= gGENFSK_AllIrqEnable;

    irqSts = GENFSK->IRQ_CTRL;
    irqSts &= ~gGENFSK_AllIrqFlags;
    irqSts |= (mask >> 16);  /* Clear interrupt flags. */
    irqSts |= (mask);
    GENFSK->IRQ_CTRL = irqSts;
    genfskLocal[mGenfskActiveInstance].genfskRegs.irqCtrl = irqSts;
#if defined (RADIO_IS_GEN_3P5) && !defined(RADIO_IS_GEN_4P0) && !defined(RADIO_IS_GEN_4P5)
    /* TODO: remove this after radio init sequence rework as XCVR_MISC->XCVR_CTRL is overwritten during radio init*/
    XCVR_MISC->XCVR_CTRL &= (uint32_t)~(uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_MASK);
    XCVR_MISC->XCVR_CTRL |= (uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL(GENFSK_LL));
#endif
#if defined(RADIO_IS_GEN_4P5)
    /* Enable timer overflow IRQ */
    GENFSK->IRQ_CTRL2 |= GENFSK_IRQ_CTRL2_EVENT_TIMER_OVER_FLOW_IRQ_EN_MASK;
#endif
}

#if (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
/* This function is overwritten when doing code coverage then do not monitor it */
/* LCOV_EXCL_START */
#endif
/*!
 * Default implementation for DSM clearing.
 * Can be overwritten on a per-device basis.
 */
WEAK bool_t PWR_ClearRSIMDsmInt(void)
{
  return FALSE;
}
#if (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
/* LCOV_EXCL_STOP */
#endif

/* LCOV_EXCL_START */
#if (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
WEAK void GENFSK_InterruptStatusHook(volatile uint32_t *irqStatus_p) {}
#endif
/* LCOV_EXCL_STOP */

/*! @brief GENFSK LL Interrupt handler. */
void GENFSK_InterruptHandler(void)
{
    uint32_t irqStatus;
    uint32_t irq2Status;
    osa_event_flags_t eventFlags = 0;

    if(!PWR_ClearRSIMDsmInt())
    {
        /* Read current XCVR status and interrupt status. */
        irqStatus = GENFSK->IRQ_CTRL;

        /* Clear all GENFSK interrupts. */
        GENFSK->IRQ_CTRL = irqStatus;

        irq2Status = GENFSK->IRQ_CTRL2;
        GENFSK->IRQ_CTRL2 = irq2Status;

        /* TX interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_TX_IRQ_MASK) != 0U)
        {
            GENFSK_IRQTx(&eventFlags);
        }

        /* RX interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_RX_IRQ_MASK) != 0U)
        {
            GENFSK_IRQRx(&eventFlags);
        }

        /* Sequence End interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK) != 0U)
        {
            GENFSK_IRQSequenceEnd(&eventFlags);
        }

        /* Network Address Match interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_NTW_ADR_IRQ_MASK) != 0U)
        {
            if ((genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskNwkAddressMatch) != 0U)
            {
                eventFlags |= gGenfskNwkAddressMatchEventFlag_c;
            }
        }

        /* Timer (T1) Compare interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_T1_IRQ_MASK) != 0U)
        {
            /* Do nothing. */
        }

        /* Timer (T2) Compare interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_T2_IRQ_MASK) != 0U)
        {
            GENFSK_TimeDisableWaitTimeout();
            GENFSK_TimeISR();
        }

        /* PLL Unlock interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_MASK) != 0U)
        {
            GENFSK_IRQPllUnlock(&eventFlags);
        }

        /* Wake interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_WAKE_IRQ_MASK) != 0U)
        {
            GENFSK_IRQWake(&eventFlags);
        }

        /* RX Watermark interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_RX_WATERMARK_IRQ_MASK) != 0U)
        {
        }

        if( (irq2Status & GENFSK_IRQ_CTRL2_EVENT_TIMER_OVER_FLOW_IRQ_MASK) != 0U)
        {
            GENFSK_EventTimerOverflowISR();
        }

        if (eventFlags != 0U)
        {
            (void)OSA_EventSet((osa_event_handle_t)mGenfskTaskEvent, eventFlags);
        }
    }
}

/*!
 * @brief GENFSK LL interrupt install.
 *
 * This function install the interrupt handler for GENFSK LL, sets interrupt priority and enables the interrupt.
 */
void GENFSK_InstallInterrupt(void)
{
/* note: Radio_0_IRQn is defined for all platforms except for CPU_K32W042S1M2VPJ_cm0plus which defines RF0_0_IRQn instead */
#if ( (defined(Radio_0_IRQn) && (gGENFSK_IrqNo_d == Radio_0_IRQn)) || (defined(RF0_0_IRQn) && (gGENFSK_IrqNo_d == RF0_0_IRQn)) )
    {
        XCVR_MISC->XCVR_CTRL &= (uint32_t)~(uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_MASK);
        XCVR_MISC->XCVR_CTRL |= (uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL(GENFSK_LL));
    }
#elif (defined(K32W232H_SERIES) || defined(KW45B41Z83_SERIES) || defined(K32W1480_SERIES))
    {
        /* TBD */
#if 0
        XCVR_MISC->XCVR_CTRL &= ;
        XCVR_MISC->XCVR_CTRL |= ;
#endif
    }
#else
    {
        XCVR_MISC->XCVR_CTRL &= (uint32_t)~(uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_MASK);
        XCVR_MISC->XCVR_CTRL |= (uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL(GENFSK_LL));
    }
#endif

    OSA_InstallIntHandler((uint32_t)gGENFSK_IrqNo_d, GENFSK_InterruptHandler);

    /* Enable transceiver interrupt request. */
    NVIC_ClearPendingIRQ(gGENFSK_IrqNo_d);
    NVIC_EnableIRQ(gGENFSK_IrqNo_d);

    /* Set transceiver interrupt priority. */
    NVIC_SetPriority(gGENFSK_IrqNo_d, gGENFSK_IrqPriority_c >> (8 - __NVIC_PRIO_BITS));

    GENFSK_UnprotectFromXcvrInterrupt();
}
