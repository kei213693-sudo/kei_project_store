#include <stdint.h>
#include <string.h>
#include "pinmux.h"
#include "peripheral.h"
#include "glcd_kd.h"
#include "glcd.h"
#include "FreeRTOS.h"
#include "task.h"

#if 1
#define LCD_CSN_PIN 45
#define LCD_CLK_PIN 44
#define LCD_SDA_PIN 11
#define LCD_DA1_PIN 43
#define LCD_DA2_PIN 48 
#define LCD_DA3_PIN 13
#define LCD_RST_PIN 47
#endif

#define GPIO_TOGGLE_PIN    8
#define TEST_PIN 8//9
extern signed short pdwSampleX;
uint8_t picture_id = 1;
signed short pdwSampleX_save;
extern uint32_t TP_int;



#define LCD_COLOR_WHITE 0xFF    /* white color */
#define LCD_COLOR_BLACK 0x00    /* black color */

//#define GLCD_X_RESOLUTION   454     /* column count*/
//#define GLCD_Y_RESOLUTION      454     /* row count*/

#define RGB565_ONEPIXEL_BYTES   (2)     /* the bytes of one pixel */

#define RGB565_FULLSCREEN_BYTES (GLCD_X_RESOLUTION*GLCD_Y_RESOLUTION*RGB565_ONEPIXEL_BYTES) /* hthe bytes of full screen*/

#define RGB565_LINE_BYTES       (454*RGB565_ONEPIXEL_BYTES) /* the bytes of row*/

/* the pic address in the psram? */
#define PICTURE_START_ADDR_IN_PSRAM    0x60000000     //0x00     //raw   //
#define PICTURE_1_START_ADDR_IN_PSRAM  PICTURE_START_ADDR_IN_PSRAM
#define PICTURE_2_START_ADDR_IN_PSRAM  (PICTURE_1_START_ADDR_IN_PSRAM+454*454*2)
#define PICTURE_3_START_ADDR_IN_PSRAM  (PICTURE_2_START_ADDR_IN_PSRAM+454*454*2)

#define LINE_BYTES RGB565_LINE_BYTES     //454*2
#define FULLSCREEN_LINE (RGB565_FULLSCREEN_BYTES/RGB565_LINE_BYTES)
#define FULLSCREEN_BYTES RGB565_FULLSCREEN_BYTES
#define RGB_ONEPIXEL_BYTES RGB565_ONEPIXEL_BYTES

/* colors data value----RGB565 */
const uint16_t colors[] = {0xFFE0/*red*/, 0xF800/*yellow*/, 0x07E0/*green*/, 0x001F/*blue*/, 0x3333/*purple*/,0x0780F}; //  0xF800/*red*/, 0xFFE0/*yellow*/, 0x07E0/*green*/, 0x001F/*blue*/, 0x07FF/*cyan*/, 0x780F/*purple*/


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

static volatile bool lcd_dma_done = 0;

void lcd_dma_done_callback(void)
{
    lcd_dma_done = true;
    //gpio_write(BITMASK(TEST_PIN),GPIO_HIGH);
    //log_debug("AA \n");
}

 void glcd_set_disp_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
		HS_SF_Type *sf = HS_SF2;
	  uint32_t cs = 1;
    uint32_t reg;
    uint32_t reg1;
#if 1
		reg = 0x2a;	//Column address set command in sf->COMMAND_DATA0_REG[0]
    //reg1 = ((x1&0xFFFF)<<16) | (x2&0xFFFF);
	  reg1 = (x1<<16) | x2;
    lcd_write_reg_4lines(sf, cs, 0xff, reg1, 8);
	  lcd_write_reg_4lines(sf, cs, reg, reg1, 64);
#endif
    reg = 0x2b; //Row address set command in sf->COMMAND_DATA0_REG[0]
    reg1 = (y1<<16) | y2;
    lcd_write_reg_4lines(sf, cs, reg, reg1, 64);
	#if 0
		reg = 0x2a;	//Column address set command in sf->COMMAND_DATA0_REG[0]
    //reg1 = ((x1&0xFFFF)<<16) | (x2&0xFFFF);
	  reg1 = (x1<<16) | x2;
    lcd_write_reg_4lines(sf, cs, reg, reg1, 64);
	#endif
}

