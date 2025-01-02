/**
 ****************************************************************************************
 *
 * @file shell.h
 *
 * @brief Simple CLI shell header
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_SHELL_H_
#define APP_SHELL_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Simple CLI shell header
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (CONFIG_APP_SHELL)

#include <stdint.h>          // Standard Integer Definition
#include "ke_task.h"         // Kernel Task Definition

/**
 * @brief Shell maximum input line length.
 */
#define SHELL_MAX_LINE_LENGTH       64

/**
 * @brief   Shell maximum arguments per command.
 */
#define SHELL_MAX_ARGUMENTS         8//4


/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */
/**
 * @brief   Command handler function type.
 */
typedef void (*shellcmd_t)(int argc, char *argv[]);

/**
 * @brief   Custom command entry type.
 */
typedef struct {
  const char            *sc_name;           /**< @brief Command name.       */
  shellcmd_t            sc_function;        /**< @brief Command function.   */
} ShellCommand;

/**
 * @brief   Shell descriptor type.
 */
typedef struct {
  HS_UART_Type          *sc_channel;        /**< @brief I/O channel associated
                                                 to the shell.              */
  const ShellCommand    *sc_commands;       /**< @brief Shell extra commands
                                                 table.                     */
} ShellConfig;


/// Shell Application Module Environment Structure
struct app_shell_env_tag
{
    ShellConfig* config;
    uint8_t       rx_fifo[SHELL_MAX_LINE_LENGTH];
    uint16_t      fifo_len;
    uint8_t       rx_buffer[SHELL_MAX_LINE_LENGTH];
    uint16_t      rx_len;
    uint16_t      rx_pos;
};

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/*
 * GLOBAL FUNCTIONS DECLARATION
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Initialize shell Application Module
 ****************************************************************************************
 */
void app_shell_init(const ShellConfig* config);

/**
 ****************************************************************************************
 * @brief recevice the data
 ****************************************************************************************
 */
void app_shell_rx(uint8_t data);

#endif //(CONFIG_APP_SHELL)

/// @} APP

#endif // APP_SHELL_H_

