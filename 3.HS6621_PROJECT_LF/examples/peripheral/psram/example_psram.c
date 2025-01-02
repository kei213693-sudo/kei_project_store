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


/*********************************************************************
 * MACROS
 */
#define BUF_SIZE                512

#define SF_TEST_ADDRESS         0x64000

#define PIN_SF_CS               21  //PB5
#define PIN_SF_SO               22  //PB6
#define PIN_SF_CK               24  //PB8
#define PIN_SF_SI               25  //PB9
#define PIN_SF_HD               20  //PB4
#define PIN_SF_WP               23  //PB7

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

volatile uint32_t psram_id = 0;

/*
 * Outside flash: sf=HS_SF, cs=1
 */
psram_main_config_t psram_cfg[] = {
    {1000000,   PSRAM_WIDTH_1LINE,  5},
    {8000000,   PSRAM_WIDTH_1LINE,  5},
    {16000000,  PSRAM_WIDTH_1LINE,  5},
    {32000000,  PSRAM_WIDTH_1LINE,  5},
    {64000000,  PSRAM_WIDTH_1LINE,  5},
    {1000000,   PSRAM_WIDTH_2LINE,  5},
    {8000000,   PSRAM_WIDTH_2LINE,  5},
    {16000000,  PSRAM_WIDTH_2LINE,  5},
    {32000000,  PSRAM_WIDTH_2LINE,  5},
    {64000000,  PSRAM_WIDTH_2LINE,  5},
    {1000000,   PSRAM_WIDTH_4LINE,  5}, 
    {8000000,   PSRAM_WIDTH_4LINE,  5}, 
    {16000000,  PSRAM_WIDTH_4LINE,  5}, 
    {32000000,  PSRAM_WIDTH_4LINE,  5}, 
    {64000000,  PSRAM_WIDTH_4LINE,  5}, 
};

CO_ALIGN(4) static uint8_t sf_read_buf1[100];
CO_ALIGN(4) static uint8_t sf_write_buf1[100];


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void psram_cfg_pin(HS_SF_Type *sf, uint32_t cs)
{
    //mww 0x400E0054 0xFFFFFFFF
    //pmu_pin_input_enable(0xFFFFFFFF);
    REGW(&HS_PMU->GPIO_IE_CTRL, MASK_1REG(PMU_GPIO_IE_CTRL_31_0, 0xFFFFFFFF));
    
    //mww 0x400e0034 0x10
    pmu_pin_mode_set(BITMASK(4), PMU_PIN_MODE_PU);

    //mww 0x400E00c4 0x80
    pmu_pin_driven_current_set(BITMASK(7),PMU_PIN_DRIVEN_CURRENT_6MA);

    //mww 0x400E00cc 0x7FFFF
    pmu_pin_driven_current_set(0x7FFFF,PMU_PIN_DRIVEN_CURRENT_12MA);

    //mww 0x40000080 0x00000000; #0:3
    //mww 0x40000084 0x1c1c1c1c; #4:7
    //mww 0x40000088 0x1c191c1c; #11:9
    //mww 0x4000008C 0x1d1E1f20; #15:12
    //mww 0x40000090 0x1c1c211c; #19:16

    if( HS_SF1 == sf )
    {
        cs = 0; /* pinmux support only cs0 */
        pinmux_config(PIN_SF_CS, PINMUX_SFLASH1_CSN_CFG);
        pinmux_config(PIN_SF_SO, PINMUX_SFLASH1_SO_CFG);
        pinmux_config(PIN_SF_CK, PINMUX_SFLASH1_CK_CFG);
        pinmux_config(PIN_SF_SI, PINMUX_SFLASH1_SI_CFG);

        pinmux_config(PIN_SF_HD, PINMUX_SFLASH1_HD_CFG);
        pinmux_config(PIN_SF_WP, PINMUX_SFLASH1_WP_CFG);
    }
    else if( HS_SF2 == sf ) 
    {
        if( cs )
        {
            pinmux_config(PIN_SF_CS,  PINMUX_SFLASH2_CSN_1_CFG);
        }
        else
        {
            pinmux_config(PIN_SF_CS,  PINMUX_SFLASH2_CSN_CFG);
        }
        pinmux_config(PIN_SF_SO, PINMUX_SFLASH2_SO_CFG);
        pinmux_config(PIN_SF_CK, PINMUX_SFLASH2_CK_CFG);
        pinmux_config(PIN_SF_SI, PINMUX_SFLASH2_SI_CFG);

        pinmux_config(PIN_SF_HD, PINMUX_SFLASH2_HD_CFG);
        pinmux_config(PIN_SF_WP, PINMUX_SFLASH2_WP_CFG);
    }
}