void glcd_disp_flush(const void *color, uint32_t color_len)
{
		HS_SF_Type *sf = HS_SF2;
	  uint32_t cs = 1;
	  //lcd_write_reg_4lines(sf, cs, reg, reg1, 64);
	  taskENTER_CRITICAL();
	  lcd_write_mem_ex_4lines(sf, cs, SFB_CS_BEGIN, 0x2c, color, color_len);
    //lcd_write_mem(lcd_sf_x, 0, 0x2C, color, color_len);
	  taskEXIT_CRITICAL();
}

 void glcd_clear(uint8_t color)
 {
	 uint16_t i = 0;
	 HS_SF_Type *sf = HS_SF2;
	 uint32_t cs = 1;
#pragma pack(4)
	 uint8_t buf[LINE_BYTES];
#pragma pack()
 
	 memset(buf, color, sizeof(buf));
 
	 glcd_set_disp_window(0, 0, GLCD_X_RESOLUTION-1, GLCD_Y_RESOLUTION-1);
 
	 for(i = 0; i < FULLSCREEN_LINE; i++)
	 {
		 lcd_write_mem_ex_4lines(sf, cs, SFB_CS_BEGIN, 0x2c, buf, sizeof(buf));
	 }
 }
 
  void glcd_test(uint8_t color)
 {
	 uint16_t i = 0;
	 HS_SF_Type *sf = HS_SF2;
	 uint32_t cs = 1;
#pragma pack(4)
	 uint8_t buf[50*50*2];
#pragma pack()
 
	 memset(buf, color, sizeof(buf));
 
	 glcd_set_disp_window(100, 100, 150-1, 150-1);
 
	 lcd_write_mem_ex_4lines(sf, cs, SFB_CS_BEGIN, 0x2c, buf, sizeof(buf));
 }
 void lcd_kd_init(HS_SF_Type *sf, uint32_t cs)
{
	uint32_t reg = 0;
	uint32_t reg1 = 0;

	uint8_t buf[200];
	uint8_t * const start = buf;
	uint8_t *p = buf;

#if 1   
    gpio_write_ex(BITMASK(15), GPIO_HIGH);
    co_delay_ms(60);
    gpio_write_ex(BITMASK(15), GPIO_LOW);
    co_delay_ms(60);
    gpio_write_ex(BITMASK(15), GPIO_HIGH);
    co_delay_ms(60);
#endif
#if 0   
    gpio_write(BITMASK(LCD_RST_PIN), GPIO_HIGH);
    co_delay_ms(60);
    gpio_write(BITMASK(LCD_RST_PIN), GPIO_LOW);
    co_delay_ms(60);
    gpio_write(BITMASK(LCD_RST_PIN), GPIO_HIGH);
    co_delay_ms(60);
#endif   

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


//	reg = 0x11;
//	lcd_write_reg_4lines(sf, cs, reg, NULL, 8);
//	co_delay_ms(10);

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

//	reg = 0x29;
//	lcd_write_reg_4lines(sf, cs, reg, NULL, 8);
//	co_delay_ms(1);


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

//	reg = 0x00;
//	lcd_write_reg_4lines(sf, cs, reg, NULL, 8);

    reg = 0x00;
    p = start;
    *p++ = NULL;
    lcd_write_reg_and_data_4lines(sf, cs, reg, start, p-start);

//	reg = 0x00;
//	lcd_write_reg_4lines(sf, cs, reg, NULL, 8);
}

