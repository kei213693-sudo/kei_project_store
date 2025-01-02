 #include <stdint.h>
#include <string.h>
#include "pinmux.h"
#include "peripheral.h"
#include "glcd_kd.h"
#include "glcd.h"
#include "FreeRTOS.h"
#include "task.h"

#define LCD_CSN_PIN 45
#define LCD_CLK_PIN 44
#define LCD_SDA_PIN 11
#define LCD_DA1_PIN 43
#define LCD_DA2_PIN 48 
#define LCD_DA3_PIN 13
#define LCD_RST_PIN 47

#define GLCD_MAX_DATA_BYTES     (454*72*2)

typedef struct {
    volatile uint8_t gspi_finish;               // 0: gspi is busy, 1: gspi is idle (finish)
    volatile uint8_t flush_finish;              // 0: flush is busy, 1: flush is idle (finish)
    volatile uint8_t display_buffer_increase;   // 1: display buffer is increasing, 1: display buffer is stable
    uint8_t* display_buffer;                    // function glcd_disp_flush get user buffer stored in this variable
    uint32_t display_number_bytes;              // function glcd_disp_flush get user buffer length stored in this variable
    uint32_t max_number_bytes;                  // function glcd_write_memory once write max_number_bytes data to gspi
    uint8_t* clear_data_buffer;                 // function glcd_clear use this malloc buffer to clear display
    glcd_cb_t gc9c01_cb;                        // user callback function
} gc9c01_env_t;

static gc9c01_env_t gc9c01_env = {
    .gspi_finish = 1,
    .flush_finish = 1,
    .gc9c01_cb = NULL,
    .display_buffer = NULL,
    .display_buffer_increase = 0,
    .display_number_bytes = 0,
    .max_number_bytes = GLCD_MAX_DATA_BYTES,
    .clear_data_buffer = NULL,
};


const lcd_config_t lcd_cfg = {
    .line_mode = SFB_LCD_4LANE,
    .rgb_mode = SFB_LCD_RGB565,
    .wire_mode =SFB_LCD_3WIRE_MODE1,
    .swap_mode = SFB_LCD_SWAP_NONE,
    .rw_mode = SFB_LCD_WRITE,
};

const sfb_config_t sf_ctrl_cfg[] = {
    { 16000000, 0, SFB_SPI_MODE_0, SFB_SPI_CS_LOW_ACTIVE},
    { 32000000, 0, SFB_SPI_MODE_0, SFB_SPI_CS_LOW_ACTIVE},
    { 64000000, 0, SFB_SPI_MODE_0, SFB_SPI_CS_LOW_ACTIVE},
};

//static void gc9c01_callback(glcd_cb_t* cb)
//{
//    gc9c01_env.gspi_finish = 1;

//    if(gc9c01_env.display_number_bytes > 0) {   // still have data to transfer

//        uint32_t current_send_bytes = gc9c01_env.display_number_bytes > gc9c01_env.max_number_bytes? gc9c01_env.max_number_bytes : gc9c01_env.display_number_bytes;
//        glcd_write_memory(0x3C, gc9c01_env.display_buffer, current_send_bytes);   // call glcd_write_memory will reset cs.
//        gc9c01_env.display_number_bytes -= current_send_bytes;
//        if(gc9c01_env.display_buffer_increase) {
//            gc9c01_env.display_buffer += current_send_bytes;
//        }
//    } else {
//        if(gc9c01_env.gc9c01_cb) {          // run gc9c01 call back if registered
//            gc9c01_env.gc9c01_cb();
//        }
//        if(gc9c01_env.clear_data_buffer) {  // run clear data buffer if glcd_clear
//            om_mem_free(OM_MEM_DMA, gc9c01_env.clear_data_buffer);
//            gc9c01_env.clear_data_buffer = NULL;
//        }
//        gc9c01_env.flush_finish = 1;        // flush finish
//    }
//}

static void gc9c01_callback(glcd_cb_t* cb)
{
    gc9c01_env.gspi_finish = 1;

    if(gc9c01_env.gc9c01_cb) {          // run gc9c01 call back if registered
        gc9c01_env.gc9c01_cb();
    }
}

static void glcd_hardware_init()
{
    pinmux_config(LCD_RST_PIN, PINMUX_GPIO_MODE_CFG);
    gpio_set_direction_ex(BITMASK(15), GPIO_OUTPUT);
    
    pinmux_config(LCD_CSN_PIN, PINMUX_SFLASH2_CSN_1_CFG);
    pinmux_config(LCD_CLK_PIN, PINMUX_SFLASH2_CK_CFG);
    pinmux_config(LCD_SDA_PIN, PINMUX_SFLASH2_SI_CFG);
    pinmux_config(LCD_DA1_PIN, PINMUX_SFLASH2_SO_CFG);
    pinmux_config(LCD_DA2_PIN, PINMUX_SFLASH2_WP_CFG);
    pinmux_config(LCD_DA3_PIN, PINMUX_SFLASH2_HD_CFG);

    //5ma, 48mhz
    pmu_pin_driven_current_set_ex(BIT_MASK(LCD_CLK_PIN-32),PMU_PIN_DRIVEN_CURRENT_12MA);
    pmu_pin_driven_current_set(BIT_MASK(LCD_SDA_PIN),PMU_PIN_DRIVEN_CURRENT_12MA);
    pmu_pin_driven_current_set_ex(BIT_MASK(LCD_DA1_PIN-32),PMU_PIN_DRIVEN_CURRENT_12MA);
    pmu_pin_driven_current_set_ex(BIT_MASK(LCD_DA2_PIN-32),PMU_PIN_DRIVEN_CURRENT_12MA);
    pmu_pin_driven_current_set(BIT_MASK(LCD_DA3_PIN),PMU_PIN_DRIVEN_CURRENT_12MA);

    pinmux_config(LCD_RST_PIN, PINMUX_GPIO_MODE_CFG);
}

