/**
 * @file dma_test.c
 * @brief 
 * @date Wed, Oct  9, 2019  2:10:24 PM
 * @author houguozhi
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
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include "peripheral.h"
#include "shell.h"
#include "spi_flash.h"
#include "cmd.h"

#if (CONFIG_APP_SHELL)

#if (SHELL_CMD_USE_DMA)

/*********************************************************************
 * MACROS
 */

#define PIN_SPI0_MISO                   20
#define PIN_SPI0_MOSI                   19
#define PIN_SPI0_CSN                    17
#define PIN_SPI0_SCK                    18

#define PIN_SPI1_MISO                   27
#define PIN_SPI1_MOSI                   28
#define PIN_SPI1_CSN                    30
#define PIN_SPI1_SCK                    11

#define PIN_I2S_SDI                     8 //
#define PIN_I2S_SDO_0                   9
#define PIN_I2S_SDO_1                   10 //



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
/**********************************************************************/

#ifdef CONFIG_HS6621
void cmd_dma(int argc, char *argv[])
{
    return;
}
#else
#define BUF_SIZE      512

/**
 * DMA callback function of type @p dma_callback_t.
 */
static void dma_cb(dma_status_t status, uint32_t cur_src_addr, uint32_t cur_dst_addr, uint32_t xfer_size)
{
    log_debug("\r\n");
    if (status == DMA_STATUS_BLOCK_OK) {
        log_debug("DMA block transfer succeed, block info: ");
    }
    else if (status == DMA_STATUS_ABORT) {
        log_debug("DMA block transfer aborted, current info: ");
    }
    else {
        log_debug("DMA block transfer error, current info: ");
    }
    log_debug("status = 0x%08x, src_addr = 0x%08x, dst_addr = 0x%08x, xfer_size = %d\r\n",
           status, cur_src_addr, cur_dst_addr, xfer_size);
    fflush(stdout);
}

static void print_buf(void *buf, int length, char *title)
{
    if (title)
        log_debug("%s:\r\n", title);

    // use length == 0 as string flag.
    if (length == 0)
        log_debug("%s\r\n", (char *)buf);
    else {
        for (int i = 0; i < length; i++)
            log_debug("%02x%s", ((uint8_t *)buf)[i], (i%16 != 15) ? " " : "\r\n");
    }
    log_debug("\r\n");
}

static void dma_print_block(dma_block_t *block)
{
    for (dma_block_t *p = block; p; p = (dma_block_t *)p->LLPointer)
        log_debug("block: addr = 0x%08x, Ctrl = 0x%08x, SrcAddr = 0x%08x, DstAddr = 0x%08x, LLPointer = 0x%08x\r\n",
               p, p->Ctrl, p->SrcAddr, p->DstAddr, p->LLPointer);
}

/**
 * @brief ROM to RAM transfer.
 * @Note  HS6621C.ROM (ORIGIN = 0x00100000, LENGTH = 512K) is not supported by DMA.
 */
static void cmd_dma_rom_to_ram(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    log_debug("Note: HS6621C.ROM (ORIGIN = 0x00100000, LENGTH = 512K) is not supported by DMA.\r\n");
}

/**
 * Single block transfer test using hs6621-compatible APIs.
 *
 */
static void cmd_dma_mem_to_mem_single_block(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    log_debug("mem to mem single block transfer test:\r\n");
    log_debug("Transfer: buf[0] => buf[1].\r\n");

    // Init buffers
    uint32_t buf_32bits_aligned[BUF_SIZE/4];
    uint32_t buf_8bits_aligned[BUF_SIZE];
    uint8_t *buf[2] = {(uint8_t *)buf_32bits_aligned, (uint8_t *)buf_8bits_aligned};
    for (int i = 0; i < BUF_SIZE; i++)
        buf[0][i] = i;
    memset(buf[1], 0, BUF_SIZE);

    // Setup DMA and start transfer.
    dma_init();
    HS_DMA_CH_Type *dma_ch = dma_allocate();
    dma_config_t config;
    DMA_SETUP_CONFIG(config,
                     DMA_MEM_TO_MEM,
                     MEM_DMA_ID,
                     DMA_SLAVE_BUSWIDTH_32BITS,
                     DMA_SLAVE_BUSWIDTH_8BITS,
                     DMA_BURST_LEN_4UNITS,
                     DMA_BURST_LEN_32UNITS,
                     false,
                     0,
                     NULL);
    
    if (!dma_config(dma_ch, &config)) {
        log_debug("DMA config fails.\r\n");
        return;
    }

    /* memcpy(buf[1], buf[0], BUF_SIZE); */
    dma_start(dma_ch, (uint32_t)buf[0], (uint32_t)buf[1], BUF_SIZE/4);
    dma_wait_stop(dma_ch);
    dma_release(dma_ch);
    
    // Check result
    print_buf(buf[0], BUF_SIZE, "buf[0]:");
    print_buf(buf[1], BUF_SIZE, "buf[1]:");

    if (memcmp(buf[0], buf[1], BUF_SIZE) == 0)
        log_debug("DMA transfer Mem-to-Mem succeeds.\r\n");
    else
        log_debug("DMA transfer Mem-to-Mem fails.\r\n");
}