void lcd_hw_cfg(HS_SF_Type *sf, uint32_t cs)
{
    /* Cfg the LCD_RST_PIN to be mode pu */
    /* Cfg the LCD_RST_PIN to be mode pu */
    pinmux_config(LCD_RST_PIN, PINMUX_GPIO_MODE_CFG);
//	pinmux_config(LCD_DCX, PINMUX_GPIO_MODE_CFG);
//	pinmux_config(LCD_TE, PINMUX_GPIO_MODE_CFG);
    gpio_set_direction_ex(BITMASK(15), GPIO_OUTPUT);
//	gpio_set_direction(BITMASK(LCD_DCX), GPIO_OUTPUT);
//	gpio_set_direction(BITMASK(LCD_TE), GPIO_OUTPUT);
    if( HS_SF1 == sf )
    {
        cs = 0; /* pinmux support only cs0 */
        pinmux_config(LCD_CSN_PIN, PINMUX_SFLASH1_CSN_CFG); 
        pinmux_config(LCD_CLK_PIN, PINMUX_SFLASH1_CK_CFG);
        pinmux_config(LCD_SDA_PIN, PINMUX_SFLASH1_SI_CFG);
        pinmux_config(LCD_DA1_PIN, PINMUX_SFLASH1_SO_CFG);
        
        pinmux_config(LCD_DA2_PIN, PINMUX_SFLASH1_WP_CFG);
        pinmux_config(LCD_DA3_PIN, PINMUX_SFLASH1_HD_CFG);
    }
    else if( HS_SF2 == sf ) 
    {
        if( cs )
        {
            pinmux_config(LCD_CSN_PIN, PINMUX_SFLASH2_CSN_1_CFG);
        }
        else
        {
            pinmux_config(LCD_CSN_PIN, PINMUX_SFLASH2_CSN_CFG);
        }
        pinmux_config(LCD_CLK_PIN, PINMUX_SFLASH2_CK_CFG);
        pinmux_config(LCD_SDA_PIN, PINMUX_SFLASH2_SI_CFG);
        pinmux_config(LCD_DA1_PIN, PINMUX_SFLASH2_SO_CFG);
        
        pinmux_config(LCD_DA2_PIN, PINMUX_SFLASH2_WP_CFG);
        pinmux_config(LCD_DA3_PIN, PINMUX_SFLASH2_HD_CFG);
    }

    //5ma, 48mhz
    pmu_pin_driven_current_set_ex(BIT_MASK(LCD_CLK_PIN-32),PMU_PIN_DRIVEN_CURRENT_12MA);
    pmu_pin_driven_current_set(BIT_MASK(LCD_SDA_PIN),PMU_PIN_DRIVEN_CURRENT_12MA);
    pmu_pin_driven_current_set_ex(BIT_MASK(LCD_DA1_PIN-32),PMU_PIN_DRIVEN_CURRENT_12MA);
    pmu_pin_driven_current_set_ex(BIT_MASK(LCD_DA2_PIN-32),PMU_PIN_DRIVEN_CURRENT_12MA);
    pmu_pin_driven_current_set(BIT_MASK(LCD_DA3_PIN),PMU_PIN_DRIVEN_CURRENT_12MA);

    pinmux_config(LCD_RST_PIN, PINMUX_GPIO_MODE_CFG);
}

void lcd_hw_reset(void)
{
#if 1   
    gpio_write_ex(BITMASK(15), GPIO_HIGH);
    co_delay_ms(60);
    gpio_write_ex(BITMASK(15), GPIO_LOW);
    co_delay_ms(60);
    gpio_write_ex(BITMASK(15), GPIO_HIGH);
    co_delay_ms(60);
#endif
#if 0   
    gpio_write(BITMASK(LCD_RST_PIN), GPIO_HIGH);
    co_delay_ms(60);
    gpio_write(BITMASK(LCD_RST_PIN), GPIO_LOW);
    co_delay_ms(60);
    gpio_write(BITMASK(LCD_RST_PIN), GPIO_HIGH);
    co_delay_ms(60);
#endif   
}

//typedef void (*sfb_callback_t)(HS_SF_Type *sf);
static lcd_cb_t glcd_cb = NULL;

void glcd_callback(HS_SF_Type *sf) {
    if(glcd_cb) {
        glcd_cb();
    }
}

void glcd_register_cb(lcd_cb_t cb)
{
    HS_SF_Type *sf = HS_SF2;

    glcd_cb = cb;
}

void glcd_init()
{
    HS_SF_Type *sf = HS_SF2;
    uint32_t cs = 1;
    /* Config Lcd pin */
    lcd_hw_cfg(sf,cs);

    /* Reset Lcd pin */
    lcd_hw_reset();

    /* Open Lcd  */
    lcd_open(sf);
    lcd_config(sf,cs, &sf_ctrl_cfg[2], &lcd_cfg);

    /* Init Lcd kd */
    lcd_kd_init(sf,cs);

    glcd_cb = NULL;
    //lcd_register_event_callback(sf, glcd_callback);
    sfb_dma_done_event_register(sf, glcd_callback);
}