static psram_out_cmd_op(HS_SF_Type *sf, uint32_t cs)
{
    int i = 0;
    int j = 0;
    uint32_t status = 0;

    for(i=0; i<sizeof(sf_write_buf1); ++i)
        sf_write_buf1[i] = i;

    for(i=0; i<sizeof(sf_read_buf1); ++i)
        sf_read_buf1[i] = 0;

    psram_cfg_pin(sf, cs);

    for(j = 0; j < sizeof(psram_cfg)/sizeof(sf_config_t); j++)
    {
        log_debug("--------------------------------------\n");
        log_debug("\n");
        log_debug("freq_hz = %d, width = %d, delay = %d\n", psram_cfg[j].freq_hz, psram_cfg[j].width, psram_cfg[j].delay);
        // open
        psram_open(sf);

        // config
        psram_config(sf, cs, &psram_cfg[j]);

        // psram ID
        psram_id = psram_read_id(sf, cs);
        log_debug("out psram ID: %08X\n", psram_id);

        psram_write_spi(sf, cs, SF_TEST_ADDRESS, (uint8_t *)sf_write_buf1, sizeof(sf_write_buf1));
        for(i=0; i<sizeof(sf_read_buf1); i++)
            sf_read_buf1[i] = 0;

        psram_read_spi(sf, cs, SF_TEST_ADDRESS, (uint8_t *)sf_read_buf1, sizeof(sf_read_buf1));
        for(i=0; i<sizeof(sf_read_buf1); ++i)
        {
            if( sf_read_buf1[i] != sf_write_buf1[i] )
            {
                log_debug("step1 Fail id=%d, r=%x,w=%x \n", i,sf_read_buf1[i],sf_write_buf1[i]);
                status = 1;
                break;
            }
        }

        if( 0 == status )
        {
            log_debug("step1 Pass!!! WRITE-2-S-READ-2-S \n");
        }

        //should enter qpi mode in order to op in qpi mode
        psram_quad_enter(sf, cs);
        psram_write_qpi(sf, cs, SF_TEST_ADDRESS+sizeof(sf_write_buf1), (uint8_t *)sf_write_buf1, sizeof(sf_write_buf1));

        for(i=0; i<sizeof(sf_read_buf1); i++)
            sf_read_buf1[i] = 0;

        psram_read_fast_qpi(sf, cs, SF_TEST_ADDRESS+sizeof(sf_write_buf1), (uint8_t *)sf_read_buf1, sizeof(sf_read_buf1));
        for(i=0; i<sizeof(sf_read_buf1); i++)
        {
            if( sf_read_buf1[i] != sf_write_buf1[i] )
            {
                log_debug("step2 Fail id=%d, r=%x,w=%x \n", i,sf_read_buf1[i],sf_write_buf1[i]);
                status = 1;
                break;
            }
        }
        //should exit qpi mode in order to op in spi mode
        psram_quad_exit(sf, cs);   

        if( 0 == status )
        {
            log_debug("step2 Pass!!! WRITE-4-Q-READ_FAST-2-Q \n");
        }

        psram_write_quad_spi(sf, cs, SF_TEST_ADDRESS+2*sizeof(sf_write_buf1), (uint8_t *)sf_write_buf1, sizeof(sf_write_buf1));

        for(i=0; i<sizeof(sf_read_buf1); i++)
            sf_read_buf1[i] = 0;

        //should enter qpi mode in order to op in qpi mode
        psram_quad_enter(sf, cs);   
        psram_read_fast_quad_qpi(sf, cs, SF_TEST_ADDRESS+2*sizeof(sf_write_buf1), (uint8_t *)sf_read_buf1, sizeof(sf_read_buf1));
        for(i=0; i<sizeof(sf_read_buf1); i++)
        {
            if( sf_read_buf1[i] != sf_write_buf1[i] )
            {
                log_debug("step3 Fail id=%d, r=%x,w=%x \n", i,sf_read_buf1[i],sf_write_buf1[i]);
                status = 1;
                break;
            }
        }
        if( 0 == status )
        {
            log_debug("step3 Pass!!! WRITE_QUAD-4-Q-READ_FAST_QUAD-4-Q \n");
        }

        psram_write_quad_qpi(sf, cs, SF_TEST_ADDRESS+3*sizeof(sf_write_buf1), (uint8_t *)sf_write_buf1, sizeof(sf_write_buf1));
        //should exit qpi mode in order to op in spi mode
        psram_quad_exit(sf, cs);   

        for(i=0; i<sizeof(sf_read_buf1); ++i)
            sf_read_buf1[i] = 0;

        psram_read_spi(sf, cs, SF_TEST_ADDRESS+3*sizeof(sf_write_buf1), (uint8_t *)sf_read_buf1, sizeof(sf_read_buf1));
        for(i=0; i<sizeof(sf_read_buf1); i++)
        {
            if( sf_read_buf1[i] != sf_write_buf1[i] )
            {
                log_debug("step4 Fail id=%d, r=%x,w=%x \n", i,sf_read_buf1[i],sf_write_buf1[i]);
                status = 1;
                break;
            }
        }
        if( 0 == status )
        {
            log_debug("step4 Pass!!! WRITE_QUAD-4-Q-READ_FAST_QUAD-4-Q \n");
        }

        //should enter qpi mode in order to op in qpi mode
        psram_quad_enter(sf, cs);   
        psram_write_quad_qpi(sf, cs, SF_TEST_ADDRESS+4*sizeof(sf_write_buf1), (uint8_t *)sf_write_buf1, sizeof(sf_write_buf1));
        for(i=0; i<sizeof(sf_read_buf1); ++i)
            sf_read_buf1[i] = 0;
        //should exit qpi mode in order to op in spi mode
        psram_quad_exit(sf, cs);   

        psram_read_fast_spi(sf, cs, SF_TEST_ADDRESS+4*sizeof(sf_write_buf1), (uint8_t *)sf_read_buf1, sizeof(sf_read_buf1));
        for(i=0; i<sizeof(sf_read_buf1); i++)
        {
            if( sf_read_buf1[i] != sf_write_buf1[i] )
            {
                log_debug("step5 Fail id=%d, r=%x,w=%x \n", i,sf_read_buf1[i],sf_write_buf1[i]);
                status = 1;
                break;
            }
        }
        if( 0 == status )
        {
            log_debug("step5 Pass!!! WRITE_QUAD-4-Q-READ_FAST_2 \n");
        }

        //should enter qpi mode in order to op in qpi mode
        psram_quad_enter(sf, cs);   
        psram_write_quad_qpi(sf, cs, SF_TEST_ADDRESS+5*sizeof(sf_write_buf1), (uint8_t *)sf_write_buf1, sizeof(sf_write_buf1));
        for(i=0; i<sizeof(sf_read_buf1); ++i)
            sf_read_buf1[i] = 0;
        //should exit qpi mode in order to op in spi mode
        psram_quad_exit(sf, cs);   
        psram_read_fast_quad_spi(sf, cs, SF_TEST_ADDRESS+5*sizeof(sf_write_buf1), (uint8_t *)sf_read_buf1, sizeof(sf_read_buf1));
        for(i=0; i<sizeof(sf_read_buf1); i++)
        {
            if( sf_read_buf1[i] != sf_write_buf1[i] )
            {
                log_debug("step6 Fail id=%d, r=%x,w=%x \n", i,sf_read_buf1[i],sf_write_buf1[i]);
                status = 1;
                break;
            }
        }

        if( 0 == status )
        {
            log_debug("step6 Pass!!! WRITE_QUAD-4-Q-READ_FAST_QUAD_2 \n");
        }
            log_debug("\n");
    }

}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_psram(void)
{
    log_debug("psram test start\n\n");

    log_debug("\r\nPSRAM out SF1 0 ctl begin\r\n");
    psram_out_cmd_op(HS_SF1,0);
   
    log_debug("\r\nPSRAM out SF2 0 ctl begin\r\n");
    psram_out_cmd_op(HS_SF2,0);

    log_debug("\r\nPSRAM out SF2 1 ctl begin\r\n");
    psram_out_cmd_op(HS_SF2,1);

    log_debug("psram test end\n\n");
}

/** @} */