/**
 * Multiple block transfer tests using hs6621c's simplified APIs.
 */
static void cmd_dma_mem_to_mem_multi_blocks(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // Allocate and init buffers:
    // Buffer must be aligned to source/destination's bus width.
    // block transfer size must be aligned to source/destination's (bus_width * burst_size)
    // Or, in other words, src/dst burst size must be selected according to src/dst's bus width and block transfer size.
    uint32_t buf_32bits_aligned[2][BUF_SIZE/4];
    uint8_t  buf_8bits_aligned[2][BUF_SIZE];
    
    uint8_t *buf[4] = {(uint8_t *)buf_32bits_aligned[0], (uint8_t *)buf_32bits_aligned[1],
                       (uint8_t *)buf_8bits_aligned[0],  (uint8_t *)buf_8bits_aligned[1]};
    for (int i = 0; i < BUF_SIZE; i++) {
        buf[0][i] = i;
        buf[1][i] = ~i;
        buf[2][i] = 0;
        buf[3][i] = 0;
    }

    log_debug("mem to mem multiple block transfer test:\r\n");
    log_debug("Transfer: buf[0] => buf[2].\r\n");
    log_debug("Transfer: buf[1] => buf[3].\r\n");

    // Build blocks
    dma_block_t block[2];
    dma_dev_t src = {
        .id         = MEM_DMA_ID,
        .addr       = buf[0],
        .addr_ctrl  = DMA_ADDR_CTRL_INC,
        .bus_width  = DMA_SLAVE_BUSWIDTH_32BITS,
        .burst_size = DMA_BURST_LEN_4UNITS,
    };
    dma_dev_t dst = {
        .id         = MEM_DMA_ID,
        .addr       = buf[2],
        .addr_ctrl  = DMA_ADDR_CTRL_INC,
        .bus_width  = DMA_SLAVE_BUSWIDTH_8BITS,
        .burst_size = DMA_BURST_LEN_1UNITS,
    };
    dma_block_config_t block_config = {
        .src                 = &src,
        .dst                 = &dst,
        .block_size_in_bytes = BUF_SIZE,
        .priority            = 0,
        .flow_controller     = DMA_FLOW_CONTROLLER_USE_NONE,
        .intr_en             = false,
    };
    dma_build_block(&block[0], &block_config);
    
    src.addr = buf[1];
    dst.addr = buf[3];
    dst.bus_width = DMA_SLAVE_BUSWIDTH_32BITS;
    dma_build_block(&block[1], &block_config);
    
    dma_append_block(block, block+1);
    dma_print_block(block);

    // Init DMA and start transfer.
    dma_init();
    HS_DMA_CH_Type *dma_ch = dma_start_transfer(NULL, block, dma_cb);
    if (dma_ch == NULL) {
        log_debug("DMA transfer fails.\r\n");
        return;
    }
    dma_wait_stop(dma_ch);
    dma_release(dma_ch);
    
    // Check result
    print_buf(buf[0], BUF_SIZE,   "buf[0]");
    print_buf(buf[1], BUF_SIZE/2, "buf[1]");
    print_buf(buf[2], BUF_SIZE,   "buf[2]");
    print_buf(buf[3], BUF_SIZE/2, "buf[3]");
    
    if (memcmp(buf[0], buf[2], BUF_SIZE) == 0 && memcmp(buf[1], buf[3], BUF_SIZE/2) == 0)
        log_debug("DMA transfer Mem-to-Mem succeeds.\r\n");
    else
        log_debug("DMA transfer Mem-to-Mem fails.\r\n");
}

static void cmd_dma_mem_to_mem(int argc, char *argv[])
{
    cmd_dma_rom_to_ram(argc, argv);
    cmd_dma_mem_to_mem_single_block(argc, argv);
    cmd_dma_mem_to_mem_multi_blocks(argc, argv);
}


/*
 * ADC DMA test
 ****************************************************************************************
 */
#define BLOCK_SIZE                256
#define ADC_DMA_BUFFER_SIZE       1024
#define BLOCK_NUM                 (ADC_DMA_BUFFER_SIZE*4/BLOCK_SIZE)
DMA_DECLARE_LLIP(adc_dma_block_llip, BLOCK_NUM);

static uint32_t adc_dma_buffer[ADC_DMA_BUFFER_SIZE];
static HS_DMA_CH_Type *adc_dma = NULL;

