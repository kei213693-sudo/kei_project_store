/**
 * @file psram_serial.c
 * @brief 
 * @date Mon, Jul  8, 2019  2:10:02 PM
 * @author tangtanglin
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
#define PSRAM_CMD_RDID              0x9fu /* Read JEDEC ID */
#define PSRAM_CMD_WRITE             0x02    /* Write */
#define PSRAM_CMD_READ              0x03    /* Read */
#define PSRAM_CMD_FAST_READ         0x0B    /* Fast Read */
#define PSRAM_CMD_ENTER_QUAD        0x35    /* Enter Quad Mode */
#define PSRAM_CMD_QUAD_WRITE        0x38    /* Quad Write */
#define PSRAM_CMD_RESET_ENABLE      0x66    /* Reset Enable */
#define PSRAM_CMD_WRAP_WRITE        0x82    /* Wrap Write */
#define PSRAM_CMD_WRAP_READ         0x8B    /* Wrap Read */
#define PSRAM_CMD_RESET             0x99    /* Reset */
#define PSRAM_CMD_READ_ID           0x9F    /* Read Id */
#define PSRAM_CMD_MODE_REG_WRITE    0xB1    /* Write Mode Register */
#define PSRAM_CMD_MODE_REG_READ     0xB5    /* Read Mode Register */
#define PSRAM_CMD_BSTLEN_TOGGLE     0xC0    /* Burst Len Toggle */
#define PSRAM_CMD_QUAD_READ         0xEB    /* Quad Read */
#define PSRAM_CMD_EXIT_QUAD         0xF5    /* Exit Quad Mode */

#define PSRAM_RW_PARAM_CMD_ADDR_8BITS_DECLARE(name, cmd, addr, data, len) \
        sfb_rw_params_t name = {{{((cmd)<<24), (addr<<8)}}, 8, (data), (len)}
#define PSRAM_RW_PARAM_CMD_8BITS_DECLARE(name, cmd, data, len) \
        sfb_rw_params_t name = {{{((cmd)<<24), 0}}, 8, (data), (len)}
#define PSRAM_RW_PARAM_CMD_ADDR_32BITS_DECLARE(name, cmd, addr, data, len) \
        sfb_rw_params_t name = {{{((cmd)<<24)|(addr), 0}}, 32, (data), (len)}


/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */
static const psram_config_t psram_cmd_cfg_part_qspi_tbl[PSRAM_MAX] =
{
    { PSRAM_CMD_WRITE,          0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_READ,           0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_FAST_READ,      0,  0,  0,  0,  8,  0,  0 },
    { PSRAM_CMD_ENTER_QUAD,     0, 0, 0,0 },
    { PSRAM_CMD_QUAD_WRITE,     8,  WIDTH_1BIT, WIDTH_4BITS,    24, 0,  WIDTH_4BITS,    1},
    { PSRAM_CMD_WRAP_READ,      0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_RESET_ENABLE,   0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_WRAP_WRITE,     0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_WRAP_READ,      0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_READ_ID,        0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_MODE_REG_WRITE, 0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_MODE_REG_READ,  0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_BSTLEN_TOGGLE,  0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_QUAD_READ,      8,  WIDTH_1BIT, WIDTH_4BITS,    24, 6,  WIDTH_4BITS,    1},
    { PSRAM_CMD_EXIT_QUAD,      0, 0, 0,0, 0, 0,0 },
};

static const psram_config_t psram_cmd_cfg_all_qpi_table[PSRAM_MAX] =
{
    { PSRAM_CMD_WRITE,          8,  WIDTH_4BITS, WIDTH_4BITS,    24, 0,  WIDTH_4BITS,    1},
    { PSRAM_CMD_READ,           0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_FAST_READ,      8,  WIDTH_4BITS, WIDTH_4BITS,    24, 4,  WIDTH_4BITS,    1},
    { PSRAM_CMD_ENTER_QUAD,     0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_QUAD_WRITE,     8,  WIDTH_4BITS, WIDTH_4BITS,    24, 0,  WIDTH_4BITS,    1},
    { PSRAM_CMD_WRAP_READ,      0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_RESET_ENABLE,   0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_WRAP_WRITE,     0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_WRAP_READ,      0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_READ_ID,        0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_MODE_REG_WRITE, 0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_BSTLEN_TOGGLE,  0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_BSTLEN_TOGGLE,  0, 0, 0,0, 0, 0,0 },
    { PSRAM_CMD_QUAD_READ,      8,  WIDTH_4BITS, WIDTH_4BITS,   24, 6,  WIDTH_4BITS,    1},
    { PSRAM_CMD_EXIT_QUAD,      8,  WIDTH_4BITS, WIDTH_1BIT,    0,  0,  WIDTH_1BIT, 1},
};


