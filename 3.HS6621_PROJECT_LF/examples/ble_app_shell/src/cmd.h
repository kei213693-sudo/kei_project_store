/**
 ****************************************************************************************
 *
 * @file shell.h
 *
 * @brief Simple CLI shell command header
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_CMD_H_
#define APP_CMD_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Simple CLI shell command header
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

#define SHELL_CMD_USE_ADC      1
#define SHELL_CMD_USE_AUDIO    1
#define SHELL_CMD_USE_DMA      0
/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/*
 * GLOBAL FUNCTIONS DECLARATION
 ****************************************************************************************
 */
void cmd_adc(int argc, char *argv[]);
void cmd_audio(int argc, char *argv[]);
void cmd_dma(int argc, char *argv[]);
void cmd_mem(int argc, char *argv[]);

#define shell_cmd(cmd)  {#cmd, cmd_##cmd},

#define shell_cmd_mem  shell_cmd(mem)

#if (SHELL_CMD_USE_ADC)
#define shell_cmd_adc   shell_cmd(adc)
#else
#define shell_cmd_adc
#endif

#if (SHELL_CMD_USE_AUDIO)
#define shell_cmd_audio   shell_cmd(audio)
#else
#define shell_cmd_audio
#endif

#if (SHELL_CMD_USE_DMA)
#define shell_cmd_dma   shell_cmd(dma)
#else
#define shell_cmd_dma
#endif

#define TEST_HAL_CMD \
        shell_cmd_mem \
        shell_cmd_adc \
        shell_cmd_audio \
        shell_cmd_dma \


#endif //(CONFIG_APP_SHELL)

/// @} APP

#endif // APP_SHELL_H_