void _adc_dma_callback(adc_event_t event, float adc_data)
{
    if (event&ADC_EVENT_CH_T_DONE)
    {
        log_debug("temperature = %f\n", adc_data);
    }
    else if (event&ADC_EVENT_CH_V_DONE)
    {
        log_debug("battery = %f\n", adc_data);
    }
    else if (event&ADC_EVENT_DMA_DONE)
    {
        adc_stop_convert();
        adc_stop();
        dma_stop(adc_dma);
        int32_t sum1 = 0;

        log_debug("dma data:\n");
        for(uint32_t i = 0; i < ADC_DMA_BUFFER_SIZE; ++i)
        {
            sum1 += (int16_t)adc_dma_buffer[i];
            if((i>0)&&(0 == (i%8)))
            {
                log_debug("\n");
            }
            log_debug("%08x ", adc_dma_buffer[i]);
        }
        log_debug("\n");

        sum1 = sum1/ADC_DMA_BUFFER_SIZE;
        log_debug("ch:5, %08x, %f\n", sum1, (float)(sum1)*0.8/2048.0);

        log_debug("adc_dma_buffer addr: %08x\n", adc_dma_buffer);
    }
    else
    {
        log_debug("event=%x, pin = %f\n", event, adc_data);
    }
}

static void cmd_dma_adc_to_mem(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    memset(adc_dma_buffer, 0, ADC_DMA_BUFFER_SIZE*4);
    
#if 0    
    float ret = adc_battery_voltage_read((adc_callback_t)_adc_callback);
    adc_start_convert();
    log_debug("battery = %f\n", ret);

#else    
    const adc_dma_config_t adc_dma_cfg = {
        .use_fifo       = false,
        .buffer         = adc_dma_buffer,
        .buffer_len     = ADC_DMA_BUFFER_SIZE*4,
        .block_llip     = adc_dma_block_llip,
        .block_num      = BLOCK_NUM,
        .callback       = dma_cb, // NULL,
    };

    log_debug("adc dma test start\n");
    /* dma_fifo_init(&dma_fifo, adc_dma_buffer, ADC_DMA_BUFFER_SIZE*4); */
    dma_init();

    adc_channel_config_t channel_config;
    adc_config_t config;
    uint32_t src_addr;
    
    adc_init();
    adc_start();
        
    config.trigger_mode   = ADC_TRIGGER_MODDE_SW;
    config.trigger_res    = ADC_TRIG_RES_CHANNEL;
    config.trigger_count  = 0;
    config.hw_trigger_sel = ADC_HW_TIMER0_0;
    adc_config(&config);

    channel_config.callback = (adc_callback_t)_adc_dma_callback;
    channel_config.inp_gp   = ADC_CHANNEL_EXTERN_CH2;
    channel_config.inn_gp   = ADC_CHANNEL_CHIP_VCM;
    channel_config.clk_sel  = ADC_CLK_SEL_2MHZ;
    //channel_config.count    = 0;
    channel_config.gtune_gp = ADC_GTUNE_GP_0P5;
    channel_config.scal_gp  = ADC_SCAL_GP_ENABLE;
    channel_config.vcm_gp   = ADC_VCM_SEL_000mV;
    adc_add_channel(&channel_config);
        
    if (argc ==2 && strcmp(argv[1], "debug") == 0) {
        /// debug for dma on FPGA
        register_set(&HS_GPADC->ADC_CFG1, MASK_1REG(GPADC_SW_DBG,  1));
        register_set(&HS_GPADC->CMPN_CTRL, MASK_1REG(GPADC_COMPEN_EN, 0));
    }

    log_debug("cfg1=%x\n", HS_GPADC->ADC_CFG1);
    log_debug("reg0=%x\n", HS_GPADC->CH_0_CFG);
    log_debug("reg1=%x\n", HS_GPADC->CH_1_CFG);
    log_debug("reg2=%x\n", HS_GPADC->CH_2_CFG);
    log_debug("reg3=%x\n", HS_GPADC->CH_3_CFG);
    log_debug("reg4=%x\n", HS_GPADC->CH_4_CFG);
    /// the result value is (config*8-1)&0x0FFF
    log_debug("reg5=%x,%x\n", HS_GPADC->CH_5_CFG,(HS_GPADC->CH_5_CFG*8-1)&0x0FFF);
    log_debug("reg6=%x\n", HS_GPADC->CH_6_CFG);
    log_debug("reg7=%x\n", HS_GPADC->CH_7_CFG);
    log_debug("reg8=%x\n", HS_GPADC->CH_8_CFG);
    log_debug("reg9=%x\n", HS_GPADC->CH_9_CFG);
    log_debug("rega=%x\n", HS_GPADC->CH_A_CFG);
    log_debug("regb=%x\n", HS_GPADC->CH_B_CFG);
    log_debug("regc=%x\n", HS_GPADC->CH_C_CFG);
        
    adc_dma = adc_dma_config((HS_DMA_CH_Type *)NULL, &adc_dma_cfg, &src_addr);
    dma_start(adc_dma, src_addr, (uint32_t)adc_dma_buffer, ADC_DMA_BUFFER_SIZE);
    adc_start_convert();

    dma_wait_stop(adc_dma);
    
    if (0) {
        adc_stop_convert();
        adc_stop();
        dma_stop(adc_dma);
        int32_t sum1 = 0;

        log_debug("dma data:\n");
        for(uint32_t i = 0; i < ADC_DMA_BUFFER_SIZE; ++i)
        {
            sum1 += (int16_t)adc_dma_buffer[i];
            if((i>0)&&(0 == (i%8)))
            {
                log_debug("\n");
            }
            log_debug("%08x ", adc_dma_buffer[i]);
        }
        log_debug("\n");

        sum1 = sum1/ADC_DMA_BUFFER_SIZE;
        log_debug("ch:5, %08x, %f\n", sum1, (float)(sum1)*0.8/2048.0);
    }
#endif

}

