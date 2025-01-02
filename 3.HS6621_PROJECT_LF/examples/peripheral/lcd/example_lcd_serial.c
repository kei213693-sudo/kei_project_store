/**
 * @file example_lcd_serial.c
 * @brief example for lcd serial
 * @date 2019/07/12 17:53:27
 * @author liqiang
 *
 * @defgroup EXAMPLE_LCD LCD Example
 * @ingroup EXAMPLE
 * @brief example for LCD
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

/*********************************************************************
 * STATIC FUNCTIONS
 */

static volatile bool lcd_dma_done;

static void lcd_dma_done_handler(HS_SF_Type *sf)
{
    lcd_dma_done = true;
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_lcd_serial(void)
{
    uint16_t x1 = 0, y1 = 0, x2 = 240 - 1, y2 = 320 - 1;
    CO_ALIGN(4) uint8_t d[32];

    sfb_config_t config = {
        .freq_hz = 32*1000*1000,
        .delay = 0,
        .transmode = SFB_SPI_MODE_0,
        .cs_pol = SFB_SPI_CS_LOW_ACTIVE,
    };

#ifdef CONFIG_SF_XJF_DESIGN
    lcd_config_t lcdconfig = {
        .rw_mode = LCD_WRITE,
        .line_mode = LCD_2LANE,
        .rgb_mode = LCD_RGB565,
        .wire_mode = LCD_3WIRE_MODE2,
        .swap_mode = LCD_SWAP_LEN_16BITS,
    };
#else
    sfb_lcd_config_t lcdconfig = {
        .rw_mode = SFB_LCD_WRITE,
        .rgb_mode = SFB_LCD_RGB565,
        .rgb_transmode = SFB_LCD_2LANE_1PIXLE_1TRANS,
        .wire_mode = SFB_LCD_9BITS_WITH_DCX,
    };
#endif

    // pinmux
    pinmux_config(7, PINMUX_LCD_DCX_RS_CFG);
    pinmux_config(8, PINMUX_LCD_CSN_CFG);
    pinmux_config(9, PINMUX_LCD_SCL_CFG);
    pinmux_config(19, PINMUX_LCD_SDA_CFG);

    // open
    lcd_open(HS_SF1);

    // config
    lcd_config(HS_SF1, 0, &config, &lcdconfig);
    // No block for lcd_write_mem/lcd_write_mem_ex
    lcd_dma_done_event_register(HS_SF1, lcd_dma_done_handler);

    // Sleep Out
    lcd_write_reg(HS_SF1, 0, 0x11, NULL, 0);
    co_delay_ms(5);

    // Interface Pixel Format
    d[0] = 0x55;
    lcd_write_reg(HS_SF1, 0, 0x3A, d, 1);

    // SPI2 Enable
    d[0] = 0x10;
    lcd_write_reg(HS_SF1, 0, 0xE7, d, 1);

    // Display On
    lcd_write_reg(HS_SF1, 0, 0x29, NULL, 0);

    // Column Address Set
    d[0] = x1>>8; d[1] = x1>>0; d[2] = x2>>8; d[3] = x2>>0;
    lcd_write_reg(HS_SF1, 0, 0x2A, d, 4);

    // Row Address Set
    d[0] = y1 >> 8; d[1] = y1 >> 0; d[2] = y2 >> 8; d[3] = y2 >> 0;
    lcd_write_reg(HS_SF1, 0, 0x2B, d, 4);

    while(1)
    {
        CO_ALIGN(4) static uint16_t screen_buf[240*320/32];
        const uint16_t colors[] = {0xF800/*red*/, 0xFFE0/*yellow*/, 0x07E0/*green*/, 0x001F/*blue*/, 0x07FF/*cyan*/, 0x780F/*purple*/};

        for(int i=0; i<sizeof(colors)/2; ++i)
        {
            for(int j=0; j<sizeof(screen_buf)/2; ++j)
                screen_buf[j] = colors[i];

            lcd_dma_done = false;
            lcd_write_mem_ex(HS_SF1, 0, SFB_CS_BEGIN, 0x2c, screen_buf, sizeof(screen_buf));
            while(!lcd_dma_done);

            for(int k=0; k<30; ++k)
            {
                lcd_dma_done = false;
                lcd_write_mem_ex(HS_SF1, 0, SFB_CS_KEEP, 0x2c, screen_buf, sizeof(screen_buf));
                while(!lcd_dma_done);
            }

            lcd_dma_done = false;
            lcd_write_mem_ex(HS_SF1, 0, SFB_CS_END, 0x2c, screen_buf, sizeof(screen_buf));
            while(!lcd_dma_done);

            co_delay_ms(1000);
        }
    }
}

/** @} */