static void glcd_reset()
{
    gpio_write_ex(BITMASK(15), GPIO_HIGH);
    co_delay_ms(60);
    gpio_write_ex(BITMASK(15), GPIO_LOW);
    co_delay_ms(60);
    gpio_write_ex(BITMASK(15), GPIO_HIGH);
    co_delay_ms(60);
}

static void glcd_sequence_init()
{
    HS_SF_Type *sf = HS_SF2;
    uint32_t cs = 1;

    uint32_t reg = 0;
    uint32_t reg1 = 0;

    uint8_t buf[200];
    uint8_t * const start = buf;
    uint8_t *p = buf;

    glcd_reset();

    reg = 0xC0;
    p = start;
    *p++ = 0x5A;
    *p++ = 0x5A;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0xC1;
    p = start;
    *p++ = 0x5A;
    *p++ = 0x5A;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0x11;
    p = start;
    *p++ = NULL;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(10);


//    reg = 0x11;
//    lcd_write_reg_4lines(sf, cs, reg, NULL, 8);
//    co_delay_ms(10);

    reg = 0x3A;
    p = start;
    *p++ = 0x75;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0x2A;
    p = start;
    *p++ = 0x00;
    *p++ = 0x00;
    *p++ = 0x01;
    *p++ = 0xC5;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0x2B;
    p = start;
    *p++ = 0x00;
    *p++ = 0x00;
    *p++ = 0x01;
    *p++ = 0xC5;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0x44;
    p = start;
    *p++ = 0x01;
    *p++ = 0xC2;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0x35;
    p = start;
    *p++ = 0x00;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0xB0;
    p = start;
    *p++ = 0x16;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0xB1;
    p = start;
    *p++ = 0x01;
    *p++ = 0x05;
    *p++ = 0x00;
    *p++ = 0xA2;
    *p++ = 0x00;
    *p++ = 0xA7;
    *p++ = 0x00;
    *p++ = 0xA7;
    *p++ = 0x00;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0x53;
    p = start;
    *p++ = 0x28;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0xC4;
    p = start;
    *p++ = 0x84;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(25);

    reg = 0x29;
    p = start;
    *p++ = NULL;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

//    reg = 0x29;
//    lcd_write_reg_4lines(sf, cs, reg, NULL, 8);
//    co_delay_ms(1);


    reg = 0xBA;
    p = start;
    *p++ = 0x80;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0xB1;
    p = start;
    *p++ = 0xC0;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0xC0;
    p = start;
    *p++ = 0xA5;
    *p++ = 0xA5;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0xC1;
    p = start;
    *p++ = 0xA5;
    *p++ = 0xA5;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
    co_delay_ms(1);

    reg = 0x00;
    p = start;
    *p++ = NULL;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);

//    reg = 0x00;
//    lcd_write_reg_4lines(sf, cs, reg, NULL, 8);

    reg = 0x00;
    p = start;
    *p++ = NULL;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);
}


void glcd_init()
{
    HS_SF_Type *sf = HS_SF2;
    uint32_t cs = 1;
    glcd_hardware_init();
    glcd_reset();

    lcd_open(sf);                                           // rom function
    lcd_config(sf, cs, &sf_ctrl_cfg[2], &lcd_cfg);          // rom function
    
    glcd_sequence_init();
    sfb_dma_done_event_register(sf, (void*)gc9c01_callback);
}

void glcd_set_disp_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    HS_SF_Type *sf = HS_SF2;
    uint32_t cs = 1;
    uint32_t reg;
    uint32_t reg1;

    reg = 0x2a;
    reg1 = (x1<<16) | x2;
    lcd_write_reg_4lines(sf, cs, 0xff, reg1, 8);
    lcd_write_reg_4lines(sf, cs, reg, reg1, 64);

    reg = 0x2b;
    reg1 = (y1<<16) | y2;
    while(!gc9c01_env.gspi_finish);
    lcd_write_reg_4lines(sf, cs, reg, reg1, 64);
}

void glcd_disp_flush(const void *color, uint32_t color_len)
{
    HS_SF_Type *sf = HS_SF2;
    uint32_t cs = 1;
    while(!gc9c01_env.gspi_finish);
    gc9c01_env.gspi_finish = 0;
    lcd_write_mem_ex_4lines(sf, cs, SFB_CS_NOKEEP, 0x2c, color, color_len);
}

void glcd_disp_flush_finish()
{
    while(!gc9c01_env.gspi_finish);
}

void glcd_clear(uint32_t color)
{
    static uint16_t clear_buff[454];
    
    for(uint32_t i=0; i<454; i++){
        clear_buff[i] = color;
    }
    for(uint32_t i=0; i<454; i++) {
        glcd_set_disp_window(0, i, 454-1, i+1);
        glcd_disp_flush(clear_buff, 454*2);
    }
}

void glcd_register_cb(glcd_cb_t cb)
{
    gc9c01_env.gc9c01_cb = cb;
}