static void cmd_dma_mem_to_uart(int argc, char *argv[])
{
    (void)argc;

    if (strcmp(argv[0], "1") != 0) {
        log_debug("Only uart 1 is supported by DMA.\r\n");
        return;
    }

    // Allocate and init buffers
    uint32_t buf_32bits_aligned[2][BUF_SIZE/4];
    char *buf[2] = {(char *)buf_32bits_aligned[0], (char *)buf_32bits_aligned[1]};
    for (int i = 0; i < BUF_SIZE; i++) {
        buf[0][i] = i;
        buf[1][i] = ~i;
    }
    sprintf(buf[0], "Hello world!s");
    sprintf(buf[1], "Good morning!");

    log_debug("mem to uart1 multiple block transfer test:\r\n");
    log_debug("Transfer: buf[0] => uart1: \"%s\"\r\n", buf[0]);
    log_debug("Transfer: buf[1] => uart1: \"%s\"\r\n", buf[1]);

    // Build blocks
    dma_block_t block[2];
    dma_dev_t dma_mem = {
        .id         = MEM_DMA_ID,
        .addr       = buf[0],
        .addr_ctrl  = DMA_ADDR_CTRL_INC,
        .bus_width  = DMA_SLAVE_BUSWIDTH_8BITS,
        .burst_size = DMA_BURST_LEN_1UNITS,
    };
    dma_dev_t dma_uart = {
        .id         = UART1_TX_DMA_ID,
        .addr       = (uint8_t *)&HS_UART1->THR,
        .addr_ctrl  = DMA_ADDR_CTRL_FIX,
        .bus_width  = DMA_SLAVE_BUSWIDTH_8BITS,
        .burst_size = DMA_BURST_LEN_1UNITS,
    };
    dma_block_config_t block_config = {
        .src                 = &dma_mem,
        .dst                 = &dma_uart,
        .block_size_in_bytes = strlen(buf[0]),
        .priority            = 0,
        .flow_controller     = DMA_FLOW_CONTROLLER_USE_NONE,
        .intr_en             = false,
    };
    dma_build_block(&block[0], &block_config);
    
    dma_mem.addr = buf[1];
    block_config.block_size_in_bytes = strlen(buf[1]);
    dma_build_block(&block[1], &block_config);
    
    dma_append_block(block, block+1);
    dma_print_block(block);
    
    // Init DMA and start transfer.
    dma_init();
    HS_DMA_CH_Type *dma_ch = dma_start_transfer(NULL, block, dma_cb);
    if (dma_ch == NULL) {
        log_debug("DMA mem-to-uart1 transfer fails.\r\n");
        return;
    }
    dma_wait_stop(dma_ch);
    dma_release(dma_ch);

    // Check result by manual
}

static void cmd_dma_uart_to_mem(int argc, char *argv[])
{
    (void)argc;

    if (strcmp(argv[0], "1") != 0) {
        log_debug("Only uart 1 is supported by DMA.\r\n");
        return;
    }

    // Allocate and init buffers
    uint8_t buf[3][BUF_SIZE];
    for (int i = 0; i < BUF_SIZE; i++) {
        buf[0][i] = 0;
        buf[1][i] = 0;
        buf[2][i] = 0;
    }

    log_debug("uart1 to mem multiple block transfer test:\r\n");
    log_debug("Transfer: uart1 => buf[0].\r\n");
    log_debug("Transfer: uart1 => buf[1].\r\n");
    log_debug("Transfer: uart1 => buf[2].\r\n");

    // Build blocks
    dma_block_t block[3];
    dma_dev_t dma_mem = {
        .id         = MEM_DMA_ID,
        .addr       = buf[0],
        .addr_ctrl  = DMA_ADDR_CTRL_INC,
        .bus_width  = DMA_SLAVE_BUSWIDTH_8BITS,
        .burst_size = DMA_BURST_LEN_1UNITS,
    };
    dma_dev_t dma_uart = {
        .id         = UART1_RX_DMA_ID,
        .addr       = (void *)&HS_UART1->RBR,
        .addr_ctrl  = DMA_ADDR_CTRL_FIX,
        .bus_width  = DMA_SLAVE_BUSWIDTH_8BITS,
        .burst_size = DMA_BURST_LEN_1UNITS,
    };
    dma_block_config_t block_config = {
        .src                 = &dma_uart,
        .dst                 = &dma_mem,
        .block_size_in_bytes = 1,
        .priority            = 0,
        .flow_controller     = DMA_FLOW_CONTROLLER_USE_NONE,
        .intr_en             = false,
    };
    /* dma_build_block_ex(&block[0], &dma_uart, &dma_mem, 1, 0, DMA_FLOW_CONTROLLER_USE_NONE, false); */
    /* dma_build_block_ex(&block[1], &dma_uart, &dma_mem, 3, 0, DMA_FLOW_CONTROLLER_USE_NONE, false); */
    /* dma_build_block_ex(&block[2], &dma_uart, &dma_mem, 5, 0, DMA_FLOW_CONTROLLER_USE_NONE, false); */
    dma_build_block(&block[0], &block_config);
    
    dma_mem.addr = buf[1];
    block_config.block_size_in_bytes = 3;
    dma_build_block(&block[1], &block_config);

    dma_mem.addr = buf[2];
    block_config.block_size_in_bytes = 5;
    dma_build_block(&block[2], &block_config);
    
    dma_append_block(block, block+1);
    dma_append_block(block+1, block+2);
    dma_print_block(block);

    // Init DMA and start transfer.
    dma_init();
    /**
     * 1. DMA Software Ackknowledge must be executed before start using UART1 DMA after manual UART1 transfers, 
     * because UART1 has no DMA enable/disable control, and its DMA interface is always enabled from the start.
     * So any transfer before connecting to DMA module will make the dma_tx/rx_req asserted and keeping, and a
     * dma_tx/rx_ack is required to deassert the corresponding req signal before using DMA.
     * 2. uart.dma_tx_req will always be asserted when the tx fifo is empty, so it can't and there's no need to be cleared.
     * 3. Do not use HS_UART->FCR.bit1 to clear the UART.dma_rx_req signal, cause it reset whole control portion of rx fifo.
     */
    HS_UART1->DMASA = 1;
    HS_DMA_CH_Type *dma_ch = dma_start_transfer(NULL, block, dma_cb);
    if (dma_ch == NULL) {
        log_debug("DMA uart1-to-mem transfer fails.\r\n");
        return;
    }
    dma_wait_stop(dma_ch);
    dma_release(dma_ch);

    // Check result by manual
    print_buf(buf[0], 0, "buf[0]");
    print_buf(buf[1], 0, "buf[1]");
    print_buf(buf[2], 0, "buf[2]");
    
}

