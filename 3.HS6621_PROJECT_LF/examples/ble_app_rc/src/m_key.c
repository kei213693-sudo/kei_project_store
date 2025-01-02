#include "rwip_config.h" // RW SW configuration

#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include "rwip.h"      // RW SW initialization
#include "dbg.h"
#include "peripheral.h"

#include "m_key.h"
#include "m_led.h"
#include "key_process.h"

#if CONFIG_KEY_ENABLE

#define KPP_SCAN_TIME                    10    //Column select and row scan, unit:us
#define KPP_SCAN_INTERVAL                8     //Keys scan interval, unit:ms

static uint32_t key_row_pins_mask;
static uint32_t key_col_pins_mask;
#if 0 //#ifndef CONFIG_GPIO1_ABSENT
static uint32_t key_row_pins_ex_mask;
static uint32_t key_col_pins_ex_mask;
#endif
/**
* @brief Get the row and column pins mask and quantity.
**/
static void row_col_init(void)
{
    uint8_t pin_key_row[8] = {PIN_KEY_ROW_0, PIN_KEY_ROW_1, PIN_KEY_ROW_2, PIN_KEY_ROW_3, 
                              PIN_KEY_ROW_4, PIN_KEY_ROW_5, PIN_KEY_ROW_6, PIN_KEY_ROW_7};
    uint8_t pin_key_col[8] = {PIN_KEY_COL_0, PIN_KEY_COL_1, PIN_KEY_COL_2, PIN_KEY_COL_3, 
                              PIN_KEY_COL_4, PIN_KEY_COL_5, PIN_KEY_COL_6, PIN_KEY_COL_7};
    
    for(uint8_t i=0; i<8; i++)
    {
        if(pin_key_row[i] < 32)
        {
            key_row_pins_mask |= BIT_MASK(pin_key_row[i]);
        }
        #if 0 //#ifndef CONFIG_GPIO1_ABSENT
        else if(pin_key_row[i] < 0xFF)
        {
            key_row_pins_ex_mask |= BIT_MASK(pin_key_row[i]-32);
        }
        #endif
        
        if(pin_key_col[i] < 32)
        {
            key_col_pins_mask |= BIT_MASK(pin_key_col[i]);
        }
        #if 0 //#ifndef CONFIG_GPIO1_ABSENT
        else if(pin_key_row[i] < 0xFF)
        {
            key_col_pins_ex_mask |= BIT_MASK(pin_key_col[i]-32);
        }
        #endif
    }
    
//  log_debug("row pins mask: 0x%08x, column pins mask: 0x%08x.\n", key_row_pins_mask, key_col_pins_mask);
}


/**
* @brief Mux configuration of pins, which used to keyboard.
**/
static void key_pinmux_init(void)
{
    // column
#if (PIN_KEY_COL_0<0xFF)
    pinmux_config(PIN_KEY_COL_0,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_COL_1<0xFF)
    pinmux_config(PIN_KEY_COL_1,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_COL_2<0xFF)
    pinmux_config(PIN_KEY_COL_2,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_COL_3<0xFF)
    pinmux_config(PIN_KEY_COL_3,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_COL_4<0xFF)
    pinmux_config(PIN_KEY_COL_4,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_COL_5<0xFF)
    pinmux_config(PIN_KEY_COL_5,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_COL_6<0xFF)
    pinmux_config(PIN_KEY_COL_6,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_COL_7<0xFF)
    pinmux_config(PIN_KEY_COL_7,  PINMUX_GPIO_MODE_CFG);
#endif

    // row
#if (PIN_KEY_ROW_0<0xFF)
    pinmux_config(PIN_KEY_ROW_0,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_ROW_1<0xFF)
    pinmux_config(PIN_KEY_ROW_1,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_ROW_2<0xFF)
    pinmux_config(PIN_KEY_ROW_2,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_ROW_3<0xFF)
    pinmux_config(PIN_KEY_ROW_3,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_ROW_4<0xFF)
    pinmux_config(PIN_KEY_ROW_4,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_ROW_5<0xFF)
    pinmux_config(PIN_KEY_ROW_5,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_ROW_6<0xFF)
    pinmux_config(PIN_KEY_ROW_6,  PINMUX_GPIO_MODE_CFG);
#endif
#if (PIN_KEY_ROW_7<0xFF)
    pinmux_config(PIN_KEY_ROW_7,  PINMUX_GPIO_MODE_CFG);
#endif

    pmu_pin_mode_set(key_col_pins_mask, PMU_PIN_MODE_OD);//open drain
    pmu_pin_mode_set(key_row_pins_mask, PMU_PIN_MODE_PU);//pull up
    #if 0 //#ifndef CONFIG_GPIO1_ABSENT
    pmu_pin_mode_set_ex(key_col_pins_ex_mask, PMU_PIN_MODE_OD);//open drain
    pmu_pin_mode_set_ex(key_row_pins_ex_mask, PMU_PIN_MODE_PU);//pull up
    #endif
}

