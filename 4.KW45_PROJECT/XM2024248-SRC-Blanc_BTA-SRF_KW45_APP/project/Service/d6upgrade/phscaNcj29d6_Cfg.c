/*
 (c) NXP B.V. 2009-2019. All rights reserved.

 Disclaimer
 1. The NXP Software/Source Code is provided to Licensee "AS IS" without any
 warranties of any kind. NXP makes no warranties to Licensee and shall not
 indemnify Licensee or hold it harmless for any reason related to the NXP
 Software/Source Code or otherwise be liable to the NXP customer. The NXP
 customer acknowledges and agrees that the NXP Software/Source Code is
 provided AS-IS and accepts all risks of utilizing the NXP Software under
 the conditions set forth according to this disclaimer.

 2. NXP EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT OF INTELLECTUAL PROPERTY
 RIGHTS. NXP SHALL HAVE NO LIABILITY TO THE NXP CUSTOMER, OR ITS
 SUBSIDIARIES, AFFILIATES, OR ANY OTHER THIRD PARTY FOR ANY DAMAGES,
 INCLUDING WITHOUT LIMITATION, DAMAGES RESULTING OR ALLEGDED TO HAVE
 RESULTED FROM ANY DEFECT, ERROR OR OMMISSION IN THE NXP SOFTWARE/SOURCE
 CODE, THIRD PARTY APPLICATION SOFTWARE AND/OR DOCUMENTATION, OR AS A
 RESULT OF ANY INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHT OF ANY
 THIRD PARTY. IN NO EVENT SHALL NXP BE LIABLE FOR ANY INCIDENTAL,
 INDIRECT, SPECIAL, EXEMPLARY, PUNITIVE, OR CONSEQUENTIAL DAMAGES
 (INCLUDING LOST PROFITS) SUFFERED BY NXP CUSTOMER OR ITS SUBSIDIARIES,
 AFFILIATES, OR ANY OTHER THIRD PARTY ARISING OUT OF OR RELATED TO THE NXP
 SOFTWARE/SOURCE CODE EVEN IF NXP HAS BEEN ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGES.

 3. NXP reserves the right to make changes to the NXP Software/Sourcecode any
 time, also without informing customer.

 4. Licensee agrees to indemnify and hold harmless NXP and its affiliated
 companies from and against any claims, suits, losses, damages,
 liabilities, costs and expenses (including reasonable attorney's fees)
 resulting from Licensee's and/or Licensee customer's/licensee's use of the
 NXP Software/Source Code.
 */

/*
 *    @file: phscaNcj29d6_Cfg.c
 *   @brief: Configuration of driver for NCJ29D6 UWB module
 */

/* =============================================================================
 * External Includes
 * ========================================================================== */
#include "phscaTypes.h"
#include "hal_pin_if.h"
#include "hal_spi_if.h"
#include "hal_rte.h"

#include "phscaUwb.h"
#include "D6Update.h"



/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_systickCounter  = 0U;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/


/* =============================================================================
 * Internal Includes
 * ========================================================================== */
#define PHSCANCJ29D6_CFG_EXTERN_GUARD
#include "phscaNcj29d6_Cfg.h"
#include "phscaNcj29d6.h"
#undef PHSCANCJ29D6_CFG_EXTERN_GUARD

/* =============================================================================
 * Private Symbol Defines
 * ========================================================================== */


/* =============================================================================
 * Private Function-like Macros
 * ========================================================================== */
#define LOW_SET_CS() hal_pin_if_write(J29D6_CS_INDEX,HAL_PIN_LEVEL_LOW)
#define HIGH_SET_CS() hal_pin_if_write(J29D6_CS_INDEX,HAL_PIN_LEVEL_HIGH)
#define STA_GET_RDY() hal_pin_if_read(UWB_RDY_INDEX)
#define STA_GET_INT() hal_pin_if_read(UWB_INT0_INDEX)
#define HIGH_SET_RST() hal_pin_if_write(UWB_RST_INDEX,HAL_PIN_LEVEL_HIGH)
#define LOW_SET_RST() hal_pin_if_write(UWB_RST_INDEX,HAL_PIN_LEVEL_LOW)
/* =============================================================================
 * Private Type Definitions
 * ========================================================================== */

/* =============================================================================
 * Private Function Prototypes
 * ========================================================================== */

/* =============================================================================
 * Private Module-wide Visible Variables
 * ========================================================================== */