static void cmd_dma_mem_to_spi(int argc, char *argv[])
{
    HS_SPI_Type *spi = (strcmp(argv[0], "0") == 0) ? HS_SPI0 : HS_SPI1;
    spi_dma_t spi_dma = {0};
    uint32_t flash_id = 0, dma_flash_id = 0;

    // pinmux config
    if (spi == HS_SPI0) {
        // SPI0
        pinmux_config(PIN_SPI0_MISO, PINMUX_SPI0_MST_SDA_I_CFG);
        pinmux_config(PIN_SPI0_MOSI, PINMUX_SPI0_MST_SDA_O_CFG);
        pinmux_config(PIN_SPI0_CSN,  PINMUX_SPI0_MST_CSN_CFG);
        pinmux_config(PIN_SPI0_SCK,  PINMUX_SPI0_MST_SCK_CFG);

        pmu_pin_mode_set(BITMASK(PIN_SPI0_MISO), PMU_PIN_MODE_PU);
        pmu_pin_mode_set(BITMASK(PIN_SPI0_MOSI), PMU_PIN_MODE_PP);
        pmu_pin_mode_set(BITMASK(PIN_SPI0_CSN),  PMU_PIN_MODE_PP);
        pmu_pin_mode_set(BITMASK(PIN_SPI0_SCK),  PMU_PIN_MODE_PP);
    }
    else {
        // SPI1
        pinmux_config(PIN_SPI1_MISO, PINMUX_SPI1_MST_SDA_I_CFG);
        pinmux_config(PIN_SPI1_MOSI, PINMUX_SPI1_MST_SDA_O_CFG);
        pinmux_config(PIN_SPI1_CSN,  PINMUX_SPI1_MST_CSN_CFG);
        pinmux_config(PIN_SPI1_SCK,  PINMUX_SPI1_MST_SCK_CFG);

        pmu_pin_mode_set(BITMASK(PIN_SPI1_MISO), PMU_PIN_MODE_PU);
        pmu_pin_mode_set(BITMASK(PIN_SPI1_MOSI), PMU_PIN_MODE_PP);
        pmu_pin_mode_set(BITMASK(PIN_SPI1_CSN),  PMU_PIN_MODE_PP);
        pmu_pin_mode_set(BITMASK(PIN_SPI1_SCK),  PMU_PIN_MODE_PP);
    }
    
    // Manual
    sflash_init(spi, NULL);
    flash_id = sflash_read_jedec_id(spi, NULL);
    sflash_deinit(spi, NULL);
    if (flash_id == 0x0 || flash_id == 0xFFFFFFFF) {
        log_debug("Manual read flash id failed.\r\n");
        return;
    }

    // dma
    dma_init();
    sflash_init(spi, &spi_dma);
    dma_flash_id = sflash_read_jedec_id(spi, &spi_dma);
    sflash_deinit(spi, &spi_dma);
    if (dma_flash_id == 0x0 || dma_flash_id == 0xFFFFFFFF || dma_flash_id != flash_id) {
        log_debug("DMA read flash id failed.\r\n");
        return;
    }
}

static void cmd_dma_spi_to_mem(int argc, char *argv[])
{
    cmd_dma_mem_to_spi(argc, argv);
}

