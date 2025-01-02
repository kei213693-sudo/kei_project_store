/**
 * @file example_configuration.c
 * @brief example for Configuration
 * @date 2017/07/13 15:32:13
 * @author liqiang
 *
 * @defgroup EXAMPLE_CONFIGURATION Configuration Example
 * @ingroup EXAMPLE
 * @brief example for Configuration
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "cfg.h"
#include "peripheral.h"

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_configuration(void)
{
    uint8_t buffer[6] = {0};
    uint16_t len;
    int err;

    // Put data to tag 0
    err = cfg_put(CFG_SECTION_USR, 0, buffer, 6);
    if(err >= 0)
        log_debug("cfg put successfully\n");

    // Get data from tag 0
    len = 6;
    err = cfg_get(CFG_SECTION_USR, 0, buffer, &len);
    if(err >= 0)
        log_debug("cfg get successfully, length=%d\n", len);

    // Delete tag 0
    err = cfg_del(CFG_SECTION_USR, 0);
    if(err >= 0)
        log_debug("cfg delete successfully\n");

    while(1);
}

/** @} */