/*********************************************************************
 * LOCAL VARIABLES
 */


/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */

/**
 * @brief psram_sw_config
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] config  config
 * @param[in] byteCnt  byteCnt
 **/
static void psram_sw_config(HS_SF_Type * sf, uint32_t cs, const psram_config_t * config, uint32_t byteCnt)
{
    // Write REG
    REGW(&sf->SW_SPI_CFG0_REG, MASK_5REG(
            SF_SW_CFG0_CMD_P0_BIT_CNT,      config->cmd_p0_bit_cnt,
            SF_SW_CFG0_CMD_P0_BUS_WIDTH,    config->cmd_p0_width,
            SF_SW_CFG0_CMD_P1_BUS_WIDTH,    config->cmd_p1_width,
            SF_SW_CFG0_CMD_P1_BIT_CNT,      config->cmd_p1_bit_cnt,
            SF_SW_CFG0_DUMMY_CYCLE_CNT,     config->sw_dummy_cycle_cnt));

    REGW(&sf->SW_SPI_CFG1_REG, MASK_4REG(
            SF_SW_CFG1_SDATA_BYTE_CNT,      byteCnt,
            SF_SW_CFG1_SDATA_BUS_WIDTH,     config->sdata_width,
            SF_SW_CFG1_BUF_WIDTH_BYTES,     4,
            SF_SW_CFG1_SW_CFG_EN,           config->sw_cfg_en));
}

/**
 * @brief psram_sw_enable
 *
 * @param[in] enable  enable sw
 **/
static void psram_sw_enable(HS_SF_Type * sf, bool enable)
{
    REGW(&sf->SW_SPI_CFG1_REG, MASK_1REG(SF_SW_CFG1_SW_CFG_EN,enable));
}


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 * @brief psram_config()
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] config  sf config
 **/
void psram_config(HS_SF_Type *sf, uint32_t cs, const psram_main_config_t *config)
{
    sfb_config_t sfbconfig;

    // config
    sfbconfig.freq_hz = config->freq_hz;
    sfbconfig.delay = config->delay;
    sfbconfig.transmode = SFB_SPI_MODE_0;
    sfbconfig.cs_pol = SFB_SPI_CS_LOW_ACTIVE;
    sfb_config(sf, cs, &sfbconfig);
}

/**
 * @brief read id
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 *
 * @return sflash id (24bits)
 **/
uint32_t psram_read_id(HS_SF_Type *sf, uint32_t cs)
{
    uint32_t id[3];

    psram_sw_enable(sf, 0);

    PSRAM_RW_PARAM_CMD_ADDR_32BITS_DECLARE(param, PSRAM_CMD_RDID, 0, id, 9);
    sfb_read_nodma(sf, cs, &param);

    // ignore EID[39:0]
    return ((id[0] & 0xFF0000) >> 16) | (id[0] & 0x00FF00) | ((id[0] & 0x0000FF) << 16);
}

/**
 * @brief sf psram quad enter
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 **/
void psram_quad_enter( HS_SF_Type *sf, uint32_t cs )
{
    psram_sw_enable(sf, 0);

    PSRAM_RW_PARAM_CMD_8BITS_DECLARE(param, PSRAM_CMD_ENTER_QUAD, NULL, 0);
    sfb_write_nodma(sf, cs, &param);
}

/**
 * @brief sf psram quad exit
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 **/
void psram_quad_exit( HS_SF_Type *sf, uint32_t cs )
{
    psram_config_t config;
    config = psram_cmd_cfg_all_qpi_table[PSRAM_EXIT_QUAD];

    psram_sw_config(sf, cs, &config, 0);

    PSRAM_RW_PARAM_CMD_8BITS_DECLARE(param, PSRAM_CMD_EXIT_QUAD, NULL, 0);
    sfb_read_nodma(sf, cs, &param);

    psram_sw_enable(sf, 0);
}