/**
 * @brief   dma mem to i2s and i2s to mem tests.
 * @note:   
 *  1. Use audio.i2s_rx.clk as master clock for peripheral.i2s_tx; 
 *     use audio.i2s_tx.clk as master clock for peripheral.i2s_rx.
 *  2. Use peripheral.i2s_tx.sd0 as peripheral.i2s_rx.sdi: 
 *     Connect FPGA_Board.Bluetooth_DEBUG1.Pin9 and Pin10 to loop i2s_tx data out back to i2s_rx data in.
 *  3. Compare rx data with tx data.
 */
#define I2S_DMA_BLOCK_NUM     8
static void cmd_dma_mem_to_i2s(int argc, char *argv[])
{
    (void)argv;
    (void)argc;

    HS_DMA_CH_Type *txdma;
    HS_DMA_CH_Type *rxdma;
    
    pinmux_config(PIN_I2S_SDI,     PINMUX_I2S_SDI_CFG);
    pinmux_config(PIN_I2S_SDO_0,   PINMUX_I2S_SDO_0_CFG);
    /* pinmux_config(PIN_I2S_SDO_1,   PINMUX_I2S_SDO_1_CFG); */
    
    pmu_pin_mode_set(BITMASK(PIN_I2S_SDI),     PMU_PIN_MODE_PD);
    pmu_pin_mode_set(BITMASK(PIN_I2S_SDO_0),   PMU_PIN_MODE_PP);
    /* pmu_pin_mode_set(BITMASK(PIN_I2S_SDO_1),   PMU_PIN_MODE_PP); */
    
    uint16_t txbuf[BUF_SIZE], rxbuf[BUF_SIZE];
    for (int i = 0; i < BUF_SIZE; i++) {
        txbuf[i] = i;
        rxbuf[i] = 0;
    }    

    dma_llip_t txllip[I2S_DMA_BLOCK_NUM], rxllip[I2S_DMA_BLOCK_NUM];

    i2s_config_t i2stxcfg, i2srxcfg;
    i2s_dma_config_t rxdmacfg, txdmacfg;

    
    i2stxcfg.sample_width = I2S_BITWIDTH_16BIT;
    i2stxcfg.pcmmode      = I2S_PCMMODE_STEREO;
    i2stxcfg.sample_rate  = I2S_SAMPLE_8K;
    i2stxcfg.ws_width     = I2S_BITWIDTH_16BIT,
    i2stxcfg.workmode     = I2S_WORKMODE_SLAVE,
    
    i2srxcfg.sample_width = I2S_BITWIDTH_16BIT;
    i2srxcfg.pcmmode      = I2S_PCMMODE_STEREO;
    i2srxcfg.sample_rate  = I2S_SAMPLE_8K;
    i2srxcfg.ws_width     = I2S_BITWIDTH_16BIT,
    i2srxcfg.workmode     = I2S_WORKMODE_SLAVE,
    
    rxdmacfg.use_fifo   = false;
    rxdmacfg.buffer     = rxbuf;
    rxdmacfg.buffer_len = BUF_SIZE;
    rxdmacfg.block_llip = rxllip;
    rxdmacfg.block_num  = I2S_DMA_BLOCK_NUM;
    rxdmacfg.callback   = NULL;

    txdmacfg.use_fifo   = false;
    txdmacfg.buffer     = txbuf;
    txdmacfg.buffer_len = BUF_SIZE;
    txdmacfg.block_llip = txllip;
    txdmacfg.block_num  = I2S_DMA_BLOCK_NUM;
    txdmacfg.callback   = NULL;
    
    audio_config_t config;
    config.pga_gain = AUDIO_MIC_IN_PGA_GAIN_30DB;
    config.sel_pga_input = AUDIO_SELECT_PAG_INPUT_GPIO3;
    config.adc_gain = AUDIO_ADC_GAIN_0DB;
    config.sig_or_diff = AUDIO_EN_PGA_SINGLE_IN_INNER_REF;
    
    /// start audio adc
    audio_open(NULL);
    audio_set_config(&config);
    
    dma_init();

    i2s_rx_config(&i2srxcfg);
    i2s_config(&i2stxcfg);
        
    txdma = i2s_dma_config((HS_DMA_CH_Type *)NULL, &txdmacfg);
    rxdma = i2s_rx_dma_config((HS_DMA_CH_Type *)NULL, &rxdmacfg);

    /// start dm rx
    dma_start_with_lli(rxdma);
    dma_start_with_lli(txdma);

    i2s_rx_start(&i2srxcfg);
    i2s_start();
    i2s_rx_clock_ws_generated_by_codec();
    audio_i2s_data_lineto_i2s_via_gpio();
    
    REGW(&HS_AUDIO->CODEC_CLK_CTRL_1, MASK_1REG(AU_CODEC_RSTN_REG,     1));

    //log_debug("HS_AUDIO->CODEC_CLK_CTRL_1=%08x\r\n", HS_AUDIO->CODEC_CLK_CTRL_1);
    //log_debug("HS_AUDIO->CODEC_CLK_CTRL_2=%08x\r\n", HS_AUDIO->CODEC_CLK_CTRL_2);
    //log_debug("HS_AUDIO->CODEC_CLK_CTRL_3=%08x\r\n", HS_AUDIO->CODEC_CLK_CTRL_3);
    //log_debug("HS_AUDIO->IF_CTRL=%08x\r\n", HS_AUDIO->IF_CTRL);
    //log_debug("HS_PSO->I2S_CFG=%08x\r\n", HS_PSO->I2S_CFG);
    
    dma_wait_stop(rxdma);
    dma_wait_stop(txdma);

    i2s_rx_stop();
    i2s_stop();

    print_buf(txbuf, BUF_SIZE, "i2s.txbuf:");
    print_buf(rxbuf, BUF_SIZE, "i2s.rxbuf:");

    if (memcmp(txbuf, rxbuf, BUF_SIZE) == 0)
        log_debug("dma mem i2s test pass!\r\n");
    else
        log_debug("dma mem i2s test fail!\r\n");
    
}
static void cmd_dma_i2s_to_mem(int argc, char *argv[])
{
    cmd_dma_mem_to_i2s(argc, argv);
}

