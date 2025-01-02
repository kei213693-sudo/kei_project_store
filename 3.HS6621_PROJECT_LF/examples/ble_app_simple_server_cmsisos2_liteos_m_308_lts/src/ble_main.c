/**
 * @file  examples/ble_app_simple_server/src/main.c
 * @brief  simple server
 * @date Wed, Sep  5, 2018  5:19:05 PM
 * @author liqiang
 *
 * @addtogroup APP_SIMPLE_SERVER_MAIN main.c
 * @ingroup APP_SIMPLE_SERVER
 * @details simple server
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "rwip_config.h" // RW SW configuration

#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include "rwip.h"      // RW SW initialization
#include "dbg.h"
#include "ke_event.h"
#include "peripheral.h"
#include "sysdump.h"
#include "app.h"

/* Kernel includes. */
#include "cmsis_os2.h"

/*********************************************************************
 * MACROS
 */

#define EVENT_BLUETOOTH_MASK        0x0001
#define EVENT_SYSTEM_RESERVE_MASK   0x00FF

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */
static osEventFlagsId_t xBluetoothEvent = NULL;


/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */

/**
 * @brief  bluetooth event handler
 **/
static void vBluetoothEventHandler(void)
{
    if(xBluetoothEvent)
        osEventFlagsSet(xBluetoothEvent, EVENT_BLUETOOTH_MASK);
}

/**
 * @brief  bluetooth schedule task
 *
 * @param[in] pvParameters  pv parameters
 **/
static void vBluetoothScheduleTask(void *argument)
{
    uint32_t uxBits;

    // appm init
    appm_init();

    // Create event
    xBluetoothEvent = osEventFlagsNew(NULL);

    // set ke event callback
    ke_event_schedule_trigger_callback_set(vBluetoothEventHandler);

    while (1)
    {
        // schedule
        rwip_schedule_rtos();

        // Wait for event
        uxBits = osEventFlagsWait(xBluetoothEvent, 0xFFFF, osFlagsWaitAny, osWaitForever);
        (void)uxBits;
    }
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 * @brief  v start bluetooth task
 **/
void vStartBluetoothTask(void)
{
    const osThreadAttr_t bluetoothThreadAttr =
    {
        .name = "BT_Task",
        .attr_bits = 0,
        .cb_mem = NULL,
        .cb_size = 0,
        .stack_mem = NULL,
        .stack_size = 2048,
        .priority = osPriorityNormal,
        .tz_module = 0,
    };

    // Create ble Task
    osThreadNew(vBluetoothScheduleTask, NULL, &bluetoothThreadAttr);
}

/** @} */

// vim: fdm=marker