/* =============================================================================
 * Function Definitions
 * ========================================================================== */

uint32_t tickstart = 0u; /** 开始时间 */
uint32_t tickGet = 0u;   /** 当前时间 */
uint32_t tickPass = 0u;  /** 超时时间 */

/**
 * 函 数 介 绍: 超时判定
 *
 * 参       数: ms        超时时间
 * 返   回  值: uint8_t  0 未超时
 * 返   回  值: uint8_t  1 超时
 */
uint8_t isPass(uint32_t ms)
{
    tickGet = phscaNcj29d6_Gettick();
    tickPass = (tickGet > tickstart) ? (tickGet - tickstart) : (0xffffffff - tickstart + tickGet + 1u);
    if (0u == ms)
    {
        tickstart = tickGet;
    }
    return (uint8_t)(tickPass >= ms);
}

/**
 * 函 数 介 绍: 超时判断，同时更新时基
 *
 * 参       数: ms        超时时间
 * 返   回  值: uint8_t  0 未超时
 * 返   回  值: uint8_t  1 超时
 */
uint8_t bePass(uint32_t ms)
{
    tickGet = phscaNcj29d6_Gettick();
    tickPass = (tickGet > tickstart) ? (tickGet - tickstart) : (0xffffffff - tickstart + tickGet + 1u);
    uint8_t ret = (uint8_t)(tickPass >= ms);
    tickstart = (ret || (!ms)) ? tickGet : tickstart;
    return ret;
}

/**
 * 函 数 介 绍: 内部定时器
 *
 * 参       数:
 * 返   回  值:
 * 返   回  值:
 */
void D6SWUPTICK(void)
{
     g_systickCounter++;
}

uint32_t HAL_GetTick(void)
{
    return g_systickCounter;
}

void SPI_INIT(void)
{


}

void SPI_TR(uint8_t *tp, uint8_t *rp, uint16_t len)
{
    hal_spi_if_write(J29D6_SPI_INDEX, tp, rp, len);
}


void phscaNcj29d6_InitDevice(void)
{
	hal_spi_if_init(HAL_SPI_CHANNEL1);
	hal_pin_if_irq(UWB_INT0_INDEX,5,phscaNcj29d6_IntPinCallbackIsr);
	hal_pin_if_irq_enabled(UWB_INT0_INDEX,HAL_ENABLED_ENABLE);
}

void phscaNcj29d6_DelayMilliseconds(const uint32_t u32_DelayMilliseconds)
{
    #define SYS_FRQ (72000000UL)
    /** ++ 硬延时 */
    volatile uint32_t var = u32_DelayMilliseconds*SYS_FRQ/1000UL;

    while (--var);

}

void phscaNcj29d6_SetRst(const bool b_AssertRst)
{
	/** ++ 设置复位 */
	if (b_AssertRst)
	{
		LOW_SET_RST();
	}
	else
	{
		HIGH_SET_RST();
	}
}

void phscaNcj29d6_SetCs(const bool b_AssertCs)
{
    /** ++ 设置片选线 */
    if (b_AssertCs)
    {
        LOW_SET_CS();
    }
    else
    {
        HIGH_SET_CS();
    }

}

bool phscaNcj29d6_GetRdy(void)
{
    /** ++ 获取Rdy线 */
    return (bool)STA_GET_RDY();
}

bool phscaNcj29d6_GetInt(void)
{
    /** ++ 获取Int线 */
    return (bool)STA_GET_INT();
}

uint16_t phscaNcj29d6_CalculateCrc16(uint8_t u8arr_Data[], uint16_t u16_DataLength)
{
    return phscaNcj29d6_CalculateCrc16Sw(u8arr_Data, u16_DataLength);
}

void phscaNcj29d6_ClearIntIrqStatus(void)
{
	/** ++ 清除外部中断线 */
}

void phscaNcj29d6_SpiTransceive(const uint32_t u32_DataLength, const uint8_t u8arr_DataToTransmit[],
								uint8_t u8arr_DataReceived[])
{
    /** ++ SPI 阻塞发送驱动 */
    uint8_t *tp = (uint8_t*)&u8arr_DataToTransmit[0];
    uint8_t *rp = (uint8_t*)&u8arr_DataReceived[0];
    SPI_TR(tp, rp, (uint16_t)u32_DataLength);
}

uint32_t phscaNcj29d6_Gettick(void)
{
    return HAL_GetTick();
}