/**
 * In this example the timer DMA burst feature is used to update the contents of the CCRx
 * registers (x = 2, 3, 4) with the DMA transferring half words into the CCRx registers.
 * This is done in the following steps:
 * 1. Configure the corresponding DMA channel as follows:
 *    -- DMA channel peripheral address is the DMAR register address
 *    -- DMA channel memory address is the address of the buffer in the RAM containing
 *       the data to be transferred by DMA into CCRx registers.
 *    -- Number of data to transfer = 3 (See note below).
 *    -- Circular mode disabled.
 * 2. Configure the DCR register by configuring the DBA and DBL bit fields as follows:
 *    DBL = 2 (3 transfers), DBA = 0xE.
 * 3. Enable the TIMx update DMA request (set the UDE bit in the DIER register).
 * 4. Enable TIMx
 * 5. Enable the DMA channel
 */
#define TIM_TEST_BUF_SIZE       (3*4)
static void cmd_dma_mem_to_tim(int argc, char *argv[])
{
    HS_TIM_Type *tim = HS_TIM0;
    uint32_t tim_id = 0;

    if (strcmp(argv[0], "1") == 0) {
        tim = HS_TIM1;
        tim_id = 1;
    } 
    else if (strcmp(argv[0], "2") == 0) {
        tim = HS_TIM2;
        tim_id = 2;
    }

    log_debug("dma mem tim test: txbuf => tim->CCR[1-3]\r\n");
    log_debug("dma tim mem test: tim->CCR[1-3] => rxbuf\r\n");
    
    /// 0. Prepare data
    uint32_t buf_32bits_aligned[2][TIM_TEST_BUF_SIZE/4];
    for (int i = 0; i < TIM_TEST_BUF_SIZE/4; i++) {
        buf_32bits_aligned[0][i] = (tim_id<<8) + 0xA0u + i + 1;
        buf_32bits_aligned[1][i] = 0;
    }
    
    dma_block_t block[2];
    tim_config_t config = {
        .mode                   = TIM_TIMER_MODE,
        .config.timer.period_us = 10000, /* The counter is blocked when the auto-reload value tim->ARR is null. */
        .config.timer.callback  = NULL,
    };
    tim_dma_config_t txconfig = {
        .mem_to_tim   = true,
        .tim_addr     = &tim->CCR[1],
        .mem_addr     = buf_32bits_aligned[0],
        .len_in_bytes = TIM_TEST_BUF_SIZE,
        .req          = TIM_DIER_UDE | TIM_DIER_TDE,
        .block        = &block[0],
    };
    tim_dma_config_t rxconfig = {
        .mem_to_tim   = false,
        .tim_addr     = &tim->CCR[1],
        .mem_addr     = buf_32bits_aligned[1],
        .len_in_bytes = TIM_TEST_BUF_SIZE,
        .req          = TIM_DIER_UDE | TIM_DIER_TDE,
        .block        = &block[1],
    };

    /// 1. Init and configure TIMx
    tim_init();
    tim_config(tim, &config);
    tim_dma_config(tim, &txconfig);
    tim_dma_config(tim, &rxconfig);

    /// 2. Configure DMA transfer and init.
    dma_append_block(block, block+1);
    dma_print_block(&block[0]);
    dma_init();
    HS_DMA_CH_Type *dma_ch = dma_start_transfer(NULL, &block[0], dma_cb);

    /// 3. Start TIMx
    tim_start(tim); /* tim->CR1 = 1; // Enable TIMx: CR1.CER = 1 */

    /// 4. Wait for dma transfer complete
    dma_wait_stop(dma_ch);
    dma_release(dma_ch);

    /// 5. Result
    print_buf(buf_32bits_aligned[0], TIM_TEST_BUF_SIZE, "tim.txbuf:");
    print_buf(buf_32bits_aligned[1], TIM_TEST_BUF_SIZE, "tim.rxbuf:");
    if (memcmp(buf_32bits_aligned[0], buf_32bits_aligned[1], TIM_TEST_BUF_SIZE) == 0)
        log_debug("dma mem to/from tim tests pass.\r\n");
    else
        log_debug("dma mem to/from tim tests fail.\r\n");
}

