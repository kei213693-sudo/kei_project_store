#ifndef __FF_SD_CONFIG_H
#define __FF_SD_CONFIG_H

//#include "board_OM662x.h"
//#include "om662x_driver.h"
//#include "components.h"
#include "cmsis_os.h"
#include "board.h"
#include "driver.h"

#define SD_SPI      OM_SPI0

#define SD_SCLK                             22
#define SD_MOSI                             28
#define SD_MISO                             26
#define SD_CS                               27


#define SD_SPI_LOW_SPEED            SD_SPI->CTRL |= 0x00FF
#define SD_SPI_HIGH_SPEED           SD_SPI->CTRL &= 0xFFFFFF00



#define SD_CS_SET                       (OM_GPIO0->DATAOUT = OM_GPIO0->DATAOUT | (1 << SD_CS))
#define SD_CS_CLR                       (OM_GPIO0->DATAOUT = OM_GPIO0->DATAOUT & (~(1 << SD_CS)))
#define SD_CS_MODE_OUTPUT               (OM_GPIO0->OUTENSET = (1UL << SD_CS))



__STATIC_FORCEINLINE void SD_SpiInit(void){
    const pin_config_t spi_sck_PinConfig = {
        .port = 0,
        .pin = SD_SCLK,
        .func = PINMUX_SPI0_MST_SCK_CFG,
        .mode = PIN_MODE_PP,
        .drv = PIN_DRIVER_CURRENT_MAX,
    };

   const pin_config_t spi_mosi_PinConfig = {
        .port = 0,
        .pin  = SD_MOSI,
        .func = PINMUX_SPI0_MST_SDA_O_CFG,
        .mode = PIN_MODE_PP,
        .drv = PIN_DRIVER_CURRENT_MAX,
    };
    const pin_config_t spi_miso_PinConfig = {
        .port = 0,
        .pin  = SD_MISO,
        .func = PINMUX_SPI0_MST_SDA_I_CFG,
        .mode = PIN_MODE_PP,
        .drv = PIN_DRIVER_CURRENT_MAX,
    };

    const pin_config_t gpioPinConfig = {
        .port   = 0,
        .pin    = SD_CS,
        .func   =  PINMUX_GPIO_MODE_CFG,
        .mode   = PIN_MODE_PP,
        .drv = PIN_DRIVER_CURRENT_MAX,
    };
    pin_init(&gpioPinConfig);
    pin_init(&spi_sck_PinConfig);
    pin_init(&spi_mosi_PinConfig);
    pin_init(&spi_miso_PinConfig);

    SD_CS_MODE_OUTPUT;
    RCC_CLK_EN_SPI0();
    OM_SPI0->CTRL=0xc233007F;
    SD_CS_SET;
}






#endif  /* FF_SD_CONFIG_H    */
