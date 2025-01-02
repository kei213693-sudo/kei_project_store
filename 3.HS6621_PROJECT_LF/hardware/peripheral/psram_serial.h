/**
 * @file psram_serial.h
 * @brief psram_serial driver
 * @date Fri, Jul 12, 2019  3:16:15 PM
 * @author tangtanglin
 *
 * @defgroup psram_serial
 * @ingroup PERIPHERAL
 * @brief psram_serial driver
 * @details psram_serial driver
 *
 * @{
 *
 * @example example_psram.c
 * This is an example of how to use the psram
 *
 */

#ifndef __PSRAM_SERIAL_H__
#define __PSRAM_SERIAL_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

/*********************************************************************
 * MACROS
 */
/// Set dma done event callback, reference @ref sfb_dma_done_event_register
#define psram_dma_done_event_register(psram, cb)    sfb_dma_done_event_register(psram, cb);
/// psram open, reference @ref sfb_open
#define psram_open(psram)                           sfb_open(psram);
/// psram close, reference @ref sfb_close
#define psram_close(psram)                          sfb_close(psram);
/// psram enable, reference @ref sfb_enable
#define psram_enable(psram, cs)                     sfb_enable(psram, cs);

/*********************************************************************
 * TYPEDEFS
 */

/// PSRAM wire width
typedef enum
{
    PSRAM_WIDTH_1LINE = 1,
    PSRAM_WIDTH_2LINE = 2,
    PSRAM_WIDTH_4LINE = 4,
}psram_width_t;

/// PSRAM status
typedef enum
{
    /// Not detected
    PSRAM_STATUS_NONE,
    /// Detected and sflash absent
    PSRAM_STATUS_ABSENT,
    /// Detected and sflash present
    PSRAM_STATUS_PRESENT,
}psram_status_t;

/// PSRAM config
typedef struct
{
    /// frequency in Hz
    uint32_t        freq_hz;
    /// width @ref sf_width_t
    psram_width_t   width;
    /// Delayed Sampling, default is 0
    uint8_t         delay;
}psram_main_config_t;

typedef enum
{
    PSRAM_WRITE = 0,
    PSRAM_READ, 
    PSRAM_FAST_READ,
    PSRAM_ENTER_QUAD,
    PSRAM_QUAD_WRITE,
    PSRAM_RESET_ENABLE,
    PSRAM_WRAP_WRITE,
    PSRAM_WRAP_READ,
    PSRAM_RESET,
    PSRAM_READ_ID,
    PSRAM_MODE_REG_WRITE, 
    PSRAM_MODE_REG_READ,
    PSRAM_BURSTLEN_TOGGLE,
    PSRAM_QUAD_READ,
    PSRAM_EXIT_QUAD, 
    PSRAM_MAX
}psram_cmd_t; /* aps1604m_3sqr_qspi */

typedef enum
{
    WIDTH_1BIT = 0,
    WIDTH_2BITS = 1,
    WIDTH_4BITS = 2,
    WIDTH_MAX,
}psram_sw_bus_width;

typedef struct
{
    psram_cmd_t         cmd;

    uint8_t             cmd_p0_bit_cnt;
    psram_sw_bus_width  cmd_p0_width;
    psram_sw_bus_width  cmd_p1_width;
    uint8_t             cmd_p1_bit_cnt;
    uint8_t             sw_dummy_cycle_cnt;

    psram_sw_bus_width  sdata_width; 
    bool                sw_cfg_en;
}psram_config_t;

/*********************************************************************
 * EXTERN VARIABLES
 */


/*********************************************************************
 * EXTERN FUNCTIONS
 */
/**
 * @brief read id
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 *
 * @return psram id (24bits)
 **/
uint32_t psram_read_id(HS_SF_Type *sf, uint32_t cs);

/**
 * @brief psram read normal without any quad
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_read_spi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, void *data, uint32_t length );

/**
 * @brief psram fast read with part qpi that wait cycle is 8
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_read_fast_spi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, void *data, uint32_t length );

/**
 * @brief psram fast read with all qpi
 *
 * @note You may need to call psram_quad_enter()/psram_quad_exit() on the outside
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_read_fast_qpi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, void *data, uint32_t length );

/**
 * @brief psram fast quad read with part qpi
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_read_fast_quad_spi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, void *data, uint32_t length );

/**
 * @brief psram fast quad read with all qpi
 *
 * @note You may need to call psram_quad_enter()/psram_quad_exit() on the outside
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_read_fast_quad_qpi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, void *data, uint32_t length );

/**
 * @brief psram write with part qpi
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_write_spi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, const void *data, uint32_t length );

/**
 * @brief psram write with all qpi
 *
 * @note You may need to call psram_quad_enter()/psram_quad_exit() on the outside
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_write_qpi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, const void *data, uint32_t length );

/**
 * @brief psram write with part qpi
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_write_quad_spi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, const void *data, uint32_t length );

/**
 * @brief psram write with all qpi
 *
 * @note You may need to call psram_quad_enter()/psram_quad_exit() on the outside
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_write_quad_qpi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, const void *data, uint32_t length );

#ifdef __cplusplus
}
#endif

#endif

/** @} */