static void cmd_dma_tim_to_mem(int argc, char *argv[])
{
    cmd_dma_mem_to_tim(argc, argv);
}

static void cmd_dma_mem_to_i2c(int argc, char *argv[])
{
    (void)argv;
    (void)argc;

    log_debug("Not tested temporarily, due to the fact that the same hard I2C module in HS6621 went die when used heavily, and soft I2c is used instead.\r\n");
}

static void cmd_dma_i2c_to_mem(int argc, char *argv[])
{
    cmd_dma_mem_to_i2c(argc, argv);
}



void cmd_dma(int argc, char *argv[])
{
  	if (argc == 0 || argc == 1) {
print_help:
        log_debug("Usage: dma from to\r\n");
        log_debug("       dma mem  mem:      Move data from memory to memory by dma.\r\n");
        log_debug("       dma adc  mem:      Move data from ADC    to memory by dma.\r\n");
        log_debug("       dma mem  uart x:   Move data from memory to UARTx  by dma, x=1.\r\n");
        log_debug("       dma uart mem  x:   Move data from UARTx  to memory by dma, x=[0,1].\r\n");
        log_debug("       dma mem  spi  x:   Move data from memory to SPIx   by dma, x=[0,1].\r\n");
        log_debug("       dma spi  mem  x:   Move data from SPIx   to memory by dma, x=[0,1].\r\n");
        log_debug("       dma mem  tim  x:   Move data from memory to TIMx   by dma, x=[0,1].\r\n");
        log_debug("       dma tim  mem  x:   Move data from TIMx   to memory by dma, x=[0,1].\r\n");
        log_debug("       dma mem  i2c  x:   Move data from memory to I2Cx   by dma, x=[0,1,2].\r\n");
        log_debug("       dma i2c  mem  x:   Move data from I2Cx   to memory by dma, x=[0,1,2].\r\n");
        log_debug("       dma mem  i2s:      Move data from memory to I2S    by dma.\r\n");
        log_debug("       dma i2s  mem:      Move data from I2S    to memory by dma.\r\n");
        log_debug("                          Note: Connect FPGA_Board.Bluetooth_DEBUG1.Pin9 and Pin10 to\r\n");
        log_debug("                                loop i2s_tx data out back to i2s_rx data in.\r\n");
        log_debug("\r\n");
        log_debug("Peripherals supported by DMA: \r\n");
        log_debug("       mem (ram only), uart 1, spi 0/1, i2c 0/1/2, i2s, adc, timer 0/1/2. \r\n");
        log_debug("\r\n");
        log_debug("Example: \r\n");
        log_debug("       dma mem mem\r\n");
        log_debug("       dma adc mem\r\n");
        log_debug("       dma mem i2s\r\n");
        log_debug("       dma mem uart 1\r\n");
        log_debug("       dma spi mem 0\r\n");
        log_debug("       dma tim mem 0\r\n");

        return;
    }

    else if (argc == 2) {
        if (strcmp(argv[0], "rom") == 0 && strcmp(argv[1], "ram") == 0) {
            cmd_dma_rom_to_ram(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "mem") == 0 && strcmp(argv[1], "mem") == 0) {
            cmd_dma_mem_to_mem(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "adc") == 0 && strcmp(argv[1], "mem") == 0) {
            cmd_dma_adc_to_mem(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "mem") == 0 && strcmp(argv[1], "i2s") == 0) {
            cmd_dma_mem_to_i2s(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "i2s") == 0 && strcmp(argv[1], "mem") == 0) {
            cmd_dma_i2s_to_mem(argc-2, argv+2);
        }
    	else
            goto print_help;

    }
    else if (argc == 3) {
        if (strcmp(argv[0], "mem") == 0 && strcmp(argv[1], "uart") == 0) {
            cmd_dma_mem_to_uart(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "uart") == 0 && strcmp(argv[1], "mem") == 0) {
            cmd_dma_uart_to_mem(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "mem") == 0 && strcmp(argv[1], "spi") == 0) {
            cmd_dma_mem_to_spi(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "spi") == 0 && strcmp(argv[1], "mem") == 0) {
            cmd_dma_spi_to_mem(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "mem") == 0 && strcmp(argv[1], "tim") == 0) {
            cmd_dma_mem_to_tim(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "tim") == 0 && strcmp(argv[1], "mem") == 0) {
            cmd_dma_tim_to_mem(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "mem") == 0 && strcmp(argv[1], "i2c") == 0) {
            cmd_dma_mem_to_i2c(argc-2, argv+2);
        }
        else if (strcmp(argv[0], "i2c") == 0 && strcmp(argv[1], "mem") == 0) {
            cmd_dma_i2c_to_mem(argc-2, argv+2);
        }
        else
            goto print_help;

    }
	else
		goto print_help;

  
}

/*********************************************************************/
#endif
#endif
#endif

/** @} */