static void kpp_row_wakeup_handler(uint32_t pin_mask)
{
    kpp_sim_gpio_handler(pin_mask);
}

#if 0 //#ifndef CONFIG_GPIO1_ABSENT
static void kpp_row_ex_wakeup_handler(uint32_t pin_mask)
{
    kpp_sim_gpio_ex_handler(pin_mask);
}
#endif
/**
* @brief The system kpp module initialization.
**/
static void key_peripheral_init(void)
{
    const kpp_sim_config_t kpp_cfg =
    {
        key_row_pins_mask,    //ROW enable bitmask (pull-up)
        key_col_pins_mask,    //Column enable bitmask (open-drain)
        KPP_SCAN_TIME,
        KPP_SCAN_INTERVAL,
        keyscan_event_handler,
#if 0 //#ifndef CONFIG_GPIO1_ABSENT
        key_row_pins_ex_mask,    //ROW enable bitmask (pull-up)
        key_col_pins_ex_mask,    //Column enable bitmask (open-drain)
#endif
    };
    
    kpp_sim_config(&kpp_cfg);
    
    gpio_set_interrupt_callback(kpp_row_wakeup_handler);
    
    // wakeup pin set
    pmu_wakeup_pin_set(key_row_pins_mask, PMU_PIN_WAKEUP_LOW_LEVEL);
    pmu_wakeup_pin_register_callback(kpp_row_wakeup_handler, kpp_row_wakeup_handler);

    #if 0 //#ifndef CONFIG_GPIO1_ABSENT
    gpio_set_interrupt_callback_ex(kpp_row_ex_wakeup_handler);
    pmu_wakeup_pin_set_ex(key_row_pins_ex_mask, PMU_PIN_WAKEUP_LOW_LEVEL);
    pmu_wakeup_pin_register_callback_ex(kpp_row_ex_wakeup_handler, NULL);
    #endif
}

void key_init(void)
{
    row_col_init();
    key_pinmux_init();
    key_peripheral_init();
}

/**
* @brief Check the specific key is pressed or not.
**/
static bool key_check(uint32_t col_pin_mask, uint32_t row_pin_mask)
{
    bool key_found = false;
    
    gpio_write(row_pin_mask, GPIO_LOW);
    
    if(!gpio_read(col_pin_mask))
        key_found = true;
    
    gpio_write(row_pin_mask, GPIO_HIGH);
    
    return key_found;
}

/**
* @brief Initialize a matrix keyboard, and check which column key is pressed.
**/
void key_scan(app_key_t *key)
{
#if (APP_CUSTOMER_ID == 2)
    uint32_t row_mask[5] = 
           {BIT_MASK(PIN_KEY_ROW_0), BIT_MASK(PIN_KEY_ROW_1), BIT_MASK(PIN_KEY_ROW_2),
            BIT_MASK(PIN_KEY_ROW_3), BIT_MASK(PIN_KEY_ROW_4) };
    uint32_t col_mask[4] = 
           {BIT_MASK(PIN_KEY_COL_0), BIT_MASK(PIN_KEY_COL_1), BIT_MASK(PIN_KEY_COL_2), 
            BIT_MASK(PIN_KEY_COL_3)};
#else
    uint32_t row_mask[3] = 
           {BIT_MASK(PIN_KEY_ROW_0), BIT_MASK(PIN_KEY_ROW_1), BIT_MASK(PIN_KEY_ROW_2)};
    uint32_t col_mask[6] = 
           {BIT_MASK(PIN_KEY_COL_0), BIT_MASK(PIN_KEY_COL_1), BIT_MASK(PIN_KEY_COL_2), 
            BIT_MASK(PIN_KEY_COL_3)};
#endif    
    key_pinmux_init();
    pmu_pin_mode_set(key_row_pins_mask, PMU_PIN_MODE_PP);
    pmu_pin_mode_set(key_col_pins_mask, PMU_PIN_MODE_PD);
    gpio_set_direction(key_row_pins_mask, GPIO_OUTPUT);
    gpio_set_direction(key_col_pins_mask, GPIO_INPUT);
    gpio_write(key_row_pins_mask, GPIO_HIGH);
    
    for(int i=0; i<KEY_COL_COUNT; i++)
    {
        if(gpio_read(col_mask[i]))
        {
            for(int j=0; j<KEY_ROW_COUNT; j++)
            {
                if(key_check(col_mask[i], row_mask[j]))
                {
                    key->row = j;
                    key->col = i;
                    return;
                }
            }
        }
        else
        {
            key->row = 0;
            key->col = 0;
        }
    }
}

#endif /* CONFIG_KBD_ENABLED */