/**
 * @brief psram read normal without any quad
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_read_spi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, void *data, uint32_t length )
{
    co_assert((length & 3) == 0);
    co_assert(((uint32_t)data & 3) == 0);

    psram_sw_enable(sf, 0);

    PSRAM_RW_PARAM_CMD_ADDR_32BITS_DECLARE(param, PSRAM_CMD_READ, addr, data, length);
    sfb_read_dma(sf, cs, &param);
}

/**
 * @brief psram fast read with part qpi that wait cycle is 8
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_read_fast_spi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, void *data, uint32_t length )
{
    co_assert((length & 3) == 0);
    co_assert(((uint32_t)data & 3) == 0);

    psram_config_t config;
    config = psram_cmd_cfg_part_qspi_tbl[PSRAM_FAST_READ];

    psram_sw_config(sf, cs, &config, length);

    PSRAM_RW_PARAM_CMD_ADDR_32BITS_DECLARE(param, PSRAM_CMD_FAST_READ, addr, data, length);
    sfb_read_dma(sf, cs, &param);
}

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
void psram_read_fast_qpi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, void *data, uint32_t length )
{
    co_assert((length & 3) == 0);
    co_assert(((uint32_t)data & 3) == 0);

    psram_config_t config;
    config = psram_cmd_cfg_all_qpi_table[PSRAM_FAST_READ];

    psram_sw_config(sf, cs, &config, length);

    PSRAM_RW_PARAM_CMD_ADDR_8BITS_DECLARE(param, PSRAM_CMD_FAST_READ, addr, data, length);
    sfb_read_dma(sf, cs, &param);
}

/**
 * @brief psram fast quad read with part qpi
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_read_fast_quad_spi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, void *data, uint32_t length )
{
    co_assert((length & 3) == 0);
    co_assert(((uint32_t)data & 3) == 0);

    psram_config_t config;
    config = psram_cmd_cfg_part_qspi_tbl[PSRAM_QUAD_READ];

    psram_sw_config(sf, cs, &config, length);

    PSRAM_RW_PARAM_CMD_ADDR_8BITS_DECLARE(param, PSRAM_CMD_QUAD_READ, addr, data, length);
    sfb_read_dma(sf, cs, &param);
}

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
void psram_read_fast_quad_qpi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, void *data, uint32_t length )
{
    co_assert((length & 3) == 0);
    co_assert(((uint32_t)data & 3) == 0);

    psram_config_t config;
    config = psram_cmd_cfg_all_qpi_table[PSRAM_QUAD_READ];

    psram_sw_config(sf, cs, &config, length);

    PSRAM_RW_PARAM_CMD_ADDR_8BITS_DECLARE(param, PSRAM_CMD_QUAD_READ, addr, data, length);
    sfb_read_dma(sf, cs, &param);
}

/**
 * @brief psram write with part qpi
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_write_spi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, const void *data, uint32_t length )
{
    co_assert((length & 3) == 0);
    co_assert(((uint32_t)data & 3) == 0);

    psram_sw_enable(sf, 0);

    PSRAM_RW_PARAM_CMD_ADDR_32BITS_DECLARE(param, PSRAM_CMD_WRITE, addr, data, length);
    sfb_write_dma(sf, cs, &param);
}

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
void psram_write_qpi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, const void *data, uint32_t length )
{
    co_assert((length & 3) == 0);
    co_assert(((uint32_t)data & 3) == 0);

    psram_config_t config;
    config = psram_cmd_cfg_all_qpi_table[PSRAM_WRITE];

    psram_sw_config(sf, cs, &config, length);

    PSRAM_RW_PARAM_CMD_ADDR_8BITS_DECLARE(param, PSRAM_CMD_WRITE, addr, data, length);
    sfb_write_dma(sf, cs, &param);
}

/**
 * @brief psram write with part qpi
 *
 * @param[in] sf  sf object
 * @param[in] cs  cs (0 ~ SFB_CS_NUM-1)
 * @param[in] addr  sflash address
 * @param[out] data  read data buffer
 * @param[in] length  length
 **/
void psram_write_quad_spi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, const void *data, uint32_t length )
{
    co_assert((length & 3) == 0);
    co_assert(((uint32_t)data & 3) == 0);

    psram_config_t config;
    config = psram_cmd_cfg_part_qspi_tbl[PSRAM_QUAD_WRITE];

    psram_sw_config(sf, cs, &config, length);

    PSRAM_RW_PARAM_CMD_ADDR_8BITS_DECLARE(param, PSRAM_CMD_QUAD_WRITE, addr, data, length);
    sfb_write_dma(sf, cs, &param);
}

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
void psram_write_quad_qpi( HS_SF_Type *sf, uint32_t cs, uint32_t addr, const void *data, uint32_t length )
{
    co_assert((length & 3) == 0);
    co_assert(((uint32_t)data & 3) == 0);

    psram_config_t config;
    config = psram_cmd_cfg_all_qpi_table[PSRAM_QUAD_WRITE];

    psram_sw_config(sf, cs, &config, length);

    PSRAM_RW_PARAM_CMD_ADDR_8BITS_DECLARE(param, PSRAM_CMD_QUAD_WRITE, addr, data, length);
    sfb_write_dma(sf, cs, &param);
}

/** @} */

